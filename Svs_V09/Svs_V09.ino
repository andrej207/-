//Релиз от 22,05,2020    Ver.09
#include "Setting.h"
// Используемые библиотеки -----------------------------
#include <UTFT.h>
#include <AnalogKey5.h>
#include <stdint.h>
#include <RotaryEncoder.h>
#include "Cl_do_btn_long.h"
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <EEPROM.h>

//тест гитхаба
UTFT myGLCD(ILI9488, 38, 39, 40, 41);

#include "AddPicture.h"
//-------- кнопки ----------------------------
#include "Batton1.h"
// -------- TouchScreen ----------------------
#include "Touch.h"
// -------- энкодер --------------------------
#include "Encoder.h"
//--------- Temp -----------------------------
#include "Temp6675.h"
//-----------Peremen.h------------------------
#include "Peremen.h"
//--------------------------------------------
#include "Gvard.h"
//--------------------------------------------

void *Wr_mem, *Wr_pr;
char *Ad_tekct;
// -----------------
tmElements_t tm;
uint8_t hh, mm, ss;
uint8_t Shms[4], Stat;
bool F_timK = false;
//
boolean initial = 1;
boolean RetGr;
float sx, sy, mx, my, hx, hy;
float sdeg, mdeg, hdeg, Fconst = 0.0174532925;
uint16_t osx, osy, omx, omy, ohx, ohy;
int16_t x0, x1, yy0, yy1;
uint16_t xpos; // x
uint16_t ypos; // y
bool Clock = false, Gt_Set = false, Svkl_L = false;  // Gt_Set=true;
static byte OldSek;

int Pr_Line_M[10][4];
// ------------------------------ 
void Pr_WinTime() {   // -------------для замера времени ------------
  //Serial.print("time - ");
  //Serial.println(micros() - WinStartTime);
}
// ------------------
void ChangeState(reflowState_t ChSt, bool UpScr) {
   reflowState = ChSt;
   updateScreen = UpScr;  
}
// ------------------
void Pr_Line_Load(int i_i, int X_St, int Y_St, int X_En, int Y_En) {
  Pr_Line_M[i_i][0] =  X_St;
  Pr_Line_M[i_i][1] =  Y_St;
  Pr_Line_M[i_i][2] =  X_En;
  Pr_Line_M[i_i][3] =  Y_En;
}
// -----------------
double M_tg_Line(int i_i) {
  double Tg_ugla;
  Tg_ugla = 0;
  if (Pr_Line_M[i_i][2] != Pr_Line_M[i_i][0]) {
    Tg_ugla = Pr_Line_M[i_i][3] - Pr_Line_M[i_i][1];
    Tg_ugla = Tg_ugla / (Pr_Line_M[i_i][2] - Pr_Line_M[i_i][0]);
  }
  // Serial.print("Tg - "); Serial.println(Tg_ugla);
  return (Tg_ugla);
}
// -----------------
void M_ln_point (int X_tec, int XxSt, int YySt, int XxEn, int YyEn) {  // вывод линии попиксельно
  float Tg_ugla;
  if (X_tec - X_graf <= XxSt + 1 || X_tec - X_graf > XxEn + 1) return;
  // Serial.print(XxSt);  Serial.print("  ");  Serial.print(YySt);  Serial.print("  ");
  // Serial.print(XxEn);  Serial.print("  ");  Serial.println(YyEn);
  if (YySt == YyEn) {
    myGLCD.drawPixel(X_tec - 1, Y_graf - YySt);
    return;
  }
  float B_1 = XxEn - XxSt;
  float A_1 = YyEn - YySt;
  if (B_1 != 0) Tg_ugla = A_1 / B_1;
  else Tg_ugla = 0;                 // 30, 40, 210, 145
  int tt = Y_graf - (YySt + (X_tec - X_graf - XxSt) * Tg_ugla);
  // Serial.print(Tg_ugla,6);  Serial.print("  ");  Serial.println(tt);
  myGLCD.drawPixel(X_tec - 1, tt + 1);
  return;
}
// --------------------------------------------
void M_dr_line (int XxSt, int YySt, int XxEn, int YyEn) {   // рисуем линию на графике
  myGLCD.drawLine(X_graf + XxSt, Y_graf - YySt, X_graf + XxEn, Y_graf - YyEn);
  return;
}
// --------------------------------------------
void M_dr_point (int XxSt, int YySt) {                      // рисуем точку на графике
  myGLCD.drawPixel(X_graf + XxSt, Y_graf - YySt);
  return;
}
// --------------------------------------------
void M_dr_int (int IiInt, int XxSt, int YySt) {             // пишем число на графике
  myGLCD.printNumI(IiInt, X_graf + XxSt, Y_graf - YySt, 3);
  return;
}
//---------------------------------------------
void ButRight() {   //    "Button - RIGHT"
  if (reflowState == REFLOW_STATE_IDLE) {
    ChangeState(REFLOW_STATE_PROFILE, 1);
    return;
  }
  if (reflowState == REFLOW_STATE_PROFILE) {
    ChangeState(REFLOW_STATE_SETTING,1);
    RetGr = true;
    return;
  }
  if (reflowState == REFLOW_STATE_SETTING) {
    if (Vklad_G == 1 && Shag < 3) {
      Shag += 1;
      return;
    }
    Shag = 0;  Shag_old = 0;
    Vklad_G += 1;
    if (Vklad_G >= Win1) Vklad_G = 0;
    return;
  }
  if (reflowState == REFLOW_STATE_TIME_EDIT) {
    F_timK = true;
    if (Stat < 3) Stat += 1;
      else Stat = 0;
    return;  
   }
  if (reflowState == REFLOW_STATE_PROFILE_LOAD) {
    if (Stat < 2) Stat += 1;
      else Stat = 1;
    Obsi2_Old = 15;  
    return;
  }
}
//---------------------------------------------
void ButUp()    {    //    "Button - UP"
  if (reflowState == REFLOW_STATE_IDLE) {
    currentProfile = currentProfile + 1;
    if (currentProfile > numMax)  currentProfile = 1;
    loadProfile(currentProfile);
    VievTemp();
    PrPrint(B_SetX + 2, T_SetY);
    return;
  }
  if (reflowState == REFLOW_STATE_SETTING) {
    Vklad_L -= 1;
    if  (Vklad_G == 1) {
      if (Vklad_L < 1) Vklad_L = Win2 - 1;
      Shag_old = 9;
    }
    else if (Vklad_L < 0) Vklad_L = Win2 - 1;
    return;
  }
  if (reflowState == REFLOW_STATE_TIME_EDIT) {
    F_timK = true;
    if (Stat == 0) {
      if (Shms[Stat] < 23) Shms[Stat] +=1;
       else Shms[Stat] = 0;
      return;
      }
    if (Shms[Stat] < 59) Shms[Stat] +=1;
       else Shms[Stat] = 0;
      return;
   }
  if (reflowState == REFLOW_STATE_SET_EDIT) {
    ByteMax = 255;
    if (Vklad_G==0 || Vklad_G==1) {
      if (Vklad_G==0 && Vklad_L==0) ByteMax = 3;
      if (Vklad_L==1) ByteMax = 30;
      if (Vklad_L==4 || Vklad_L==5) ByteMax = 99;
      } 
     if (Vklad_G==3 && Vklad_L==3)  ByteMax = 1;    
      if (ByteMax == 3 && Edit_Byte == ByteMax) {
       Edit_Byte = 1;
       return;
       }
      if (Edit_Byte < ByteMax) Edit_Byte += 1;
        else Edit_Byte = 0;  
      return;     
  }
  if (reflowState == REFLOW_STATE_PROFILE) {
    currentProfile = currentProfile + 1;
    if (currentProfile > numMax)  currentProfile = 1;
    loadProfile(currentProfile);
    updateScreen = true;
    return;
  }
  if (reflowState == REFLOW_STATE_PROFILE_LOAD) {
    if(Stat == 1) { 
      if (Obsi2 < numMax) Obsi2 +=1;
        else Obsi2=0;
    }
    return;
    }
}
//---------------------------------------------
void ButDown()  {    //    "Button - DOWN"
  if (reflowState == REFLOW_STATE_IDLE) {
    currentProfile = currentProfile - 1;
    if (currentProfile < 1) currentProfile = numMax;
    loadProfile(currentProfile);
    VievTemp();
    PrPrint(B_SetX + 2, T_SetY);
    return;
  }
  if (reflowState == REFLOW_STATE_SETTING) {
    Vklad_L += 1;
    if (Vklad_G == 1) {
      if (Vklad_L >= Win2) Vklad_L = 1;
      Shag_old = 8;
    }
    else if (Vklad_L >= Win2) Vklad_L = 0;
   return;
  }
  if (reflowState == REFLOW_STATE_TIME_EDIT) {
    F_timK = true;
    if (Stat == 0) {
      if (Shms[Stat] == 0) Shms[Stat] = 23;
       else Shms[Stat] -= 1;
      return;
      }
    if (Shms[Stat] == 0) Shms[Stat] = 59;
       else Shms[Stat] -= 1;
      return;
   }
  if (reflowState == REFLOW_STATE_SET_EDIT) {
    ByteMax = 255;
    if (Vklad_G==0 || Vklad_G==1) {
      if (Vklad_G==0 && Vklad_L==0) ByteMax = 3;
      if (Vklad_L==1) ByteMax = 30;
      if (Vklad_L==4 || Vklad_L==5) ByteMax = 99;
      }     
    if (Vklad_G==3 && Vklad_L==3)  ByteMax = 1; 
    if (ByteMax == 3 && Edit_Byte == 1) {
       Edit_Byte = ByteMax;
       return;
       }
    if (Edit_Byte != 0) Edit_Byte -= 1;
      else Edit_Byte = ByteMax;
    return;
   
  }  
  if (reflowState == REFLOW_STATE_PROFILE) {
    currentProfile = currentProfile - 1;
    if (currentProfile < 1) currentProfile = numMax;
    loadProfile(currentProfile);
    updateScreen = true;
    return;
  }
  if (reflowState == REFLOW_STATE_PROFILE_LOAD) {
    if (Stat == 1) {
      if(Obsi2 == 0) Obsi2 = numMax;
        else Obsi2 -=1;
      }
    return;
    }

}
//---------------------------------------------
void ButLeft()  {    //    "Button - LEFT"
  if (reflowState == REFLOW_STATE_SETTING) {
    if (Vklad_G == 1 && Shag > 0) {
      if (Shag > 0)  Shag -= 1;
      return;
    }
    Vklad_G -= 1;
    if (Vklad_G < 0) {
      Vklad_G = 0;
      SaveProfile(currentProfile);
      if (RetGr == true) ChangeState(REFLOW_STATE_PROFILE,1);
        else ChangeState(REFLOW_STATE_IDLE,1);
      }
    return;
  }
  if (reflowState == REFLOW_STATE_TIME_EDIT) {
    F_timK = true;
    if (Stat == 0) {
      ChangeState(REFLOW_STATE_SETTING,0);
      Vklad_L_old = 10;
      myGLCD.setColor(VGA_BLACK);
      myGLCD.fillRoundRect(10, 105, 468, 177);
      return;
      }
      else Stat -= 1;
      return;
   }
  if (reflowState == REFLOW_STATE_IDLE) {
    ChangeState(REFLOW_STATE_SETTING,1);
    RetGr = false;
    return;
  }
  if (reflowState == REFLOW_STATE_PROFILE) {
    ChangeState(REFLOW_STATE_IDLE,1);
    RetGr = false;
    return;
  }
  if (reflowState == REFLOW_STATE_BRAZE_NOW) {
    ChangeState(REFLOW_STATE_BRAZE_STOP,1);
    return;
  }
  if (reflowState == REFLOW_STATE_BRAZE_STOP) {
    ChangeState(REFLOW_STATE_IDLE,1);
    return;
  }
  if (reflowState == REFLOW_STATE_PROFILE_LOAD) {
    if (Stat == 1) {
      ChangeState(REFLOW_STATE_SETTING,0);
      Vklad_L_old = 10;
      WinRamka (9, 105, 465, 209, VGA_BLACK, 1);
      myGLCD.fillRoundRect(320, Y_shift, 470, Y_shift+16);
      return;
    }
      else Stat -= 1;
     Obsi2_Old = 15;
  }
}
//---------------------------------------------
void ButOk()    {    //    "Button - OK"
  if (reflowState == REFLOW_STATE_IDLE) {
    ChangeState(REFLOW_STATE_BRAZE_START,1);
    return;
    }
  if (reflowState == REFLOW_STATE_SETTING) {   // переходим в режим редактирования
    if (Vklad_G == 3) {
      if (Vklad_L == 1) {
        ChangeState(REFLOW_STATE_PROFILE_LOAD,1);
        return;
        }
      if (Vklad_L == 0) {
        ChangeState(REFLOW_STATE_TIME_EDIT,1);
        return;
        }
      }
    ChangeState(REFLOW_STATE_SET_EDIT,1);
    return;
  }
  if (reflowState == REFLOW_STATE_TIME_EDIT) { 
    if (Stat == 3) {
      tm.Hour = Shms[0];
      tm.Minute = Shms[1];
      tm.Second = Shms[2];
      RTC.write(tm);    // Записываем время в модуль:
      initial = true;
      ChangeState(REFLOW_STATE_SETTING,0);
      Vklad_L_old = 10;
      myGLCD.setColor(VGA_BLACK);
      myGLCD.fillRoundRect(10, 105, 468, 177);
      return;
      }
    }
  if (reflowState == REFLOW_STATE_SET_EDIT) {  // возврат в меню настроек после редактирования
    *(byte*)Wr_mem = Edit_Byte;
    Vklad_L_old = 10;
    if (Vklad_G == 1) Shag_old = 8;
    ChangeState(REFLOW_STATE_SETTING,0);
    WinRamka (9, 105, 465, 209, VGA_BLACK, 1); // 355, 240, 113, 69, VGA_BLACK, 1
    return;
  }
  if (reflowState == REFLOW_STATE_PROFILE) {
    if (Gt_Set == false) Gt_Set = true;
    else Gt_Set = false;
  }
  if (reflowState == REFLOW_STATE_PROFILE_LOAD) {
    if (Stat == 2) {
      if (Obsi2 == 0) {
        for (i=1; i<=numMax; i++) {
          WrLoadPr(i, i);
          }
        currentProfile = 1;
        Obsi2 = 1;
        PrPrint(0, 2);
        }
       else { 
         WrLoadPr(currentProfile, Obsi2);         
         PrPrint(0, 2);
         }
      currentProfile = Obsi2; 
      loadProfile(currentProfile);  
      ChangeState(REFLOW_STATE_SETTING,0);
      Vklad_L_old = 10;
      WinRamka (9, 105, 465, 209, VGA_BLACK, 1);
      myGLCD.fillRoundRect(320, Y_shift, 470, Y_shift+16);
      }
    return;
   }
}
//---------------------------------------------
#ifdef SetAnalogBatton
  void Kn_work() {                    //  обработка нажатия кнопок
    if (AKeys.Key1() && !Key1_Old)  { //    "Button - RIGHT"
      Key1_Old = AKeys.Key1();         // отработка нажатия кнопки
      ButRight();
      //   Serial.println("Button - RIGHT");
      return;
    }
    if (AKeys.Key2() && !Key2_Old)  { //    "Button - UP"
      Key2_Old = AKeys.Key2();         // отработка нажатия кнопки
      // Serial.println("Button - UP");
      ButUp();
      return;
    }
    if (AKeys.Key3() && !Key3_Old)  { //    "Button - DOWN"
      Key3_Old = AKeys.Key3();        // отработка нажатия кнопки
      //   Serial.println("Button - DOWN");
      ButDown();
      return;
    }
    if (AKeys.Key4() && !Key4_Old)  { //    "Button - LEFT"
      Key4_Old = AKeys.Key4();           // отработка нажатия кнопки
      //   Serial.println("Button - LEFT");
      ButLeft();
      return;
    }
    if (AKeys.Key5() && !Key5_Old)  { //    "Button - OK"
      Key5_Old = AKeys.Key5();         // отработка нажатия кнопки
      //   Serial.println("Button - OK");
      ButOk();
      return;
    }
    return;
  }
