/*!
 * \file ft800Reg.h
 * \brief FT800 Register and Memory Map Definitions
 */

 #ifndef FT800_REG_H
 #define FT800_REG_H
 
 // --------------------------------------
 // FT800 Memory Map Regions
 // --------------------------------------
 
 static const uint32_t RAM_G          = 0x000000;  //!< General-purpose RAM
 static const uint32_t ROM_CHIPID     = 0x0C0000;  //!< Chip ID ROM
 static const uint32_t ROM_FONT       = 0x0BB23C;  //!< Font ROM
 static const uint32_t ROM_FONT_ADDR  = 0x0FFFFC;  //!< Font ROM address pointer
 static const uint32_t RAM_DL         = 0x100000;  //!< Display List RAM
 static const uint32_t RAM_PAL        = 0x102000;  //!< Palette RAM
 static const uint32_t RAM_CMD        = 0x108000;  //!< Co-Processor Command Buffer RAM
 
 // --------------------------------------
 // FT800 Register Addresses
 // --------------------------------------
 
 static const uint32_t REG_ID             = 0x102400; //!< Device ID
 static const uint32_t REG_FRAMES         = 0x102404; //!< Frame counter
 static const uint32_t REG_CLOCK          = 0x102408; //!< System clock cycles
 static const uint32_t REG_FREQUENCY      = 0x10240C; //!< System clock frequency
 static const uint32_t REG_RENDERMODE     = 0x102410; //!< Rendering mode
 static const uint32_t REG_SNAPY          = 0x102414; //!< Snapshot Y-coordinate
 static const uint32_t REG_SNAPSHOT       = 0x102418; //!< Snapshot control
 static const uint32_t REG_CPURESET       = 0x10241C; //!< CPU reset control
 static const uint32_t REG_TAP_CRC        = 0x102420; //!< Test access port CRC
 static const uint32_t REG_TAP_MASK       = 0x102424; //!< Test access port mask
 static const uint32_t REG_HCYCLE         = 0x102428; //!< Horizontal cycle
 static const uint32_t REG_HOFFSET        = 0x10242C; //!< Horizontal offset
 static const uint32_t REG_HSIZE          = 0x102430; //!< Horizontal size
 static const uint32_t REG_HSYNC0         = 0x102434; //!< Horizontal sync timing 0
 static const uint32_t REG_HSYNC1         = 0x102438; //!< Horizontal sync timing 1
 static const uint32_t REG_VCYCLE         = 0x10243C; //!< Vertical cycle
 static const uint32_t REG_VOFFSET        = 0x102440; //!< Vertical offset
 static const uint32_t REG_VSIZE          = 0x102444; //!< Vertical size
 static const uint32_t REG_VSYNC0         = 0x102448; //!< Vertical sync timing 0
 static const uint32_t REG_VSYNC1         = 0x10244C; //!< Vertical sync timing 1
 static const uint32_t REG_DLSWAP         = 0x102450; //!< Display List swap control
 static const uint32_t REG_ROTATE         = 0x102454; //!< Screen rotation
 static const uint32_t REG_OUTBITS        = 0x102458; //!< Display output format
 static const uint32_t REG_DITHER         = 0x10245C; //!< Dithering enable
 static const uint32_t REG_SWIZZLE        = 0x102460; //!< Output color swizzle
 static const uint32_t REG_CSPREAD        = 0x102464; //!< Clock spread spectrum enable
 static const uint32_t REG_PCLK_POL       = 0x102468; //!< Pixel clock polarity
 static const uint32_t REG_PCLK           = 0x10246C; //!< Pixel clock
 static const uint32_t REG_TAG_X          = 0x102470; //!< Touch tag X-coordinate
 static const uint32_t REG_TAG_Y          = 0x102474; //!< Touch tag Y-coordinate
 static const uint32_t REG_TAG            = 0x102478; //!< Touch tag value
 static const uint32_t REG_VOL_PB         = 0x10247C; //!< Playback volume
 static const uint32_t REG_VOL_SOUND      = 0x102480; //!< Sound synthesizer volume
 static const uint32_t REG_SOUND          = 0x102484; //!< Sound synthesizer effect
 static const uint32_t REG_PLAY           = 0x102488; //!< Sound play trigger
 static const uint32_t REG_GPIO_DIR       = 0x10248C; //!< GPIO direction control
 static const uint32_t REG_GPIO           = 0x102490; //!< GPIO data
 
 static const uint32_t REG_INT_FLAGS      = 0x102498; //!< Interrupt flags
 static const uint32_t REG_INT_EN         = 0x10249C; //!< Interrupt enable
 static const uint32_t REG_INT_MASK       = 0x1024A0; //!< Interrupt mask
 
 // Sound playback
 static const uint32_t REG_PLAYBACK_START = 0x1024A4; //!< Playback buffer start address
 static const uint32_t REG_PLAYBACK_LENGTH = 0x1024A8; //!< Playback buffer length
 static const uint32_t REG_PLAYBACK_READPTR = 0x1024AC; //!< Playback buffer read pointer
 static const uint32_t REG_PLAYBACK_FREQ  = 0x1024B0; //!< Playback frequency
 static const uint32_t REG_PLAYBACK_FORMAT = 0x1024B4; //!< Playback format
 static const uint32_t REG_PLAYBACK_LOOP  = 0x1024B8; //!< Playback loop enable
 static const uint32_t REG_PLAYBACK_PLAY  = 0x1024BC; //!< Playback trigger
 
 // PWM control
 static const uint32_t REG_PWM_HZ         = 0x1024C0; //!< PWM frequency
 static const uint32_t REG_PWM_DUTY       = 0x1024C4; //!< PWM duty cycle
 
 // Macros and Co-Processor
 static const uint32_t REG_MACRO_0        = 0x1024C8; //!< Macro 0
 static const uint32_t REG_MACRO_1        = 0x1024CC; //!< Macro 1
 static const uint32_t REG_CMD_READ       = 0x1024E4; //!< Command buffer read pointer
 static const uint32_t REG_CMD_WRITE      = 0x1024E8; //!< Command buffer write pointer
 static const uint32_t REG_CMD_DL         = 0x1024EC; //!< Command buffer Display List
 
 // Touchscreen settings
 static const uint32_t REG_TOUCH_MODE     = 0x1024F0;
 static const uint32_t REG_TOUCH_ADC_MODE = 0x1024F4;
 static const uint32_t REG_TOUCH_CHARGE   = 0x1024F8;
 static const uint32_t REG_TOUCH_SETTLE   = 0x1024FC;
 static const uint32_t REG_TOUCH_OVERSAMPLE = 0x102500;
 static const uint32_t REG_TOUCH_RZTHRESH = 0x102504;
 static const uint32_t REG_TOUCH_RAW_XY   = 0x102508;
 static const uint32_t REG_TOUCH_RZ       = 0x10250C;
 static const uint32_t REG_TOUCH_SCREEN_XY = 0x102510;
 static const uint32_t REG_TOUCH_TAG_XY   = 0x102514;
 static const uint32_t REG_TOUCH_TAG      = 0x102518;
 static const uint32_t REG_TOUCH_TRANSFORM_A = 0x10251C;
 static const uint32_t REG_TOUCH_TRANSFORM_B = 0x102520;
 static const uint32_t REG_TOUCH_TRANSFORM_C = 0x102524;
 static const uint32_t REG_TOUCH_TRANSFORM_D = 0x102528;
 static const uint32_t REG_TOUCH_TRANSFORM_E = 0x10252C;
 static const uint32_t REG_TOUCH_TRANSFORM_F = 0x102530;
 static const uint32_t REG_TOUCH_DIRECT_XY = 0x102574;
 static const uint32_t REG_TOUCH_DIRECT_Z = 0x102578;
 
 // Tracker memory (for touch tracking)
 static const uint32_t REG_TRACKER = 0x109000;
 
 // --------------------------------------
 // Special values
 // --------------------------------------
 
 static const uint32_t DLSWAP_FRAME = 0x00000002; //!< Swap display list after current frame
 
 #endif // FT800_REG_H
 