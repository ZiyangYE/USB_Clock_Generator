#define I2CADDR 0x60
sbit SCL = P3^2;
sbit SDA = P1^4;

void UDELAY(){
	UINT8 a;
	for(a=0;a<7;a++);
}

BOOL I2CRead(UINT8 a, UINT8* d){
	UINT8 t;
	UINT8 i;
	
	//start
	SDA=0; UDELAY();
	SCL=0; UDELAY();
	
	//dev address
	t=I2CADDR;
	t<<=1;
	for(i=0;i<7;i++){
		if(t&0x80)
			SDA=1;
		else
			SDA=0;
		t<<=1;
		UDELAY();
		SCL=1; UDELAY();
		SCL=0; UDELAY();
	}
	
	//write addr
	SDA=0; UDELAY();
	SCL=1; UDELAY();
	SCL=0; UDELAY();
	
	//release SDA
	SDA=1; UDELAY();
	
	SCL=1; UDELAY();
	if(SDA==1){
		//no ack
		return FALSE;
	}
	
	//reg address
	SCL=0; UDELAY();
	t=a;
	for(i=0;i<8;i++){
		if(t&0x80)
			SDA=1;
		else
			SDA=0;
		t<<=1;
		UDELAY();
		SCL=1; UDELAY();
		SCL=0; UDELAY();
	}
	
	//release SDA
	SDA=1; UDELAY();
	
	SCL=1; UDELAY();
	if(SDA==1){
		//no ack
		return FALSE;
	}
	
	SCL=0; UDELAY();
	SDA=0; UDELAY();
	SCL=1; UDELAY();
	SDA=1; UDELAY();
	
	//start
	SDA=0; UDELAY();
	SCL=0; UDELAY();
	
	//dev address
	t=I2CADDR;
	t<<=1;
	for(i=0;i<7;i++){
		if(t&0x80)
			SDA=1;
		else
			SDA=0;
		t<<=1;
		UDELAY();
		SCL=1; UDELAY();
		SCL=0; UDELAY();
	}
	
	//read
	SDA=1; UDELAY();
	SCL=1; UDELAY();
	SCL=0; UDELAY();
	
	//release SDA
	SDA=1; UDELAY();
	
	SCL=1; UDELAY();
	if(SDA==1){
		//no ack
		return FALSE;
	}
	
	//data
	SCL=0; UDELAY();
	t=0;
	for(i=0;i<8;i++){
		t<<=1;
		if(SDA==1)
			t|=0x01;
		UDELAY();
		SCL=1; UDELAY();
		SCL=0; UDELAY();
	}
	
	//send ACK
	SDA=0; UDELAY();
	
	//terminate
	SCL=1; UDELAY();
	SDA=1; UDELAY();
	
	*d=t;
	
	return TRUE;
}


BOOL I2CWrite(UINT8 a, UINT8 d){
	UINT8 t;
	UINT8 i;
	
	//start
	SDA=0; UDELAY();
	SCL=0; UDELAY();
	
	//dev address
	t=I2CADDR;
	t<<=1;
	for(i=0;i<7;i++){
		if(t&0x80)
			SDA=1;
		else
			SDA=0;
		t<<=1;
		UDELAY();
		SCL=1; UDELAY();
		SCL=0; UDELAY();
	}
	
	//write
	SDA=0; UDELAY();
	SCL=1; UDELAY();
	SCL=0; UDELAY();
	
	//release SDA
	SDA=1; UDELAY();
	
	SCL=1; UDELAY();
	if(SDA==1){
		//no ack
		return FALSE;
	}
	
	//reg address
	SCL=0; UDELAY();
	t=a;
	for(i=0;i<8;i++){
		if(t&0x80)
			SDA=1;
		else
			SDA=0;
		t<<=1;
		UDELAY();
		SCL=1; UDELAY();
		SCL=0; UDELAY();
	}
	
	//release SDA
	SDA=1; UDELAY();
	
	SCL=1; UDELAY();
	if(SDA==1){
		//no ack
		return FALSE;
	}
	
	//data
	SCL=0; UDELAY();
	t=d;
	for(i=0;i<8;i++){
		if(t&0x80)
			SDA=1;
		else
			SDA=0;
		t<<=1;
		UDELAY();
		SCL=1; UDELAY();
		SCL=0; UDELAY();
	}
	
	//release SDA
	SDA=1; UDELAY();
	
	SCL=1; UDELAY();
	if(SDA==1){
		//no ack
		return FALSE;
	}
	
	SCL=0; UDELAY();
	SDA=0; UDELAY();
	SCL=1; UDELAY();
	SDA=1; UDELAY();
	
	return TRUE;
}
