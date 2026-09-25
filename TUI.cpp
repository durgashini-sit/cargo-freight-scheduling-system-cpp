#include "TUI.h"
#include "CargoList.h"
#include "FreightList.h"
#include "Cargo.h"
#include "Freight.h"
#include "Schedule.h"

#include "ArrivalSortStrategy.h"
#include "CapacitySortStrategy.h"
#include "TxtFileSaver.h"
#include "SchedulingStrategy.h"        
#include <iostream>
#include <limits>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <fstream>


// Constructor 
TUI::TUI(
    CargoList& cargoList,
    FreightList& freightList,
    CargoDataManager& cargoDM,
    FreightDataManager& freightDM,
    ScheduleList& scheduleList)
    : cargoList_(cargoList),
    freightList_(freightList),
    cargoDM_(cargoDM),
    freightDM_(freightDM),
    scheduleList_(scheduleList)
{
    
}

//---------------------------------------------------------------------------------

// DATA LOADING  

void TUI::loadDataMenu() {
    std::cout << "=========== CARGO-FREIGHT SCHEDULER ==========" << '\n';
	std::cout << "Enter folder name containing cargo.txt & freight.txt (Hint: data)" << '\n'; //Loading data from folder
	while (true) { // Loop until valid folder is provided
        std::string folder = input("Folder name");
		if (folder != "data") // Only accept "data"
        {
            std::cout << "Invalid folder, please try again. (Hint: data)" << '\n';
            continue;
        }
		bool cargoOK = cargoList_.load();     //load cargo list
		bool freightOK = freightList_.load(); //load freight list
		if (!cargoOK || !freightOK) {  // Failed to load either cargo or freight
            std::cout << "\nError! Failed to load from ./data." << '\n';
        }
		else { // Successfully loaded both cargo and freight
            std::cout << "\nLoaded " << cargoList_.size() << " cargos and " << freightList_.size() << " freights." << '\n'; return;
        }
    }
}

//---------------------------------------------------------------------------------
 
// Main Program Lopp
void TUI::run()
{
    loadDataMenu();

    while (true) {
        showMainMenu();
        int choice = inputInt("Choose option", 0, 4);

        switch (choice) {
        case 1: showCargoMenu();      break;
        case 2: showFreightMenu();    break;
        case 3: showSchedulingMenu(); break;
        case 4: showReportsMenu();    break;
        case 0:
            std::cout << "Exiting program...\n";
            return;
        }
    }
}


// Main Menu Display 
void TUI::showMainMenu()
{
    std::cout << "\n========== MAIN MENU ==========\n";
    std::cout << "1. Cargo operations\n";
    std::cout << "2. Freight operations\n";
    std::cout << "3. Scheduling\n";
    std::cout << "4. Reports\n";
    std::cout << "0. Exit\n";
}

//---------------------------------------------------------------------------
bool TUI::isValidMenuChoice(const std::string& s,        
	const std::vector<std::string>& valid) 
{
	return std::find(valid.begin(), valid.end(), s) != valid.end(); // Check if s is in valid choices
}

