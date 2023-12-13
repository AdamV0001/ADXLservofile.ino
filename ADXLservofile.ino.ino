#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Servo.h>
#include <math.h>

// Create an instance of the Adafruit ADXL345 accelerometer
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

// Create Servo objects for the X and Y axes
Servo servoX;
Servo servoY;

// Define constants and variables for angle smoothing
const int WINDOW_SIZE = 3;           // Size of the angle smoothing window
float xAngleWindow[WINDOW_SIZE];      // Circular buffer for X-axis angles
float yAngleWindow[WINDOW_SIZE];      // Circular buffer for Y-axis angles
int windowIndex = 0;                  // Index to update the circular buffer

// Function to display detailed sensor information
void displaySensorDetails(void) {
  sensor_t sensor;
  accel.getSensor(&sensor);

  // Print sensor details to Serial monitor
  Serial.println("------------------------------------");
  Serial.print("Sensor:       "); Serial.println(sensor.name);
  Serial.print("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
}

// Function to display accelerometer data rate
void displayDataRate(void) {
  Serial.print("Data Rate:    ");

  // Determine and print the current data rate setting
  switch (accel.getDataRate()) {
    case ADXL345_DATARATE_3200_HZ:
      Serial.print("3200 ");
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print("1600 ");
      break;
      // ... (other cases, unchanged)
  }
  Serial.println(" Hz");
}

// Function to display accelerometer range
void displayRange(void) {
  Serial.print("Range:         +/- ");

  // Determine and print the current accelerometer range setting
  switch (accel.getRange()) {
    case ADXL345_RANGE_16_G:
      Serial.print("16 ");
      break;
    case ADXL345_RANGE_8_G:
      Serial.print("8 ");
      break;
      // ... (other cases, unchanged)
  }
  Serial.println(" g");
}

// Function to update the circular angle window
void updateWindow(float value, float window[]) {
  // Update the circular buffer with the latest angle value
  window[windowIndex] = value;
  windowIndex = (windowIndex + 1) % WINDOW_SIZE;
}

// Function to calculate the median of an array
float calculateMedian(float window[]) {
  // Bubble sort for simplicity
  for (int i = 0; i < WINDOW_SIZE - 1; i++) {
    for (int j = 0; j < WINDOW_SIZE - i - 1; j++) {
      // Swap elements if they are in the wrong order
      if (window[j] > window[j + 1]) {
        float temp = window[j];
        window[j] = window[j + 1];
        window[j + 1] = temp;
      }
    }
  }

  // Return the median value from the sorted array
  return window[WINDOW_SIZE / 2];
}

// Setup function
void setup(void) {
#ifndef ESP8266
  while (!Serial); // Wait for serial connection on certain boards
#endif

  // Start serial communication
  Serial.begin(9600);
  Serial.println("Accelerometer Test"); Serial.println("");

  // Check if the accelerometer is connected
  if (!accel.begin()) {
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while (1);
  }

  // Set accelerometer range and display sensor details
  accel.setRange(ADXL345_RANGE_16_G);
  displaySensorDetails();
  displayDataRate();
  displayRange();

  // Attach servos to pins 9 and 10
  servoX.attach(9);
  servoY.attach(10);

  Serial.println("");
}

// Loop function
void loop(void) {
  sensors_event_t event;
  accel.getEvent(&event);

  // Calculate angles in degrees
  float angleX = atan2(event.acceleration.x, event.acceleration.z) * RAD_TO_DEG;
  float angleY = atan2(event.acceleration.y, event.acceleration.z) * RAD_TO_DEG;

  // Update the angle windows
  updateWindow(angleX, xAngleWindow);
  updateWindow(angleY, yAngleWindow);

  // Calculate and display the median angles
  float medianX = calculateMedian(xAngleWindow);
  float medianY = calculateMedian(yAngleWindow);

  // Control servos based on median angles
  int servoXPosition = map(medianX, -90, 90, 0, 180);
  int servoYPosition = map(medianY, -90, 90, 0, 180);

  // Set servo positions
  servoX.write(servoXPosition);
  servoY.write(servoYPosition);

  // Display the results
  Serial.print("X Median Angle: "); Serial.print(medianX); Serial.println(" degrees");
  Serial.print("Y Median Angle: "); Serial.print(medianY); Serial.println(" degrees");
  Serial.print("Servo X Position: "); Serial.println(servoXPosition);
  Serial.print("Servo Y Position: "); Serial.println(servoYPosition);

  // Add a delay or other control logic if needed
}
