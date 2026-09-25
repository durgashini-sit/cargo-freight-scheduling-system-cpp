#pragma once
#include <string>
#include "Freight.h"
#include "Time.h"

class FreightFactory {
public:
    static Freight createFreight(const std::string& id,
        const std::string& dest,
        const Time& departure,
        const std::string& label);
};
