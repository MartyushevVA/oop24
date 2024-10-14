#pragma once

#include "platform.h"

class StaticPlatform : public Platform {
public:
    StaticPlatform(int x, int y, const std::string& description, int energyLevel, int slotCount);
    virtual ~StaticPlatform() = default;
    
    void move(int dx, int dy) override;
};