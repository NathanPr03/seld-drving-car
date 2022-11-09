#include <QTRSensors.h>
#include "HUSKYLENS.h"

using namespace std;

int motorLeft[] = {A4, A5};
int motorRight[] = {D6, D5};

int onBoardLed = D7;

QTRSensors qtr;
HUSKYLENS huskylens;

const uint8_t SensorCount = 3;
uint16_t sensorValues[SensorCount];

const int LEFT_SENSOR = 0;
const int MIDDLE_SENSOR = 1;
const int RIGHT_SENSOR = 2;

const int LARGE_DISPARITY_BETWEEN_SENSORS = 60;
int vehicleStatus;

enum vehicleStati {STATIONARY = 0, MOVING = 1, ENCOUNTERED_OBSTACLE = 2};

void setup() {
    Serial.begin(9600);
    
    Particle.variable("vehicleStatus", vehicleStatus);
    vehicleStatus = STATIONARY;

    Wire.begin();
    while (!huskylens.begin(Wire))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }

    qtr.setTypeRC(); 
    qtr.setSensorPins((const uint8_t[]){A2, A1, A0}, SensorCount);
    
    pinMode(onBoardLed, OUTPUT);

    for(int i = 0; i < 2; i++){
        pinMode(motorLeft[i], OUTPUT);
        pinMode(motorRight[i], OUTPUT);
    }
    
    led_on(onBoardLed);
    // analogRead() takes about 0.1 ms on an AVR.
    // 0.1 ms per sensor * 4 samples per sensor re ad (default) * 6 sensors
    // * 10 reads per calibrate() call = ~24 ms per calibrate() call.
    // Call calibrate() 400 times to make calibration take about 10 seconds.
    for (uint16_t i = 0; i < 400; i++)
    {
        qtr.calibrate();
    }
    led_off(onBoardLed); 

    for (uint8_t i = 0; i < SensorCount; i++)
    {
        Serial.println("Minimum calibrated value");
        Serial.println(String(qtr.calibrationOn.minimum[i]));
    }
    

    // print the calibration maximum values measured when emitters were on
    for (uint8_t i = 0; i < SensorCount; i++)
    {
        Serial.println("Maximum calibrated value");
        Serial.println(String(qtr.calibrationOn.maximum[i]));
    } 
    Serial.println("End of setup");
    delay(5000);

}

enum direction { TURN_LEFT_GRADUAL = 0, STRAIGHT_AHEAD = 1, TURN_RIGHT_GRADUAL = 2, TURN_LEFT_HARD = 3, TURN_RIGHT_HARD = 4 };

void loop() {
    Serial.println("Start of loop");
    huskyLens();
    useSensors();
    //delay(1000);
}

void huskyLens()
{
    if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
    else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
    else
    {
        Serial.println(F("###########"));
        while (huskylens.available())
        {
            HUSKYLENSResult result = huskylens.read();
            printResult(result);
            if(result.ID == 1){
                motor_stop(2000);
                vehicleStatus = ENCOUNTERED_OBSTACLE;
            }
        }    
    }
}

void printResult(HUSKYLENSResult result)
{
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Object unknown!");
    }

    if(result.ID == 1){
        Serial.println("Person spotted");
    }
}

void drive(int direction)
{
    switch(direction) {
        case(TURN_LEFT_GRADUAL):
            turn_left_gradual(1);
            vehicleStatus = MOVING;
            break;
        case(TURN_RIGHT_GRADUAL):
            turn_right_gradual(1);
            vehicleStatus = MOVING;
            break;
        case(TURN_RIGHT_HARD):
            turn_right_hard(1);
            vehicleStatus = MOVING;
            break;
        case(TURN_LEFT_HARD):
            turn_left_hard(1);
            vehicleStatus = MOVING;
            break;
        case(STRAIGHT_AHEAD):
            drive_forward(1);
            vehicleStatus = MOVING;
            break;
        default:
            vehicleStatus = STATIONARY;
            Serial.println("-----NO MATCH FOUND-----");
            break;
    }
}

void useSensors(){
    qtr.read(sensorValues);
    Serial.println("Value for left sensor ");
    Serial.println(String(sensorValues[0]));

    Serial.println("Value for middle sensor");
    Serial.println(String(sensorValues[1]));

    Serial.println("Value for right sensor ");
    Serial.println(String(sensorValues[2]));

    int direction = calculate_direction(sensorValues);

    drive(direction);
}

