/*!
 * \file main.cpp
 * \brief Example main() located in /src using FT800 graphics library
 */

 #include <cstdio>
 #include <unistd.h>
 #include "../graphics/ft800.h"
 
 int main()
 {
     Ft800 ft800(1, 0);
 
     if (ft800.initialise(1, 0x10, 0x10, 0x10) == 0)
     {
         printf("FT800 display initialized successfully.\n");
 
         ft800.cmd_dlstart();
         ft800.clear_color_rgb(0x00, 0x00, 0x00);
         ft800.clear(1, 1, 1);
         ft800.color_rgb(255, 255, 255);
         ft800.cmd_text(240, 136, 30, Ft800::OPT_CENTER, "FT800 SDK Test");
         ft800.display();
         ft800.cmd_swap();
 
         sleep(3);
     }
     else
     {
         printf("FT800 initialization failed.\n");
         return -1;
     }
 
     return 0;
 }
 