//
// Created by Magnus Nordlander on 2021-08-20.
//

#ifndef FIRMWARE_TYPES_H
#define FIRMWARE_TYPES_H

#include <pico/time.h>

typedef enum {
    SYSTEM_CONTROLLER_STATE_IDLE,
    SYSTEM_CONTROLLER_STATE_HEATUP,
    SYSTEM_CONTROLLER_STATE_WARM,
    SYSTEM_CONTROLLER_STATE_SLEEPING,
    SYSTEM_CONTROLLER_STATE_BAILED,
    SYSTEM_CONTROLLER_STATE_FIRST_RUN
} SystemControllerState;

typedef enum {
    BAIL_REASON_NONE = 0,
    BAIL_REASON_CB_UNRESPONSIVE,
    BAIL_REASON_TOO_FAR_BEHIND_ON_RESPONSE,
    BAIL_REASON_TOO_FAR_BEHIND_BETWEEN_PACKETS,
    BAIL_REASON_SSR_QUEUE_EMPTY,
} SystemControllerBailReason;

struct PidSettings {
    double Kp{};
    double Ki{};
    double Kd{};
};

struct PidRuntimeParameters {
    double integral = 0;
    double p = 0;
    double i = 0;
    double d = 0;
};

struct SystemControllerStatusMessage{
    absolute_time_t timestamp{};
    float brewTemperature{};
    float brewSetPoint{};
    PidSettings brewPidSettings{};
    PidRuntimeParameters brewPidParameters{};
    float serviceTemperature{};
    float serviceSetPoint{};
    PidSettings servicePidSettings{};
    PidRuntimeParameters servicePidParameters{};
    bool ecoMode{};
    SystemControllerState state{};
    SystemControllerBailReason bailReason{};
    bool currentlyBrewing{};
    bool currentlyFillingServiceBoiler{};
    bool waterTankLow{};
};

typedef enum {
    COMMAND_SET_BREW_SET_POINT,
    COMMAND_SET_BREW_PID_PARAMETERS,
    COMMAND_SET_SERVICE_SET_POINT,
    COMMAND_SET_SERVICE_PID_PARAMETERS,
    COMMAND_SET_ECO_MODE,
    COMMAND_SET_SLEEP_MODE,
    COMMAND_UNBAIL,
    COMMAND_TRIGGER_FIRST_RUN,
} SystemControllerCommandType;

struct SystemControllerCommand {
    SystemControllerCommandType type{};
    float float1{};
    float float2{};
    float float3{};
    bool bool1{};
};

#endif //FIRMWARE_TYPES_H
