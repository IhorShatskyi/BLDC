#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "stm32f446xx.h"
#include "list.h"

#define SIZE_BLUETOOTH 256

extern list listBluetooth;

extern const struct bluetoothFunctions {
	
	struct {
		void (*const Data)(int32_t);
		void (*const Srting)(char *);
	} Send;

} Bluetooth;

#endif
