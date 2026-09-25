#pragma once
#include "Record.h"

class Cargo : public Record {
private:
    int groupSize{ 0 };

public:
    Cargo(const std::string& id,
        const std::string& dest,
        const Time& t,
        int g);

    int  getGroupSize() const { return groupSize; }
    void setGroupSize(int g) { groupSize = g; }

    //ensure Cargo uses own format over Record format
    std::string toString() const override;
};
