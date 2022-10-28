#ifndef SUPERVISORYCONTROLLER_TIME_H
#define SUPERVISORYCONTROLLER_TIME_H

// Threshold value for opening / closing
#define LOWER_THRESHOLD 0.001
#define UPPER_THRESHOLD 1.099
#define ACCELERATE_THRESHOLD 6
#define CONSTANT_THRESHOLD 149
#define DECELERATE_THRESHOLD 6
#define ZEROTARGET_THRESHOLD 199

// Tunable parameter for opening / closing speed
#define kp 5
#define kp_accelerate 5
#define kp_constant 5
#define kp_decelerate 5
#define kp_zerotarget 5
#define kp_emergency 1

// Target pose for opening / closing (in meters)
#define target_rest 0
#define target_standby 1.099
#define target_emergency 1.099

enum States {
	REST,
	OPENING_ACCELERATE,
	OPENING_CONSTANT,
	OPENING_DECELERATE,
	OPENING_ZEROTARGET,
	STANDBY,
	CLOSING_ACCELERATE,
	CLOSING_CONSTANT,
	CLOSING_DECELERATE,
	CLOSING_ZEROTARGET,
	EMERGENCY_OPEN
};

/**
 * @brief Door lift FSM 
 */
class supervisorycontroller {
	private:
		float pose = 0;													// Door position
		float velocity = 0;												// Door's velocity
		float control_effort = 0;										// Control effort for controlling door's speed
		bool but_state = 0;												// Button signal (for opening/closing the door)
		bool sense_state = 0;											// Human sensor (modelled as button)
		float velocity_target = 0;										// Target velocity
		States state = REST;											// Door fsm's state
		int count = 0;
	public:
		float getPose();												// Get current door's position
		float getVelocity();											// Get current door's velocity
		float getControlEffort();										// Get current control effort
		float getTarget();												// Get current door's velocity target
		int getState();													// Get current state
		void setPose(float val);										// Set current pose
		void setVelocity(float val);									// Set current velocity
		void setButState (bool val);									// Set current button state
		void setSenseState (bool val);									// Set current human sensor state
		void setFeedback(float pose, float vel, bool but, bool sense);	// Set pose, button, and human sensor state
		float computeControl();											// Compute control effort
};

#endif