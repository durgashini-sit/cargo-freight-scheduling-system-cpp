#pragma once
#include <string>
#include "Record.h"
#include "Time.h"

class Freight : public Record {
private:
    std::string label_;
    int capacity_;   // just stored, not computed here

public:
    Freight(const std::string& id, const std::string& dest,
        const Time& t, const std::string& label, int capacity);


    const std::string& getLabel() const { return label_; }
    int getCapacity() const { return capacity_; }

    //ensure Freight uses own format over Record format
    std::string toString() const override; 
};
