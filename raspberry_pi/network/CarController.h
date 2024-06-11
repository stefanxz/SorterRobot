#ifndef CARCONTROLLER_H
#define CARCONTROLLER_H

#include <string>
#include <curl/curl.h>

class CarController {
public:
    CarController(const std::string& base_url);
    ~CarController();

    void drive(int gateNumber);
    bool isCarReady();

private:
    CURL* curl;
    std::string base_url;

    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* str);
};

#endif // CARCONTROLLER_H