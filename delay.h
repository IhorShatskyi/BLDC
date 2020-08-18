#ifndef DELAY_H
#define DELAY_H

extern const struct delayFunctions {
	
	void (*const MicroSecond)(float);
	void (*const MilliSecond)(float);
	void (*const Second)(float);

} Delay;

#endif
