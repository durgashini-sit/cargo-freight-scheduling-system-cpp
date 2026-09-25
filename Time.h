#pragma once
#include <string>

class Time {
private:
    int hour;
    int minute;

public:
    Time();
    Time(int h, int m);

    int  getHour() const;
    int  getMinute() const;
    int  toMinutes() const;

    static Time fromString(std::string timeStr);
    std::string toString() const;
};
