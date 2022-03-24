int i;
UINT8 processBuf[16];

//counted in 1Hz
//0 means disable
UINT32 Freq0;
UINT32 Freq1;
UINT32 Freq2;

UINT32 ACTUALPLL1;
UINT32 ACTUALPLL2;

UINT32 ACTUALFREQ0;
UINT32 ACTUALFREQ1;
UINT32 ACTUALFREQ2;

UINT8 checkData(){
	UINT8 CHK;
	int i;
	
	if(processBuf[0]!=0xAA)return 0;
	if(processBuf[15]!=0x55)return 0;
	
	CHK = 0;
	for(i=0;i<14;i++)CHK ^= processBuf[i];
	if(CHK!=0)return 0;
	
	for(i=0;i<15;i++)CHK += processBuf[i];
	if(CHK!=0)return 0;
	
	Freq0 = processBuf[1]; Freq0 <<= 8; Freq0+= processBuf[2]; Freq0<<=8; Freq0+= processBuf[3]; Freq0<<=8; Freq0+=processBuf[4];
	Freq1 = processBuf[5]; Freq1 <<= 8; Freq1+= processBuf[6]; Freq1<<=8; Freq1+= processBuf[7]; Freq1<<=8; Freq1+=processBuf[8];
	Freq2 = processBuf[9]; Freq2 <<= 8; Freq2+= processBuf[10]; Freq2<<=8; Freq2+= processBuf[11]; Freq2<<=8; Freq2+=processBuf[12];
	
	i=0;
	if(Freq0!=0)i++;
	if(Freq1!=0)i++;
	if(Freq2!=0)i++;
	if(i==3){
		return 0;
	}
	
	return 1;
}


void genData(){
	UINT8 CHK;
	int i;
	
	SendBuf[0]=0xAA;
	SendBuf[15]=0x55;
	
	SendBuf[1]=(ACTUALFREQ0>>24)&0xFF; SendBuf[2]=(ACTUALFREQ0>>16)&0xFF; SendBuf[3]=(ACTUALFREQ0>>8)&0xFF; SendBuf[4]=ACTUALFREQ0&0xFF;
	SendBuf[5]=(ACTUALFREQ1>>24)&0xFF; SendBuf[6]=(ACTUALFREQ1>>16)&0xFF; SendBuf[7]=(ACTUALFREQ1>>8)&0xFF; SendBuf[8]=ACTUALFREQ1&0xFF;
	SendBuf[9]=(ACTUALFREQ2>>24)&0xFF; SendBuf[10]=(ACTUALFREQ2>>16)&0xFF; SendBuf[11]=(ACTUALFREQ2>>8)&0xFF; SendBuf[12]=ACTUALFREQ2&0xFF;
	
	CHK = 0;
	for(i=0;i<13;i++)CHK ^= SendBuf[i];
	SendBuf[13]=CHK;
	
	CHK = 0;
	for(i=0;i<14;i++)CHK += SendBuf[i];
	SendBuf[14]=0-CHK;
}

UINT8 OUT0EN;
UINT8 OUT1EN;
UINT8 OUT2EN;

UINT8 DIV0SRC;
UINT8 DIV1SRC;
UINT8 DIV2SRC;

UINT32 PLL1P1;
UINT32 PLL1P2;
UINT32 PLL1P3;

UINT32 PLL2P1;
UINT32 PLL2P2;
UINT32 PLL2P3;

UINT8 OUT0DIV;
UINT8 OUT1DIV;
UINT8 OUT2DIV;

UINT32 DIV0P1;
UINT32 DIV1P1;
UINT32 DIV2P1;

//IF DIV BY 4, P1=0, DIVBY4 = 11b

UINT8 i2crecv;

code UINT8 addrs[28]={3   ,24  ,16  ,17  ,18  ,42  ,43  ,47  ,48  ,49  ,50  ,51  ,55  ,56  ,57  ,58  ,59  ,63  ,64  ,65  ,149 ,150 ,161 ,165 ,166 ,167 ,183 ,187 };
code UINT8 cmds[28]= {0x07,0x2A,0xCF,0xCF,0xCF,0x00,0x01,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0xD2,0x00};

