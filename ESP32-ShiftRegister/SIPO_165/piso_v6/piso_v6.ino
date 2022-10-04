
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

//PISO Variables
#define number_of_74hc165 2
#define numOfOutPins number_of_74hc165 * 8
#define clk 13   // SN74HC165N pin 2, CLK Shift1 and Shift2
#define dataP 12   // SN74HC165N pin 9, QH Shift2
#define lock 18  // SN74HC165N pin 1, SH/LD Shift1 and Shift2
// SN74HC165N CLK INH connected to GND
// SN74HC165N A to H connected to GND or VDD pin 3 to 6 and 11 to 14
// SN74HC165N _QH, connected to GND Shift1 and Shift2 pin 7
// SN74HC165N SER, connected to GND Shift2 pin 10
// SN74HC165N QH, from Shift1 to SER shift2 pin 9 Shift1 to pin 10 Shift2

//SIPO Variables
#define number_of_74hc595s 2
#define numOfRegisterPins number_of_74hc595s * 8
#define SER_Pin 25
#define RCLK_Pin 33
#define SRCLK_Pin 32

uint8_t registers [numOfRegisterPins];
uint8_t registersFullAuto [numOfRegisterPins];


//Tasks
static int taskCoreOne = 1;
TaskHandle_t task_handle_ledblink, task_handle_fullauto;

const char *ssid = "botoneirawireless";
const char *password = "12345678";

AsyncWebServer server(80);

int buttonToClick = -1;



//HTML page

// HTML web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
    <title>ESP Pushbutton Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body {
            font-family: Arial;
            text-align: center;
            margin: 0px auto;
            padding-top: 30px;
        }

        .button {
            padding: 10px 20px;
            font-size: 24px;
            text-align: center;
            outline: none;
            color: #fff;
            background-color: #2f4468;
            border: none;
            border-radius: 5px;
            box-shadow: 0 6px #999;
            cursor: pointer;
            -webkit-touch-callout: none;
            -webkit-user-select: none;
            -khtml-user-select: none;
            -moz-user-select: none;
            -ms-user-select: none;
            user-select: none;
            -webkit-tap-highlight-color: rgba(0, 0, 0, 0);
        }

        .button:hover {
            background-color: #1f2e45
        }

        .button:active {
            background-color: #1f2e45;
            box-shadow: 0 4px #666;
            transform: translateY(2px);
        }

        .content {
            display: none
        }

        .button {
            border: none;
            text-align: center;
            display: flex;
            align-items: center;
            justify-content: center;
            text-decoration: none;
            display: inline-block;
            font-size: 10px;
            margin: 4px 2px;
            width: 50px;
            height: 50px;
        }

        .buttonPlay {
            background-color: #90c6d1;
            color: rgb(0, 0, 0);
            padding: 25px;
            display: inline-block;
            font-size: 16px;
            margin: 4px 2px;
            border: 2px solid rgb(2, 2, 255);
            border-radius: 60%;
            width: 70px;
            height: 70px;

        }

        .buttonService {
            background-color: rgb(182, 215, 168);
            color: rgb(0, 0, 0);
            padding: 15px 8px;
            border: 2px solid #38ff22;
            border-radius: 12px;

        }

        .buttonGameRules {
            background-color: rgb(255, 229, 153);
            color: rgb(0, 0, 0);
            padding: 15px 8px;
            border: 2px solid rgb(255, 153, 0);
            border-radius: 12px;

        }

        .buttonCashout {
            background-color: rgb(249, 203, 156);
            color: rgb(0, 0, 0);
            padding: 15px 8px;
            border: 2px solid rgb(255, 153, 0);
            border-radius: 12px;
            width: 100px;
        }

        .buttonPlayBet {
            background-color: #A4C2F4;
            color: rgb(0, 0, 0);
            padding: 15px 8px;
            border: 2px solid #1818FF;
            border-radius: 12px;

        }

        .buttonMode {
            background-color: #EA9999;
            color: rgb(0, 0, 0);
            padding: 15px 8px;
            border: 2px solid #F83030;
            border-radius: 12px;

        }

        h2 {
            text-align: center;
        }
    </style>
</head>

