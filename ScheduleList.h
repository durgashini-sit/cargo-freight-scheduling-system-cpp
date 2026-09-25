#pragma once

#include <vector>
#include <memory>
#include <map>
#include <string>

#include "Schedule.h"
#include "SchedulingStrategy.h"
#include "Cargo.h"

class CargoList;
class FreightList;
class SchedulingStrategy;

class ScheduleList
{
private:
	std::vector <Schedule> schedules;
	std::unique_ptr<SchedulingStrategy> strategy;

	//grouping cargos by destination (shared preprocessing)
	std::map < std::string, std::vector<Cargo> > groupCargosByDestination(CargoList& CL);

public:
	ScheduleList();

	void setStrategy(std::unique_ptr<SchedulingStrategy> s);

	//delegate scheduling to strategy
	void generate(CargoList& CL, FreightList& FL);

	void add(const Schedule& s);
	void clear();

	const std::vector<Schedule>& all() const;

	size_t getSize() const;
	bool remove(size_t index);
	//bool save();
};

