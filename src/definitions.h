#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <time.h>
#include <math.h>

// Generic function return values
typedef enum
{
   RETURN_OK = 0,
   RETURN_ERROR
} return_value_t;

// System error modes
typedef enum
{
    no_error,
    error_temperature_control_interface,
    error_battery_interface,
    error_temperature_failure,
    error_battery_discharged,
    error_measurement_process_interface,
    error_internal_system,
} system_error_t;

typedef enum
{
    measurement_below_threashold,
    measurement_above_threashold,
    no_sample_detected,
    leak_detected,
    scan_error,
    cartridge_removed
} measurement_failure_reason_t;

typedef enum
{
    fail_low,
    warning_low,
    pass,
    warning_high,
    fail_high,
    undefined_result
} Measurement_result_status_t;

typedef struct
{
    float viscosity;
    Measurement_result_status_t status;
    time_t time_stamp;
} measurement_result_t;

static const size_t test_specification_max_string = 50;
static const size_t version_max_string = 50;

typedef struct Test_parameter_stc_t
{
    char name[test_specification_max_string];

    float viscosity;
    float upper_limit;
    float lower_limit;
    float upper_warning;
    float lower_warning;

    Test_parameter_stc_t (void)
    {
        name[0] = '\0';
        viscosity = 0.0;
        upper_limit = 0.0;
        lower_limit = 0.0;
        upper_warning = 0.0;
        lower_warning = 0.0;
    };
} Test_parameter_stc_t;

static const float unknown_temperature = NAN;
static const uint8_t unknown_battery_charge = 255;

// Configuration file definitions
static const char configuration_general_section_name[] = "General";
static const char configuration_target_temperature_string[] = "target_temperature";
static const char configuration_temperature_max_error_string[] = "temperature_max_error";
static const char configuration_temperature_settled_period_string[] = "temperature_settled_period";
static const char configuration_initial_temperature_timeout_string[] = "initial_temperature_timeout";
static const char configuration_screen_idle_timeout_string[] = "screen_idle_timeout";
static const char configuration_system_idle_timeout_string[] = "system_idle_timeout";
static const char configuration_minimum_battery_threshold_string[] = "minimum_battery_threshold";
static const char configuration_pin_value_string[] = "pin_value";
static const char configuration_notice_display_period_string[] = "notice_display_period";

static const char configuration_control_parameters_section_name[] = "Control Parameters";
static const char configuration_control_type_string[] = "control_type";
static const char configuration_p_term_string[] = "p_term";
static const char configuration_i_term_string[] = "i_term";
static const char configuration_d_term_string[] = "d_term";
static const char configuration_derivative_filter_string[] = "derivative_filter";
static const char configuration_dead_band_string[] = "dead_band";
static const char configuration_power_up_state_string[] = "power_up_state";

static const char configuration_output_parameters_section_name[] = "Output Parameters";
static const char configuration_polarity_string[] = "polarity";
static const char configuration_minimum_output_string[] = "minimum";
static const char configuration_maximum_output_string[] = "maximum";
static const char configuration_output_frequency_string[] = "frequency";

static const char configuration_warm_up_section_name[] = "Warm Up";
static const char configuration_warm_up_period_string[] = "warm_up_period";
static const char configuration_monitor_warm_up_temperature_string[] = "monitor_warm_up_temperature";

static const char configuration_sample_detection_section_name[] = "Sample Detection";
static const char configuration_minimum_stable_scans_string[] = "minimum_stable_scans";
static const char configuration_maximum_frequency_change_string[] = "maximum_frequency_change";
static const char configuration_sample_detection_timeout_string[] = "sample_detection_timeout";
static const char configuration_start_frequency_string[] = "start_frequency";
static const char configuration_step_size_string[] = "step_size";
static const char configuration_number_of_samples_string[] = "number_of_samples";
static const char configuration_number_of_observations_string[] = "number_of_observations";
static const char configuration_savitzky_golay_window_string[] = "savitzky_golay_window";
static const char configuration_air_threshold_frequency_string[] = "air_threshold_frequency";
static const char configuration_air_threshold_amplitude_string[] = "air_threshold_amplitude";

static const char configuration_optimisation_scan_section_name[] = "Optimisation Scan";

static const char configuration_measurement_scans_section_name[] = "Measurement Scans";
static const char configuration_number_of_scans_string[] = "number_of_scans";

static const char configuration_scan_process_section_name[] = "Scan Process";
static const char configuration_port_number_string[] = "port_number";
static const char configuration_logger_port_string[] = "logger_port";

static const char configuration_viscosity_calculation_section_name[] = "Viscosity Calculation";
static const char configuration_q_factor_offset_string[] = "q_factor_offset";
static const char configuration_q_factor_divisor_string[] = "q_factor_divisor";
static const char configuration_exponent_divisor_string[] = "exponent_divisor";

