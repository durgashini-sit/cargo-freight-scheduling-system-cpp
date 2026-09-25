#include "CargoDataManager.h"
#include "Cargo.h"
#include <iostream>

CargoDataManager::CargoDataManager(CargoList& list)
    : cargoList(list) {
}

// ADD a new cargo.
// New cargo always starts with groupSize = 1.
// Returns false if ID already exists.
bool CargoDataManager::add(const std::string& id,
    const std::string& dest,
    const std::string& time)
{
    if (itemExists(id)) {
        std::cout << "[CargoDM] Add failed: ID already exists.\n";
        return false;
    }
	try { // Catch invalid time format
        Time t = Time::fromString(time);
        Cargo newCargo(id, dest, t, 1);

        cargoList.add(newCargo);
        return true;

    }
	catch (...) { // Invalid time format
		std::cout << "[CargoDM] Add failed: Invalid time format.\n";
        return false;
    }
    
}

// EDIT cargo information.
// We rebuild the Cargo object because Cargo has no setters.
// Group size is preserved from the original item.
bool CargoDataManager::edit(const std::string& id,
    const std::string& dest,
    const std::string& time)
{
    auto& list = cargoList.all();

    for (size_t i = 0; i < list.size(); ++i) {
        if (list[i].getId() == id) {
             
			try { // Catch invalid time format
                Time t = Time::fromString(time);

				Cargo updated(id, dest, t, list[i].getGroupSize()); 

				cargoList.remove(i); // Remove old entry
				cargoList.add(updated); // Add updated entry
                return true; 
            }
			catch (...) { // Invalid time format
                std::cout << "[CargoDM] Edit failed: Invalid time format.\n";
                return false;
            }
        }
    }

    return false; // ID not found
}

// REMOVE a cargo by matching ID.
bool CargoDataManager::remove(const std::string& id)
{
    const auto& list = cargoList.all();

    for (size_t i = 0; i < list.size(); ++i) {
        if (list[i].getId() == id) {
            return cargoList.remove(i);
        }
    }

    return false;
}

// Check if cargo exists in the list.
bool CargoDataManager::itemExists(const std::string& id) const
{
    const auto& list = cargoList.all();

    for (const auto& c : list) {
        if (c.getId() == id)
            return true;
    }

    return false;
}
