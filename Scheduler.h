#pragma once
#include <map>
#include <vector>
#include <string>

#include "CargoList.h"
#include "FreightList.h"
#include "ScheduleList.h"
#include "ArrivalSortStrategy.h"
#include "CapacitySortStrategy.h"

class Scheduler {
public:
    Scheduler(CargoList& cargoList,
        FreightList& freightList,
        ScheduleList& scheduleList);

    // Use ArrivalSortStrategy
    void scheduleByArrival();

    // Use CapacitySortStrategy
    void scheduleByCapacity();

    // Expose lists read-only (for TUI reports)
    const CargoList& getCargoList() const { return cargoList_; }
    const FreightList& getFreightList() const { return freightList_; }
    const ScheduleList& getScheduleList() const { return scheduleList_; }

private:
    CargoList& cargoList_;
    FreightList& freightList_;
    ScheduleList& scheduleList_;

    std::map<std::string, std::vector<Cargo>> groupByDestination() const;
};
