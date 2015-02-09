#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "Arduino.h"
#include "UdpController.h"

byte MacAddress[] = {0x00, 0xAA, 0xBB, 0xCC, 0x28, 0x07};
char Buffer[UDP_TX_PACKET_MAX_SIZE];
EthernetUDP Udp;

UdpController::UdpController(SwitchBus* switchBus)
{
    _switchBus = switchBus;
}

void UdpController::Setup()
{
    if (Ethernet.begin(MacAddress) == 0)
    {
        for(;;);
    }

    Udp.begin(Port);
}

void UdpController::Loop()
{
    int packetSize = Udp.parsePacket();

    if (packetSize && packetSize >= 4)
    {
        Udp.read(Buffer, UDP_TX_PACKET_MAX_SIZE);

        if (Buffer[0] == 0x28 && Buffer[1] == 0x07)
        {
            int state = (int)Buffer[2];
            for (int i = 3; i < packetSize; ++i)
            {
                int pinIndex = (int)Buffer[i];
                _switchBus->Set(pinIndex, state);
            }
        }
    }
}