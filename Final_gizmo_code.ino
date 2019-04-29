/* Project: DE2 Gizmo - Interactive blocks
 *  Written by: Higor Alves
 *  Date: Term 2 - DE Class of 17/18
 *  Description: 
 *  The following code defines motor movements based on length and number of interactions with IR sensors/buttons. These
 *  movements define personalities for 6 different linearly actuated blocks in my Gizmo.
 *  Note:
 *  The first, ''Shy'' persoanlity function in the Drawer class is fully commented, and only things changed in the subsequent 
 *  personalities are commented; refere to the Shy one for comprehensive notes.
 *  IR sensors and buttons behave differently. The code below was written for button for testing (hence the variable names) but was 
 *  then altered to use an IR sensor instead, hence the variable name "button" remaining in places instead of IR.
 */

#include <VarSpeedServo.h>    //Used to allow for variable speed of the servos. This library does so similarly to
                              //PWM for DC motors, turning the servo between small angle deltas with delays inbetween.
                              //This does not reduce torque.
#include "Arduino.h"
#define PIN_IR1 7
#define PIN_IR2 6
#define PIN_IR3 5
#define PIN_IR4 4
#define PIN_IR5 3
#define PIN_IR6 2

#define PIN_MOTOR1 8
#define PIN_MOTOR2 9
#define PIN_MOTOR3 10
#define PIN_MOTOR4 11
#define PIN_MOTOR5 12
#define PIN_MOTOR6 13


//-----------------------------------------------DRAWER CLASS-----------------------------------------------------//
class Drawer{   
  //drawer class with different personalities
  
  //define input variables that dont have to be seen outside the class 
  private:
    VarSpeedServo myservo;
    unsigned long startTime;
    unsigned long pressTime;
    unsigned long startMillis;
    unsigned long currentMillis;
    boolean buttonState = HIGH;     
    boolean lastButtonState = HIGH;       //set intial trigger state as off
    int buttonPushCounter = 0;        //counter for how many times trigger is actuated
    int firstTime = 1;      
    //const unsigned long period = 1000;      //unused
    unsigned long timeNow;  
    int pos = 0;              //set initial position as zero
    byte pinIR;
    byte pinMOTOR;
    
  //define input variables that have to be seen outside of the class
  public:
    Drawer(byte pinIR, byte pinMOTOR){
      this->pinIR = pinIR;
      this->pinMOTOR = pinMOTOR;
    }
   
  //assign IR sensors and motors to pins and begin timer
  void enable() {
    myservo.attach(pinMOTOR);
    pinMode(pos, OUTPUT);
    pinMode(pinIR, INPUT_PULLUP);
    Serial.begin(9600);      //allows use of serial monitor for debugging
    startMillis = millis;
   }
  
