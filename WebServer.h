#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

/**
 * @class WebServer
 * @brief Represents a web server that processes requests
 */
class WebServer {
private:
    int serverId;              ///< Unique server identifier
    bool busy;                 ///< Server status: true if processing, false if idle
    Request currentRequest;    ///< Current request being processed
    int timeRemaining;         ///< Time remaining for current request

public:
    /**
     * @brief Constructor
     * @param id Server identifier
     */
    WebServer(int id);

    /**
     * @brief Check if server is idle
     * @return true if idle, false if busy
     */
    bool isIdle() const;

    /**
     * @brief Assign a request to this server
     * @param req Request to process
     */
    void assignRequest(const Request& req);

    /**
     * @brief Process one clock cycle
     * @return true if request completed, false otherwise
     */
    bool processCycle();

    /**
     * @brief Get server ID
     * @return Server identifier
     */
    int getId() const;

    /**
     * @brief Get current request
     * @return Current request being processed
     */
    Request getCurrentRequest() const;
};

#endif
