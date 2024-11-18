#pragma once

#include <memory>
#include <vector>

#include "platform.h"
#include "environment.h"

class AI {
private:
    std::vector<std::shared_ptr<Platform>> connectedPlatforms_ {};
    Environment* environment_ = nullptr;
public:
    AI() = default;
    AI(Environment* environment) : environment_(environment) {};

    Environment* getEnvironment() const {return environment_;}
    void setEnvironment(Environment* environment) {environment_ = environment;}
    void addConnectedPlatform(std::shared_ptr<Platform> platform) {connectedPlatforms_.push_back(platform);}
    void refreshConnectedPlatforms();
    void eliminateAllIntruders();
};



/*на каждом шаге аи чекает подключенные платформы на наличие сенсоров
и сканирует окружение с них если есть чем.
в программе платформа всегда знает свое положение.
*/