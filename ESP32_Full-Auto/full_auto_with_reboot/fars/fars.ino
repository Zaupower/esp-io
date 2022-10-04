#include <WiFi.h>
//include storage persistance 
#include <EEPROM.h>

//number of bytes to acces nvs
#define EEPROM_SIZE 1

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define buttonPlay 5

//General IO

#define buttonReboot 12

//SPINS
#define buttonGallery 4
#define buttonPlay1 18
#define buttonPlay5 16
#define buttonPlay10 17

//BINGO
#define buttonAbreTodos 22
#define buttonAbreSeq 23
#define buttonAposta 19
#define buttonCambiar 21

//#define ledGallery 19
#define ledPlay 15


//Reboot range time in minutes
const int REBOOTTIMEMINRANGE = 5;
const int REBOOTTIMEMAXRANGE = 1000;


// Replace with your network credentials
const char *ssid = "ESP1";
const char *password = "fmq.1234";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

const int buttonPin = 0;     // the number of the pushbutton pin
const int ledPin = 2;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status


int button = 0;
int modePlay = 1;

int modePlay1 = 1;
int modePlay5 = 1;
int modePlay10 = 1;
int modeGallery = 1;

int modeAbreTodos = 1;
int modeAbreSeq = 1;
int modeAposta = 1;
int modeCambiar = 1;

//if gameType = 0 Bingo; if gameType = 1 Slot
int gameType = 0;

// define two tasks for Blink & AnalogRead
void TaskBlink(void *pvParameters);
void TaskAnalogReadA3(void *pvParameters);

void rebootService(void *pvParameters);
void rebootService2(void *pvParameters);

void rebootService2(void *pvParameters){

  
    int playQ = random(REBOOTTIMEMINRANGE,REBOOTTIMEMAXRANGE);
    
    if(playQ > 990){
      digitalWrite(buttonReboot, HIGH); 
      delay(2000);
      digitalWrite(buttonReboot, LOW);
    }
    
  
  }

void rebootService(void *pvParameters){

  Serial.println("Start Reboot function");
  
  for(;;){
    int delayTime = random(REBOOTTIMEMINRANGE,REBOOTTIMEMAXRANGE);
    
    delayTime = delayTime * 120000;
    
    Serial.println("Delay: ");
    Serial.println(delayTime);
    delay(delayTime);        
    digitalWrite(buttonReboot, HIGH); 
    delay(2000);
    digitalWrite(buttonReboot, LOW);
  }
}
void setup()
{

    //init EEPROM with predefined size
    EEPROM.begin(EEPROM_SIZE);
    gameType = EEPROM.read(0);

    // initialize serial communication at 115200 bits per second:
    Serial.begin(115200);
    Serial.println("Starting BLE work!");

    // initialize the pushbutton pin as an input:
    pinMode(buttonPin, INPUT);
    pinMode(ledPin,OUTPUT); // LED output

    pinMode(buttonReboot, OUTPUT);
    digitalWrite(buttonReboot, LOW);
    
    // Connect to Wi-Fi network with SSID and password
    Serial.print("Setting AP (Access Point)…");
    // Remove the password parameter, if you want the AP (Access Point) to be open
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.begin();

    //Add reboot 
    //rebootMachhine();

    xTaskCreatePinnedToCore(rebootService,"rebootService",10000,NULL,2,NULL,1);

   
    delay(300);
    // Now set up two tasks to run independently.
    xTaskCreatePinnedToCore(
        TaskBlink, "TaskBlink" // A name just for humans
        ,
        1024 // This stack size can be checked & adjusted by reading the Stack Highwater
        ,
        NULL, 2 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,
        NULL, ARDUINO_RUNNING_CORE);

    xTaskCreatePinnedToCore(
        TaskBlink2, "TaskBlink" // A name just for humans
        ,
        1024 // This stack size can be checked & adjusted by reading the Stack Highwater
        ,
        NULL, 2 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,
        NULL, ARDUINO_RUNNING_CORE);

    xTaskCreatePinnedToCore(
        TaskBlink3, "TaskBlink" // A name just for humans
        ,
        1024 // This stack size can be checked & adjusted by reading the Stack Highwater
        ,
        NULL, 2 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,
        NULL, ARDUINO_RUNNING_CORE);

    xTaskCreatePinnedToCore(
        TaskBlink4, "AnalogReadA3", 1024 // Stack size
        ,
        NULL, 1 // Priority
        ,
        NULL, ARDUINO_RUNNING_CORE);
    xTaskCreatePinnedToCore(
        TaskBlink5, "AnalogReadA3", 1024 // Stack size
        ,
        NULL, 1 // Priority
        ,
        NULL, ARDUINO_RUNNING_CORE);

      pinMode(ledPlay, OUTPUT);
    // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void rebootMachhine(){
  Serial.println("Start Reboot function");
  
  while(true){
    int delayTime = random(10,30);
    
    delayTime = delayTime * 9000;
    
    Serial.println("Delay: ");
    Serial.println(delayTime);
    delay(delayTime);        
    digitalWrite(buttonReboot, HIGH); 
    delay(2000);
    digitalWrite(buttonReboot, LOW);
  }
}