void init5351(){
	UINT8 iinit5351;
	
	i2crecv=0x80;
	do{
		I2CRead(0,&i2crecv);
	}while(i2crecv&0x80);
	
	for(iinit5351=0;iinit5351<28;iinit5351++)I2CWrite(addrs[iinit5351],cmds[iinit5351]);
	/*
	I2CWrite( 3,0x07);//disable output
	I2CWrite(24,0x2A);//higi res when disable
	
	I2CWrite(16,0xCF);//disable div0
	I2CWrite(17,0xCF);//disable div1
	I2CWrite(18,0xCF);//disable div2
	
	//set div frac to 0/1
	//DIV0
	I2CWrite(42,0x00);
	I2CWrite(43,0x01);
	I2CWrite(47,0x00);
	I2CWrite(48,0x00);
	I2CWrite(49,0x00);
	//DIV1
	I2CWrite(50,0x00);
	I2CWrite(51,0x01);
	I2CWrite(55,0x00);
	I2CWrite(56,0x00);
	I2CWrite(57,0x00);
	//DIV2
	I2CWrite(58,0x00);
	I2CWrite(59,0x01);
	I2CWrite(63,0x00);
	I2CWrite(64,0x00);
	I2CWrite(65,0x00);
	
	//disable SS
	I2CWrite(149,0x00);
	I2CWrite(150,0x01);
	I2CWrite(161,0x00);
	
	//disble phrase offset
	I2CWrite(165,0x00);
	I2CWrite(166,0x00);
	I2CWrite(167,0x00);
	
	//set xtal to 10pF
	I2CWrite(183,0xD2);
	
	//output ctrl
	I2CWrite(187,0x00);
	*/
}

UINT32 PLL1;
UINT32 PLL2;
UINT32 Temp0;

void calculatecfg(){
	PLL1=600000000;
	PLL2=600000000;


	if(Freq0 != 0) OUT0EN = 1;
	else OUT0EN = 0;
	if(Freq1 != 0) OUT1EN = 1;
	else OUT1EN = 0;
	if(Freq2 != 0) OUT2EN = 1;
	else OUT2EN = 0;

	//do not allow output 3 clocks
	
	if(OUT0EN){
		DIV0SRC=0;
		DIV1SRC=1;
		DIV2SRC=1;
	}else{
		DIV1SRC=0;
		DIV2SRC=1;
	}

	if(OUT0EN){
		OUT0DIV=0;
		if(Freq0>150000000){
			DIV0P1=0;
			if(DIV0SRC){
				PLL2=Freq0*4;
			}else
				PLL1=Freq0*4;
		}else{
			Temp0=Freq0;
			while(OUT0DIV!=7 && Temp0<600000000/2048){
				OUT0DIV++;
				Temp0=Temp0*2;
			}
			for(DIV0P1=4;DIV0P1<2050 && Temp0<600000000/DIV0P1;DIV0P1+=2);
			if(DIV0SRC){
				PLL2=Temp0*DIV0P1;
			}else
				PLL1=Temp0*DIV0P1;
			DIV0P1=(DIV0P1-4)*128;
		}
	}
	if(OUT1EN){
		OUT1DIV=0;
		if(Freq1>150000000){
			DIV1P1=0;
			if(DIV1SRC){
				PLL2=Freq1*4;
			}else
				PLL1=Freq1*4;
		}else{
			Temp0=Freq1;
			while(OUT1DIV!=7 && Temp0<600000000/2048){
				OUT1DIV++;
				Temp0=Temp0*2;
			}
			for(DIV1P1=4;DIV1P1<2050 && Temp0<600000000/DIV1P1;DIV1P1+=2);
			if(DIV1SRC){
				PLL2=Temp0*DIV1P1;
			}else
				PLL1=Temp0*DIV1P1;
			DIV1P1=(DIV1P1-4)*128;
		}
	}
	if(OUT2EN){
		OUT2DIV=0;
		if(Freq2>150000000){
			DIV2P1=0;
			if(DIV2SRC){
				PLL2=Freq2*4;
			}else
				PLL1=Freq2*4;
		}else{
			Temp0=Freq2;
			while(OUT2DIV!=7 && Temp0<600000000/2048){
				OUT2DIV++;
				Temp0=Temp0*2;
			}
			for(DIV2P1=4;DIV2P1<2050 && Temp0<600000000/DIV2P1;DIV2P1+=2);
			if(DIV2SRC){
				PLL2=Temp0*DIV2P1;
			}else
				PLL1=Temp0*DIV2P1;
			DIV2P1=(DIV2P1-4)*128;
		}
	}
	//PLL=25M*(4+(p1+p2/p3)/128)
	//128*PLL = 25M*(512 + p1+p2/p3)
	//PLL = (25M / 128)*(512 + P1+P2/P3)
	//PLL - (25M/128*512) = (P1+P2/P3)*(25M/128)
	//25M/128*512=100000000
	//PLL - 100000000 = (P1+P2/P3)*(25M/128)
	//25M * 2 / 128 = 390625
	//2*(PLL - 100000000) = (P1+P2/P3)*390625
	//K = 2*(PLL - 100000000)
	//P1 = K / 390625
	//P3 = 1024
	//P2 = ((K - P1*390625)*P3)/390625

	if(PLL1<600000000)PLL1=600000000;
	if(PLL1>900000000)PLL1=900000000;

	if(PLL2<600000000)PLL2=600000000;
	if(PLL2>900000000)PLL2=900000000;

	PLL1P3=1024;
	PLL1 = 2*(PLL1 - 100000000);
	PLL1P1 = PLL1 / 390625;
	PLL1P2 = ((PLL1 - PLL1P1*390625)*PLL1P3)/390625;

	PLL2P3=1024;
	PLL2 = 2*(PLL2 - 100000000);
	PLL2P1 = PLL2 / 390625;
	PLL2P2 = ((PLL2 - PLL2P1*390625)*PLL2P3)/390625;

	
	ACTUALPLL1=100000000 + 390625 * PLL1P1 / 2 + 390625 * PLL1P2 / PLL1P3 / 2;
	ACTUALPLL2=100000000 + 390625 * PLL2P1 / 2 + 390625 * PLL2P2 / PLL2P3 / 2;

	if(Freq0!=0)
		ACTUALFREQ0=((DIV0SRC==0)?ACTUALPLL1:ACTUALPLL2)/(4 * (1<<OUT0DIV) + (DIV0P1 * (1<<OUT0DIV)) / 128);
	else
		ACTUALFREQ0=0;
	
	if(Freq1!=0)
		ACTUALFREQ1=((DIV1SRC==0)?ACTUALPLL1:ACTUALPLL2)/(4 * (1<<OUT1DIV) + (DIV1P1 * (1<<OUT1DIV)) / 128);
	else
		ACTUALFREQ1=0;
	
	if(Freq2!=0)
		ACTUALFREQ2=((DIV2SRC==0)?ACTUALPLL1:ACTUALPLL2)/(4 * (1<<OUT2DIV) + (DIV2P1 * (1<<OUT2DIV)) / 128);
	else
		ACTUALFREQ2=0;
		
}