<body>

    <button onclick="spinAndWinClick(1)" class="btn btn-primary" type="button">
    </button>

    <form action="/action_page.php">
        <label for="cars">Choose a button panel layout - EGM:</label>
        <select name="cars" id="cars">
            <option value="waysOccidental">WAYS - Occidental LAYOUT</option>
            <option value="waysOriental">WAYS - Oriental LAYOUT</option>
            <option value="linesOccidental">LINES - Occidental LAYOUT</option>
            <option value="linesOriental">LINES - Oriental LAYOUT</option>

            <option value="shadow">Layout Shadow</option>
            <option value="phantom">Phantom</option>
            <option value="galaxyl">Galaxy I</option>
            <option value="galaxyll">Galaxy II</option>
        </select>
    </form>

    <div id="mainContent">

    </div>

    <div class="collapse" id="WAYSOccidentalLAYOUT">
        <h2>WAYS - Occidental LAYOUT</h2>
        <table class="fixed">
            <tr>

                <td>

                </td>
            </tr>
            <tr>

                <td rowspan="2">
                    <div style="background-color: #ff00004a; text-align: center;">
                        
                    </div>
                    <button class="button" onmousedown="clickButton(1);" class="button buttonPlay">PLAY</button>
                </td>
    </div>
    </td>

    <td>
        <div style="background-color: #ff00004a; text-align: center;">
            <button class="button buttonService">SERVICE</button>
        </div>
    </td>

    <td>
        <div style="background-color: #ff00004a; text-align: center;">
            <button class="button buttonGameRules">GAME RULES</button>
        </div>
    </td>

    <td>
        <div style="background-color: #ff00004a; text-align: center;">
            <button class="button buttonPlayBet">PLAY BET 1</button>
        </div>
    </td>

    <td>
        <div style="background-color: #ff00004a; text-align: center;">
            <button class="button buttonPlayBet">PLAY BET 2</button>
        </div>
    </td>
    <td>
        <div style="background-color: #ff00004a; text-align: center;">
            <button class="button buttonPlayBet">PLAY BET 3</button>
        </div>
    </td>
    <td>
        <div style="background-color: #ff00004a; text-align: center;">
            <button class="button buttonPlayBet">PLAY BET 6</button>
        </div>
    </td>
    <td>
        <div style="background-color: #ff00004a; text-align: center;">
            <button class="button buttonPlayBet">PLAY BET 10</button>
        </div>
    </td>
    <td rowspan="2">
        <div style="background-color: #ff00004a; text-align: center;"></div>
        <button class="button buttonPlay">PLAY</button>
        </div>
    </td>

    </tr>

    <tr>
        <td colspan="2">
            <div style="background-color: #ff00004a; text-align: center;">
                <button class="button buttonCashout">CASHOUT</button>
            </div>
        </td>

        <td>
            <div style="background-color: #ff00004a; text-align: center;">
                <button class="button buttonMode">MODE 8</button>
            </div>
        </td>

        <td>
            <div style="background-color: #ff00004a; text-align: center;">
                <button class="button buttonMode">MODE 18</button>
            </div>
        </td>
        <td>
            <div style="background-color: #ff00004a; text-align: center;">
                <button class="button buttonMode">MODE 38</button>
            </div>
        </td>
        <td>
            <div style="background-color: #ff00004a; text-align: center;">
                <button class="button buttonMode">MODE 68</button>
            </div>
        </td>
        <td>
            <div style="background-color: #ff00004a; text-align: center;">
                <button class="button buttonMode">MODE 88</button>
            </div>
        </td>
    </tr>

    </table>
    </div>


    <h1>Botoneira Wireless</h1>
    <button class="button" onmousedown="toggleCheckbox();">Start Full Auto</button>
    <-- Table Test-->

        <script>
            var currentState = "unknow";
            function toggleCheckbox() {

                var xhr = new XMLHttpRequest();

                if (currentState === "unknow") {
                    currentState = "on";
                    xhr.open("GET", "/" + "on", true);
                } else {
                    if (currentState === "on") {
                        console.log("Sending state off because current state was on");
                        currentState = "off";
                        xhr.open("GET", "/" + "off", true);
                    } else {
                        console.log("Sending state on because current state was off");
                        currentState = "on";
                        xhr.open("GET", "/" + "on", true);
                    }
                }

                xhr.send();
            }
            function clickButton(button_id) {
                console.log("Clicked on button with id" + button_id);
                var xhr = new XMLHttpRequest();
                xhr.open("GET", "/clickButton", button_id);
            }
        </script>
        <script src='https://cdnjs.cloudflare.com/ajax/libs/jspdf/1.3.2/jspdf.min.js'></script>
        <script src='https://cdnjs.cloudflare.com/ajax/libs/jspdf-autotable/3.5.6/jspdf.plugin.autotable.js'></script>
        <script src='https://ajax.googleapis.com/ajax/libs/jquery/3.6.0/jquery.min.js'></script>

        <script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.4.1/js/bootstrap.min.js'></script>


  </body>
</html>)rawliteral";

//HTML page

