/* 
 * Arduino
 * hot wire junction 
 * created by agung nursyeha
 */

#include "i2c.h"
#include "compass_gy26.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "i2c_lcd.h"


unsigned int adc_a,adc_a1;  
unsigned int adc_b,adc_b1;
unsigned int adc_c,adc_c1;
unsigned int adc_d;
unsigned int mav_a[20],mav_b[20],mav_c[20];
float vel_a; 
float vel_b;
float vel_c;
float magnitude_velocity;

int encoder,encoder_temp;
int baca_kompas,sudut,sudut_aktual;
int magnitude_eror,magnitude_eror1,eror,eror_bandingan;

boolean rotation_status; //1 counter clockwise, 0 clockwise
boolean encoderB_state;

#define temperature 0x00 //kanal 0
#define sensorA 0x01 //kanal 2
#define sensorB 0x02 //kanal 3
#define sensorC 0x03 //kanal 1

char buff[64];

unsigned int a[4],b[4],c[4];
unsigned long fuzzy_map[64];
int angle_rule[64]= {0,0,0,0,135,90,45,23,135,113,90,68,135,120,113,90
                    ,225,315,315,328,180,0,338,348,158,135,45,105,135,125,90,45
                    ,225,235,315,280,190,255,315,305,180,180,0,15,170,190,225,45
                    ,225,235,250,270,205,255,245,270,190,210,255,270,180,200,205,0};

int cek_bit_on_register(int nama_register, int lokasi){
  int val;
  val= nama_register >> lokasi;
  val= val & 1;
  return val;
}

int adc_val(uint8_t kanal){
  int val= 0;
  //ambil nilai ADC data register
  ADMUX = kanal; //kanal yang dilihat
  ADMUX |= (1<<REFS0); //tegangan referensi dari VREF
  ADCSRA |= (1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2); //ADC ENABLE, PRESCALER 128
  while(!cek_bit_on_register(ADCSRA, 4)){
    while(!cek_bit_on_register(ADCSRA, 6)){
      ADCSRA |= (1<<ADSC); //start conversion
    }
  }
  
  //pindah ADCL dan ADCH ke val
  val= ADCL+(ADCH<< 8);
  ADCSRA &= 0x10; //clear flag dan disable adc
  return val;
}

 float velocity(int val){
  float temp=0;
  float alo[4]= {3.69*pow(10,-2),1.1*pow(10,-3),-4.96*pow(10,-5),2.14*pow(10,-6)};
  int xalo[4]= {307,380,390,403};
  int i= 0;
  while(i<4){
    int j= 0;
    int k= 1;
    while(j<k){
      alo[i]= alo[i]*(val-xalo[j]);  
      j++;
    }
    temp= temp+alo[i];
    k++;
    i++;
  }
  return temp;
}

float resultan(int val_a,int val_b,int val_c){
  float temp,vA,vB,vC;
  vA= velocity(val_a);
  vB= velocity(val_b);
  vC= velocity(val_c);

  temp= sqrt(pow(vA,2)+pow(vB,2)+pow(vC,2));
  return temp;
}

//rule fuzzification function
int fs_segitiga(int val,int low_bound,int middle,int up_bound){
  if ((val>= low_bound)&&(val<=middle))  {
    return ((val-low_bound)*100)/(middle-low_bound);
  }
  else if ((val> middle)&&(val<= up_bound))  {
    return ((up_bound-val)*100)/(up_bound-middle);
  }
  else  {
    return 0;
  }
}

int fs_trapesium1(int val,int low_bound,int l_triangle,int up_bound){
  if ((val>up_bound)&&(val<=l_triangle))  {
    return ((l_triangle-val)*100)/(l_triangle-up_bound);
  }
  else if ((val>= low_bound)&(val<=up_bound))  {
    return 100;
  }
  else  {
    return 0;
  }
}

int fs_trapesium2(int val,int low_bound,int l_triangle,int up_bound){
  if((val>= l_triangle)&&(val<=low_bound))  {
    return ((val-l_triangle)*100)/(low_bound-l_triangle);
  }
  else if((val> low_bound)&&(val<=up_bound))  {
    return 100;
  }
  else  {
    return 0;
  }
}

int centroid(){
  int i=0;
  unsigned int miu=0;
  unsigned int mius=0;
  while(i<64){
    mius=mius+(fuzzy_map[i]*angle_rule[i]);
    miu=miu+fuzzy_map[i];
    i=i+1;
  }
  return (mius/miu);
}

void timer2_init(){
  //inisialisasi timer normal
  TCCR2B= TCCR2B|(1<<CS20)|(1<<CS21)|(1<<CS22); //normal mode timer prescaler 1024
  TIMSK2= TIMSK2|(1<<TOIE2); //timer interupt overflow enable 
}

