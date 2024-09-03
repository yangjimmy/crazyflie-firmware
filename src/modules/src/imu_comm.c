#include "config.h"
#include "debug.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "static_mem.h"
#include "task.h"
#include "stabilizer_types.h"
#include "imu_comm.h"

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "usec_time.h"

#include "radiolink.h"
#include "configblock.h"

#define DEBUG_MODULE "imu_comm"
#include "debug.h"

#define MESSAGE_LENGTH 30

// static xQueueHandle inputQueue;
// STATIC_MEM_QUEUE_ALLOC(inputQueue, 1, sizeof(state_t));

static void imuComm();
STATIC_MEM_TASK_ALLOC(imuComm, IMU_COMM_TASK_STACKSIZE);

static bool isInit = false;

extern state_address sa;
float roll;
float pitch;
float yaw;
state_t state;

void imuCommInit() {
  // inputQueue = STATIC_MEM_QUEUE_CREATE(inputQueue);
  // TODO
  STATIC_MEM_TASK_CREATE(imuComm, imuComm, IMU_COMM_TASK_NAME, NULL, IMU_COMM_TASK_PRI);
  isInit = true;
}

bool imuCommTest() {
  return isInit;
}

static void imuComm() {
  DEBUG_PRINT("Example task main function is running!");

  // initialize radio broadcast
  DEBUG_PRINT("Waiting for activation ...\n");

  // Initialize the p2p packet 
  static P2PPacket p_reply;
  p_reply.port=0x00;
  
  // Get the current address of the crazyflie and obtain
  //   the last two digits and send it as the first byte
  //   of the payload
  uint64_t address = configblockGetRadioAddress();
  uint8_t my_id =(uint8_t)((address) & 0x00000000ff);
  p_reply.data[0]=my_id;

  //Put a string in the payload
  //char *str="Hello World Hello World Hello World H";
  
  char str[MESSAGE_LENGTH];

  // send data 
  // while (1) {
    // int input;
    // if (pdTRUE == xQueueReceive(inputQueue, &input, portMAX_DELAY)) {
    //   // Respond to input here!
    // }

    state = *(state_t*)(sa.address);
    roll = state.attitude.roll;
    pitch = state.attitude.pitch;
    yaw = state.attitude.yaw;

    snprintf( str, sizeof(char)*MESSAGE_LENGTH, "r:%5.2f p:%5.2f y:%5.2f", (double)roll, (double)pitch, (double)yaw);
    memcpy(&p_reply.data[1], str, sizeof(char)*MESSAGE_LENGTH);
    
    // Set the size, which is the amount of bytes the payload with ID and the string 
    p_reply.size=sizeof(char)*MESSAGE_LENGTH+1;

    //vTaskDelay(M2T(2000));
    //vTaskDelay(M2T(2)); // 500 Hz
    //vTaskDelay(M2T(5)); // 200 Hz
    // vTaskDelay(M2T(10)); // 100
    vTaskDelay(M2T(20)); // 50
    // vTaskDelay(M2T(50)); // 20
    // vTaskDelay(M2T(100)); // 10
    radiolinkSendP2PPacketBroadcast(&p_reply);
  // }
}