void led_on(int led){
    Serial.println("LED on");
    digitalWrite(led, HIGH);
}

void led_off(int led){
    Serial.println("LED off");
    digitalWrite(led, LOW);
}

void motor_stop(int duration){
    Serial.println("Stopping");

    digitalWrite(motorLeft[0], LOW);
    digitalWrite(motorLeft[1], LOW);

    digitalWrite(motorRight[0], LOW);
    digitalWrite(motorRight[1], LOW);

    delay(duration);
}

void turn_right_hard(int duration){
    Serial.println("Turning right hard");

    digitalWrite(motorLeft[0], HIGH);
    digitalWrite(motorLeft[1], LOW);

    digitalWrite(motorRight[0], HIGH);
    digitalWrite(motorRight[1], LOW);

    delay(duration);
}

void turn_right_gradual(int duration){
    Serial.println("Turning right gradually");

    digitalWrite(motorLeft[0], LOW);
    digitalWrite(motorLeft[1], LOW);

    digitalWrite(motorRight[0], HIGH);
    digitalWrite(motorRight[1], LOW);

    delay(duration);
}

void turn_left_hard(int duration){
    Serial.println("Turning left hard");

    digitalWrite(motorLeft[0], LOW);
    digitalWrite(motorLeft[1], HIGH);

    digitalWrite(motorRight[0], LOW);
    digitalWrite(motorRight[1], HIGH);

    delay(duration);
}

void turn_left_gradual(int duration){
    Serial.println("Turning left gradually");

    digitalWrite(motorLeft[0], LOW);
    digitalWrite(motorLeft[1], HIGH);

    digitalWrite(motorRight[0], LOW);
    digitalWrite(motorRight[1], LOW);

    delay(duration);
}

void drive_forward(int duration){
    Serial.println("Driving forward");

    digitalWrite(motorLeft[0], LOW);
    digitalWrite(motorLeft[1], HIGH);

    digitalWrite(motorRight[0], HIGH);
    digitalWrite(motorRight[1], LOW);
    
    delay(duration);
}

void drive_backward(int duration){
    Serial.println("Driving backward");

    digitalWrite(motorLeft[0], HIGH);
    digitalWrite(motorLeft[1], LOW);

    digitalWrite(motorRight[0], LOW);
    digitalWrite(motorRight[1], HIGH);

    delay(duration);
}

int calculate_direction(uint16_t sensorValues[]){
    if(black_line_in_middle()){
        return STRAIGHT_AHEAD;
    }else if (right_hard_turn()) {
        return TURN_RIGHT_HARD;
    }else if (left_hard_turn()){
        return TURN_LEFT_HARD;
    }else if(black_line_on_left()) {
        return TURN_LEFT_GRADUAL;
    }else if(black_line_on_right()){
        return TURN_RIGHT_GRADUAL;
    }
    return STRAIGHT_AHEAD;
}

bool black_line_in_middle() {
    return sensorValues[MIDDLE_SENSOR] > sensorValues[LEFT_SENSOR] && 
    sensorValues[MIDDLE_SENSOR] > sensorValues[RIGHT_SENSOR];
}

bool right_hard_turn() {
    int disparityBetweenSensors = sensorValues[RIGHT_SENSOR] - sensorValues[LEFT_SENSOR];    

    return disparityBetweenSensors >= LARGE_DISPARITY_BETWEEN_SENSORS;
}

bool left_hard_turn() {
    int disparityBetweenSensors = sensorValues[LEFT_SENSOR] - sensorValues[RIGHT_SENSOR];

    return disparityBetweenSensors >= LARGE_DISPARITY_BETWEEN_SENSORS;
}

bool black_line_on_left() {
    return sensorValues[LEFT_SENSOR] > sensorValues[MIDDLE_SENSOR] && sensorValues[LEFT_SENSOR] > sensorValues[RIGHT_SENSOR];
}

bool black_line_on_right() {
    return sensorValues[RIGHT_SENSOR] > sensorValues[LEFT_SENSOR] && sensorValues[RIGHT_SENSOR] > sensorValues[MIDDLE_SENSOR];
}
