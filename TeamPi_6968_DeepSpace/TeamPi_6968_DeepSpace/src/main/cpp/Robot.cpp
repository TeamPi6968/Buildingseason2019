/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Robot.h"
//#include <frc/livewindow/LiveWindow.h>

#include <iostream>
#include <frc/WPILib.h>
#include <frc/smartdashboard/SmartDashboard.h>


//frc::I2C *I2CBus;
S01_PI_Sensors *sensors;

int count = 0;

int LiftLevel=0;

void Robot::RobotInit()
{
  m_chooser.SetDefaultOption(kAutoNameDefault, kAutoNameDefault);
  m_chooser.AddOption(kAutoNameCustom, kAutoNameCustom);
  frc::SmartDashboard::PutData("Auto Modes", &m_chooser);

  //sensors:
  sensors = new S01_PI_Sensors(); //check the cpp file for sensor definitions

  //input:
  input = new S02_PI_Input();

  //NetworkTable = new S00_PI_Network();
  lift = new S05_PI_Lift(7);  //create a lift using the talon on CAN 7

  //manual:
  manual = new M00_PI_Manual(input, lift);

}

/**
 * This function is called every robot packet, no matter the mode. Use
 * this for items like diagnostics that you want ran during disabled,
 * autonomous, teleoperated and test.
 *
 * <p> This runs after the mode specific periodic functions, but before
 * LiveWindow and SmartDashboard integrated updating.
 */
void Robot::RobotPeriodic() {}

/**
 * This autonomous (along with the chooser code above) shows how to select
 * between different autonomous modes using the dashboard. The sendable chooser
 * code works with the Java SmartDashboard. If you prefer the LabVIEW Dashboard,
 * remove all of the chooser code and uncomment the GetString line to get the
 * auto name from the text box below the Gyro.
 *
 * You can add additional auto modes by adding additional comparisons to the
 * if-else structure below with additional strings. If using the SendableChooser
 * make sure to add them to the chooser code above as well.
 */
void Robot::AutonomousInit()
{
  m_autoSelected = m_chooser.GetSelected();
  // m_autoSelected = SmartDashboard::GetString(
  //     "Auto Selector", kAutoNameDefault);
  std::cout << "Auto selected: " << m_autoSelected << std::endl;

  if (m_autoSelected == kAutoNameCustom)
  {
    // Custom Auto goes here
  }
  else
  {
    // Default Auto goes here
  }
}

void Robot::AutonomousPeriodic()
{
  //std::cout << "main function\n";
  if (m_autoSelected == kAutoNameCustom)
  {
    // Custom Auto goes here
  }
  else
  {
    // Default Auto goes here
  }
}

void Robot::TeleopInit() {


}

void Robot::TeleopPeriodic()
{
  sensors->refresh();

  //manual stuff:
  manual->driving();
  manual->functions();

  if (count == 100)
  {
    //execute code in here roughly once a second.



    //switch between the levels:
    //lift->goToLvl(LiftLevel);

    LiftLevel=(LiftLevel+1)%3; //loop


    //pixy->Update();
    //std::cout << pixy->latestVector->x0;
    //test the Ultrasound sensors:
    //std::cout << "Current: " << sensors->USLeft->getCurrent() << '\n';
    //std::cout << "Distance: " << sensors->USLeft->getDist() << '\n';

    //test arduno communication :
    /*uint8_t data[2];
    if(arduino->read(data,2))
      std::cout<<"Read error \n";

    for(int i=0;i<2;i++){
      std::cout<<data[i];
    }
    std::cout<<"\n";
    */
  }
  count = (count + 1) % 200;
}

void Robot::TestPeriodic()
{
}

#ifndef RUNNING_FRC_TESTS
int main()
{
  return frc::StartRobot<Robot>();
}
#endif
