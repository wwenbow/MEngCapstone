#ifndef __SSC32_CONTROLLER__
#define __SSC32_CONTROLLER__

enum Joint { 
    ANKLE
};

class SSC32Servo {

    public:
    int pin; // Pin number 0 - 31
    int position; // Postition 500 - 2500
    int time; // Time in milliseconds
    int speed; // Speed in microseconds per second

    SSC32Servo();
    ~SSC32Servo();
    calcServoOutput();
    

};

#endif
