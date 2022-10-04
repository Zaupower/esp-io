#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>     // Include the Wi-Fi library

#define LED 2

AsyncWebServer server(80);

//Ligar hotspot do portatil e adicionar os seguintes campos
//dns http://serial_out_IP/webserial
const char* ssid     = "PFMQ052";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "qeGaJRmZ";     // The password of the Wi-Fi network

bool printLogs = false;
void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  
  WebSerial.println(d);
  if (d == "ON"){
    printLogs = true;
    digitalWrite(LED, HIGH);
  }
  if (d=="OFF"){
    printLogs = false;
    digitalWrite(LED, LOW);
  }
}

void setup() {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
  
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer

  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin();
}

String command = "";

void loop() { 
  if(Serial.available()){
      command = Serial.readStringUntil('\n'); 
      WebSerial.println(command);
  }
  Serial.println(command);
  delay(1000);
}
