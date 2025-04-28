/*!
 * \file ft800_if.h
 * \brief FT800 Interface header file
 */

 #ifndef FT800_IF_H
 #define FT800_IF_H
 
 #include "spiIf.h"
 
 class Ft800_interface
 {
 public:
     typedef enum
     {
         ACTIVE  = 0x00,
         STANDBY = 0x41,
         SLEEP   = 0x42,
         PWRDOWN = 0x50,
         CLKEXT  = 0x44,
         CLK48M  = 0x62,
         CLK36M  = 0x61,
         CORERST = 0x68
     } command_t;
 
     typedef enum 
     {
         BITMAPS = 1,
         POINTS = 2,
         LINES = 3,
         LINE_STRIP = 4,
         EDGE_STRIP_R = 5,
         EDGE_STRIP_L = 6,
         EDGE_STRIP_A = 7,
         EDGE_STRIP_B = 8,
         RECTS = 9
     } Begin_primative_t;
 
     Ft800_interface (uint8_t bus_number, uint8_t channel_number);
     ~Ft800_interface (void);
 
     bool read_8_bit (uint32_t address, uint8_t* data);
     bool read_16_bit (uint32_t address, uint16_t* data);
     bool read_32_bit (uint32_t address, uint32_t* data);
 
     bool write_8_bit (uint32_t address, uint8_t data);
     bool write_16_bit (uint32_t address, uint16_t data);
     bool write_32_bit (uint32_t address, uint32_t data);
 
     bool host_command (command_t command);
 
     uint32_t clear_color_rgb (uint8_t red, uint8_t green, uint8_t blue);
     uint32_t clear (uint8_t color, uint8_t stencil, uint8_t tag);
     uint32_t display (void) { return display_bits; };
     uint32_t color_rgb (uint8_t red, uint8_t green, uint8_t blue);
     uint32_t color_alpha (uint8_t alpha);
 
     uint32_t begin (Begin_primative_t primative);
     uint32_t end (void) { return end_bits; };
     uint32_t vertex2f (int32_t x, int32_t y);
     uint32_t vertex2ii (uint16_t x, uint16_t y, uint8_t handle, uint8_t cell);
 
     uint32_t point_size (uint16_t size);
     uint32_t line_width (uint16_t width);
 
     uint32_t macro (uint8_t macro_id);
 
     uint32_t bitmap_source (uint32_t address);
     uint32_t bitmap_layout (uint16_t format, uint16_t stride, uint16_t height);
     uint32_t bitmap_size (uint8_t filter, uint8_t wrap_x, uint8_t wrap_y, uint16_t width, uint16_t height);
     uint32_t bitmap_handle (uint8_t handle);
 
     uint32_t tag (uint8_t value);
 
     uint32_t save_context (void) { return save_context_bits; };
     uint32_t restore_context (void) { return restore_context_bits; };
 
 private:
     static const uint8_t memory_read_transaction_bits  = 0x00;
     static const uint8_t memory_write_transaction_bits = 0x80;
     static const uint8_t dummy_byte_value = 0x00;
 
     static const uint32_t bitmap_source_bits = 0x01000000;
     static const uint32_t bitmap_layout_bits = 0x07000000;
     static const uint32_t bitmap_size_bits = 0x08000000;
     static const uint32_t bitmap_handle_bits = 0x05000000;
 
     static const uint32_t tag_bits = 0x03000000;
 
     static const uint32_t clear_color_rgb_bits = 0x02000000;
     static const uint32_t clear_bits = 0x26000000;
     static const uint32_t clear_color_buffer_bit = 0x00000004;
     static const uint32_t clear_stencil_bit = 0x00000002;
     static const uint32_t clear_tag_bit = 0x00000001;
 
     static const uint32_t color_rgb_bits = 0x04000000;
     static const uint32_t color_alpha_bits = 0x10000000;
     static const uint32_t display_bits = 0x00000000;
     static const uint32_t begin_bits = 0x1f000000;
     static const uint32_t end_bits = 0x21000000;
     static const uint32_t vertex2f_bits = 0x40000000;
     static const uint32_t vertex2ii_bits = 0x80000000;
 
     static const uint32_t point_size_bits = 0x0D000000;
     static const uint32_t line_width_bits = 0x0E000000;
 
     static const uint32_t macro_bits = 0x25000000;
 
     static const uint32_t save_context_bits = 0x22000000;
     static const uint32_t restore_context_bits = 0x23000000;
 
     Spi_interface spi_interface;
 
 };
 
 #endif
 