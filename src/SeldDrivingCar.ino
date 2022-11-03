#include <QTRSensors.h>
#include "HUSKYLENS.h"
#include "CommandFactory.cpp"

using namespace std;

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
int motorLeft[2] = {A4, A5};
int motorRight[2] = {D6, D5};

CommandFactory* commandFactory = new CommandFactory();
Command* driveCommand;

void setup() {
    Serial.println("Command pattern :)");
    Particle.variable("vehicleStatus", vehicleStatus);
    vehicleStatus = STATIONARY;

    Serial.begin(9600);

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

void loop() {
    Serial.println("Start of loop");
    huskyLens();
    useSensors();
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
                motor_stop(2000);
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

    calculate_direction(sensorValues);

    driveCommand->execute();
}

void calculate_direction(uint16_t sensorValues[]){
    if(black_line_in_middle()){
        driveCommand = commandFactory->create(STRAIGHT_AHEAD);
    }else if (right_hard_turn()) {
        driveCommand = commandFactory->create(TURN_RIGHT_HARD);
    }else if (left_hard_turn()){
        driveCommand = commandFactory->create(TURN_LEFT_HARD);
    }else if(black_line_on_left()) {
        driveCommand = commandFactory->create(TURN_LEFT_GRADUAL);
    }else if(black_line_on_right()){
        driveCommand = commandFactory->create(TURN_RIGHT_GRADUAL);
    }
    driveCommand->undo();
}

void motor_stop(int duration) {
    Serial.println("Stopping");

    digitalWrite(motorLeft[0], LOW);
    digitalWrite(motorLeft[1], LOW);

    digitalWrite(motorRight[0], LOW);
    digitalWrite(motorRight[1], LOW);

    delay(duration);
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