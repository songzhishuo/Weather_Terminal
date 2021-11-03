/*
 * @Author: Argon
 * @Date: 2021-11-02 15:41:41
 * @LastEditTime: 2021-11-03 18:00:07
 * @LastEditors: Argon
 * @Description: 
 * @FilePath: \esp8266_get_weather\src\main.cpp
 * 1Talk is cheap. Show me the code. (*｀皿´*)ﾉ 
 */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
// void setup() {
//   // put your setup code here, to run once:
// }

// void loop() {
//   // put your main code here, to run repeatedly:
// }

#define BAUD_RATE 115200
#define DEBUG 1

#define GET_DATA_FAIL_MAX 5

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


typedef enum{
    W_SUN = 0,              //晴天
    W_PARTLY_CLOUDY,        //多云
    W_OVERCAST,             //阴天
    W_FOG,                  //雾
    W_LIGHT_RAIN,           //小雨
    W_MODERATE_RAIN,        //中雨
    W_HEAVY_RAIN,           //大雨
    W_RAINSTORM,            //暴雨
    W_MAX,                //最大范围
    W_UNKNOW,               //未知天气
}WEATHER_E;

typedef enum{
    WEAK_SUN = 0,           //周日
    WEAK_MON,               //周一
    WEAK_TUES,
    WEAK_WED,
    WEAK_THUR,
    WEAK_FRI,
    WEAK_SAT,               //周六
    WEAK_MAX, 
    WEAK_UNKNOW,            //未知日期
     
}WEAK_DAY_E;

typedef struct{
    char day_temp;                   //白天温度
    char night_temp;                 //夜晚温度
    char weakday;                   //星期
    char weather;                   //天气  
    char date[16];                  //2021-11-03
    char sun_rise_time[8];          //日出时间
    char sun_down_time[8];          //日落时间
}DAY_WEATHER_T;

typedef struct 
{
    int city_id;                            //城市编号

    char now_temp;                          //当前温度
    char pm2_5_val;                         //PM2.5 浓度
    char pm10_val;                          //PM10 浓度
    char so2_val;                           //so2 浓度

    DAY_WEATHER_T day_weather_data[7];      //近5天的天气情况
    char city_name[64];                     //城市名称
    char publishTime[32];                   //数据更新时间
}ALL_WEATHER_DATA_T;



bool autoConfig(void);
void smartConfig(void);
void http_client_init(void);
int get_http_data(String &response);
void json_data_analyze(String http_data);
WEAK_DAY_E weakday_data_analyze(char * weakday_data);
WEATHER_E weather_data_analyze(char weather_data);

int get_fail_cnt = 0;           //HTTP数据获取失败计数
int flag = HIGH;//默认当前灭灯
long lastTime = 0;


String url_master_data = "http://aider.meizu.com/app/weather/listWeather?cityIds=";
String city_id = "101210101";

HTTPClient http;
WiFiClient client;


const char WEAK_DEF_ARRY[WEAK_MAX][32] = {"星期日","星期一","星期二","星期三","星期四","星期五","星期六"};
const char WEATHER_DEF_ARRY[W_E_MAX][32] = {"晴","多云","阴","雾","小雨","中雨","大雨","暴雨"}; 
ALL_WEATHER_DATA_T all_weather_data = {0};
/**
* @Desc 初始化操作
*/
void setup() {
    Serial.begin(BAUD_RATE);

    WiFi.disconnect();
    if(!autoConfig()){
    smartConfig();
    DebugPrint("Connecting to WiFi");//写几句提示，哈哈
        while (WiFi.status() != WL_CONNECTED) {
        //这个函数是wifi连接状态，返回wifi链接状态
            delay(500);
            DebugPrint(".");
        }
    }
  
    delay(1000);
    //digitalWrite(LED, LOW);
    DebugPrintln("IP address: ");
    DebugPrintln(WiFi.localIP());//WiFi.localIP()返回8266获得的ip地址
    lastTime = millis();

    http_client_init();
  //使能软件看门狗的触发间隔
  //ESP.wdtEnable(5000);
}

/**
* @Desc  主函数
*/
void loop() {
    String data_str;
    int ret = 0;
    ret = get_http_data(data_str);
    if (ret == 0)       //get data successful
    {
        DebugPrintln("!!!!!!!!!!!!!!!!!----------\r\n");
            
        json_data_analyze(data_str);                        //解析数据              
        DebugPrintln("\r\n");

    }
    delay(2000);
}

