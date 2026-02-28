#ifndef REQUEST_H
#define REQUEST_H

#include <string>

/**
 * @class Request
 * @brief Represents a web request with IP addresses, processing time, and job type
 */
class Request {
public:
    std::string ipIn;      ///< Incoming IP address
    std::string ipOut;     ///< Outgoing IP address
    int time;              ///< Processing time in clock cycles
    char jobType;          ///< Job type: 'P' (Processing) or 'S' (Streaming)

    /**
     * @brief Default constructor
     */
    Request();

    /**
     * @brief Parameterized constructor
     * @param ipIn Incoming IP address
     * @param ipOut Outgoing IP address
     * @param time Processing time
     * @param jobType Type of job ('P' or 'S')
     */
    Request(std::string ipIn, std::string ipOut, int time, char jobType);

    /**
     * @brief Generate a random IP address
     * @return Random IP address string
     */
    static std::string generateRandomIP();
};

#endif
