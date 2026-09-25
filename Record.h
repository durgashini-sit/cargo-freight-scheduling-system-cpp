#pragma once
#include <string>
#include "Time.h"

class Record {
protected:
    std::string id_;
    std::string destination_;
    Time        time_;

public:
    Record(const std::string& id, const std::string& dest, const Time& t)
        : id_(id), destination_(dest), time_(t) {
    }

    virtual ~Record() = default;

    const std::string& getId() const { return id_; }
    const std::string& getDestination() const { return destination_; }
    const Time& getTime() const { return time_; }

    void setId(const std::string& v) { id_ = v; }
    void setDestination(const std::string& v) { destination_ = v; }
    void setTime(const Time& t) { time_ = t; }

    virtual std::string toString() const = 0;
};
