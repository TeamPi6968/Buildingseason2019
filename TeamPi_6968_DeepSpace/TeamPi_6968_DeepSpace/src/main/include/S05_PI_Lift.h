/*
Control the lift system of the 2019 Team Pi Robot

The system works by moving the grabber carrige using a winch.
The winch is controlled using a Talon SRX with an SRX magnetic encoder.
This allows it to be controlled by a PID loop making the movement precise.

The Target positions are given in absolute motor turns.These are calibrated
and stored as constants in this file.

The start and end position of the lift can be determined using end switches.


Created by Eduard Cazacu on 11 February 2019
Team Pi 6968
*/

#pragma once

//the heights (in rotations) of all the levels from the bottom:
#define LVL1 10
#define LVL2 50
#define LVL3 100

#include "C00_PI_Talon.h"

class S05_PI_Lift{
    public:
        /*
            Description:    Constructor. Create a new lift object
            Input:          [uint8_t] talon driver CAN bus address
            Output:         none          
        */
       S05_PI_Lift(uint8_t talonCAN);

        /*
            Description:    Go to a certain position using PID
            Input:          [double] position in absolute rotations in relation to the bottom.
            Output:         none;
        */
       void goTo(double pos);

        /*
            Description:    go down until the limit switch confirms and reset the current position to 0
            Input:          none;
            Output:         none;
        */
       void reset();

    private:

    double *_pos;   //the current position of the robot.

};
