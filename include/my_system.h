#ifndef __MY_SYSTEM_H__
#define __MY_SYSTEM_H__

/*System config*/

#define BAUD_RATE 115200
#define DEBUG 1


#define NETWORK 1               /*网络能力*/
#define LVGL 1                  /*LVGL 图形*/

/*LVGL config*/
#define USE_LV_LOG 0
#define LVGL_TICK_PERIOD 1      /*LVGL 时间片周期ms*/

/****************************
 * 需要在lvgl的配置文件中修改如下配置
 * #define LV_DISP_DEF_REFR_PERIOD      10      //[ms]
 * #define LV_FONT_MONTSERRAT_32    1
*****************************/

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

const String url_master_data = "http://aider.meizu.com/app/weather/listWeather?cityIds=";
const String city_id = "101210101";

#endif //__MY_SYSTEM_H__