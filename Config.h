#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

/**
 * @class Config
 * @brief Configuration settings for load balancer simulation
 */
class Config {
public:
    int minTaskTime;                      ///< Minimum task processing time
    int maxTaskTime;                      ///< Maximum task processing time
    int waitCycles;                       ///< Cycles to wait between server changes
    int requestGenerationChance;          ///< Percentage chance to generate request per cycle
    std::vector<std::string> blockedIPRanges;  ///< Blocked IP ranges for firewall

    /**
     * @brief Constructor with defaults
     */
    Config();

    /**
     * @brief Load configuration from file
     * @param filename Configuration file path
     * @return true if loaded successfully, false otherwise
     */
    bool loadFromFile(const std::string& filename);

    /**
     * @brief Check if an IP is blocked
     * @param ip IP address to check
     * @return true if blocked, false otherwise
     */
    bool isIPBlocked(const std::string& ip) const;
};

#endif
