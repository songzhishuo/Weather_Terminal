<!--
 * @Author: Argon
 * @Date: 2021-11-04 15:00:04
 * @LastEditTime: 2021-11-08 10:55:02
 * @LastEditors: Argon
 * @Description: 
 * @FilePath: \esp8266_get_weather\term.md
 * 1Talk is cheap. Show me the code. (*｀皿´*)ﾉ 
-->
# 数据获取

## 天气获取接口

可以通过魅族的天气API接口获取未来5天天气：

http://aider.meizu.com/app/weather/listWeather?cityIds=101240101

### 基本信息：

| 接口说明 |                                                              |
| -------- | ------------------------------------------------------------ |
| 接口地址 | http://aider.meizu.com/app/weather/listWeather?cityIds=101240101 |
| 返回格式 | json                                                         |
| 请求方式 | get/post                                                     |

### 请求参数说明：

| 名称    | 类型   | 必填 | 说明   |
| ------- | ------ | ---- | ------ |
| cityIds | string | 必填 | 城市ID |

### 范围参数说明：

| 名称 | 类型 | 说明 |
| ---- | ---- | ---- |
|      |      |      |
|      |      |      |
|      |      |      |



### 注：

#### 城市编号ID：

https://docs.qq.com/sheet/DSk9STW5ta2lzRlhM?tab=BB08J2

#### 常用API接口网站汇总：

https://www.free-api.com/ 这个网站上有很多免费的API接口可以参考使用。

## 数据验证

```python
import  requests
import  json

__WEATHER_URL_API = 'http://aider.meizu.com/app/weather/listWeather?cityIds='   ## meizu phone get weather API, this api needn't using API key or licence
__CITY_ID = '101210101'       ##city id 杭州

json_data = requests.get(__WEATHER_URL_API+__CITY_ID)
json_data.encoding = 'utf-8'
print(json_data.json())
out_data = json.dumps(json_data.json(),indent=4)
print(out_data)

# get tomorrow day temperature
print(json_data['value'][0]['weathers'][1]['temp_day_c'])

```

# ICON 适配
和风天气开发者平台制作了很多的天气图标，只需将其转换为我们所需的格式即可。仓库地址(https://github.com/qwd/Icons/tree/main/icons)

