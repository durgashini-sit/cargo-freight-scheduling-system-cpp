#include "FreightList.h"
#include "TxtFileLoader.h"
#include "TxtFileSaver.h"

bool FreightList::load() {
    lastErrors.clear();
    items.clear();

    TxtFileLoader loader;
    loader.loadFromDataFolder();

    // stores freights & errors
    items = loader.getFreights();
    lastErrors = loader.getErrors();

    //checks thru errors for 'missing file' error
    bool missing = false;
    for (const auto& e : lastErrors)
        if (e.find("Missing file:") != std::string::npos) { missing = true; break; }

    return !missing;
}

bool FreightList::save(const std::string& filepath) const {
    TxtFileSaver saver;

    // Pass empty cargos and current freights
    std::vector<Cargo> emptyCargos;
    saver.saveToDataFolder(emptyCargos, items);

    return true;
}

bool FreightList::remove(size_t index) {
    if (index < items.size()) {
        items.erase(items.begin() + index);
        return true;
    }
    else
        return false;
}