void loop()
{

  //    if(strcmp(pCharacteristic->getValue().c_str(),new_value.c_str()) !=0 )
  // {
  //  new_value = pCharacteristic->getValue().c_str();
  //    Serial.println(new_value);
  // }

    if (gameType == 0) {
      digitalWrite(15,LOW);
    } else {
      digitalWrite(15,HIGH);
    }
    // read the state of the pushbutton value:
    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW) {
      if (gameType == 0) {
        // if to change game based on actual game
        Serial.println("gameType changed to SLOT");
        gameType = 1; 
      } else {
        Serial.println("gameType changed to BINGO");
        gameType = 0;
      }
       delay(1000);
    }
      
   
    WiFiClient client = server.available(); // Listen for incoming clients

    if (client)
    { // If a new client connects,
        currentTime = millis();
        previousTime = currentTime;
        //WiFiClient client = server.available(); // Listen for incoming clients

        if (client)
        {                                  // If a new client connects,
            Serial.println("New Client."); // print a message out in the serial port
            String currentLine = "";       // make a String to hold incoming data from the client
            while (client.connected())
            { // loop while the client's connected
                if (client.available())
                {                           // if there's bytes to read from the client,
                    char c = client.read(); // read a byte, then
                    Serial.write(c);        // print it out the serial monitor
                    header += c;
                    if (c == '\n')
                    { // if the byte is a newline character
                        // if the current line is blank, you got two newline characters in a row.
                        // that's the end of the client HTTP request, so send a response:
                        if (currentLine.length() == 0)
                        {
                            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                            // and a content-type so the client knows what's coming, then a blank line:
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:text/html");
                            client.println("Connection: close");
                            client.println();

                            // turns the Game bingo and spin
                            if (header.indexOf("GET /G/on") >= 0)
                            {
                                Serial.println("gameType = 0");
                                gameType = 0;
                                //save to nvs
                                EEPROM.write(0,0);
                                EEPROM.commit();
                                digitalWrite(15,HIGH);
                            }
                            else if (header.indexOf("GET /G/off") >= 0)
                            {
                                Serial.println("gameType = 1");
                                gameType = 1;
                                
                                //save to nvs
                                EEPROM.write(0,1);
                                EEPROM.commit();
                                digitalWrite(15,LOW);
                            }
                            // turns the GPIOs on and off
                            else if (header.indexOf("GET /4/on") >= 0)
                            {
                                Serial.println("modeGallery/modeAposta = 0");
                                modeGallery = 0;
                                modeAposta = 0;
                            }
                            else if (header.indexOf("GET /4/off") >= 0)
                            {
                                Serial.println("modeGallery/modeAposta = 1");
                                modeGallery = 1;
                                modeAposta = 1;
                            }
                            else if (header.indexOf("GET /4/press") >= 0)
                            {
                                Serial.println("Press Gallery/Aposta");
                                digitalWrite(4, LOW);
                                digitalWrite(19, LOW);
                                delay(100);
                                digitalWrite(4, HIGH);
                                digitalWrite(19, HIGH);
                            }
                            else if (header.indexOf("GET /5/on") >= 0)
                            {
                                Serial.println("modePlay = 0");
                                modePlay = 0;
                            }
                            else if (header.indexOf("GET /5/off") >= 0)
                            {
                                Serial.println("modePlay = 1");
                                modePlay = 1;
                            }
                            else if (header.indexOf("GET /5/press") >= 0)
                            {
                                Serial.println("Press Play");
                                digitalWrite(5, LOW);
                                delay(100);
                                digitalWrite(5, HIGH);
                            }    
                            else if (header.indexOf("GET /16/on") >= 0)
                            {
                                Serial.println("modePlay5/modeCambiar = 0");
                                modePlay5 = 0;
                                modeCambiar = 0;
                            }
                            else if (header.indexOf("GET /16/off") >= 0)
                            {
                                Serial.println("modePlay5/modeCambiar = 1");
                                modePlay5 = 1;
                                modeCambiar = 1;
                            }
                            else if (header.indexOf("GET /16/press") >= 0)
                            {
                                Serial.println("Press Play 5 / Cambiar");
                                digitalWrite(16, LOW);
                                digitalWrite(21, LOW);
                                delay(100);
                                digitalWrite(16, HIGH);
                                digitalWrite(21, HIGH);
                            }  
                            else if (header.indexOf("GET /17/on") >= 0)
                            {
                                Serial.println("modePlay10/modeAbreTodos = 0");
                                modePlay10 = 0;
                                modeAbreTodos = 0;
                            }
                            else if (header.indexOf("GET /17/off") >= 0)
                            {
                                Serial.println("modePlay10/modeAbreTodos = 1");
                                modePlay10 = 1;
                                modeAbreTodos = 1;
                            }
                            else if (header.indexOf("GET /17/press") >= 0)
                            {
                                Serial.println("Press Play 10 / Abre Todos");
                                digitalWrite(17, LOW);
                                digitalWrite(22, LOW);
                                delay(100);
                                digitalWrite(17, HIGH);
                                digitalWrite(22, HIGH);
                            }  
                            else if (header.indexOf("GET /18/on") >= 0)
                            {
                                Serial.println("modePlay1/modeAbreSeq = 0");
                                modePlay1 = 0;
                                modeAbreSeq = 0;
                            }
                            else if (header.indexOf("GET /18/off") >= 0)
                            {
                                Serial.println("modePlay1/modeAbreSeq = 1");
                                modePlay1 = 1;
                                modeAbreSeq = 1;
                            }
                            else if (header.indexOf("GET /18/press") >= 0)
                            {
                                Serial.println("Press Play 1 / Abre Sequencial");
                                digitalWrite(18, LOW);
                                digitalWrite(23, LOW);
                                delay(100);
                                digitalWrite(18, HIGH);
                                digitalWrite(23, HIGH);
                            }  
                       

                            // Display the HTML web page
                            client.println("<!DOCTYPE html><html>");
                            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                            client.println("<link rel=\"icon\" href=\"data:,\">");
                            // CSS to style the on/off buttons
                            // Feel free to change the background-color and font-size attributes to fit your preferences
                            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
                            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                            client.println(".slider { width: 300px; }");
                            client.println(".button2 {background-color: #555555;}</style></head>");
                            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");

                            // Web Page Heading
                            client.println("<body><h1>ESP</h1>");
                            client.println("<body><h2>da Desgraca</h2>");
// Display current game, and 0/1 buttons for Bingo / Slot
                            client.println("<p>Slot(1) | Bingo(0)- State " + String(gameType) + "</p>");
                            // If the gameType is 0, Bingo is Displayed
                            if (gameType == 1)
                            {
                                client.println("<p><a href=\"/G/on\"><button class=\"button\">CHANGE TO BINGO</button></a></p>");
                       
                            }
                            else
                            {
                                client.println("<p><a href=\"/G/off\"><button class=\"button button2\">CHANGE TO SLOT</button></a></p>");
                            }
// Display current state, and ON/OFF buttons for Gallery/Aposta
                            client.println("<p>Gallery/Aposta - State " + String(modeGallery) + "</p>");
                            // If the output4State is off, it displays the ON button
                            if (modeGallery == 1)
                            {
                                client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
                            }
                            else
                            {
                                client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
                            }
// Display current state, and ON/OFF buttons for Play
                            client.println("<p>Play - State " + String(modePlay) + "</p>");
                            // If the output5State is off, it displays the ON button
                            if (modePlay == 1)
                            {
                                client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
                            }
                            else
                            {
                                client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
                            }

// Display current state, and ON/OFF buttons for Play1/AbreSeq
                            client.println("<p>Play1/AbreSeq - State " + String(modePlay1) + "</p>");
                            // If the output18State is off, it displays the ON button
                            if (modePlay1 == 1)
                            {
                                client.println("<p><a href=\"/18/on\"><button class=\"button\">ON</button></a></p>");
                            }
                            else
                            {
                                client.println("<p><a href=\"/18/off\"><button class=\"button button2\">OFF</button></a></p>");
                            }
                            
// Display current state, and ON/OFF buttons for Play5/Cambiar
                            client.println("<p>Play5/Cambiar - State " + String(modePlay5) + "</p>");
                            // If the output16State is off, it displays the ON button
                            if (modePlay5 == 1)
                            {
                                client.println("<p><a href=\"/16/on\"><button class=\"button\">ON</button></a></p>");
                            }
                            else
                            {
                                client.println("<p><a href=\"/16/off\"><button class=\"button button2\">OFF</button></a></p>");
                            }

// Display current state, and ON/OFF buttons for Play10/AbreTodas
                            client.println("<p>Play10/AbreTodas - State " + String(modePlay10) + "</p>");
                            // If the output17State is off, it displays the ON button
                            if (modePlay10 == 1)
                            {
                                client.println("<p><a href=\"/17/on\"><button class=\"button\">ON</button></a></p>");
                            }
                            else
                            {
                                client.println("<p><a href=\"/17/off\"><button class=\"button button2\">OFF</button></a></p>");
                            }

                            client.println("</body></html>");

                            // The HTTP response ends with another blank line
                            client.println();
                            // Break out of the while loop
                            break;
                        }
                        else
                        { // if you got a newline, then clear currentLine
                            currentLine = "";
                        }
                    }
                    else if (c != '\r')
                    {                     // if you got anything else but a carriage return character,
                        currentLine += c; // add it to the end of the currentLine
                    }
                }
            }
            // Clear the header variable
            header = "";
            // Close the connection
            //client.stop();
            Serial.println("Client disconnected.");
            Serial.println("");
        }
    }
}
    /*--------------------------------------------------*/
    /*---------------------- Tasks ---------------------*/
    /*--------------------------------------------------*/

    void TaskBlink(void *pvParameters) // This is a task.
    {
        (void)pvParameters;
        int IO;
        for (;;) // A Task shall never return or exit.
        {
            //Timers between inputs
            int waiTimeBlink1 = 1000;

            if (modePlay == 0)
            {
               vTaskDelay(500); 
               continue;
            }
            if (gameType == 1)
            {
              waiTimeBlink1 = waiTimeBlink1;
            } else {
              waiTimeBlink1 = waiTimeBlink1*4;
            }
            IO = buttonPlay;
            pinMode(IO, OUTPUT);
            digitalWrite(ledPlay, HIGH);           // turn the LED ob by making the voltage HIGH
            digitalWrite(IO, LOW);                    // turn the LED off by making the voltage LOW
            vTaskDelay(100);                          // one tick delay (15ms) in between reads for stability
            digitalWrite(ledPlay, LOW);            // turn the LED off by making the voltage LOW
            digitalWrite(IO, HIGH);                   // turn the LED on (HIGH is the voltage level)
            //xTaskCreatePinnedToCore(rebootService2,"rebootService2",1000,NULL,2,NULL,1); //maybe reboot
            vTaskDelay(waiTimeBlink1 + random(500, 2500)); // one tick delay (15ms) in between reads for stability
        }
    }

    void TaskBlink2(void *pvParameters) // This is a task.
    {
        (void)pvParameters;

        int IO;
        for (;;) // A Task shall never return or exit.
        {
            //Timers between inputs
            int waiTimeBlink2 = 1000;
            if (modePlay10 == 0)
            {
               vTaskDelay(500); 
               continue;
            }
            if (gameType == 1)
            {
              IO = buttonPlay10;
              waiTimeBlink2 = waiTimeBlink2;
            } else {
              IO = buttonAbreTodos;
              waiTimeBlink2 = waiTimeBlink2/4;
            }
            
            pinMode(IO, OUTPUT);
            digitalWrite(IO, LOW);                    // turn the LED off by making the voltage LOW
            vTaskDelay(100);                          // one tick delay (15ms) in between reads for stability
            digitalWrite(IO, HIGH);                   // turn the LED on (HIGH is the voltage level)
            xTaskCreatePinnedToCore(rebootService2,"rebootService2",1000,NULL,2,NULL,1); //maybe reboot
            vTaskDelay(waiTimeBlink2 + random(500, 1000)); // one tick delay (15ms) in between reads for stability
        }
    }

    void TaskBlink3(void *pvParameters) // This is a task.
    {
        (void)pvParameters;

        int IO;

        for (;;) // A Task shall never return or exit.
        {
            //Timers between inputs
            int waiTimeBlink3 = 1000;
            if (modePlay5 == 0)
            {
               vTaskDelay(500); 
               continue;
            }
            if (gameType == 1)
            {
              IO = buttonPlay5;
            } else {
              IO = buttonCambiar;
            }
            pinMode(IO, OUTPUT);
            digitalWrite(IO, LOW);                    // turn the LED off by making the voltage LOW
            vTaskDelay(100);                          // one tick delay (15ms) in between reads for stability
            digitalWrite(IO, HIGH);                   // turn the LED on (HIGH is the voltage level)
            xTaskCreatePinnedToCore(rebootService2,"rebootService2",1000,NULL,2,NULL,1); //maybe reboot
            vTaskDelay(waiTimeBlink3 + random(700, 1200)); // one tick delay (15ms) in between reads for stability    digitalWrite(IO, LOW);    // turn the LED off by making the voltage LOW 
        }
    }

    void TaskBlink4(void *pvParameters) // This is a task.
    {
        (void)pvParameters;

        int IO;
        for (;;) // A Task shall never return or exit.
        {
            //Timers between inputs
            int waiTimeBlink4 = 1000;
            if (modeGallery == 0)
            {     
               vTaskDelay(500); 
               continue;
            }
            if (gameType == 1)
            {
              IO = buttonGallery;
            } else {
              IO = buttonAposta;
            }
            pinMode(IO, OUTPUT);
            digitalWrite(IO, LOW);                    // turn the LED off by making the voltage LOW
            vTaskDelay(100);                          // one tick delay (15ms) in between reads for stability
            digitalWrite(IO, HIGH);                   // turn the LED on (HIGH is the voltage level)

            xTaskCreatePinnedToCore(rebootService2,"rebootService2",1000,NULL,2,NULL,1); //maybe reboot
            
            vTaskDelay(waiTimeBlink4 + random(500, 1000)); // one tick delay (15ms) in between reads for stability    digitalWrite(IO, LOW);    // turn the LED off by making the voltage LOW
        }
    }

    void TaskBlink5(void *pvParameters) // This is a task.
    {
        (void)pvParameters;

        int IO;

        for (;;) // A Task shall never return or exit.
        {
            //Timers between inputs
            int waiTimeBlink5 = 1000;
            if (modePlay1 == 0)
            {
               vTaskDelay(500); 
               continue;
            }
            if (gameType == 1)
            {
              IO = buttonPlay1;
            } else {
              IO = buttonAbreSeq;
            }
            pinMode(IO, OUTPUT);
            digitalWrite(IO, LOW);                    // turn the LED off by making the voltage LOW
            vTaskDelay(100);                          // one tick delay (15ms) in between reads for stability
            digitalWrite(IO, HIGH);                   // turn the LED on (HIGH is the voltage level)
            xTaskCreatePinnedToCore(rebootService2,"rebootService2",1000,NULL,2,NULL,1); //maybe reboot
            vTaskDelay(waiTimeBlink5 + random(500, 1000)); // one tick delay (15ms) in between reads for stability    digitalWrite(IO, LOW);    // turn the LED off by making the voltage LOW
        }
    }
