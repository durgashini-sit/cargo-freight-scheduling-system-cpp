#include "CargoList.h"
#include "TxtFileLoader.h"
#include "TxtFileSaver.h"

bool CargoList::load() {
    lastErrors.clear();
    items.clear();

    TxtFileLoader loader;
    loader.loadFromDataFolder();

    // stores cargos & errors
    items = loader.getCargos();
    lastErrors = loader.getErrors();

    //checks thru errors for 'missing file' error
    bool missing = false;
    for (const auto& e : lastErrors)
        if (e.find("Missing file:") != std::string::npos) { missing = true; break; }

    return !missing;
}

bool CargoList::save(const std::string& filepath) const {
    TxtFileSaver saver;

    // pass empty freights and current cargos
    std::vector<Freight> emptyFreights;
    saver.saveToDataFolder(items, emptyFreights);

    return true;
}

bool CargoList::remove(size_t index) {
    if (index < items.size()) {
        items.erase(items.begin() + index);
        return true;
    }
    else
        return false;
}