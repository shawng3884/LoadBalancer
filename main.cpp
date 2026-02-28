#include <iostream>
#include <cstdlib>
#include <ctime>
#include "LoadBalancer.h"

/**
 * @brief Main driver program for load balancer simulation
 */
int main() {
    srand(time(0));

    int numServers, totalTime;
    
    std::cout << "Load Balancer Simulation" << std::endl;
    std::cout << "Enter number of servers: ";
    std::cin >> numServers;
    
    std::cout << "Enter simulation time (clock cycles): ";
    std::cin >> totalTime;

    // Create load balancer with initial servers
    LoadBalancer lb(numServers, 10, 100, 5);
    
    // Generate initial queue (servers * 100)
    int initialQueueSize = numServers * 100;
    lb.generateInitialQueue(initialQueueSize);
    
    // Open log file
    lb.openLog("loadbalancer.log");
    
    std::cout << "Starting simulation with " << numServers << " servers and " 
              << initialQueueSize << " initial requests..." << std::endl;

    // Main simulation loop
    for (int cycle = 0; cycle < totalTime; cycle++) {
        // Randomly add new requests (15% chance per cycle)
        if (rand() % 100 < 15) {
            char jobType = (rand() % 2 == 0) ? 'P' : 'S';
            int time = 10 + (rand() % 91);
            Request newReq(Request::generateRandomIP(), Request::generateRandomIP(), time, jobType);
            lb.addRequest(newReq);
        }
        
        lb.processCycle();
    }

    // Write summary
    lb.writeSummary();
    
    std::cout << "Simulation complete!" << std::endl;
    std::cout << "Queue remaining: " << lb.getQueueSize() << std::endl;
    std::cout << "Servers active: " << lb.getServerCount() << std::endl;
    std::cout << "Completed requests: " << lb.getCompletedRequests() << std::endl;

    return 0;
}
