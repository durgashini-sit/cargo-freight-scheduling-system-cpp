#pragma once
#include "FileLoader.h"

class TxtFileLoader : public FileLoader {
private:
    std::vector<Cargo> cargos_;
    std::vector<Freight> freights_;
    std::vector<std::string> errors_;
    static std::vector<std::string> splitCSV(const std::string& line);
    void loadCargoFile(const std::string& path);
    void loadFreightFile(const std::string& path);

public:
    void loadFromDataFolder() override;
    const std::vector<Cargo>& getCargos()   const override { return cargos_; }
    const std::vector<Freight>& getFreights() const override { return freights_; }
    const std::vector<std::string>& getErrors() const override { return errors_; }
};
