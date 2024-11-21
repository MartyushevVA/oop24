#pragma once

#include <random>

#include "interfaces.h"

class Environment;

class Intruder : 
    public Placeholder,
    public IAvoidanceMovement,
    public ISensor {
private:
    int sensorRange_ = 0;

public:
    Intruder(Pair position, Environment* environment, int speed, int sensorRange)
        : Placeholder(position, environment, speed), sensorRange_(sensorRange) {}

    void setSpeed(int speed) override {
        if (speed <= 0)
            throw std::invalid_argument("Speed must be positive");
        speed_ = speed;
    }

    MobilePlatform* nearestPredatorWithinRange() const;

    Pair calculateAvoidanceMove(Pair threat) const override;
    Report getSurrounding() const override;
};