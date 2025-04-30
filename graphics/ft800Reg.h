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
 
 static const uint32_t REG_ID             = 0x102400;
 static const uint32_t REG_FRAMES         = 0x102404;
 static const uint32_t REG_CLOCK          = 0x102408;
 static const uint32_t REG_FREQUENCY      = 0x10240C;
 static const uint32_t REG_RENDERMODE     = 0x102410;
 static const uint32_t REG_SNAPY          = 0x102414;
 static const uint32_t REG_SNAPSHOT       = 0x102418;
 static const uint32_t REG_CPURESET       = 0x10241C;
 static const uint32_t REG_TAP_CRC        = 0x102420;
 static const uint32_t REG_TAP_MASK       = 0x102424;
 static const uint32_t REG_HCYCLE         = 0x102428;
 static const uint32_t REG_HOFFSET        = 0x10242C;
 static const uint32_t REG_HSIZE          = 0x102430;
 static const uint32_t REG_HSYNC0         = 0x102434;
 static const uint32_t REG_HSYNC1         = 0x102438;
 static const uint32_t REG_VCYCLE         = 0x10243C;
 static const uint32_t REG_VOFFSET        = 0x102440;
 static const uint32_t REG_VSIZE          = 0x102444;
 static const uint32_t REG_VSYNC0         = 0x102448;
 static const uint32_t REG_VSYNC1         = 0x10244C;
 static const uint32_t REG_DLSWAP         = 0x102450;
 static const uint32_t REG_ROTATE         = 0x102454;
 static const uint32_t REG_OUTBITS        = 0x102458;
 static const uint32_t REG_DITHER         = 0x10245C;
 static const uint32_t REG_SWIZZLE        = 0x102460;
 static const uint32_t REG_CSPREAD        = 0x102464;
 static const uint32_t REG_PCLK_POL       = 0x102468;
 static const uint32_t REG_PCLK           = 0x10246C;
 static const uint32_t REG_TAG_X          = 0x102470;
 static const uint32_t REG_TAG_Y          = 0x102474;
 static const uint32_t REG_TAG            = 0x102478;
 static const uint32_t REG_VOL_PB         = 0x10247C;
 static const uint32_t REG_VOL_SOUND      = 0x102480;
 static const uint32_t REG_SOUND          = 0x102484;
 static const uint32_t REG_PLAY           = 0x102488;
 static const uint32_t REG_GPIO_DIR       = 0x10248C;
 static const uint32_t REG_GPIO           = 0x102490;
 static const uint32_t REG_INT_FLAGS      = 0x102498;
 static const uint32_t REG_INT_EN         = 0x10249C;
 static const uint32_t REG_INT_MASK       = 0x1024A0;
 
 // Sound playback
 static const uint32_t REG_PLAYBACK_START   = 0x1024A4;
 static const uint32_t REG_PLAYBACK_LENGTH  = 0x1024A8;
 static const uint32_t REG_PLAYBACK_READPTR = 0x1024AC;
 static const uint32_t REG_PLAYBACK_FREQ    = 0x1024B0;
 static const uint32_t REG_PLAYBACK_FORMAT  = 0x1024B4;
 static const uint32_t REG_PLAYBACK_LOOP    = 0x1024B8;
 static const uint32_t REG_PLAYBACK_PLAY    = 0x1024BC;
 
 // PWM control
 static const uint32_t REG_PWM_HZ         = 0x1024C0;
 static const uint32_t REG_PWM_DUTY       = 0x1024C4;
 
 // Macros and Co-Processor
 static const uint32_t REG_MACRO_0        = 0x1024C8;
 static const uint32_t REG_MACRO_1        = 0x1024CC;
 static const uint32_t REG_CMD_READ       = 0x1024E4;
 static const uint32_t REG_CMD_WRITE      = 0x1024E8;
 static const uint32_t REG_CMD_DL         = 0x1024EC;
 
 // Touchscreen settings
 static const uint32_t REG_TOUCH_MODE         = 0x1024F0;
 static const uint32_t REG_TOUCH_ADC_MODE     = 0x1024F4;
 static const uint32_t REG_TOUCH_CHARGE       = 0x1024F8;
 static const uint32_t REG_TOUCH_SETTLE       = 0x1024FC;
 static const uint32_t REG_TOUCH_OVERSAMPLE   = 0x102500;
 static const uint32_t REG_TOUCH_RZTHRESH     = 0x102504;
 static const uint32_t REG_TOUCH_RAW_XY       = 0x102508;
 static const uint32_t REG_TOUCH_RZ           = 0x10250C;
 static const uint32_t REG_TOUCH_SCREEN_XY    = 0x102510;
 static const uint32_t REG_TOUCH_TAG_XY       = 0x102514;
 static const uint32_t REG_TOUCH_TAG          = 0x102518;
 static const uint32_t REG_TOUCH_TRANSFORM_A  = 0x10251C;
 static const uint32_t REG_TOUCH_TRANSFORM_B  = 0x102520;
 static const uint32_t REG_TOUCH_TRANSFORM_C  = 0x102524;
 static const uint32_t REG_TOUCH_TRANSFORM_D  = 0x102528;
 static const uint32_t REG_TOUCH_TRANSFORM_E  = 0x10252C;
 static const uint32_t REG_TOUCH_TRANSFORM_F  = 0x102530;
 static const uint32_t REG_TOUCH_DIRECT_XY    = 0x102574;
 static const uint32_t REG_TOUCH_DIRECT_Z     = 0x102578;
 
 // Touch tracker
 static const uint32_t REG_TRACKER = 0x109000;
 
 // --------------------------------------
 // Special values
 // --------------------------------------
 
 static const uint32_t DLSWAP_FRAME = 0x00000002; //!< Swap DL at frame end
 
 #endif // FT800_REG_H
 