#include <iostream>

// Core project includes
#include "CargoList.h"
#include "FreightList.h"
#include "ScheduleList.h"
#include "Schedule.h"

// Strategy Pattern
#include "CapacitySortStrategy.h"
#include "ArrivalSortStrategy.h"

// CRUD Managers
#include "CargoDataManager.h"
#include "FreightDataManager.h"


// SECTION 1 — Helper: Print a schedule plan

static void printSchedulePlan(const std::string& title, const ScheduleList& plan)
{
    std::cout << "\n==== " << title << " ====\n";
    const auto& schedules = plan.all();
    std::cout << "Total schedules: " << schedules.size() << "\n";

    for (const auto& s : schedules)
        std::cout << "  " << s.toString() << "\n";
}


// SECTION 2 — isMatch() Test
static void testBruteForceMatching()
{
    std::cout << "\n========= TEST MATCHING =========\n";

    CargoList cargos;
    FreightList freights;

    cargos.load();
    freights.load();

    std::cout << "[DEBUG] Loaded cargos:   " << cargos.size() << "\n";
    std::cout << "[DEBUG] Loaded freights: " << freights.size() << "\n";

    std::cout << "\n[DEBUG] Valid matches using Schedule::isMatch():\n";

    for (const auto& cargo : cargos.all())
    {
        for (const auto& freight : freights.all())
        {
            Schedule sched(freight, cargo);
            if (sched.isMatch())
            {
                std::cout << "  Cargo " << cargo.getId()
                    << " -> Freight " << freight.getId()
                    << " | cargo " << cargo.getTime().toString()
                    << " | freight " << freight.getTime().toString()
                    << "\n";
            }
        }
    }
}


// SECTION 3 — Strategy Scheduling Test

static void testStrategyScheduling()
{
    std::cout << "\n========= TESTING STRATEGY SCHEDULING =========\n";

    CargoList cargos;
    FreightList freights;

    cargos.load();
    freights.load();

    // ---- Capacity Strategy ----
    ScheduleList capPlan;
    capPlan.setStrategy(std::make_unique<CapacitySortStrategy>());
    capPlan.generate(cargos, freights);
    printSchedulePlan("Capacity Strategy Result", capPlan);

    // Reload to restore original data
    cargos.load();
    freights.load();

    // ---- Arrival Strategy ----
    ScheduleList arrPlan;
    arrPlan.setStrategy(std::make_unique<ArrivalSortStrategy>());
    arrPlan.generate(cargos, freights);
    printSchedulePlan("Arrival Strategy Result", arrPlan);
}


// SECTION 4 — CargoDataManager CRUD
static void testCargoManager()
{
    std::cout << "\n========= TESTING CargoDataManager =========\n";

    CargoList cl;
    CargoDataManager cm(cl);

    std::cout << "\n[TEST] Adding C01...\n";
    std::cout << (cm.add("C01", "Tokyo", "5:10pm") ? "OK" : "FAIL") << "\n";

    std::cout << "[TEST] Adding C02...\n";
    std::cout << (cm.add("C02", "Amsterdam", "7:20am") ? "OK" : "FAIL") << "\n";

    std::cout << "\n[TEST] Adding DUPLICATE C01...\n";
    std::cout << (cm.add("C01", "Tokyo", "8:00pm") ? "OK" : "FAIL (correct)") << "\n";

    std::cout << "\n[TEST] itemExists(\"C02\") → "
        << (cm.itemExists("C02") ? "TRUE" : "FALSE") << "\n";

    std::cout << "[TEST] itemExists(\"C99\") → "
        << (cm.itemExists("C99") ? "TRUE" : "FALSE (correct)") << "\n";

    std::cout << "\n[TEST] Editing C02...\n";
    std::cout << (cm.edit("C02", "Amsterdam", "7:45am") ? "OK" : "FAIL") << "\n";

    std::cout << "\n[TEST] Removing C01...\n";
    std::cout << (cm.remove("C01") ? "OK" : "FAIL") << "\n";

    std::cout << "[TEST] Removing NON-EXISTENT C77...\n";
    std::cout << (cm.remove("C77") ? "OK?" : "FAIL (correct)") << "\n";

    std::cout << "\n=========== FINAL CARGO LIST ===========\n";
    for (const auto& c : cl.all())
    {
        std::cout << c.getId() << " | "
            << c.getDestination() << " | "
            << c.getTime().toString()
            << " | group=" << c.getGroupSize() << "\n";
    }
    std::cout << "==========================================\n";
}


// SECTION 5 — YOUR TEST: FreightDataManager CRUD
static void testFreightManager()
{
    std::cout << "\n========= TESTING FreightDataManager =========\n";

    FreightList fl;
    FreightDataManager fm(fl);

    std::cout << "\n[TEST] Adding F01mm...\n";
    std::cout << (fm.add("F01mm", "Paris", "7:00am") ? "OK" : "FAIL") << "\n";

    std::cout << "[TEST] Adding F02cc...\n";
    std::cout << (fm.add("F02cc", "Tokyo", "9:30pm") ? "OK" : "FAIL") << "\n";

    std::cout << "\n[TEST] Adding DUPLICATE F01mm...\n";
    std::cout << (fm.add("F01mm", "Paris", "8:00am") ? "OK" : "FAIL (correct)") << "\n";

    std::cout << "\n[TEST] itemExists(\"F02cc\") → "
        << (fm.itemExists("F02cc") ? "TRUE" : "FALSE") << "\n";

    std::cout << "[TEST] itemExists(\"F99xx\") → "
        << (fm.itemExists("F99xx") ? "TRUE" : "FALSE (correct)") << "\n";

    std::cout << "\n[TEST] Editing F02cc...\n";
    std::cout << (fm.edit("F02cc", "Tokyo", "10:00pm") ? "OK" : "FAIL") << "\n";

    std::cout << "\n[TEST] Removing F01mm...\n";
    std::cout << (fm.remove("F01mm") ? "OK" : "FAIL") << "\n";

    std::cout << "[TEST] Removing NON-EXISTENT F77xx...\n";
    std::cout << (fm.remove("F77xx") ? "OK?" : "FAIL (correct)") << "\n";

    std::cout << "\n=========== FINAL FREIGHT LIST ===========\n";
    for (const auto& f : fl.all())
    {
        std::cout << f.getId() << " | "
            << f.getDestination() << " | "
            << f.getTime().toString()
            << " | Capacity=" << f.getCapacity() << "\n";
    }
    std::cout << "==========================================\n";
}


// MAIN — Runs ALL TESTS 
int main()
{
    std::cout << "\n================ DEBUG MODE ================\n";

    testBruteForceMatching();
    testStrategyScheduling();
    testCargoManager();
    testFreightManager();

    std::cout << "\n============== END OF DEBUG ==============\n";
    return 0;
}