void setup()
{
  Serial.begin(115200);

//Wifi setup
  WiFi.softAP(ssid, password);
  delay(100);
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  //PISO
  pinMode(clk, OUTPUT);
  pinMode(dataP, INPUT);
  pinMode(lock, OUTPUT);
  delay(100);

  //SIPO
  pinMode(SER_Pin, OUTPUT);
  pinMode(RCLK_Pin, OUTPUT);
  pinMode(SRCLK_Pin, OUTPUT);
  //clearRegisters();
  //writeRegisters();
  delay(100);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest * request) {
    //stopFullAuto();
  });

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest * request) {
    /*xTaskCreate(
      fullAuto,         // Function that should be called
      "Task 1",      // Name of the task (for debugging)
      10000,          // Stack size (bytes)
      NULL,          // Parameter to pass
      1,             // Task priority
      &task_handle_fullauto  // Task handle
    );*/
    request->send(200, "text/plain", "Start fullauto");
  });

  server.on("/clickButton", HTTP_GET, [](AsyncWebServerRequest * request) {
      //int buttonC = request->params();
    
      Serial.println("Value received on clickButton: ");
      //Serial.println(buttonC);
     /*
      xTaskCreate(
        clickButton,         // Function that should be called
        "clickButton",      // Name of the task (for debugging)
        1000,          // Stack size (bytes)
        (void *)buttonToClick,          // Parameter to pass
        1,             // Task priority
        NULL  // Task handle
      );*/
      
    });
    /*
  server.on("/stopfullauto", HTTP_GET, [](AsyncWebServerRequest * request) {
    vTaskDelete(task_handle_fullauto);
    request->send(200, "text/plain", "Start fullauto");
  });
 
  server.begin();
  //Start led blink state reader
  xTaskCreate(
    ledBlink,         // Function that should be called
    "ledBlink",      // Name of the task (for debugging)
    1000,          // Stack size (bytes)
    NULL,          // Parameter to pass
    1,             // Task priority
    &task_handle_ledblink  // Task handle
  );
  
 
    //Start FullAuto
    xTaskCreate(
        fullAuto,         // Function that should be called
        "full auto",      // Name of the task (for debugging)
        1000,          // Stack size (bytes)
        NULL,          // Parameter to pass
        1,             // Task priority
        &task_handle_fullauto  // Task handle
      );

     xTaskCreate(
        clickButton,         // Function that should be called
        "clickButton",      // Name of the task (for debugging)
        1000,          // Stack size (bytes)
        (void *)buttonToClick,          // Parameter to pass
        1,             // Task priority
        NULL  // Task handle
      );
  */
  server.begin();
}

void fullAuto(void * parameter) {
  while (1) {
    clearRegisters();

    writeRegisters();

    registers[0] = 1;
    writeRegisters();
    clearRegisters();

    writeRegisters();

    delay(700);
  }

}

void stopFullAuto() {
  
Serial.println("STOP FULLAUTO");
//  vTaskDelete(task_handle_fullauto);
}

void clickButton(void * parameter) {

  Serial.println("Click Button");
 /* int * button = (int *)parameter;
  Serial.println("Button Received: ");
  Serial.println(*button);
  
  if (*button >= 0 && *button < 23) {
    clearRegisters();
    writeRegisters();

    registers[0] = 1;
    writeRegisters();
    delay(100);
    clearRegisters();
    writeRegisters();

    delay(50);
  }
  vTaskDelete(NULL);*/
}
void ledBlink(void * parameter) {
  while (1) {
    boolean bitVal;
    // This line made it work: Turn clock HIGH before sampling
    digitalWrite(clk, HIGH);

    // Sample the parallel input
    digitalWrite(lock, LOW);
    delayMicroseconds(20);
    digitalWrite(lock, HIGH);

    Serial.print(" LED's States: ");
    for (int i = 0; i < numOfOutPins; i++)
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

}


void clearRegisters() { /* function clearRegisters */
  //// Write register after being set
  digitalWrite(RCLK_Pin, LOW);

  for (int i = numOfRegisterPins - 1; i >=  0; i--) {

    digitalWrite(SRCLK_Pin, LOW);


    digitalWrite(SER_Pin, LOW);//High or LOW
    digitalWrite(SRCLK_Pin, HIGH);
    delayMicroseconds(20);

  }

  digitalWrite(RCLK_Pin, HIGH);
  delay(50);
}

void writeRegisters() { /* function writeRegisters */
  //// Write register after being set
  digitalWrite(RCLK_Pin, LOW);

  for (int i = numOfRegisterPins - 1; i >=  0; i--) {

    digitalWrite(SRCLK_Pin, LOW);
    boolean val = registers[i];
    digitalWrite(SER_Pin, val);
    digitalWrite(SRCLK_Pin, HIGH);
    delayMicroseconds(20);
  }

  digitalWrite(RCLK_Pin, HIGH);
  delay(50);
}

void loop()
{
}
