#include "modules.h"

#include <cmath>
#include <thread>
#include <shared_mutex>
#include <mutex>
#include <iostream>

#include "../objects/objects.h"
#include "../system/ai.h"

std::vector<std::weak_ptr<ConnectionModule>> ConnectionModule::scanForModules(Pair pos) const {
    std::vector<std::weak_ptr<ConnectionModule>> result;
    auto host = host_.lock();
    if (!host) return result;
    auto env = host->getEnvironment().lock();
    if (!env) return result;
    auto area = env->getArea(pos, range_);
    for (const auto& [checkPos, token] : area)
        if (auto platform = std::dynamic_pointer_cast<Platform>(token))
            if (platform.get() != host.get())
                if (auto module = platform->findModuleOfType<ConnectionModule>())
                    result.push_back(module);
    return result;
}

bool ConnectionModule::establishConnection(std::weak_ptr<ConnectionModule> target, bool isResponse) {
    auto targetPtr = target.lock();
    if (!targetPtr) return false;
    if ((int)sessionList_.size() < maxSessions_ && std::find_if(routeList_.begin(), routeList_.end(),
    [targetPtr](const routeNode& a) {return a.destination.lock() == targetPtr;}) == routeList_.end()) {
        if (!isResponse)
            if (!targetPtr->establishConnection(shared_from_this(), true))
                return false;
        sessionList_.push_back(target);
        routeList_.push_back(routeNode{target, target});
        std::vector<routeNode> partnersTargets = targetPtr->requestRouteList(shared_from_this());
        for (auto& route : partnersTargets)
            routeList_.push_back(routeNode{target, route.destination});
        for (auto session : sessionList_) {
            auto sessionPtr = session.lock();
            if (!sessionPtr) continue;
            if (sessionPtr != targetPtr) 
                sessionPtr->recursiveRouteNodeImplementation(shared_from_this(), routeList_);
        }
        return true;
    }
    return false;
}

bool ConnectionModule::closeConnection(std::weak_ptr<ConnectionModule> target, bool isResponse) {
    auto targetPtr = target.lock();
    if (!targetPtr) return false;
    if (!isResponse)
        targetPtr->closeConnection(shared_from_this(), true);
    sessionList_.erase(std::remove_if(sessionList_.begin(), sessionList_.end(),
        [&targetPtr](std::weak_ptr<ConnectionModule>& session) { return session.lock() == targetPtr; }),
        sessionList_.end());

    routeList_.erase(std::remove_if(routeList_.begin(), routeList_.end(),
        [targetPtr](const routeNode& node) { return node.destination.lock() == targetPtr; }),
        routeList_.end());
    
    targetPtr->recursiveDiscord(shared_from_this(), routeList_);
    return true;
}

std::vector<routeNode> ConnectionModule::requestRouteList(std::weak_ptr<ConnectionModule> source) const {
    std::vector<routeNode> routeList {};
    auto sourcePtr = source.lock();
    if (!sourcePtr) return routeList;
    for (auto node : routeList_) {
        if (node.destination.lock() != sourcePtr)
            routeList.push_back(node);
    }
    return routeList;
}

void ConnectionModule::recursiveRouteNodeImplementation(std::weak_ptr<ConnectionModule> gate, std::vector<routeNode> routeList) {
    bool isEntered = false;
    for (auto& node : routeList)
        if (std::find_if(routeList_.begin(), routeList_.end(),
        [&node](const routeNode& a) {return a == node;}) == routeList_.end() && node.destination.lock() != shared_from_this()) {
            isEntered = true;
            routeList_.push_back(routeNode{gate, node.destination});
        }
    if (isEntered) {
        for (auto session : sessionList_)
            if (session.lock() != gate.lock())
                session.lock()->recursiveRouteNodeImplementation(gate, routeList);
    }
}

void ConnectionModule::recursiveDiscord(std::weak_ptr<ConnectionModule> gate, std::vector<routeNode> targetList) {
    bool isEntered = false;
    for (auto node : targetList)
        if (auto it = std::find(routeList_.begin(), routeList_.end(), routeNode{gate, node.destination}); it != routeList_.end()) {
            isEntered = true;
            routeList_.erase(it);
        }
    if (isEntered)
        for (auto module : sessionList_)
            module.lock()->recursiveDiscord(gate, targetList);
}

bool ConnectionModule::isGateToAI(std::weak_ptr<ConnectionModule> gate) const {
    auto aiDest = getConnectedToAIDirectly().lock();
    for (auto node : routeList_) {
        if (node.destination.lock() == aiDest)
            return node.gate.lock() == gate.lock();
    }
    return false;
}

bool ConnectionModule::isSafeForSystem(Pair newPosition) const {
    auto host = host_.lock();
    if (!host) return false;
    auto staticPlatform = getConnectedToAIDirectly().lock();
    if (!staticPlatform) return false;
    for (const auto& session : sessionList_) {
        auto sessionPtr = session.lock();
        if (!sessionPtr) continue;
        auto distance = host->getEnvironment().lock()->calculateDistance(newPosition, sessionPtr->getHost()->getPosition());
        if ((distance > range_ || distance > sessionPtr->getRange()) && (sessionPtr->isGateToAI(host->findModuleOfType<ConnectionModule>()) || isGateToAI(session)))
            return false;
    }
    return true;
}

std::weak_ptr<const ConnectionModule> ConnectionModule::getConnectedToAIDirectly() const {
    if (auto staticPlatform = dynamic_cast<StaticPlatform*>(host_.lock().get()); staticPlatform)
        return weak_from_this();
    for (auto node : routeList_)
        if (auto staticPlatform = dynamic_cast<StaticPlatform*>(node.destination.lock()->getHost().get()); staticPlatform)
            return node.destination;
    return {};
}

void ConnectionModule::update() {
    auto host = host_.lock();
    if (!host) return;
    auto environment = host->getEnvironment().lock();
    if (!environment) return;

    auto newNeighbors = scanForModules(host->getPosition());

    for (const auto& neighborWeak : newNeighbors) {
        auto neighbor = neighborWeak.lock();
        if (!neighbor) continue;
        if (std::find_if(sessionList_.begin(), sessionList_.end(),
        [&neighbor](const std::weak_ptr<ConnectionModule>& session) {return session.lock() == neighbor;}) == sessionList_.end()) {
            if (establishConnection(neighbor)) {
                auto ai = getConnectedToAIDirectly().lock()->getHost()->getAI().lock();
                if (ai)
                    ai->addConnectedPlatform(neighbor);
            }
        }
    }
    auto it = sessionList_.begin();
    while (it != sessionList_.end()) {
        const auto session = *it;
        if (!session.lock() || std::find_if(newNeighbors.begin(), newNeighbors.end(),
        [&session](const std::weak_ptr<ConnectionModule>& neighbor) {return neighbor.lock() == session.lock();}) == newNeighbors.end())
            it = sessionList_.erase(it);
        else
            ++it;
    }
}

void ConnectionModule::turnOn() {
    setIsOn(true);
    host_.lock()->setEnergyLevel(host_.lock()->getEnergyLevel() + energyConsumption_);
}

void ConnectionModule::turnOff() {
    setIsOn(false);
    host_.lock()->setEnergyLevel(host_.lock()->getEnergyLevel() - energyConsumption_);
}