  void shy() {
    //----------------------------SHY PERSONALITY CYCLE----------------------------//
    
    buttonState = digitalRead(pinIR);
    if (buttonState != lastButtonState) {
      if (buttonState == LOW){
        buttonPushCounter++;    //increments a button counter every time the button/IR is triggered
        Serial.println("on");
        Serial.print("number of button pushes: ");
        Serial.println(buttonPushCounter);
      }
      else{
        Serial.println("off");
      }
    }
    
    lastButtonState = buttonState;
  
    if (buttonState == LOW){
       if(firstTime == 1){
          startTime = millis();      //start timer for length of button press if its the first time the button has been pressed
          firstTime = 0;
       }
  
       pressTime = millis() - startTime;   //variable for the time the button is being pressed for
  
       if (pressTime >= 1){               //Counts and displays length of time button is pressed for. Used for debugging.
          Serial.print("Time: ");
          Serial.print(pressTime);
          Serial.print(" milliseconds ");
          Serial.print(int(pressTime/1000));
          Serial.println(" seconds");
       }
  
       if (buttonPushCounter % 3 == 0 ){     //if the button push counter is a multiple of three
          myservo.attach(pinMOTOR);       
          fwd(180, 200);
          currentMillis = millis();
          startMillis = currentMillis;    //restart timer since last interaction
          if(pressTime > 5000){           //
            myservo.write(120, 200);      //if the interaction with the drawer is longer than 5s, go to angle 120 at speed 200
            delay(100);                   //hold for 0.1s
            myservo.write(180);           //note: when speed unspecified on myservo.write(angle, speed), speed is taken to be 255 (max)
            delay(200);                   //
            myservo.detach();             //      
          }
  
       }
       else if (buttonPushCounter %2==0){    //if the button counter is a multiple of two
          myservo.attach(pinMOTOR);           
          fwd(135, 30);
          currentMillis = millis();         
          startMillis = currentMillis;          //restart time since last interaction
          if(pressTime > 2500){                 //if the interaction is longer than 2.5s
            myservo.write(0);                   //go to start position
            myservo.detach();
          }
       }
       else{   //all other interactions
        myservo.attach(pinMOTOR);
          fwd(90, 100);
          currentMillis = millis();
          startMillis = currentMillis;           //restart timer since last interaction
          if(pressTime > 2000){                  //if the interaction is longer than 2s
            myservo.write(0);                    //go to start position
            myservo.detach();
          buttonPushCounter = 0;                 //buttonPushCounter is reset and personality loop is reset
          }
       }
    }
       else if (firstTime == 0){       
        firstTime = 1;        //reset timer for length of time button is pressed for
        Serial.println("Time: 0 milleseconds; 0 seconds");
       }
  
    if (buttonState == HIGH){         //if button/IR not triggered
       bkwd(180, 150);                //go back to start position
       delay(50);                     //allow time for drawer to return 
    }
    //--------------------------------------------------------------------------------//
}

  void grumpy() {
    //-----------------------------GRUMPY PERSONALITY CYCLE---------------------------//
    
    buttonState = digitalRead(pinIR);
    if (buttonState != lastButtonState) {
      if (buttonState == LOW){
        buttonPushCounter++;
        Serial.println("on");
        Serial.print("number of button pushes: ");
        Serial.println(buttonPushCounter);
      }
      else{
        Serial.println("off");
      }
    }
    
    lastButtonState = buttonState;
  
    if (buttonState == LOW){
       if(firstTime == 1){
          startTime = millis();    
          firstTime = 0;
       }
  
       pressTime = millis() - startTime;  
  
       if (pressTime >= 1){               
          Serial.print("Time: ");
          Serial.print(pressTime);
          Serial.print(" milliseconds ");
          Serial.print(int(pressTime/1000));
          Serial.println(" seconds");
       }

       if (buttonPushCounter == 1 ){        //if its the first interaction
          currentMillis = millis();
          myservo.attach(pinMOTOR);
          fwd(100, 50);
          startMillis = currentMillis;  
          if(pressTime > 2000){
            bkwd(100, 50);
          }
  
       }
       
       else if (buttonPushCounter == 2){    //if its the second interaction
          currentMillis = millis();
          myservo.attach(pinMOTOR);
          fwd(100, 200);
          currentMillis = millis();
          startMillis = currentMillis;
          if(pressTime > 2500){
            myservo.write(0, 200);
          }
       }
       
       else{                                //all other interactions
          currentMillis = millis();
          myservo.attach(pinMOTOR);
          myservo.write(90, 180,true);      //move to position 90, with a speed of 180, wait until move is complete
          delay(200);                       
          myservo.write(0, 180);            
          startMillis = currentMillis;
          buttonPushCounter = 0;
       }
    }
    
       else if (firstTime == 0){  
        firstTime = 1;
        Serial.println("Time: 0 milleseconds; 0 seconds");
       }
  
    if (buttonState == HIGH){               //if button not pressed return to start position
       bkwd(180, 150);
       delay(50);
    }
    //--------------------------------------------------------------------------------//
}

