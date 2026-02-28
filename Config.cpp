#include "Config.h"
#include <fstream>
#include <sstream>
#include <iostream>

Config::Config() 
    : minTaskTime(10), maxTaskTime(100), waitCycles(5), requestGenerationChance(15) {}

bool Config::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open config file. Using defaults." << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            if (key == "minTaskTime") {
                minTaskTime = std::stoi(value);
            } else if (key == "maxTaskTime") {
                maxTaskTime = std::stoi(value);
            } else if (key == "waitCycles") {
                waitCycles = std::stoi(value);
            } else if (key == "requestGenerationChance") {
                requestGenerationChance = std::stoi(value);
            } else if (key == "blockedIPRange") {
                blockedIPRanges.push_back(value);
            }
        }
    }

    file.close();
    return true;
}

bool Config::isIPBlocked(const std::string& ip) const {
    for (const auto& range : blockedIPRanges) {
        // Simple prefix matching for IP ranges
        if (ip.find(range) == 0) {
            return true;
        }
    }
    return false;
}
