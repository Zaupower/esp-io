#include "RemoteServer.h"

//Construimos o server com o valor de serverPort
RemoteServer::RemoteServer(int serverPort) : server(serverPort)
{
}

void RemoteServer::begin()
{
    //Iniciamos o server para que este possa receber conexões
    server.begin();
}

void RemoteServer::onStatusChange(OnStatusChangeCallback callback)
{
    this->callback = callback;
}
//ou
// void RemoteServer::onStatusChange(void (*callback)(int pinNumber, int pinStatus))
// {
//     this->callback = callback;
// }
//ou
// void RemoteServer::onStatusChange(void (*callback)(int, int))
// {
//     this->callback = callback;
// }

void RemoteServer::loop()
{
    //Verificamos se temos um client tentando se conectar
    WiFiClient client = server.available();

    //Se temos
    if (client)
    {
        //Colocamos no final do vector que armazena os clients
        clients.push_back(client);
    }

    for (std::vector<WiFiClient>::iterator it = clients.begin(); it != clients.end();)
    {
        //O conteúdo apontado pelo iterator é o client
        WiFiClient client = *it;

        if(client.connected())
        {
            //Se o client possui algo para nos enviar
            if (client.available())
            {
                readAndNotify(client);
            }

            //Vamos para o próximo client
            it++;
        }
        //Se não estiver mais conectado
        else
        {
            //Apagamos do vector
            clients.erase(it);
        }
    }
}

void RemoteServer::readAndNotify(WiFiClient client)
{
    //Fazemos a leitura dos dados do client para a variável pin
    Pin pin;
    client.read((uint8_t*)&pin, sizeof(pin));

    //Se possuímos um callback adicionado
    if(callback)
    {
        //Chamamos o callback informando o número do pino que foi alterado e o novo estado
        callback(pin.number, pin.status);
    }
}

void RemoteServer::stop()
{
    //Paramos o servidor
    server.stop();
}