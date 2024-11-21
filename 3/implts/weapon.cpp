#include "module_types.h"

void WeaponModule::startCharging() {
    if (!host_ || !isOn_ || isCharging_ || isCharged_)
        return;
    if (host_->getMaxEnergyLevel() < host_->getEnergyLevel() + energyConsumption_)
        return;
    isCharging_ = true;
    chargingStarted_ = std::chrono::steady_clock::now();
    host_->setEnergyLevel(host_->getEnergyLevel() + energyConsumption_);
}

void WeaponModule::refreshState() {
    if (isCharged_) return;
    if (!isCharging_) startCharging();
    else {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - chargingStarted_);
        if (elapsedTime >= chargingDuration_) {
            isCharged_ = true;
            isCharging_ = false;
            host_->setEnergyLevel(host_->getEnergyLevel() - energyConsumption_);
        }
    }
}

Pair WeaponModule::findAttackableIntruder(Report report) const {
    for (const auto& token : report.objects) {
        if (Intruder* intruder = dynamic_cast<Intruder*>(token.get()))
            if (host_->getEnvironment()->hasLineOfSight(host_->getPosition(), intruder->getPosition()) && (host_->getEnvironment()->howFar(host_->getPosition(), intruder->getPosition(), range_) <= 1))
                return intruder->getPosition();
    }
    return {-1, 0};
}

void WeaponModule::attack(Pair intruder) {
    if (!host_ || !isOn_ || !host_->getEnvironment()->hasLineOfSight(host_->getPosition(), intruder)) {
        return;
    }
    if (isCharged_) {
        host_->getEnvironment()->extractToken(intruder);
        isCharged_ = false;
    }
}

void WeaponModule::attachTo(Platform* host) const {
    host->installModule(std::make_unique<WeaponModule>(*this));
}