// -------------------------------
#if defined(SetTouch1) || defined(SetTouch2)   // если используется тач 
  #define Set_Touch 
#endif
// -------------------------------
#ifdef SetTouch2
  #include <UTouch.h>
  // URTouch      myTouch(48, 50, 51, 46, 2);    // это мой дисплей для дуе
  UTouch myTouch( 52, 53, 51, 50, 45);  // для дисплея ST7796S
    int xpok, ypok;         //screen coordinates
#endif
// -------------------------------
#ifdef SetTouch1                  // Тач аналоговый
  #include <TouchScreen.h> 
  uint8_t YP = A0;   // must be an analog pin, use "An" notation!
  uint8_t XM = A1;   // must be an analog pin, use "An" notation!
  uint8_t YM = A2;   // can be a digital pin
  uint8_t XP = A3;   // can be a digital pin
  uint8_t SwapXY = 0;
  #define MINPRESSURE 20
  #define MAXPRESSURE 1000
  #define SWAP(a, b) {uint16_t tmp = a; a = b; b = tmp;}
 
  TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
  TSPoint tp;
    
  uint16_t TS_LEFT = 904;
  uint16_t TS_RT  = 170;
  uint16_t TS_TOP = 950;
  uint16_t TS_BOT = 158;
  int xpok, ypok;         //screen coordinates
  uint8_t Orientation = 3;     //PORTRAIT

void SetPinTouch1() {  
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  pinMode(XP, OUTPUT);
  pinMode(YM, OUTPUT);
}
#endif
