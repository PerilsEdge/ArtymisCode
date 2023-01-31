#include "main.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"

// using pros::E_CONTROLLER_ANALOG_LEFT_X;
using pros::E_CONTROLLER_ANALOG_LEFT_Y;
using pros::E_CONTROLLER_ANALOG_RIGHT_X;
using pros::E_CONTROLLER_DIGITAL_R1;
using pros::E_CONTROLLER_DIGITAL_R2;
using pros::E_CONTROLLER_DIGITAL_L1;
using pros::E_CONTROLLER_DIGITAL_L2;
using pros::E_CONTROLLER_DIGITAL_A;
using pros::E_CONTROLLER_DIGITAL_B;
//using pros::E_CONTROLLER_DIGITAL_UP;
//using pros::E_CONTROLLER_DIGITAL_DOWN;
using pros::E_MOTOR_ENCODER_DEGREES;
using pros::E_MOTOR_GEARSET_36;
using pros::E_MOTOR_GEARSET_18;
using pros::E_MOTOR_GEARSET_06;


void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Slay");

}

void disabled() {}

void competition_initialize() {}

pros::Motor LF(10, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
pros::Motor RF (8, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
pros::Motor LB (16, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
pros::Motor RB (11, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
pros::Motor Conveyor (1, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES);
pros::Motor Roller (19, E_MOTOR_GEARSET_36, false, E_MOTOR_ENCODER_DEGREES);
pros::Motor Spool ( 5, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES);

pros::Controller master (pros::E_CONTROLLER_MASTER);

void autonomous() {
  bool yes = true;
  while (yes) {
        //move back
        RB.move_velocity(-75);
        LB.move_velocity(-75);
        pros::delay(200);
        RB.move_velocity(0);
        LB.move_velocity(0);

        //roller upward
        Roller.move_velocity(-95);
        pros::delay(780);
        Roller.move_velocity(0);
/*
        //move forward
        RB.move_velocity(75);
        LB.move_velocity(75);
        pros::delay(150);
        RB.move_velocity(0);

        //turn 90* right
        RF.move_velocity(-75);
        pros::delay(820);

        //move forward
        RF.move_velocity(100);
        LF.move_velocity(100);
        RB.move_velocity(100);
        LB.move_velocity(100);
        pros::delay(2800);
        RF.move_velocity(0);
        LF.move_velocity(0);
        RB.move_velocity(0);
        LB.move_velocity(0);

        Conveyer.move_velocity(-200);
    */    
        yes = false;
      }
}


void opcontrol() 
{

    while (true) 
    {    
    // Y-axis control(acceleration)
    RF.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) * -250 / 127);
    LF.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) * -250 / 127);
    RB.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) * -250 / 127);
    LB.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) * -250 / 127);

    // X-axis control(turning)
    if (master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) <= -10) {
      LF.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) * 300 / 127);
      RB.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) * -300 / 127);
    } else if (master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) >= 10) {
      RF.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) * -300/ 127);
      LB.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) * 300 / 127);
    }

    // Expansion Spool
    if (master.get_digital(E_CONTROLLER_DIGITAL_R1)) {
      Spool.move_velocity(-450);
    } else if (master.get_digital(E_CONTROLLER_DIGITAL_R2)) {
      Spool.move_velocity(450);
    } else {
      Spool.move_velocity(0);
    }

    //Roller Movement
    if (master.get_digital(E_CONTROLLER_DIGITAL_L1)) {
      Roller.move_velocity(-75);
      //move roller up
    } else if (master.get_digital(E_CONTROLLER_DIGITAL_L2)) {
      Roller.move_velocity(75);
      //move roller down
    } else {
      Roller.move_velocity(0);
    }

    // Conveyor Movement
    if (master.get_digital(E_CONTROLLER_DIGITAL_A)) {
      Conveyor.move_velocity(550);
    } else if (master.get_digital(E_CONTROLLER_DIGITAL_B)) {
      Conveyor.move_velocity(-550);
    } else {
      Conveyor.move_velocity(0);
    }

      pros::delay(20);
    }
}
