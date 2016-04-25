
/******************************************************************************/
/* LCD Functions - Modified from code written by Rajendra Bhatt               */
/*                 @ http://embedded-lab.com/blog/why-pay-for-serial-lcds-when-you-can-make-your-own/ */
/******************************************************************************/

void write_LCD_Nibble(unsigned short N)
{
    unsigned short Mask, i, Flag;
    
	LCD_Enable_Pin = 0;
	// ****** Write RS *********
	LCD_Clk_Pin = 0;
	LCD_Data_Pin = RS;
	LCD_Clk_Pin = 1;
	LCD_Clk_Pin = 0;
	// ****** End RS Write

	// Shift in 4 more bits to shift register with Bit-Banging
	Mask = 8; // 0b1000
	for (i=0; i<4; i++)
	{
		Flag = N & Mask;
		if(Flag==0)
            LCD_Data_Pin = 0;
		else
            LCD_Data_Pin = 1;
		LCD_Clk_Pin = 1;
		LCD_Clk_Pin = 0;
		Mask = Mask >> 1;
	}
    
	// One more clock edge because SC and ST clocks are tied on shift register.
	LCD_Clk_Pin = 1;
	LCD_Clk_Pin = 0;
	LCD_Data_Pin = 0;
    
    // Send parallel outputs of shift register to inputs of LCD.
	LCD_Enable_Pin = 1;
        __delay_ms(2);      // Debugging: Try 1-2ms. Depends on max write frequency of LCD Display. //charliemark
	LCD_Enable_Pin = 0;
}

void write_LCD_Data(unsigned short D)
{
	RS = 1; // It is Data, not command
	Low_Nibble = D & 15; // 15 == 0b0000_1111
	High_Nibble = D/16; // Right shift 4 bits.
	write_LCD_Nibble(High_Nibble);
	write_LCD_Nibble(Low_Nibble);
}

void write_LCD_Cmd(unsigned short C)
{
	RS = 0; // It is command, not data
	Low_Nibble = C & 15; // 15 == 0b0000_1111
	High_Nibble = C/16; // Right shift 4 bits.
	write_LCD_Nibble(High_Nibble);
	write_LCD_Nibble(Low_Nibble);
}

void initialize_LCD()
{
    LCD_Clk_PinDIR = 0; // LCD Clock Pin = Output
    LCD_Data_PinDIR = 0; // LCD Data Pin = Output
    LCD_Enable_PinDIR = 0; // LCD Enable Pin = Output
    
	__delay_ms(50);
	write_LCD_Nibble(0x03); // Wake-Up Sequence
	__delay_ms(50);
	write_LCD_Nibble(0x03);
	__delay_ms(50);
	write_LCD_Nibble(0x03);
	__delay_ms(50);
    write_LCD_Nibble(0x02);
	__delay_ms(50);
	write_LCD_Cmd(0x28); // 4-bits, 2 lines, 5x7 font
	__delay_ms(50);
    write_LCD_Cmd(0x01); // Clear
	__delay_ms(50);
	write_LCD_Cmd(0x0C); // Display ON, No cursors
	__delay_ms(50);
	write_LCD_Cmd(0x06); // Entry mode- Auto-increment, No Display shifting
	__delay_ms(50);
}

void position_LCD(unsigned short x, unsigned short y) // Row, Column starting from 1.
{
    unsigned short temp;
	temp = 127 + y; // 127 = 0b0111_1111
	if (x == 2) temp = temp + 64; // 64 = 0b0100_0000 for second row of LCD
	write_LCD_Cmd(temp);
}

void write_LCD_Text(char *StrData)
{
    static unsigned short i, length, temp;
	length = strlen(StrData);
	
	for (i = 0; i<length; i++)
	{
		temp = StrData[i];
		write_LCD_Data(temp);
	}
}

void clear_LCD()
{
    write_LCD_Cmd(0x01);
}



/******************************************************************************/
/* RGB Functions - Modified from code written by Limor Fried and Ladyada      */
/*                 @ Adafruit Industries                                      */
/******************************************************************************/

void initialize_RGB()
{
    RGB_Data_PinDIR = 0; // RGB Data Pin = Output
    RGB_Clk_PinDIR = 0; // RGB Clock Pin = Output
    RGB_Lat_PinDIR = 0; // RGB Lat Pin = Output
    
    clear_RGB();
}

void write_RGB()
{
    int8_t c, b; // c = channel, b = bit
    
    RGB_Lat_Pin = 0;
    
    //24 channels, [23:0]
    for (c=24-1; c >= 0 ; c--)
    {
        // 12 bits per channel, send MSB first
        for (b=11; b>=0; b--)
        {
            RGB_Clk_Pin = 0;
            
            if (pwmbuffer[c] & (1 << b))
                RGB_Data_Pin = 1;
            else
                RGB_Data_Pin = 0;
            
            RGB_Clk_Pin = 1;
        }
    }
    RGB_Clk_Pin = 0;
    
    RGB_Lat_Pin = 1;
    RGB_Lat_Pin = 0;
}

