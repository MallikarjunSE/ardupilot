#pragma once

#include <GCS_MAVLink/GCS.h>

  // set 0 in 4.6, remove feature in 4.7:
#ifndef AP_MAVLINK_MAV_CMD_NAV_SET_YAW_SPEED_ENABLED
#define AP_MAVLINK_MAV_CMD_NAV_SET_YAW_SPEED_ENABLED 0
#endif

#include "defines.h"

class GCS_MAVLINK_Rover : public GCS_MAVLINK
{
public:

    using GCS_MAVLINK::GCS_MAVLINK;

protected:

    MAV_RESULT _handle_command_preflight_calibration(const mavlink_command_int_t &packet, const mavlink_message_t &msg) override;
    MAV_RESULT handle_command_int_packet(const mavlink_command_int_t &packet, const mavlink_message_t &msg) override;
    MAV_RESULT handle_command_int_do_reposition(const mavlink_command_int_t &packet);
    MAV_RESULT handle_command_nav_set_yaw_speed(const mavlink_command_int_t &packet, const mavlink_message_t &msg);

    void send_position_target_global_int() override;

    uint64_t capabilities() const override;

    void send_nav_controller_output() const override;
    void send_pid_tuning() override;

#if HAL_LOGGING_ENABLED
    uint32_t log_radio_bit() const override { return MASK_LOG_PM; }
#endif

    // Send the mode with the given index (not mode number!) return the total number of modes
    // Index starts at 1
    uint8_t send_available_mode(uint8_t index) const override;

private:

    void handle_message(const mavlink_message_t &msg) override;
    bool handle_guided_request(AP_Mission::Mission_Command &cmd) override;
    bool try_send_message(enum ap_message id) override;

    void handle_manual_control_axes(const mavlink_manual_control_t &packet, const uint32_t tnow) override;
    void handle_set_attitude_target(const mavlink_message_t &msg);
    void handle_set_position_target_local_ned(const mavlink_message_t &msg);
    void handle_set_position_target_global_int(const mavlink_message_t &msg);
    void handle_radio(const mavlink_message_t &msg);
    void handle_landing_target(const mavlink_landing_target_t &msg, uint32_t timestamp_ms) override;

    void send_servo_out();

    uint8_t base_mode() const override;
    MAV_STATE vehicle_system_status() const override;

    int16_t vfr_hud_throttle() const override;

#if AP_RANGEFINDER_ENABLED
    void send_rangefinder() const override;

    // send WATER_DEPTH - metres and temperature
    void send_water_depth();
    // state variable for the last rangefinder we sent a WATER_DEPTH
    // message for.  We cycle through the rangefinder backends to
    // limit the amount of telemetry bandwidth we consume.
    uint8_t last_WATER_DEPTH_index;
#endif

#if HAL_HIGH_LATENCY2_ENABLED
    uint8_t high_latency_tgt_heading() const override;
    uint16_t high_latency_tgt_dist() const override;
    uint8_t high_latency_tgt_airspeed() const override;
    uint8_t high_latency_wind_speed() const override;
    uint8_t high_latency_wind_direction() const override;
#endif // HAL_HIGH_LATENCY2_ENABLED
};
