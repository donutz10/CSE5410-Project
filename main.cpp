// Arduino-Ardumoto obstacle avoiding robot
#include <NewPing.h>
#define TRIGGER_PIN 7      // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN 6         // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200   // Maximum distance we want to ping for (in centimeters). Maximum
#define LED_OBSTACLE_PIN 8 // LED PIN FOR OBSTACLE
// sensor distance is rated at 400-1000cm.

#define CW 0
#define CCW 1

// Motor definitions to make life easier:
#define MOTOR_A 0
#define MOTOR_B 1
// Pin Assignments
// Don’t change these! These pins are statically defined by the shield layout
const byte PWMA = 3;                                // PWM control (speed) for motor A
const byte PWMB = 11;                               // PWM control (speed) for motor B
const byte DIRA = 12;                               // Direction control for motor A
const byte DIRB = 13;                               // Direction control for motor B
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum

// distance.

void setup()
{
    setupArdumoto();                   // Set all pins as outputs
    pinMode(LED_OBSTACLE_PIN, OUTPUT); // setup the LED for obstacle
}
void loop()
{
    delay(50);
    unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
    if (uS / US_ROUNDTRIP_CM > 50 || uS / US_ROUNDTRIP_CM == 0)
    {
        // Move forward
        forward(255);
    }
    else if (uS / US_ROUNDTRIP_CM < 50)
    {
        digitalWrite(LED_OBSTACLE_PIN, HIGH); // set LED to High if obstacle is ahead
        delay(1000);
        digitalWrite(LED_OBSTACLE_PIN, LOW);
        delay(1000);
        digitalWrite(LED_OBSTACLE_PIN, HIGH);
        delay(1000);
        digitalWrite(LED_OBSTACLE_PIN, LOW);
        turnRight(255);
        delay(500);
    }
}
// driveArdumoto drives ’motor’ in direction ’dir’ at speed ’spd’
void driveArdumoto(byte motor, byte dir, byte spd)
{
    if (motor == MOTOR_A)
    {
        digitalWrite(DIRA, dir);
        analogWrite(PWMA, spd);
    }
    else if (motor == MOTOR_B)
    {
        digitalWrite(DIRB, dir);
        analogWrite(PWMB, spd);
    }
}
void forward(byte spd) // Runs both motors at speed ’spd’
{
    driveArdumoto(MOTOR_A, CW, spd); // Motor A at speed spd
    driveArdumoto(MOTOR_B, CW, spd); // Motor B at speed spd
}
void turnRight(byte spd)
{
    stopArdumoto(MOTOR_B);           // Motor B stop
    driveArdumoto(MOTOR_A, CW, spd); // Motor A run
}
// new function by nawaf - reverse
void reverse(byte spd)
{
    driveArdumoto(MOTOR_A, CCW, spd); // needed to be tested
    driveArdumoto(MOTOR_B, CCW, spd); // needed to be tested
}
// stopArdumoto makes a motor stop
void stopArdumoto(byte motor)
{
    driveArdumoto(motor, 0, 0);
}

// setupArdumoto initializes all pins
void setupArdumoto()
{
    // All pins should be set up as outputs:
    pinMode(PWMA, OUTPUT);
    pinMode(PWMB, OUTPUT);
    pinMode(DIRA, OUTPUT);
    pinMode(DIRB, OUTPUT);
    // Initialize all pins as low:
    digitalWrite(PWMA, LOW);
    digitalWrite(PWMB, LOW);
    digitalWrite(DIRA, LOW);
    digitalWrite(DIRB, LOW);
}