//Cargo Menu
    void TUI::showCargoMenu()
{
    int page = 1;

    while (true)
    {
        if (page == 1)
            page = showCargoPage1();

        else if (page == 2)
            page = showCargoPage2();

        else
            return; //exit
    }
}

    int TUI::showCargoPage1()
    {
        std::cout << "\n------ Cargo Menu (Page 1/2) ------\n";
        std::cout << "1. Add Cargo\n";
        std::cout << "2. Edit Cargo\n";
        std::cout << "3. Delete Cargo\n";
        std::cout << "N. Next Page\n";
        std::cout << "0. Back\n";

        std::string choice = input("Choose");

		//Valid choices for page 1
        std::vector<std::string> valid = { "1", "2", "3", "N", "n", "0" };

		// aValidate choice
        if (!isValidMenuChoice(choice, valid))
        {
            std::cout << "Invalid choice. Try again.\n";
            return 1; // stays on page 1
        }

        // If valid, perform action
		if (choice == "1") addCargo(); // add cargo
		else if (choice == "2") editCargo(); // edit cargo
		else if (choice == "3") deleteCargo(); // delete cargo
		else if (choice == "N" || choice == "n") return 2; //next page
		else if (choice == "0") return 0; // back

        return 1;
    }

    int TUI::showCargoPage2()
    {
        std::cout << "\n------ Cargo Menu (Page 2/2) ------\n";
        std::cout << "4. List All Cargo\n";
        std::cout << "5. Save Cargo/Freight Data\n";
        std::cout << "P. Previous Page\n";
        std::cout << "0. Back\n";

        std::string choice = input("Choose");

		// Valid choices for page 2
        std::vector<std::string> valid = { "4", "5", "P", "p", "0" };

		// Validate choice
        if (!isValidMenuChoice(choice, valid))
        {
            std::cout << "Invalid choice. Try again.\n";
            return 2; // stays on page 2
        }

		if (choice == "4") listAllCargo(); // list all cargo
		else if (choice == "5") saveCargoToFile(); // save cargo data
		else if (choice == "P" || choice == "p") return 1; // previous page
		else if (choice == "0") return 0; // back

        return 2;
    }



//Add Cargo
void TUI::addCargo()
{

    std::string idNum = input("Cargo Number (without C, e.g. 01)");   // Ask for id number only
    if (!idNum.empty() && (idNum[0] == 'C' || idNum[0] == 'c')) { // Handle case where user includes C
        idNum = idNum.substr(1);
    }
    std::string id = "C" + idNum;   // Prepend C to form full ID
	

    std::string dest = input("Destination");
    std::string time = input("Arrival Time (HH:MMam/HH:MMpm)");

    if (cargoDM_.add(id, dest, time)) // Use DataManager to add cargo
        std::cout << "Cargo added \n";
    else
        std::cout << "Failed to add cargo.\n";
}


// Edit Cargo
void TUI::editCargo()
{
	std::string idNum = input("Cargo Number to edit (without C,e.g. 01)"); // Ask for id number only
    if (!idNum.empty() && (idNum[0] == 'C' || idNum[0] == 'c')) { // Handle case where user includes C
        idNum = idNum.substr(1);
    }
	std::string id = "C" + idNum;  // Prepend C to form full ID

    std::string dest = input("New destination");
    std::string time = input("New time (HH:MMam/HH:MMpm)");

	if (cargoDM_.edit(id, dest, time)) // Use DataManager to edit cargo
        std::cout << "Cargo updated.\n";
    else
        std::cout << "Update failed.\n";
}


//Delete Cargo
void TUI::deleteCargo()
{
	std::string idNum = input("Cargo Number to delete (without C,e.g. 01)"); // Ask for id number only
    if (!idNum.empty() && (idNum[0] == 'C' || idNum[0] == 'c')) { // Handle case where user includes C
        idNum = idNum.substr(1);
    }
	std::string id = "C" + idNum; // Prepend C to form full ID

	if (cargoDM_.remove(id))  // Use DataManager to remove cargo
        std::cout << "Cargo removed.\n";
    else
        std::cout << "Cargo not found.\n";
}


//List All Cargo
void TUI::listAllCargo()
{
    for (const auto& c : cargoList_.all()) {  
		std::cout << c.toString() << "\n"; // Display each cargo
    }
}


// Save Cargo to File
void TUI::saveCargoToFile()
{
	TxtFileSaver saver; // Use TxtFileSaver to save data
	saver.saveToDataFolder(cargoList_.all(), freightList_.all()); // Save both cargo and freight

    std::cout << "Cargo/Freight data saved to ./data/output files.\n";
}


//---------------------------------------------------------------------------

// Freight Menu
void TUI::showFreightMenu()
{
    int page = 1;

    while (true)
    {
        if (page == 1)
			page = showFreightPage1(); // Show page 1
        else if (page == 2)
			page = showFreightPage2(); // Show page 2
        else
            return;
    }
}

