#pragma once
#include "Cargo.h"
#include "Freight.h"

#include <vector>
#include <string>

class Schedule {
private:
	Cargo cargo; // composition relationship
	Freight freight; // composition relationship

public:
	Schedule(const Freight& freight, const Cargo& cargo); // constructor
	bool isMatch() const; // for matching logic

	std::string toString() const; // to override from Record

	// getter functions
	const Freight& getFreight() const;
	const Cargo& getCargo() const;

};

