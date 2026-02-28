#include "WebServer.h"

WebServer::WebServer(int id) : serverId(id), busy(false), timeRemaining(0) {}

bool WebServer::isIdle() const {
    return !busy;
}

void WebServer::assignRequest(const Request& req) {
    currentRequest = req;
    timeRemaining = req.time;
    busy = true;
}

bool WebServer::processCycle() {
    if (!busy) return false;
    
    timeRemaining--;
    if (timeRemaining <= 0) {
        busy = false;
        return true;
    }
    return false;
}

int WebServer::getId() const {
    return serverId;
}

Request WebServer::getCurrentRequest() const {
    return currentRequest;
}
