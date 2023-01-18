#include "main.h"

// using pros::E_CONTROLLER_ANALOG_LEFT_X;
using pros::E_CONTROLLER_ANALOG_LEFT_Y;
using pros::E_CONTROLLER_ANALOG_RIGHT_X;
using pros::E_CONTROLLER_DIGITAL_R1;
using pros::E_CONTROLLER_DIGITAL_R2;
using pros::E_CONTROLLER_DIGITAL_L1;
using pros::E_CONTROLLER_DIGITAL_L2;
using pros::E_CONTROLLER_DIGITAL_A;
using pros::E_CONTROLLER_DIGITAL_B;
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
pros::Motor Conveyer (5, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES);
pros::Motor Roller (19, E_MOTOR_GEARSET_36, false, E_MOTOR_ENCODER_DEGREES);
pros::Motor Spool ( 1, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES);

pros::Controller master (pros::E_CONTROLLER_MASTER);

void autonomous() {

  while (true) {
    LB.move_relative(50,100); //move down
    Conveyer.move_relative(20,150); //moves the conveyor
    RB.move_relative(150,100); //move up
    RF.move_relative(10,100); //move left
    LF.move_relative(120,100); //move right
  }

}


void opcontrol() 
{
    while (true) 
    {
    // Y-axis control(acceleration)
    RF.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) * 250 / 127);
    LF.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) * 250 / 127);
    RB.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) * 250 / 127);
    LB.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) * 250 / 127);

    // X-axis control(turning)
    if (master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) <= -10) {
      LF.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) * 300 / 127);
      RB.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) * -300 / 127);
    } else if (master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) >= 10) {
      RF.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) * -300/ 127);
      LB.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) * 300 / 127);
    }

    // Conveyer Movement
    if (master.get_digital(E_CONTROLLER_DIGITAL_R1)) {
      Conveyer.move_velocity(-550);
    } else if (master.get_digital(E_CONTROLLER_DIGITAL_R2)) {
      Conveyer.move_velocity(550);
    } else {
      Conveyer.move_velocity(0);
    }

    //Roller Movement
    if (master.get_digital(E_CONTROLLER_DIGITAL_L1)) {
      Roller.move_velocity(-75);
    } else if (master.get_digital(E_CONTROLLER_DIGITAL_L2)) {
      Roller.move_velocity(75);
    } else {
      Roller.move_velocity(0);
    }

    //Expansion Spool
    if (master.get_digital(E_CONTROLLER_DIGITAL_A)) {
      Spool.move_velocity(400);
    } else if (master.get_digital(E_CONTROLLER_DIGITAL_B)) {
      Spool.move_velocity(-400);
    } else {
      Spool.move_velocity(0);
    }

      pros::delay(20);
    }
}
