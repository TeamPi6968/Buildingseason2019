#include "S04_PI_Drivetrain.h"
#include <iostream>
#include "cmath"
/* to add to drive train this is the open loop or closed loop command for ramping the talons (smooth acceleration) 
    if using it make sure you only use it for the master and give the followers ther own ramp up and put it in th open or clossed loop of the drive train system 
    TalonR->getTalonObject->configOpenloopRamp([Time to ramp up], [amount of follwors]); // {time to ramp up in seconds, this is the time it takes to go to the set speed in this case 2s } 
                                                                            // [amount of followers is the amount of followers for the tallon(master)]
    TalonL->getTalonObject->configOpenloopRamp([Time to ramp up], [amount of follwors]); 
    */

S04_PI_Drivetrain::S04_PI_Drivetrain(C00_PI_Talon *talonL, C01_PI_Victor *victorL1, C01_PI_Victor *victorL2, C00_PI_Talon *talonR, C01_PI_Victor *victorR1, C01_PI_Victor *victorR2)
{
    //assign the drivers:
    _talonL = talonL;
    _talonR = talonR;
    _victorL1 = victorL1;
    _victorL2 = victorL2;
    _victorR1 = victorR1;
    _victorR2 = victorR2;

    //set the followers:
    this->_victorL1->GetVictorObject()->Follow(*(_talonL->GetTalonObject()));
    this->_victorL2->GetVictorObject()->Follow(*(_talonL->GetTalonObject()));
    this->_victorR1->GetVictorObject()->Follow(*(_talonR->GetTalonObject()));
    this->_victorR2->GetVictorObject()->Follow(*(_talonR->GetTalonObject()));

    this->_diffDrive = new frc::DifferentialDrive(*(_talonL->GetTalonObject()), *(_talonR->GetTalonObject()));

    usingPositioning = false;
}

S04_PI_Drivetrain::S04_PI_Drivetrain(C00_PI_Talon *talonL, C01_PI_Victor *victorL1, C01_PI_Victor *victorL2, C00_PI_Talon *talonR, C01_PI_Victor *victorR1, C01_PI_Victor *victorR2, S03_PI_Positioning *robotPos)
{
    S04_PI_Drivetrain(talonL, victorL1, victorL2, talonR, victorR1, victorR2);
    usingPositioning = true;
    this->robotPos = robotPos;

    input = new PI_PIDSource();
    output = new PI_PIDOutput();

    pidRotation = new frc::PIDController(KP_R, KI_R, KD_R, input, output);
    pidRotation->Disable();//don;t run it until needed.
}

void S04_PI_Drivetrain::drive(double speed, double rotation)
{
    _diffDrive->ArcadeDrive(speed, rotation);
}

bool S04_PI_Drivetrain::Rotate(double angle)
{
    if (!usingPositioning)
    {
        std::cout << "ERROR: Cannot auto rotate. Drive train has not been initialized with a positioning object \n";
        return false;
    }
    if(angle<-360||angle>360){
        std::cout<< "ERROR: Angle out of bounds. range is (0,360) \n";
        return false;
    }
    //if first call:
    if (!pidRotationStarted)
    {
        pidRotationStarted = true;
        targetAngle = robotPos->Get()->rotation->z + angle;
        //normalize the target to [0,360]
        targetAngle = normalizeAngle(targetAngle);

        //determine which way should the robot turn to get there the quickest:
        if(abs(targetAngle-robotPos->Get()->rotation->z)<180){
            //turn counter clockwise
            turnDirection=-1;
        }
        else{
            //turn clockwise;
            turnDirection=1;
        }

        //start the pid loop:
        pidRotation->Enable();
    }

    targetAngle = (int)(targetAngle + 360) % 360;
    //calculate the angle error:
    input->Set(abs(targetAngle-robotPos->Get()->rotation->z));
    
    //write the output of the pid loop to the drivetrain:
    drive(output->Get(),turnDirection);


    //chek if it got there
    if(abs(targetAngle-robotPos->Get()->rotation->z)<errorTolerance){
        //disable the loop and let the caller know that we got there
        pidRotation->Reset();
        pidRotationStarted = false;
        return true;
    }

    //determine the best direction to rotate in (1 = CW, 0 = CCW):
    /*double diff = (angle - robotPos->Get()->rotation->z);
    bool dir = ((diff > 0 && diff > 180) || (diff < 0 && diff < 180));
    int dirVector = dir ? 1 : -1;
*/

    return false;
}

//normalizes the angle to (0,359);
double S04_PI_Drivetrain::normalizeAngle(double angle){
    angle = (int)(angle + 360) % 360;
    return angle;
}

bool S04_PI_Drivetrain::driveDist(double distance){
    if(!autoDriveStarted){
        autoDriveStarted=true;
        targetDistance =  robotPos->getDistance() + distance;
    }

    double diff = targetDistance-robotPos->getDistance();
    int dist =  diff/(abs(diff));

    drive(dist*autoDriveSpeed,0);

    if(abs(targetDistance-robotPos->getDistance())<1){
        autoDriveStarted=false;
        return true;
    }
    return false;
}