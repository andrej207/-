// *** Hardwarespecific functions ***
void UTFT::_hw_special_init()
{
}
#if defined(USE_UNO_SHIELD_ON_MEGA)

	#define EMASK         0x38
	#define GMASK         0x20
	#define HMASK         0x78
void write_8(char x) {
	PORTH &= ~HMASK; PORTG &= ~GMASK; PORTE &= ~EMASK; \
	PORTH |= (((x) & (3<<0)) << 5); 
	PORTE |= (((x) & (3<<2)) << 2); 
	PORTG |= (((x) & (1<<4)) << 1); 
	PORTE |= (((x) & (1<<5)) >> 2); 
	PORTH |= (((x) & (3<<6)) >> 3); 
 };
#endif	
					 
void UTFT::LCD_Writ_Bus(uint8_t VH,uint8_t VL, byte mode)
{   
	switch (mode) 
	{
	case 1: {
		if (display_serial_mode==SERIAL_4PIN)
		{
		if (VH==1)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		}
		else
		{
		if (VH==1)
			sbi(P_RS, B_RS);
		else
			cbi(P_RS, B_RS);
		}

		if (VL & 0x80)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x40)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x20)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x10)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x08)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x04)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x02)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
		if (VL & 0x01)
			sbi(P_SDA, B_SDA);
		else
			cbi(P_SDA, B_SDA);
		pulse_low(P_SCL, B_SCL);
	break; }
	case 8: {
#if defined(USE_UNO_SHIELD_ON_MEGA)
		write_8(VH);
		pulse_low(P_WR, B_WR);
		write_8(VL);
		pulse_low(P_WR, B_WR);
#else
	//Serial.println(" Bit8");	
	PORTA = VH;
		pulse_low(P_WR, B_WR);
		PORTA = VL;
		pulse_low(P_WR, B_WR);
#endif
	break; }
	case 16: {
		PORTA = VH;
		PORTC = VL;
		pulse_low(P_WR, B_WR);
	break; }
	case LATCHED_16:  {
		PORTG &= ~0x20;	
		PORTG |= (VH & 0x10)<<1;	
		PORTH &= ~0x18;	
		PORTH |= (VH & 0xC0)>>3;	
		PORTE &= ~0x3B;	
		PORTE |= (VH & 0x03) + ((VH & 0x0C)<<2) + ((VH & 0x20)>>2);
		cbi(P_ALE, B_ALE);
		pulse_high(P_ALE, B_ALE);
		cbi(P_CS, B_CS);
		PORTG &= ~0x20;	
		PORTG |= (VL & 0x10)<<1;	
		PORTH &= ~0x18;	
		PORTH |= (VL & 0xC0)>>3;	
		PORTE &= ~0x3B;	
		PORTE |= (VL & 0x03) + ((VL & 0x0C)<<2) + ((VL & 0x20)>>2);
		pulse_low(P_WR, B_WR);
		sbi(P_CS, B_CS);
	break; }
	}
}

void UTFT::LCD_Write_Bus_8(uint8_t V8)
{
#if defined(USE_UNO_SHIELD_ON_MEGA)
	write_8(V8);
	pulse_low(P_WR, B_WR);
#else
	#if(USE_8BIT_ON_MEGA==0)
	  PORTA = V8;
     #elif (USE_8BIT_ON_MEGA==1)	
	  PORTC = V8;
	#endif
	pulse_low(P_WR, B_WR);
#endif
}

void UTFT::_set_direction_registers(byte mode)
{
#if defined(USE_UNO_SHIELD_ON_MEGA)
	DDRH |=  HMASK; DDRG |=  GMASK; DDRE |=  EMASK; 
#else
	if (mode!=LATCHED_16)
	{
		#if(USE_8BIT_ON_MEGA==0)
			DDRA = 0xFF;
			if (mode==16)
				DDRC = 0xFF;
		#elif (USE_8BIT_ON_MEGA==1)
			DDRC = 0xFF; 
		#endif
	}
	else
	{
		DDRH = 0x78;
		DDRG = 0x20;
		DDRE = 0x38;
	}
#endif
}

void UTFT::_fast_fill_16(int ch, int cl, long pix)
{
#if defined(USE_UNO_SHIELD_ON_MEGA)
	if (ch==cl)
		_fast_fill_8(ch, pix);
	else
	{
		for (int i=0; i<pix; i++)
		{
			write_8(ch);
			pulse_low(P_WR, B_WR);
			write_8(cl);
			pulse_low(P_WR, B_WR);
		}
	}
#else
	long blocks;

	PORTA = ch;
	PORTC = cl;

	blocks = pix/16;
	for (int i=0; i<blocks; i++)
	{
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);
	}
	if ((pix % 16) != 0)
		for (int i=0; i<(pix % 16)+1; i++)
		{
			pulse_low(P_WR, B_WR);
		}
#endif
}

void UTFT::_fast_fill_8(int ch, long pix)
{
	long blocks;

#if defined(USE_UNO_SHIELD_ON_MEGA)
	write_8(ch);
#else
	#if(USE_8BIT_ON_MEGA==0)
		PORTA = ch;
	#elif (USE_8BIT_ON_MEGA==1)
		PORTC = ch;
	#endif
	

#endif

	blocks = pix/16;
	for (int i=0; i<blocks; i++)
	{
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
	}
	if ((pix % 16) != 0)
		for (int i=0; i<(pix % 16)+1; i++)
		{
			pulse_low(P_WR, B_WR);pulse_low(P_WR, B_WR);
		}
}
