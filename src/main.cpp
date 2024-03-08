#define KYLE_DRIVING false
#define TESTING false //NEVER DOWNLOAD CODE WITH THIS SET TO TRUE. BLOCKS CODE FROM RUNNING TO RUN TEST CODE

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
	#include "autons/Skills_Red.cpp"
	#include "autons/Bowl_Red.cpp"
#endif

using namespace pros;

bool initializing = false;
bool selectingAuton = false;
AutonSelector selector(1);

void print_debug() {
	lcd::initialize();
	#if ORANGE_BOT
		while(true) {
			lcd::set_text(1, "Climber position: " + std::to_string(climbMotor.get_position()));
			lcd::set_text(2, "Static imu: " + std::to_string(static_imu.get_rotation()));
			lcd::set_text(3, "Slapper pos: " + std::to_string(SlapperMotor.get_position()));

			delay(10);

			lcd::clear();
		}
	#else
		while(true) {
			lcd::set_text(1, "Static imu: " + std::to_string(static_imu.get_rotation()));

			delay(10);

			lcd::clear();
		}
	#endif
}

void select_auton_thread() {
	if(selectingAuton)
		return; //no duplicate threads please

	selectingAuton = true;

	//add the autons to the selector
	#if ORANGE_BOT
		//orange bot autons
		selector.add("AWP");
		selector.add("Bowl down", "alley", "(10x)");
		selector.add("Skills");
	#else
		//red bot autons
		selector.add("Skills");
		selector.add("Supreme", "bowler");
		selector.add("AFK");
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
		}

		delay(10);
  	}
}

void initialize() {	
	IntakeMotor.set_brake_mode(E_MOTOR_BRAKE_COAST);

	LeftSlapperMotor.set_gearing(E_MOTOR_GEAR_200);
	LeftSlapperMotor.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
	RightSlapperMotor.set_gearing(E_MOTOR_GEAR_200);
	RightSlapperMotor.set_brake_mode(E_MOTOR_BRAKE_BRAKE);

	climbMotor.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	climbMotor.tare_position();

	Task t(print_debug);
}

void disabled() {}

void competition_initialize() {
	if(!selectingAuton) {
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

		case 2:
			run_skills();
			break;

		default:
			std::cout << "Auton not found" << std::endl;
			break;
	}

	#else
	//red bot autons
	switch(selector.getSelected()) {
		case 0:
			run_skills();
			break;

		case 1:
			SupremeBowling();
			break;

		case 2:
			//afk
			startIntake(false); //drop intake
			delay(1500); //wait
			stopIntake(); //no need wasting that valuable battery juice
			break;

		default:
			std::cout << "Auton not found" << std::endl;
			break;
	}
	#endif
}

void opcontrol() {
	if(TESTING) {
		imu.reset();
		static_imu.reset();
		delay(3000);
		imu.tare_rotation();
		static_imu.tare();
	}

	bool frontWingsDeployed = false;
	bool leftBackWingsDeployed = false;
	bool rightBackWingsDeployed = false;
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
		#if ORANGE_BOT
		if(master.get_digital(E_CONTROLLER_DIGITAL_R1))
			IntakeMotor.move(127);
		else if(master.get_digital(E_CONTROLLER_DIGITAL_R2))
			IntakeMotor.move(-127);
		else
			IntakeMotor.brake();
		#else
		if(master.get_digital(E_CONTROLLER_DIGITAL_R1))
			IntakeMotor.move(-127);
		else if(master.get_digital(E_CONTROLLER_DIGITAL_R2))
			IntakeMotor.move(127);
		else
			IntakeMotor.brake();
		#endif

		//wings
		#if ORANGE_BOT
		if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_L1)) {
			frontWingsDeployed = !frontWingsDeployed;
			frontWings.set_value(frontWingsDeployed);
		}
		#endif

		if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_Y)) {
			leftBackWingsDeployed = !leftBackWingsDeployed;
			leftBackWings.set_value(leftBackWingsDeployed);
		}
		if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_A)) {
			rightBackWingsDeployed = !rightBackWingsDeployed;
			rightBackWings.set_value(rightBackWingsDeployed); 
		}

		//slappers
		if(master.get_digital(E_CONTROLLER_DIGITAL_X))
			LeftSlapperMotor.move(110);
		else
			LeftSlapperMotor.brake();
		
		#if !ORANGE_BOT
		if(master.get_digital(E_CONTROLLER_DIGITAL_B))
			RightSlapperMotor.move(110);
		else
			RightSlapperMotor.brake();
		#endif

		//climber
		if(master.get_digital(E_CONTROLLER_DIGITAL_UP))
			climbMotor.move(-127);
		else if(master.get_digital(E_CONTROLLER_DIGITAL_DOWN))
			climbMotor.move(127);
		else
			climbMotor.brake();

		delay(10);
	}
}
