#include "Schedule.h"
#include <iostream>
#include <sstream>


Schedule::Schedule(const Freight& freight, const Cargo& cargo)
	: freight(freight), cargo(cargo) {
}

bool Schedule::isMatch() const {
	int cargoMin = cargo.getTime().toMinutes(); // to get cargo time
	int freightMin = freight.getTime().toMinutes(); // to get freight time

	// Allow freight up to 15 minutes earlier, but not later than cargo time
	if (freightMin < cargoMin - 15 || freightMin > cargoMin) return false;

	// 3) Enough capacity will be checked separately in ArrivalSortStrategy
	return true;
		
}

std::string Schedule::toString() const {
	std::ostringstream oss;

	oss << "Freight " << freight.getId()
		<< " (" << freight.getDestination()
		<< ", " << freight.getTime().toString()
		<< ")  <-->  Cargo " << cargo.getId()
		<< " (" << cargo.getDestination()
		<< ", " << cargo.getTime().toString()
		<< ", Group=" << cargo.getGroupSize()
		<< ")";

	return oss.str();
}

const Freight& Schedule::getFreight() const {
	return freight;
}

const Cargo& Schedule::getCargo() const {
	return cargo;
}
