#include "CarController.h"
#include <iostream>
#include <sstream>

// Constructor: Initializes the CarController with the base URL and sets up CURL.
CarController::CarController(const std::string &base_url) : base_url(base_url) {
    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "CURL initialization failed." << std::endl;
    }
}

// Destructor: Cleans up CURL resources.
CarController::~CarController() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
}

// Sends a drive request to the specified gate number.
void CarController::drive(int gateNumber) {
    if (!curl) return; // If CURL is not initialized, return.

    // Prepare the data to be sent in the POST request.
    std::ostringstream dataStream;
    dataStream << "gate=" << gateNumber;
    std::string data = dataStream.str();
    std::string url = base_url + "/drive";

    // Set CURL options for the request.
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);

    // Prepare a string to hold the response.
    std::string response_string;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

    // Perform the request.
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "CURL failed: " << curl_easy_strerror(res) << std::endl;
    } else {
        std::cout << "Response: " << response_string << std::endl;
    }
}

// Checks if the car is ready by sending a GET request.
bool CarController::isCarReady() {
    if (!curl) return false; // If CURL is not initialized, return false.

    std::string url = base_url + "/ready";

    // Set CURL options for the request.
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);

    // Prepare a string to hold the response.
    std::string response_string;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

    // Perform the request.
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "CURL failed: " << curl_easy_strerror(res) << std::endl;
        return false;
    } else {
        std::cout << "Response: " << response_string << std::endl;
        // Assuming the response is "0: car is not ready" or "1: car is ready"
        if (response_string.find('1') != std::string::npos) {
            return true;
        } else {
            return false;
        }
    }
}

// Callback function for handling data received from CURL requests.
size_t CarController::writeCallback(void *contents, size_t size, size_t nmemb, std::string *str) {
    const size_t totalBytes = size * nmemb;
    str->append(static_cast<char *>(contents), totalBytes);
    return totalBytes;
}
