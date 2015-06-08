/*
sovgvd@gmail.com
RC Car ESC speed limiter

D2 - RC input
D10 - RC out
A0 - potentiometer
*/

//#define SERIAL_COM_SPEED 115200
int rawIn;
static uint32_t rcTime;
static uint32_t currentTime;

byte newbit,oldbit,changed;
unsigned long startIn;
unsigned long time;
int blockedRX=0;
int potPin = 0;
int val = 0;
int rawOut=1499;
int outPinPPM=10;
int preRawOut=1499;

#define MASKPCINT0 (1<<2)
ISR(PCINT2_vect)
{
  time=micros(); 
  newbit=PIND;
  changed=newbit^oldbit;
  if (changed&MASKPCINT0)
    if (newbit&MASKPCINT0) {
      startIn=time;
    } else { 
      rawIn=time-startIn; 
    }
  oldbit=newbit;
  blockedRX=0;
}


void setup() {
  //Serial.begin(SERIAL_COM_SPEED);
  pinMode(2, INPUT);
  pinMode(outPinPPM, OUTPUT);
  PCICR |= (1 << PCIE2);
  PCMSK2 = (1 << PCINT18);
  rawIn=1499;
  delay(200);
}

void loop () {
  if (currentTime > (rcTime + 20000) ) { // 50Hz
	val = map(analogRead(potPin),0,1023,0,500);	//0...1023 to ~0...500
	//Serial.print(val);
	//Serial.print("\t");
	//Serial.print(rawIn);
        if (rawIn>2000) rawIn=2000;
        if (rawIn<1000) rawIn=1000;
	rawOut=map(rawIn,1000,2000,1000+val,2000-val);
	//Serial.print("\t");
	//Serial.print(rawOut);
        if (rawOut>2000) rawOut=2000;
        if (rawOut<1000) rawOut=1000;
        preRawOut=rawOut;
	//Serial.print("\t");
	//Serial.print(rawOut);
	//Serial.println("");
	analogWrite(outPinPPM,rawOut>>3);
  rcTime = currentTime; 
  }
  currentTime = micros();
}