// Freight Menu Page 1
int TUI::showFreightPage1()
{
    std::cout << "\n------ Freight Menu (Page 1/2) ------\n";
    std::cout << "1. Add Freight\n";
    std::cout << "2. Edit Freight\n";
    std::cout << "3. Delete Freight\n";
    std::cout << "N. Next Page\n";
    std::cout << "0. Back\n";

    std::string choice = input("Choose");
	std::vector<std::string> valid = { "1", "2", "3", "N", "n", "0" }; // Valid choices for page 1

    if (!isValidMenuChoice(choice, valid))
    {
        std::cout << "Invalid choice. Try again.\n";
        return 1;
    }
	// If valid, perform action
	if (choice == "1") addFreight(); // Add freight
	else if (choice == "2") editFreight(); // Edit freight
	else if (choice == "3") deleteFreight(); // Delete freight
	else if (choice == "N" || choice == "n") return 2; // Next page
	else return 0; // Back

    return 1;
}

// Freight Menu Page 2
int TUI::showFreightPage2() 
{
    std::cout << "\n------ Freight Menu (Page 2/2) ------\n";
    std::cout << "4. List All Freight\n";
    std::cout << "5. Save Cargo/Freight Data\n";
    std::cout << "P. Previous Page\n";
    std::cout << "0. Back\n";

	std::string choice = input("Choose"); // Get user choice
	std::vector<std::string> valid = { "4", "5", "P", "p", "0" }; // Valid choices for page 2

    if (!isValidMenuChoice(choice, valid))
    {
        std::cout << "Invalid choice. Try again.\n";
        return 2;
    }

	if (choice == "4") listAllFreight(); // List all freight
	else if (choice == "5") saveFreightToFile(); // Save freight data
	else if (choice == "P" || choice == "p") return 1; // Previous page
	else return 0; // Back

    return 2;
}


// Add Freight  
void TUI::addFreight()
{
	std::cout << "Freight type determined by suffix: mm, cc, mc\n"; // Inform user about suffix requirement

	// Ask for freight number only, F will be hardcoded to prevent user error
    std::string number = input("Freight Number (without F, e.g. 01)");
    
	if (!number.empty() && (number[0] == 'F' || number[0] == 'f')) { // Handle case where user includes F
        number = number.substr(1);
    }

    // Ask for suffix ONLY
    std::string suffix;
    while (true) {
        suffix = input("Freight Type Suffix (mm / cc / mc)");
        if (suffix == "mm" || suffix == "cc" || suffix == "mc")
            break;
        std::cout << "Invalid suffix! Must be mm, cc, or mc.\n";
    }

    std::string id = "F" + number + suffix;

    std::string dest = input("Destination");
    std::string time = input("Departure Time (HH:MMam/HH:MMpm)");

    if (freightDM_.add(id, dest, time))
        std::cout << "Freight added \n";
    else
        std::cout << "Failed to add freight.\n";
}

// Edit Freight
void TUI::editFreight()
{
	std::string number = input("Freight Number to edit (without F,e.g. 01)"); // Ask for id number only
    if (!number.empty() && (number[0] == 'F' || number[0] == 'f')) { // Handle case where user includes F
        number = number.substr(1);
    }
	std::string suffix = input("Freight Type Suffix (mm / cc / mc)"); // Ask for suffix 
	std::string id = "F" + number + suffix; // Prepend F to form full ID

    std::string dest = input("New destination");
    std::string time = input("New departure time (HH:MMam/HH:MMpm)");

    if (freightDM_.edit(id, dest, time))
        std::cout << "Freight updated.\n";
    else
        std::cout << "Update failed.\n";
}


// Delete Freight
void TUI::deleteFreight()
{
	std::string number = input("Freight Number to delete (without F,e.g. 01)");   // Ask for id number only   
    if (!number.empty() && (number[0] == 'F' || number[0] == 'f')) { // Handle case where user includes F
        number = number.substr(1);
    }
	std::string suffix = input("Freight Type Suffix (mm / cc / mc)"); // Ask for suffix 
	std::string id = "F" + number + suffix;     // Prepend F to form full ID
     
    if (freightDM_.remove(id))
        std::cout << "Freight removed.\n";
    else
        std::cout << "Freight not found.\n";
}

