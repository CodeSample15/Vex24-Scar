#define KYLE_DRIVING false

#include "main.h"
#include "pros/misc.h"
#include "robot.h"

#include "library/IntakeController.h"
#include "library/AutonSelector.h"

#include <iostream> // for debugging

//autons:
#if ORANGE_BOT
	#include "autons/AWPRight_Orange.cpp"
	#include "autons/Skills_Orange.cpp"
	#include "autons/Bowl_Orange.cpp"
#else
	#include "autons/AWPRight_Red.cpp"
#endif

using namespace pros;

bool initializing = false;
bool selectingAuton = false;
AutonSelector selector;

void print_debug() {
	lcd::initialize();

	while(true) {
		lcd::set_text(1, "Climber position: " + std::to_string(climbMotor.get_position()));
		lcd::set_text(2, "Static imu: " + std::to_string(static_imu.get_rotation()));

		delay(10);

		lcd::clear();
	}
}

void select_auton_thread() {
	if(selectingAuton)
		return; //no duplicate threads please

	selectingAuton = true;

	//add the autons to the selector
	#if ORANGE_BOT
		//orange bot autons
		selector.add("AWP");
		selector.add("Bowl down", "alley", "(5x)");
	#else
		//red bot autons
		selector.add("AWP");
	#endif


	bool updateScreen = true;

	master.clear();
	delay(60);

	//Allow the user to select an auton
	while(selectingAuton) {
		if(updateScreen) {
			selector.display_autons(); //update screen
			updateScreen = false;
		}

		if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_UP)) {
			selector.iterate();
			updateScreen = true;
			std::cout << "pressed" << std::endl;
		}

		delay(10);
  	}
}

void initialize() {	
	IntakeMotor.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	climbMotor.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	climbMotor.tare_position();

	Task t(print_debug);
}

void disabled() {}

void competition_initialize() {
	//calibrate imu (blocking)
	master.clear();
	delay(60);
	master.set_text(0, 0, "Calibrating...");
	delay(60);

	imu.reset();
	static_imu.reset();
	delay(3000);
	imu.tare_rotation();
	static_imu.tare();

	//start auton selection thread
	Task t(select_auton_thread);
}

void autonomous() {
	selectingAuton = false; //kill auton selection thread

	#if ORANGE_BOT
	//orange bot autons
	switch(selector.getSelected()) {
		case 0:
			runRightAwpAuton();
			break;

		case 1:
			BowlDownAlley();
			break;

		default:
			std::cout << "Auton not found" << std::endl;
			break;
	}

	#else
	//red bot autons
	switch(selector.getSelected()) {
		case 0:
			runRightAwpAuton();
			break;

		default:
			std::cout << "Auton not found" << std::endl;
			break;
	}
	#endif
}

void opcontrol() {
	bool frontWingsDeployed = false;
	bool backWingsDeployed = false;
	rightMotors.set_brake_modes(E_MOTOR_BRAKE_COAST);
	leftMotors.set_brake_modes(E_MOTOR_BRAKE_COAST);

	while(!selectingAuton) {
		//driving
		if(KYLE_DRIVING) {
			double leftAmount = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
			double rightAmount = master.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y);
			driveChassis.DriveTank(leftAmount, rightAmount);
		}
		else {
			double driveAmount = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
			double turnAmount = master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X)/1.5;
			driveChassis.DriveArcade(driveAmount, turnAmount); 
		}

		//intake
		if(master.get_digital(E_CONTROLLER_DIGITAL_R1))
			IntakeMotor.move(127);
		else if(master.get_digital(E_CONTROLLER_DIGITAL_R2))
			IntakeMotor.move(-127);
		else
			IntakeMotor.brake();

		//wings
		if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_L1)) {
			frontWingsDeployed = !frontWingsDeployed;
			frontWings.set_value(frontWingsDeployed);
		}
		if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_L2)) {
			backWingsDeployed = !backWingsDeployed;
			backWings.set_value(backWingsDeployed);
		}

		//climber
		#if ORANGE_BOT
		if(master.get_digital(E_CONTROLLER_DIGITAL_UP))
			climbMotor.move(-127);
		else if(master.get_digital(E_CONTROLLER_DIGITAL_DOWN))
			climbMotor.move(127);
		else
			climbMotor.brake();
		#endif

		delay(10);
	}
}
