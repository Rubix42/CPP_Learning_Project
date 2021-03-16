#include "aircraft_manager.hpp"

#include <utility>

void AircraftManager::add(std::unique_ptr<Aircraft> aircraft)
{
    aircrafts.emplace_back(std::move(aircraft));
}

bool AircraftManager::update()
{
    for(auto it = aircrafts.begin(); it != aircrafts.end();){
        auto& aircraft = **it;
        if (aircraft.update())
        {
            ++it;
        }
        else
        {
            it = aircrafts.erase(it);
        }
    }
    return true;
}