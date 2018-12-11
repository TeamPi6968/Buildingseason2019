/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <IterativeRobot.h>
#include <Joystick.h>
#include <WPILib.h>
#include <CameraServer.h>

#include <DigitalInput.h>
#include <DigitalOutput.h>

//Our own libraries
#include <iostream>
#include "PiMovement.h"
#include "PiPowerUp.h"
#include "PiPosition.h"
#include "PiEncoder.h"
#include "PiUltrasoon.h"

class Robot: public frc::IterativeRobot {
public:
	//driving:
	PiMovement *piMovement = new PiMovement();

	WPI_TalonSRX * _rEncoder = new WPI_TalonSRX(4);
	WPI_TalonSRX * _lEncoder = new WPI_TalonSRX(2);

	//positioning:
	const double wheelRadius = 76.2f;			//random value for now
	PiEncoder *piEncoder = new PiEncoder(_lEncoder, _rEncoder, wheelRadius);
	PiPosition *position = new PiPosition(piEncoder);

	//functions:
	PiPowerUp *power = new PiPowerUp();
	PiUltrasoon *Ultra1 = new PiUltrasoon(6, 7);

	//tele op:
	frc::Joystick m_stick { 0 };
	frc::Joystick boxStick { 1 };	//second controller for box pickup
	//speed reduction:
	double speedReductionFactor = 0.7;

	//box pickup:
	bool armState = false, lastButtonValue = false;

	void TeleopPeriodic() {
		// drive with arcade style
		piMovement->move(m_stick.GetY() * speedReductionFactor,
				m_stick.GetZ() * 0.7);

		//box intake:
		intakeSystem();

		// utlrasonic sesnor stuf
		/*double c = Ultra1->UltrasoonValue(1, 20);
		 std::cout << "This is the distance in front of ultra1: " << c
		 << std::endl;
		 */

		/*if (leftRPM || rightRPM)
			std::cout << "Left RPM: " << leftRPM << " Right RPM: " << rightRPM
					<< "\n";
		*/
		position->updatePosition();

		std::cout << "Distance travelled: " << position->getDistance()<<"\n";
		std::cout<< "angle: "<<position->Get()->rotation->z<<'\n';
		std::cout<<"coordinates: "<<position->Get()->position->x<<" , "<<position->Get()->position->y<<"\n";

	}

	void RobotInit() {
		piMovement->init();
		CameraServer::GetInstance()->StartAutomaticCapture();
	}

	void intakeSystem() {
		//all the box itake stuff and pneumatics here:

		//handle the intake system for the box
		power->moveBox(boxStick.GetY());
		power->intakeBox(boxStick.GetY());

		//open close arms:
		bool buttonValue = boxStick.GetRawButton(1);

		if (buttonValue == true) {
			armState = !armState;

			//only do this once:
			if (lastButtonValue == false) {
				std::cout << "The state of the arms is" << armState;
				if (armState) {
					//if 1, then open:
					power->openPistons();
				} else {
					//close:
					power->closePistons();
				}
			}
			lastButtonValue = true;

		} else {
			lastButtonValue = false;
		}
	}
};

START_ROBOT_CLASS(Robot)
