//Constants
#define number_of_74hc595s 3
#define numOfRegisterPins number_of_74hc595s * 8
#define SER_Pin 25
#define RCLK_Pin 33
#define SRCLK_Pin 32

//Variables
boolean registers [numOfRegisterPins];

void setup(){
  //Init Serial USB
  Serial.begin(115200);
  Serial.println(F("Initialize System"));
  //Init register
  pinMode(SER_Pin, OUTPUT);
  pinMode(RCLK_Pin, OUTPUT);
  pinMode(SRCLK_Pin, OUTPUT);
  clearRegisters();
  writeRegisters();
  
  delay(500);
}

void loop(){
  
  for(int i =0; i < numOfRegisterPins; i++ ){
    
    delay(100);
    setRegisterPin(i, HIGH);
    writeRegisters();
    delay(100);
    clearRegisters();
    
  }
}

void clearRegisters(){/* function clearRegisters */ 
  //// Clear registers variables 
  for(int i = numOfRegisterPins-1; i >=  0; i--){
    registers[i] = LOW;//HIGH LOW;
  }
}

void writeRegisters(){/* function writeRegisters */ 
  //// Write register after being set 
  digitalWrite(RCLK_Pin, LOW);
   
   for(int i = numOfRegisterPins-1; i >=  0; i--){
    
    digitalWrite(SRCLK_Pin, LOW); 
    int val = registers[i];
    digitalWrite(SER_Pin, val);
    digitalWrite(SRCLK_Pin, HIGH);
    
  }
  
  digitalWrite(RCLK_Pin, HIGH);
  delay(50);
}

void setRegisterPin(int index,int value){/* function setRegisterPin */ 
  ////Set register variable to HIGH or LOW
  registers[index] = value;
  
}

void writeGrpRelay(){/* function writeGrpRelay */ 
  for(int i = numOfRegisterPins-1; i >=  0; i--){
     Serial.print(F("Relay "));Serial.print(i);Serial.println(F(" HIGH"));
     setRegisterPin(i, LOW);
     writeRegisters();
     delay(200); 
     Serial.print(F("Relay "));Serial.print(i);Serial.println(F(" LOW"));
     setRegisterPin(i, HIGH);
     writeRegisters();
    delay(500);      
  }
}
