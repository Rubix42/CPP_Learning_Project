#include "aircraft_manager.hpp"

#include <utility>

void AircraftManager::add(std::unique_ptr<Aircraft> aircraft)
{
    aircrafts.emplace_back(std::move(aircraft));
}

int AircraftManager::count_airline(const std::string& airline)
{
    return std::count_if(
        aircrafts.begin(),
        aircrafts.end(),
        [airline](const std::unique_ptr<Aircraft>& aircraft) { return aircraft->get_flight_num().find(airline) == 0; }
        );
}

bool AircraftManager::update()
{
    /*
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
    */
    aircrafts.erase(
        std::remove_if(
            aircrafts.begin(),
            aircrafts.end(),
            [](const std::unique_ptr<Aircraft>& aircraft){ return !aircraft->update(); }),
        aircrafts.end());
    return true;
}