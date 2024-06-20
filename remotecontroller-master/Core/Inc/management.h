#ifndef __MANAGEMENT_H
#define __MANAGEMENT_H

#include "main.h"
#include "cmsis_os.h"

#include "24l01.h"
#include "oled.h"
#include "joystick.h"
#include "radiolink.h"

#include <stdio.h>



//flag
#define LINK_Status_Flag  0x01



#define TASK_Canrun_Flag   0x10





void HardWare_Init(void);
void Management_Prepare(void);
void Task_Management(void);




#endif   


