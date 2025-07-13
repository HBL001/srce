
/**
 * @file graph_ft800.h
 * @brief Header for GraphFt800 class interfacing with FT800 kernel driver via IOCTL.
 */

 #ifndef GRAPH_FT800_H
 #define GRAPH_FT800_H
 
 #include <cstdint>
 #include <cstddef>
 
 struct ft800_cal_data;  // Forward declare for calibration
 
 class GraphFt800
 {
 public:
     explicit GraphFt800(const char* device_path);
     ~GraphFt800();
 
     bool initialise();
     void cmd_dlstart();
     void cmd_swap();
     void cmd_button(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t font, uint16_t options, const char* text);
     void cmd_text(uint16_t x, uint16_t y, uint16_t font, uint16_t options, const char* text);
     void cmd_spinner(uint16_t x, uint16_t y, uint16_t style, uint16_t scale);
     void cmd_calibrate();
     void begin_bitmap(uint8_t handle);
     void bitmap_layout(uint16_t format, uint16_t linestride, uint16_t height);
     void bitmap_size(uint8_t filter, uint8_t wrapx, uint8_t wrapy, uint16_t width, uint16_t height);
     void clear(bool c, bool s, bool t);
     void clear_color_rgb(uint8_t r, uint8_t g, uint8_t b);
     void display();
     void end();
     bool get_touch_raw_xy(uint16_t* x, uint16_t* y);
     bool get_touch_screen_xy(uint16_t* x, uint16_t* y);
     uint8_t get_touch_tag();
     void tag(uint8_t tag);
     bool fifo_empty();
     void update_fifo_write_pointer(uint32_t ptr);
     void load_bitmap(uint32_t dst_addr, const void* src, size_t size);
     void set_calibration(const struct ft800_cal_data& cal);
     bool calibration_complete();
 
 private:
     int fd;
     bool display_initialised;
     uint32_t write_index;
 };
 
 #endif // GRAPH_FT800_H
 