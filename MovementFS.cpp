#include "main.h"
#include "pros/adi.h"
#include "pros/adi.hpp"
#include "pros/imu.h"
#include "pros/imu.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"
#include <type_traits>

//using pros::E_CONTROLLER_ANALOG_LEFT_X;
using pros::E_CONTROLLER_ANALOG_LEFT_Y;
using pros::E_CONTROLLER_ANALOG_RIGHT_X;
using pros::E_CONTROLLER_DIGITAL_R1;
using pros::E_CONTROLLER_DIGITAL_R2;
using pros::E_CONTROLLER_DIGITAL_L1;
using pros::E_CONTROLLER_DIGITAL_L2;
using pros::E_CONTROLLER_DIGITAL_A;
using pros::E_CONTROLLER_DIGITAL_B;
using pros::E_CONTROLLER_DIGITAL_UP;
//using pros::E_CONTROLLER_DIGITAL_DOWN;
using pros::E_MOTOR_ENCODER_DEGREES;
using pros::E_MOTOR_GEARSET_36;
using pros::E_MOTOR_GEARSET_18;
using pros::E_MOTOR_GEARSET_06;
// using namespace pros;
//using pros::IMU;

void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Slay");

}


void competition_initialize() {}

pros::Motor LF (10, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
pros::Motor RF (8, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
pros::Motor LB (16, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
pros::Motor RB (11, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
pros::Motor Expansion (1, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES);
pros::Motor Conveyor (17, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
pros::Motor Shooter1 (19, E_MOTOR_GEARSET_36, false, E_MOTOR_ENCODER_DEGREES);
pros::Motor Shooter2 ( 5, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
pros::Motor_Group Wheels ({LF, RF, LB, RB});
pros::Motor_Group Shooter ({Shooter1, Shooter2});

pros::ADIDigitalOut Pneumatics ('A');
//pros::IMU inertial (12);

pros::Controller master (pros::E_CONTROLLER_MASTER);

bool pneu = false;

void autonomous() {
  bool yes = true;
  //Set on the roller
  while (yes) {
        //move back
        RB.move_velocity(-75);
        LB.move_velocity(-75);
        pros::delay(500);
        Wheels.move_velocity(0);

        //roller upward
        Conveyor.move_velocity(-75);
        pros::delay(200);
        Conveyor.move_velocity(0);

        //move forward
        RB.move_velocity(75);
        LB.move_velocity(75);
        pros::delay(250);
        RB.move_velocity(0);

        //turn 90* right
        RF.move_velocity(-75);
        LB.move_velocity(75);
        pros::delay(800);
        Wheels.move_velocity(0);

        //move forward
        Wheels.move_velocity(100);
        pros::delay(2500);
        Wheels.move_velocity(0);

        //Conveyer out
        Conveyor.move_velocity(280);
        pros::delay(2400);
        Conveyor.move_velocity(0);
        yes = false;
      }

/*
  //Set off of the Roller
    while (yes) {
        //move forward
        Wheels.move_velocity(-100);
        pros::delay(1200);
        Wheels.move_velocity(0);

        //turn 90* right
        RF.move_velocity(-75);
        LB.move_velocity(75);
        pros::delay(800);
        Wheels.move_velocity(0);

        //move forward
        RB.move_velocity(-75);
        LB.move_velocity(-75);
        pros::delay(700);
        Wheels.move_velocity(0);

        //roller upward
        Roller.move_velocity(-75);
        pros::delay(300);
        Roller.move_velocity(0);

        yes = false;
      }
*/

}


void opcontrol()
{

    while (true)
    {
    // Y-axis control(acceleration)
    LF.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) * 250 / 127);
    RF.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) * 250 / 127);
    LB.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) * 250 / 127);
    RB.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) * 250 / 127);

    // X-axis control(turning)
    if (master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) <= -10) {
      LF.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) * 250 / 127);
      RB.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) * -250 / 127);
    } else if (master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) >= 10) {
      RF.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) * -250/ 127);
      LB.move_velocity(master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) * 250 / 127);
    }

    // Conveyor+Roller Movement
    if (master.get_digital(E_CONTROLLER_DIGITAL_R1)) {
      Conveyor.move_velocity(195);
    } else if (master.get_digital(E_CONTROLLER_DIGITAL_R2)) {
      Conveyor.move_velocity(-195);
    } else {
      Conveyor.move_velocity(0);
    }

    //Shooter Movement
    if (master.get_digital(E_CONTROLLER_DIGITAL_L1)) {
      Shooter1.move_velocity(550);
      Shooter2.move_velocity(195);
      if (master.get_digital(E_CONTROLLER_DIGITAL_L2)) {
        Pneumatics.set_value(true);   
        pros::delay(1000);
        Pneumatics.set_value(false);
      }
    } else if (master.get_digital(E_CONTROLLER_DIGITAL_UP)) {
      Shooter.move_velocity(0);
    }

    //Expansion
    if (master.get_digital(E_CONTROLLER_DIGITAL_A)) {
        Expansion.move_velocity(95);
    } else if (master.get_digital(E_CONTROLLER_DIGITAL_B)) {
        Expansion.move_velocity(-95);
    } else {
        Expansion.move_velocity(0);
    }

      pros::delay(20);
    }
}
