/*
 * @Author: Argon
 * @Date: 2021-11-02 15:41:41
 * @LastEditTime: 2021-11-03 15:24:06
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

bool autoConfig(void);
void smartConfig(void);
void http_client_init(void);
int get_http_data(String response);
void json_data_analyze(String http_data);

int get_fail_cnt = 0;           //HTTP数据获取失败计数
int flag = HIGH;//默认当前灭灯
long lastTime = 0;


String url_data = "http://aider.meizu.com/app/weather/listWeather?cityIds=";
String city_id = "101210101";

HTTPClient http;
WiFiClient client;

typedef enum{
    W_SUN = 0,              //晴天
    W_PARTLY_CLOUDY,        //多云
    W_OVERCAST,             //阴天
    W_LIGHT_RAIN,           //小雨
    W_MODERATE_RAIN,        //中雨
    W_HEAVY_RAIN,           //大雨
    W_RAINSTORM,            //暴雨
    W_UNKNOW,               //未知天气
    W_E_MAX,                //最大范围
}WEATHER_E;


typedef struct{
    int day_temp;
    int night_temp;
    WEATHER_E weather;              //天气  
    char date[12];                  //2021-11-03
    char sun_rise_time[6];
    char sun_down_time[6];
    
}DAY_WEATHER_T;


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
    url_data = url_data + city_id;

    DebugPrint("HTTP url :");
    DebugPrintln(url_data);

    http.setTimeout(5000);      //http超时时间5S
    http.begin(client,url_data);

    /*代理信息设置*/
}

int get_http_data(String response)
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

            DebugPrintln(response);
           
            ret = 0;
        }
        else
        {
            DebugPrintln("get data error");
            ret = -1;
        }

    }
    else 
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
        ret = -1;
    }
    http.end();

    return ret;
}

//天气数据解析
void json_data_analyze(String http_data)
{
    //DynamicJsonBuffer json_buffer;    //5K
    DynamicJsonBuffer jsonBuffer;
    //String today_temp;
    JsonObject& root = jsonBuffer.parseObject(http_data);
    if(!root.success())
    {
        DebugPrintln("analyze data error"); 
        return ;
    }

    // char str[15] = {0};
    // String today_temp;
    // strcpy(str, root["value"][0]["weathers"][0]["temp_day_c"]);
    // today_temp= str;

    int day_temp = root["value"][0]["weathers"][0]["temp_day_c"];
    DebugPrint("--->today Template :");
    DebugPrintln(day_temp);

   // DebugPrint("--->tomorrow Template data");
   // DebugPrintln(root["value"][0]["weathers"][1]["temp_day_c"]);

    //root.

  //String output;
  //root.printTo(output);


}
