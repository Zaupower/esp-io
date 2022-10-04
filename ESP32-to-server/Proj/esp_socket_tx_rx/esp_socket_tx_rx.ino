#include <WiFi.h>
 
#define SSID "PFMQ052"
#define PASSWD "00000000"

unsigned long int last_time = millis();

const uint16_t port = 1234;
const uint16_t portReceive = 5555;
const char * host = "172.21.28.77";
WiFiServer server(portReceive);

void txLEDS(void *pvParameters){
long randNumber;

   WiFiClient client2;
      if (!client2.connect(host, portReceive)){ 
        Serial.println("Falha de conexao socket");
        delay(1000);
       }
  while(1){
      randNumber = random(300);
      Serial.println("Conectado!");
      randNumber = random(10, 200);
      client2.print("A3       3434" + randNumber);
      delay(3000);
  }  
}

void rxPins(){

    delay(500);
    WiFiClient clientRx = server.available();
    while(1){
      if (clientRx.available() > 0) {
        // read the bytes incoming from the client:
        char thisChar = clientRx.read();
        
        Serial.write(thisChar);
    }   
  }
}


void setup(){
    Serial.begin(115200);
 
    WiFi.begin(SSID,PASSWD);
    while (WiFi.status() != WL_CONNECTED){delay(100);}
 
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    delay(500);
    
    //server.begin();
    
    delay(500);
    xTaskCreatePinnedToCore(txLEDS,"txLEDS",10000,NULL,2,NULL,1);
    delay(500);
    //xTaskCreatePinnedToCore(rxPins,"rxPins",10000,NULL,2,NULL,1);
    rxPins();
    
}
 
void loop(){
 delay(500);
}
