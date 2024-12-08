#pragma once

#include <atomic>
#include <mutex>
#include <thread>

#include "../system/ai.h"
#include "../system/environment.h"
#include "../../import/import.h"
#include "../../view/graphics.h"

class Game {
private:
    std::vector<std::thread> gameThreads_;

    AI ai_;
    Environment environment_;
    Graphics graphics_;
    std::chrono::milliseconds updateInterval_;
    bool isRunning_{false};
    
    void initializeField(const GameConfig& config);
public:
    Game(const std::string& gameConfigFile, const std::string& graphicsConfigFile) :
        ai_(&environment_),
        graphics_(graphicsConfigFile),
        isRunning_(false) {
        initializeField(Import::importGameConfig(gameConfigFile));
    }    
    void start();
};