#ifndef __MY_SYSTEM_H__
#define __MY_SYSTEM_H__

/*System config*/

#define BAUD_RATE 115200
#define DEBUG 1


#define NETWORK 1               /*网络能力*/
#define LVGL 1                  /*LVGL 图形*/

/*LVGL config*/
#define USE_LV_LOG 0
#define LVGL_TICK_PERIOD 5

const char* ssid = "JDCwifi_B89E";
const char* password =  "199709183614";

/* Debug code */
/* Debug printf to Serial printf ,If define 'DEBUG' is enable*/
#ifdef DEBUG
#define DebugPrintln(message)    Serial.println(message)
#else
#define DebugPrintln(message)
#endif

#ifdef DEBUG
#define DebugPrint(message)    Serial.print(message)
#else
#define DebugPrint(message)
#endif

#endif //__MY_SYSTEM_H__