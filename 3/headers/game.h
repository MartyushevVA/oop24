#pragma once

#include "environment.h"
#include "platform.h"
#include "module_types.h"
#include "module.h"
#include "ai.h"
#include "mobile_platform.h"
#include "static_platform.h"
#include "intruder.h"
#include <vector>
#include <memory>
#include <string>

class Game {
private:
    Environment environment_;
    AI ai_;
    std::vector<std::unique_ptr<Module>> storage_;

public:
    Game(Environment* environment) : ai_(environment) {}

    void loadFieldFromFile(const std::string& filename, Environment& environment);
    void addToStorage(std::unique_ptr<Module> module) {storage_.push_back(std::move(module));}
    void start();
    void stop();
};