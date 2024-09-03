#ifndef __IMU_COMM_H__
#define __IMU_COMM_H__

#pragma once

#include <stdbool.h>

typedef struct state_address {
   void* address;
} state_address;

#define STATE_ADDRESS(ADDRESS) \
   (state_address){ .address = (void*)(ADDRESS)}

void imuCommInit();
bool imuCommTest();

#endif // __IMU_COMM_H__