#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include "WebServer.h"
#include "Request.h"
#include <queue>
#include <vector>
#include <string>
#include <fstream>

/**
 * @class LoadBalancer
 * @brief Manages web servers and distributes requests
 */
class LoadBalancer {
private:
    std::queue<Request> requestQueue;     ///< Queue of pending requests
    std::vector<WebServer*> servers;      ///< Active web servers
    int currentCycle;                     ///< Current clock cycle
    int totalRequests;                    ///< Total requests processed
    int completedRequests;                ///< Completed requests count
    int rejectedRequests;                 ///< Rejected requests count
    std::ofstream logFile;                ///< Log file stream
    int minTaskTime;                      ///< Minimum task time
    int maxTaskTime;                      ///< Maximum task time
    int waitCycles;                       ///< Cycles to wait after server change
    int cyclesSinceChange;                ///< Cycles since last server change

public:
    /**
     * @brief Constructor
     * @param initialServers Number of servers to start with
     * @param minTime Minimum task time
     * @param maxTime Maximum task time
     * @param waitCycles Cycles to wait between server changes
     */
    LoadBalancer(int initialServers, int minTime, int maxTime, int waitCycles);

    /**
     * @brief Destructor
     */
    ~LoadBalancer();

    /**
     * @brief Add a request to the queue
     * @param req Request to add
     */
    void addRequest(const Request& req);

    /**
     * @brief Process one clock cycle
     */
    void processCycle();

    /**
     * @brief Add a new server
     */
    void addServer();

    /**
     * @brief Remove a server
     */
    void removeServer();

    /**
     * @brief Get current queue size
     * @return Number of requests in queue
     */
    int getQueueSize() const;

    /**
     * @brief Get number of active servers
     * @return Number of servers
     */
    int getServerCount() const;

    /**
     * @brief Generate initial queue
     * @param count Number of requests to generate
     */
    void generateInitialQueue(int count);

    /**
     * @brief Open log file
     * @param filename Log file name
     */
    void openLog(const std::string& filename);

    /**
     * @brief Write summary to log
     */
    void writeSummary();

    /**
     * @brief Get completed requests count
     * @return Number of completed requests
     */
    int getCompletedRequests() const;

    /**
     * @brief Get rejected requests count
     * @return Number of rejected requests
     */
    int getRejectedRequests() const;
};

#endif
