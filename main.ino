// #include <D:/arduino-1.8.13/hardware/arduino/avr/cores/arduino/arduino.h>
#include <arduino.h>

/**
 * 
 * Attiny85 Eye Timer
 * 
 * A dedicated timer to rest eyes every 20 minutes while working
 * 
 **/

//States - waitMain(0), workTimer(1), waitRest(2), restTimer(3)
int programState = 0;
int currButtonState;
int prevButtonState;

//Leds
int powerLED = 0;
int workLED = 1;
int restLED = 2;

//Buttons
int startButton = 3;
int restButton = 4;

//Delays
int mainDelay = 500;

//Timing
// unsigned long twentyInMillis = 1200000;
static unsigned long twentyInMillis = 10000;
unsigned long startTime = 0;
bool timerDone = false;

void setup()
{
    Serial.begin(9600);
    pinMode(powerLED, OUTPUT);
    pinMode(workLED, OUTPUT);
    pinMode(restLED, OUTPUT);

    pinMode(startButton, INPUT);
    pinMode(restButton, INPUT);
    digitalWrite(powerLED, HIGH);
}

void loop()
{
    Serial.println("Current State: %s", programState);
    bool transition = false;
    switch(programState) 
    {
        case 0:
            //Main waiting loop
            transition = checkButtons(startButton);
            if (transition)
            {
                Serial.println("Start button pressed. Transitioning...");
                startTime = millis();
                pulseLED(workLED);
                programState = 1;
            }
            break;
        case 1:
            //20 minute timer loop
            if (!timerDone)
            {
                transition = checkButtons(startButton);
                if (transition)
                {
                    pulseLED(powerLED);
                    programState = 0;
                }
                else
                {
                    checkTime();
                }
            }
            else
            {
                transition = checkButtons(restButton);
                if (transition)
                {
                    programState = 2;
                    startTime = millis();
                    pulseLED(workLED);
                }
                else
                {
                    flashLED(workLED);
                }
            }
            break;
        case 2:
            //Wait period to initiate rest loop
            transition = checkButtons(startButton);
            if (transition)
            {
                //FIXME
            }
            else
            {
                //FIXME
            }
        case 3:
            //20 second rest loop
            if (!timerDone)
            {
                transition = checkButtons(startButton);
                if (transition)
                {
                    //FIXME - take current millis
                }
                else
                {
                    //FIXME - pulse led and check time
                }
            }
            else
            {
                //FIXME - flash restLED (while loop until button)
            }
            break;
    }
    // checkButtons();
}

bool checkButtons(int buttonPin)
{
    bool result = false;
    currButtonState = digitalRead(buttonPin);
    if (prevButtonState == HIGH && currButtonState == LOW)
    {
        result = true;
    }

    return result;
}

void checkTime()
{
    unsigned long currDuration = millis() - startTime;
    if (currDuration >= twentyInMillis)
    {
        timerDone = true;
    }
    else
    {
        if (currDuration % 100 == 0) 
        {
            // pulseLED();
        }
    }
}

void pulseLED(int led)
{
    digitalWrite(led, HIGH);
    delay(200);
    digitalWrite(led, LOW);
}

void flashLED(int led)
{
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(100);
}