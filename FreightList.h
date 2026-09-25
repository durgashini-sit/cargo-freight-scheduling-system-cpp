#pragma once
#include <vector>
#include <string>
#include "Freight.h"

class FreightList {
private:
    std::vector<Freight> items;
    std::vector<std::string> lastErrors;

public:
    // loads freights
    bool load();

    // saves freights
    bool save(const std::string& filepath = "./data/output_freight.txt") const;

    const std::vector<Freight>& all() const { return items; }
    const std::vector<std::string>& errors() const { return lastErrors; }

    void clear() { items.clear(); }
    void add(const Freight& f) { items.push_back(f); }
    std::size_t size() const { return items.size(); }
    bool remove(size_t index);
};
