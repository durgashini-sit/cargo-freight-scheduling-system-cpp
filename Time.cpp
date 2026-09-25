#include "Time.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cctype>



// store time as HHMM internally
Time::Time() : hour(0), minute(0) {}
Time::Time(int h, int m) : hour(h), minute(m) {
    if (h < 0 || h > 23 || m < 0 || m > 59)
        throw std::invalid_argument("Invalid time: hour or minute out of range");
}

// convert time to only mins for easy comparison
int Time::getHour() const { return hour; }
int Time::getMinute() const { return minute; }
int Time::toMinutes() const { return hour * 60 + minute; }

// accept only am/pm format & converts to lowercase
Time Time::fromString(std::string timeStr) {
    timeStr.erase(remove_if(timeStr.begin(), timeStr.end(), ::isspace), timeStr.end());
    transform(timeStr.begin(), timeStr.end(), timeStr.begin(), ::tolower);

    // look for am/pm
    bool isPM = false;
    size_t amPos = timeStr.find("am");
    size_t pmPos = timeStr.find("pm");

    // if am, remove am
    if (amPos != std::string::npos)
        timeStr = timeStr.substr(0, amPos);

    // if pm, remove pm & let isPM=true
    else if (pmPos != std::string::npos) {
        timeStr = timeStr.substr(0, pmPos);
        isPM = true;
    }
    // if there isnt am/pm, time is invalid
    else throw std::invalid_argument("Invalid time format: missing am/pm");

    // find for colon, if there isnt one, time is invalid
    size_t colonPos = timeStr.find(':');
    if (colonPos == std::string::npos)
   throw std::invalid_argument("Invalid time format: missing colon");

    // extract h & m
    int h = std::stoi(timeStr.substr(0, colonPos));
    int m = std::stoi(timeStr.substr(colonPos + 1));

    // convert h to 24hour format
    if (h == 12) h = 0;
    if (isPM) h += 12;

    return Time(h, m);
}

// convert back to am/pm
std::string Time::toString() const {
    int displayHour = hour % 12;
    if (displayHour == 0) displayHour = 12;
    bool isPM = hour >= 12;

    // ensure 2 digits for mins
    // add am/pm
    std::ostringstream oss;
    oss << displayHour << ':' << std::setw(2) << std::setfill('0') << minute
        << (isPM ? "pm" : "am");
    return oss.str();
}
