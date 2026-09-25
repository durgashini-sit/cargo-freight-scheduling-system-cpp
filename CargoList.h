#pragma once
#include <vector>
#include <string>
#include "Cargo.h"

class CargoList {
private:
    std::vector<Cargo> items;
    std::vector<std::string> lastErrors;

public:
    // loads cargos
    bool load();

    // saves cargos
    bool save(const std::string& filepath = "./data/output_cargo.txt") const;

    const std::vector<Cargo>& all() const { return items; }
    const std::vector<std::string>& errors() const { return lastErrors; }

    void clear() { items.clear(); }
    void add(const Cargo& c) { items.push_back(c); }
    std::size_t size() const { return items.size(); }
	bool remove(size_t index);
};
