/*=============================================================================
 * Controls servos using the SSC32 Servo controller
 * # <ch>P <pw>S <spd> ...# <ch>P <pw>S <spd>T <time> <cr>
 * <ch> = Channel number in decimal,0- 31.
 * <pw> = Pulse width in microseconds, 500- 2500.
 * <spd> = Movement speed in uS per second for one channel. (Optional)
 * <time> = Time in mS for the entire move, affects all channels, 65535 max. (Optional)
 * <cr> = Carriage return character, ASCII 13. (Required to initiate action)
 * <esc> = Cancel the current command, ASCII 27.
 *===========================================================================*/

/*
 * pins are numbered with letter and number on UBW32, mpide only has 
 * numbers. To calculate: Letter*16 + number
 * UART1TX = F8
 */
static const int LED_PIN = 65; //LED2 red
static const int TIME_STEP = 1000; //Time step in milliseconds
static const int NUM_SERVOS = 4; //Servos numbered 0-31

String servoIDs[NUM_SERVOS];
String sscOutputs[NUM_SERVOS];
String sscSpeeds[NUM_SERVOS];
String sscCommands[NUM_SERVOS];

String sscFinalCommand = "";

int counter = 0;

// Given a servo calculate output as ssc Positon Value
String calcServoOutput(int servo, int time);
// A sine function that generates an output from 500 to 2500
String sineFunctionA(int time);

void setup() 
{   
    //UART to SSC32
    Serial0.begin(9600);   

    //USB to PC for commands/debug
    Serial.begin(9600);

    //LED to flash satus
    pinMode(LED_PIN, OUTPUT); 
    digitalWrite(LED_PIN, HIGH); //High is off

    //Initialize Servo values;
    servoIDs[0] = 0;
    servoIDs[1] = 4;
    servoIDs[2] = 8;
    servoIDs[3] = 12;
}

void loop()
{
    //Serial0.println("#1 P500 T1000"); //turns the servo to the initial position in 1 second
    //Serial0.println("#1 P2500 T1000"); //turns the servo to the final position in 1 second

    for (int i = 0; i < NUM_SERVOS; i++) {
        sscOutputs[i] = calcServoOutput(i, counter);
        sscSpeeds[i] = TIME_STEP;
        sscCommands[i] = "#" + servoIDs[i] + " P" + sscOutputs[i];
        sscFinalCommand += sscCommands[i] + " ";
    }

    sscFinalCommand += "T" + String(TIME_STEP);

    digitalWrite(LED_PIN, LOW);
    delay(TIME_STEP);

    for (int i = 0; i < NUM_SERVOS; i++) {
        Serial.println(sscCommands[i] + " T" + String(TIME_STEP));
        Serial0.println(sscCommands[i] + " T" + String(TIME_STEP));
        //Serial.println(sscFinalCommand);
        //Serial0.println(sscFinalCommand);
        //sscFinalCommand = "";
    }

    digitalWrite(LED_PIN, HIGH);  

    delay(TIME_STEP);
    counter++;
}

String calcServoOutput(int servo, int time) {
    switch (servo) {
        case 0:
            return sineFunctionA(time);
            break;
        case 1:
            return sineFunctionA(time);
            break;
        case 2:
            return sineFunctionA(time);
            break;
        case 3:
            return sineFunctionA(time);
            break;
        default:
            return 0;
            break;
    }
}

String sineFunctionA(int time) {
    double sinVal = sin(0.6*time);
    int sscVal = (1+sinVal)*500 + 1000;
    return String(sscVal);
}
