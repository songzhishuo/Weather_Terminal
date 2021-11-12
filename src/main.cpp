#include <Arduino.h>

//TFT screen Library
#include "SPI.h"

//Network Library
#include "rpcWiFi.h"
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include "HTTPClient.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "WiFiClient.h"

/*My include file*/
#include "my_system.h"
#include "my_weather.h"         //weather struct


#include"TFT_eSPI.h"
#include"Free_Fonts.h" //include the header file
#include <lvgl.h>
#include "lv_demo_stress.h"

#define GET_DATA_FAIL_MAX 5

WiFiMulti wifiMulti;
HTTPClient my_http;
WiFiClient client;



void http_client_init(void);
int get_http_data(String &response);
void json_data_analyze(String http_data,ALL_WEATHER_DATA_T *weather_data);
WEAK_DAY_E weakday_data_analyze(char* weakday_data);
WEATHER_E weather_data_analyze(char* weather_data);
void wifi_connect(const char* ssid, const char * password);
void tft_test();

int get_fail_cnt = 0;           //HTTP数据获取失败计数
int flag = HIGH;//默认当前灭灯
long lastTime = 0;

ALL_WEATHER_DATA_T all_weather_data = {0};

TFT_eSPI tft = TFT_eSPI(); 
#if (LVGL == 1)
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];
#endif

void setup() {
    tft_test();
 
    delay(300);

    Serial.begin(115200);
    while(!Serial); // Wait for Serial to be ready

    Serial.println("hello");
#if (NETWORK == 1)
    // WiFi.mode(WIFI_AP_STA);
    // WiFi.disconnect();

    // Serial.println("Connecting to WiFi..");
    // WiFi.begin(ssid, password);

    // while (WiFi.status() != WL_CONNECTED) {
    //     delay(500);
    //     Serial.println("Connecting to WiFi..");
    //     WiFi.begin(ssid, password);
    // }

    wifi_connect(ssid, password);

    http_client_init();   
#endif
    /*TFT LCD Test*/

}
 
#if (USE_LV_LOG != 0) && (LVGL == 1)
/* Serial debugging */
void my_print(lv_log_level_t level, const char * file, uint32_t line, const char * dsc)
{

  Serial.printf("%s@%d->%s\r\n", file, line, dsc);
  delay(100);
}
#endif

#if (LVGL == 1)
/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint16_t c;

  tft.startWrite(); /* Start new TFT transaction */
  tft.setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1)); /* set the working window */
  for (int y = area->y1; y <= area->y2; y++) {
    for (int x = area->x1; x <= area->x2; x++) {
      c = color_p->full;
      tft.writeColor(c, 1);
      color_p++;
    }
  }
  tft.endWrite(); /* terminate TFT transaction */
  lv_disp_flush_ready(disp); /* tell lvgl that flushing is done */
}


/* Reading input device (simulated encoder here) */
bool read_encoder(lv_indev_drv_t * indev, lv_indev_data_t * data)
{
  static int32_t last_diff = 0;
  int32_t diff = 0; /* Dummy - no movement */
  int btn_state = LV_INDEV_STATE_REL; /* Dummy - no press */

  data->enc_diff = diff - last_diff;;
  data->state = btn_state;

  last_diff = diff;

  return false;
}

void tft_test()
{

  lv_init();

#if USE_LV_LOG != 0
  lv_log_register_print(my_print); /* register print function for debugging */
#endif

  tft.begin(); /* TFT init */
  tft.setRotation(3); /* Landscape orientation */

  lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

  /*Initialize the display*/
  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 320;
  disp_drv.ver_res = 240;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.buffer = &disp_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the touch pad*/
  lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_ENCODER;
  indev_drv.read_cb = read_encoder;
  lv_indev_drv_register(&indev_drv);
  /*stress test*/
  lv_demo_stress();
}


/* Interrupt driven periodic handler */
static void lv_tick_handler(void)
{

  lv_tick_inc(LVGL_TICK_PERIOD);
}

#endif /*LVGL == 1*/

unsigned int task_cnt = 0;

