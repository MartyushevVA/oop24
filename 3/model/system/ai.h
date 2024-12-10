#pragma once

#include <vector>
#include <map>
#include <memory>

#include "../common_types.h"

class Environment;
class Platform;
class StaticPlatform;
class MobilePlatform;
class Suspect;
class Placeholder;

class AI : public std::enable_shared_from_this<AI> {
private:
    std::weak_ptr<Environment> environment_;
    std::vector<std::shared_ptr<StaticPlatform>> staticPlatforms_ {};
    std::vector<Platform*> allConnectedPlatforms_ {};
    std::map<Pair, std::shared_ptr<Placeholder>> spottedSuspects_ {};  

public:
    AI(std::shared_ptr<Environment> environment) : environment_(environment) {};

    void addStaticPlatform(std::shared_ptr<StaticPlatform> platform) {
        if (platform) {
            staticPlatforms_.push_back(platform);
            platform->setAI(getSharedPtr());
        }
    }
    void eliminateAllSuspects();
    void getNetworkForest();
    const std::vector<Platform*>& getAllConnectedPlatforms() const {return allConnectedPlatforms_;}
    
    void addSuspects(std::map<Pair, std::shared_ptr<Placeholder>> suspects);
    void removeSuspect(std::shared_ptr<Placeholder> suspect);
    const std::map<Pair, std::shared_ptr<Placeholder>>& getSuspects() const {return spottedSuspects_;}

    std::shared_ptr<AI> getSharedPtr() { return shared_from_this(); }
};