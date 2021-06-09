// ---------Константы настройки профиля--------
        const char str_0[] PROGMEM = "НИЖНИЙ ";
        const char str_1[] PROGMEM = "ВЕРХ ";
        const char str_2[] PROGMEM = "ПИД ";
        const char str_3[] PROGMEM = "ОБЩИЕ ";
        const char* myStr2[4] PROGMEM = {str_0, str_1, str_2, str_3}; // адреса констант-заголовков меню
        const char stp_1[] PROGMEM = "РАЗМЕР НИЗА ";
        const char stp_2[] PROGMEM = "СК-ТЬ НАГРЕВА ";
        const char stp_3[] PROGMEM = "ТЕМПЕРАТУРА ";
        const char stp_4[] PROGMEM = "ВРЕМЯ ЗАДЕРЖКИ ";
        const char stp_5[] PROGMEM = "МОЩНОСТЬ МИН ";
        const char stp_6[] PROGMEM = "МОЩНОСТЬ МАХ ";
        const char stp_7[] PROGMEM = "ШАГ ПРОФИЛЯ  < > ";
        const char stp_8[] PROGMEM = "СК-ТЬ НАГРЕВА ";
        const char stp_9[] PROGMEM = "ВРЕМЯ ПЕРЕХОДА ";
        const char stp_10[] PROGMEM = "ВЕРХ ПРОПОРЦИОНАЛ ";
        const char stp_11[] PROGMEM = "    ИНТЕГРАЛ ";
        const char stp_12[] PROGMEM = "    ДИФФЕРЕНЦИАЛ ";
        const char stp_13[] PROGMEM = "НИЗ ПРОПОРЦИОНАЛ ";
        const char stp_14[] PROGMEM = "ЗАЩИТА НИЗ ";
        const char stp_15[] PROGMEM = "ЗВУКИ ";
        const char stp_16[] PROGMEM = "УСТАНОВКА ЧАСОВ  ";
        const char stp_17[] PROGMEM = "ЗАГРУЗКА ПРОФИЛЯ ";
        const char stp_18[] PROGMEM = " -  ";
        const char* const myStr3[4][6] PROGMEM = {              // адреса констант-надписей меню
          { stp_1, stp_2, stp_3, stp_4, stp_5, stp_6 },
          { stp_7, stp_8, stp_3, stp_9, stp_5, stp_6 },
          { stp_10, stp_11, stp_12, stp_13, stp_11, stp_12 },
          { stp_16, stp_17, stp_15, stp_14, stp_18, stp_18 }
        };
        void *Adresa[4][6] = {              // адреса переменных для меню настроек
          {&u.Profili.TableSize, &u.Profili.rampRateBOTTOM, &u.Profili.temperatureBOTTOM, &u.Profili.dwellTimerBOTTOM, &u.Profili.min_pwr_BOTTOM, &u.Profili.max_pwr_BOTTOM },
          {&Shag, &u.Profili.rampRateStep[0], &u.Profili.temperatureStep[0], &u.Profili.dwellTimerStep[0], &u.Profili.min_pwr_TOPStep[0], &u.Profili.max_pwr_TOPStep[0]},
          {&u.Profili.kp1, &u.Profili.ki1, &u.Profili.kd1, &u.Profili.kp2, &u.Profili.ki2, &u.Profili.kd2},
          {&Obsi1, &Obsi2, &Obsi3, &Obsi4, &Obsi5, &Obsi6}
        };
        bool Cln_st = false;
