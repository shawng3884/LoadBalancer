#include <iostream>
#include <cstdlib>
#include <ctime>
#include "LoadBalancer.h"
#include "Config.h"
#include "Colors.h"

/**
 * @brief Main driver program for load balancer simulation
 */
int main() {
    srand(time(0));

    // Load configuration
    Config config;
    config.loadFromFile("loadbalancer.config");

    int numServers, totalTime;
    
    std::cout << Colors::BOLD << Colors::CYAN << "Load Balancer Simulation" << Colors::RESET << std::endl;
    std::cout << "Enter number of servers: ";
    std::cin >> numServers;
    
    std::cout << "Enter simulation time (clock cycles): ";
    std::cin >> totalTime;

    // Create load balancer with configuration
    LoadBalancer lb(numServers, config.minTaskTime, config.maxTaskTime, config.waitCycles);
    
    // Add blocked IP ranges from config
    for (const auto& range : config.blockedIPRanges) {
        lb.addBlockedIPRange(range);
        std::cout << Colors::RED << "Blocking IP range: " << range << Colors::RESET << std::endl;
    }
    
    // Generate initial queue (servers * 100)
    int initialQueueSize = numServers * 100;
    lb.generateInitialQueue(initialQueueSize);
    
    // Open log file
    lb.openLog("loadbalancer.log");
    
    std::cout << Colors::GREEN << "Starting simulation with " << numServers << " servers and " 
              << initialQueueSize << " initial requests..." << Colors::RESET << std::endl;

    // Main simulation loop
    int newRequestsGenerated = 0;
    for (int cycle = 0; cycle < totalTime; cycle++) {
        // Randomly add new requests
        if (rand() % 100 < config.requestGenerationChance) {
            char jobType = (rand() % 2 == 0) ? 'P' : 'S';
            int time = config.minTaskTime + (rand() % (config.maxTaskTime - config.minTaskTime + 1));
            Request newReq(Request::generateRandomIP(), Request::generateRandomIP(), time, jobType);
            lb.addRequest(newReq);
            newRequestsGenerated++;
        }
        
        lb.processCycle();
        
        // Show progress every 1000 cycles
        if (cycle % 1000 == 0 && cycle > 0) {
            std::cout << Colors::YELLOW << "[Cycle " << cycle << "] Queue: " << lb.getQueueSize() 
                      << ", Servers: " << lb.getServerCount() 
                      << ", New requests: " << newRequestsGenerated << Colors::RESET << std::endl;
            newRequestsGenerated = 0;
        }
    }

    // Write summary
    lb.writeSummary();
    
    std::cout << Colors::BOLD << Colors::GREEN << "\nSimulation complete!" << Colors::RESET << std::endl;
    std::cout << Colors::CYAN << "Queue remaining: " << Colors::RESET << lb.getQueueSize() << std::endl;
    std::cout << Colors::CYAN << "Servers active: " << Colors::RESET << lb.getServerCount() << std::endl;
    std::cout << Colors::CYAN << "Completed requests: " << Colors::RESET << lb.getCompletedRequests() << std::endl;
    std::cout << Colors::RED << "Rejected requests: " << Colors::RESET << lb.getRejectedRequests() << std::endl;

    return 0;
}
