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


#if (NETWORK == 1)
void http_client_init(void);
int get_http_data(String &response);
#endif
void json_data_analyze(String http_data,ALL_WEATHER_DATA_T *weather_data);

WEAK_DAY_E weakday_data_analyze(char* weakday_data);
WEATHER_E weather_data_analyze(char* weather_data);
void wifi_connect(const char* ssid, const char * password);
void software_rtc(void) ;
#if (LVGL == 1)
void tft_test();
static void lv_tick_handler(void);
#endif

/*全局变量声明*/
int get_fail_cnt = 0;           //HTTP数据获取失败计数
int flag = HIGH;//默认当前灭灯
long lastTime = 0;
static char page_flag = 0;      /*当前页面 0：今日天气主页 1：近5日天气*/

/*用与比较拉取的数据是否更新*/
static char night_tmp_old[5] = {0};
static char day_tmp_old[5] = {0};

ALL_WEATHER_DATA_T all_weather_data = {0};

TFT_eSPI tft = TFT_eSPI(); 

#if (LVGL == 1)
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];
#endif

#if 0
static void Swtmr1_Callback(void* parameter)
{
    //软件定时器的回调函数，用户自己实现
    Serial.println("hello timer test");
}
#endif

unsigned long timer_tick_last = 0;
unsigned long timer_tick_now = 0;



