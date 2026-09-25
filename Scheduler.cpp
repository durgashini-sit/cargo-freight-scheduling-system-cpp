#include "Scheduler.h"

Scheduler::Scheduler(CargoList& cargoList,
    FreightList& freightList,
    ScheduleList& scheduleList)
    : cargoList_(cargoList),
    freightList_(freightList),
    scheduleList_(scheduleList)
{
}

std::map<std::string, std::vector<Cargo>> Scheduler::groupByDestination() const { std::map<std::string, std::vector<Cargo>> grouped;

    const auto& cargos = cargoList_.all();
    for (const auto& c : cargos) {
        grouped[c.getDestination()].push_back(c);
    }
    return grouped;
}

void Scheduler::scheduleByArrival() {
    auto grouped = groupByDestination();
    ArrivalSortStrategy strategy; 

    scheduleList_.clear();
    strategy.generate(grouped, freightList_, scheduleList_);
}

void Scheduler::scheduleByCapacity() {
    auto grouped = groupByDestination();
    CapacitySortStrategy strategy;

    scheduleList_.clear();
    strategy.generate(grouped, freightList_, scheduleList_);
}
