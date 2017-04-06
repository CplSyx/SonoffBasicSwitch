/* Permits Sonoff Basic Switch to work with a physical switch */
/* Sam Hissaund 2017 */

#define DEBUG 0
#define SWITCH A0
#define GPIO0 12
#define GPIO12 A2 
#define REFERENCE 500
#define BUTTONHOLD 300
#define SONOFFRESPONSE 1000
#define POLLDELAY 100

bool oldSwitchValue = getSwitchStatus();
bool oldSonoffStatus = getSonoffStatus();
int loopCount = 0; //Only used for Debugging

//******************************** Custom Functions ********************************

/*  getSwitchStatus()
 *  Performs an analog read of SWITCH and compares to REFERENCE to determine if on or off. 
 *  Returned as a bool status.
 */
bool getSwitchStatus()
{
  int i = analogRead(SWITCH);
  if (i >= REFERENCE)
  {
    return true; 
  }
  else
  {
    return false;
  }
}

/*  getSonoffStatus()
 *  Performs an analog read of GPIO12 and compares to REFERENCE to determine if on or off. 
 *  Returned as a bool status.
 */
bool getSonoffStatus()
{
  int i = analogRead(GPIO12);
  if (i >= REFERENCE)
  {
    return true; 
  }
  else
  {
    return false;
  }
}


//******************************** Main Program ********************************
void setup() {
  if(DEBUG)
  {
    Serial.begin(9600);
  }

  //Set all used pins to input - high impedance
  pinMode(SWITCH, INPUT);
  pinMode(GPIO0, INPUT);
  pinMode(GPIO12, INPUT);
}

void loop() {
  //Wait on loop - no need to poll overly fast
  delay(POLLDELAY);

  //Get the status of switch and sonoff now.
  bool switchValue = getSwitchStatus();
  bool sonoffStatus = getSonoffStatus();

  if(DEBUG)
  {
    Serial.print("Loop Count: ");
    Serial.print(loopCount);
    Serial.print(", Old Switch value: ");
    Serial.print(oldSwitchValue);
    Serial.print(", Switch value: ");
    Serial.print(switchValue);
    Serial.print(", Sonoff status: ");
    Serial.println(sonoffStatus);
  }

  //Compare the value we just got, with the one from the last loop
  if (oldSwitchValue != switchValue)
  {
    //Switch status has changed, so we need to act.

    //Does the current Sonoff status still match the old one? If so, loop.
    while(sonoffStatus == oldSonoffStatus)
    {      
      //Sink the button connection for "BUTTONHOLD"ms
      pinMode(GPIO0, OUTPUT);
      delay(BUTTONHOLD);
      pinMode(GPIO0, INPUT);

      if(DEBUG)
      {             
        Serial.println("    Button pressed.");
        
        Serial.print("    Old Switch value: ");
        Serial.print(oldSwitchValue);
        Serial.print(", Switch value: ");
        Serial.print(switchValue);
        Serial.print(", Old Sonoff status: ");
        Serial.println(oldSonoffStatus);
      }
      
      //Wait for the Sonoff to respond before checking and updating the status to determine if it has changed on the next loop.
      delay(SONOFFRESPONSE);
      sonoffStatus = getSonoffStatus();
    }

    //At this point, we've toggled the Sonoff. Set the switch status into the global variable so we can perform another comparison.
    oldSwitchValue = getSwitchStatus();
    
    
  }

  //We've done a check, but need to update the Sonoff status into the global variable in case it has been toggled externally (push button / Alexa / Wifi) so we don't reset it next time
  oldSonoffStatus = getSonoffStatus();
  loopCount++;
}
