#include "CarController.h"
#include <iostream>
#include <sstream>

CarController::CarController(const std::string& base_url) : base_url(base_url) {
    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "CURL initialization failed." << std::endl;
    }
}

CarController::~CarController() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
}

void CarController::sendDriveCommand(int gateNumber) {
    if (!curl) return;

    std::ostringstream dataStream;
    dataStream << "gate=" << gateNumber;
    std::string data = dataStream.str();
    std::string url = base_url + "/drive";

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);

    std::string response_string;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "CURL failed: " << curl_easy_strerror(res) << std::endl;
    } else {
        std::cout << "Response: " << response_string << std::endl;
    }
}

void CarController::isCarReady() {
    if (!curl) return;

    std::string url = base_url + "/ready";

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);

    std::string response_string;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "CURL failed: " << curl_easy_strerror(res) << std::endl;
    } else {
        std::cout << "Response: " << response_string << std::endl;
    }
}

size_t CarController::writeCallback(void* contents, size_t size, size_t nmemb, std::string* str) {
    const size_t totalBytes = size * nmemb;
    str->append(static_cast<char*>(contents), totalBytes);
    return totalBytes;
}