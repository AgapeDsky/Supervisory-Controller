#include <Arduino_FreeRTOS.h>

// Constants
#define tau 1.2
#define T 0.008
#define fixed_point_factor 10000.0

// Computing variables
float prev_input = 0, prev_output = 0;
float cur_input = 0, cur_output = 0;
float pose = 0;

// Functions
float compute();
void Task();

// FreeRTOS Handler
TaskHandle_t xHandle = NULL;
TickType_t xLastWakeTime = 0;

void setup() {
	// Initialize Serial
	Serial.begin(115200);
	
	// Initialize FreeRTOS
	xTaskCreate(Task, NULL, 512, NULL, 1, &xHandle);
	vTaskStartScheduler();

  // Initialize input pins
  pinMode(2, INPUT);
  pinMode(3, INPUT);
}

void loop() {
  // Emptied for FreeRTOS
}

/**
 * @brief Function to Compute Door's Response to a Given Control Effort
 * @return Final Door's Velocity
 */
float compute() {
  pose += cur_output * T;
	float retVal = prev_input/(2*tau+T)*T + cur_input/(2*tau+T)*T - prev_output*(T-2*tau)/(2*tau+T);
	return retVal;
}

/**
 * FreeRTOS Task
 */
void Task(void *pvParameters) {
	
	while (1) {
    // Get Time Stamp
    xLastWakeTime = xTaskGetTickCount();

    // Check Serial Port for Incoming Data
		if (Serial.available()) {
      pose  = pose + T*cur_output;
      prev_input = cur_input;
      prev_output = cur_output;
    
      // If Data is Present, Compute Output Value
			cur_input = Serial.parseInt()/fixed_point_factor;
			cur_output = prev_input/(2*tau+T)*T + cur_input/(2*tau+T)*T - prev_output*(T-2*tau)/(2*tau+T);
		}

    // Send Computational Output and Sensor's State to the Serial Port
    Serial.print((int)(fixed_point_factor*pose));
    Serial.print(",");
    Serial.print((int)(fixed_point_factor*cur_output));
    Serial.print(",");
    Serial.print(digitalRead(2));
    Serial.print(",");
    Serial.print(digitalRead(3));
    Serial.print(",");
    Serial.println();
    // Wait for the Period
		vTaskDelayUntil(&xLastWakeTime, 8/portTICK_PERIOD_MS);
	}
	
	vTaskDelete(xHandle);
}
		