// List All Freight
void TUI::listAllFreight()
{
	for (const auto& f : freightList_.all()) // Iterate through all freights
        std::cout << f.toString() << "\n";
}

// Save Freight 
void TUI::saveFreightToFile()
{
    TxtFileSaver saver;
	saver.saveToDataFolder(cargoList_.all(), freightList_.all()); 
	std::cout << "Cargo/Freight data saved to ./data/output files.\n"; 
}



//---------------------------------------------------------------------------
// Scheduling Menu
void TUI::showSchedulingMenu()
{
    std::cout << "\n------ Scheduling ------\n";
    std::cout << "1. Schedule by Arrival Time\n";
    std::cout << "2. Schedule by Freight Capacity\n";
    std::cout << "0. Back to Main Menu\n";

	int choice = inputInt("Choose", 0, 2); // get user choice

    switch (choice) {
    case 1: scheduleByArrival();  break;
    case 2: scheduleByCapacity(); break;
    case 0: return;
    }
}

// Scheduling (Arrival Sort)
void TUI::scheduleByArrival()
{
	scheduleList_.clear(); // Clear existing schedules
	scheduleList_.setStrategy(std::make_unique<ArrivalSortStrategy>()); // Set strategy to Arrival Sort
	scheduleList_.generate(cargoList_, freightList_); // Generate schedules

    std::cout << "Schedules generated (Arrival Sort).\n";
}


// Scheduling (Capacity Sort)
void TUI::scheduleByCapacity()
{
	scheduleList_.clear(); // Clear existing schedules
	scheduleList_.setStrategy(std::make_unique<CapacitySortStrategy>()); // Set strategy to Capacity Sort
	scheduleList_.generate(cargoList_, freightList_); // Generate schedules

    std::cout << "Schedules generated (Capacity Sort).\n";
}


//---------------------------------------------------------------------------
// Report Menu
void TUI::showReportsMenu()
{
    int page = 1;

    while (true)
    {
        if (page == 1)
			page = showReportsPage1(); // Show page 1
        else if (page == 2)
			page = showReportsPage2(); // Show page 2
        else
            return;
    }
}

// Report Menu Page 1
int TUI::showReportsPage1()
{
    std::cout << "\n------ Reports (Page 1/2) ------\n";
    std::cout << "1. Schedules (Arrival Sort)\n";
    std::cout << "2. Schedules (Capacity Sort)\n";
    std::cout << "N. Next Page\n";
    std::cout << "0. Back\n";

    std::string choice = input("Choose");
    std::vector<std::string> valid = { "1", "2", "N", "n", "0" };

    if (!isValidMenuChoice(choice, valid))
    {
        std::cout << "Invalid choice.\n";
        return 1;
    }

    if (choice == "1") listSchedulesByArrival(); // List schedules by arrival 
    else if (choice == "2") listSchedulesByCapacity(); // List schedules by capacity
    else if (choice == "N" || choice == "n") return 2; // Next page
	else return 0; // Back

    return 1;
}

// Report Menu Page 2
int TUI::showReportsPage2()
{
    std::cout << "\n------ Reports (Page 2/2) ------\n";
    std::cout << "3. Freights Not Full\n";
    std::cout << "4. Unassigned Cargo\n";
    std::cout << "5. Save Schedule Plan\n";
    std::cout << "P. Previous Page\n";
    std::cout << "0. Back\n";

    std::string choice = input("Choose");
    std::vector<std::string> valid = { "3", "4", "5", "P", "p", "0" };

    if (!isValidMenuChoice(choice, valid))
    {
        std::cout << "Invalid choice.\n";
        return 2;
    }

	if (choice == "3") listFreightsNotFull(); // List freights not full
	else if (choice == "4") listUnassignedCargo(); // List unassigned cargo
	else if (choice == "5") saveScheduleToFile(); // Save schedule to file
	else if (choice == "P" || choice == "p") return 1; // Previous page
	else return 0;  // Back

    return 2;
}


