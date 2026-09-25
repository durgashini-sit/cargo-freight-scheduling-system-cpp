#pragma once
#include "DataManager.h"
#include "FreightList.h"
#include "FreightFactory.h"
#include "Time.h"
#include <string>


class FreightDataManager : public DataManager
{
private:

    FreightList& list;

    // Helper function:
    // Extracts label from the freight ID (e.g., "F01mm" → "mm").
    // Factory depends on this label to choose the right freight type.
    std::string extractLabel(const std::string& id) const;

public:
    FreightDataManager(FreightList& fl);
        // Create a new freight.
        bool add(const std::string& id,const std::string& dest,const std::string& time) override;

        // Update an existing freight.
        bool edit(const std::string& id,const std::string& newDest,const std::string& newTime) override;

        // Remove a freight by ID.
        bool remove(const std::string& id) override;

        // Check if a freight exists in the list.
        bool itemExists(const std::string& id) const override;
};
