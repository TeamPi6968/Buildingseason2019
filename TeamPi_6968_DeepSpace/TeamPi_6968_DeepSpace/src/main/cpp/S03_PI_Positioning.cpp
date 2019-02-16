#include "S03_PI_Positioning.h"

S03_PI_Positioning::S03_PI_Positioning(C00_PI_Talon *leftEnc, C00_PI_Talon *rightEnc, double drivetrainWidth)
{
    _lEnc = leftEnc;
    _rEnc = rightEnc;

    _pos = new PiTransform();

    _drivetrainWidth = drivetrainWidth;

    this->totalDistance = 0;
}

void S03_PI_Positioning::refresh()
{
    double rDist = -_rEnc->deltaDistance(); //one of the sides needs to be reversed
    double lDist = _lEnc->deltaDistance();

    //find the distance
    double distance = (rDist - lDist) / 2 + lDist;
    double tempAngle = atan((rDist - lDist) / this->_drivetrainWidth);

    tempAngle = tempAngle * (180.0 / M_PI);
    //add the angle
    this->_pos->rotation->z += tempAngle;

    //normalize the angle to 0,359
    this->_pos->rotation->z = (int)(this->_pos->rotation->z + 360) % 360;

    //x-y position:
    this->_pos->position->y += cos((this->_pos->rotation->z * M_PI / 180)) * distance;
    this->_pos->position->x += sin((this->_pos->rotation->z * M_PI / 180)) * distance;

    //add the distance to the total distance:
    totalDistance += distance;
}

PiTransform *S03_PI_Positioning::Get()
{
    return this->_pos;
}

double S03_PI_Positioning::getDistance(){
    totalDistance;
}
void S03_PI_Positioning::reset(PiTransform newPos)
{
    *_pos = newPos;
}

void S03_PI_Positioning::reset()
{
    _pos->position->x = 0;
    _pos->position->y = 0;
    _pos->position->z = 0;
    _pos->rotation->x = 0;
    _pos->rotation->y = 0;
    _pos->rotation->z = 0;
}