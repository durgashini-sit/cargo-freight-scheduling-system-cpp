#include "SchedulingStrategy.h"
#include "ScheduleList.h"
#include "CargoList.h"
#include "FreightList.h"

#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stdexcept>

ScheduleList::ScheduleList() {}

void ScheduleList::setStrategy(std::unique_ptr<SchedulingStrategy> s) {
	strategy = std::move(s);
}

//adding schedule to list
void ScheduleList::add(const Schedule& s) {
	schedules.push_back(s);
}

//clearing all schedules
void ScheduleList::clear() {
	schedules.clear();
}

//getting all schedules
const std::vector<Schedule>& ScheduleList::all() const {
	return schedules;
}

//getting size function 
size_t ScheduleList::getSize() const {
	return schedules.size();
}

//removing index
bool ScheduleList::remove(size_t index) {
	if (index < schedules.size()) {
		schedules.erase(schedules.begin() + index);
		return true;
	}
	else
		return false;
}

//grouping cargos by destination
std::map < std::string, std::vector<Cargo> > ScheduleList::groupCargosByDestination(CargoList& CL) {
	std::map < std::string, std::vector<Cargo> > groupedCargos;
	for (const auto& cargo : CL.all())
		groupedCargos[cargo.getDestination()].push_back(cargo);
	return groupedCargos;
}

// Strategy Pattern: 
// 1. Ensure a strategy is set.
// 2. Group cargos by destination.
// 3. Delegate scheduling work entirely to the chosen strategy.

void ScheduleList::generate(CargoList& CL, FreightList& FL)
{
	if (!strategy) {
		throw std::runtime_error("Scheduling strategy not set.");
	}

	schedules.clear();   // Reset schedules

	// Preprocessing step: group cargos by destination
	auto grouped = groupCargosByDestination(CL);

	// Delegate all scheduling work to the strategy
	strategy->generate(grouped, FL, *this);

	std::cout << "Generated " << schedules.size()
		<< " schedules using strategy.\n";
}


