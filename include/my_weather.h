#ifndef __MY_WEATHER_H__
#define __MY_WEATHER_H__

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
    char publish_time[32];                   //数据更新时间
}ALL_WEATHER_DATA_T;

const char WEAK_DEF_ARRY[WEAK_MAX][32] = {"星期日","星期一","星期二","星期三","星期四","星期五","星期六"};
const char WEATHER_DEF_ARRY[W_MAX][32] = {"晴","多云","阴","雾","小雨","中雨","大雨","暴雨"}; 



#endif //__MY_WEATHER_H__