/**
* @Desc  主函数
*/
void loop() {

    String data_str;
    int ret = 0;
#if (LVGL == 1)  
    lv_tick_handler();
    lv_task_handler(); /* let the GUI do its work */
#endif    
    task_cnt ++;  
  
    if(task_cnt >= 400)
    {
      task_cnt = 0;
      /*get weather data*/
      ret = get_http_data(data_str);
      if (ret == 0)       //get data successful
      {
          DebugPrintln("!!!!!!!!!!!!!!!!!----------\r\n");
              
          json_data_analyze(data_str, &all_weather_data);                        //解析数据              
          DebugPrintln("\r\n");
      }
    }
}


#if (NETWORK == 1)
void http_client_init()
{
    String url_data = url_master_data + city_id;

    DebugPrint("HTTP url :");
    DebugPrintln(url_data);

    my_http.setTimeout(5000);      //http超时时间5S
    my_http.begin(client,url_data);

    /*代理信息设置*/
}

int get_http_data(String &response)
{
    int ret = 0;
    int http_code = my_http.GET();
    //String response;
    if(http_code)                       // get data successful
    {
        Serial.printf("[HTTP]GET code:%d\n", http_code);
        if(http_code == HTTP_CODE_OK)
        {
            response =  my_http.getString();
            DebugPrintln("[HTTP]get data successfully!!!!");

            //DebugPrintln(response);                       //打印原始数据
           
            ret = 0;
        }
        else
        {
            DebugPrintln("[HTTP]get data error");
            ret = -1;
            goto GET_DATA_ERR;
        }

    }
    else 
    {
        get_fail_cnt++;
        if(get_fail_cnt >= GET_DATA_FAIL_MAX)           //超过获取失败最大次数
        {
            Serial.printf("[HTTP] get weather data error cnt full, clean it\r\n");
            get_fail_cnt = 0;
            /*reconnect HTTP Server*/
            //my_http.disconnect(false);
            my_http.end();
            http_client_init();
        }
        else
        {
            Serial.printf("[HTTP]get weather data error, cnt[%d]", get_fail_cnt);
        }
        ret = -1;
    }
    my_http.end();

    return ret;

GET_DATA_ERR:
    {
        get_fail_cnt++;
        if(get_fail_cnt >= GET_DATA_FAIL_MAX)           //超过获取失败最大次数
        {
            Serial.printf("[HTTP] get weather data error cnt full, clean it");
            get_fail_cnt = 0;
            /*reconnect HTTP Server*/
            //my_http.disconnect(false);
            my_http.end();
            http_client_init();
        }
        else
        {
            Serial.printf("[HTTP]get weather data error, cnt[%d]", get_fail_cnt);
        }

        return ret;
    }
}

