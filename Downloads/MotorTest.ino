// Arduino-Ardumoto obstacle avoiding robot
#include <NewPing.h>
#define TRIGGER_PIN 7      // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN 6         // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200   // Maximum distance we want to ping for (in centimeters). Maximum
#define LED_OBSTACLE_PIN 8 // LED PIN FOR OBSTACLE
#define RED_RGB_PIN 11     // Red pin
#define GREEN_RGB_PIN 10   // green pin
#define BLUE_RGB_PIN 9     // blue pin
// sensor distance is rated at 400-1000cm.

#define CW 0
#define CCW 1
#define SPEED 200
// Motor definitions to make life easier:
#define MOTOR_A 0
#define MOTOR_B 1
// Pin Assignments
// Don’t change these! These pins are statically defined by the shield layout
const byte PWMA = 3;                                // PWM control (speed) for motor A
const byte PWMB = 11;                               // PWM control (speed) for motor B
const byte DIRA = 2;                                // Direction control for motor A
const byte DIRB = 4;                                // Direction control for motor B
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum

// distance.

void setup()
{
    setupArdumoto();                   // Set all pins as outputs
    pinMode(LED_OBSTACLE_PIN, OUTPUT); // setup the LED for obstacle
    pinMode(RED_RGB_PIN, OUTPUT);      // red pin
    pinMode(GREEN_RGB_PIN, OUTPUT);    // green pin
    pinMode(BLUE_RGB_PIN, OUTPUT);     // blue pin
    Serial.begin(9600);
}
void loop()
{
    // Contains {Distance,Direction} where if Direction is 1 its right, 0 left
    int Distance_Direction[2][2] = {{0, 1}, {0, 0}};

    delay(50);
    unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
    if (uS / US_ROUNDTRIP_CM > 50 || uS / US_ROUNDTRIP_CM == 0)
    {
        // Move forward
        Serial.println("Straight");
        digitalWrite(LED_OBSTACLE_PIN, LOW);
        forward(SPEED);
    }
    else if (uS / US_ROUNDTRIP_CM < 50)
    {
        ///
        digitalWrite(LED_OBSTACLE_PIN, HIGH); // set LED to High if obstacle is ahead
        delay(1000);
        digitalWrite(LED_OBSTACLE_PIN, LOW);
        delay(1000);
        digitalWrite(LED_OBSTACLE_PIN, HIGH);
        delay(1000);
        digitalWrite(LED_OBSTACLE_PIN, LOW);

        ///
        int maxVa = 0;
        int maxVal_Occ = 0;

        reverse(150);

        // Turn right and left and check if anything returns
        delay(1500);
        Serial.println("RIGHT");
        turnRight(180);
        // capture distance
        uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
        Distance_Direction[0][0] = uS / US_ROUNDTRIP_CM;
        Serial.println(Distance_Direction[0][0]);
        delay(1200);

        Serial.println("LEFT");
        turnLeft(180);
        uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
        Distance_Direction[1][0] = uS / US_ROUNDTRIP_CM;
        Serial.println(Distance_Direction[1][0]);
        delay(1200);

        // if anything was hit get the max, this will give largest distance
        // from the object it saw, meaning their is space
        maxVa = max(Distance_Direction[0][0], Distance_Direction[1][0]);
        if (maxVa != 0)
        {
            Serial.println("max val: ");
            Serial.println(maxVa);

            // Find where the max value occurred
            for (size_t i = 0; i <= 1; i++)
            {
                if (maxVa == Distance_Direction[i][0])
                {
                    Serial.println("max val is a: ");
                    Serial.println(Distance_Direction[i][1]);
                    maxVal_Occ = Distance_Direction[i][1];
                }
            }

            // Turn the car towards the space
            if (maxVal_Occ == 1)
            {
                Serial.println("TURN RIGHT");
                turnRight(100);
            }
            else
            {
                Serial.println("TURN LEFT");
                turnRight(100);
            }

            delay(1500);
        }
        // If the max returns zero, it means either the object was close or
        // there was nothing to ping
        else
        {
            reverse(150);
            delay(2250);
        }
    }
}

// Function prototype
//  driveArdumoto drives ’motor’ in direction ’dir’ at speed ’spd’
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
    stop_rgb();
    green_rgb();
}
void turnRight(byte spd)
{
    stopArdumoto(MOTOR_B);           // Motor B stop
    driveArdumoto(MOTOR_A, CW, spd); // Motor A run
}
// turn left function
void turnLeft(byte spd)
{
    stopArdumoto(MOTOR_A);           // Motor B stop
    driveArdumoto(MOTOR_B, CW, spd); // Motor A run
}
// new function by nawaf - reverse
void reverse(byte spd)
{
    driveArdumoto(MOTOR_A, CCW, spd); // needed to be tested
    driveArdumoto(MOTOR_B, CCW, spd); // needed to be tested
    stop_rgb();
    white_rgb();
}
// stopArdumoto makes a motor stop
void stopArdumoto(byte motor)
{
    driveArdumoto(motor, 0, 0);
}

void setColor(int red, int green, int blue)
{
    analogWrite(RED_RGB_PIN, red);
    analogWrite(GREEN_RGB_PIN, green);
    analogWrite(BLUE_RGB_PIN, blue);
}
void green_rgb()
{
    setColor(0, 255, 0);
}
void white_rgb()
{
    setColor(255, 255, 255);
}
void stop_rgb(int red, int green, int blue);
{
    analogWrite(RED_RGB_PIN, 0);
    analogWrite(GREEN_RGB_PIN, 0);
    analogWrite(BLUE_RGB_PIN, 0);
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