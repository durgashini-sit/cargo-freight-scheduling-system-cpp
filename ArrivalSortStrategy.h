#pragma once

#include "SchedulingStrategy.h"

class ArrivalSortStrategy : public SchedulingStrategy
{
	public:
	ArrivalSortStrategy() = default;

	void generate(
		const std::map<std::string, std::vector<Cargo>>& groupedCargos, FreightList& freightList, ScheduleList& scheduleList) override;
};

