#ifndef RTC_DS1307_H
#define RTC_DS1307_H


#include "Arduino.h"
#include "stdint.h"
#include "inttypes.h"
#include "i2c.h"
#include "stdio.h"
#include "stdlib.h"

//configuration
#define F0 0x00
#define F1 0x01
#define F2 0x02
#define F3 0x03
#define SQWEN 0x10
#define	SQWDIS 0x00
#define CHEN 0x00
#define CHDIS 0x80

//device address
#define DS1307_WR 0xD0
#define DS1307_RD 0xD1

//word address
#define SEKON	0x00
#define MENIT	0x01
#define JAM	0x02
#define HARI	0x03
#define TANGGAL	0x04
#define BULAN	0x05
#define TAHUN	0x06
#define CTRL	0x07

//hari
#define MINGGU	1
#define SENIN	2
#define SELASA	3
#define RABU	4
#define KAMIS	5
#define JUMAT	6
#define SABTU	7

//bulan
#define JANUARI	  1
#define FEBRUARI  2
#define MARET	  3
#define APRIL 	  4
#define MEI 	  5
#define JUNI	  6
#define JULI	  7
#define AGUSTUS	  8
#define SEPTEMBER 9
#define OKTOBER	  10
#define NOVEMBER  11
#define DESEMBER  12

#ifdef __cplusplus
extern "C" {
#endif

void send_to_ds1307(uint8_t word_address,uint8_t data);
void set_seconds(int detik);
void set_minute(int menit);
void set_hour(int jam);
void set_day(int hari);
void set_date(int tanggal);
void set_month(int bulan);
void set_year(int tahun);
void ds1307_ctrreg(uint8_t);

uint8_t get_from_ds1307(uint8_t word_address);
int get_seconds();
int get_minute();
int get_hour();
int get_day();
int get_date();
int get_month();
int get_year();

#ifdef __cplusplus
} //extern "C"
#endif


#endif //RTC_DS1307_H
