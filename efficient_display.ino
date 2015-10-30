//Some code used here was taken from 'https://learn.adafruit.com/photocells/using-a-photocell', it was then repurposed to be used on many different FSRs at the same time. The output data was then analyzed and used to create a real-time display of force on each FSR.

int input = 50; // the FSR and cap are connected to pin50
int reset = 49;
int *fsrReading; // the digital readings
int myPins[] = {27, 28, 31, 32, 35, 36, 39, 40, 43, 44}; // The pins used as VCC for the FSRs
int minmax[] = {23, 24}; // The resistors used to find a min and max count
int *rcounts; // The counts from the resistors


void setup(void) {
// We'll send debugging information via the Serial monitor
Serial.begin(9600);

 for (int counter = 0; counter<10; counter++){//set FSR VCC pins to output mode and low state
  pinMode(myPins[counter], OUTPUT);
  digitalWrite(myPins[counter], LOW);
  } 
  
for (int counter = 0; counter<2; counter++){//set resistor VCC pins to output mode and low state
  pinMode(minmax[counter], OUTPUT);
  digitalWrite(minmax[counter], LOW);
  }

pinMode(reset,OUTPUT);
}

void loop(void) {
// read the resistor using the RCtime technique

rcounts = getminmax(minmax, input, reset);

fsrReading = RCtime(input, myPins, reset);

Serial.print("Resistors: ");
Serial.print(rcounts[0]); // the raw analog reading
Serial.print(" ");
Serial.print(rcounts[1]);
Serial.print(" FSRs: ");

for (int count = 0; count<10; count++){//4th fsr is working fsr INDEX FINGER
Serial.print(fsrReading[count]); // the raw analog reading
Serial.print(" ");
}
Serial.println();

}

// Uses a digital pin to measure a resistor (like an FSR or photocell!)
// We do this by having the resistor feed current into a capacitor and
// counting how long it takes to get to Vcc/2 (for most arduinos, thats 2.5V)
int *RCtime(int RCpin, int pins[], int reset) {
int reading[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // start with 0s

for (int count=0; count<10; count++){
digitalWrite(reset, HIGH);  //set reset to high
// set the pin to an output and pull to LOW (ground)
pinMode(RCpin, OUTPUT);
digitalWrite(RCpin, LOW);


// Now set the pin to an input and...
pinMode(RCpin, INPUT);


digitalWrite(pins[count], HIGH);//Set VCC to FSR input

while (digitalRead(RCpin) == LOW) { // count how long it takes to rise up to HIGH
reading[count]++; // increment to keep track of time 

if (reading[count] == 30000) {
// if we got this far, the resistance is so high
// its likely that nothing is connected!
break; // leave the loop
}
}
digitalWrite(pins[count], LOW);//Set VCC pin back to low for next iteration
digitalWrite(reset, LOW);//discharge capacitor  
}
// OK either we maxed out at 30000 or hopefully got a reading, return the count after resetting FSR Vin pin

return reading;
}

int *getminmax(int minmax[], int RCpin, int reset){
  
int mmcount[] = {0, 0};

for (int counter = 0; counter<2; counter++){
digitalWrite(reset, HIGH);  
pinMode(RCpin, OUTPUT);
digitalWrite(RCpin, LOW);

// Now set the pin to an input and...
pinMode(RCpin, INPUT);

digitalWrite(minmax[counter], HIGH);//Set VCC to resistor input

while (digitalRead(RCpin) == LOW) { // count how long it takes to rise up to HIGH
mmcount[counter]++; // increment to keep track of time
}
digitalWrite(minmax[counter], LOW);//Set resistor input low
digitalWrite(reset, LOW); //discharge capacitor 
}
return mmcount;
}
