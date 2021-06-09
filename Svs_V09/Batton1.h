 //--------настройка кнопок управления------------------------------------

#ifdef SetDigitBatton
  #define Batt_UP A11
  #define Batt_DWN A12
  #define Batt_LFT A13
  #define Batt_RHT A14
  #define Batt_MID A15
  Cl_do_btn_long Btn_UP(Batt_UP);
  Cl_do_btn_long Btn_DWN(Batt_DWN);
  Cl_do_btn_long Btn_LFT(Batt_LFT);
  Cl_do_btn_long Btn_RHT(Batt_RHT);
  Cl_do_btn_long Btn_MID(Batt_MID);
  // --------------------------------------------
  void longPr_MID () {
    return;
  }
#endif

  boolean T_KeyUp_Old = false, T_KeyDown_Old = false;
  boolean T_KeyNext = false, T_KeyOk = false;
  boolean T_KeyLeft_Old = false, T_KeyRight_Old = false;
  boolean K_Kpres_ok = false, K_Lpres_ok = false;

#ifdef SetAnalogBatton
  AnalogKey5 AKeys(SetRIGHT, SetUP, SetDOWN, SetLEFT, SetSELECT, 10);
  boolean Key1_Old, Key2_Old, Key3_Old, Key4_Old, Key5_Old;
#endif
