#include "ArrivalSortStrategy.h"

#include "Cargo.h"
#include "Freight.h"
#include "FreightList.h"
#include "ScheduleList.h"
#include "Schedule.h"
#include "Time.h"

#include <vector>
#include <map>
#include <limits>
#include <cmath>
#include <iostream>
#include <algorithm>

// ArrivalSortStrategy:
// Also respects time + destination + capacity, but when there are multiple valid freights,
// it chooses the freight whose departure time is closest to the cargo's required time.
// This gives a "time-priority" schedule compared to capacity-based packing.
void ArrivalSortStrategy::generate(
    const std::map<std::string, std::vector<Cargo>>& groupedCargos,
    FreightList& freightList,
    ScheduleList& scheduleList)
{
    // Start fresh
    scheduleList.clear();

    // to keep count of number of matches
    int count = 0; //initialized to 0

	// fetch all freights. Cannot modify them directly.
    const auto& freights = freightList.all();      // const std::vector<Freight>&

    // Track remaining capacity for each freight
    std::vector<int> remainingCapacity(freights.size());

    for (std::size_t i = 0; i < freights.size(); ++i)
        remainingCapacity[i] = freights[i].getCapacity();


	// loop through cargos grouped by destination (same destination per group).
    for (const auto& pair : groupedCargos) {
        const std::string& destination = pair.first;
        const std::vector<Cargo>& cargos = pair.second;

        // Debug
        std::cout << "\n[ArrivalSortStrategy] Destination: " << destination << " | Cargos : ";
        for (const auto& cargo : cargos) {
            std::cout << cargo.getId()
                << "@" << cargo.getTime().toString()
                << " (Size=" << cargo.getGroupSize() << ") ";
        }

        std::cout << std::endl;

        // Get indices of freights going to this destination
        std::vector<size_t> freightIndices;
        for (size_t i = 0; i < freights.size(); ++i)
            if (freights[i].getDestination() == destination)
                freightIndices.push_back(i);

        // Sort freight indices by arrival time ascending
        std::sort(freightIndices.begin(), freightIndices.end(),
            [&](size_t a, size_t b) {
                return freights[a].getTime().toMinutes() < freights[b].getTime().toMinutes();
            });

        // Sort cargos by their time ascending
        std::vector<Cargo> sortedCargos = cargos;
        std::sort(sortedCargos.begin(), sortedCargos.end(),
            [](const Cargo& a, const Cargo& b) {
                return a.getTime().toMinutes() < b.getTime().toMinutes();
            });
      
        // Try to assign each cargo to the "closest in time" freight.
        for (const auto& cargo : sortedCargos) {
            int remainingCargo = cargo.getGroupSize(); // track unscheduled portion
            bool assignedAny = false;

            // Get indices of freights for this destination, sorted by arrival time
            std::vector<size_t> freightIndices;
            for (size_t i = 0; i < freights.size(); ++i)
                if (freights[i].getDestination() == destination)
                    freightIndices.push_back(i);

            std::sort(freightIndices.begin(), freightIndices.end(),
                [&](size_t a, size_t b) {
                    return freights[a].getTime().toMinutes() < freights[b].getTime().toMinutes();
                });

			// check all freights for this cargo
            //for (std::size_t i = 0; i < freights.size() && remainingCargo>0; ++i) {
            for (size_t idx : freightIndices) {
                const auto& freight = freights[idx];

                // 1) Check base matching rules via Schedule::isMatch():
               //    - same destination
               //    - freight time is within allowed time window for the cargo.
                Schedule temp(freight, cargo);
                if (!temp.isMatch()) {
                    continue;
                }

				// 2)check: freight must have enough space left for this cargo
                int rem = remainingCapacity[idx];
                if (rem <= 0) {
                    continue;
                }

                int assignSize = std::min(rem, remainingCargo); // split if needed
                if (assignSize <= 0) continue;
                remainingCapacity[idx] -= assignSize;
                remainingCargo -= assignSize;
                assignedAny = true;

                Cargo partialCargo = cargo;       // copy
                partialCargo.setGroupSize(assignSize); // adjust group size for this match

                scheduleList.add(Schedule(freight, partialCargo));

                count++;
                std::cout << "[ArrivalSortStrategy Match #" << count << "] Scheduled "
                    << partialCargo.getId() << " with Freight " << freight.getId()
                    << " | Assigned " << assignSize
                    << ", Remaining Capacity: " << remainingCapacity[idx] << "\n";

                if (remainingCargo == 0) break;

                /*if (!assignedAny) {
                    std::cout << "[ArrivalSortStrategy] No suitable freight found for Cargo "
                        << cargo.getId() << "\n";
                }*/

            }
            if (!assignedAny) {
                std::cout << "[ArrivalSortStrategy] No suitable freight found for Cargo "
                    << cargo.getId() << "\n";
            }
        }
    }

    std::cout << "\nArrivalSortStrategy: Generated "
        << scheduleList.all().size() << " schedules.\n\n";
}
