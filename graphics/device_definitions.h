/*!
 * \file device_definitions.h
 * \brief Common device definitions for GUI, touch, and workflow states
 *
 * Provides neutral, general-purpose constants, enums, and bitmap structures
 * for embedded GUI applications using FT800.
 */

 #ifndef DEVICE_DEFINITIONS_H
 #define DEVICE_DEFINITIONS_H
 
 #include <cstdint>
 
 namespace Device_definitions
 {
 
 /*!
  * \enum test_result_t
  * \brief Result codes for a general test or measurement
  */
 enum class test_result_t : uint8_t
 {
     TEST_PASS = 0,
     TEST_FAIL,
     TEST_ABORTED,
     TEST_ERROR,
     TEST_PENDING,
     TEST_IN_PROGRESS
 };
 
 /*!
  * \enum workflow_step_t
  * \brief Steps of a general workflow or user procedure
  */
 enum class workflow_step_t : uint8_t
 {
     NO_MODULE = 0,
     MODULE_INSERTED,
     MODULE_REMOVED,
     MODULE_PROCESSING,
     MODULE_READY,
     MODULE_ERROR
 };
 
 /*!
  * \enum battery_state_t
  * \brief Battery charge state
  */
 enum class battery_state_t : uint8_t
 {
     BATTERY_FULL = 0,
     BATTERY_THREE_QUARTER,
     BATTERY_QUARTER,
     BATTERY_LOW,
     BATTERY_CRITICAL
 };
 
 /*!
  * \struct bitmap_info_t
  * \brief Information about a bitmap stored in graphics RAM
  */
 struct bitmap_info_t
 {
     uint32_t ram_g_offset;     //!< RAM_G address where the bitmap is stored
     uint16_t width;            //!< Width in pixels
     uint16_t height;           //!< Height in pixels
     uint16_t stride;           //!< Bytes per line (stride)
     uint16_t format;           //!< FT800 pixel format code
     uint8_t handle;            //!< FT800 bitmap handle number
     uint8_t filter;            //!< Bitmap filter setting (NEAREST or BILINEAR)
     uint8_t wrap_x;            //!< Bitmap wrap X setting
     uint8_t wrap_y;            //!< Bitmap wrap Y setting
     const uint8_t* pixel_data; //!< Pointer to raw pixel data
     size_t length;             //!< Length of pixel data in bytes
 };
 
 } // namespace Device_definitions
 
 /*!
  * \struct calibration_stc_t
  * \brief Touch screen calibration transformation coefficients
  */
 struct calibration_stc_t
 {
     uint32_t transform_a; //!< Coefficient A
     uint32_t transform_b; //!< Coefficient B
     uint32_t transform_c; //!< Coefficient C
     uint32_t transform_d; //!< Coefficient D
     uint32_t transform_e; //!< Coefficient E
     uint32_t transform_f; //!< Coefficient F
 };
 
 #endif // DEVICE_DEFINITIONS_H
 