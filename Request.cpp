#include "Request.h"
#include <cstdlib>
#include <sstream>

Request::Request() : ipIn(""), ipOut(""), time(0), jobType('P') {}

Request::Request(std::string ipIn, std::string ipOut, int time, char jobType)
    : ipIn(ipIn), ipOut(ipOut), time(time), jobType(jobType) {}

std::string Request::generateRandomIP() {
    std::stringstream ss;
    ss << (rand() % 256) << "."
       << (rand() % 256) << "."
       << (rand() % 256) << "."
       << (rand() % 256);
    return ss.str();
}
