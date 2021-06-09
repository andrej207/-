case ILI9488:

	LCD_Write_COM(0x11);
	delay(20);

	LCD_Write_COM(0xF7);		// ?????
	LCD_Write_DATA(0xA9);
	LCD_Write_DATA(0x51);
	LCD_Write_DATA(0x2C);
	LCD_Write_DATA(0x82);

	LCD_Write_COM(0xC0);		// ?????
	LCD_Write_DATA(0x11);
	LCD_Write_DATA(0x09);

	LCD_Write_COM(0xC1);		// ?????
	LCD_Write_DATA(0x41);

	LCD_Write_COM(0xC5);		// ?????
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x0A);
	LCD_Write_DATA(0x80);

	LCD_Write_COM(0xB1);		// 
	LCD_Write_DATA(0xB0);
	LCD_Write_DATA(0x11);

	LCD_Write_COM(0xB4);		// 
	LCD_Write_DATA(0x02);
	
	LCD_Write_COM(0xB6);		// 
	LCD_Write_DATA(0x02);
	LCD_Write_DATA(0x42);

	LCD_Write_COM(0xB7);		// 
	LCD_Write_DATA(0xC6);
	
	LCD_Write_COM(0xBE);		// 
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x04);

	LCD_Write_COM(0xE9);		// 
	LCD_Write_DATA(0x00);
	
	LCD_Write_COM(0x36);		// 
	LCD_Write_DATA(0x08);
	
	LCD_Write_COM(0x3A);		// 
	LCD_Write_DATA(0x55);
	
	LCD_Write_COM(0xE0);			// Display Function Control
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x07);		// 0x42 = Rotate display 180 deg.
	LCD_Write_DATA(0x10);
	LCD_Write_DATA(0x09);
	LCD_Write_DATA(0x17);
	LCD_Write_DATA(0x0B);
	LCD_Write_DATA(0x41);
	LCD_Write_DATA(0x89);
	LCD_Write_DATA(0x4B);
	LCD_Write_DATA(0x0A);
	LCD_Write_DATA(0x0C);
	LCD_Write_DATA(0x0E);
	LCD_Write_DATA(0x18);
	LCD_Write_DATA(0x1B);
	LCD_Write_DATA(0x0F);

	LCD_Write_COM(0xE1);		// NGAMCTRL (Negative Gamma Control)
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x17);
	LCD_Write_DATA(0x1A);
	LCD_Write_DATA(0x04);
	LCD_Write_DATA(0x0E);
	LCD_Write_DATA(0x06);
	LCD_Write_DATA(0x2F);
	LCD_Write_DATA(0x45);
	LCD_Write_DATA(0x43);
	LCD_Write_DATA(0x02);
	LCD_Write_DATA(0x0A);
	LCD_Write_DATA(0x09);
	LCD_Write_DATA(0x32);
	LCD_Write_DATA(0x36);
	LCD_Write_DATA(0x0F); 

	LCD_Write_COM(0x11);		// Display Inversion OFF
	LCD_Write_DATA(0x00);		//C8 	 

	delay(120);
	
	LCD_Write_COM(0x29);		// Display ON
	LCD_Write_COM(0x00);		// Memory Write
	break;