static const char configuration_touch_screen_calibration_section_name[] = "Touch Screen Calibration";
static const char configuration_calibration_transform_a_string[] = "transform_a";
static const char configuration_calibration_transform_b_string[] = "transform_b";
static const char configuration_calibration_transform_c_string[] = "transform_c";
static const char configuration_calibration_transform_d_string[] = "transform_d";
static const char configuration_calibration_transform_e_string[] = "transform_e";
static const char configuration_calibration_transform_f_string[] = "transform_f";

// Default Configuration values

// General Section
static const float default_target_temperature = 40.0;
static const float default_temperature_max_error = 0.1;
static const uint32_t default_temperature_settled_period = 10;
static const uint32_t default_initial_temperature_timeout = 600;
static const uint32_t default_screen_idle_timeout = 300;
static const uint32_t default_system_idle_timeout = 60;
static const uint8_t  default_minimum_battery_threshold = 10;
static const uint32_t default_pin_value = 1234;
static const uint32_t default_notice_display_period = 5;

// Warm Up Section
static const uint32_t default_warm_up_period = 60;
static const bool default_monitor_warm_up_temperature = false;

// Sample Detection Section
static const uint32_t default_sample_detection_minimum_stable_scans = 12;
static const uint32_t default_sample_detection_maximum_frequency_change = 5;
static const uint32_t default_sample_detection_timeout = 120;
static const uint32_t default_sample_detection_start_frequency = 18000;
static const uint32_t default_sample_detection_step_size = 30;
static const uint32_t default_sample_detection_number_of_samples = 450;
static const uint32_t default_sample_detection_number_of_observations = 1;
static const uint32_t default_sample_detection_savitzky_golay_window = 41;
static const uint32_t default_air_threshold_frequency = 26000;
static const int32_t  default_air_threshold_amplitude = -30;

// Optimisation Scan Section
static const uint32_t default_optimisation_scan_start_frequency = 22000;
static const uint32_t default_optimisation_scan_step_size = 24;
static const uint32_t default_optimisation_scan_number_of_samples = 240;
static const uint32_t default_optimisation_scan_number_of_observations = 4;
static const uint32_t default_optimisation_scan_savitzky_golay_window = 41;

// Measurement Scans Section
static const uint32_t default_measurement_number_of_scans = 12;
static const uint32_t default_measurement_maximum_frequency_change = 10;

// Scan Process Section
static const uint16_t default_scan_port_number = 1510;
static const uint16_t default_scan_logger_port = 1510;

// Sensor 1 Section
static const uint8_t default_i2c_channel = 2;
static const uint16_t default_sensor_port_number = 1500;
static const uint32_t default_initial_settling_period = 10;
static const uint32_t default_settling_cycles = 10;

// Signal Processing Section
static const uint32_t default_calibration_polynomial_order = 2;
static const int32_t default_gain_adjustment = -90;
static const uint32_t default_savitsky_golay_polynomial_order = 2;
static const int32_t default_minimum_amplitude = -60;
static const uint32_t default_interpolation_ratio = 10;
static const uint32_t default_lorentzian_fit_window_threshold = 2;
static const float default_lorentzian_fit_initial_a_value = -56.6;
static const float default_lorentzian_fit_initial_b_value = 8332;
static const float default_lorentzian_fit_initial_c_value = 10;

// Scan Optimisation Section
static const uint32_t default_target_window_size = 500;
static const float default_optimisation_window_multiplier_a = 0.8;
static const float default_optimisation_window_multiplier_b = 2;
static const float default_optimisation_observation_q_power = -0.9;
static const float default_optimisation_observation_multiplier = 200;
static const float default_optimisation_savitzky_golay_multiplier = 820.45;
static const float default_optimisation_savitzky_golay_q_power = -1.347;

// Viscosity Calculation Section
static const float default_q_factor_offset = 0;
static const float default_q_factor_divisor = 206.559;
static const float default_exponent_divisor = -0.417;

// Button tag values
static const uint8_t no_tag = 0x00;
static const uint8_t spanner_tag = 0x80;
static const uint8_t next_tag = 'N';
static const uint8_t back_arrow_tag = 0x08;
static const uint8_t ok_tag = 0x0D;
static const uint8_t untagged_icon = 0xFF;

// Test parameter file values
static const char test_parameter_name_string[] = "test_name";
static const char test_parameter_viscosity_string[] = "viscosity";
static const char test_parameter_upper_limit_string[] = "upper_limit";
static const char test_parameter_lower_limit_string[] = "lower_limit";
static const char test_parameter_upper_warning_string[] = "upper_warning";
static const char test_parameter_lower_warning_string[] = "lower_warning";

// File names
static const char version_file_name[] = "/opt/HBL/VERSION";

#endif
