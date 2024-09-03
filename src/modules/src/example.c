#include "config.h"
#include "debug.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "static_mem.h"
#include "task.h"
#include "system.h"

#include "radiolink.h"
#include "configblock.h" // configblockGetRadioAddress
// #include <string.h> // memcpy
#include <stdio.h> // snprintf
#define MESSAGE_LENGTH 38

#include "stabilizer_types.h"
#define UPDATE_RATE 50

uint32_t taskStep = 0;


static xQueueHandle inputQueue;
STATIC_MEM_QUEUE_ALLOC(inputQueue, 1, sizeof(float));

static void exampleTask(void*);
STATIC_MEM_TASK_ALLOC(exampleTask, EXAMPLE_TASK_STACKSIZE);

static bool isInit = false;

char str[38];

void exampleTaskInit() {
  if (isInit) {
    return;
  }
  inputQueue = STATIC_MEM_QUEUE_CREATE(inputQueue);
  // TODO
  STATIC_MEM_TASK_CREATE(exampleTask, exampleTask, EXAMPLE_TASK_NAME, NULL, EXAMPLE_TASK_PRI);
  isInit = true;
}

bool exampleTaskTest() {
  return isInit;
}

static void exampleTask(void* parameters) {
  systemWaitStart();

  DEBUG_PRINT("Example task main function is running!");
  // float input;
  
  //static P2PPacket p_reply;
  P2PPacket p_reply;
  p_reply.port=0x00;
  uint64_t address = configblockGetRadioAddress();
  uint8_t my_id =(uint8_t)((address) & 0x00000000ff);
  p_reply.data[0]=my_id;
  //p_reply.size=sizeof(char)*MESSAGE_LENGTH+1;
  p_reply.size=sizeof(char)*2+1;
  p_reply.data[1] = 1;
  
  while (true) {
    // if (pdTRUE == xQueueReceive(inputQueue, &input, portMAX_DELAY)) {
    //   // Respond to input here!
    //   DEBUG_PRINT("Received input!\n");
    // }
    // xQueueReceive(inputQueue, &input, portMAX_DELAY);
    // DEBUG_PRINT("Roll: %f", (double)input);
    // snprintf( str, sizeof(char)*MESSAGE_LENGTH, "%d", 1 );
    
    
    // memcpy(&p_reply.data[1], str, sizeof(char)*MESSAGE_LENGTH);
    // if (RATE_DO_EXECUTE(UPDATE_RATE, taskStep)) {
      radiolinkSendP2PPacketBroadcast(&p_reply);
    // }
    // taskStep++;
    
    vTaskDelay(M2T(50));
  }
  while(1) {
    vTaskDelay(portMAX_DELAY);
  }
}

void exampleTaskEnqueueInput(float value) {
  xQueueOverwrite(inputQueue, &value);
}

