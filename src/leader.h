#ifndef _LEADER_H_
#define _LEADER_H_ 

#include <iostream>
#include "enviro.h"

using namespace enviro;
using std::cout;
using std::endl;

// A class that model the leader that could move as user's manipulation with WASD
class LeaderController : public Process, public AgentInterface {

    public:

    // Ctor
    LeaderController() : Process(), AgentInterface(), UP(false), LEFT(false), DOWN(false), RIGHT(false), target(0) {}

    // Watch keyboard manipulation
    void init() {
        watch("keydown", [&](Event& e) {  // Watch keyboard press to define the moving direction and target angle
            std::string k = e.value()["key"];
            if ( k == "w") {
                UP = true;
                target = 3;
            } else if ( k == "a" ) {
                LEFT = true;
                target = 2;
            } else if ( k == "s" ) {
                DOWN = true;
                target = 1;
            } else if ( k == "d" ) {
                RIGHT = true;
                target = 0;
            } 
        });
        watch("keyup", [&](Event& e) {  // Watch keyboard lift
            std::string k = e.value()["key"];
            if ( k == "w") {
                UP = false;
            } else if ( k == "a" ) {
                LEFT = false;
            } else if ( k == "s" ) {
                DOWN = false;
            } else if ( k == "d" ) {
                RIGHT = false;
            } 
        }); 
    }

    void start() {}

    // Emit its position, rotate and move as users' want
    void update() {
        emit(Event("leader_position", {  // Emit the position to followers
            { "x", position().x },
            { "y", position().y }
        }));
        
        double angle_diff = target * 1.5707963 - angle();  // Compute the angle difference for future rotation
        while (angle_diff > 2 * 1.5707963) angle_diff -= 4 * 1.5707963;  // Make the the angle difference less than pi
        while (angle_diff < -2 * 1.5707963) angle_diff += 4 * 1.5707963;  // Make the the angle difference more than -pi

        if (abs(angle_diff) < 1e-2) {  // Leader has finished rotation and could move forward
            double fx, fy;
            if ( RIGHT ) {
                vx = VEL;
            } if ( LEFT ) {
                vx = -VEL;
            } else if ( !RIGHT && !LEFT ) {
                vx = 0;
            }
            fx = -K * (velocity().x - vx);

            if ( UP ) {
                vy = -VEL;
            } if ( DOWN ) {
                vy = VEL;
            } else if ( !UP && !DOWN ) {
                vy = 0;
            }
            fy = -K * (velocity().y - vy);

            omni_apply_force(fx, fy);
        } else {  // Leader should rotate to target direction
            track_velocity(0, abs(angle_diff)>0.05?angle_diff:(angle_diff / abs(angle_diff) * 0.05));
        }
    }
    void stop() {}

    bool UP, LEFT, DOWN, RIGHT;
    int target;  // Target direction
    double vx, vy;
    const double VEL = 5;
    const double K = 15;
};

class Leader : public Agent {
    public:
    Leader(json spec, World& world) : Agent(spec, world) {
        add_process(lc);
    }
    private:
    LeaderController lc;
};

DECLARE_INTERFACE(Leader)

#endif  // _LEADER_H_