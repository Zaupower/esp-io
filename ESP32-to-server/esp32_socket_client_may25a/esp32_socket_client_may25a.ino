#include <WiFi.h>
#include <RemoteClient.h>


#define SSID "PFMQ052"
#define PASSWORD "00000000"
#define SERVER_PORT 5000

//Quantidade de pinos cujo os estados serão verificados
#define PIN_COUNT 1
//Array com os pinos que iremos verificar
int pinNumbers[PIN_COUNT] = {35};

//Definir Server IP
const char *serverIPch = "172.21.28.77";
IPAddress serverIP;

//Debug 

//to see last pin sended to server
Pin p1;
const int ledPOutputDebug = 5;

//Debug

//Objeto que verificara se os pinos mudaram de estado e enviara para o server
RemoteClient remoteClient(pinNumbers, PIN_COUNT);

void setup()
{
    Serial.begin(115200);
    //Conecta à rede criada pelo outro ESP
    connectWiFi();
    Serial.print("server ip: ");

    if (serverIP.fromString(serverIPch)) { // try to parse into the IPAddress
      Serial.println(serverIP); // print the parsed IPAddress 
    } else {
      Serial.println("unparsable IP");
    }

    remoteClient.setServerAddressAndPort(serverIP, SERVER_PORT);
    
    //Debug
    pinMode (ledPOutputDebug, OUTPUT);
    digitalWrite (ledPOutputDebug, HIGH);  //
    
}

void connectWiFi()
{
    Serial.print("Connecting to " + String(SSID));
    //Conectamos ao Access Point criado pelo outro ESP
    WiFi.begin(SSID, PASSWORD);

    //Esperamos conectar
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }

    Serial.println();
    Serial.println("Connected!");
}

void loop()
{ 
    if(WiFi.status() != WL_CONNECTED)
    {
        connectWiFi();
    }

    //Chama o método loop do remoteClient e este verificará se aconteceu mudança no estado dos pinos
    //e enviará para o server caso tenha acontecido
    remoteClient.loop();

    
    Serial.print("OUTPUT pin debug: ");
    
    Serial.println(ledPOutputDebug);
    Serial.println("Last Pin send");
    
    int r = remoteClient.getPinsSended();

    Serial.println(r);
    //Serial.println(p1.number);
    //Serial.println(p1.status);
    
}