void timer1_init(){
  TCCR1A= 0xA3; //phase correct 10 bit PWM pada pin OCR1A dan OCR1B
  TCCR1B= 0x04; //no prescaler
}

void interupt_init(){
  EICRA= EICRA|(1<<ISC00)|(1<<ISC01); //external interupsi
  EIMSK= EIMSK|(1<<INT0);  
}

void fuzzifikasi(){
  //sensor A
  a[0]=fs_trapesium1(adc_a,0,309,320);
  a[1]=fs_segitiga(adc_a,309,320,342);
  a[2]=fs_segitiga(adc_a,320,342,360);
  a[3]=fs_trapesium2(adc_a,342,360,1024);
  //sensor B
  b[0]=fs_trapesium1(adc_b,0,309,320);
  b[1]=fs_segitiga(adc_b,309,320,342);
  b[2]=fs_segitiga(adc_b,320,342,360);
  b[3]=fs_trapesium2(adc_b,342,360,1023);
  //sensor C
  c[0]=fs_trapesium1(adc_c,0,309,320);
  c[1]=fs_segitiga(adc_c,309,320,342);
  c[2]=fs_segitiga(adc_c,320,342,360);
  c[3]=fs_trapesium2(adc_c,342,360,1023);
}

void mamdani(){
  fuzzifikasi();
  int i=0;
  int l=0;
  while(i<4){
    int j=0;
    while(j<4){
      int k=0;
      while(k<4){
        fuzzy_map[l]=0;
        if ((a[k]!=0)&&(b[j]!=0)&&(c[i]!=0)){
          if(a[k]<b[j]){
            fuzzy_map[l]= a[k];
            if(c[i]<fuzzy_map[l]){
              fuzzy_map[l]= c[i];    
            }
          }  
          else{
            fuzzy_map[l]= b[j];
            if(c[i]<fuzzy_map[l]){
              fuzzy_map[l]= c[i];
            }
          }
        }
        l=l+1;
        k=k+1;
      }
      j=j+1;
    }
    i=i+1;
  }
  sudut= (centroid());
  //OCR1A=1023;
  pid_controller(8,0.9,0.4);
}

void pid_controller(double kp,double ki,double kd){
  //controller PID untuk motor DC
  eror= (360+sudut-sudut_aktual)%360;  
  eror_bandingan= abs(360+sudut_aktual-sudut)%360;
  int pid=0;

  if(eror<eror_bandingan){
    pid=(kp*eror)+(ki*(eror+magnitude_eror))+(kd*(eror-magnitude_eror));
    searah(pid);
    magnitude_eror=eror;
  }
  else{
    pid=(kp*eror_bandingan)+(ki*(eror+magnitude_eror))+(kd*(eror-magnitude_eror));
    berlawanan_arah(pid);
    magnitude_eror=eror_bandingan;
  }
}

void searah(int val){
  rotation_status= HIGH;
  OCR1B=0;
  PORTD= 0x40;
  if (val<= 1023){
    OCR1A= val;  
  }
  else{
    OCR1A= 1023;
  }
}

void berlawanan_arah(int val){
  rotation_status= LOW;
  OCR1A=0;
  PORTD=0x20;
  if (val<=1023){
    OCR1B= val;
  }
  else{
    OCR1B= 1023;
  }
}

