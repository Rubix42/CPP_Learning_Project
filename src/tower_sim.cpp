#include "tower_sim.hpp"

#include "GL/opengl_interface.hpp"
#include "aircraft.hpp"
#include "airport.hpp"
#include "config.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>

using namespace std::string_literals;

const std::string airlines[8] = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "BW" };

TowerSimulation::TowerSimulation(int argc, char** argv) :
    help { (argc > 1) && (std::string { argv[1] } == "--help"s || std::string { argv[1] } == "-h"s) }
{
    MediaPath::initialize(argv[0]);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    GL::init_gl(argc, argv, "Airport Tower Simulation");

    create_keystrokes();

    GL::move_queue.emplace(&manager);
}

TowerSimulation::~TowerSimulation()
{
    delete airport;
}

std::unique_ptr<Aircraft> TowerSimulation::create_aircraft(const AircraftType& type) const
{
    assert(airport); // make sure the airport is initialized before creating aircraft

    const std::string flight_number = airlines[std::rand() % 8] + std::to_string(1000 + (rand() % 9000));
    const float angle       = (rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and 2pi
    const Point3D start     = Point3D { std::sin(angle), std::cos(angle), 0 } * 3 + Point3D { 0, 0, 2 };
    const Point3D direction = (-start).normalize();

    return std::make_unique<Aircraft>(type, flight_number, start, direction, airport->get_tower());
}

std::unique_ptr<Aircraft> TowerSimulation::create_random_aircraft() const
{
    return create_aircraft(*(aircraft_types[rand() % 3]));
}

void TowerSimulation::display_stats(int airline)
{
    std::cout << "Number of Aicrafts on the airline : " + airlines[airline]
        << " " << manager.count_airline(airlines[airline]) << std::endl;
}

void TowerSimulation::create_keystrokes()
{
    GL::keystrokes.emplace('0', [this]() { display_stats(0); });
    GL::keystrokes.emplace('1', [this]() { display_stats(1); });
    GL::keystrokes.emplace('2', [this]() { display_stats(2); });
    GL::keystrokes.emplace('3', [this]() { display_stats(3); });
    GL::keystrokes.emplace('4', [this]() { display_stats(4); });
    GL::keystrokes.emplace('5', [this]() { display_stats(5); });
    GL::keystrokes.emplace('6', [this]() { display_stats(6); });
    GL::keystrokes.emplace('7', [this]() { display_stats(7); });
    GL::keystrokes.emplace('x', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('q', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('c', [this]() { manager.add(create_random_aircraft()); });
    GL::keystrokes.emplace('+', []() { GL::change_zoom(0.95f); });
    GL::keystrokes.emplace('-', []() { GL::change_zoom(1.05f); });
    GL::keystrokes.emplace('f', []() { GL::toggle_fullscreen(); });
    GL::keystrokes.emplace('$', []() { GL::ticks_per_sec = std::max(GL::ticks_per_sec - 1u,1u);});
    GL::keystrokes.emplace('*', []() { GL::ticks_per_sec = std::min(GL::ticks_per_sec + 1u,180u);});
    GL::keystrokes.emplace('p', []() { GL::is_paused = !GL::is_paused; });
}

void TowerSimulation::display_help() const
{
    std::cout << "This is an airport tower simulator" << std::endl
              << "the following keysstrokes have meaning:" << std::endl;

    for (const auto& [ks, value] : GL::keystrokes)
    {
        std::cout << ks << ' ';
    }

    std::cout << std::endl;
}

void TowerSimulation::init_airport()
{
    airport = new Airport { one_lane_airport, Point3D { 0, 0, 0 },
                            new img::Image { one_lane_airport_sprite_path.get_full_path() } };

    GL::move_queue.emplace(airport);
}

void TowerSimulation::launch()
{
    if (help)
    {
        display_help();
        return;
    }

    init_airport();
    init_aircraft_types();

    GL::loop();
}
