#include <pigpio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	gpioInitialise();

	gpioSetMode(17, PI_OUTPUT);
	
	gpioServo(17, 1500);
	sleep(2);
	gpioServo(17, 500);
	sleep(2);
	gpioServo(17, 1500);
	sleep(2);
	gpioServo(17, 2500);
	sleep(2);

	gpioTerminate();


	return EXIT_SUCCESS;
}
