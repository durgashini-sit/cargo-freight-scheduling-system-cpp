#pragma once
#include <vector>
#include "Cargo.h"
#include "Freight.h"

class FileSaver {
public:
    virtual ~FileSaver() = default;
    virtual void saveToDataFolder(const std::vector<Cargo>& cargos,
        const std::vector<Freight>& freights) = 0;
};
