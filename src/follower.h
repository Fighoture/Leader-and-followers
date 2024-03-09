#ifndef _FOLLOWER_H_
#define _FOLLOWER_H_

#include <iostream>
#include <chrono>
#include <string>
#include "enviro.h"

using namespace enviro;
using std::cout;
using std::endl;
using std::string;

// A class the model followers
class FollowerController : public Process, public AgentInterface {

    public:

    // Ctor
    FollowerController() : Process(), AgentInterface() {}

    // Define the watch target and watch conresponding event
    void init() {
        if (id() == 1) {  // First follower should follow leader
            watch_name = "leader_position";
            emit_name = "agent_1_position";
        }
        else {  // Other followers should follow their precedent
            watch_name = "agent_" + std::to_string(id() - 1) + "_position";
            emit_name = "agent_" + std::to_string(id()) + "_position";
        }
        cout << "id: " << id() << endl;
        cout << "watch_name: " << watch_name << endl;
        cout << "emit_name: " << emit_name << endl;

        target_x = 0;
        target_y = 0;
        watch(watch_name, [this](Event e) { // Watching the followed postion 
            target_x = e.value()["x"];
            target_y = e.value()["y"];
        });
    }

    void start() {}

    // Emit its position and follower their precedents
    void update() { 
        emit(Event(emit_name, { // Emit the position to followers
            { "x", position().x },
            { "y", position().y }
        }));
        if (sensor_value(0) < 30 || sensor_value(1) < 30 || sensor_value(2) < 30) {  // Follower has been close to precedent or obstacles
            track_velocity(-5, rand() % 2 == 0 ? 4 : -4);  // Backwards and rotate ramdomly
        } else {
            move_toward(target_x, target_y);
        }
    }
    void stop() {}

    double target_x, target_y;
    int last_agent_id;
    string watch_name;
    string emit_name;
};

class Follower : public Agent {
    public:
    Follower(json spec, World& world) : Agent(spec, world) {
        add_process(fc);
    }
    private:
    FollowerController fc;
};

DECLARE_INTERFACE(Follower)

#endif  // _FOLLOWER_H_