#endif 
// --------------------------------------------
void TempRead() {                   //  чтение температуры
  if (Input1 == 0) Input1 = ReadCelsius(thermoCLK, thermoCS_T, thermoDO);
    else Input1 = Input1 * 0.8 + 0.2 * (ReadCelsius(thermoCLK, thermoCS_T, thermoDO) + Corect_T);
  if (Input2 ==0) Input2 = ReadCelsius(thermoCLK2, thermoCS_B, thermoDO2);
    else Input2 = Input2 * 0.8 + 0.2 * (ReadCelsius(thermoCLK2, thermoCS_B, thermoDO2) + Corect_B);
  tc1 = Input1;
  tc2 = Input2;
}
// --------------------------------------------
void VievTempGl() {
  if (Input1<0) {
    myGLCD.setFont(BigFont);
    myGLCD.setColor(VGA_RED);
    myGLCD.print(F("ERROR"), 12, 37);
  }
  else {
    myGLCD.setFont(SevenSegNumFont);
    myGLCD.setColor(VGA_SILVER);
    myGLCD.printNumI(tc1, 5, 20, 3, '0');
  }
  if (Input2<0) {
    myGLCD.setFont(BigFont);
    myGLCD.setColor(VGA_RED);
    myGLCD.print(F("ERROR"), 388, 37);
  }
  else {
    myGLCD.setFont(SevenSegNumFont);
    myGLCD.setColor(VGA_SILVER);
    myGLCD.printNumI(tc2, 380, 20, 3, '0');
  }
}
//---------------------------------------------
void ProfEdit_2() {                 //  начало экрана настроек
  Vklad_G = 0;
  Vklad_L = 0;
  myGLCD.clrScr();
  PrPrint(0, 2);
  Vklad_G_old = 15;
  Vklad_L_old = 15;
  Svkl_L = false;
  K_Kpres_ok = false;
  NewPos = 0; LastPos = 0;
  encoder.setPosition(0);
  updateScreen = false;
}
// --------------------------------------------
void Dimming() {
  OutPWR_TOP();
  OutPWR_BOTTOM();
}
// --------------------------------------------
void OutPWR_TOP() {
  reg1 = round(Output1) + er1; //pwr- задание выходной мощности в %,в текущем шаге профиля, er- ошибка округления
  if (reg1 < 50) {
    out1 = LOW;
    er1 = reg1; // reg- переменная для расчетов
  }
  else {
    out1 = HIGH;
    er1 = reg1 - 100;
  }
  digitalWrite(RelayPin1, out1); //пин через который осуществляется дискретное управление
}
//---------------------------------------------
void OutPWR_BOTTOM() {
  reg2 = round(Output2) + er2; //pwr- задание выходной мощности в %, er- ошибка округления
  if (reg2 < 50) {
    out2 = LOW;
    er2 = reg2; // reg- переменная для расчетов
  }
  else {
    out2 = HIGH;
    er2 = reg2 - 100;
  }
  digitalWrite(RelayPin2, out2); //пин через который осуществляется дискретное управление
}
//---------------------------------------------
byte Pid1(double temp, double ust, byte kP, byte kI, byte kd)   { // верх
  byte out = 0;
  static float ed = 0;
  e1 = (ust - temp); //ошибка регулирования
  p1 =  (kP * e1); //П составляющая
  integra = (integra < i_min) ? i_min : (integra > i_max) ? i_max : integra + (kI * e1) / 1000.0; //И составляющая
  d1 = kd * (e1 - ed); //Д составляющая
  ed = e1;
  out = (p1 + integra + d1 < u.Profili.min_pwr_TOPStep[currentStep-1])
        ? u.Profili.min_pwr_TOPStep[currentStep-1]
        : (p1 + integra + d1 > u.Profili.max_pwr_TOPStep[currentStep-1])
        ? u.Profili.max_pwr_TOPStep[currentStep-1] : p1 + integra + d1;
  return out;
}
//---------------------------------------------
byte Pid2(double temp, double ust, byte kP, byte kI, byte kd)   { // низ 
  byte out = 0;
  static float ed = 0;
  e2 = (ust - temp);   //ошибка регулирования
  p2 =  (kP * e2);     //П составляющая
  integra2 = (integra2 < i_min) ? i_min : (integra2 > i_max) ? i_max : integra2 + (kI * e2) / 1000.0; //И составляющая
  d2 = kd * (e2 - ed); //Д составляющая
  ed = e2;
  out = (p2 + integra2 + d2 < u.Profili.min_pwr_BOTTOM) ? u.Profili.min_pwr_BOTTOM : (p2 + integra2 + d2 > bottomMaxPwr) ? bottomMaxPwr : p2 + integra2 + d2;
  return out;
}
// --------------------------------------------
void Ramka(int i, word color) {
  myGLCD.setColor(color);
  myGLCD.drawRoundRect(BattSet[i][0], BattSet[i][1], BattSet[i][2], BattSet[i][3]);
  myGLCD.drawRoundRect(BattSet[i][0] + 1, BattSet[i][1] + 1, BattSet[i][2] - 1, BattSet[i][3] - 1);
}
// ---------------------------------------------------
void Zegar() {                      // рисуем циферблат

  myGLCD.setColor(VGA_LIME);
  myGLCD.drawCircle(xpos, ypos, ypos - 1);
  myGLCD.drawCircle(xpos, ypos, ypos - 2);
  myGLCD.setColor(VGA_RED);
  myGLCD.drawCircle(xpos, ypos, ypos - 5);
  myGLCD.drawCircle(xpos, ypos, ypos - 6);
  for (int i = 0; i < 360; i += 30) {       // Draw 12 lines
    sx = cos((i - 90) * Fconst);
    sy = sin((i - 90) * Fconst);
    x0 = sx * (ypos - 6) + xpos;
    yy0 = sy * (ypos - 6) + ypos;
    x1 = sx * (ypos - 22) + xpos;
    yy1 = sy * (ypos - 22) + ypos;
    myGLCD.drawLine(x0, yy0, x1, yy1);
  }
  myGLCD.setColor(VGA_WHITE);
  for (int i = 0; i < 360; i += 6) {        // Draw 60 dots
    sx = cos((i - 90) * Fconst);
    sy = sin((i - 90) * Fconst);
    x0 = sx * (ypos - 22) + xpos;
    yy0 = sy * (ypos - 22) + ypos;
    // Draw minute markers
    myGLCD.drawPixel(x0, yy0);
    // Draw main quadrant dots
    if (i == 0 || i == 90 || i == 180 || i == 270) myGLCD.fillCircle(x0, yy0, 4);
  }
  myGLCD.fillCircle(xpos, ypos, 7);

}
// --------------------------------------------
void VievTemp() {                   // вывод температуры профиля на главный экран
  myGLCD.setFont(BigFont);
  myGLCD.setColor(VGA_SILVER);
  myGLCD.printNumF(u.Profili.rampRateBOTTOM * 0.1, 0, 385, 75);
  myGLCD.printNumF(u.Profili.rampRateStep[0] * 0.1, 0, 0, 75);
  j = 0;
  for (i = 0; i < 4; i++) {
    if (u.Profili.temperatureStep[i] > j) j = u.Profili.temperatureStep[i];
  }
  myGLCD.printNumI(j, 0, 95, 3, '0');
  myGLCD.printNumI(u.Profili.temperatureBOTTOM, 410, 95, 3, '0');

}
// --------------------------------------------
void GlabPrint() {                  // главный экран
  Clock = true;
  myGLCD.clrScr();
  Zegar();
  myGLCD.setFont(BigFontRus);
  myGLCD.setColor(VGA_LIME);
  myGLCD.textRus(F("ВЕРХ "), 22, 2);
  myGLCD.textRus(F("НИЗ "), 404, 2);
  myGLCD.setColor(VGA_SILVER);
  myGLCD.textRus(F("` "), 55, 95);
  myGLCD.textRus(F("`/с "), 50, 75);
  myGLCD.textRus(F("` "), 460, 95);
  myGLCD.textRus(F("`/с "), 435, 75);
  VievTemp();
  K_Kpres_ok = false;
  PrPrint(B_SetX + 2, T_SetY);
  myGLCD.setColor(VGA_ORAN);
  for (i = 0; i < 5; i++) {
    if (i == LastPos) Ramka( i, VGA_RED);
    else Ramka( i, VGA_SILVER);
  }
#ifdef Set_Picture
  myGLCD.drawBitmap(BattSet[1][0] + 10, BattSet[1][1] + 7, 68, 48, Graf, 1);
  myGLCD.drawBitmap(BattSet[0][0] + 10, BattSet[0][1] + 7, 68, 48, Graf2, 1);
  myGLCD.drawBitmap(BattSet[2][0] + 10, BattSet[2][1] + 7, 68, 48, Graf3, 1);
  myGLCD.drawBitmap(BattSet[3][0] + 10, BattSet[3][1] + 7, 68, 48, Graf4, 1);
#endif
  updateScreen = false;
}
// --------------------------------------------
void PrPrint(int X, int Y) {        // номер и название профиля
  myGLCD.setColor(VGA_BLACK);
  myGLCD.fillRoundRect(X, Y, 479 - X - 1, Y + 15);
  myGLCD.setFont(BigFont);
  myGLCD.setColor(VGA_ORAN);
  myGLCD.printNumI(currentProfile, X, Y, 2);
  myGLCD.setFont(BigFontRus);
  myGLCD.setColor(VGA_LIME);
  myGLCD.textRus(DuStr, X + 40, Y);
}
// --------------------------------------------
void DumpMem (byte NnProf) {        // читаем ПЗУ в СОМ порт

  // Для экономии места все Serial закомментированы
  // для печати на экран все комменты < // > убрать
  // и в функции loadProfile снять комменты с вызова DumpMem

  // Serial.println("Читаем пзу ");
  
  String DuStr = "";
  for (byte j = 0; j <= SizePrrof - 1; j++) {
    if (j % 10 == 0) {
     // Serial.print(Adres+((NnProf-1)*SizePrrof)+j);
     // Serial.print(" ");
     }
  // byte  DuBre = EEPROM.read(Adres + ((NnProf - 1) * SizePrrof) + j);
    // Serial.print(DuBre);
    //Serial.print(" ");
    //if (j%10 == 9) Serial.println("");
    if (j == ArrMax - 1) {
     // Serial.println("");
      DuStr = String(u.Profili.HeadProf);
     // Serial.println(DuStr);
     // Serial.print(Adres+((NnProf-1)*SizePrrof)+j);
     // Serial.print(" ");
    }

  }
  //Serial.println("");
}
// ------------------------------
void loadProfile(byte NnProf) {     //  загрузка текущего профиля
  DuStr = "";
  String DuVr;
  for (byte j = 0; j <= SizePrrof - 1; j++) {
    u.Mode[j] = EEPROM.read(Adres + ((NnProf - 1) * SizePrrof) + j);
  }
  DuVr = String(u.Profili.HeadProf);
  if (u.Mode[1] == 255)  DuStr += "НЕИЗВЕСТНЫЙ ПРОФИЛЬ " + String(NnProf) + " ";
  else {
    for (byte i = 0; i < ArrMax; i++) {
      if (DuVr.substring(i, i + 1) == "\0" || DuVr.substring(i, i + 1) == "\n") {
        DuStr += DuVr.substring(0, i);
        DuStr += "\n";
        i = ArrMax;
      }
    }
  }
  if (DuStr.length() == 0) DuStr += "НЕИЗВЕСТНЫЙ ПРОФИЛЬ " + String(NnProf) + " ";
  Obsi2 = NnProf;
  // Serial.print(DuStr.length()); Serial.print(" -"); Serial.println(DuStr);
  // DumpMem(NnProf);
}
// --------------------------------------------
void SaveProfile(byte NnProf) {     //  сохранение текущего профиля
 //Serial.print(" >> "); Serial.println(NnProf);
  for (byte j = 0; j <= SizePrrof - 1; j++) {    //j = ArrMax - 1  без заголовка
    EEPROM.update(Adres + ((NnProf - 1)*SizePrrof) + j, u.Mode[j]);
 }
}
// --------------------------------------------
void In_Line(int t_start) {         //  рисуем профиль на графике
  int Temp_Start = t_start, Temp_End;
  int Time_Start = 0, Time_End;
  int Rt;
  // рисуем график температуры НИЗа
  myGLCD.setColor(VGA_RED);
  Temp_End = u.Profili.temperatureBOTTOM;
  Time_End = Time_Start + (10 * (Temp_End - t_start)) / u.Profili.rampRateBOTTOM;
  M_dr_line (Time_Start, Temp_Start, Time_End, u.Profili.temperatureBOTTOM);          // участок роста температуры
  Pr_Line_Load (0, Time_Start, Temp_Start, Time_End, u.Profili.temperatureBOTTOM);
  M_dr_int(u.Profili.dwellTimerBOTTOM, Time_End, u.Profili.temperatureBOTTOM - 3);    // надписи температуры и задержка
  M_dr_int(u.Profili.temperatureBOTTOM, Time_End - 26, u.Profili.temperatureBOTTOM + 10);
  Rt = Time_End;
  Time_End += u.Profili.dwellTimerBOTTOM;
  Time_Start = Time_End;
  Temp_Start = u.Profili.temperatureBOTTOM;

  // рисуем график температуры ВЕРХа по шагам
  myGLCD.setColor(VGA_ORAN);
  BrStepMax = 8;
  for (int i = 0; i < 4; i++) {
    if (u.Profili.rampRateStep[i] == 0) {
      BrStepMax = i * 2;
      break;
    }
    Temp_End = u.Profili.temperatureStep[i] - Temp_Start;
    if (Temp_End < 0) Temp_End = - Temp_End;
    Time_End = Time_Start + (10 * Temp_End) / u.Profili.rampRateStep[i];
    M_dr_line (Time_Start, Temp_Start, Time_End, u.Profili.temperatureStep[i]);
    Pr_Line_Load ((i + 1) * 2, Time_Start, Temp_Start, Time_End, u.Profili.temperatureStep[i]);

    M_dr_int(u.Profili.dwellTimerStep[i], Time_End, u.Profili.temperatureStep[i] - 3);
    M_dr_int(u.Profili.temperatureStep[i], Time_End - 26, u.Profili.temperatureStep[i] + 10);

    M_dr_line (Time_End, u.Profili.temperatureStep[i], Time_End + u.Profili.dwellTimerStep[i], u.Profili.temperatureStep[i]);
    Pr_Line_Load( (i + 1) * 2 + 1, Time_End, u.Profili.temperatureStep[i], Time_End + u.Profili.dwellTimerStep[i], u.Profili.temperatureStep[i]);
    Time_End += u.Profili.dwellTimerStep[i];
    Temp_Start = u.Profili.temperatureStep[i];
    Time_Start = Time_End;
  }
  myGLCD.setColor(VGA_RED);
  M_dr_line (Rt, u.Profili.temperatureBOTTOM, Time_Start, u.Profili.temperatureBOTTOM);    // горизонтальный участок низа
  Pr_Line_Load(1, Rt, u.Profili.temperatureBOTTOM, Time_Start, u.Profili.temperatureBOTTOM);

  /* for (i=0; i<10; i++) {    // вывод в порт массива
      for (int j=0; j<4; j++) {
       Serial.print(Pr_Line_M[i][j]);
       Serial.print("  ");
      }
      Serial.println("");
     } // */
}
// --------------------------------------------
void Sec_metr() {
  RTC.read(tm);
  if (OldSek != tm.Second) {
    OldSek = tm.Second;
    ss++;
    if (ss == 60) {
      ss = 0;
      mm++;
    }
    myGLCD.setColor(VGA_BLACK);
    myGLCD.fillRoundRect(432, TempY, 447, TempY + 16);
    myGLCD.setFont(BigFont);
    myGLCD.setColor(VGA_SILVER);

    myGLCD.printNumI(mm, 400, TempY, 2, '0');
    myGLCD.printNumI(ss, 448, TempY, 2, '0');
   
  }
}
// --------------------------------------------
bool TikTime () {
   if (!RTC.read(tm))  Serial.println(" не прочитались часы");
  if (OldSek == tm.Second) return(false);
  ss = tm.Second;             // Advance second
  OldSek = ss;
  mm = tm.Minute;             // Advance minute
  hh = tm.Hour;               // Advance hour 
  Obsi1 = hh;
 return(true);
}
// --------------------------------------------
void Pr_Stels () {                  //  движение стрелок в часах
  if (!TikTime()) return;
  sdeg = ss * 6;                     // 0-59 -> 0-354
  mdeg = mm * 6 + sdeg * 0.01666667; // 0-59 -> 0-360 - includes seconds, but these increments are not used
  hdeg = hh * 30 + mdeg * 0.0833333; // 0-11 -> 0-360 - includes minutes and seconds, but these increments are not used
  hx = cos((hdeg - 90) * Fconst);
  hy = sin((hdeg - 90) * Fconst);
  mx = cos((mdeg - 90) * Fconst);
  my = sin((mdeg - 90) * Fconst);
  sx = cos((sdeg - 90) * Fconst);
  sy = sin((sdeg - 90) * Fconst);

  myGLCD.setColor(VGA_BLACK);
  if (ss == 0 || initial) {
    initial = 0;
    // Erase hour and minute hand positions every minute
    myGLCD.drawLine(ohx, ohy, xpos, ypos);
    ohx = hx * (ypos / 2) + xpos + 1;
    ohy = hy * (ypos / 2) + ypos;
    myGLCD.drawLine(omx, omy, xpos, ypos);
    omx = mx * (ypos / 2 + 30) + xpos;
    omy = my * (ypos / 2 + 30) + ypos;
  }
  // Redraw new hand positions, hour and minute hands not erased here to avoid flicker
  myGLCD.drawLine(osx, osy, xpos, ypos);
  osx = sx * (ypos / 2 + 40) + xpos + 1;
  osy = sy * (ypos / 2 + 40) + ypos;

  myGLCD.setColor(VGA_WHITE);
  myGLCD.drawLine(ohx, ohy, xpos, ypos);  // часовая стрелка
  myGLCD.drawLine(omx, omy, xpos, ypos);  // минутная стрелка

  myGLCD.setColor(VGA_RED);               // секундная стрелка
  myGLCD.drawLine(osx, osy, xpos, ypos);
  myGLCD.fillCircle(xpos, ypos, 7);
}
// --------------------------------------------
void Pr_Ramka_Y (int i) {           //  рисуем желтую рамку большую
  myGLCD.setColor(VGA_BLACK);     // стираем староое
  myGLCD.drawLine(0, Ywin1, 479, Ywin1);
  myGLCD.drawLine(0, Ywin1 - 1, 479, Ywin1 - 1);
  myGLCD.drawLine(0, Ywin1 - 2, 479, Ywin1 - 2);

  myGLCD.setColor(VGA_ORAN);   // рисуем желтую рамку большую
  myGLCD.drawLine(Xsize * i + 1, Yset + 1, Xsize * i + 1, Ywin1 - 1);
  myGLCD.drawLine(Xsize * i + 2, Yset + 1, Xsize * i + 2, Ywin1 - 2); //2
  myGLCD.drawLine(Xsize * i, Ywin1 - 2, 1, Ywin1 - 2);
  myGLCD.drawLine(Xsize * i, Ywin1 - 1, 0, Ywin1 - 1); //2
  myGLCD.drawLine(0, Ywin1 - 2, 0, 318);
  myGLCD.drawLine(1, Ywin1, 1, 318);                     //2
  myGLCD.drawLine(1, 319, 478, 319);
  myGLCD.drawLine(1, 318, 478, 318);                            //2
  myGLCD.drawLine(479, 318, 479, Ywin1 - 1);
  myGLCD.drawLine(478, 318, 478, Ywin1 - 1);               //2
  myGLCD.drawLine(479, Ywin1 - 2, Xsize * (i + 1) - 2, Ywin1 - 2);
  myGLCD.drawLine(478, Ywin1 - 1, Xsize * (i + 1) - 2, Ywin1 - 1); //2
  myGLCD.drawLine(Xsize * (i + 1) - 2, Ywin1 - 2, Xsize * (i + 1) - 2, Yset + 2);
  myGLCD.drawLine(Xsize * (i + 1) - 3, Ywin1 - 2, Xsize * (i + 1) - 3, Yset + 2); //2
  myGLCD.drawLine(Xsize * i + 2, Yset, Xsize * (i + 1) - 3, Yset);
  myGLCD.drawLine(Xsize * i + 1, Yset + 1, Xsize * (i + 1) - 2, Yset + 1); //2
}
// --------------------------------------------
void K_Setka() {
  myGLCD.setFont(SmallFont);
  for (i = 0; i <= K_line; i++)  {      // рисуем горизонтальные линии
    myGLCD.setColor(VGA_ORAN);         // 250, 180, 000
    myGLCD.printNumI((K_line - i) * 20, 0, Py_pr + i * 20 , 3); //,'0'
    myGLCD.setColor(30, 30, 30);
    myGLCD.drawLine(X_graf, Py_pr + 6 + i * 20, 479, Py_pr + 6 + i * 20);
  }
  myGLCD.setColor(VGA_ORAN);
  myGLCD.drawLine(X_graf - 3, Y_graf, 479, Y_graf);
  myGLCD.drawLine(X_graf - 3, Y_graf + 1, 479, Y_graf + 1);
  byte nn = 0;
  for (i = 0; i <= 14; i++)  {           // рисуем нижнюю шкалу
    myGLCD.drawLine(X_graf + i * 30, Y_graf - 6, X_graf + i * 30, Y_graf - 1);
    if (i > 0) nn = 4;
    if (i > 3) nn = 8;
    myGLCD.printNumI(i * 30, nn + 9 + i * 30, Y_graf + 4 , 3);
  }
}
// --------------------------------------------
void press_ok() {               // обработчик короткого  ok()
  if (reflowState == REFLOW_STATE_IDLE)       {
    switch (LastPos)   {
      case 0: {
          if (!K_Kpres_ok) {   // ButLeft переход в настройки
            ButLeft();
            K_Kpres_ok = true;
            Svkl_L = false;
          }
          break;
        }
      case 1: {
          if (!K_Kpres_ok) {   // ButRight переход к графикам профиля
            ButRight();
            K_Kpres_ok = true;
          }
          break;
        }
      case 2: {
          ButUp();
          break;
        }
      case 3: {
          ButDown();
          break;
        }
      case 4: {
          ButOk();
          break;
        }
    }
  }
  if (reflowState == REFLOW_STATE_SETTING)    {
    if (!Svkl_L && !K_Kpres_ok)  {
      Vklad_G_Mem = Vklad_G;
      SetStrL(true);
      Vklad_L = 0;
      if (Vklad_G == 1) Vklad_L = 1;
      NewPos = Vklad_L;
      encoder.setPosition(NewPos);
      K_Kpres_ok = true;
    }
    if (Svkl_L && !K_Kpres_ok)  {     // переход в редактирование параметра
      Pos_Mem = LastPos;
      K_Kpres_ok = true;
      //NewPos = 2;
      //encoder.setPosition(NewPos);
      ButOk();
    }
  }
  if (reflowState == REFLOW_STATE_PROFILE)    {
    if (!K_Kpres_ok) {
      ButRight();
    }
  }
  if (reflowState == REFLOW_STATE_SET_EDIT)   {  // возврат в меню настроек после редактирования
    if (K_Kpres_ok) return;
    NewPos = Pos_Mem;
    encoder.setPosition(NewPos);
    ButOk();
  }
  if (reflowState == REFLOW_STATE_PROFILE_LOAD)   {  // возврат в меню настроек загрузки профиля
    if (K_Kpres_ok) return;
    ButRight();
  }
  if (reflowState == REFLOW_STATE_TIME_EDIT) {
    F_timK = true;
    if (Stat < 3) Stat += 1;
      else Stat = 0;
    NewPos = Shms[Stat];
    encoder.setPosition(NewPos);
    LastPos = NewPos;
    }
}
// --------------------------------------------
void longPress_ok() {           // обработчик длинного ok()
  if (reflowState == REFLOW_STATE_SETTING) {
    if (Svkl_L) {
      SetStrL(false);
      NewPos = Vklad_L;
      encoder.setPosition(NewPos);
      Shag_old = 10;
      Vklad_L = 0;
    }
    else {
      Vklad_G = -1;
      NewPos = 0;
      ButLeft();
    }
  return;
  }
  if (reflowState == REFLOW_STATE_PROFILE || reflowState == REFLOW_STATE_BRAZE_NOW
      || reflowState == REFLOW_STATE_BRAZE_STOP  )    {
    Vklad_G = -1;
    NewPos = 0;
    ButLeft();
  }
  if (reflowState == REFLOW_STATE_PROFILE_LOAD)   {  // возврат в меню настроек загрузки профиля
    if (Stat == 2) ButOk();
      else ButLeft();
    }
  if (reflowState == REFLOW_STATE_TIME_EDIT) {  // установка часов
    if (Stat == 3) ButOk();
      else {
        Stat = 0;
        ButLeft();
      }
    }
}
// --------------------------------------------
void TimeLine(int XT) {
  byte hh = 0;
  if (XT > 50 && XT < 146) hh = 50;
  if (XT >= 146 && XT < 182) hh = 17;
  myGLCD.setColor(VGA_BLACK);
  myGLCD.drawLine(XT - 1, Py_pr + hh + 1, XT - 1, 263); // стираем старую таймлайн линию
  myGLCD.setColor(VGA_SILVER);     // 250, 180, 000
  myGLCD.drawLine(XT, Py_pr + hh + 1, XT, 263);  // рисуем новую
  myGLCD.setColor(30, 30, 30);     // VGA_SILVER
  for (i = 0; i < K_line; i++)  {                 // восстанавливаем сетку
    myGLCD.drawPixel(XT - 1, Py_pr + 6 + i * 20);
  }
  myGLCD.setColor(VGA_ORAN);
}
// --------------------------------------------
void SetStrL (bool Vk_L) {      // Боковая красная линия признак нахождения в нижнем меню
  Svkl_L = Vk_L;
  if (!Vk_L) myGLCD.setColor(VGA_BLACK);
  else myGLCD.setColor(VGA_RED);
  myGLCD.drawLine(3, Ywin1, 3, 316);
  myGLCD.drawLine(4, Ywin1, 4, 316);                     //2
  myGLCD.drawLine(5, Ywin1, 5, 316);
}
// --------------------------------------------
void TempBraze () {
  if (tc2<0) {
    myGLCD.setFont(BigFont);
    myGLCD.setColor(VGA_RED);
    myGLCD.print(F("ERORR"), 347, 217);    
    }
   else {
    myGLCD.setFont(SevenSegNumFont);
    myGLCD.setColor(VGA_RED);
    myGLCD.printNumI(tc2, 340, 200, 3, '0'); // выводим температуру низа
    }
  myGLCD.setFont(BigFont);
  myGLCD.printNumI(Output2 , 440, 200, 2, '0'); // выводим мощность низа

  if (tc1<0) {
    myGLCD.setFont(BigFont);
    myGLCD.setColor(VGA_RED);
    myGLCD.print(F("ERORR"), 57, 47);
    }
   else { 
    if (TopStart == false)  myGLCD.setColor(VGA_SILVER);
    myGLCD.printNumI(Output1, 150, 30, 2, '0');   // выводим мощность верха
    myGLCD.setFont(SevenSegNumFont);
    myGLCD.printNumI(tc1, 50, 30, 3, '0'); // выводим температуру верха
   }
}
// --------------------------------------------
void TonGo () {
  //Мелодия приветствия Марио
  tone(buzzerPin,1318,150);
  delay(150);
  tone(buzzerPin,1318,300);
  delay(300);
  tone(buzzerPin,1318,150);
  delay(300);
  tone(buzzerPin,1046,150);
  delay(150);
  tone(buzzerPin,1318,300);
  delay(300);
  tone(buzzerPin,1568,600);
  delay(600);
  tone(buzzerPin,784,600);
  delay(600);
  noTone(buzzerPin); 
 
}
// --------------------------------------------
void HeartPic () {
  #ifdef SetPicHeater
    myGLCD.drawBitmap(65, 105, 64, 64, Heater2, 1);
    #define SsPic1 150
    #define SsPic2 164
  #else
    #define SsPic1 CENTER
    #define SsPic2 CENTER
  #endif
    myGLCD.setColor(VGA_GREEN);
    myGLCD.setFont(BigFont);
    myGLCD.print(F("ARDUINO MEGA2560"),SsPic1, 105);   //CENTER
    myGLCD.setColor(VGA_RED);
    myGLCD.print(F("REWORK STATION"),SsPic2, 140);     //
  //  myGLCD.setColor(250, 180, 000);
  //  myGLCD.print(F("TFT-LCD 480*320 MAX6675_Dimmer"),CENTER, 180);
    myGLCD.setColor(VGA_SILVER);
    myGLCD.print(F("Watashi Svs_V09"),SsPic2, 175);

    delay(2000);   
  // TonGo();       //Мелодия приветствия Марио
}
// --------------------------------------------
void WinRamka (int KorX, int KorY, int DLX, int DLY, word Color, bool Clean) {
  if (Clean) {
     myGLCD.setColor(Color);
     myGLCD.fillRoundRect(KorX, KorY, KorX+DLX, KorY+DLY);
    }
    else {
      myGLCD.setColor(Color);  
      myGLCD.drawRoundRect(KorX, KorY, KorX+DLX, KorY+DLY);
      myGLCD.drawRoundRect(KorX+1, KorY+1, KorX+DLX-1, KorY+DLY-1); 
    }
}
// --------------------------------------------
void WrLoadPr (byte Pr_Out, byte Pr_In) {
  for (j=0; j<ArrMax; j++) u.Profili.HeadProf[j]=32;
  strcpy_P(u.Profili.HeadProf, (char *)pgm_read_word(&(AdrProfN[Pr_In-1])));  //  пишем в буфер
  for (j=1; j<=(SizePrrof-ArrMax); j++) {
    u.Mode[ArrMax+j-1] = pgm_read_byte_near(ProfDate[Pr_In-1]+j);
    } 
  SaveProfile(Pr_Out);
}
// --------------------------------------------
#ifdef SetTouch1
bool TouchGet() {
 // Serial.print("Touch1 -");
  tp = ts.getPoint();   // tp.x, tp.y считываем тач
  if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) {
    if (SwapXY != (Orientation & 1)) SWAP(tp.x, tp.y);
    xpok = map(tp.x, TS_LEFT, TS_RT, 0, DisXSize);
    ypok = map(tp.y, TS_TOP, TS_BOT, 0, DisYSize);
    //Serial.println("xpok = " + String(xpok) + "  ypok = " + String(ypok));
    return (true);
  }
  return (false);
}
#endif  
// --------------------------------------------
#ifdef SetTouch2
bool TouchGet() {
  
  if (myTouch.dataAvailable() == true) {
    myTouch.read();
    xpok = myTouch.getX();
    ypok = myTouch.getY();
//   Serial.print("Touch2 -");   Serial.print(xpok, HEX); Serial.print(" "); Serial.println(ypok);
    if ((xpok != -1) || (ypok != -1))  {
      return (true);
      }
    }
  return (false);
}
#endif
void TimTouSet() {
  TouchSet = millis() + 400; 
  return;
}
//       Pr_WinTime();    // на экран засеченное время в мсек
//       WinStartTime = micros();

