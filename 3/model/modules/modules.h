#pragma once

#include <chrono>

#include "base_module.h"
#include "../interfaces.h"

class ConnectionModule :
    public Module,
    public IConnection {
private:
    int maxSessions_ = 5;
    std::vector<ConnectionModule*> sessionList_ {};
    std::vector<routeNode> routeList_ {};

public:
    ConnectionModule(int slotsOccupied, int energyConsumption, bool isOn, int range, int maxSessions)
        : Module(slotsOccupied, energyConsumption, isOn, range), maxSessions_(maxSessions) {}

    int getMaxSessions() const {return maxSessions_;}
    void setMaxSessions(int maxSessions) {maxSessions_ = maxSessions;}
    std::vector<ConnectionModule*> getSessionList() const {return sessionList_;}
    std::vector<routeNode> getRouteList() const {return routeList_;}
    
    std::vector<ConnectionModule*> scanForModules(Pair position = {-1, 0}) const;

    bool establishConnection(ConnectionModule* module, bool isResponse = false) override;
    bool closeConnection(ConnectionModule* module, bool isResponse = false) override;
    
    std::vector<routeNode> requestRouteList(ConnectionModule* source) const;
    
    void recursiveRouteNodeImplementation(ConnectionModule* gate, std::vector<routeNode> routeList);
    void recursiveDiscord(ConnectionModule* gate, std::vector<routeNode> targetList);

    bool attachableTo(std::shared_ptr<Platform> host) const override;

    bool connectedToAI(const ConnectionModule* source) const;
    bool isSafeForSystem(Pair position) const;

    void update() override;
    void setUp() override;
};

class SensorModule :
    public Module,
    public ISensor {
private:
    SensorType type_ = SensorType::Optical;
    Report getSurrounding() const override;

public:
    SensorModule(int slotsOccupied, int energyConsumption, bool isOn, int range, SensorType type)
        : Module(slotsOccupied, energyConsumption, isOn, range), type_(type) {}

    SensorType getType() const {return type_;}
    void setType(SensorType type) {type_ = type;}

    std::shared_ptr<Placeholder> getNearestVisibleOpponent() const override;
    std::vector<std::shared_ptr<Placeholder>> getSuspects() const;

    bool attachableTo(std::shared_ptr<Platform> host) const override;

    void update() override {}
    void setUp() override;
};

class WeaponModule :
    public Module,
    public IWeapon {
private:
    std::chrono::steady_clock::time_point chargingStarted_;
    std::chrono::milliseconds chargingDuration_;
    bool isCharging_ = false;
    bool isCharged_ = false;

public:
    WeaponModule(int slotsOccupied, int energyConsumption, bool isOn, int range, std::chrono::milliseconds chargingDuration)
        : Module(slotsOccupied, energyConsumption, isOn, range), chargingDuration_(chargingDuration) {}
    
    std::chrono::milliseconds getChargingDuration() const {return chargingDuration_;}
    void setChargingDuration(std::chrono::milliseconds chargingDuration) {chargingDuration_ = chargingDuration;}
    bool getIsCharging() const {return isCharging_;}
    void setIsCharging(bool isCharging) {isCharging_ = isCharging;}
    bool getIsCharged() const {return isCharged_;}
    void setIsCharged(bool isCharged) {isCharged_ = isCharged;}

    void startCharging();

    bool attack(Pair suspect) override;

    bool attachableTo(std::shared_ptr<Platform> host) const override;

    void update() override;
    void setUp() override {};
};