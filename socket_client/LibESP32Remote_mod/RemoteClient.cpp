#include "RemoteClient.h"

//pinNumbers - Ponteiro para um array com os números dos pinos que serão verificados
//pinCount - Quantidade de pinos no array pinNumbers
RemoteClient::RemoteClient(int* pN, int pC) : pinNumbers(pN), pinCount(pC)
{
    //Para cada pino
    for(int i=0; i<pinCount; i++)
    {
        Pin pin;
        //Vinculamos o pino da struct com o recebido por parâmetro
        pin.number = pinNumbers[i];
        //Colocamos como INPUT
        pinMode(pin.number, INPUT);
        //Dizemos que o estado está em LOW
        pin.status = LOW;
        //adicionamos ao vector
        pins.push_back(pin);
    }
}

//Endereço ip do server e a porta
void RemoteClient::setServerAddressAndPort(IPAddress serverIpAddress, int serverPort)
{
    this->serverIpAddress = serverIpAddress;
    this->serverPort = serverPort;
}

void RemoteClient::loop()
{
    //Para cada pino
    for(int i=0; i<pinCount; i++)
    {
        //Se o estado foi alterado
        if(hasPinStatusChanged(i))
        {
            //Enviaremos para o server os dados do pino cujo estado foi alterado
            sendPinStatus(i);
        }
    }
}

bool RemoteClient::hasPinStatusChanged(int i)
{
    //Faz a leitura do estado do pino na posição 'i' do array de pinos
    int status = digitalRead(pins[i].number);

    //Se está diferente
    if(pins[i].status != status)
    {
        //Atualizamos os dados do pino
        pins[i].status = status;
        return true;
    }

    return false;
}

void RemoteClient::sendPinStatus(int i)
{
    //Conectamos com o server
    WiFiClient client;
    Serial.println("infoSend:");

    Serial.println((uint8_t*) &pins[i]);
    Serial.println("Size of pin");
    Serial.println(sizeof(Pin));
    
    
    client.connect(serverIpAddress, serverPort);
    
    //Enviamos os dados do pino na posição 'i'do array para o server
    client.write((uint8_t*) &pins[i], sizeof(Pin));
    client.flush();
    client.stop();
}