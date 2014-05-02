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
static const int TIME_STEP = 100; //Time step in milliseconds
static const int NUM_SERVOS = 12; //Servos numbered 0-31

enum JointType { 
    HIP1,   //Hip Rotate
    HIP2,   //Hip in/out
    HIP3,   //Hip front/back
    KNEE,   //Knee
    ANKLE1, //Ankle front/back, 
    ANKLE2  //Ankle left/right 
};

typedef struct {
    JointType jointType;
    String pin;
    String postion;
    String movTime;
    String command;
} Servo;

String servoIDs[NUM_SERVOS];
String sscOutputs[NUM_SERVOS];
String sscSpeeds[NUM_SERVOS];
String sscCommands[NUM_SERVOS];

String sscFinalCommand = "";
String terminalCommand = ""; //command from PC terminal

int counter = 0;
int operatingMode = 2; // 0 stop, 1 move, 2 manual servo control
bool commandComplete = false;

// Given a servo calculate output as ssc Positon Value
String calcServoOutput(int servo, int time);
// A sine function that generates an output from 500 to 2500
String sineFunctionA(int time);

void setup() 
{   
    //UART to SSC32
    Serial0.begin(38400);   

    //USB to PC for commands/debug
    Serial.begin(9600);

    //LED to flash satus
    pinMode(LED_PIN, OUTPUT); 
    digitalWrite(LED_PIN, HIGH); //High is off

    //Initialize Servo pins;
    //Right leg 0-5
    servoIDs[0] = 0; //Hip Rotate
    servoIDs[1] = 1; //Hip in/out
    servoIDs[2] = 2; //Hip front/back
    servoIDs[3] = 3; //Knee
    servoIDs[4] = 4; //Ankle front/back
    servoIDs[5] = 5; //Ankle left/right

    //Left leg 16-21
    servoIDs[6] = 16; //Hip Rotate
    servoIDs[7] = 17; //Hip in/out
    servoIDs[8] = 18; //Hip front/back
    servoIDs[9] = 19; //Knee
    servoIDs[10] = 20; //Ankle front/back
    servoIDs[11] = 21; //Ankle left/right
}

void loop()
{
    //Serial0.println("#1 P500 T1000"); //turns the servo to the initial position in 1 second
    //Serial0.println("#1 P2500 T1000"); //turns the servo to the final position in 1 second

    // Check and parse commands, manual mode
    readCommand();
    parseCommand();

    if (operatingMode == 1) 
        walkingMode();
    else if (operatingMode == 0) 
        stopMode();
}

void readCommand()
{
    while (Serial.available()>0) {
        char c = (char)Serial.read();
        Serial.print(c);

        terminalCommand += c;

        if (c == '\n') {
            commandComplete = true;
            Serial.println("command complete");
        }
    }
}

void parseCommand()
{
    if (commandComplete) {
        Serial.print("Recieved: " + terminalCommand);
        if (terminalCommand == "walk\n")
            operatingMode = 1;
        else if (terminalCommand == "stop\n")
            operatingMode = 0;
        else if (terminalCommand == "manual\n")
            operatingMode = 2;
        else {
            sendSSC32Command(terminalCommand);
        }
        terminalCommand = "";
        commandComplete = false;
        Serial.print("Current mode: ");
        Serial.println(operatingMode);
    }
}

void sendSSC32Command(String command) 
{
    digitalWrite(LED_PIN, LOW);

    Serial0.println(command);

    digitalWrite(LED_PIN, HIGH);  
}

void walkingMode()
{
    int start = millis();
    for (int i = 0; i < NUM_SERVOS; i++) {
        sscOutputs[i] = calcServoOutput(i, counter);
        sscSpeeds[i] = TIME_STEP;
        sscCommands[i] = "#" + servoIDs[i] + " P" + sscOutputs[i];
        sscFinalCommand += sscCommands[i] + " ";
    }
    int stop = millis() - start;

    sscFinalCommand += "T" + String(TIME_STEP);
    delay(TIME_STEP-4-stop-10);

    sendSSC32Command(sscFinalCommand);
    sscFinalCommand = "";

    delay(4);
    counter++;
}

void stopMode()
{
    for (int i = 0; i < NUM_SERVOS; i++) {
        sscOutputs[i] = 1500;
        sscSpeeds[i] = TIME_STEP;
        sscCommands[i] = "#" + servoIDs[i] + " P" + sscOutputs[i];
        sscFinalCommand += sscCommands[i] + " ";
    }

    sscFinalCommand += "T" + String(TIME_STEP);

    sendSSC32Command(sscFinalCommand);
    sscFinalCommand = "";

    delay(TIME_STEP+100);
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
