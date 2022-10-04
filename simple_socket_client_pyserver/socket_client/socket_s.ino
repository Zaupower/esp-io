//Apenas vai compilar o código contido neste arquivo
//caso IS_SERVER NÃO esteja definido
//(if n def, atenção para o 'n')
#include <WiFi.h>

#define SSID "PFMQ052"
#define PASSWORD "00000000"
#define SERVER_PORT 5010


//Pino que vamos fazer a leitura
#define IN_PIN 35

//Definir Server IP
const char *serverIPch = "172.21.28.77";
IPAddress serverIP;

int buttonState = 0;


//Protocolo que o Server e o Client utilizarão para se comunicar
enum Protocol{
    ID, //ID unico
    PIN, //Pino que se deseja alterar o estado
    VALUE, //Estado para qual o pino deve ir (HIGH = 1 ou LOW = 0)
    BUFFER_SIZE //O tamanho do nosso protocolo. IMPORTANTE: deixar sempre como último do enum
};

void setup(){
    serverIP.fromString(serverIPch);
    Serial.begin(115200);
    //Colocamos o pino em modo de leitura
    pinMode(IN_PIN, INPUT);
    //Conectamos Access Point criado
    
    WiFi.begin(SSID, PASSWORD);

    //Esperamos conectar
     Serial.println("Awaiting WIFI connection");
    while (WiFi.status() != WL_CONNECTED){
      Serial.print(".");
        delay(500);
    }
}


void loop(){
  buttonState = digitalRead(IN_PIN);
    
   Serial.println(buttonState);

  if (buttonState == HIGH) {


 //Variável que utlizaremos para conectar ao servidor
    WiFiClient client;
    
    //Se não conseguiu se conectar então retornamos
    if (!client.connect(serverIP, 5000)){
      
        return;
    }
    
    
      //Criamos um buffer para colocar os dados
      uint8_t buffer[Protocol::BUFFER_SIZE];
      //Fazemos a leitura do pino
      int value = digitalRead(IN_PIN);
      //Colocamos no buffer o número do pino
      //cujo estado queremos enviar
      buffer[Protocol::ID] = 01;
      buffer[Protocol::PIN] = IN_PIN;
      //Colocamos no buffer o estado atual do pino
      buffer[Protocol::VALUE] = value;
      //Enviamos e finalizamos a conexão
      client.write(buffer, Protocol::BUFFER_SIZE);
      client.flush();
      client.stop();
      }
  
}
//Encerra o #ifndef do começo do arquivo
