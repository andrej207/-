
// Секция начальных установок

#define SetAltPin             // Раскомментировать, если используются АЛЬТЕРНАТИВНЫЕ пины для подключения
//#define SetConnectPC        // Закоментировать, если вообще не надо связь с РС

 #define Set_Picture          // Если нужно выводить иконки
 #define SetPicHeater         // Раскомментировать если нужна картинка на стартовом экране

 #define SetEncoder           // Если используется encoder
 //#define SetAnalogBatton    // Если используется аналоговая клавиатура
 //#define SetDigitBatton     // Если используется цифровая клавиатура

 #define SetKalk              // устанавливается ввод параметров с сенсорных кнопок, если закомментировать - их не будет
 #define SetQvard 1           // установка защиты низа по умолчанию 0 - выкл, 1 - вкл

 #define HeavyBottom 1         // Если инерционный низ: 0 - выкл, 1 - вкл включаем преднагрев
 #define HeavyTemp  45        // температура до которой нагревается плата и потом включается основной профиль
 //#define HeavyPause 40        // или выдерживается пауза с подачей на низ % мощности !!! пока этот параметр не задейсвован !!!
 #define HeavyPower 30        // максимальная мощность подаваемая в НИ 
 
 // ------------------TouchScreen ----------------------------------------------
 // Должен быть раскомменчен один из #define
 // если оба закоменчены тач отключается
// #define SetTouch1            // включается тач где используются аналоговые сигналы
 #define SetTouch2         // включается тач где используется интерфейс SPI
  // ----------------------------------------------------------------------------
#if defined(SetTouch1) || defined(SetTouch2)   // если используется тач 
  #define Set_Touch 
#endif
// Секция СТАНДАРТНАЯ кнопок, пинов подключеня  --------------
#ifndef SetAltPin
  #define RelayPin1 7  //назначаем пин "ВЕРХНЕГО" нагревателя
  #define RelayPin2 6  //назначаем пин "НИЖНЕГО" нагревателя
  #define P1_PIN 9     //назначаем пин реле 1
  #define P2_PIN 10    //назначаем пин реле 2
  #define P3_PIN 11    //назначаем пин реле 3
  #define P4_PIN 12    //назначаем пин реле 4
  #define buzzerPin 8  // пин для буззера
  #define Knopki A0    // пин подключения аналоговой клавиатуры
  //назначаем пины усилителя термопары MAX6675 "ВЕРХНЕГО" нагревателя   clk=sck cs=cs do=so
  #define thermoCLK 17    //=sck
  #define thermoCS_T 18   //=cs
  #define thermoDO 19     //=so
  //назначаем пины усилителя термопары MAX6675 "НИЖНЕГО" нагревателя clk=sck cs=cs do=so
  #define thermoCLK2 14   //=sck
  #define thermoCS_B 15   //=cs
  #define thermoDO2 16    //=so 

// Секция АЛЬТЕРНАТИВНАЯ кнопок, пинов подключеня ---------------------------
#else
  #define RelayPin1 7  //назначаем пин "ВЕРХНЕГО" нагревателя
  #define RelayPin2 6  //назначаем пин "НИЖНЕГО" нагревателя
  #define P1_PIN 9     //назначаем пин реле 1
  #define P2_PIN 10    //назначаем пин реле 2
  #define P3_PIN 11    //назначаем пин реле 3
  #define P4_PIN 12    //назначаем пин реле 4
  #define buzzerPin 8  // пин для буззера
  #define Knopki A5    // пин подключения аналоговой клавиатуры
  //назначаем пины усилителя термопары MAX6675 "ВЕРХНЕГО" нагревателя   clk=sck cs=cs do=so
  #define thermoCS_T 15   //=cs
  //назначаем пины усилителя термопары MAX6675 "НИЖНЕГО" нагревателя clk=sck cs=cs do=so
  #define thermoCS_B 17   //=cs
  //
  #define thermoCLK 14    //=sck
  #define thermoDO 16     //=so
  //--------------------------------------
  #define thermoCLK2 thermoCLK
  #define thermoDO2 thermoDO
#endif
// ------------ Энкодер --------------------------------
#define BTN_OK A8                 // Кнопка энкодера 
#define Enc_Clk A10
#define Enc_DT A9
#define ROTARYSTEPS 1 
//---------Управление реле------------------------------
#define SetReleNull     // если раскомменчена то реле включаются "0", иначе "1"
  #ifdef SetReleNull
    #define SetReleON LOW
    #define SetReleOFF HIGH
   #else
    #define SetReleON HIGH 
    #define SetReleOFF LOW 
  #endif
//------------------------------------------------------
// если детектор нуля не используется - закомментировать оба варианта
// и будет использоваться программный таймер (нужна библиотека MsTimer2.h)
// скачать по ссылке https://github.com/PaulStoffregen/MsTimer2
//
// назначение пина детектора нуля ZCC
// #define SetInterrupt 0     // подключение к пину 2
// #define SetInterrupt 1     // подключение к пину 3
#ifndef SetInterrupt
  #include <MsTimer2.h>  
#endif
//------------------------------------------------------
// ожидаемые значения для псевдо-кнопок
#define SetRIGHT 144
#define SetUP 353
#define SetDOWN 711
#define SetLEFT 569
#define SetSELECT 4
//------------------------------------------------------
// корректировка показаний термопар
// вместо 0 ставим +- число
#define Corect_T 0      // термопара верхнего нагревателя
#define Corect_B 0      // термопара нижнего нагревателя
//------------------------------------------------------