void write_RGB_Animated()
{
    int8_t c, b; // c = channel, b = bit
    
    //clear_RGB(); // DON'T CALL! Clears pwmbuffer[] to all zeros.
    RGB_Lat_Pin = 0;
    
    //24 channels, [23:0]
    for (c=24-1; c >= 0 ; c--)
    {
        // 12 bits per channel, send MSB first
        for (b=11; b>=0; b--)
        {
            RGB_Clk_Pin = 0;
            
            if (pwmbuffer[c] & (1 << b))
                RGB_Data_Pin = 1;
            else
                RGB_Data_Pin = 0;
            __delay_ms(1);
            RGB_Clk_Pin = 1;
            __delay_ms(1);
        }
        if(c%3 == 0)
        {
            RGB_Lat_Pin = 1;
            RGB_Lat_Pin = 0;
            __delay_ms(5); // Debgging: Set animation speed of LED Array.//charliemark
        }
    }
    RGB_Clk_Pin = 0;
    
    RGB_Lat_Pin = 1;
    RGB_Lat_Pin = 0;
}

void setPWMOneColor(uint16_t Rpwm, uint16_t Gpwm, uint16_t Bpwm)
{
    uint8_t i;
    for(i=0; i<8; ++i)
    {
        pwmbuffer[i*3] = Rpwm;
        pwmbuffer[i*3 + 1] = Gpwm;
        pwmbuffer[i*3 + 2] = Bpwm;
    }
}




void clear_RGB()
{
    setPWMOneColor(0, 0, 0);
    write_RGB();
}



/******************************************************************************/
/* ADC Functions                                                           */
/******************************************************************************/

void initialize_ADC()
{
    AD1CON1bits.ADON = 0; // ADC OFF
    AD1CON1bits.ADSIDL = 0; // continue module operation in idle mode
    AD1CON1bits.AD12B = 1; // 12-bit, 1-channel ADC operation
    AD1CON1bits.FORM = 0b00; // unsigned integer format
    AD1CON1bits.SSRC = 0b111; // auto-convert
    AD1CON1bits.ASAM = 0; // Sampling begins when SAMP bit is set
    
    AD1CON2bits.VCFG = 0b000; // Voltage Reference = AVDD & AVSS
    AD1CON2bits.CSCNA = 0; // Do not scan inputs
    
    AD1CON3bits.ADRC = 1; // ADC internal RC clock
    AD1CON3bits.SAMC = 0b11111; //auto sample time bits
    
    AD1CHS0bits.CH0NA = 0; //negative input is VREF-
    AD1CHS0bits.CH0NB = 0; //negative input is VREF-

    KeyInL_AN = 0; // LeftKey analog mode
    KeyInR_AN = 0; // Right key analog mode
    
    AD1CON1bits.ADON = 1; // Turn ADC ON
    
    //Configure ADC conversion interrupts 
    //_AD1IF = 0; //ad1if is interrupt flag register of "adc conversion complete?" 
    //_AD1IE = 1; //enable interrupt when adc conversion is complete 
    //_AD1IP = 6; //set priority of adc conversion interrupt to 6 (7 is max) 
    
    // Key Input Pins
    KeyInLDIR = 1; //set left key direction to input
    KeyInRDIR = 1;
    
  
}

void poll_ADC() 
{
    AD1CHS0bits.CH0SA = 0; // Select analog input pin //charliemark
    AD1CON1bits.SAMP = 1; //begins sampling on channel defined above
    while (!AD1CON1bits.DONE); //waits until adc stabilized
    leftADCvalue = ADC1BUF0; //adc1buf is what the adc stabilizes on range -> [0,4095]
    setKey(1, leftADCvalue); // Set left key

    //DEBUGGING
//    __delay_ms(1000);
//    char array[50];
//    sprintf(array,"the left value is: '%i'\r\n",leftADCvalue);
//    UART_SEND(array);

    AD1CHS0bits.CH0SA = 1; // Select analog input pin//charliemark 
    AD1CON1bits.SAMP = 1;//begins sampling on channel defined above
    while (!AD1CON1bits.DONE); // Removed when handled with interrupts
    rightADCvalue = ADC1BUF0; //adc1buf is what the adc stabilizes on range -> [0,4095]
    setKey(2, rightADCvalue); // Set right key

    //DEBUGGING
//    __delay_ms(1000);
//
//    sprintf(array,"the right value is: '%i'\r\n",rightADCvalue);
//    UART_SEND(array);
}

/*void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void) //interrupts when adc is done converting
{
    ADCvalue = ADC1BUF0;
    setKey();
    _AD1IF = 0; //clear the interrupt flag
}*/

