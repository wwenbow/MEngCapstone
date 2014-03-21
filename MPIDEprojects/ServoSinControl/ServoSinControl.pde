/*=============================================================================
 * Controls servos using the SSC32 Servo controller
 *
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

    //Initialize Servo pins;
    servoIDs[0] = 0; //Hip in/out
    servoIDs[1] = 3; //Hip forward back
    servoIDs[2] = 4; //Knee
    servoIDs[3] = 7; //Ankle forward back
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

    Serial.println(sscFinalCommand);
    Serial0.println(sscFinalCommand);
    sscFinalCommand = "";

    digitalWrite(LED_PIN, HIGH);  

    delay(TIME_STEP+100);
    counter++;
}

String calcServoOutput(int servo, int time) {
    switch (servo) {
        case 0:
            return "1400";
            break;
        case 1:
            return "1500";
            break;
        case 2:
            return sineKnee(time);
            break;
        case 3:
            return "1600";
            break;
        default:
            return "1600";
            break;
    }
}

/* Sine function for knee joint
 * Output values:
 * 2500 is fully straightened
 * 1250 is fully curled
 */
String sineKnee(int time) {
    // calulate output
    int period = 20; //number of time steps for full cycle
    double coef = 2*3.14/period;
    double sinVal = sin(coef*time); //
    
    // convert to int
    int range = 2500 - 1250;
    int minimum = 1250;
    int sscVal = (1+sinVal)*range/2 + minimum;

    return String(sscVal);
}

/* Sine function for hip foward/back
 * Output Values:
 * 1250 full back
 * 1500 full straight
 * 2500 full foward
 */
String sineHipForwardBack(int time) {
    int period = 20; //number of time steps for full cycle
    double coef = 2*3.14/period;
    double sinVal = sin(coef*time);

    int range = 2500 - 1250;
    int minimum = 1250;
    int sscVal = (1+sinVal)*range/2 + minimum;

    return String(sscVal);
}
