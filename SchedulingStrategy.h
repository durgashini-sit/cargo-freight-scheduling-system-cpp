#pragma once
#include <map>
#include <vector>
#include <string>

class Cargo;
class FreightList;
class ScheduleList;

class SchedulingStrategy
{
	public:
	virtual ~SchedulingStrategy() = default;
	virtual void generate(const std::map<std::string, std::vector<Cargo>>& groupedCargos, FreightList& freightList, ScheduleList& scheduleList) = 0;
	

};

