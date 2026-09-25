#pragma once
#include "FileSaver.h"

class TxtFileSaver : public FileSaver {
public:
    void saveToDataFolder(const std::vector<Cargo>& cargos,
        const std::vector<Freight>& freights) override;
};