//List Schedules (Arrival Sort)
void TUI::listSchedulesByArrival()
{
	auto schedules = scheduleList_.all(); // Get all schedules
     
	std::sort(schedules.begin(), schedules.end(), //    Sort by arrival time
        [](const Schedule& a, const Schedule& b) {
			return a.getCargo().getTime().toString() < b.getCargo().getTime().toString(); // Ascending order
        });

	for (const auto& s : schedules) // Display each schedule
        std::cout << s.toString() << "\n";
}

//List Schedules (Capacity Sort)
void TUI::listSchedulesByCapacity()
{
	auto schedules = scheduleList_.all(); // Get all schedules

	std::sort(schedules.begin(), schedules.end(), // Sort by freight capacity (descending)
        [](const Schedule& a, const Schedule& b) {
			return a.getFreight().getCapacity() > // Descending order
				b.getFreight().getCapacity(); // Compare capacities
        });

    for (const auto& s : schedules)
        std::cout << s.toString() << "\n";
}
 
// Get Freight Load
int TUI::getFreightLoad(const std::string& freightId)
{
    int load = 0;
	for (const auto& s : scheduleList_.all()) {  // Iterate through all schedules
		if (s.getFreight().getId() == freightId) {  // Match freight ID
            load += s.getCargo().getGroupSize(); 
        }
    }
    return load;
}


// Freights Not Full
void TUI::listFreightsNotFull()
{
	const auto& freights = freightList_.all(); // Get all freights

    bool any = false;

	for (const auto& f : freights) { // Iterate through freights

		int cap = f.getCapacity(); // Get freight capacity
		int load = getFreightLoad(f.getId()); // Get current load

		if (load < cap) {  // Check if not full
            std::cout << f.toString() 
				<< " | Load " << load << "/" << cap << "\n"; // Display info of freights that are not full
            any = true;
        }
    }

    if (!any)
        std::cout << "All freights are fully loaded.\n";
}

//---------------------------------------------------------------------------
// Unassigned Cargo
void TUI::listUnassignedCargo()
{
	const auto& cargos = cargoList_.all(); 
	const auto& schedules = scheduleList_.all(); 

	std::unordered_set<std::string> assigned; 

    for (const auto& s : schedules)
        assigned.insert(s.getCargo().getId());

    bool any = false;

    for (const auto& c : cargos) {
        if (!assigned.count(c.getId())) {
            std::cout << c.toString() << "\n";
            any = true;
        }
    }

    if (!any)
        std::cout << "All cargos are assigned.\n";
}

//---------------------------------------------------------------------------
// Save Schedule to File
void TUI::saveScheduleToFile()
{
	std::ofstream out("./data/schedule_output.txt"); // Open output file

    if (!out) {
		std::cout << "ERROR: Cannot open schedule_output.txt.\n"; // Error handling
        return;
    }

	for (const auto& s : scheduleList_.all()) // Write each schedule to file
        out << s.toString() << "\n";

	std::cout << "Schedule saved to ./data/schedule_output.txt\n";// Notify user of successful save
}



//---------------------------------------------------------------------------
// Helpers for Inputs

std::string TUI::input(const std::string& prompt)
{
	std::cout << prompt << ": "; // Display prompt
    std::string s;
	std::getline(std::cin, s); // Get user input

	if (s.empty()) // Handle empty input
		std::getline(std::cin, s); //Retry reading input

    return s;
}

int TUI::inputInt(const std::string& prompt, int min, int max) // Input integer within range
{
    while (true) {
		std::cout << prompt << " [" << min << "-" << max << "]: "; // Display prompt with range

        int v;
		if (std::cin >> v && v >= min && v <= max) { // Validate input
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return v;
        }

		std::cin.clear(); // Clear error state
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
        std::cout << "Invalid input.\n";
    }
}
