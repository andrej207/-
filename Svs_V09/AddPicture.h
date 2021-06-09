// Подгружаем шрифты
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t BigFontRus[];       //Кирилица
extern uint8_t SevenSegNumFont[];

// ---- картинка на стартовый экран
#ifdef SetPicHeater
  extern unsigned int Heater2[0x1000];
#endif
// ---- Иконки
#ifdef Set_Picture
  extern unsigned int Graf[0xCC0];
  extern unsigned int Graf2[0xCC0];
  extern unsigned int Graf3[0xCC0];
  extern unsigned int Graf4[0xCC0];
  extern unsigned int Graf5[0x12C0];
  extern unsigned int yes[0x271];
  extern unsigned int no[0x271];
#endif
