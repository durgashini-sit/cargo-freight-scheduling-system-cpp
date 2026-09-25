#include <iostream>

#include "CargoList.h"
#include "FreightList.h"
#include "CargoDataManager.h"
#include "FreightDataManager.h"
#include "ScheduleList.h"
#include "TUI.h"

int main()
{
	// Core data structures
    CargoList cargoList;
    FreightList freightList;

    // Data managers operate directly on the lists
    CargoDataManager cargoDM(cargoList);
    FreightDataManager freightDM(freightList);

    // Scheduling structure
    ScheduleList scheduleList;

	TUI ui( // Initialize TUI with all components
        cargoList,
        freightList,
        cargoDM,
        freightDM,
        scheduleList
    );

	// Start the user interface
    ui.run();

    return 0;
}
