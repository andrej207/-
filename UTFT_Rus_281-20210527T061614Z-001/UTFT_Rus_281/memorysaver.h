// UTFT Memory Saver
// -----------------
//
// Since most people have only one or possibly two different display modules a lot
// of memory has been wasted to keep support for many unneeded controller chips.
// You now have the option to remove this unneeded code from the library with
// this file.
// By disabling the controllers you don't need you can reduce the memory footprint
// of the library by several Kb.
//
// Uncomment the lines for the displaycontrollers that you don't use to save
// some flash memory by not including the init code for that particular
// controller.

#define DISABLE_HX8347A 1
#define DISABLE_HX8352A 1
#define DISABLE_ILI9327 1
#define DISABLE_SSD1289 1
#define DISABLE_ILI9325C 1
#define DISABLE_ILI9325D 1
#define DISABLE_ILI9325D_ALT 1
#define DISABLE_HX8340B_8 1
#define DISABLE_HX8340B_S 1
#define DISABLE_ST7735 1
#define DISABLE_ST7735S 1
#define DISABLE_ST7735_ALT 1
#define DISABLE_ST7796S 1
#define DISABLE_S1D19122 1
#define DISABLE_PCF8833 1
#define DISABLE_SSD1963_480 1
#define DISABLE_SSD1963_800 1
#define DISABLE_OTM8009A 1
#define DISABLE_S6D1121 1
#define DISABLE_ILI9481 1 // CTE32HR
#define DISABLE_S6D0164 1
#define DISABLE_ILI9341_S4P 1
#define DISABLE_ILI9341_S5P 1
#define DISABLE_R61581 1
#define DISABLE_ILI9486 1	// ILI9486	(16bit)
//#define DISABLE_ILI9488 1
#define DISABLE_HX8357B 1 // QDTFT Module for 3.2 inch IPS 320X480
#define DISABLE_CPLD 1
#define DISABLE_HX8353C 1
