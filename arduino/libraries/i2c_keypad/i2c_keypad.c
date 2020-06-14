#include "i2c_keypad.h"

int keypad(){
	i2c_address(KEYPADWR);
	i2c_send(0xF0);
	i2c_stop();

	i2c_address(KEYPADRD);
	int val= i2c_getval(NACK);
	i2c_stop();

return val;
}
