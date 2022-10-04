#ifndef RemoteServer_h
#define RemoteServer_h

#include <Arduino.h>
#include <WiFi.h>
#include <vector>
#include "Pin.h"

typedef void (*OnStatusChangeCallback)(int pinNumber, int pinStatus);

class RemoteServer
{
    public:
        RemoteServer(int serverPort);
        std::vector<WiFiClient> clients;
        void begin();
        void loop();
        void stop();
        void onStatusChange(OnStatusChangeCallback callback);
        //ou
        // void onStatusChange(void (*callback)(int pinNumber, int pinStatus));
        //ou
        // void onStatusChange(void (*callback)(int, int));
        //ou
        // void onStatusChange(void (*)(int, int));
    private:
        void readAndNotify(WiFiClient client);
        WiFiServer server;
        void (*callback)(int, int); //Variável que irá apontar para função callback
        //ou
        // void (*callback)(int pinNumber, int pinStatus);
        //ou
        // OnStatusChangeCallback callback;
};

#endif