void setup() {
    pinMode(WIO_5S_PRESS, INPUT_PULLUP);
    pinMode(WIO_BUZZER, OUTPUT);

#if (LVGL == 1)    
    tft_test();
#endif
    delay(300);

    Serial.begin(BAUD_RATE);

#if (DEBUG == 1)
    //while(!Serial); // Wait for Serial to be ready
    Serial.println("hello");
#endif
    
#if (NETWORK == 1)
    wifi_connect(ssid, password);
    http_client_init();   
#endif
    /*TFT LCD Test*/
    timer_tick_last = millis();             //初始化lvgl tick 定时器
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

void memclr_weather_data_old()
{
    memset(night_tmp_old, 0,sizeof(night_tmp_old));
    memset(day_tmp_old, 0,sizeof(day_tmp_old));
}

unsigned char memcpy_weather_data(ALL_WEATHER_DATA_T data)
{

    char i = 0;

    for(i = 0;i<5;i++)
    {
        if(data.day_weather_data[i].night_temp != night_tmp_old[i])     //不相同
        {
            //chart_1->points[i] = data.day_weather_data[i].night_temp;
            night_tmp_old[i] = data.day_weather_data[i].night_temp;
        }
        else
        {
            return -1;
        }

        if(data.day_weather_data[i].day_temp != day_tmp_old[i])     //不相同
        {
            //chart_0->points[i] = data.day_weather_data[i].day_temp;
            day_tmp_old[i] = data.day_weather_data[i].day_temp;
        }
        else
        {
            return -1;
        }
       
    }

    return 0;
}

void button_task()              //按键函数
{
    unsigned int button_flag = 0;
    static unsigned int button_flag_old = 1;
    
    button_flag = digitalRead(WIO_5S_PRESS);

    if(button_flag_old!=button_flag)
    {
        button_flag_old=button_flag;
        if(digitalRead(WIO_5S_PRESS) == HIGH)           //按键任务 按键抬起 此处后面要改成函数指针
        {

            change_page(page_flag);
            page_flag = !page_flag;
            Serial.println("5 Way Press");

        }
            
    }
}



void network_task(lv_task_t * task)
{
    String data_str;
    static char page_flag_last = 1;     /*页面切换状态保存*/

    static unsigned int cnt = 0;
    //unsigned long temp_delay = 0;
    int ret = 0;

    if(cnt >= 60)               //60S
    {
        cnt = 0;
    }
    
    
    if (cnt == 0)
    {
        /*get weather data*/
#if (NETWORK == 1)      
        ret = get_http_data(data_str);
        if (ret == 0)       //get data successful
        {
            DebugPrintln("!!!!!!!!!!!!!!!!!----------\r\n");
                
            json_data_analyze(data_str, &all_weather_data);                        //解析数据              
            DebugPrintln("\r\n");
        }
#endif

    }
    cnt++;

    if(page_flag != page_flag_last)
    {
        page_flag_last = page_flag;
        memclr_weather_data_old();              /*清除缓存强制刷新*/
    }

    if(page_flag == 1)      /*只有在近5日天气的页面才去刷新，否则会卡死*/
    {
        if(memcpy_weather_data(all_weather_data) != 0)       /*对比数据*/
        {
            chart_test(all_weather_data);                   /*数据不相同*/
        }
        
    }
    
    Serial.println("---> network_task");
}

static char blink_flag = 0;

void my_task(lv_task_t * task)
{
    button_task() ;
    Serial.println("---> my_task");

        //  digitalWrite(WIO_BUZZER, blink_flag);
        // blink_flag = !blink_flag;
    //software_rtc();
}

unsigned int time_cnt = 0;
char time_sec = 0;
char time_min = 0;
char time_hour = 0;

void software_rtc()     //软件RTC
{
    time_cnt++;

    //button_task() ;

    if(time_cnt >= 1000)            //1mS
    {
        time_cnt = 0;
        time_sec++;

        digitalWrite(WIO_BUZZER, blink_flag);
        blink_flag = !blink_flag;

        if(time_sec >= 60)
        {
            time_sec = 0;
            time_min ++;
            //set_time(0,sec);
            //Serial.printf("min{%d}\n",min);

            if(time_min >= 60)
            {
                time_min = 0;
                time_hour++;

                if(time_hour >= 24)
                {
                    time_hour = 0;
                }
            }
        }
       
        

    }

}

/**
* @Desc  主函数
*/
unsigned int task_cnt = 0;

unsigned int blink_cnt = 0;

unsigned char init_flag = 0;

lv_task_t * task1;
void loop() {
    if(!init_flag)
    {
        init_flag = true;
        /**Init lvlg task**/
        
        lv_task_t * task2 = lv_task_create(network_task, 1000, LV_TASK_PRIO_MID, _NULL);       // 1S
        task1 = lv_task_create(my_task, 10, LV_TASK_PRIO_HIGHEST, _NULL);
        lv_task_ready(task1);
    }

#if (LVGL == 1)  
    
    lv_task_handler(); /* let the GUI do its work */
#endif    
    
    

    timer_tick_now = micros();
    if((timer_tick_now - timer_tick_last) >= 1000)       //1ms
    {
        lv_tick_handler();
        timer_tick_last = micros(); 
    }

    // if(task_cnt >= 65530)
    // {
    //   task_cnt = 0;


//       /*get weather data*/
// #if (NETWORK == 1)      
//       ret = get_http_data(data_str);
//       if (ret == 0)       //get data successful
//       {
//           DebugPrintln("!!!!!!!!!!!!!!!!!----------\r\n");
              
//           json_data_analyze(data_str, &all_weather_data);                        //解析数据              
//           DebugPrintln("\r\n");
//       }
// #endif
    
    // }
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
//StaticJsonDocument<500> root;
//天气数据解析
void json_data_analyze(String http_data,ALL_WEATHER_DATA_T *weather_data)
{
    DynamicJsonBuffer  jsonBuffer;
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

    char i = 0;
    for(i = 0;i<5;i++)
    {
        day_temp = root["value"][0]["weathers"][i]["temp_day_c"];                   //白天温度
        night_temp = root["value"][0]["weathers"][i]["temp_night_c"];               //夜间温度

        weather_data->day_weather_data[i].day_temp = day_temp;
        weather_data->day_weather_data[i].night_temp = night_temp;
        weather_data->day_weather_data[i].weakday = weakday;
        weather_data->day_weather_data[i].weather = weather;
        strcpy(weather_data->day_weather_data[i].date, date);
        strcpy(weather_data->day_weather_data[i].sun_rise_time, sun_rise_time);
        strcpy(weather_data->day_weather_data[i].sun_down_time, sun_down_time);

    }


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

