#include "rtc_ds1307.h"

void send_to_ds1307(uint8_t word_address, uint8_t data){
	i2c_address(DS1307_WR);
	i2c_send(word_address);
	i2c_send(data);
	i2c_stop();
}

void set_seconds(int detik){
	int tens, ones;
	div_t temp;
	temp= div(detik, 10);
	
	ones= (temp.rem);
	tens= (temp.quot) << 4;
	
	uint8_t tmkeep;
	tmkeep= ones| tens;
	
	//send to ds1307
	send_to_ds1307(SEKON,detik);	
}

void set_minute(int menit1){
	int tens, ones;
	div_t temp;
	temp= div(menit1,10);
	
	ones= (temp.rem);
	tens= (temp.quot) << 4;
	
	uint8_t tmkeep;
	tmkeep= ones| tens;
	
	//send to ds1307
	send_to_ds1307(MENIT,menit1);
}

void set_hour(int jam1){
	int tens, ones;
	div_t temp;
	temp= div(jam1,10);
	
	ones= (temp.rem);
	tens= (temp.quot) << 4;
	
	uint8_t tmkeep;
	tmkeep= ones| tens;
	
	//send to ds1307
	send_to_ds1307(JAM,jam1);
}

void set_day(int hari1){
	uint8_t tmkeep;
	tmkeep= hari1;
	
	//send to ds1307
	send_to_ds1307(HARI,hari1);
}

void set_date(int tanggal1){
	int tens, ones;
	div_t temp;
	temp= div(tanggal1,10);
	
	ones= (temp.rem);
	tens= (temp.quot) << 4;
	
	uint8_t tmkeep;
	tmkeep= ones| tens;
	
	//send to ds1307
	send_to_ds1307(TANGGAL, tanggal1);
}

void set_month(int bulan1){
	int tens, ones;
	div_t temp;
	temp= div(bulan1,10);
	
	ones= temp.rem;
	tens= temp.quot;
	
	uint8_t tmkeep;
	tmkeep= ones| tens;
	
	//send to ds1307
	send_to_ds1307(BULAN, bulan1);
}

void set_year(int tahun1){
	int tens, ones;
	div_t temp;
	temp= div(tahun1,10);
	
	ones= temp.rem;
	tens= temp.quot;
	
	uint8_t tmkeep;
	tmkeep= ones| tens;
	
	//send to ds1307
	send_to_ds1307(TAHUN, tahun1);
}

void ds1307_ctrreg(uint8_t data){
	send_to_ds1307(SEKON,CHEN);
	send_to_ds1307(CTRL,data);
}

uint8_t get_from_ds1307(uint8_t word_address){
	uint8_t temp;
	i2c_address(DS1307_WR);
	i2c_send(word_address);
	i2c_address(DS1307_RD);
	temp= i2c_getval(NACK);
	i2c_stop();
return temp;
}

int get_seconds(){
	uint8_t val, ones, tens;
	val= get_from_ds1307(SEKON);
	ones= val & 0x0F;
	tens= (val & 0x70)>>4;
	int temp= (tens*10)+ones;
return temp;
}

int get_minute(){
	uint8_t val, ones, tens;
	val= get_from_ds1307(MENIT);
	ones= val & 0x0F;
	tens= (val & 0x70)>>4;
	int temp= (tens*10)+ones;
return temp;
}

int get_hour(){
	uint8_t val, ones, tens;
	val= get_from_ds1307(JAM);
	ones= val & 0x0F;
	tens= (val & 0x30)>>4;
	int temp= (tens*10)+ones;
return temp;
}

int get_day(){
	uint8_t val, ones, tens;
	val= get_from_ds1307(HARI);
	ones= val & 0x0F;
return ones;
}

int get_date(){
	uint8_t val, ones, tens;
	val= get_from_ds1307(TANGGAL);
	ones= val & 0x0F;
	tens= (val & 0x30)>>4;
	int temp= (tens*10)+ones;
return temp;
}

int get_month(){
	uint8_t val, ones, tens;
	val= get_from_ds1307(SEKON);
	ones= val & 0x0F;
	tens= (val & 0x10)>>4;
	int temp= (tens*10)+ones;
return temp;
}

int get_year(){
	uint8_t val, ones, tens;
	val= 
	ones= val & 0x0F;
	tens= (val & 0xF0)>>4;
	int temp= (tens*10)+ones;
return temp;
}

