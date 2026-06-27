#include <iostream>
#include <vector>

class IDisplay {
public:
    virtual void update(int speed) = 0;
    virtual ~IDisplay() = default;
};
class SpeedSensor {
private:
    int currentSpeed = 0;
    std::vector<IDisplay*> displays;
public:
    void attach(IDisplay* display) {
        displays.push_back(display);
    }
    void setSpeed(int speed) {
        currentSpeed = speed;
        for (auto display : displays) {
            display->update(currentSpeed);
        }
    }
};
class SpeedDisplay : public IDisplay {
private:
    int speed = 0;

public:
    void showSpeed() const {
        std::cout << "Current Speed: " << speed << " km/h\n";
    }
    void update(int s) override {
        speed = s;
        showSpeed();
    }
};

class BatteryManager : public IDisplay {
private:
    int previousSpeed = 0;
    double batteryLevel = 88.0;
    const double vehicleMass = 1800.0;

    double currentPowerKw = 0.0;
    double consumedKwh = 0.0;

    void calculatePowerDemand(int currentSpeed) {
        double constantLoad = currentSpeed * 0.15;
        double accelerationLoad = 0.0;
        int speedDelta = currentSpeed - previousSpeed;

        if (speedDelta > 0) {
            double acceleration = speedDelta / 3.6; // m/s^2 (1 saniyelik zaman dilimi için)
            double force = vehicleMass * acceleration;
            accelerationLoad = (force * (currentSpeed / 3.6) / 1000.0) / 0.90;
        }
        currentPowerKw = constantLoad + accelerationLoad;
        std::cout << "Instant Power Demand: " << currentPowerKw << " kW\n";
        previousSpeed = currentSpeed;
    }
    void updateBatteryLevel() {
        consumedKwh = currentPowerKw / 3600.0; // 1 saniyede harcanan enerji
        batteryLevel -= consumedKwh;

        if (batteryLevel < 0) {
            batteryLevel = 0.0;
        }

        std::cout << "Remaining Battery: " << batteryLevel << " kWh\n";
    }
public:
    void update(int speed) override {
        calculatePowerDemand(speed);
        updateBatteryLevel();
    }
};

int main() {
    SpeedSensor carSensor;

    SpeedDisplay dashboardSpeed;
    BatteryManager dashboardBattery;

    carSensor.attach(&dashboardSpeed);
    carSensor.attach(&dashboardBattery);

    std::cout << "--- SCENARIO 1: 0 km/h -> 10 km/h ---\n";
    carSensor.setSpeed(10);

    std::cout << "\n--- SCENARIO 2: 10 km/h -> 40 km/h ---\n";
    carSensor.setSpeed(40);

    return 0;
}
