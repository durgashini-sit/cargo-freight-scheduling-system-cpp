#include "TxtFileLoader.h"
#include "FreightFactory.h"
#include "Utils.h"
#include "Time.h"
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <algorithm>

// path helper
static inline std::string joinPath(const std::string& a, const std::string& b) {
    if (!a.empty() && (a.back() == '/' || a.back() == '\\')) return a + b;
    return a + "/" + b;
}

// splits line at commas & trims whitespace
std::vector<std::string> TxtFileLoader::splitCSV(const std::string& line) {
    std::vector<std::string> out;
    std::stringstream ss(line);
    std::string item;
    while (std::getline(ss, item, ',')) out.push_back(Utils::trim(item));
    return out;
}

void TxtFileLoader::loadFromDataFolder() {
    cargos_.clear(); freights_.clear(); errors_.clear();
    loadCargoFile(joinPath("./data", "cargo.txt"));
    loadFreightFile(joinPath("./data", "freight.txt"));
}

void TxtFileLoader::loadCargoFile(const std::string& path) {

    // incase of missing file
    std::ifstream in(path);
    if (!in) { errors_.push_back("Missing file: " + path); return; }

    // tracks dupicates, counts lines, skips blank lines
    std::unordered_set<std::string> seen;
    std::string line; int lineNo = 0;
    while (std::getline(in, line)) {
        ++lineNo;
        auto trimmed = Utils::trim(line);
        if (trimmed.empty()) continue;

        // expect 4 fields
        auto f = splitCSV(trimmed);            
        if (f.size() != 4) { errors_.push_back("cargo.txt line " + std::to_string(lineNo) + ": expected 4 fields"); continue; }

        const std::string& id = f[0];
        const std::string& dest = f[1];
        const std::string& tStr = f[2];
        const std::string& gStr = f[3];

        // check if first char is C, extract data, prevent duplication
        if (!Utils::isValidID(id, 'C')) { errors_.push_back("cargo.txt line " + std::to_string(lineNo) + ": invalid cargo ID"); continue; }
        if (seen.count(id)) { errors_.push_back("cargo.txt line " + std::to_string(lineNo) + ": duplicate cargo ID"); continue; }
        seen.insert(id);

        try {
            Time t = Time::fromString(tStr);
            int g = std::stoi(gStr);                 
            cargos_.emplace_back(id, dest, t, g);    
        }
        catch (const std::exception& e) {
            errors_.push_back("cargo.txt line " + std::to_string(lineNo) + ": " + e.what());
        }
    }
}

void TxtFileLoader::loadFreightFile(const std::string& path) {

    // incase of missing file
    std::ifstream in(path);
    if (!in) { errors_.push_back("Missing file: " + path); return; }

    // tracks dupicates, counts lines, skips blank lines
    std::unordered_set<std::string> seen;
    std::string line; int lineNo = 0;
    while (std::getline(in, line)) {
        ++lineNo;
        auto trimmed = Utils::trim(line);
        if (trimmed.empty()) continue;

        // expect 4 fields
        auto f = splitCSV(trimmed);            
        if (f.size() != 4) { errors_.push_back("freight.txt line " + std::to_string(lineNo) + ": expected 4 fields"); continue; }

        const std::string& id = f[0];
        const std::string& dest = f[1];
        const std::string& tStr = f[2];
        const std::string& label = f[3];

        // check if first char is F, extract data, prevent duplication
        if (!Utils::isValidID(id, 'F')) { errors_.push_back("freight.txt line " + std::to_string(lineNo) + ": invalid freight ID"); continue; }
        if (seen.count(id)) { errors_.push_back("freight.txt line " + std::to_string(lineNo) + ": duplicate freight ID"); continue; }
        seen.insert(id);

        // send to FreightFactory
        try {
            Time t = Time::fromString(tStr);
            Freight f = FreightFactory::createFreight(id, dest, t, label);


            if (f.getCapacity() == 0) {
                errors_.push_back("freight.txt line " + std::to_string(lineNo) + ": unknown label");
            }

            else {
                freights_.push_back(f);
            }
        }
        catch (const std::exception& e) {
            errors_.push_back("freight.txt line " + std::to_string(lineNo) + ": " + e.what());
        }
    }
}
