#include <Arduino.h>

#include "ODriveLib.h"

static const int kMotorOffsetFloat = 2;
static const int kMotorStrideFloat = 28;
static const int kMotorOffsetInt32 = 0;
static const int kMotorStrideInt32 = 4;
static const int kMotorOffsetBool = 0;
static const int kMotorStrideBool = 4;
static const int kMotorOffsetUint16 = 0;
static const int kMotorStrideUint16 = 2;

// Print with stream operator
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

ODriveClass::ODriveClass(Stream& serial)
    : serial_(serial) {}

// ODrive Movement Commands
void ODriveClass::SetPosition(int motor_number, float position) {
    SetPosition(motor_number, position, 0.0f, 0.0f);
}

void ODriveClass::SetPosition(int motor_number, float position, float velocity_feedforward) {
    SetPosition(motor_number, position, velocity_feedforward, 0.0f);
}

void ODriveClass::SetPosition(int motor_number, float position, float velocity_feedforward, float current_feedforward) {
    serial_ << "p " << motor_number  << " " << position << " " << velocity_feedforward << " " << current_feedforward << "\n";
}

void ODriveClass::SetVelocity(int motor_number, float velocity) {
    SetVelocity(motor_number, velocity, 0.0f);
}

void ODriveClass::SetVelocity(int motor_number, float velocity, float current_feedforward) {
    serial_ << "v " << motor_number  << " " << velocity << " " << current_feedforward << "\n";
}

void ODriveClass::SetCurrent(int motor_number, float current) {
    serial_ << "c " << motor_number << " " << current << "\n";
}

void ODriveClass::TrapezoidalMove(int motor_number, float position){
    serial_ << "t " << motor_number << " " << position << "\n";
}

// ODrive Control Mode Commands
void ODriveClass::SetControlModeTraj(int axis) {
    serial_ << "w axis" << axis << ".controller.config.control_mode " << CTRL_MODE_TRAJECTORY_CONTROL << "\n";
}

void ODriveClass::SetControlModeVel(int axis) {
    serial_ << "w axis" << axis << ".controller.config.control_mode " << CTRL_MODE_VELOCITY_CONTROL << "\n";
}

// Motor configuration Commands
int ODriveClass::MotorCalibrationStatus(int axis){
    serial_ << "r axis" << axis << ".motor.is_calibrated\n";
    return readInt();
}

void ODriveClass::MotorPreCalibrated(int axis, bool request){
    serial_ << "w axis" << axis << ".motor.config.pre_calibrated " << request << "\n";
}

// Encoder Configuration Commands
int ODriveClass::EncoderReadyStatus(int axis){
    serial_ << "r axis" << axis << ".encoder.is_ready";
    return readInt();
}

void ODriveClass::EncoderUseIndex(int axis, bool request){
    serial_ << "w axis" << axis << ".encoder.config.use_index " << request << "\n";
}

void ODriveClass::EncoderPreCalibrated(int axis, bool request){
    serial_ << "w axis" << axis << ".encoder.config.pre_calibrated " << request << "\n";
}

// Startup Configuration Commands
void ODriveClass::StartupMotorCalibration(int axis, bool request){
    serial_ << "w axis" << axis << ".config.startup_motor_calibration " << request << "\n";
}

void ODriveClass::StartupEncoderIndexSearch(int axis, bool request){
    serial_ << "w axis" << axis << ".config.startup_encoder_index_search " << request << "\n";
}

void ODriveClass::StartupEncoderOffsetCalibration(int axis, bool request){
    serial_ << "w axis" << axis << ".config.startup_encoder_offset_calibration " << request << "\n";
}

void ODriveClass::StartupClosedLoop(int axis, bool request){
    serial_ << "w axis" << axis << ".config.startup_closed_loop_control " << request << "\n";
}

void ODriveClass::StartupSensorless(int axis, bool request){
    serial_ << "w axis" << axis << ".config.startup_sensorless_control " << request << "\n";
}

// Axis Limit Commands
void ODriveClass::ConfigureBrakingResistance(float braking_resistance){
    serial_ << "w config.brake_resistance " << braking_resistance << "\n";
}

