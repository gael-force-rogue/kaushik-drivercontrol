#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
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
	pros::lcd::set_text(1, "Hello PROS User!");

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
void autonomous() {}

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
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor left1(-7);
	pros::Motor left2(8);
	pros::Motor left3(-3);
	pros::Motor right1(10);
	pros::Motor right2(-12);
	pros::Motor right3(11);
	
	pros::Motor intake(-6);
	pros::ADIDigitalOut piston('H');

	while (true) {
		int leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = 0.8*master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        left1.move(leftY + rightX);
		left2.move(leftY + rightX);
		left3.move(leftY + rightX);
        right1.move(leftY - rightX);
		right2.move(leftY - rightX);
		right3.move(leftY - rightX);
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
			piston.set_value(true); 
		}
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
			piston.set_value(false); 
		}
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
			intake.move(127); 
		} 
		else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
			intake.move(-127); 
		} else {
			intake.move(0);
		}
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
			intake.move(0); 
		}
		pros::delay(20);
	}
}
