#ifndef __DIFF_DRIVE_AGENT__H
#define __DIFF_DRIVE_AGENT__H 

#include <random>
#include <iostream>
#include "enviro.h"

using namespace enviro;
using std::cout;
using std::endl;

// A class that model the abstacles that could exist randomly
class Block : public Agent {
    public:

    // Ctor
    Block(json spec, World& world) : Agent(spec, world) {}

    // Randomly set the number of abstacles, and their position and angle
    void start() {
        if ( get_id() == 6 ) {
            std::random_device rd;  // Obtain a random number from hardware
            std::mt19937 gen(rd());  // Seed the generator
            std::uniform_int_distribution<> add_num_distr(3, 5);  // Randomly get the obstacle number
            int add_num = add_num_distr(gen);

            std::uniform_real_distribution<> xy_distr(-180, 180);
            std::uniform_real_distribution<> theta_distr(0, 4 * 1.5707963);
            for (int i = 0; i < add_num; i++) {
                // Randomly get the obstacle position and angle
                double x = xy_distr(gen);
                double y = xy_distr(gen);
                double theta = theta_distr(gen);
                Agent& new_thing = add_agent("Block", x, y, theta, {});
                cout << "Group " << ": (x, y, theta) = (" << x << ", " << y << ", " << theta << ")" << endl;
            }
        }
    }

};

DECLARE_INTERFACE(Block)

#endif