//天气数据解析
void json_data_analyze(String http_data,ALL_WEATHER_DATA_T *weather_data)
{
    DynamicJsonBuffer jsonBuffer;
 
    char weakday_data_str[32] = {0};                //星期原始数据
    char weather_data_str[32] = {0};                //天气原始数据

    char date[16] = {0};                            //日期

    char sun_rise_time[8] = {0};                    //日出时间
    char sun_down_time[8] = {0};                    //日落时间
    char pub_time_str[32] = {0};                    //数据更新时间
    char city_name_str[64] = {0};                   //城市名

    char day_temp = 0;
    char night_temp = 0;

    char weakday = 0;
    char weather = 0;

    char pm2_5 = 0;
    char pm10 = 0;
    char so2 = 0;
    char temp = 0;
    char city_id = 0;

    JsonObject& root = jsonBuffer.parseObject(http_data);
    if(!root.success())
    {
        DebugPrintln("analyze data error"); 
        return ;
    }

    day_temp = root["value"][0]["weathers"][0]["temp_day_c"];                   //白天温度
    night_temp = root["value"][0]["weathers"][0]["temp_night_c"];               //夜间温度

    strcpy(weakday_data_str, root["value"][0]["weathers"][0]["week"]);          //星期
    weakday = weakday_data_analyze(weakday_data_str);

    strcpy(weather_data_str, root["value"][0]["weathers"][0]["weather"]);
    weather = weather_data_analyze(weather_data_str);

    strcpy(date, root["value"][0]["weathers"][0]["date"]);
    strcpy(sun_rise_time, root["value"][0]["weathers"][0]["sun_rise_time"]);
    strcpy(sun_down_time, root["value"][0]["weathers"][0]["sun_down_time"]);

    pm2_5 = root["value"][0]["pm25"]["pm25"];
    pm10 = root["value"][0]["pm25"]["pm10"];
    so2 = root["value"][0]["pm25"]["so2"];
    temp = root["value"][0]["realtime"]["temp"];

    strcpy(pub_time_str, root["value"][0]["pm25"]["upDateTime"]);
    strcpy(city_name_str, root["value"][0]["city"]);                               //中文城市名称
    city_id = root["value"][0]["cityid"];

    weather_data->day_weather_data[0].day_temp = day_temp;
    weather_data->day_weather_data[0].night_temp = night_temp;
    weather_data->day_weather_data[0].weakday = weakday;
    weather_data->day_weather_data[0].weather = weather;
    strcpy(weather_data->day_weather_data[0].date, date);
    strcpy(weather_data->day_weather_data[0].sun_rise_time, sun_rise_time);
    strcpy(weather_data->day_weather_data[0].sun_down_time, sun_down_time);

    weather_data->city_id = city_id;
    weather_data->now_temp = temp;
    weather_data->pm2_5_val = pm2_5;
    weather_data->pm10_val = pm10;
    weather_data->so2_val = so2;
    strcpy(weather_data->publish_time, pub_time_str);
    strcpy(weather_data->city_name, city_name_str);

    Serial.printf("day tem[%d] night tem[%d] weak[%d] date[%s]\r\n", day_temp, night_temp, weakday, date);
    Serial.printf("rise time[%s] down time[%s]\r\n", sun_rise_time, sun_down_time);
    Serial.printf("weather[%d] pm2.5[%d] pm10[%d] so2[%d] temp[%d]\r\n", weather, pm2_5, pm10, so2, temp);
    Serial.printf("publish time[%s] city name[%s]\r\n", pub_time_str, city_name_str);
}

//解析周数据
WEAK_DAY_E weakday_data_analyze(char * weakday_data)
{
    char weak_enum = WEAK_UNKNOW;

    //WEAK_DEF_ARRY
    for(int i = 0; i < WEAK_MAX; i++)
    {
        if(strcmp(weakday_data, WEAK_DEF_ARRY[i]) == 0)  //找到匹配项            
        {
            weak_enum = i;
            break;
        }
        else 
        {
            if(i == WEAK_MAX-1)                 //最后一项还没匹配到
            {
                weak_enum = WEAK_UNKNOW;   
            }
        }
    }

    if((weak_enum == WEAK_UNKNOW) || (weak_enum == WEAK_MAX))
    {
        Serial.printf("[data analyze] weak data illegal,weak data [%d]\r\n",weak_enum);
    }
    return (WEAK_DAY_E)weak_enum;
}

//天气数据解析
WEATHER_E weather_data_analyze(char *weather_data)
{
    char weather_enum = 0;

    //WEATHER_DEF_ARRY
    for(int i = 0; i < WEAK_MAX; i++)
    {
        if(strcmp(weather_data, WEATHER_DEF_ARRY[i]) == 0)  //找到匹配项            
        {
            weather_enum = i;
            break;
        }
        else 
        {
            if(i == W_MAX-1)                 //最后一项还没匹配到
            {
                weather_enum = W_UNKNOW;   
            }
        }
    }

    if((weather_enum == W_UNKNOW) || (weather_enum == W_MAX))
    {
        Serial.printf("[data analyze] weather data illegal,weather data [%d]\r\n",weather_enum);
    }
    return (WEATHER_E)weather_enum;
}


void wifi_connect(const char* ssid, const char * password)
{
// Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
 
    Serial.println("Connecting to WiFi..");
    WiFi.begin(ssid, password);
 
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi..");
        WiFi.begin(ssid, password);
    }

    Serial.println("Connected to the WiFi network");
    Serial.print("IP Address: ");
    Serial.println (WiFi.localIP()); // prints out the device's IP address
    Serial.print("Dev mac:  ");
    Serial.println(WiFi.macAddress());
}

#endif /*NETWORK == 1*/