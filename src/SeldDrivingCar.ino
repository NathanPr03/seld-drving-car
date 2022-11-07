#include <QTRSensors.h>
#include "HUSKYLENS.h"
#include "CommandFactory.cpp"

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

CommandFactory* commandFactory = new CommandFactory();
Command* driveCommand;

void setup() {
    Serial.begin(9600);
    Serial.println("Command pattern :)");

    Particle.variable("vehicleStatus", vehicleStatus);
    vehicleStatus = STATIONARY;

    // Wire.begin();
    // while (!huskylens.begin(Wire))
    // {
    //     Serial.println(F("Begin failed!"));
    //     Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
    //     Serial.println(F("2.Please recheck the connection."));
    //     delay(100);
    // }

    qtr.setTypeRC(); 
    qtr.setSensorPins((const uint8_t[]){A2, A1, A0}, SensorCount);
    
    pinMode(onBoardLed, OUTPUT);
    
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
    //delay(5000);
}

void loop() {
    Serial.println("Start of loop");
    
    huskyLens();
    useSensors();
    //delay(2000);
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
                vehicleStatus = ENCOUNTERED_OBSTACLE;
                //motor_stop(2000);
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

void useSensors(){
    qtr.read(sensorValues);
    Serial.println("Value for left sensor ");
    Serial.println(String(sensorValues[0]));

    Serial.println("Value for middle sensor");
    Serial.println(String(sensorValues[1]));

    Serial.println("Value for right sensor ");
    Serial.println(String(sensorValues[2]));

    calculate_direction();

    if(driveCommand != NULL) {
        driveCommand->execute();
    }
}

void calculate_direction(){
    if(black_line_in_middle()){
        Serial.println("MIDDLE IS CALLED");
        vehicleStatus = MOVING;
        driveCommand = commandFactory->create(STRAIGHT_AHEAD, motorLeft, motorRight);
    }else if (right_hard_turn()) {
        Serial.println("HARD RIGHT IS CALLED");
        vehicleStatus = MOVING;
        driveCommand = commandFactory->create(TURN_RIGHT_HARD, motorLeft, motorRight);
    }else if (left_hard_turn()){
        Serial.println("HARD LEFT IS CALLED");
        vehicleStatus = MOVING;
        driveCommand = commandFactory->create(TURN_LEFT_HARD, motorLeft, motorRight);
    }else if(black_line_on_left()) {
        Serial.println("LEFT IS CALLED");
        vehicleStatus = MOVING;
        driveCommand = commandFactory->create(TURN_LEFT_GRADUAL, motorLeft, motorRight);
    }else if(black_line_on_right()){
        Serial.println("RIGHT IS CALLED");
        vehicleStatus = MOVING;
        driveCommand = commandFactory->create(TURN_RIGHT_GRADUAL, motorLeft, motorRight);
    }else{
        Serial.println("UNDO IS CALLED");
        if(driveCommand != NULL){
            vehicleStatus = MOVING;
            driveCommand->undo();
        }
    }
}

void led_on(int led){
    Serial.println("LED on");
    digitalWrite(led, HIGH);
}

void led_off(int led){
    Serial.println("LED off");
    digitalWrite(led, LOW);
}

bool black_line_in_middle() {
    return (sensorValues[MIDDLE_SENSOR] > sensorValues[LEFT_SENSOR] && 
    sensorValues[MIDDLE_SENSOR] > sensorValues[RIGHT_SENSOR]) || sensorValues[LEFT_SENSOR] == sensorValues[RIGHT_SENSOR];
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