  void angry() {
    //------------------------------SAD PERSONALITY CYCLE-----------------------------//
    
    buttonState = digitalRead(pinIR);
    if (buttonState != lastButtonState) {
      if (buttonState == LOW){
        buttonPushCounter++;
        Serial.println("on");
        Serial.print("number of button pushes: ");
        Serial.println(buttonPushCounter);
      }
      else{
        Serial.println("off");
      }
    }
    
    lastButtonState = buttonState;
  
    if (buttonState == LOW){
       if(firstTime == 1){
          startTime = millis();      
          firstTime = 0;
       }
  
       pressTime = millis() - startTime;   
  
       if (pressTime >= 1){               
          Serial.print("Time: ");
          Serial.print(pressTime);
          Serial.print(" milliseconds ");
          Serial.print(int(pressTime/1000));
          Serial.println(" seconds");
       }
  
       if (buttonPushCounter % 2 == 0 ){     //every second interaction
          myservo.attach(pinMOTOR);
          delay(1000);         //even slower reaction to user on second interaction
          fwd(180, 250);
          currentMillis = millis();
          startMillis = currentMillis;
          if(pressTime > 1000){
            bkwd(50,30);
          }
       }

       else{    //every first interaction
          myservo.attach(pinMOTOR);
          delay(1000);   //"reluctant" 500ms delayed reaction to input
          fwd(155, 255);
          currentMillis = millis();
          startMillis = currentMillis;    //restart timer since last interaction
          if(pressTime > 2500){
            bkwd(100, 30);
          }
       }
    }
       else if (firstTime == 0){     
        firstTime = 1;
        Serial.println("Time: 0 milleseconds; 0 seconds");
       }
  
    if (buttonState == HIGH){         
       bkwd(180, 150);
       delay(50);
    }
    //--------------------------------------------------------------------------------//
}

  void scared1() {
    //-----------------------------SCARED PERSONALITY CYCLE---------------------------//
    
    buttonState = digitalRead(pinIR);
    if (buttonState != lastButtonState) {
      if (buttonState == LOW){
        buttonPushCounter++;
        Serial.println("on");
        Serial.print("number of button pushes: ");
        Serial.println(buttonPushCounter);
      }
      else{
        Serial.println("off");
      }
    }
    
    lastButtonState = buttonState;
  
    if (buttonState == LOW){
       if(firstTime == 1){
          startTime = millis();      
          firstTime = 0;
       }
  
       pressTime = millis() - startTime;   
  
       if (pressTime >= 1){               
          Serial.print("Time: ");
          Serial.print(pressTime);
          Serial.print(" milliseconds ");
          Serial.print(int(pressTime/1000));
          Serial.println(" seconds");
       }
  
       if (buttonPushCounter % 1 == 0 ){     //every interaction
          myservo.attach(pinMOTOR);
          myservo.write(180, 250, true);
          delay(1000);
          currentMillis = millis();
       }
    }
       else if (firstTime == 0){     
        firstTime = 1;
        Serial.println("Time: 0 milleseconds; 0 seconds");
       }
  
    if (buttonState == HIGH){         
       bkwd(180, 20);
       delay(50);
    }
    //--------------------------------------------------------------------------------//
}

  void scared2() {
    //--------------------------2ND SCARED PERSONALITY CYCLE--------------------------//
    
    buttonState = digitalRead(pinIR);
    if (buttonState != lastButtonState) {
      if (buttonState == LOW){
        buttonPushCounter++;
        Serial.println("on");
        Serial.print("number of button pushes: ");
        Serial.println(buttonPushCounter);
      }
      else{
        Serial.println("off");
      }
    }
    
    lastButtonState = buttonState;
  
    if (buttonState == LOW){
       if(firstTime == 1){
          startTime = millis();      
          firstTime = 0;
       }
  
       pressTime = millis() - startTime;   
  
       if (pressTime >= 1){               
          Serial.print("Time: ");
          Serial.print(pressTime);
          Serial.print(" milliseconds ");
          Serial.print(int(pressTime/1000));
          Serial.println(" seconds");
       }
  
       if (buttonPushCounter % 1 == 0 ){     //every interaction
          myservo.attach(pinMOTOR);
          myservo.write(180, 250, true);
          delay(1000);
          currentMillis = millis();
          startMillis = currentMillis;    //restart timer since last interaction
          if(pressTime > 2500){
            bkwd(180, 250);
            delay(10);
            fwd(180,250);
          }
       }
    }
       else if (firstTime == 0){     
        firstTime = 1;
        Serial.println("Time: 0 milleseconds; 0 seconds");
       }
  
    if (buttonState == HIGH){         
       bkwd(180, 20);
       delay(50);
    }
    //--------------------------------------------------------------------------------//
  }

