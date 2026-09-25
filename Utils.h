#pragma once
#include <string>
#include <algorithm>
#include "Time.h"

class Utils {
public:
    // returns true if first char is C/F & rest are digits
    static bool isValidID(const std::string& id, char prefix) {
        if (id.size() < 2 || id[0] != prefix) return false;
        return std::all_of(id.begin() + 1, id.end(), ::isdigit);
    }

    //returns true if time is am/pm
    static bool isValidTime(const std::string& timeStr) {
        try { Time::fromString(timeStr); return true; }
        catch (...) { return false; }
    }

    // remove whitespace
    static std::string trim(const std::string& s) {
        size_t start = s.find_first_not_of(" \t\n\r");
        size_t end = s.find_last_not_of(" \t\n\r");
        return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
    }

    // make strings lowercase, AM becomes am
    // allows accpetance of AM Am aM/PM Pm pM
    static std::string toLower(const std::string& s) {
        std::string res = s;
        std::transform(res.begin(), res.end(), res.begin(), ::tolower);
        return res;
    }
};
