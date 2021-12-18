
#ifndef __MY_WIFI_H__
#define __MY_WIFI_H__


//Network Library
#include "rpcWiFi.h"
#include <WiFiMulti.h>
#include <WiFiSTA.h>
//#include "HTTPClient.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "WiFiClient.h"

/*用户自定义头文件*/
#include "my_system.h"
#include "my_weather.h" 


class MyWifi{


    private:

    int get_fail_cnt;
    WEAK_DAY_E weakday_data_analyze(char * weakday_data);
    WEATHER_E weather_data_analyze(char *weather_data);

    public:
    MyWifi();
 
    void wifi_connect(const char* ssid, const char * password);
    void http_client_init();
    int get_http_data(String &response);
    void json_data_analyze(String http_data,ALL_WEATHER_DATA_T *weather_data);
 
};

extern MyWifi mywifi;

#endif

