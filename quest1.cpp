#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <ctime>
#include <cmath>
#include <stdexcept> // Include for std::runtime_error

class Line {
public:
    std::string name;

    Line(const std::string& lineName) : name(lineName) {}
};

class Platform {
private:
    int id;
    std::vector<std::time_t> stoppageTimings;
    std::vector<std::time_t> throughTimings;

public:
    Platform(int platformID) : id(platformID) {}

    bool canAccommodateStoppage(std::time_t time) {
        for (const auto& t : stoppageTimings) {
            if (std::fabs(std::difftime(t, time)) < 30 * 60) {
                return false;
            }
        }
        stoppageTimings.push_back(time);
        return true;
    }

    bool canAccommodateThrough(std::time_t time) {
        for (const auto& t : throughTimings) {
            if (std::fabs(std::difftime(t, time)) < 10 * 60) {
                return false;
            }
        }
        throughTimings.push_back(time);
        return true;
    }

    int getID() const {
        return id;
    }
};

class Station {
private:
    std::string idStr;
    int idInt;
    bool isStringID;
    std::vector<std::shared_ptr<Line>> lines;
    std::vector<Platform> platforms;

public:
    Station(const std::string& stationID) : idStr(stationID), isStringID(true) {}
    Station(int stationID) : idInt(stationID), isStringID(false) {}

    void addLine(const std::string& lineName) {
        lines.push_back(std::make_shared<Line>(lineName));
    }

    void addPlatform(int platformID) {
        platforms.emplace_back(platformID);
    }

    bool scheduleStoppage(int platformID, std::time_t time) {
        for (auto& platform : platforms) {
            if (platform.getID() == platformID) {
                if (platform.canAccommodateStoppage(time)) {
                    return true;
                } else {
                    throw std::runtime_error("Time conflict for stoppage at platform " + std::to_string(platformID));
                }
            }
        }
        throw std::invalid_argument("Platform ID " + std::to_string(platformID) + " not found.");
    }

    bool scheduleThrough(int platformID, std::time_t time) {
        for (auto& platform : platforms) {
            if (platform.getID() == platformID) {
                if (platform.canAccommodateThrough(time)) {
                    return true;
                } else {
                    throw std::runtime_error("Time conflict for through train at platform " + std::to_string(platformID));
                }
            }
        }
        throw std::invalid_argument("Platform ID " + std::to_string(platformID) + " not found.");
    }

    void displayID() const {
        if (isStringID) {
            std::cout << "Station ID (String): " << idStr << "\n";
        } else {
            std::cout << "Station ID (Integer): " << idInt << "\n";
        }
    }

    void displayLines() const {
        std::cout << "Lines:\n";
        for (const auto& line : lines) {
            std::cout << "- " << line->name << "\n";
        }
    }
};

std::time_t getTimeInput() {
    int hour, minute;
    std::cout << "Enter time (HH MM): ";
    std::cin >> hour >> minute;

    std::tm timeStruct = {};
    timeStruct.tm_hour = hour;
    timeStruct.tm_min = minute;
    return std::mktime(&timeStruct);
}

int main() {
    try {
        // User input for station ID type
        std::cout << "Enter station ID type (1 for integer, 2 for string): ";
        int idType;
        std::cin >> idType;

        std::shared_ptr<Station> station;

        // Get station ID based on user input
        if (idType == 1) {
            int stationID;
            std::cout << "Enter integer Station ID: ";
            std::cin >> stationID;
            station = std::make_shared<Station>(stationID);
        } else {
            std::string stationID;
            std::cout << "Enter string Station ID: ";
            std::cin >> stationID;
            station = std::make_shared<Station>(stationID);
        }

        // Adding lines to the station
        station->addLine("Blue Line");
        station->addLine("Yellow Line");

        // Adding platforms to the station
        station->addPlatform(1);
        station->addPlatform(2);

        // Display station details
        station->displayID();
        station->displayLines();

        // Schedule a stoppage
        int platformID;
        std::cout << "Enter platform ID for stoppage: ";
        std::cin >> platformID;
        std::time_t stoppageTime = getTimeInput();
        if (station->scheduleStoppage(platformID, stoppageTime)) {
            std::cout << "Stoppage scheduled successfully.\n";
        }

        // Schedule a through train
        std::cout << "Enter platform ID for through train: ";
        std::cin >> platformID;
        std::time_t throughTime = getTimeInput();
        if (station->scheduleThrough(platformID, throughTime)) {
            std::cout << "Through train scheduled successfully.\n";
        }

    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid input: " << e.what() << "\n";
    } catch (const std::runtime_error& e) {
        std::cerr << "Runtime error: " << e.what() << "\n";
    } catch (...) {
        std::cerr << "An unexpected error occurred.\n";
    }

    return 0;
}