void ODriveClass::ConfigureCurrentLimit(int axis, float current_limit){
    serial_ << "w axis" << axis << ".motor.config.current_lim " << current_limit << "\n";
}

void ODriveClass::ConfigureCalibrationCurrent(int axis, float current_calib){
    serial_ << "w axis" << axis << ".motor.config.calibration_current " << current_calib << "\n";
}

void ODriveClass::ConfigureVelLimit(int axis, float velocity){
    serial_ << "w axis" << axis << ".controller.config.vel_limit " << velocity << "\n";
}

void ODriveClass::ConfigurePolePairs(int axis, int pole_pairs){
    serial_ << "w axis" << axis << ".motor.config.pole_pairs " << pole_pairs << "\n";
}

void ODriveClass::ConfigureMotorType(int axis, int motor_type){
    serial_ << "w axis" << axis << ".motor.config.motor_type " << motor_type << "\n";
}

void ODriveClass::ConfigureCPR(int axis, int cpr){
    serial_ << "w axis" << axis << ".encoder.config.cpr " << cpr << "\n";
}

void ODriveClass::ConfigureEncoderMode(int axis, int mode){
    serial_ << "w axis" << axis << ".encoder.config.mode " << mode << "\n";
}

// Trajectory Limit Commands
void ODriveClass::ConfigureTrajVelLimit(int axis, float velocity){
    serial_ << "w axis" << axis << ".trap_traj.config.vel_limit " << velocity << "\n";
}

void ODriveClass::ConfigureTrajAccelLimit(int axis, float acceleration){
    serial_ << "w axis" << axis << ".trap_traj.config.accel_limit " << acceleration << "\n";
}

void ODriveClass::ConfigureTrajDecelLimit(int axis, float deceleration){
    serial_ << "w axis" << axis << ".trap_traj.config.decel_limit " << deceleration << "\n";
}

// PID Calibration Commands
void ODriveClass::ConfigurePosGain(int axis, float pos_gain){
    serial_ << "w axis" << axis << ".controller.config.pos_gain " << pos_gain << "\n";
}

void ODriveClass::ConfigureVelGain(int axis, float vel_gain){
    serial_ << "w axis" << axis << ".controller.config.vel_gain " << vel_gain << "\n";
}

void ODriveClass::ConfigureVelIntGain(int axis, float vel_int_gain){
    serial_ << "w axis" << axis << ".controller.config.vel_integrator_gain " << vel_int_gain << "\n";
}

// System Configuration Commands
float ODriveClass::BusVoltage(void){
    serial_ << "r vbus_voltage\n";
    return readFloat();
}

void ODriveClass::SaveConfiguration(void){
    serial_ << "ss\n";
}

void ODriveClass::EraseConfiguration(void){
    serial_ << "se\n";
}

void ODriveClass::Reboot(void){
    serial_ << "sb\n";
    //need to restart serial comms after rebooting
    delay(100);
    odrive_serial.begin(ODRIVE_SERIAL_BAUD);
    while(!odrive_serial);
}

float ODriveClass::readFloat() {
    return readString().toFloat();
}

int32_t ODriveClass::readInt() {
    return readString().toInt();
}

int32_t ODriveClass::readState(int axis) {
    serial_ << "r axis" << axis << ".current_state\n";
    return readInt();
}

bool ODriveClass::run_state(int axis, int requested_state, bool wait) {
    int timeout_ctr = 100;
    serial_ << "w axis" << axis << ".requested_state " << requested_state << '\n';
    if (wait) {
        do {
            delay(100);
            serial_ << "r axis" << axis << ".current_state\n";
        } while (readInt() != AXIS_STATE_IDLE && --timeout_ctr > 0);
    }

    return timeout_ctr > 0;
}

String ODriveClass::readString() {
    String str = "";
    static const unsigned long timeout = 1000;
    unsigned long timeout_start = millis();
    for (;;) {
        while (!serial_.available()) {
            if (millis() - timeout_start >= timeout) {
                return str;
            }
        }
        char c = serial_.read();
        if (c == '\n')
            break;
        str += c;
    }
    return str;
}
