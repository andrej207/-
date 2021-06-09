// ------------ Профили ----------------------
#define numMax 10           // Максимальное количество профилей
#define ArrMax 50           // Максимальное количество символов в названии профиля (в русских буквах в два раза меньше)
#define Adres 1000          // адрес начала записи в EPROM 

bool stProf = false;
byte numProf;
struct Profili_t {         // основные поля профиля
  char HeadProf[ArrMax];   // заголовок профиля ArrMax символов
  byte TableSize;          // размер низа
  byte rampRateBOTTOM;     // скорсть нагрева низа
  byte temperatureBOTTOM;  // температура низа в градусах
  byte dwellTimerBOTTOM;   // время в сек перехода к включению верха
  byte min_pwr_BOTTOM;     // мощность низа в % min
  byte max_pwr_BOTTOM;     //     --            max
  byte rampRateStep[4];    // скорсть нагрева верхом по шагам
  byte temperatureStep[4]; // температура верха по шагам
  byte dwellTimerStep[4];  // время в сек перехода на следующий шаг
  byte min_pwr_TOPStep[4]; // min мощности по шагам
  byte max_pwr_TOPStep[4]; // max мощности по шагам
  byte kp1;                // коэф ПИД 1 верха
  byte ki1;
  byte kd1;
  byte kp2;                // коэф ПИД 2 низа
  byte ki2;
  byte kd2;
};
int SizePrrof = sizeof(Profili_t);
union {
  byte Mode[sizeof(Profili_t)];
  Profili_t Profili;
} u;
// --------- profile end ------------------------------
byte Cgr, CTgr = 0;
int Xgr, XTgr = 2;                 // x координата графика
int er1 = 1, er2 = 1;
int reg1, reg2;
int pwr1, pwr2;
double prev_millis = 0;
boolean out1, out2;
boolean TopStart = false, BottomStart = false, HeavyStart = false, HeavyText = false;
boolean FlTone = false;
String DuStr = "";
byte currentProfile;
byte currentStep, BrStep;
byte profileName;
double counter;
long previousMillis;          // это для счетчиков
char buffer[ArrMax];          // 
// ----------------------------------------------------
 extern byte ProfDate[10][33];
 extern char* AdrProfN[10];
// -------секция ввода/вывода для ПЭВМ-----------------
char buf[20];   //буфер вывода сообщений через сом порт

typedef enum REFLOW_STATE {   // состояния контроллера
  REFLOW_STATE_IDLE,          // главный экран
  REFLOW_STATE_SETTING,       // меню настроек
  REFLOW_STATE_TIME_EDIT,     // установка часов
  REFLOW_STATE_SET_EDIT,      // редактирование выбранной настройки
  REFLOW_STATE_PROFILE_LOAD,  // загрузка профилей в eeprom 
  REFLOW_STATE_PROFILE,       // просмотр профиля
  REFLOW_STATE_BRAZE_START,   // Старт пайки
  REFLOW_STATE_BRAZE_NOW,     // отработка профиля
  REFLOW_STATE_BRAZE_STOP,    // Стоп пайки
}
reflowState_t;
reflowState_t reflowState;          // Reflow oven controller state machine state variable

#define SENSOR_SAMPLING_TIME 250    //read tc every second
#define i_min 0.0//минимум И составляющей
#define i_max 100.0//максимум И составляющей

unsigned long nextRead1; //переменная для обновления текущей температуры
unsigned long WinStartTime, BrStartTime, BotStartTime, TouchSet;
byte Setpoint1, Setpoint2, bottomMaxPwr;
byte Output1, Output2, TouOld = 10;
float Input_f1, Input_f2, Input_ft1, Input_ft2;
float integra, integra2, BrTg;
float e1, p1, d1, e2, p2, d2;
double Input1,  Setpoint1d, T_mlx;
double Input2,  Setpoint2d;
double bottomTemp;
int SP1 = Setpoint1, SP2 = Setpoint2, Count1, Count2;
int tc1, tc2;                       // переменные для калмана
int i, j, w;
int chast, kluch;
int tmp, DisXSize, DisYSize;

boolean alarmOn = false;
boolean updateScreen = true;        //Update whole screen boolean
// --------------------
#define VGA_ORAN 0xFD40
#define Draw_X 90
#define Draw_Y 80
#define Simb_X 18*16
#define Win1 4                                // всего вкладок меню
#define Win2 6                                // кол-во строк
#define K_line 12
#define Py_pr 31 //40
#define Yset 25                               // отступ сверху                          
#define Ysize 36                              // высота окна меню
#define Ywin0 Ysize+Yset+6                    // начало окна параметров
#define Ywin1 Ysize+Yset
#define X_graf 27
#define Y_graf Py_pr+246
#define T_SetY 293
#define B_SetX 3
#define B_SetY 140
#define BattX 87          // размер кнопки по X 
#define BattY 60          // размер кнопки по Y
#define TempY 30
#define TimeSt_M 0
#define TempSt_M 1
#define TimeEnd_M 2
#define TempEnd_M 3
#define Batt_X 60         // размер сенсорных цифровых кнопок 
#define Batt_Y 50  
#define Mkk 109

int BattSet[5][4] = { {B_SetX, B_SetY + 10 + BattY, B_SetX + BattX, B_SetY + 10 + 2 * BattY},
  {B_SetX, B_SetY, B_SetX + BattX, B_SetY + BattY},
  {479 - B_SetX - BattX, B_SetY, 479 - B_SetX, B_SetY + BattY},
  {479 - B_SetX - BattX, B_SetY + 10 + BattY, 479 - B_SetX, B_SetY + 10 + 2 * BattY},
  {B_SetX, B_SetY + 20 + 2 * BattY, 479 - B_SetX, 319}
};

byte  Edit_Byte_old, Edit_old, BrStepMax, ByteMax;
byte Obsi1 = 1, Obsi2 = 2, Obsi3 = 3, Obsi4 = SetQvard, Obsi5 = 5, Obsi6 = 6;
byte Obsi2_Old;
int Xsize;                                    // ширина окна меню
int Y_S_win;                                  // высота строки
int Edit_Byte, Y_shift;
int8_t Vklad_G = 0, Vklad_G_old = 0, Vklad_G_Mem, Vklad_L_Mem;          // выбранная вкладка (новая, старая)
int8_t Vklad_L = 0, Vklad_L_old = 0;          // выбранная строка  (новая, старая)
int8_t Shag = 0, Shag_old = 0;
bool F_Alarm, TouDec = false;
