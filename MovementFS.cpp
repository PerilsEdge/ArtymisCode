#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
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

void on_center_button() {
  static bool pressed = false;
  pressed = !pressed;
  if (pressed) {
    pros::lcd::set_text(2, "I was pressed!");
  } else {
    pros::lcd::clear_line(2);
  }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Slay");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
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

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
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