  void tricky() {
    //----------------------------TRICKY PERSONALITY CYCLE----------------------------//
    
    buttonState = digitalRead(pinIR);
    if (buttonState != lastButtonState) {
      if (buttonState == LOW){
        buttonPushCounter++;    //increments a button counter every time the button/IR is triggered
        Serial.println("on");
        Serial.print("number of button pushes: ");
        Serial.println(buttonPushCounter);
      }
      else{
        Serial.println("off");
      }
    }
    
    lastButtonState = buttonState;
  
    if (buttonState == LOW){
       if(firstTime == 1){
          startTime = millis();    
          firstTime = 0;
       }
  
       pressTime = millis() - startTime;   //time button is being pressed for
  
       if (pressTime >= 1){               //Counts and displays length of time button is pressed for. used for debugging
          Serial.print("Time: ");
          Serial.print(pressTime);
          Serial.print(" milliseconds ");
          Serial.print(int(pressTime/1000));
          Serial.println(" seconds");
       }
  
       if (buttonPushCounter == 1 ){    
          myservo.attach(pinMOTOR);         
          delay(2000);                           
          myservo.write(180, 50, true);
          currentMillis = millis();
          startMillis = currentMillis;    //restart timer since last interaction
       }
       
       else if(buttonPushCounter == 2){    //second interaction
          myservo.attach(pinMOTOR);                                               
          myservo.write(0, 200, true);
          currentMillis = millis();
          startMillis = currentMillis;   
          buttonPushCounter = 0;            //restart time since last interaction
       }
    }
       else if (firstTime == 0){       
        firstTime = 1;        //reset timer for length of time button is pressed for
        Serial.println("Time: 0 milleseconds; 0 seconds");
       }
    //--------------------------------------------------------------------------------//
  }

//---------------------------MOTOR DIRECTION FUNCTIONS----------------------------//
int bkwd(int p, const int v)          //backward move function
{
 for(pos = 0; pos < p; pos += p)  // goes from p degrees to 0 degrees 
 {                                  
   myservo.write(pos, v);              // tell servo to go to position 'pos', at speed v 
   delay(50);                       // waits 50ms for the servo to reach the position 
 }   
}

int fwd(int p, const int v)          //forward move function
{
 for(pos = p; pos>=p; pos-=p)     // goes from 0 degrees to p degrees 
 {                                
   myservo.write(pos, v);              // tell servo to go to position 'pos', at speed v 
   delay(50);                        // waits 50ms for the servo to reach the position 
 } 
}
//--------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------//
};



//---------------------------------------------MAIN PROGRAMME---------------------------------------------------//

//Drawer class instances for all the drawers on the Gizmo
Drawer drawer1 = Drawer(PIN_IR1, PIN_MOTOR1);   //top right
Drawer drawer2 = Drawer(PIN_IR3, PIN_MOTOR2);   //top left
Drawer drawer3 = Drawer(PIN_IR2, PIN_MOTOR3);   //top mid
Drawer drawer4 = Drawer(PIN_IR4, PIN_MOTOR4);   //bottom right
Drawer drawer5 = Drawer(PIN_IR5, PIN_MOTOR5);   //bottom mid    
Drawer drawer6 = Drawer(PIN_IR6, PIN_MOTOR6);   //bottom left

void setup() {
  drawer1.enable();
  drawer2.enable();
  drawer3.enable();
  drawer4.enable();
  drawer5.enable();
  drawer6.enable();
}

void loop() {
  drawer1.scared2();  //top right
  drawer2.grumpy();   //top left  
  drawer3.angry();    //top mid 
  drawer4.shy();      //bottom right
  drawer5.tricky();   //bottom mid
  drawer6.scared1();  //bottom left
}

//--------------------------------------------------------------------------------------------------------------//
