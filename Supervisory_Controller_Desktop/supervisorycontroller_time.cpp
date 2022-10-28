#include "supervisorycontroller_time.h"

float supervisorycontroller :: getPose() {
	return pose;
}

float supervisorycontroller :: getVelocity() {
	return velocity;
}

float supervisorycontroller :: getTarget() {
	return velocity_target;
}

float supervisorycontroller :: getControlEffort() {
	return control_effort;
}

int supervisorycontroller :: getState() {
	return state;
}

void supervisorycontroller :: setPose(float val) {
	pose = val;
}

void supervisorycontroller :: setVelocity(float val) {
	velocity = val;
}

void supervisorycontroller :: setButState (bool val) {
	but_state = val;
}

void supervisorycontroller :: setSenseState (bool val) {
	sense_state = val;
}

void supervisorycontroller :: setFeedback(float pose, float vel, bool but, bool sense) {
	setPose(pose);
	setVelocity(vel);
	setButState(but);
	setSenseState(sense);
}

float supervisorycontroller :: computeControl() {
	/**
	 * Resting state
	 */
	if (state == REST) {
		// If button is pressed, the door will open
		if (but_state) {
			state = OPENING_ACCELERATE;
			velocity_target = velocity_target + 0.15;
			control_effort = kp_accelerate * (velocity_target - velocity);
            count = 0;
		}
		// If the button is not pressed, the door will stay still
		else {
			velocity_target = 0;
			control_effort = kp * (velocity_target - velocity);
		}
	}
	
	/**
	 * Opening Accelerated state
	 */
	else if (state == OPENING_ACCELERATE) {
		// If some sequence has been reached, let the door's speed be constant
		if (count >= ACCELERATE_THRESHOLD) {
			state = OPENING_CONSTANT;
			control_effort = kp_constant * (velocity_target - velocity);
            count = 0;
		}
		// If the final sequence has not been reached, continue to accelerate
		else {
			velocity_target = velocity_target + 0.15;
			control_effort = kp_accelerate * (velocity_target - velocity);
            count ++;
		}
	}

	/**
	 * Opening Constant state
	 */
	else if (state == OPENING_CONSTANT) {
		// If some sequences have been reached, decelerate
		if (count >= CONSTANT_THRESHOLD) {
			state = OPENING_DECELERATE;
			velocity_target = velocity_target - 0.15;
			control_effort = kp_decelerate * (velocity_target - velocity);
            count = 0;
		}
		// If the final sequence has not been reached, continue constant velocity
		else {
			control_effort = kp_constant * (velocity_target - velocity);
            count ++;
		}
	}

	/**
	 * Opening Decelerated state
	 */
	else if (state == OPENING_DECELERATE) {
		// If the door position target is reached, go to pre-standby mode
		if (count >= DECELERATE_THRESHOLD) {
			state = OPENING_ZEROTARGET;
			velocity_target = 0;
			control_effort = kp_zerotarget * (velocity_target - velocity);
            count = 0;
		}
		// If the final sequence has not been reached, continue to decelerate
		else {
			velocity_target = velocity_target - 0.15;
			control_effort = kp_decelerate * (velocity_target - velocity);
            count ++;
		}
	}

	/**
	 * Pre-stand by state
	 */
	else if (state == OPENING_ZEROTARGET) {
		// Go to standby mode
		if (count >= ZEROTARGET_THRESHOLD) {
			state = STANDBY;
			velocity_target = 0;
			control_effort = kp * (velocity_target - velocity);
            count = 0;
		}
		// Wait several sequences with 0 target
		else {
			velocity_target = 0;
			control_effort = kp_zerotarget * (velocity_target - velocity);
            count ++;
		}
	}
	
	/**
	 * Stand by state
	 */
	else if (state == STANDBY) {
		// If button is pressed, proceed to closing the door
		if (but_state && !sense_state) {
			state = CLOSING_ACCELERATE;
			velocity_target = velocity_target - 0.15;
			control_effort = kp_accelerate * (velocity_target - velocity);
            count = 0;
		}
		// If the button is not pressed, the door will stay still
		else {
			velocity_target = 0;
			control_effort = kp * (velocity_target - velocity);
		}
	}
	
	/**
	 * Closing Accelerated state
	 */
	else if (state == CLOSING_ACCELERATE) {
		if (sense_state) {
			state = EMERGENCY_OPEN;
			velocity_target = 0;
			control_effort = kp_emergency * (target_emergency - pose);
		}
		// If some sequence has been reached, let the door's speed be constant
		else if (count >= ACCELERATE_THRESHOLD) {
			state = CLOSING_CONSTANT;
			control_effort = kp_constant * (velocity_target - velocity);
            count = 0;
		}
		// If the final sequence has not been reached, continue to accelerate
		else {
			velocity_target = velocity_target - 0.15;
			control_effort = kp_accelerate * (velocity_target - velocity);
            count ++;
		}
	}

	/**
	 * Closing Constant state
	 */
	else if (state == CLOSING_CONSTANT) {
		if (sense_state) {
			state = EMERGENCY_OPEN;
			velocity_target = 0;
			control_effort = kp_emergency * (target_emergency - pose);
		}
		// If some sequence has been reached, let the door's speed be decelerating
		else if (count >= CONSTANT_THRESHOLD) {
			state = CLOSING_DECELERATE;
			velocity_target = velocity_target + 0.15;
			control_effort = kp_decelerate * (velocity_target - velocity);
            count = 0;
		}
		// If the final sequence has not been reached, continue with constant velocity
		else {
			control_effort = kp_constant * (velocity_target - velocity);
            count++;
		}
	}

	/**
	 * Closing Decelerated state
	 */
	else if (state == CLOSING_DECELERATE) {
		if (sense_state) {
			state = EMERGENCY_OPEN;
			velocity_target = 0;
			control_effort = kp_emergency * (target_emergency - pose);
		}
		// If some sequence has been reached, let the door's speed targets 0 
		else if (count >= DECELERATE_THRESHOLD) {
			state = CLOSING_ZEROTARGET;
			velocity_target = 0;
			control_effort = kp_zerotarget * (velocity_target - velocity);
            count = 0;
		}
		// If the final sequence has not been reached, continue to decelerate
		else {
			velocity_target = velocity_target + 0.15;
			control_effort = kp_decelerate * (velocity_target - velocity);
            count++;
		}
	}

	/**
	 * Closing pre-rest state
	 */
	else if (state == CLOSING_ZEROTARGET) {
		if (sense_state) {
			state = EMERGENCY_OPEN;
			velocity_target = 0;
			control_effort = kp_emergency * (target_emergency - pose);
		}
		// If some sequence has been reached, go to rest mode
		else if (count >= ZEROTARGET_THRESHOLD) {
			state = REST;
			velocity_target = 0;
			control_effort = kp * (velocity_target - velocity);
            count = 0;
		}
		// If the door position target is not yet reached, continue to target 0 pose and velocity
		else {
			velocity_target = 0;
			control_effort = kp_zerotarget * (velocity_target - velocity);
            count++;
		}
	}

	/**
	 * Emergency Opening state
	 */
	else if (state == EMERGENCY_OPEN) {
		if ((pose >= target_emergency - 0.05) && (pose <= target_emergency + 0.05) && velocity <= 0.05 && velocity >= 0.05) {
			state = STANDBY;
			control_effort = 0;
		}
		else {
			// While the door is not opened, try opening the door
			velocity_target = 0;
			control_effort = kp_emergency * (target_emergency - pose);
		}
	}
	
	// Return control effort value
	return control_effort;
}
