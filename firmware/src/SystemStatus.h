//
// Created by Magnus Nordlander on 2021-07-02.
//

#ifndef FIRMWARE_SYSTEMSTATUS_H
#define FIRMWARE_SYSTEMSTATUS_H

#include <mbed.h>
#include <lcc_protocol.h>
#include <control_board_protocol.h>

class SystemStatus {
public:
    SystemStatus();

    bool hasSentLccPacket = false;
    rtos::Kernel::Clock::time_point lastLccPacketSentAt;
    LccParsedPacket lccPacket;

    bool hasReceivedControlBoardPacket = false;
    rtos::Kernel::Clock::time_point lastControlBoardPacketReceivedAt;
    ControlBoardParsedPacket controlBoardPacket;
    ControlBoardRawPacket controlBoardRawPacket;

    bool has_bailed;

    bool ecoMode = false;

    float targetBrewTemperature = 0.0f;
    float targetServiceTemperature = 0.0f;

    float brewTemperatureOffset = -10.0f;

    bool wifiConnected = false;
    bool mqttConnected = false;

    inline float getOffsetTargetBrewTemperature() const { return targetBrewTemperature + brewTemperatureOffset; }
};


#endif //FIRMWARE_SYSTEMSTATUS_H
