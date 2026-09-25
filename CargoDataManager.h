#pragma once
#include "DataManager.h"
#include "CargoList.h"
#include "Time.h"


class CargoDataManager : public DataManager
{
private:
    CargoList& cargoList;

public:
    CargoDataManager(CargoList& list);
    // Add new cargo 
    bool add(const std::string& id, const std::string& dest,const std::string& time) override;

    // Edit destination/time of an existing cargo.
    bool edit(const std::string& id, const std::string& dest,const std::string& time) override;

    // Remove cargo by ID.
    bool remove(const std::string& id) override;

    // Check if cargo exists.
    bool itemExists(const std::string& id) const override;
  
  
};
