UTFT
===========

Библиотека для Arduino, позволяющая выводить текст и красочную графику на TFT-дисплеи с разрешением [240x320](http://amperka.ru/product/tft-color-display-320x240) и [480x320](http://amperka.ru/product/tft-color-display-480x320) с помощью Arduino без необходимости задумываться о деталях протокола.

Установка
=========

Скачайте последний релиз библиотеки:

<a class="btn btn-sm btn-primary" href="https://github.com/amperka/utft/releases/download/utft.zip">Скачать библиотеку UTFT </a>

В Arduino IDE выберите пункт меню «Скетч» → «Импортировать библиотеку» →
«Добавить библиотеку…». В появившемся окне выберите скачаный архив с
библиотекой. Установка завершена.


Изменения от Watashi:

Для работы с дисплеем ILI9488 на 8 бит http://www.lcdwiki.com/3.95inch_Arduino_Display-Mega2560
который использует для обмена данными порт С (а не А как обычно бывает) надо а файле
 "C:\Users\WatashiPC\Documents\Arduino\libraries\UTFT_Rus_281\hardware\avr\HW_AVR_defines.h"
 строки привести к такому виду
 
  #define USE_8BIT_ON_MEGA 1		// use 8 bit bas
// #define USE_8BIT_ON_MEGA 0		// use 16 bit bas

Вызов такой: UTFT myGLCD(ILI9488_8, 38, 39, 40, 41);