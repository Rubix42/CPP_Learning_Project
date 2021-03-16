#pragma once

#include "GL/dynamic_object.hpp"
#include "aircraft.hpp"


#include <memory>
#include <vector>

class AircraftManager : public GL::DynamicObject
{
public:
    void add(std::unique_ptr<Aircraft>);

    int count_airline(const std::string& airline);

    bool update() override;
private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts;
};