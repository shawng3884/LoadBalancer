#include "LoadBalancer.h"
#include "Colors.h"
#include <iostream>
#include <cstdlib>

LoadBalancer::LoadBalancer(int initialServers, int minTime, int maxTime, int waitCycles)
    : currentCycle(0), totalRequests(0), completedRequests(0), rejectedRequests(0),
      minTaskTime(minTime), maxTaskTime(maxTime), waitCycles(waitCycles), cyclesSinceChange(0) {
    for (int i = 0; i < initialServers; i++) {
        servers.push_back(new WebServer(i));
    }
}

LoadBalancer::~LoadBalancer() {
    for (auto server : servers) {
        delete server;
    }
    if (logFile.is_open()) {
        logFile.close();
    }
}

void LoadBalancer::addRequest(const Request& req) {
    // Check if IP is blocked
    if (isIPBlocked(req.ipIn)) {
        rejectedRequests++;
        if (logFile.is_open() && rejectedRequests <= 20) {
            logFile << "[Cycle " << currentCycle << "] BLOCKED request from " << req.ipIn 
                    << " (Type: " << req.jobType << ")" << std::endl;
        }
        return;
    }
    
    requestQueue.push(req);
    totalRequests++;
    
    // Log every 100th request added
    if (logFile.is_open() && totalRequests % 100 == 0) {
        logFile << "[Cycle " << currentCycle << "] New request added to queue (Type: " 
                << req.jobType << ", Time: " << req.time << ", From: " << req.ipIn 
                << ") - Total: " << totalRequests << std::endl;
    }
}

void LoadBalancer::processCycle() {
    currentCycle++;
    cyclesSinceChange++;

    int assignedThisCycle = 0;

    // Assign requests to idle servers
    for (auto server : servers) {
        if (server->isIdle() && !requestQueue.empty()) {
            Request req = requestQueue.front();
            requestQueue.pop();
            server->assignRequest(req);
            assignedThisCycle++;
            
            // Log some assignments for demonstration
            if (logFile.is_open() && assignedThisCycle <= 3 && currentCycle % 1000 == 0) {
                logFile << "[Cycle " << currentCycle << "] Server " << server->getId() 
                        << " assigned request (Type: " << req.jobType << ", Time: " << req.time 
                        << ", From: " << req.ipIn << ")" << std::endl;
            }
        }
    }

    // Process one cycle on all servers
    for (auto server : servers) {
        if (server->processCycle()) {
            completedRequests++;
        }
    }

    // Periodic status logging with counts
    if (logFile.is_open() && currentCycle % 1000 == 0) {
        logFile << "[Cycle " << currentCycle << "] Status - Queue: " << requestQueue.size() 
                << ", Servers: " << servers.size() 
                << ", Total Completed: " << completedRequests << std::endl;
    }

    // Dynamic server management
    int queueSize = requestQueue.size();
    int serverCount = servers.size();

    if (cyclesSinceChange >= waitCycles) {
        if (queueSize > 80 * serverCount) {
            addServer();
            cyclesSinceChange = 0;
        } else if (queueSize < 50 * serverCount && serverCount > 1) {
            removeServer();
            cyclesSinceChange = 0;
        }
    }
}

void LoadBalancer::addServer() {
    int newId = servers.size();
    servers.push_back(new WebServer(newId));
    if (logFile.is_open()) {
        logFile << "[Cycle " << currentCycle << "] Added server " << newId 
                << " (Total: " << servers.size() << ")" << std::endl;
    }
    std::cout << Colors::MAGENTA << "[Cycle " << currentCycle << "] Added server " << newId 
              << " (Total: " << servers.size() << ")" << Colors::RESET << std::endl;
}

void LoadBalancer::removeServer() {
    if (servers.size() > 1) {
        for (auto it = servers.begin(); it != servers.end(); ++it) {
            if ((*it)->isIdle()) {
                int removedId = (*it)->getId();
                delete *it;
                servers.erase(it);
                if (logFile.is_open()) {
                    logFile << "[Cycle " << currentCycle << "] Removed server " << removedId 
                            << " (Total: " << servers.size() << ")" << std::endl;
                }
                std::cout << Colors::RED << "[Cycle " << currentCycle << "] Removed server " << removedId 
                          << " (Total: " << servers.size() << ")" << Colors::RESET << std::endl;
                break;
            }
        }
    }
}

int LoadBalancer::getQueueSize() const {
    return requestQueue.size();
}

int LoadBalancer::getServerCount() const {
    return servers.size();
}

void LoadBalancer::generateInitialQueue(int count) {
    for (int i = 0; i < count; i++) {
        char jobType = (rand() % 2 == 0) ? 'P' : 'S';
        int time = minTaskTime + (rand() % (maxTaskTime - minTaskTime + 1));
        Request req(Request::generateRandomIP(), Request::generateRandomIP(), time, jobType);
        addRequest(req);
    }
}

void LoadBalancer::openLog(const std::string& filename) {
    logFile.open(filename);
    if (logFile.is_open()) {
        logFile << "=== Load Balancer Simulation Log ===" << std::endl;
        logFile << "Task Time Range: " << minTaskTime << " - " << maxTaskTime << " cycles" << std::endl;
        logFile << "Starting Queue Size: " << requestQueue.size() << std::endl;
        logFile << "Initial Servers: " << servers.size() << std::endl;
        
        // Log blocked IP ranges
        if (!blockedIPRanges.empty()) {
            logFile << "Blocked IP Ranges: ";
            for (const auto& range : blockedIPRanges) {
                logFile << range << " ";
            }
            logFile << std::endl;
        }
        
        logFile << "\n--- Simulation Events ---" << std::endl;
    }
}

void LoadBalancer::writeSummary() {
    if (logFile.is_open()) {
        logFile << "\n=== Simulation Summary ===" << std::endl;
        logFile << "Total Cycles: " << currentCycle << std::endl;
        logFile << "Task Time Range: " << minTaskTime << " - " << maxTaskTime << " cycles" << std::endl;
        logFile << "Remaining Queue Size: " << requestQueue.size() << std::endl;
        logFile << "Active Servers: " << servers.size() << std::endl;
        
        // Count idle vs busy servers
        int idleServers = 0;
        int busyServers = 0;
        for (auto server : servers) {
            if (server->isIdle()) {
                idleServers++;
            } else {
                busyServers++;
            }
        }
        logFile << "Idle Servers: " << idleServers << std::endl;
        logFile << "Busy Servers: " << busyServers << std::endl;
        
        logFile << "Total Requests Generated: " << totalRequests << std::endl;
        logFile << "Completed Requests: " << completedRequests << std::endl;
        logFile << "Rejected Requests: " << rejectedRequests << std::endl;
        logFile << "Average Requests per Cycle: " << (double)totalRequests / currentCycle << std::endl;
        logFile << "Completion Rate: " << (double)completedRequests / totalRequests * 100 << "%" << std::endl;
    }
}

int LoadBalancer::getCompletedRequests() const {
    return completedRequests;
}

int LoadBalancer::getRejectedRequests() const {
    return rejectedRequests;
}

void LoadBalancer::addBlockedIPRange(const std::string& ipRange) {
    blockedIPRanges.insert(ipRange);
}

bool LoadBalancer::isIPBlocked(const std::string& ip) const {
    for (const auto& range : blockedIPRanges) {
        if (ip.find(range) == 0) {
            return true;
        }
    }
    return false;
}
