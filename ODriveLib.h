/* ODrive Library Header */
#ifndef ODRIVELIB_H
#define ODRIVELIB_H

#include "Arduino.h"
#include "options.h"

class ODriveClass {
public:
    enum AxisState_t {
        AXIS_STATE_UNDEFINED = 0,           //<! will fall through to idle
        AXIS_STATE_IDLE = 1,                //<! disable PWM and do nothing
        AXIS_STATE_STARTUP_SEQUENCE = 2, //<! the actual sequence is defined by the config.startup_... flags
        AXIS_STATE_FULL_CALIBRATION_SEQUENCE = 3,   //<! run all calibration procedures, then idle
        AXIS_STATE_MOTOR_CALIBRATION = 4,   //<! run motor calibration
        AXIS_STATE_SENSORLESS_CONTROL = 5,  //<! run sensorless control
        AXIS_STATE_ENCODER_INDEX_SEARCH = 6, //<! run encoder index search
        AXIS_STATE_ENCODER_OFFSET_CALIBRATION = 7, //<! run encoder offset calibration
        AXIS_STATE_CLOSED_LOOP_CONTROL = 8  //<! run closed loop control
    };

    ODriveClass(Stream& serial);

    // Commands
    void SetPosition(int motor_number, float position);
    void SetPosition(int motor_number, float position, float velocity_feedforward);
    void SetPosition(int motor_number, float position, float velocity_feedforward, float current_feedforward);
    void SetVelocity(int motor_number, float velocity);
    void SetVelocity(int motor_number, float velocity, float current_feedforward);
    void SetCurrent(int motor_number, float current);
    void TrapezoidalMove(int motor_number, float position);

    // Motor Configuration Commands
    int MotorCalibrationStatus(int axis);
    void MotorPreCalibrated(int axis, bool request);

    // Encoder Configuration Commands
    int EncoderReadyStatus(int axis);
    void EncoderUseIndex(int axis, bool request);
    void EncoderPreCalibrated(int axis, bool request);

    // Startup Configuration Commands
    void StartupMotorCalibration(int axis, bool request);
    void StartupEncoderIndexSearch(int axis, bool request);
    void StartupEncoderOffsetCalibration(int axis, bool request);
    void StartupClosedLoop(int axis, bool request);
    void StartupSensorless(int axis, bool request);

    // System Commands
    float BusVoltage(void);
    void SaveConfiguration(void);
    void EraseConfiguration(void);
    void Reboot(void);

    // General params
    float readFloat();
    int32_t readInt();
    int32_t readState(int axis);

    // State helper
    bool run_state(int axis, int requested_state, bool wait);
private:
    String readString();

    Stream& serial_;
};

#endif //ODRIVELIB_H
