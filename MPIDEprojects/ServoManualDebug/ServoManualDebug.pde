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
String servoCommand = "";
bool commandComplete = false;

void setup() 
{   
    //UART to SSC32
    Serial0.begin(9600);   

    //USB to PC for commands/debug
    Serial.begin(9600);

    //LED to flash satus
    pinMode(LED_PIN, OUTPUT); 
    digitalWrite(LED_PIN, HIGH); //High is off
}

void loop()
{
    //Serial0.println("#1 P500 T1000"); //turns the servo to the initial position in 1 second
    //Serial0.println("#1 P2500 T1000"); //turns the servo to the final position in 1 second

    while (Serial.available()>0) {
        char c = (char)Serial.read();
        Serial.print(c);

        servoCommand += c;

        if (c == '\n') {
            commandComplete = true;
            Serial.println("command complete");
        }

    }

    if (commandComplete) {
        digitalWrite(LED_PIN, LOW);
        delay(1000); //delay for 1 second

        Serial.println(servoCommand);
        Serial0.println(servoCommand);
        servoCommand = "";
        commandComplete = false;

        digitalWrite(LED_PIN, HIGH);  
        delay(1000);              // wait for a second
    }
}
