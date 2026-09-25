#include "TxtFileSaver.h"
#include <fstream>
#include <iostream>

void TxtFileSaver::saveToDataFolder(const std::vector<Cargo>& cargos,
    const std::vector<Freight>& freights) {

    // open output streams, write new files in 'data' folder
    // if either already exist, its overwritten by default
    std::ofstream cargoOut("./data/output_cargo.txt");
    std::ofstream freightOut("./data/output_freight.txt");

    // error
    if (!cargoOut || !freightOut) {
        std::cerr << "Error opening output files.\n";
        return;
    }

    // loop thru each obj
    for (const auto& c : cargos)   cargoOut << c.toString() << '\n';
    for (const auto& f : freights) freightOut << f.toString() << '\n';
}
