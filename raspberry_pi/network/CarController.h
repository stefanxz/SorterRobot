#ifndef CARCONTROLLER_H
#define CARCONTROLLER_H

#include <string>
#include <curl/curl.h>

// CarController class manages communication with the car control server.
class CarController {
public:
    // Constructor: Initializes the CarController with the base URL.
    CarController(const std::string& base_url);

    // Destructor: Cleans up CURL resources.
    ~CarController();

    // Sends a drive request to the specified gate number.
    void drive(int gateNumber);

    // Checks if the car is ready by sending a GET request.
    bool isCarReady();

private:
    CURL* curl;               // CURL handle for making HTTP requests.
    std::string base_url;     // Base URL for the car control server.

    // Callback function for handling data received from CURL requests.
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* str);
};

#endif // CARCONTROLLER_H
