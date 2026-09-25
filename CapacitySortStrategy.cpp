#include <vector>
#include <memory>
#include <map>
#include <limits>
#include <iostream>

#include "CapacitySortStrategy.h"
#include "Cargo.h"
#include "Freight.h"
#include "FreightList.h"
#include "ScheduleList.h"
#include "Schedule.h"
#include <algorithm>

// CapacitySortStrategy:
// Tries to pack cargos onto freights by prioritising freights
// that will have the smallest remaining capacity after assignment.
// Still respects time + destination rules via Schedule::isMatch().
void CapacitySortStrategy::generate(
    const std::map<std::string, std::vector<Cargo>>& groupedCargos,
    FreightList& freightList,
    ScheduleList& scheduleList)
{
    // Start fresh
    scheduleList.clear();

    //to keep count of matches 
    int count = 0; // initialized to 0

    //Get all freights. Cannot modify them directly.
    const auto& freights = freightList.all();      // const std::vector<Freight>&

    // We keep track of remaining capacity per freight here, using a parallel array.
    // Index i in remainingCapacity matches index i in freights.
    std::vector<int> remainingCapacity(freights.size());
    for (size_t i = 0; i < freights.size(); ++i)
        remainingCapacity[i] = freights[i].getCapacity();

    // Process cargos by destination
    for (const auto& pair : groupedCargos) {
        const std::string& destination = pair.first;
        std::vector<Cargo> cargos = pair.second;

        // Sort cargos by arrival time ascending
        std::sort(cargos.begin(), cargos.end(),
            [](const Cargo& a, const Cargo& b) {
                return a.getTime().toMinutes() < b.getTime().toMinutes();
            });

        // Filter freights for this destination and sort by capacity descending
        std::vector<size_t> freightIndices;
        for (size_t i = 0; i < freights.size(); ++i) {
            if (freights[i].getDestination() == destination)
                freightIndices.push_back(i);
        }
        std::sort(freightIndices.begin(), freightIndices.end(),
            [&](size_t a, size_t b) {
                return remainingCapacity[a] > remainingCapacity[b];
            });

        // Debug
        std::cout << "\n[CapacitySortStrategy] Destination: " << destination << " | Cargos: ";
       for (const auto& cargo : cargos) {
                std::cout << cargo.getId()
                    << "@" << cargo.getTime().toString()
                    << " (Size=" << cargo.getGroupSize() << ") ";
       }
       std::cout << std::endl;

       // For each cargo in this destination group…
       for (const auto& cargo : cargos) {
            int remainingCargo = cargo.getGroupSize();
            bool assignedAny = false;

            // Check all freights to find the "best" one for this cargo.
            //for (std::size_t i = 0; i < freights.size(); ++i) {
            for (size_t idx : freightIndices) {
                const auto& freight = freights[idx];

                // 1) Check if freight and cargo are compatible in terms of:
                //    - same destination
                //    - time constraint (<= 15 min early or on time)
                // This logic is encapsulated inside Schedule::isMatch().
                Schedule tempSchedule(freight, cargo);
                if (!tempSchedule.isMatch()) {       // you had this backwards earlier
                    continue;
                }

                // 2) Check if this freight still has enough remaining capacity
                //    to take this cargo group.
                int rem = remainingCapacity[idx];
                if (rem <= 0) {
                    continue;
                }

                // to determine how much of the cargo can be assigned to this freight
                // assign as much as possible without exceeding freight capacity or cargo group size
                int assignSize = std::min(rem, remainingCargo);
                remainingCapacity[idx] -= assignSize;
                remainingCargo -= assignSize;
                assignedAny = true;

                Cargo partialCargo = cargo;
                partialCargo.setGroupSize(assignSize);

                //add match to schedulelist
                scheduleList.add(Schedule(freight, partialCargo));

                //debug
                count += 1;
                std::cout << "[CapacitySortStrategy Match #" << count << "] Scheduled Cargo " << cargo.getId()
                    << " with Freight " << freight.getId()
                    << " | Remaining Capacity: " << remainingCapacity[idx]
                    << "\n";

                if (remainingCargo == 0) break; // when all cargos are assigned
            }
            if (!assignedAny) {
                //no suitable freight found for this cargo
                std::cout << "[CapacitySortStrategy] No suitable freight found for Cargo "
                    << cargo.getId() << "\n";
            }
       }
    }

    std::cout << "\nCapacitySortStrategy: Generated "
        << scheduleList.all().size() << " schedules.\n\n";
}

