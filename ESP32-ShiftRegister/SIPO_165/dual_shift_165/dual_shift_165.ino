const int clk = 13;    // SN74HC165N pin 2, CLK
const int dataP = 12;   // SN74HC165N pin 9, QH
const int lock = 18;   // SN74HC165N pin 1, SH/LD
// SN74HC165N CLK INH connected to GND
// SN74HC165N A to H connected to GND or VDD, see question
// SN74HC165N _QH, connected to GND
// SN74HC165N SER, connected to GND

void setup() 
{
  Serial.begin(115200);
  pinMode(clk, OUTPUT);  
  pinMode(dataP, INPUT);   
  pinMode(lock, OUTPUT);  


  
}

void loop() 
{
  long bitVal;
  // This line made it work: Turn clock HIGH before sampling
  digitalWrite(clk, HIGH);

  // Sample the parallel input
  digitalWrite(lock, LOW);
  delayMicroseconds(20);
  digitalWrite(lock, HIGH);
  /*
  // Read
  byte b = shiftIn(dataP, clk, MSBFIRST);
  Serial.println(b);
*/
  

  for(int i = 0; i < 16; i++)
    {
        

        /* Pulse the Clock (rising edge shifts the next bit).
        */
        digitalWrite(clk, HIGH);
        delayMicroseconds(20);
        digitalWrite(clk, LOW);

        bitVal = digitalRead(dataP);

        /* Set the corresponding bit in bytesVal.
        */
        Serial.print(bitVal);
    }
    Serial.println("");

    delay(500);
}