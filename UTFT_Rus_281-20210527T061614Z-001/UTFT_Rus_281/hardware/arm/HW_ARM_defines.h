// CTE TFT LCD/SD Shield for Arduino Due
// -------------------------------------
// Uncomment the following line if you are using this shield
//#define CTE_DUE_SHIELD 1
//
// For this shield: RS=25, WR=26, CS=27, RST=28
//********************************************************************

// ElecHouse TFT LCD/SD Shield for Arduino Due
// -------------------------------------
// Uncomment the following line if you are using this shield
//#define EHOUSE_DUE_SHIELD 1
//
// For this shield: RS=22, WR=23, CS=31, RST=33
//********************************************************************
// #define USE_UNO_SHIELD_ON_MEGA 1
// *** Hardwarespecific defines ***
#if defined(ENERGIA)
	#define cbi(reg, bitmask) HWREG((uint32_t)reg + 0x3FC) &= ~bitmask
	#define sbi(reg, bitmask) HWREG((uint32_t)reg + 0x3FC) |= bitmask
#else
	#define cbi(reg, bitmask) *reg &= ~bitmask
	#define sbi(reg, bitmask) *reg |= bitmask
#endif
#define pulse_high(reg, bitmask) sbi(reg, bitmask); cbi(reg, bitmask);
#define pulse_low(reg, bitmask) cbi(reg, bitmask); sbi(reg, bitmask);

#define cport(port, data) port &= data
#define sport(port, data) port |= data

#define swap(type, i, j) {type t = i; i = j; j = t;}

#define fontbyte(x) cfont.font[x]  

#if defined(__SAM3X8E__) || defined(__SAM3A8C__)
	#define pgm_read_word(addr) (*(const unsigned short *)(addr))
	#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
	#define bitmapdatatype unsigned int*
#else
	#define pgm_read_word(data) *data
	#define pgm_read_byte(data) *data
	#define bitmapdatatype unsigned short*
#endif

//#define bitmapdatatype unsigned char*	

#if defined(TEENSYDUINO) && TEENSYDUINO >= 117
  #define regtype volatile uint8_t
  #define regsize uint8_t
#else
  #define regtype volatile uint32_t
  #define regsize uint32_t
#endif
