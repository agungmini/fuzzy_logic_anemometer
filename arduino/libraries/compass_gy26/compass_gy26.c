
#include "compass_gy26.h"

void reset_gy26(void){
	i2c_address(GY26_WR);
	i2c_send(PRELIMINARY);
	i2c_send(RETURN_FACTORY0);
	i2c_send(RETURN_FACTORY1);
	i2c_send(RETURN_FACTORY2);
	i2c_send(RETURN_FACTORY3);
	i2c_stop();
}

long int get_angle_gy26(){
	i2c_address(GY26_WR);
	i2c_send(PRELIMINARY);
	i2c_send(MEASURE_ANGLE);
	i2c_address(GY26_RD);

	unsigned int temp1= i2c_getval(ACK);
	unsigned int temp2= i2c_getval(ACK);
	unsigned int temp3= i2c_getval(ACK);
	unsigned int temp4= i2c_getval(ACK);
	unsigned int temp5= i2c_getval(ACK);
	unsigned int temp6= i2c_getval(ACK);
	unsigned int temp7= i2c_getval(ACK);
	unsigned int temp8= i2c_getval(NACK);
	i2c_stop();

	int angle= temp2*257+temp3;

return angle;
}

