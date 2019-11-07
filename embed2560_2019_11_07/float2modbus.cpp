// 
// 
// 

#include "float2modbus.h"

void float2modbus (float floatVal, uint16_t* uint16Array) {

	// Create union of shared memory space
	union
	{
		float uFloat;
		uint16_t uArray[2];
	} u;

	// Overite bytes of union with float variable
	u.uFloat = floatVal;
	// Assign bytes to input array
	memcpy (uint16Array, u.uArray, 4);
}

