/* Permits Sonoff Basic Switch to work with a physical switch */
/* Sam Hissaund 2017 */
/* This only deals with the physical external switch - if the on-device button is pressed or Alexa turns the device on then it resets back to old state. Need to build this in to the code. */

#define SWITCH A0
#define GPIO0 12
#define GPIO12 A2 
#define REFERENCE 500
#define BUTTONHOLD 300
#define SONOFFRESPONSE 1000

void setup() {
  //Serial.begin(9600);
  pinMode(SWITCH, INPUT);
  pinMode(GPIO0, INPUT);
  pinMode(GPIO12, INPUT);
}

void loop() {
  delay(100);
  int switchValue = analogRead(SWITCH);
  int sonoffStatus = analogRead(GPIO12);

  /*Serial.println("");

  Serial.print("Switch value: ");
  Serial.println(switchValue);

  Serial.print("Sonoff status: ");
  Serial.println(sonoffStatus);*/
  
  if(switchValue >= REFERENCE)
  {
    //Switch is on
    while(sonoffStatus < REFERENCE && switchValue >= REFERENCE)
    {
      //If Sonoff is already powering the device, we won't get here. Otherwise will need to turn on the device.

      //Sink the button connection for "BUTTONHOLD"ms
      pinMode(GPIO0, OUTPUT);
      delay(BUTTONHOLD);
      pinMode(GPIO0, INPUT);
      /*Serial.println("Button pressed to turn on");
      Serial.print("Switch value: ");
      Serial.print(switchValue);
      Serial.print(", Sonoff status: ");
      Serial.println(sonoffStatus);*/
      
      //Wait for the Sonoff to respond before checking again
      delay(SONOFFRESPONSE);
      sonoffStatus = analogRead(GPIO12);
            
    }
  }
  else
  {
    //Switch is off
    while(sonoffStatus >= REFERENCE && switchValue < REFERENCE)
    {
      //If Sonoff is not powering the device, we won't get here. Otherwise will need to turn off the device.

      //Sink the button connection for "BUTTONHOLD"ms
      pinMode(GPIO0, OUTPUT);
      delay(BUTTONHOLD);
      pinMode(GPIO0, INPUT);
      /*Serial.println("Button pressed to turn off");
      Serial.print("Switch value: ");
      Serial.print(switchValue);
      Serial.print(", Sonoff status: ");
      Serial.println(sonoffStatus);*/

      //Wait for the Sonoff to respond before checking again
      delay(SONOFFRESPONSE);
      sonoffStatus = analogRead(GPIO12);
            
    }
  }
}
