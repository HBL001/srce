/*!
 * \file ft800.cpp
 * \brief FT800 display control implementation
 */

 #include <stdint.h>
 #include <stdio.h>
 #include <unistd.h>
 #include <string.h>
 
 #include "ft800If.h"
 #include "ft800Reg.h"
 #include "ft800.h"
 
 static const uint8_t ft800_defined_character_codes = 128;
 typedef struct ft800_font_width_t
 {
     uint16_t bitmap_format;
     uint16_t bitmap_stride;
     uint16_t bitmap_width;
     uint16_t bitmap_height;
     uint32_t bitmap_rom_address;
     uint8_t font_width[ft800_defined_character_codes];
 } ft800_font_width_t;
 
 static const uint8_t ft800_first_rom_font = 16;
 static const uint8_t ft800_last_rom_font = 32;
 
 static const ft800_font_width_t ft800_rom_fonts[] = {
     /* Hardware Fonts index 16*/
     {
         1,
         1,
         8,
         8,
         1047548,
         {8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8}
     },
     /* Hardware Fonts index 17*/
     {
         1,
         1,
         8,
         8,
         1046524,
         {8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8}
     },
     /* Hardware Fonts index 18*/
     {
         1,
         1,
         8,
         16,
         1044476,
         {8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8}
     },
     /* Hardware Fonts index 19*/
     {
         1,
         1,
         8,
         16,
         1042428,
         {8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 8, 8, 8, 8, 8, 8,}
     },
     /* Hardware Fonts index 20*/
     {
         1,
         2,
         10,
         13,
         1039100,
         {0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          3, 3, 4, 6, 6, 9, 8, 2,
          4, 4, 4, 6, 3, 4, 3, 3,
          6, 6, 6, 6, 6, 6, 6, 6,
          6, 6, 3, 3, 6, 5, 6, 6,
          11, 7, 7, 8, 8, 7, 6, 8,
          8, 3, 5, 7, 6, 9, 8, 8,
          7, 8, 7, 7, 5, 8, 7, 9,
          7, 7, 7, 3, 3, 3, 6, 6,
          3, 5, 6, 5, 6, 5, 4, 6,
          6, 2, 2, 5, 2, 8, 6, 6,
          6, 6, 4, 5, 4, 5, 6, 8,
          6, 5, 5, 3, 3, 3, 7, 0}
     },
     /* Hardware Fonts index 21*/
     {
         1,
         2,
         13,
         17,
         1035580,
         {0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          4, 4, 5, 8, 8, 12, 10, 3,
          5, 5, 7, 9, 3, 4, 3, 4,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 3, 4, 8, 9, 8, 8,
          13, 9, 9, 10, 10, 9, 8, 11,
          10, 4, 7, 9, 8, 12, 10, 11,
          9, 11, 10, 9, 9, 10, 9, 13,
          9, 9, 9, 4, 4, 4, 7, 8,
          5, 8, 7, 7, 8, 8, 4, 8,
          8, 3, 3, 7, 3, 11, 8, 8,
          8, 8, 5, 7, 4, 7, 7, 10,
          7, 7, 7, 5, 3, 5, 8, 0}
     },
     /* Hardware Fonts index 22*/
     {
         1,
         2,
         14,
         20,
         1031548,
         {0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          5, 5, 6, 9, 9, 14, 11, 3,
          6, 6, 6, 10, 4, 5, 4, 5,
          9, 9, 9, 9, 9, 9, 9, 9,
          9, 9, 4, 4, 10, 10, 10, 9,
          17, 11, 11, 12, 12, 11, 10, 13,
          12, 4, 8, 11, 9, 13, 12, 13,
          11, 13, 12, 11, 10, 12, 11, 15,
          11, 11, 10, 5, 5, 5, 8, 9,
          6, 9, 9, 8, 9, 9, 5, 9,
          9, 3, 4, 8, 3, 14, 9, 9,
          9, 9, 5, 8, 5, 9, 8, 12,
          8, 8, 8, 6, 4, 6, 10, 0}
     },
     /* Hardware Fonts index 23*/
     {
         1,
         3,
         17,
         22,
         1024380,
         {0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          5, 6, 5, 10, 10, 16, 13, 3,
          6, 6, 7, 10, 5, 6, 5, 5,
          10, 10, 10, 10, 10, 10, 10, 10,
          10, 10, 5, 5, 10, 11, 10, 10,
          18, 13, 13, 14, 14, 13, 12, 15,
          14, 6, 10, 13, 11, 16, 14, 15,
          13, 15, 14, 13, 12, 14, 13, 18,
          13, 13, 12, 5, 5, 5, 9, 11,
          4, 11, 11, 10, 11, 10, 6, 11,
          10, 4, 4, 9, 4, 16, 10, 11,
          11, 11, 6, 9, 6, 10, 10, 14,
          10, 10, 9, 6, 5, 6, 10, 0}
     },
     /* Hardware Fonts index 24*/
     {
         1,
         3,
         24,
         29,
         1015356,
         {0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          6, 6, 8, 14, 13, 22, 17, 6,
          8, 8, 10, 14, 6, 8, 6, 7,
          13, 13, 13, 13, 13, 13, 13, 13,
          13, 13, 6, 6, 15, 15, 15, 12,
          25, 17, 17, 18, 18, 16, 14, 19,
          18, 8, 13, 18, 14, 21, 18, 18,
          16, 18, 17, 16, 16, 18, 17, 22,
          17, 16, 15, 7, 7, 7, 12, 14,
          7, 13, 14, 12, 14, 13, 8, 14,
          13, 6, 6, 12, 6, 20, 14, 13,
          14, 14, 9, 12, 8, 14, 13, 18,
          12, 13, 12, 8, 6, 8, 14, 0}
     },
     /* Hardware Fonts index 25*/
     {
         1,
         4,
         30,
         38,
         998684,
         {0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          9, 9, 12, 19, 18, 29, 22, 6,
          11, 11, 13, 19, 9, 11, 9, 9,
          18, 18, 18, 18, 18, 18, 18, 18,
          18, 18, 9, 9, 19, 19, 19, 18,
          34, 22, 22, 24, 24, 22, 20, 25,
          24, 9, 16, 22, 18, 27, 24, 25,
          22, 26, 24, 22, 20, 24, 22, 31,
          22, 22, 20, 9, 9, 9, 16, 18,
          11, 18, 18, 16, 18, 18, 9, 18,
          18, 7, 7, 16, 7, 27, 18, 18,
          18, 18, 11, 16, 9, 18, 16, 23,
          16, 16, 16, 11, 9, 11, 19, 0}
     },
     /* Hardware Fonts index 26*/
     {
         2,
         6,
         12,
         16,
         991260,
         {0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          3, 4, 5, 9, 8, 10, 9, 3,
          5, 5, 6, 8, 3, 6, 4, 6,
          8, 8, 8, 8, 8, 8, 8, 8,
          8, 8, 4, 4, 7, 8, 7, 7,
          13, 9, 9, 9, 9, 8, 8, 9,
          10, 4, 8, 9, 8, 12, 10, 10,
          9, 10, 9, 9, 9, 9, 9, 12,
          9, 9, 8, 4, 6, 4, 6, 7,
          4, 8, 8, 7, 8, 7, 5, 8,
          8, 4, 4, 8, 4, 12, 8, 8,
          8, 8, 5, 7, 5, 8, 7, 11,
          7, 7, 7, 5, 3, 5, 10, 3}
     },
     /* Hardware Fonts index 27*/
     {
         2,
         8,
         16,
         20,
         973852,
         {0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          4, 4, 6, 11, 10, 12, 11, 4,
          6, 6, 7, 10, 4, 8, 5, 7,
          10, 10, 10, 10, 10, 10, 10, 10,
          10, 10, 4, 4, 9, 10, 9, 8,
          15, 11, 11, 11, 12, 9, 9, 12,
          12, 5, 9, 11, 9, 15, 12, 12,
          11, 12, 11, 10, 10, 12, 11, 15,
          11, 11, 10, 5, 7, 5, 7, 8,
          5, 9, 10, 9, 10, 9, 6, 10,
          10, 4, 4, 9, 4, 15, 10, 10,
          10, 10, 6, 9, 6, 10, 9, 13,
          9, 9, 9, 6, 4, 6, 12, 4}
     },
     /* Hardware Fonts index 28*/
     {
         2,
         9,
         18,
         25,
         950172,
         {0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          5, 6, 8, 13, 12, 15, 13, 5,
          7, 7, 9, 12, 5, 9, 6, 9,
          12, 12, 12, 12, 12, 12, 12, 12,
          12, 12, 5, 5, 11, 12, 11, 10,
          19, 13, 13, 13, 14, 12, 12, 14,
          15, 6, 12, 14, 12, 18, 15, 14,
          13, 15, 13, 13, 13, 14, 14, 18,
          13, 13, 13, 6, 9, 6, 9, 10,
          7, 12, 12, 11, 12, 11, 8, 12,
          12, 5, 5, 11, 5, 18, 12, 12,
          12, 12, 7, 11, 7, 12, 11, 16,
          11, 11, 11, 7, 5, 7, 14, 5}
     },
     /* Hardware Fonts index 29*/
     {
         2,
         11,
         22,
         28,
         917948,
         {0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          6, 6, 9, 15, 14, 18, 15, 5,
          8, 8, 10, 14, 5, 11, 6, 10,
          14, 14, 14, 14, 14, 14, 14, 14,
          14, 14, 6, 6, 12, 14, 13, 11,
          21, 15, 15, 15, 16, 13, 13, 16,
          17, 7, 13, 15, 13, 21, 17, 16,
          15, 17, 15, 15, 14, 16, 15, 21,
          15, 15, 14, 7, 10, 6, 10, 11,
          8, 13, 14, 13, 14, 13, 9, 14,
          14, 6, 6, 13, 6, 21, 14, 14,
          14, 14, 8, 13, 8, 14, 12, 18,
          12, 12, 12, 8, 6, 8, 16, 6}
     },
     /* Hardware Fonts index 30*/
     {
         2,
         14,
         28,
         36,
         863292,
         {0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          8, 8, 11, 19, 18, 23, 19, 7,
          11, 10, 13, 18, 7, 14, 8, 13,
          17, 17, 17, 17, 17, 17, 17, 17,
          17, 17, 8, 8, 16, 17, 16, 15,
          28, 20, 20, 20, 21, 17, 17, 21,
          22, 9, 17, 20, 17, 27, 22, 21,
          20, 22, 20, 19, 19, 21, 20, 27,
          20, 20, 19, 8, 13, 8, 13, 15,
          10, 17, 18, 16, 18, 16, 11, 18,
          18, 8, 8, 16, 8, 27, 18, 18,
          18, 18, 11, 16, 10, 18, 16, 23,
          16, 16, 16, 11, 8, 11, 21, 8}
     },
     /* Hardware Fonts index 31*/
     {
         2,
         18,
         36,
         49,
         766524,
         {0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          10, 11, 15, 26, 24, 31, 26, 9,
          14, 14, 18, 24, 9, 19, 11, 17,
          24, 24, 24, 24, 24, 24, 24, 24,
          24, 24, 11, 11, 21, 24, 22, 20,
          38, 27, 27, 27, 28, 23, 23, 28,
          30, 12, 23, 27, 23, 36, 30, 29,
          27, 29, 27, 26, 25, 28, 27, 36,
          27, 27, 25, 11, 18, 11, 18, 20,
          13, 23, 24, 22, 24, 22, 15, 24,
          24, 11, 11, 22, 11, 37, 24, 24,
          24, 24, 15, 22, 13, 24, 21, 32,
          21, 21, 21, 14, 10, 14, 29, 10}
     }
 }
 ;
 
 // Define maximum play delay count for mute/unmute sounds
 static const uint16_t maximum_play_delay_count = 256;
 
 Ft800::Ft800 (uint8_t bus_number, uint8_t channel_number): ft800_interface (bus_number, channel_number)
 {
     display_initialised = false;
     write_index = 0;
     keyclick_code = SILENCE;
     vertical_font = vertical_font_not_initialised;
 }
 
 Ft800::~Ft800 (void)
 {
 }
 
 bool Ft800::initialise (bool force, uint8_t red, uint8_t green, uint8_t blue)
 {
     bool result = false;
 
     if (!force)
     {
         // Test the display's registers to find if it has been initialised
         if (test_registers_initialised ())
         {
             // Display already initialised
             uint32_t result_cmd_write;
 
             if (ft800_interface.read_32_bit (REG_CMD_WRITE, &result_cmd_write))
             {
                 display_initialised = true;
                 write_index = result_cmd_write;
                 result = true;
             }
             else
             {
                 // Force initialisation on error
                 force = true;
             }
         }
         else
         {
             // Expected registers not set - force initialisation
             force = true;
         }
     }
 
     if (force)
     {
         if (ft800_interface.host_command (Ft800_interface::CORERST))
         {
             usleep (10000);
 
             if (ft800_interface.host_command (Ft800_interface::ACTIVE))
             {
                 usleep (20000);
 
                 uint8_t id_result;
                 if ((ft800_interface.read_8_bit (REG_ID, &id_result)) &&
                     (FT800_ID == id_result))
                 {
                     // An FT800 has been detected
                     if (ft800_interface.host_command (Ft800_interface::CLKEXT))
                     {
                         usleep (10000);
 
                         // Configure display registers - WQVGA resolution
                         if (ft800_interface.write_16_bit (REG_HCYCLE, 548) &&
                             ft800_interface.write_16_bit (REG_HOFFSET, 43) &&
                             ft800_interface.write_16_bit (REG_HSYNC0, 0) &&
                             ft800_interface.write_16_bit (REG_HSYNC1, 41) &&
                             ft800_interface.write_16_bit (REG_VCYCLE, 292) &&
                             ft800_interface.write_16_bit (REG_VOFFSET, 12) &&
                             ft800_interface.write_16_bit (REG_VSYNC0, 0) &&
                             ft800_interface.write_16_bit (REG_VSYNC1, 10) &&
                             ft800_interface.write_8_bit (REG_SWIZZLE, 0) &&
                             ft800_interface.write_8_bit (REG_PCLK_POL, 1) &&
                             ft800_interface.write_8_bit (REG_CSPREAD, 1) &&
                             ft800_interface.write_16_bit (REG_HSIZE, 480) &&
                             ft800_interface.write_16_bit (REG_VSIZE, 272))
                         {
                             // Mute sound to prevent start-up pop
                             mute_sound ();
 
                             // Write initial display to clear the display
                             if (ft800_interface.write_32_bit (RAM_DL+0, ft800_interface.clear_color_rgb (red, green, blue)) &&
                                 ft800_interface.write_32_bit (RAM_DL+4, ft800_interface.clear (1, 1, 1)) &&
                                 ft800_interface.write_32_bit (RAM_DL+8, ft800_interface.display ()) &&
                                 ft800_interface.write_8_bit (REG_DLSWAP, DLSWAP_FRAME))
                             {
                                 // Enable the display bit
                                 uint8_t result_8;
                                 if (ft800_interface.read_8_bit (REG_GPIO_DIR, &result_8))
                                 {
                                     if (ft800_interface.write_8_bit (REG_GPIO_DIR, 0x83 | result_8))
                                     {
                                         if (ft800_interface.read_8_bit (REG_GPIO, &result_8))
                                         {
                                             if (ft800_interface.write_8_bit (REG_GPIO, 0x083 | result_8))
                                             {
                                                 // Final command to make display visible...
                                                 if (ft800_interface.write_8_bit (REG_PCLK, 5))
                                                 {
                                                     usleep (20000);
 
                                                     // Test if initialisation complete
                                                     if (test_registers_initialised ())
                                                     {
                                                         unmute_sound ();
                                                         display_initialised = true;
                                                         write_index = 0;
                                                         result = true;
                                                     }
                                                 }
                                             }
                                         }
                                     }
                                 }
                             }
                         }
                     }
                 }
             }
         }
     }
 
     return result;
 }
 
 void Ft800::cmd_dlstart (void)
 {
     if (display_initialised)
     {
         write_command_32_bit (dlstart_command);
     }
 }
 
 void Ft800::cmd_swap (void)
 {
     if (display_initialised)
     {
         write_command_32_bit (dlswap_command);
     }
 }
 
 void Ft800::cmd_calibrate (void)
 {
     if (display_initialised)
     {
         write_command_32_bit (calibrate_command);
         write_command_32_bit (0);
     }
 }
 
 bool Ft800::calibration_complete (calibration_stc_t &calibration)
 {
     bool result = false;
 
     if (display_initialised)
     {
         if (fifo_empty ())
         {
             if (ft800_interface.read_32_bit (REG_TOUCH_TRANSFORM_A, &calibration.transform_a) &&
                 ft800_interface.read_32_bit (REG_TOUCH_TRANSFORM_B, &calibration.transform_b) &&
                 ft800_interface.read_32_bit (REG_TOUCH_TRANSFORM_C, &calibration.transform_c) &&
                 ft800_interface.read_32_bit (REG_TOUCH_TRANSFORM_D, &calibration.transform_d) &&
                 ft800_interface.read_32_bit (REG_TOUCH_TRANSFORM_E, &calibration.transform_e) &&
                 ft800_interface.read_32_bit (REG_TOUCH_TRANSFORM_F, &calibration.transform_f))
             {
                 result = true;
             }
         }
     }
 
     return result;
 }
 
 void Ft800::set_calibration (calibration_stc_t &calibration)
 {
     if (display_initialised)
     {
         ft800_interface.write_32_bit (REG_TOUCH_TRANSFORM_A, calibration.transform_a);
         ft800_interface.write_32_bit (REG_TOUCH_TRANSFORM_B, calibration.transform_b);
         ft800_interface.write_32_bit (REG_TOUCH_TRANSFORM_C, calibration.transform_c);
         ft800_interface.write_32_bit (REG_TOUCH_TRANSFORM_D, calibration.transform_d);
         ft800_interface.write_32_bit (REG_TOUCH_TRANSFORM_E, calibration.transform_e);
         ft800_interface.write_32_bit (REG_TOUCH_TRANSFORM_F, calibration.transform_f);
     }
 }
 
 void Ft800::clear_color_rgb (uint8_t red, uint8_t green, uint8_t blue)
 {
     if (display_initialised)
     {
         write_command_32_bit (ft800_interface.clear_color_rgb  (red, green, blue));
     }
 }
 
 void Ft800::clear (uint8_t color, uint8_t stencil, uint8_t tag)
 {
     if (display_initialised)
     {
        write_command_32_bit (ft800_interface.clear (color, stencil, tag));
     }
 }
 
 void Ft800::display (void)
 {
     if (display_initialised)
     {
         write_command_32_bit (ft800_interface.display ());
     }
 }
 
 void Ft800::color_rgb (uint8_t red, uint8_t green, uint8_t blue)
 {
     if (display_initialised)
     {
         write_command_32_bit (ft800_interface.color_rgb  (red, green, blue));
     }
 }
 
 void Ft800::color_alpha (uint8_t alpha)
 {
     if (display_initialised)
     {
         write_command_32_bit (ft800_interface.color_alpha  (alpha));
     }
 }
 
 void Ft800::cmd_bgcolor (uint32_t rgb)
 {
     if (display_initialised)
     {
         write_command_32_bit (background_colour_command);
         write_command_32_bit (rgb);
     }
 }
 
 void Ft800::cmd_fgcolor (uint32_t rgb)
 {
     if (display_initialised)
     {
         write_command_32_bit (foreground_colour_command);
         write_command_32_bit (rgb);
     }
 }
 
 void Ft800::cmd_text (uint16_t x, uint16_t y, uint16_t font, option_t option, const char* string)
 {
     if (display_initialised)
     {
         write_command_32_bit (text_command);
         write_command_32_bit (((uint32_t)y << 16) | (uint32_t)x);
         write_command_32_bit (((uint32_t)option << 16) | (uint32_t)font);
         write_command_string (string);
     }
 }
 
 void Ft800::cmd_button (uint16_t x, uint16_t y, uint16_t width, int16_t height, uint16_t font, option_t option, const char* string)
 {
     if (display_initialised)
     {
         write_command_32_bit (button_command);
         write_command_32_bit (((uint32_t)y << 16) | (uint32_t)x);
         write_command_32_bit (((uint32_t)height << 16) | (uint32_t)width);
         write_command_32_bit (((uint32_t)option << 16) | (uint32_t)font);
         write_command_string (string);
     }
 }
 
 void Ft800::cmd_keys (uint16_t x, uint16_t y, uint16_t width, int16_t height, uint16_t font, option_t option, const char* string)
 {
     if (display_initialised)
     {
         write_command_32_bit (keys_command);
         write_command_32_bit (((uint32_t)y << 16) | (uint32_t)x);
         write_command_32_bit (((uint32_t)height << 16) | (uint32_t)width);
         write_command_32_bit (((uint32_t)option << 16) | (uint32_t)font);
         write_command_string (string);
     }
 }
 
 void Ft800::cmd_keys (uint16_t x, uint16_t y, uint16_t width, int16_t height, uint16_t font, char pressed, const char* string)
 {
     if (display_initialised)
     {
         write_command_32_bit (keys_command);
         write_command_32_bit (((uint32_t)y << 16) | (uint32_t)x);
         write_command_32_bit (((uint32_t)height << 16) | (uint32_t)width);
         write_command_32_bit (((uint32_t)pressed << 16) | (uint32_t)font);
         write_command_string (string);
     }
 }
 
 void Ft800::cmd_spinner (uint16_t x, uint16_t y, spinner_style_t style, uint16_t scale)
 {
     if (display_initialised)
     {
         write_command_32_bit (spinner_command);
         write_command_32_bit (((uint32_t)y << 16) | (uint32_t)x);
         write_command_32_bit (((uint32_t)scale << 16) | (uint32_t)style);
     }
 }
 
 void Ft800::load_bitmap (uint32_t ram_g_offset, const uint8_t* bitmap, size_t length)
 {
     if (display_initialised)
     {
         for (size_t index = 0; index < length; index++)
         {
             ft800_interface.write_8_bit (RAM_G + ram_g_offset + index, bitmap[index]);
         }
     }
 }
 
 void Ft800::cmd_inflate (uint32_t ram_g_offset, const uint8_t* bitmap, size_t length)
 {
     if (display_initialised)
     {
         write_command_32_bit (inflate_command);
         write_command_32_bit (RAM_G + ram_g_offset);
 
         for (size_t stride = 0; stride < length / 4; stride++)
         {
             size_t index = stride * 4;
             write_command_32_bit ((bitmap[index + 3] << 24) |
                                   (bitmap[index + 2] << 16) |
                                   (bitmap[index + 1] << 8) |
                                   (bitmap[index]));
         }
         if (0 != (length % 4))
         {
             uint8_t remainder[4] = {0, 0, 0, 0};
             size_t bytes_remaining = length % 4;
             for (size_t count = 0; count < bytes_remaining; count++)
             {
                 remainder[count] = bitmap[(length - bytes_remaining) + count];
             }
             write_command_32_bit ((remainder[3] << 24) |
                                   (remainder[2] << 16) |
                                   (remainder[1] << 8) |
                                   (remainder[0]));            
         }
     }
 }
 
 void Ft800::bitmap_handle (uint8_t handle)
 {
     if (display_initialised)
     {
         write_command_32_bit (ft800_interface.bitmap_handle (handle));
     }   
 }
 
 void Ft800::bitmap_source (uint32_t ram_g_offset)
 {
     if (display_initialised)
     {
         write_command_32_bit (ft800_interface.bitmap_source (RAM_G + ram_g_offset));
     }
 }
 
 void Ft800::bitmap_layout (bitmap_format_t format, uint16_t stride, uint16_t height)
 {
     if (display_initialised)
     {
         write_command_32_bit (ft800_interface.bitmap_layout ((uint16_t)format, stride, height));
     }
 }
 
 void Ft800::bitmap_size (filter_t filter, wrap_t wrap_x, wrap_t wrap_y, uint16_t width, uint16_t height)
 {
     if (display_initialised)
     {
         write_command_32_bit (ft800_interface.bitmap_size ((uint8_t)filter, (uint8_t)wrap_x, (uint8_t)wrap_y, width, height));
     }
 }
 
 void Ft800::begin_bitmap (void)
 {
     if (display_initialised)
     {
         write_command_32_bit (ft800_interface.begin (Ft800_interface::BITMAPS));
     }
 }
 
 void Ft800::begin_point (void)
 {
     if (display_initialised)
     {
         write_command_32_bit (ft800_interface.begin (Ft800_interface::POINTS));
     }
 }
 
 void Ft800::begin_line (void)
 {
     if (display_initialised)
     {
         write_command_32_bit (ft800_interface.begin (Ft800_interface::LINES));
     }
 }
 
 void Ft800::begin_line_strip (void)
 {
     if (display_initialised)
     {
         write_command_32_bit (ft800_interface.begin (Ft800_interface::LINE_STRIP));
     }
 }
 
 void Ft800::begin_rectangle (void)
 {
     if (display_initialised)
     {
         write_command_32_bit (ft800_interface.begin (Ft800_interface::RECTS));
     }
 }
 
 void Ft800::end (void)
 {
     if (display_initialised)
     {
         write_command_32_bit (ft800_interface.end ());
     }
 }
 
 void Ft800::vertex2f (int32_t x, int32_t y)
 {
     if (display_initialised)
     {
         write_command_32_bit (ft800_interface.vertex2f (x, y));
     }
 }
 
 void Ft800::vertex2ii (uint16_t x, uint16_t y, uint8_t handle, uint8_t cell)
 {
     if (display_initialised)
     {
         write_command_32_bit (ft800_interface.vertex2ii (x, y, handle, cell));
     }
 }
 
 void Ft800::tag (uint8_t value)
 {
     if (display_initialised)
     {
         write_command_32_bit (ft800_interface.tag (value));
     }
 }
 
 void Ft800::point_size (uint16_t size)
 {
     if (display_initialised)
     {
         write_command_32_bit (ft800_interface.point_size (size));
     }
 }
 
 void Ft800::line_width (uint16_t width)
 {
     if (display_initialised)
     {
         write_command_32_bit (ft800_interface.line_width (width));
     }
 }
 
 void Ft800::cmd_macro (uint8_t macro_id)
 {
     if (display_initialised)
     {
         write_command_32_bit (ft800_interface.macro (macro_id));
     }
 }
 
 void Ft800::set_macro_color_rgb (uint8_t macro_id, uint8_t red, uint8_t green, uint8_t blue)
 {
     if (1 == macro_id)
     {
         ft800_interface.write_32_bit (REG_MACRO_1, ft800_interface.color_rgb  (red, green, blue));
     }
     else
     {
         // Default to macro 0
         ft800_interface.write_32_bit (REG_MACRO_0, ft800_interface.color_rgb  (red, green, blue));
     }
 }
 
 bool Ft800::get_touch_raw_xy (uint16_t* x, uint16_t* y)
 {
     bool result = false;
 
     if ((NULL != x) && (NULL != y))
     {
         uint32_t raw_xy_value;
         if (ft800_interface.read_32_bit (REG_TOUCH_RAW_XY, &raw_xy_value))
         {
             if (0xFFFFFFFF != raw_xy_value)
             {
                 *x = (uint16_t)(raw_xy_value >> 16);
                 *y = (uint16_t)(raw_xy_value &  0xFFFF);
                 result = true;
             }
         }
     }
 
     return result;
 }
 
 bool Ft800::get_touch_screen_xy (uint16_t* x, uint16_t* y)
 {
     bool result = false;
 
     if ((NULL != x) && (NULL != y))
     {
         uint32_t screen_xy_value;
         if (ft800_interface.read_32_bit (REG_TOUCH_SCREEN_XY, &screen_xy_value))
         {
             if (0x80008000 != screen_xy_value)
             {
                 *x = (uint16_t)(screen_xy_value >> 16);
                 *y = (uint16_t)(screen_xy_value &  0xFFFF);
                 result = true;
             }
         }
     }
 
     return result;
 }
 
 bool Ft800::get_touch_tag (uint8_t* tag)
 {
     bool result = false;
 
     if (NULL != tag)
     {
         uint8_t tag_value;
         if (ft800_interface.read_8_bit (REG_TOUCH_TAG, &tag_value))
         {
             *tag = tag_value;
             result = true;
         }
     }
 
     return result;
 }
 
 void Ft800::vertical_text (uint16_t x, uint16_t y, uint8_t font, const char* string)
 {
     if (display_initialised)
     {
         if (font != vertical_font)
         {
             write_command_32_bit (ft800_interface.bitmap_handle (vertical_text_internal_handle));
             write_command_32_bit (ft800_interface.bitmap_layout (ft800_rom_fonts[font - ft800_first_rom_font].bitmap_format,
                                                                  ft800_rom_fonts[font - ft800_first_rom_font].bitmap_stride,
                                                                  ft800_rom_fonts[font - ft800_first_rom_font].bitmap_height));
             write_command_32_bit (ft800_interface.bitmap_source (ft800_rom_fonts[font - ft800_first_rom_font].bitmap_rom_address));
             vertical_font = font;
         }
         if ((ft800_first_rom_font <= font) &&
             (ft800_last_rom_font >= font) &&
             (NULL != string))
         {
             size_t length = strnlen (string, 32);
 
             if (0 < length)
             {
                 write_command_32_bit (ft800_interface.save_context ());
                 write_command_32_bit (ft800_interface.bitmap_handle (vertical_text_internal_handle));
                 // Set size to largest dimension (always height)
                 write_command_32_bit (ft800_interface.bitmap_size (Ft800::NEAREST,
                                                                    Ft800::BORDER,
                                                                    Ft800::BORDER,
                                                                    ft800_rom_fonts[font - ft800_first_rom_font].bitmap_height,
                                                                    ft800_rom_fonts[font - ft800_first_rom_font].bitmap_height));
 
                 // Rotate 90 anti-clockwise
                 write_command_32_bit (load_identity_command);
                 write_command_32_bit (translate_command);
                 write_command_32_bit (ft800_rom_fonts[font - ft800_first_rom_font].bitmap_width * 65536 / 2);
                 write_command_32_bit (ft800_rom_fonts[font - ft800_first_rom_font].bitmap_height * 65536 / 2);
                 write_command_32_bit (rotate_command);
                 write_command_32_bit (-90 * 65536 / 360);
                 write_command_32_bit (translate_command);
                 write_command_32_bit (ft800_rom_fonts[font - ft800_first_rom_font].bitmap_width * -65536 / 2);
                 write_command_32_bit (ft800_rom_fonts[font - ft800_first_rom_font].bitmap_height * -65536 / 2);
                 write_command_32_bit (set_matrix_command);
 
                 // Layout the text
                 // Note that after rotation the top left of the character is at the bottom left of the
                 // bitmap, so the starting y position must be adjusted to achieve the expected start position
                 // for the string.
                 y -= ft800_rom_fonts[font - ft800_first_rom_font].bitmap_height;
                 write_command_32_bit (ft800_interface.begin (Ft800_interface::BITMAPS));
                 for (size_t index = 0; index < length; index++)
                 {
                     write_command_32_bit (ft800_interface.vertex2ii (x, y, vertical_text_internal_handle, string[index]));
                     if (ft800_defined_character_codes > string[index])
                     {
                         y -= ft800_rom_fonts[font - ft800_first_rom_font].font_width[string[index]];
                     }
                 }
                 write_command_32_bit (ft800_interface.end ());
 
                 write_command_32_bit (ft800_interface.restore_context ());
             }
         }
     }
 }
 
 void Ft800::configure_keyclick (uint16_t sound_code)
 {
     if ((SILENCE == sound_code) ||
         ((HARP_SOUND <= sound_code) && (BELL_SOUND >= sound_code)) ||
         ((CLICK_SOUND <= sound_code) && (CHACK_SOUND >= sound_code)) ||
         (MUTE_SOUND == sound_code) ||
         (UNMUTE_SOUND == sound_code))
     {
         keyclick_code = sound_code;
     }
     else
     {
         keyclick_code = CLICK_SOUND;
     }
 }
 
 void Ft800::key_click (void)
 {
     if (SILENCE != keyclick_code)
     {
         ft800_interface.write_8_bit (REG_VOL_SOUND, 0x80);
         ft800_interface.write_16_bit (REG_SOUND, keyclick_code);
         ft800_interface.write_8_bit (REG_PLAY, 1);
     }
 }
 
 void Ft800::mute_sound (void)
 {
     ft800_interface.write_16_bit (REG_SOUND, MUTE_SOUND);
     ft800_interface.write_8_bit (REG_PLAY, 1);
 
     bool finished = false;
     uint16_t count = 0;
 
     while (!finished)
     {
         uint8_t play_value;
         if (ft800_interface.read_8_bit (REG_PLAY, &play_value))
         {
             if (0 == play_value)
             {
                 finished = true;
             }
             else
             {
                 if (maximum_play_delay_count > count)
                 {
                     // Still playing - pause a bit...
                     usleep (1000);
                     count++;
                 }
                 else
                 {
                     // Timeout - just exit
                     finished = true;
                 }
             }
         }
         else
         {
             // Read error - just exit
             finished = true;
         }
     }
 }
 
 void Ft800::set_backlight (uint8_t percent)
 {
     uint8_t pwm_value = BACKLIGHT_PWM_MAX;
 
     if (100 > percent)
     {
         uint16_t temp_value = static_cast<uint16_t>(BACKLIGHT_PWM_MAX) * static_cast<uint16_t>(percent);
         temp_value /= 100;
         pwm_value = static_cast<uint8_t>(temp_value & ~BACKLIGHT_PWM_MAX);
     }
 
     ft800_interface.write_8_bit (REG_PWM_DUTY, pwm_value);
 }
 
 void Ft800::unmute_sound (void)
 {
     ft800_interface.write_16_bit (REG_SOUND, UNMUTE_SOUND);
     ft800_interface.write_8_bit (REG_PLAY, 1);
 
     bool finished = false;
     uint16_t count = 0;
 
     while (!finished)
     {
         uint8_t play_value;
         if (ft800_interface.read_8_bit (REG_PLAY, &play_value))
         {
             if (0 == play_value)
             {
                 finished = true;
             }
             else
             {
                 if (maximum_play_delay_count > count)
                 {
                     // Still playing - pause a bit...
                     usleep (1000);
                     count++;
                 }
                 else
                 {
                     // Timeout - just exit
                     finished = true;
                 }
             }
         }
         else
         {
             // Read error - just exit
             finished = true;
         }
     }
 }
 
 bool Ft800::write_command_32_bit (uint32_t value)
 {
     bool result = false;
 
     if (ft800_interface.write_32_bit (RAM_CMD + write_index, value))
     {
         update_fifo_write_pointer (sizeof (uint32_t));
         result = true;
     }
 
     return result;
 }
 
 bool Ft800::write_command_string (const char *string)
 {
     // Write the (null terminated) string
     return write_command_buffer ((uint8_t*)string, strlen (string) + 1);
 }
 
 bool Ft800::write_command_buffer (const uint8_t* buffer, size_t length)
 {
     bool result = true;
 
     size_t index = 0;
 
     while ((index < length) && result)
     {
         // Handle fifo wrap-round
         uint32_t fifo_address = RAM_CMD + ((write_index + index) % command_fifo_size);
         result = ft800_interface.write_8_bit (fifo_address, buffer[index]);
         index++;
     }
 
     if (result)
     {
         update_fifo_write_pointer (length);
     }
 
     return result;
 }
 
 void Ft800::update_fifo_write_pointer (size_t size)
 {
     // Ensure increment of 4 byte multiples
     if (0 != (size % 4))
     {
         size += 4 - (size % 4);
     }
     write_index += (uint16_t)size;
 
     // Wrap round the circular buffer
     write_index %= command_fifo_size;
 
     // Update the FT800
     ft800_interface.write_32_bit (REG_CMD_WRITE, write_index);
 }
 
 bool Ft800::fifo_empty (void)
 {
     bool result = false;
 
     uint32_t cmd_read;
     if (ft800_interface.read_32_bit (REG_CMD_READ, &cmd_read))
     {
         if (cmd_read == write_index)
         {
             result = true;
         }
     }
 
     return result;
 }
 
 bool Ft800::test_registers_initialised (void)
 {
     bool result = false;
 
     uint8_t result_gpio_dir;
     uint8_t result_gpio;
     uint8_t result_pclk;
 
     if ((ft800_interface.read_8_bit (REG_GPIO_DIR, &result_gpio_dir)) &&
         (ft800_interface.read_8_bit (REG_GPIO, &result_gpio)) &&
         (ft800_interface.read_8_bit (REG_PCLK, &result_pclk)))
     {
         if ((0x83 == (result_gpio_dir & 0x83)) &&
             (0x83 == (result_gpio & 0x83)) &&
             (5 == (result_pclk & 5)))
         {
             // Registers set as expected for initialised display
             result = true;
         }
     }
 
     return result;
 }
 