UINT8 calcur(UINT32 f){
	if(f>20000000)return 3;
	else if(f>15000000)return 2;
	else if(f>10000000)return 1;
	else return 0;
}

void cfg5351(){
	UINT8 CUR;
	
	I2CWrite( 3,0x07);//disable output
	
	I2CWrite(16,0xCF);//disable div0
	I2CWrite(17,0xCF);//disable div1
	I2CWrite(18,0xCF);//disable div2
	
	//26~33 PLL1
	I2CWrite(26,(PLL1P3>>8)&0xFF);
	I2CWrite(27,PLL1P3&0xFF);
	I2CWrite(28,(PLL1P1>>16)&0x03);
	I2CWrite(29,(PLL1P1>>8)&0xFF);
	I2CWrite(30,PLL1P1&0xFF);
	I2CWrite(31,((PLL1P3>>12)&0xF0)|((PLL1P2>>16)&0x0F));
	I2CWrite(32,(PLL1P2>>8)&0xFF);
	I2CWrite(33,PLL1P2&0xFF);
	//34~41 PLL2
	I2CWrite(34,(PLL2P3>>8)&0xFF);
	I2CWrite(35,PLL2P3&0xFF);
	I2CWrite(36,(PLL2P1>>16)&0x03);
	I2CWrite(37,(PLL2P1>>8)&0xFF);
	I2CWrite(38,PLL2P1&0xFF);
	I2CWrite(39,((PLL2P3>>12)&0xF0)|((PLL2P2>>16)&0x0F));
	I2CWrite(40,(PLL2P2>>8)&0xFF);
	I2CWrite(41,PLL2P2&0xFF);
	//44~46 DIV0
	if(OUT0EN){
		I2CWrite(44,(OUT0DIV<<4)|(DIV0P1==0?0x0C:0x00)|((DIV0P1>>16)&0x03));
		I2CWrite(45,(DIV0P1>>8)&0xFF);
		I2CWrite(46,DIV0P1&0xFF);
	}
	//52~54 DIV1
	if(OUT1EN){
		I2CWrite(52,(OUT1DIV<<4)|(DIV1P1==0?0x0C:0x00)|((DIV1P1>>16)&0x03));
		I2CWrite(53,(DIV1P1>>8)&0xFF);
		I2CWrite(54,DIV1P1&0xFF);
	}
	//60~62 DIV2
	if(OUT2EN){
		I2CWrite(60,(OUT2DIV<<4)|(DIV2P1==0?0x0C:0x00)|((DIV2P1>>16)&0x03));
		I2CWrite(61,(DIV2P1>>8)&0xFF);
		I2CWrite(62,DIV2P1&0xFF);
	}
	//16-18
	if(OUT0EN){
		calcur(Freq0);
		I2CWrite(16,0x4C|(DIV0SRC<<5)|CUR);
	}
	if(OUT1EN){
		calcur(Freq1);
		I2CWrite(17,0x4C|(DIV1SRC<<5)|CUR);
	}
	if(OUT2EN){
		calcur(Freq2);
		I2CWrite(18,0x4C|(DIV2SRC<<5)|CUR);
	}
	//Trigger PLL reset
	I2CWrite(177,0xAC);
	mDelaymS(1);
	
	//Check Locked
	i2crecv=0xE0;
	do{
		I2CRead(0,&i2crecv);
	}while(i2crecv&0xE0);
	mDelaymS(1);
	
	//3 ENABLE OUTPUT
	I2CWrite(3,(OUT0EN?0x00:0x01)|(OUT1EN?0x00:0x02)|(OUT2EN?0x00:0x04));
}
