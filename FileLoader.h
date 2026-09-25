#pragma once
#include <string>
#include <vector>
#include "Cargo.h"
#include "Freight.h"

class FileLoader {
public:
    virtual ~FileLoader() = default;
    virtual void loadFromDataFolder() = 0;
    virtual const std::vector<Cargo>& getCargos()   const = 0;
    virtual const std::vector<Freight>& getFreights() const = 0;
    virtual const std::vector<std::string>& getErrors() const = 0;
};