/**
* 自动连接20s 超过之后自动进入SmartConfig模式
*/
bool autoConfig(){
  WiFi.mode(WIFI_AP_STA);     //设置esp8266 工作模式
  WiFi.begin();
  delay(500);//刚启动模块的话 延时稳定一下
  DebugPrintln("AutoConfiging ......");
  for(int index=0;index<10;index++){
    int wstatus = WiFi.status();
    if (wstatus == WL_CONNECTED){
      DebugPrintln("AutoConfig Success");
      DebugPrint("SSID:");
      DebugPrintln(WiFi.SSID().c_str());
      DebugPrint("PSW:");
      DebugPrintln(WiFi.psk().c_str());
      return true;
    }else{
      DebugPrint(".");
      delay(500);
      flag = !flag;
      //digitalWrite(LED, flag);
    } 
  }
  DebugPrintln("AutoConfig Faild!");
  return false;
}

/**
* 开启SmartConfig功能
*/
void smartConfig()
{
  WiFi.mode(WIFI_STA);
  delay(1000);
  DebugPrintln("Wait for Smartconfig");
  // 等待配网
  WiFi.beginSmartConfig();
  while (1){
    DebugPrint(".");
    delay(200);
    flag = !flag;
    //digitalWrite(LED, flag);
    
    if (WiFi.smartConfigDone()){
      //smartconfig配置完毕
      DebugPrintln("\nSmartConfig Success");
      DebugPrint("SSID:");
      DebugPrintln(WiFi.SSID().c_str());
      DebugPrint("PSW:");
      DebugPrintln(WiFi.psk().c_str());
      WiFi.mode(WIFI_AP_STA);     //设置esp8266 工作模式
      WiFi.setAutoConnect(true);  // 设置自动连接
      break;
    }
  }
}

void http_client_init()
{
    String url_data = url_master_data + city_id;

    DebugPrint("HTTP url :");
    DebugPrintln(url_data);

    http.setTimeout(5000);      //http超时时间5S
    http.begin(client,url_data);

    /*代理信息设置*/
}

int get_http_data(String &response)
{
    int ret = 0;
    int http_code = http.GET();
    //String response;
    if(http_code)                       // get data successful
    {
        Serial.printf("[http] GET code:%d\n", http_code);
        if(http_code == HTTP_CODE_OK)
        {
            response =  http.getString();
            DebugPrintln("get data successfully!!!!");

            //DebugPrintln(response);                       //打印原始数据
           
            ret = 0;
        }
        else
        {
            DebugPrintln("get data error");
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
            //http.disconnect(false);
            http.end();
            http_client_init();
        }
        else
        {
            Serial.printf("[HTTP]get weather data error, cnt[%d]", get_fail_cnt);
        }
        ret = -1;
    }
    http.end();

    return ret;

GET_DATA_ERR:
    {
        get_fail_cnt++;
        if(get_fail_cnt >= GET_DATA_FAIL_MAX)           //超过获取失败最大次数
        {
            Serial.printf("[HTTP] get weather data error cnt full, clean it");
            get_fail_cnt = 0;
            /*reconnect HTTP Server*/
            //http.disconnect(false);
            http.end();
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
void json_data_analyze(String http_data)
{
    DynamicJsonBuffer jsonBuffer;
    char day_temp = 0, night_temp = 0;
    char weakday_data_str[32] = {0};
    char weakday = 0;
    char date[16] = {0};

    char sun_rise_time[8] = {0};
    char sun_down_time[8] = {0};

    //DebugPrintln(http_data);

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

    strcpy(date, root["value"][0]["weathers"][0]["date"]);
    strcpy(sun_rise_time, root["value"][0]["weathers"][0]["sun_rise_time"]);
    strcpy(sun_down_time, root["value"][0]["weathers"][0]["sun_down_time"]);

    all_weather_data.day_weather_data[0].day_temp = day_temp;
    all_weather_data.day_weather_data[0].night_temp = night_temp;
    all_weather_data.day_weather_data[0].weakday = weakday;
    strcpy(all_weather_data.day_weather_data[0].date, date);
    strcpy(all_weather_data.day_weather_data[0].sun_rise_time, sun_rise_time);
    strcpy(all_weather_data.day_weather_data[0].sun_down_time, sun_down_time);

    Serial.printf("day tem[%d] night tem[%d] weak[%d] date[%s]\r\n", day_temp, night_temp, weakday, date);
    Serial.printf("rise time[%s] down time[%s]\r\n", sun_rise_time, sun_down_time);

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
WEATHER_E weather_data_analyze(char weather_data)
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
//WEATHER_DEF_ARRY