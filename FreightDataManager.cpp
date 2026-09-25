#include "FreightDataManager.h"
#include <iostream>

FreightDataManager::FreightDataManager(FreightList& fl)
    : list(fl) {
    
}

// Extracts a label from the freight ID.
// Labels follow simple rules based on the last two characters.
// If the ID doesn't follow the format, a default label is used.
std::string FreightDataManager::extractLabel(const std::string& id) const
{
    if (id.size() < 3)
        return "mm";  // fallback default

    std::string tail = id.substr(id.size() - 2); // last 2 chars

    if (tail == "mm" || tail == "cc" || tail == "mc")
        return tail;

    return "mm";  // default if ID doesn't encode a valid label
}

// ADD a new Freight into the list.
// Uses FreightFactory so the capacity is set correctly for the label.
bool FreightDataManager::add(const std::string& id,
    const std::string& dest,
    const std::string& time)
{
    if (itemExists(id))
        return false;

	try { // Catch invalid time format
        Time t = Time::fromString(time);
        std::string label = extractLabel(id);

        // Factory creates Freight with correct capacity.
        Freight f = FreightFactory::createFreight(id, dest, t, label);

        list.add(f);
        return true;
    }
	catch (...) { // Invalid time format
        std::cout << "[FreightDM] Add failed: Invalid time format.\n";
        return false;
    }
}

// EDIT a freight entry.
// Removes the old one and re-inserts an updated version.
// We use the same ID, but update destination/time.
// Label is kept consistent with the ID.
bool FreightDataManager::edit(const std::string& id,
    const std::string& newDest,
    const std::string& newTime)
{
    const auto& freights = list.all();

    for (size_t i = 0; i < freights.size(); ++i) {
        if (freights[i].getId() == id) {

            std::string label = extractLabel(id);

            // Remove old freight from the list.
            list.remove(i);

            try {
				Time t = Time::fromString(newTime);
                Freight updated =
					FreightFactory::createFreight(id, newDest, t, label); // Create updated freight

				list.add(updated); // Add updated freight to the list
                return true;
            }
			catch (...) { // Invalid time format    
                std::cout << "[FreightDM] Edit failed: Invalid time format.\n";
                return false;
            }

        }
    }

    return false; // ID not found
}

// REMOVE a freight by ID.
// We must search by value and remove by index.
bool FreightDataManager::remove(const std::string& id)
{
    const auto& freights = list.all();

    for (size_t i = 0; i < freights.size(); ++i) {
        if (freights[i].getId() == id) {
            return list.remove(i);
        }
    }

    return false;
}

// Check if a freight exists in the list.
bool FreightDataManager::itemExists(const std::string& id) const
{
    for (const auto& f : list.all()) {
        if (f.getId() == id)
            return true;
    }
    return false;
}