ISR(TIMER2_OVF_vect){
  adc_a1= adc_val(sensorA);//analogRead(sensorA);
  adc_b1= adc_val(sensorB);//analogRead(sensorB);
  adc_c1= adc_val(sensorC);//analogRead(sensorC);

//MAV
  //SensorA
  mav_a[0]=adc_a1;
  adc_a=0;
  int i=0;
  while(i<20){
    adc_a=adc_a+mav_a[i];
    i++;
  }
  adc_a=adc_a/20;
  mav_a[20]=mav_a[19];
  mav_a[19]=mav_a[18];
  mav_a[18]=mav_a[17];
  mav_a[17]=mav_a[16];
  mav_a[16]=mav_a[15];
  mav_a[15]=mav_a[14];
  mav_a[14]=mav_a[13];
  mav_a[13]=mav_a[12];
  mav_a[12]=mav_a[11];
  mav_a[11]=mav_a[10];
  mav_a[10]=mav_a[9];
  mav_a[9]=mav_a[8];
  mav_a[8]=mav_a[7];
  mav_a[7]=mav_a[6];
  mav_a[6]=mav_a[5];
  mav_a[5]=mav_a[4];
  mav_a[4]=mav_a[3];
  mav_a[3]=mav_a[2];
  mav_a[2]=mav_a[1];
  mav_a[1]=mav_a[0];
  
  //SensorB
  mav_b[0]=adc_b1;
  adc_b=0;
  i=0;
  while(i<20){
    adc_b=adc_b+mav_b[i];
    i++;
  }
  adc_b=adc_b/20;
  mav_b[20]=mav_b[19];
  mav_b[19]=mav_b[18];
  mav_b[18]=mav_b[17];
  mav_b[17]=mav_b[16];
  mav_b[16]=mav_b[15];
  mav_b[15]=mav_b[14];
  mav_b[14]=mav_b[13];
  mav_b[13]=mav_b[12];
  mav_b[12]=mav_b[11];
  mav_b[11]=mav_b[10];
  mav_b[10]=mav_b[9];
  mav_b[9]=mav_b[8];
  mav_b[8]=mav_b[7];
  mav_b[7]=mav_b[6];
  mav_b[6]=mav_b[5];
  mav_b[5]=mav_b[4];
  mav_b[4]=mav_b[3];
  mav_b[3]=mav_b[2];
  mav_b[2]=mav_b[1];
  mav_b[1]=mav_b[0];
  
  //SensorC
  mav_c[0]=adc_c1;
  adc_c=0;
  i=0;
  while(i<20){
    adc_c=adc_c+mav_c[i];
    i++;
  }
  adc_c=adc_c/20;
  mav_c[20]=mav_c[19];
  mav_c[19]=mav_c[18];
  mav_c[18]=mav_c[17];
  mav_c[17]=mav_c[16];
  mav_c[16]=mav_c[15];
  mav_c[15]=mav_c[14];
  mav_c[14]=mav_c[13];
  mav_c[13]=mav_c[12];
  mav_c[12]=mav_c[11];
  mav_c[11]=mav_c[10];
  mav_c[10]=mav_c[9];
  mav_c[9]=mav_c[8];
  mav_c[8]=mav_c[7];
  mav_c[7]=mav_c[6];
  mav_c[6]=mav_c[5];
  mav_c[5]=mav_c[4];
  mav_c[4]=mav_c[3];
  mav_c[3]=mav_c[2];
  mav_c[2]=mav_c[1];
  mav_c[1]=mav_c[0];
  
  mamdani();
}

ISR(INT0_vect){
  if((PIND&0xC)== 12){
    encoder_temp--;
    if(encoder_temp%4==0){
      encoder--;
    }
  }
  if((PIND&0xC)== 4){
    encoder_temp++;
    if(encoder_temp%4==0){
      encoder++;
    }  
  }
  sudut_aktual= abs(360+encoder)%360; 
}

void setup(){
  //Port C sebagai input
  DDRC= 0x30;
  PORTC= 0x30;

  //Port B, pin 1 dan 2 sebagai output
  DDRB= 0x06;
  PORTB= 0x06;

  //Port D, pin
  DDRD= 0x00;
  PORTD= 0x00;
  
  encoder_temp=0;
  encoder=0;
  sudut_aktual=0;
  sudut=0;
  eror=0;
  
  magnitude_eror=0;
  magnitude_eror1=0;
  
  Serial.begin(9600);
  timer2_init();
  timer1_init();
  interupt_init();
  i2c_init(50);
  i2c_lcdInit();
}

void loop(){
  i2c_address(0xA0);
  delay(1); 
  //i2c_lcdClear();
  //baca_kompas= get_angle_gy26()/10;
  
  //tampil sudut
  //i2c_lcdSetCursor(0,0);
  //i2c_lcdWriteStr("comp|wind_dir|degree");
  //i2c_lcdSetCursor(1,0);
  //sprintf(buff,"%3d%c|   %4d%c|%4d%c",baca_kompas,0xDF,abs(sudut+baca_kompas)%360,0xDF,abs(sudut_aktual+baca_kompas)%360,0xDF);
  //i2c_lcdWriteStr(buff);

  //tampil kecepatan
  //vel_a= velocity(adc_a);
  //vel_b= velocity(adc_b);
  //vel_c= velocity(adc_c);
  //magnitude_velocity= resultan(adc_a,adc_b,adc_c);
  
  //int va= vel_a*10;
  //int vb= vel_b*10;
  //int vc= vel_c*10;
  //int mgV= magnitude_velocity*10;

  //tampil kecepatan
  //i2c_lcdSetCursor(2,0);
  //i2c_lcdWriteStr("v_A |v_B |v_C |mag_v");
  //sprintf(buff,"%d|%d|%d",adc_a,adc_b,adc_c);
  //sprintf(buff,"%2d.%1d|%2d.%1d|%2d.%1d|%2d.%1d",va/10,abs(va%10),vb/10,abs(vb%10),vc/10,abs(vc%10),mgV/10,abs(mgV%10));
  //i2c_lcdSetCursor(3,0);
  //i2c_lcdWriteStr(buff);
  //delay(250);
}