// --------------------------------------------
void setup() {
  myGLCD.InitLCD();
  myGLCD.clrScr();
  Serial.begin(9600);
#ifdef SetTouch1
  SetPinTouch1();
    switch (Orientation) {      // adjust for different aspects
    case 0:   break;          //no change,  calibrated for PORTRAIT
    case 1:   tmp = TS_LEFT, TS_LEFT = TS_BOT, TS_BOT = TS_RT, TS_RT = TS_TOP, TS_TOP = tmp;  break;
    case 2:   SWAP(TS_LEFT, TS_RT);  SWAP(TS_TOP, TS_BOT); break;
    case 3:   tmp = TS_LEFT, TS_LEFT = TS_TOP, TS_TOP = TS_RT, TS_RT = TS_BOT, TS_BOT = tmp;  break;
    } 
#endif
#ifdef SetTouch2
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
#endif
  //setup pins as input for buttons
  pinMode (buzzerPin, OUTPUT);
  pinMode(P1_PIN, OUTPUT);
  digitalWrite(P1_PIN, SetReleOFF); 
  pinMode(P2_PIN, OUTPUT);
  digitalWrite(P2_PIN, SetReleOFF); 
  pinMode(P3_PIN, OUTPUT);
  digitalWrite(P3_PIN, SetReleOFF); 
  pinMode(P4_PIN, OUTPUT);
  digitalWrite(P4_PIN, SetReleOFF); 
  HeartPic ();
  Set_Pin_6675();             // настраиваем порты max6675
  //
  DisXSize = myGLCD.getDisplayXSize();
  DisYSize = myGLCD.getDisplayYSize();
  currentProfile = 1;
  loadProfile(currentProfile);
  Xsize = myGLCD.getDisplayXSize() / Win1;
  Y_S_win = (myGLCD.getDisplayYSize() - (Ywin0) - 2) / Win2;
  Y_shift = Ywin0 + (Y_S_win - 16) / 2;
  //setup ssr pins as outputs
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  encoder.setPosition(2 / ROTARYSTEPS); // start with the value of 1

  xpos = -1 + myGLCD.getDisplayXSize() / 2;
  ypos = -1 + (myGLCD.getDisplayYSize() - 50) / 2;

  nextRead1 = millis();
#ifdef SetInterrupt
  attachInterrupt(SetInterrupt, Dimming, RISING); // настроить порт прерывания(0 или 1) 2й или 3й цифровой пин
#else
  MsTimer2::set(10, Dimming); // 100ms period
  MsTimer2::start();
#endif
} // end  setup
// ------------------------------------------------------
void loop() {
  //unsigned long currentMillis = millis();
#ifdef SetAnalogBatton
  // Serial.println(analogRead(Knopki));
  if (AKeys.Refresh(analogRead(Knopki))) Kn_work();  // состояние кнопок, если нажата кнопка отрабатываем нажатие
  else {   // сброс флага нажатия
    if (!AKeys.Key1() && Key1_Old) Key1_Old = 0;
    if (!AKeys.Key2() && Key2_Old) Key2_Old = 0;
    if (!AKeys.Key3() && Key3_Old) Key3_Old = 0;
    if (!AKeys.Key4() && Key4_Old) Key4_Old = 0;
    if (!AKeys.Key5() && Key5_Old) Key5_Old = 0;
  }
#endif

#ifdef SetDigitBatton
  Btn_UP.run(&ButUp, &longPr_MID);
  Btn_DWN.run(&ButDown, &longPr_MID);
  Btn_LFT.run(&ButLeft, &longPr_MID);
  Btn_RHT.run(&ButRight, &longPr_MID);
  Btn_MID.run(&ButOk, &longPr_MID);
#endif

#ifdef SetEncoder
  Btn_ok.run(&press_ok, &longPress_ok);              // опрашиваем кнопку энкодера
  encoder.tick();                                    // читаем енкодер
  NewPos = encoder.getPosition();
  // Serial.print(NewPos); Serial.print("  "); Serial.print(Svkl_L); Serial.print("  ");  Serial.println(reflowState);
  if (LastPos != NewPos) {
    if (reflowState == REFLOW_STATE_IDLE)       {
      if (NewPos > 4) {
        NewPos = 0;
        encoder.setPosition(NewPos);
      }
      if (NewPos < 0) {
        NewPos = 4;
        encoder.setPosition(NewPos);
      }
      Ramka(LastPos, VGA_SILVER);
      Ramka(NewPos, VGA_RED);
      LastPos = NewPos;
      
    }
    if (reflowState == REFLOW_STATE_SETTING)    {
      if (!Svkl_L) {
        if (NewPos > 3) NewPos = 0;
        if (NewPos < 0) NewPos = 3;
        encoder.setPosition(NewPos);
        if (Vklad_G == 1) {
          Serial.println(LastPos);
          if (NewPos > LastPos && Shag != 3) { Shag += 1; encoder.setPosition(LastPos); return; }
          if (NewPos < LastPos && Shag != 0) { Shag -= 1; encoder.setPosition(LastPos); return; }
          }
        Vklad_G = NewPos;
        Shag = 0;  Shag_old = 0;
        LastPos = NewPos;
        }
      else {      //Serial.print("тут1  ");
        K_Pos = 0;
        if (Vklad_G == 1) K_Pos = 1;
        if (NewPos > 5) NewPos = K_Pos;
        if (NewPos < K_Pos)  NewPos = 5;
        encoder.setPosition(NewPos);
        Vklad_L = NewPos;
        LastPos = NewPos;
        Vklad_L_old = 15;
        return;
      }
    }
    if (reflowState == REFLOW_STATE_PROFILE)    {
      if (NewPos > numMax) {
        NewPos = 1;
        encoder.setPosition(NewPos);
      }
      if (NewPos < 1) {
        NewPos = numMax;
        encoder.setPosition(NewPos);
      }
      currentProfile = NewPos;
      loadProfile(currentProfile);
      updateScreen = true;
      LastPos = NewPos;
    }
    if (reflowState == REFLOW_STATE_SET_EDIT)   {
      if (NewPos > LastPos) {
        ButUp();
        encoder.setPosition(LastPos);
        return;
      }
      if (NewPos < LastPos) {
        ButDown();
        encoder.setPosition(LastPos);
        return;
      }
    }
    if (reflowState == REFLOW_STATE_PROFILE_LOAD) {
      if (Stat != 2) {
        if (NewPos > numMax) NewPos = 0;
        if (NewPos < 0) NewPos = numMax;
        encoder.setPosition(NewPos);
        Obsi2 = NewPos;
        LastPos = NewPos;
       }
    }
    if (reflowState == REFLOW_STATE_TIME_EDIT) {
      
      if (Stat == 0) i=23;
        else i = 59;
      if (NewPos > i) NewPos = 0;
      if (NewPos < 0) NewPos = i;
      encoder.setPosition(NewPos);
      LastPos = NewPos;
      Shms[Stat] = NewPos;
      //Stat = NewPos;
      F_timK = true;
    }  
   }
   else K_Kpres_ok = false;
#endif

#ifdef Set_Touch
  if (TouchGet()) {                                // обработка нажатия на тач
 // Serial.print("Touch2 -");   Serial.print(xpok, HEX); Serial.print(" "); Serial.println(ypok);
    if (reflowState == REFLOW_STATE_IDLE)       {
      for (i = 0; i <= 4; i++) {
        if (xpok > BattSet[i][0] && xpok < BattSet[i][2] &&
            ypok > BattSet[i][1] && ypok < BattSet[i][3] ) {
        if (TouOld != i) {
          TouOld = i;
          TimTouSet();
          Ramka(LastPos, VGA_SILVER);
          Ramka(i, VGA_RED);
          LastPos = i; encoder.setPosition(i);
          switch (i)   {
            case 0: {
                if (!T_KeyLeft_Old) {
                  T_KeyLeft_Old = true;
                  ButLeft();
                  i = 5;
                }
                break;
              }
            case 1: {
                if (!T_KeyLeft_Old) {
                  T_KeyLeft_Old = true;
                  ButRight();
                } 
                break;
              }
            case 2: {
                if (!T_KeyUp_Old) {
                  T_KeyUp_Old = true;
                  ButUp(); 
                }
                break;
              }
            case 3: {
                if (!T_KeyDown_Old) {
                  T_KeyDown_Old = true;
                  ButDown();
                  break;
                }
              }
            case 4: {
                  if (!T_KeyOk) {
                  T_KeyOk = true;
                  ButOk();
                  }
                break;
              }
          }  // end switch
         } 
        }        // end проверка одной кнопки 
      }         // конец цикла
    }           // end STATE_IDLE
    if (reflowState == REFLOW_STATE_PROFILE)    {
      if (xpok > 0 && xpok < 40 && ypok > 30 && ypok < 300 && !T_KeyLeft_Old) {
        T_KeyLeft_Old = true;
        ButLeft();
        }
      if (xpok > 430 && xpok < 478 && ypok > 30 && ypok < 300) ButRight();
    }
    if (reflowState == REFLOW_STATE_BRAZE_NOW || reflowState == REFLOW_STATE_BRAZE_STOP)  {
      if (xpok > 0 && xpok < 40 && ypok > 30 && ypok < 300) ButLeft();
    }
    if (reflowState == REFLOW_STATE_SETTING)    {
      for (i = 0; i < Win1; i++)  {   // переключаем вкладки
        if (xpok > Xsize * i && xpok < Xsize * (i + 1) && ypok > 0 && ypok < Ywin0) {
          Vklad_G = i;
          encoder.setPosition(i);
          if (Vklad_G == 1) { LastPos = 1; encoder.setPosition(1); }
            SetStrL(false);
          break; 
        }
      }
      for (i = 0; i <= Win2; i++) {   // переключаем строки
        if (xpok > 40 && xpok < 355 && ypok > i * Y_S_win + Ywin0 && ypok < (i + 1)*Y_S_win + Ywin0) {
          if (Vklad_G == 1 && Shag <= 3 && !T_KeyNext && i == 0) {
            if (Svkl_L) {   
              T_KeyNext = true;
              SetStrL(false);
              LastPos = 0;
              Shag_old = 10;
              Vklad_L = 1;
              encoder.setPosition(1);
              break;
            }
            else {
              T_KeyNext = true;
              Shag += 1;
              if (Shag > 3)  Shag = 0;
              break;
            }
          }
          Vklad_L = i;
          encoder.setPosition(i);
          SetStrL(true);
          Vklad_L_old = 15;   
          //Shag_old = 10;
          break;
        }
      }
      if (xpok > 355 && xpok < 468 && ypok > Mkk && ypok < Mkk+69 && !T_KeyOk) {  // окно редактирования параметра
        T_KeyOk = true;
        TimTouSet();
        ButOk();
      }
      if (xpok > 0 && xpok < 40 && ypok > 30 && ypok < 300 && !T_KeyLeft_Old) { // выход в главное меню
        Vklad_G = 0;
        T_KeyLeft_Old  = true;
        ButLeft();
      }
    }
    if (reflowState == REFLOW_STATE_SET_EDIT)   {
      if (xpok > 355 && xpok < 468 && ypok > Mkk && ypok < Mkk+69 && !T_KeyOk) {
        TimTouSet();
        T_KeyOk = true;
        ButOk();
        return;
      }
      if (xpok > 425 && xpok < 479 && ypok > 267 && ypok < 320 && !T_KeyRight_Old) {
        T_KeyRight_Old = true;
        TimTouSet();
        ButUp();
        return;
      }
      if (xpok > 377 && xpok < 424 && ypok > 267 && ypok < 320 && !T_KeyLeft_Old) {
        T_KeyLeft_Old = true;
        TimTouSet();
        ButDown();
        return;
      }
      j=0;  w=0;
      for (i = 0; i < 10; i++) { 
        if (i>=5) { 
          j=Batt_Y+10; w=5;
          }
       // WinRamka (20+(i-w)*(Batt_X+10), 189+j, Batt_X, Batt_Y, VGA_SILVER, 0);
        if (!TouDec && xpok > 20+(i-w)*(Batt_X+10) && xpok < 20+Batt_X+(i-w)*(Batt_X+10) 
            && ypok > 189+j && ypok < 189+j+Batt_Y) {
              TouDec = true;
              TimTouSet();
              Edit_Byte = 10*Edit_Byte + i;
              if (Edit_Byte > 255)  Edit_Byte = i; 
              i=10;
            }
        }
       return; 
    }
    if (reflowState == REFLOW_STATE_TIME_EDIT)   {
      if (xpok > 0 && xpok < 30 && ypok > 30 && ypok < 300) {  // выход без установки
        Stat = 0;
        ButLeft();
        }
      if (xpok > 390 && xpok < 460 && ypok > 108 && ypok < 177 && !T_KeyOk) {
        T_KeyOk = true;
        Stat = 3;
        ButOk();
        }
      if (xpok > 327 && xpok < 380 && ypok > 105 && ypok < 177 && !T_KeyRight_Old) {
        T_KeyRight_Old = true;
        ButUp();
        }
      if (xpok > 278 && xpok < 326 && ypok > 105 && ypok < 177 && !T_KeyLeft_Old) {
        T_KeyLeft_Old = true;
        ButDown();
      }
      for (i = 0; i <= 2; i++)  {   // 
        if (xpok > 10+i*82 && xpok < 75+i*82 && ypok > 105 && ypok < 177) {
          Stat = i;
          F_timK = true;
          }
      }
    }
    if (reflowState == REFLOW_STATE_PROFILE_LOAD)   {
      if (xpok > 0 && xpok < 30 && ypok > 30 && ypok < 300)  ButLeft();
      if (xpok > 420 && xpok < 479 && ypok > Y_shift - 4 && ypok < Y_shift +30 && !T_KeyOk) {  
        T_KeyOk = true;
        TimTouSet();
        Stat = 2;
        ButOk();
        }
      if (xpok > 425 && xpok < 479 && ypok > 267 && ypok < 320 && !T_KeyRight_Old) {
        T_KeyRight_Old = true;
        TimTouSet();
        ButUp();
        }
      if (xpok > 377 && xpok < 424 && ypok > 267 && ypok < 320 && !T_KeyLeft_Old) {
        T_KeyLeft_Old = true;
        TimTouSet();
        ButDown();
        }
      }
  }
  if (millis() > TouchSet) {
    TouOld = 10;
    TouDec = false;
    T_KeyUp_Old = false;
    T_KeyDown_Old = false;
    T_KeyNext = false;
    T_KeyOk = false;
    T_KeyRight_Old = false;
    T_KeyLeft_Old = false;
    }
#endif

  switch (reflowState)              {          //  переключатель состояния
    case REFLOW_STATE_IDLE:         {          //  Главный экран
        if (updateScreen) GlabPrint();           //  настройка экрана Рабочий режим
        Pr_Stels();                              //  стрелки часов тикают

        if (millis() > nextRead1) {
          nextRead1 = millis() + SENSOR_SAMPLING_TIME;
          TempRead();    //  считываем температуру
          VievTempGl();
        }
#ifdef SetConnectPC
        sprintf (buf, "OK%03d%03d%03d%03d%03d\r\n", (Output1), (Output2), tc1, tc2, (1)); // profileName
        Serial.print(buf);
#endif
        Output2 = 0;
        Output1 = 0;
      }
      break;
    case REFLOW_STATE_SETTING:      {          //  вход в меню настроек
        #include "ConstProgm.h"
        if (updateScreen) ProfEdit_2();        
        myGLCD.setFont(BigFontRus);
        if (Vklad_G != Vklad_G_old) {       // рисуем заголовки страницы настроек
          for (i = 0; i < Win1; i++)  {
            if (Vklad_G != i) {             // рамки заголовка
              myGLCD.setColor(VGA_SILVER);
              myGLCD.drawRoundRect(Xsize * i + 1, Yset, Xsize * (i + 1) - 2, Ywin1);
              myGLCD.drawRoundRect(Xsize * i + 2, Yset + 1, Xsize * (i + 1) - 3, Ywin1 - 1);
            }
            if (Vklad_G == i) myGLCD.setColor(VGA_LIME);
            int Xset2 = (Xsize - (strlen(myStr2[i]) * 8)) / 2;
            myGLCD.textRus(myStr2[i], Xsize * i + Xset2, Yset + (Ysize - 16) / 2); // текст в рамке
            Vklad_L_old = 15;
            Cln_st = true;
          }
          Pr_Ramka_Y(Vklad_G);    // рисуем желтую рамку
          Vklad_G_old = Vklad_G;
          Vklad_L = 0;
          if (Vklad_G == 1) { Shag = 0; Shag_old = 10; Vklad_L = 1;  }          
        }
        if (Vklad_L != Vklad_L_old || Shag != Shag_old) {       // пишем строки и заполняем данными
          K_Kpres_ok = false;
          for (i = 0; i < Win2; i++) { 
            if (Cln_st) {                   // очистка строки
              myGLCD.setColor(VGA_BLACK);
              myGLCD.fillRoundRect(10, i * Y_S_win + Y_shift, 460, i * Y_S_win + Y_shift + 16);
            }                      
            if (Vklad_L == i) {             // выбранный адрес параметра и строки подсвечиваем зеленым цветом
              myGLCD.setColor(VGA_LIME);   
              Wr_mem = (byte*)Adresa[Vklad_G][i] + Shag;
              Ad_tekct = (char*)myStr3[Vklad_G][i];
              } else myGLCD.setColor(VGA_SILVER); 
            if (i == 0 && Vklad_G == 1) myGLCD.setColor(VGA_ORAN);              
              myGLCD.textRus(myStr3[Vklad_G][i], 10, i * Y_S_win + Y_shift);  // выводим текст названия параметра
            if (Shag != Shag_old && Vklad_G == 1) {
              myGLCD.setColor(VGA_ORAN);
              for (j = 0; j < 4; j++) {
                if (Shag == j) {              // выбор ШАГ ПРОФИЛЯ
                  myGLCD.setColor(VGA_RED);
                  myGLCD.setBackColor(VGA_GRAY);
                }
                myGLCD.printNumI(j + 1, j * 40 + 318, i * Y_S_win + Y_shift);
                myGLCD.setColor(VGA_ORAN);
                myGLCD.setBackColor(VGA_BLACK);
              }
              myGLCD.setColor(VGA_SILVER);  
              Shag_old = Shag;
              continue;
            }
            Wr_pr = (byte*)Adresa[Vklad_G][i] + Shag;
              if (i == 1 && Vklad_G < 2) myGLCD.printNumF(*(byte*)Wr_pr * 0.1, 0, 286, i * Y_S_win + Y_shift);
                 else  if (i != 0 || Vklad_G != 1)  myGLCD.printNumI(*(byte*)Wr_pr, 286, i * Y_S_win + Y_shift, 3, ' ');
              
            if (Vklad_G == 3 && i == 3) {
             // Serial.println("Yes");
              if (Obsi4 == 1) myGLCD.drawBitmap(312, i * Y_S_win + Y_shift-5, 25, 25, yes, 1);
                else myGLCD.drawBitmap(312, i * Y_S_win + Y_shift-5, 25, 25, no, 1);
              }          
          }
          
            myGLCD.setFont(SevenSegNumFont);                
            myGLCD.setColor(VGA_SILVER);  
            myGLCD.printNumI(*(byte*)Wr_mem, 365, Mkk+9, 3, '0');  // выбранный параметр в окно редактирования
            WinRamka (355, Mkk, 113, 69, VGA_LIME, 0);
   
            Vklad_L_old = Vklad_L;
            Cln_st = false;
            
        }
        
          if (TikTime() && Vklad_G == 3) {    // отображение времени во вкладке ОБЩИЕ
            if (Vklad_L!=0)  myGLCD.setColor(VGA_SILVER);
              else  myGLCD.setColor(VGA_LIME);
            myGLCD.setFont(BigFontRus);
            sprintf (buf, "%02d:%02d:%02d\n", hh, mm, ss);            
            myGLCD.textRus(buf, 302,  Y_shift);   //Y_S_win +
          } 
      }
      break;
    case REFLOW_STATE_SET_EDIT:     {          //  редактирование параметра

#ifdef SetKalk
        if (updateScreen) {
          WinRamka (10, 105, 460, 203, VGA_BLACK, 1);
          WinRamka (10, 105, 460, 203, VGA_SILVER, 0);
      #if defined(Set_Picture) && defined(Set_Touch)
          myGLCD.drawBitmap(377, 267, 96, 50, Graf5, 1);
      #endif
          myGLCD.setColor(VGA_LIME);
          myGLCD.setFont(BigFontRus);
          myGLCD.textRus((char*) Ad_tekct, 20, 120);  // выводим текст названия параметра
          Edit_Byte = *(byte*)Wr_mem;
          Edit_Byte_old = Edit_Byte;
       j=0;  w=0;
          for (i = 0; i < 10; i++) {      // рисуем сенсорные кнопки
            if (i>=5) { j=Batt_Y+10; w=5;  }
            WinRamka (20+(i-w)*(Batt_X+10), 189+j, Batt_X, Batt_Y, VGA_SILVER, 0);
            myGLCD.printNumI(i, 42+(i-w)*(Batt_X+10), 207+j);   // цифры в кнопках
            }
          Edit_old = -1;
          K_Kpres_ok = false;
          
         updateScreen = false;          
        }
 #else
        if (updateScreen) {
          WinRamka (355, Mkk, 113, 69, VGA_RED, 0);
      #ifdef Set_Touch
            //WinRamka (355, 240, 113, 69, VGA_SILVER, 0);
        #ifdef Set_Picture
            myGLCD.drawBitmap(377, 267, 96, 50, Graf5, 1);
        #endif
      #endif
          Edit_Byte = *(byte*)Wr_mem;
          Edit_old = Edit_Byte;
          Edit_Byte_old = Edit_Byte;
          K_Kpres_ok = false;     
          updateScreen = false;
        }
 #endif        
        if (Edit_old != Edit_Byte) {
          myGLCD.setColor(VGA_ORAN);
          if (Edit_Byte_old == Edit_Byte) myGLCD.setColor(VGA_SILVER);
          myGLCD.setFont(SevenSegNumFont);
          myGLCD.printNumI(Edit_Byte, 365, Mkk+9 , 3, '0');
          Edit_old = Edit_Byte;
        }
      }
      break;
    case REFLOW_STATE_TIME_EDIT:     {          //  редактирование параметра
        if (updateScreen) {
          WinRamka (10, 105, 458, 72, VGA_BLACK, 1);
          WinRamka (10, 105, 458, 72, VGA_RED, 0);
          myGLCD.setColor(VGA_SILVER); 
          myGLCD.textRus(F("Ч    М    С "), 86, 150);
         // WinRamka (390, 172, 70, 52, VGA_LIME, 0);
          myGLCD.textRus(F("ОК "), 410, 132);
      #if defined(Set_Picture) && defined(Set_Touch)
          myGLCD.drawBitmap(278, 116, 96, 50, Graf5, 1);
      #endif

          Stat = 0;          
          Shms[0] = hh; Shms[1] = mm; Shms[2] = ss; Shms[3] = 0; 
          NewPos = hh;
          encoder.setPosition(NewPos);
          LastPos = NewPos;
          F_timK = true;
          updateScreen = false;
          }
        if (F_timK) {
        myGLCD.setFont(SevenSegNumFont);
        myGLCD.setColor(VGA_SILVER); 
        for (i = 0; i <= 3; i++) { 
          if (Stat == 3 && i == 3 )  {
            WinRamka (390, 114, 70, 52, VGA_RED, 0);
            continue; 
            }
            else WinRamka (390, 114, 70, 52, VGA_LIME, 0);
          
          if (Stat == i) myGLCD.setColor(VGA_ORAN); 
            else myGLCD.setColor(VGA_SILVER);
          if (i < 3) myGLCD.printNumI(Shms[i], 20+i*82, 116, 2, '0');  
          }
        F_timK = false;
        } 
      }
      break;    
    case REFLOW_STATE_PROFILE:      {          //  просмотр профиля
        if (updateScreen) {
          myGLCD.clrScr();
          PrPrint(0, 2);
          updateScreen = false;
          K_Setka();
          K_Kpres_ok = false;
          In_Line(20);        // end  updateScreen
        }
      }
      break;
    case REFLOW_STATE_PROFILE_LOAD:      {          //  загрузка профиля
        if (updateScreen) {
          WinRamka (10, 105, 460, 203, VGA_BLACK, 1);
          WinRamka (10, 105, 460, 203, VGA_RED, 0);
      #if defined(Set_Picture) && defined(Set_Touch)
          myGLCD.drawBitmap(377, 267, 96, 50, Graf5, 1);
      #endif
          myGLCD.setColor(VGA_LIME); 
          myGLCD.textRus((char*) Ad_tekct, 10, Y_shift);  // выводим текст названия параметра
         // myGLCD.textRus(myStr3[Vklad_G][0], 10, Y_shift); 
          NewPos = Obsi2;
          encoder.setPosition(NewPos);
          LastPos = NewPos;
          K_Kpres_ok = false;
          Stat = 1;
          Obsi2_Old = 15;
          updateScreen = false;
          }
        if (Obsi2_Old != Obsi2) {
          Obsi2_Old = Obsi2;

          if (Stat == 2) myGLCD.setColor(VGA_RED); 
             else myGLCD.setColor(VGA_SILVER);
          myGLCD.textRus(F("ОК "), 440, Y_shift);
          if (Stat == 1)  myGLCD.setColor(VGA_RED); 
             else myGLCD.setColor(VGA_SILVER);
          if (Obsi2 == 0)   myGLCD.textRus(F("ВСЕХ   "), 300, Y_shift+1);
            else {
              myGLCD.textRus(F("ИЗ       "), 300, Y_shift+1);
              i = 0;
              if (Obsi2 == 10) i = 16;
              myGLCD.printNumI(Obsi2, 354+i, Y_shift, 2, ' ');
             
              strcpy_P(buffer, (char *)pgm_read_word(&(AdrProfN[Obsi2-1])));  //  пишем в буфер
              DuStr= "";
              for (j=0; j<ArrMax; j++)  DuStr += buffer[j];
              DuStr.trim();   
              myGLCD.setColor(VGA_BLACK);
              myGLCD.fillRoundRect(15, 115 , 468, 131);
              myGLCD.setColor(VGA_ORAN);
              myGLCD.textRus(DuStr, 15, 116);
              myGLCD.setColor(VGA_LIME);
              myGLCD.textRus(F(" НИЗ   1     2     3     4 "), 15, 148);
              myGLCD.setColor(VGA_SILVER);
              for (j=2; j<=4; j++) {            
                tmp = pgm_read_byte_near(ProfDate[Obsi2-1]+j);
                if (j == 2) myGLCD.printNumF(tmp * 0.1, 0, 26, 185+(j-2)*33);
                  else myGLCD.printNumI(tmp, 26, 185+(j-2)*33, 3, ' ');
                }
              for (j=7; j<=18; j++) {
                i = (j+1)%4; int k = (j-7)/4;
                tmp = pgm_read_byte_near(ProfDate[Obsi2-1]+j);
                if (j >=7 && j <= 10) myGLCD.printNumF(tmp * 0.1, 0, 108 + i*96, 185);
                   else myGLCD.printNumI(tmp, 108 + i*96, 185+k*33, 3, ' ');
                }
            }
          }
        }
      break;
    case REFLOW_STATE_BRAZE_START:  {          // Старт пайки все начальные установки
        // Serial.println("я тут 1");
        if (updateScreen) {
          myGLCD.clrScr();
          PrPrint(0, 2);
          updateScreen = false;
          K_Setka();
          if (HeavyBottom) { HeavyStart = true;  tc2 = HeavyTemp; }
          In_Line(tc2);
        }
        //  фиксируем размер стола
        if (u.Profili.TableSize >= 1)   digitalWrite(P1_PIN, SetReleON);
        if (u.Profili.TableSize >= 2)   digitalWrite(P2_PIN, SetReleON);
        if (u.Profili.TableSize >= 3)   digitalWrite(P3_PIN, SetReleON);

        tone(buzzerPin, 1045, 500);  //звуковой сигнал при старте профиля
        ChangeState(REFLOW_STATE_BRAZE_NOW,0);
        TopStart = false;
        BottomStart = false; HeavyText = false;
        F_Alarm = false;
        mm = 0; ss = 0;       // секундомер в ноль
        BrStartTime = millis();
        BotStartTime = millis();
        Count1 = 0; Count2 = 0; BrStep = 0;
        BrTg = M_tg_Line(BrStep);
        //Serial.print("Tg - "); Serial.println(BrTg);
#ifdef SetInterrupt
        attachInterrupt(SetInterrupt, Dimming, RISING); // настроить порт прерывания(0 или 1) 2й или 3й цифровой пин
#else
        MsTimer2::set(10, Dimming); // 10 ms period
        MsTimer2::start();
#endif
        digitalWrite(buzzerPin, LOW);
        integra = 0;
        integra2 = 0;
        currentStep = 0;
        bottomMaxPwr = u.Profili.max_pwr_BOTTOM;
        
      }
      break;
    case REFLOW_STATE_BRAZE_NOW:    {          // процесс пайки графики
        if (millis() > nextRead1)     {          // график температуры nextRead1
          nextRead1 = millis() + SENSOR_SAMPLING_TIME;
          TempRead();    //  считываем температуру

          if (HeavyStart) {
            if (!HeavyText) {
              HeavyText = true;
              myGLCD.setColor(VGA_LIME);
              myGLCD.setFont(BigFontRus);
              myGLCD.textRus(F(" ПРЕДВАРИТЕЛЬНЫЙ НАГРЕВ НИЗА  "), 20, 302);
              }
            if (Input2 < HeavyTemp) {   // если температура платы меньше 
               bottomTemp = HeavyTemp;
               bottomMaxPwr = HeavyPower;
               TempBraze ();
              }
              else {                    // если больше выходим из режима преднгрева
                HeavyStart = false;
                bottomMaxPwr = u.Profili.max_pwr_BOTTOM; 
                tone(buzzerPin, 1105, 200);  //звуковой сигнал
               }
            }
            else {            // начало основного профиля
              if (BrStep <= BrStepMax) {            // если не конец профиля
                if (chast < 450) Count1++;
                chast = Count1 / 4;
                kluch =  Count1 % 4;
                /*
                             // #define TimeSt_M 0
                             // #define TempSt_M 1
                             // #define TimeEnd_M 2
                             // #define TempEnd_M 3
                */
                if (BrStep == 0) {                              // включение Нижнего подогрева
                  if (chast < Pr_Line_M[BrStep][TimeEnd_M]) {   // рост температуры
                    bottomTemp = Pr_Line_M[BrStep][TempSt_M] + BrTg * Count1 / 4;
                    }
                   else bottomTemp = Pr_Line_M[0][TempEnd_M];   // удержание
                   if (bottomTemp != 0 && !BottomStart) {
                      BottomStart = true;
                      myGLCD.setColor(VGA_LIME);
                      myGLCD.setFont(BigFontRus);
                      myGLCD.textRus(F(" НИЖНИЙ НАГРЕВАТЕЛЬ ВКЛЮЧЕН   "), 20, 302);
                    }
                  if (Input1 > Pr_Line_M[0][TempEnd_M] && !TopStart && BrStepMax != 0)  { // ПРЕДВАРИТЕЛЬНЫЙ НАГРЕВ ВЕРХА
                    Output1 = u.Profili.min_pwr_TOPStep[0];
                    if(!FlTone) {
                      FlTone = true;
                      myGLCD.setColor(VGA_LIME);
                      myGLCD.setFont(BigFontRus);
                      myGLCD.textRus(F(" ПРЕДВАРИТЕЛЬНЫЙ НАГРЕВ ВЕРХА    "), 20, 302);
                      tone(buzzerPin, 1105, 200);  //звуковой сигнал
                    }
                    }
                  if (chast > Pr_Line_M[BrStep + 2][TimeSt_M] && BrStepMax >= 2) { // пора ли включать верх
                    TopStart = true;
                    BrStep =+ 2;
                    currentStep = BrStep/2;
                    myGLCD.setColor(VGA_LIME);
                    myGLCD.setFont(BigFontRus);
                    myGLCD.textRus(F(" ВЕРХНИЙ НАГРЕВАТЕЛЬ ВКЛЮЧЕН   "), 20, 302);
                    BrTg = M_tg_Line(BrStep);
                    Count2 = 0;
                    tone(buzzerPin, 1105, 200);  //звуковой сигнал
                    return;
                  }
                  }
                 else {                                         // включение Верхнего нагревателя
                  if (chast <= Pr_Line_M[BrStep][TimeEnd_M] ) {
                    //  Serial.print(BrStep); Serial.print(" "); Serial.println(BrTg);
                    Count2++;    
                    Setpoint1 = Pr_Line_M[BrStep][TempSt_M] + BrTg * Count2 / 4; // рост температуры
                    }
                   else Setpoint1 = Pr_Line_M[BrStep][TempEnd_M];   // удержание
                   
                  if (chast > Pr_Line_M[BrStep + 1][TimeEnd_M]) { // пора ли переходить на следующий шаг
                    BrStep += 2;
                    currentStep = BrStep/2;
                    BrTg = M_tg_Line(BrStep);
                    Count2 = 0;
                    tone(buzzerPin, 1105, 200);  //звуковой сигнал
                  }
                }
                }  
                else  {
                    ChangeState(REFLOW_STATE_BRAZE_STOP,1);   // завершение пайки
                  }
               }
            if (millis() - BotStartTime < 3000) Output2 = 5; // первые 3 сек мощность низа =5%
              else  Output2 = Pid2(Input2, bottomTemp, u.Profili.kp2, u.Profili.ki2, u.Profili.kd2);
            if (TopStart == true) Output1 = Pid1(Input1, Setpoint1, u.Profili.kp1, u.Profili.ki1, u.Profili.kd1);

            // вывод информации на экран и в монитор РС
            if (kluch == 0) {      // выводим показания секундомера
              Sec_metr();
              myGLCD.setFont(BigFontRus);
              myGLCD.printNumI(currentStep, 2, 302, 1);
            }
            if (kluch == 1) {      // рисуем графики
              // WinStartTime = micros();
              TimeLine(chast + X_graf);             // линия времени
              myGLCD.setColor(VGA_RED);
              M_dr_point(chast - 2, bottomTemp);    // рисуем ожидаемый график низа
              if (TopStart == true) {
                myGLCD.setColor(VGA_ORAN);
                M_dr_point(chast - 2, Setpoint1);   // рисуем ожидаемый график верха
              }
              myGLCD.setColor(VGA_LIME);
              M_dr_point (chast - 2, tc2);           // рисуем график низ
              myGLCD.setColor(VGA_AQUA);
              M_dr_point (chast - 2, tc1);           // рисуем график верх
              TempBraze ();
              //  Pr_WinTime();    // на экран засеченное время в мсек
            }
            if (kluch == 2) {      // выводим в порт температуру
              myGLCD.setFont(BigFontRus);
              myGLCD.setColor(VGA_SILVER);
              myGLCD.textRus(F(": "), 432, TempY);
#ifdef SetConnectPC
              sprintf (buf, "OK%03d%03d%03d%03d%03d\r\n", int(Output1), int(Output2), tc1, tc2, int(profileName)); // график ПК
              Serial.print(buf);
#endif
            }
            if (kluch == 3) {      // работа защиты от отвала термопары 
                if (Obsi4 == 1) {   // проверка защиты низа разрешена
                  if (Gvard_If()) {
                    F_Alarm = true;
                    ChangeState(REFLOW_STATE_BRAZE_STOP,1);   // завершение пайки
                    }
                }
              }
        }
      }
      break;
    case REFLOW_STATE_BRAZE_STOP:   {           // STOP процесса пайки
        if (updateScreen) {
          digitalWrite(P1_PIN, SetReleOFF);
          digitalWrite(P2_PIN, SetReleOFF);
          digitalWrite(P3_PIN, SetReleOFF);
          digitalWrite(P4_PIN, SetReleOFF);
          TopStart = false;
          Output2 = 0;
          Output1 = 0;

#ifdef SetInterrupt
          detachInterrupt(SetInterrupt); // остановить порт прерывания(0 или 1) 2й или 3й цифровой пин
#else
          MsTimer2::stop();
#endif
          digitalWrite(RelayPin1, LOW);
          digitalWrite(RelayPin2, LOW);

          if (F_Alarm) {
              myGLCD.setColor(VGA_RED);
              myGLCD.setFont(BigFontRus);
              myGLCD.textRus(F("   ОШИБКА  СТОП ПРОФИЛЯ       "), 20, 302);
              }
            else {
              myGLCD.setColor(VGA_LIME);
              myGLCD.setFont(BigFontRus);
              myGLCD.textRus(F("         СТОП ПРОФИЛЯ           "), 5, 302);
              }


          tone(buzzerPin, 945, 500);  //звуковой сигнал
          delay(150);
          tone(buzzerPin, 645, 500);  //звуковой сигнал
          updateScreen = false;
        }
        if (millis() > nextRead1)     {          // график температуры nextRead1
          nextRead1 = millis() + SENSOR_SAMPLING_TIME;
          if (chast < 450) Count1++;
          chast = Count1 / 4;
          kluch =  Count1 % 4;
          TempRead();    //  считываем температуру
          if (kluch == 0) {
            myGLCD.setColor(VGA_LIME);
            M_dr_point (chast - 2, tc2);           // рисуем график низ
            myGLCD.setColor(VGA_AQUA);
            M_dr_point (chast - 2, tc1);           // рисуем график верх
            }
          TempBraze ();
        }
        break;
      }
  }
}  // end loop
