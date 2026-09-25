#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "CargoDataManager.h"
#include "FreightDataManager.h"
#include "ScheduleList.h"

class CargoList;
class FreightList;

class TUI
{
public:
    TUI(CargoList& cargoList,
        FreightList& freightList,
        CargoDataManager& cargoDM,
        FreightDataManager& freightDM,
        ScheduleList& scheduleList);

    void run();

private:
    // Main data
    CargoList& cargoList_;
    FreightList& freightList_;
    CargoDataManager& cargoDM_;
    FreightDataManager& freightDM_;
    ScheduleList& scheduleList_;

	// Grace time is hardcoded to 15 mins 
    int graceTime = 15; 

    
    // Pagination System Functions
    int showCargoPage1();
    int showCargoPage2();

    int showFreightPage1();
    int showFreightPage2();

    int showReportsPage1();
    int showReportsPage2();


	// Menu functions
    void loadDataMenu();
    void showMainMenu();
    void showCargoMenu();
    void showFreightMenu();
    void showSchedulingMenu();
    void showReportsMenu();

    // Cargo CRUD operations 
    void addCargo();
    void editCargo();
    void deleteCargo();
    void listAllCargo();
    void saveCargoToFile();

    // Freight CRUD operations
    void addFreight();
    void editFreight();
    void deleteFreight();
    void listAllFreight();
    void saveFreightToFile();

	// Scheduling operations
    void scheduleByArrival();
    void scheduleByCapacity();

	// Report operations
    void listSchedulesByArrival();
    void listSchedulesByCapacity();
    void listFreightsNotFull();
    void listUnassignedCargo();
    void saveScheduleToFile();

    // Input helpers 
    std::string input(const std::string& prompt);
    int inputInt(const std::string& prompt, int min, int max);
    int getFreightLoad(const std::string& freightId);
    bool isValidMenuChoice(const std::string& s, const std::vector<std::string>& valid);
};