void setKey(uint8_t lock, uint16_t ADCvalue)
{
    
    if (ADCvalue < 1100) // Closer to max V than min
    {
        // Sky Key
        if (lock == 1)
            leftKey = 1;
        else if (lock == 2)
            rightKey = 1;
    }
    else if (ADCvalue < 2000)
    {
        // Aqua Key
        if (lock == 1)
            leftKey = 2;
        else if (lock == 2)
            rightKey = 2;
    }
    else if (ADCvalue < 2800)
    {
        // Flame Key
        if (lock == 1)
            leftKey = 3;
        else if (lock == 2)
            rightKey = 3;
    }
    else if (ADCvalue < 3600)
    {
        // Arc Key
        if (lock == 1)
            leftKey = 4;
        else if (lock == 2)
            rightKey = 4;
    }
    else
    {
        // No Key
        if (lock == 1)
            leftKey = 0;
        else if (lock == 2)
            rightKey = 0;
    }
    setWorld();
}

void setWorld()
{
    if ((leftKey == 1 && rightKey == 2) || (leftKey == 2 && rightKey == 1))
    {
        currentWorld = 5; // ICE WORLD
       
    }
    else if ((leftKey == 3 && rightKey == 4) || (leftKey == 4 && rightKey == 3))
    {
        currentWorld = 6; // PLASMA WORLD
       
    }
    else if ((leftKey == 2 && rightKey == 3) || (leftKey == 3 && rightKey == 2))
    {
        currentWorld = 7; // NON COMPATIBLE water + fire

    }
    else if ((leftKey == 4 && rightKey == 2) || (leftKey == 2 && rightKey == 4))
    {
        currentWorld = 8; // NON COMPATIBLE arc + water

    }
    else if ((leftKey == 4 && rightKey == 1) || (leftKey == 1 && rightKey == 4))
    {
        currentWorld = 9; // NON COMPATIBLE arc + sky

    }
    else if ((leftKey == 3 && rightKey == 1) || (leftKey == 1 && rightKey == 3))
    {
        currentWorld = 10; // NON COMPATIBLE flame + sky

    }
    else if (leftKey == 1 || rightKey == 1)
    {
        currentWorld = 1; // WIND WORLD
        
    }
    else if (leftKey == 2 || rightKey == 2)
    {
        currentWorld = 2; // WATER WORLD
    
    }
    else if (leftKey == 3 || rightKey == 3)
    {
        currentWorld = 3; // FIRE WORLD
        
    }
    else if (leftKey == 4 || rightKey == 4)
    {
        currentWorld = 4; // LIGHTNING WORLD
        
    }
    else
    {
        currentWorld = 0; //PORTALS CLOSED
     
    }
}



/******************************************************************************/
/* World Visual Functions                                                      */
/******************************************************************************/

void noWorldVisuals()
{
    clear_LCD();
    position_LCD(1,1);
    write_LCD_Text("Insert keys");
	position_LCD(2,1);
	write_LCD_Text("Portals closed");
    clear_RGB();
}

void windWorldVisuals()
{
    clear_LCD();
    position_LCD(1,1);
    write_LCD_Text("Sky Key");
	position_LCD(2,1);
	write_LCD_Text("World: Wind");
    clear_RGB();
    setPWMOneColor(0,700,2000);
    write_RGB_Animated();
}

void waterWorldVisuals()
{
    clear_LCD();
    position_LCD(1,1);
    write_LCD_Text("Aqua Key");
	position_LCD(2,1);
	write_LCD_Text("World: Water");
    clear_RGB();
    setPWMOneColor(50,0,2048);
    write_RGB_Animated();
}

void fireWorldVisuals()
{
    clear_LCD();
    position_LCD(1,1);
    write_LCD_Text("Flame Key");
	position_LCD(2,1);
	write_LCD_Text("World: Fire");
    clear_RGB();
    setPWMOneColor(3000,0,0);
    write_RGB_Animated();
}

void lightningWorldVisuals()
{
    clear_LCD();
    position_LCD(1,1);
    write_LCD_Text("Arc Key");
	position_LCD(2,1);
	write_LCD_Text("World: Lightning");
    clear_RGB();
    setPWMOneColor(2048,2048,0);
    write_RGB_Animated();
}

void iceWorldVisuals()
{
    clear_LCD();
    position_LCD(1,1);
    write_LCD_Text("Sky & Aqua Keys");
	position_LCD(2,1);
	write_LCD_Text("World: Ice");
    clear_RGB();
    setPWMOneColor(1000,1000,1000);
    write_RGB_Animated();
}

void plasmaWorldVisuals()
{
    clear_LCD();
    position_LCD(1,1);
    write_LCD_Text("Flame & Arc Keys");
	position_LCD(2,1);
	write_LCD_Text("World: Plasma");
    clear_RGB();
    setPWMOneColor(1000,0,1000);
    write_RGB_Animated();
}

