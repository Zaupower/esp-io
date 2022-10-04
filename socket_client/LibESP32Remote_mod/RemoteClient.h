#ifndef RemoteClient_h
#define RemoteClient_h

#include <Arduino.h>
#include <WiFi.h>
#include <vector>
#include "Pin.h"

class RemoteClient
{
    public:
        RemoteClient(int* pinNumbers, int pinCount);
        void loop();
        void setServerAddressAndPort(IPAddress serverIpAddress, int serverPort);
        int* pinNumbers;
        int pinCount;
        std::vector<Pin> pins;
        IPAddress serverIpAddress;
        int serverPort;
    private:
        boolean hasPinStatusChanged(int i);
        void sendPinStatus(int i);
};

#endif