#pragma once

#include "SchedulingStrategy.h"

class CapacitySortStrategy : public SchedulingStrategy {
public:
    CapacitySortStrategy() = default;

    void generate(
        const std::map<std::string, std::vector<Cargo>>& groupedCargos,FreightList& freightList,ScheduleList& scheduleList) override;
};

