#pragma once
#include <string>
#include "FreightList.h"
#include "CargoList.h"

class DataManager
{
public:
    virtual ~DataManager() = default;

    virtual bool add(const std::string& id, const std::string& dest, const std::string& time) = 0;
    virtual bool edit(const std::string& id, const std::string& dest, const std::string& time) = 0;
    virtual bool remove(const std::string& id) = 0;
    virtual bool itemExists(const std::string& id) const = 0;
};

