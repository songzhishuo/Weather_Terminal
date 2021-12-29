# Weather_Terminal
基于Seeed Wio Terminal 的天气显示终端

![image-20211226163020035](https://gitee.com/song_zhi_shu/my-image-host/raw/master/img/image-20211226163020035.png)

# 基础要求

制作一个自动联网的天气预报仪，在设计界面显示温湿度、天气情况、空气质量以及未来三天内的天气变化。

# 实现思路

基于Arduino 平台设计一款自动同步网络天气的天气显示终端，并且涵盖以下内容。

![image-20211226130313916](https://gitee.com/song_zhi_shu/my-image-host/raw/master/img/image-20211226130313916.png)

# 实现记录

## 天气数据获取

### 天气获取接口

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

### 数据验证

在进行项目构思的时候WIO 还在快递路上，所以这里使用Python 进行了HTTP API的验证，当然也可以采用postman 工具进行验证也是非常便捷的。

```
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

## 网页数据解析

### HTTP数据获取

因为WIO的网路库是和ESP32/ESP8266的网络库API互相兼容的，这里就不赘述了我们直接参考博哥的教程[(33条消息) ESP8266开发之旅 网络篇⑨ HttpClient——ESP8266HTTPClient库的使用_单片机菜鸟哥的博客-CSDN博客](https://blog.csdn.net/dpjcn1990/article/details/92830108)

在这里需要注意虽然我们使用的魅族的天气接口每天不限制请求的次数但是因为网络波动可能出现HTTP断连的情况，在断连之后需要对HTTP对象进行销毁处理，并且重新初始化http对象即可。具体操作可以参考下面代码。

```c++
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
```

### JSON解析

JSON解析依然采用Arduino 官方提供的库，这里需要注意一下库的版本（这里使用5.13.4版本），部分大版本之间是API是不能相互兼容的。

相关API可以参考博哥的教程：[(33条消息) 深入学习ArduinoJson库 V5版本_单片机菜鸟哥的博客-CSDN博客_arduinojson](https://blog.csdn.net/dpjcn1990/article/details/92831612?ops_request_misc=%7B%22request%5Fid%22%3A%22164049698016780366542912%22%2C%22scm%22%3A%2220140713.130102334.pc%5Fblog.%22%7D&request_id=164049698016780366542912&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~blog~first_rank_ecpm_v1~rank_v31_ecpm-2-92831612.nonecase&utm_term=json&spm=1018.2226.3001.4450)



## ICON 适配

### ICON 图片选择

和风天气开发者平台制作了很多的天气图标，只需将其转换为我们所需的格式即可。仓库地址(https://github.com/qwd/Icons/tree/main/icons)。这里我认为和风老的拟物的ICON（Set2）更加好看所以使用了这份老的ICON，仓库地址（[和风天气ICON](https://github.com/songzhishuo/Icons)）

![screenshot](https://gitee.com/song_zhi_shu/my-image-host/raw/master/img/screenshot.png)

### ICON存放

一般我们的ICON图片最小尺寸占用空间也是64*64的RGB图，因此将占用较大的ROM空间，通过查看手册可知WIO 拥有高达512KB的内部存储区，存储几张ICON还是很轻松的，并且还外挂了一颗4MB NOR Flash。如果后续需要进行多图的显示可以将图片转换成二进制文件存放在tf卡中，然后开机加载文件系统的时候将其读如RAM即可。这里我偷个懒直接将图片以.c文件的形式存放在内部ROM中。

![image-20211226132334799](https://gitee.com/song_zhi_shu/my-image-host/raw/master/img/image-20211226132334799.png)

## LVGL适配

Arduino 原生提供的GUI较为的简陋，这里我们使用LVGL来绘制我们的GUI。Seeed官方已经对WIO进行了LVGL的适配（版本为LVGL v7.0.2）为了方便开发这里我们使用`GUI-Guider`+`Vs2019`模拟仿真的方式进行GUI的基础开发。

中文参考手册：http://lvgl.100ask.net/7.11/

### VS2019仿真环境搭建

可以参考如下教程进行搭建：

https://blog.51cto.com/u_15244533/2862443

搭建过程中需要注意版本，目前最新稳定版本为lvgl8，但是Arduino适配的是v7版本，所以一定要下载v7版本。

### GUI Guider 使用

GUI Guider是NXP 开发的一款支持拖拽布局的LVGL 代码生成器，使用这款软件可以让我们在不熟悉LVGL API的时候快速完成基础界面的布局。

![image-20211226141844352](https://gitee.com/song_zhi_shu/my-image-host/raw/master/img/image-20211226141844352.png)

在进行拖拽布局完成之后需要点击菜单栏的`Generate Code`按钮来生成源码，生成的代码在工程目录的`/generated/`目录下并找到`gui_guider.c`文件，一般命名为GUI Guider中手动设置的页面名称。

### 将GUI guider生成文件 进行仿真

#### 拷贝主页面的setup_ui函数

在文件中直接将`setup_ui()`函数拷贝到VS2019工程下并包含头文件即可。这里setup相当于对页面中所有资源的初始化并且将资源加载到主缓存区供lvgl进行调用。可以简单的理解为一个init函数。

```c
#include "lvgl/lvgl.h"		//这里需要替换为#include "lvgl.h" 因为arduino默认其在工程目录下，PIO VS2019仿真同理
#include <stdio.h>
#include "gui_guider.h"

void setup_ui(lv_ui *ui){
	LV_THEME_DEFAULT_INIT(LV_THEME_DEFAULT_COLOR_PRIMARY, LV_THEME_DEFAULT_COLOR_SECONDARY, 
		LV_THEME_MATERIAL_FLAG_LIGHT, LV_THEME_DEFAULT_FONT_SMALL,  LV_THEME_DEFAULT_FONT_NORMAL, 
		LV_THEME_DEFAULT_FONT_SUBTITLE, LV_THEME_DEFAULT_FONT_TITLE);
	setup_scr_home(ui);				//初始化控件
	lv_scr_load(ui->home);			//将控件加载到主屏并显示
}
```

#### 拷贝 setup_scr_home 文件和lv_ui结构体

我们的所以页面布局和控件的样式都是在setup_scr_home 文件中的setup_scr_home（）函数中定义的。因此需要将其原封不动的拷贝到VS2019的工程目录下。

lv_ui 其实是对setup_scr_home 中调用对象实例的一个大集合，其中包含了每个控件实例化之后的指针。因此我们只需要全局初始化一下这个结构体，然后每次操作的时候直接对其中的对象指针进行操作即可。

最后移植完成点击运行即可运行主界面，如下图所示：

![image-20211226143454694](https://gitee.com/song_zhi_shu/my-image-host/raw/master/img/image-20211226143454694.png)

### 在Arduino中运行

​	为了方便可以直接使用如下这个函数框架来运行LVGL，一般情况下loop中除了进行LVGL的任务切换和界面刷新，不再进行其他的操作，其他的操作均单独开辟task进行。

```c++
#include <Arduino.h>
#include "SPI.h"
#include"TFT_eSPI.h"
#include"Free_Fonts.h" 
#include <lvgl.h>


void setup() {
	//spi初始化
	
	//屏幕初始化

	//LVGL 初始化
	lv_init();
 	lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

    /*Initialize the display*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 320;						//屏幕尺寸
    disp_drv.ver_res = 240;					
    disp_drv.flush_cb = my_disp_flush;			//刷新方式
    disp_drv.buffer = &disp_buf;				//屏幕显存区域
    lv_disp_drv_register(&disp_drv);

    /*Initialize the touch pad*/
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = read_encoder;
    lv_indev_drv_register(&indev_drv);
    /*stress test*/

    setup_ui();					//对应上述setup_ui函数中的内容，这里为伪代码需要在适当位置对lv_ui 结构体初始化
}

void loop() {

    lv_task_handler(); /* let the GUI do its work */
    
    timer_tick_now = micros();
    if((timer_tick_now - timer_tick_last) >= 1000)       //1ms
    {
        lv_tick_handler();			//线程调度时基
        timer_tick_last = micros(); 
    }
}

```





在VS2019中运行没有问题之后既可将源码原封不动的拷贝到Arduino工程目录下运行即可，笔者在此步骤中也遇到了部分问题，将在后面问题汇总中阐述。
		



## Arduino 功能实现

### 软件定时

为了方便各个功能周期的运行，这里使用了LVGL的task 任务功能，LVGL的任务功能远远没有FreeRTOS实现的那么严谨，可以理解成一个软件定时器功能，在其回调函数中任务会周期执行。

#### 定时器适配

在使用task任务的时候需要对任务调度进行初始化，就是对任务的调度时间进行一个定义。可以参考LVGL适配章节中的lv_tick_handler 函数的放置位置进行放置。任务创建函数如下图所示：

```C++
lv_task_t * lv_task_create(lv_task_cb_t task_xcb, uint32_t period, lv_task_prio_t prio, void * user_data)
```



#### 代码开辟线程介绍：

time_task_id：定时获取网络时间的线程

button_scan_task：按键状态检测线程，并且进行LVGL页面切换和资源申请释放

network_task：解析HTTP数据和获取天气资源的线程



# 资料链接
Github：https://github.com/songzhishuo/Weather_Terminal/tree/wio_terminal


# 演示视频

https://www.bilibili.com/video/BV1EM4y1F7jJ?share_source=copy_web

# 问题记录

## Platform 安装失败或配置失败

### 问题描述：

在配置pio时，发现更新包总是失败（如下图所示），开启代理后成功。

![image-20211108234728133](https://gitee.com/song_zhi_shu/my-image-host/raw/master/img/image-20211108234728133.png)

### 代理开启方法：

[VSCode — PlatformIO latest documentation](https://docs.platformio.org/en/latest/integration/ide/vscode.html#proxy-server-support)

![image-20211108234916424](https://gitee.com/song_zhi_shu/my-image-host/raw/master/img/image-20211108234916424.png)



配置完成之后如下所示：

![image-20211108235208883](https://gitee.com/song_zhi_shu/my-image-host/raw/master/img/image-20211108235208883.png)

![image-20211108235239052](https://gitee.com/song_zhi_shu/my-image-host/raw/master/img/image-20211108235239052.png)

参考：

[PlatformIO使用中遇到的坑 | spaceman (nu-ll.github.io)](https://nu-ll.github.io/2021/02/24/PlatformIO使用中遇到的坑/)

[pio platform install — PlatformIO latest documentation](https://docs.platformio.org/en/latest//core/userguide/platforms/cmd_install.html)

## 能识别到端口，但是下载失败

### 问题描述：

有时会偶现下载失败的问题，原因为下载端口在下载复位后发生了改变，PlatformIO 无法检测到下载端口。



### 解决方法：

将工程配置清空，从新编译下载。

![image-20211109002150393](https://gitee.com/song_zhi_shu/my-image-host/raw/master/img/image-20211109002150393.png)

## wifi 无响应

### 问题描述

在烧录demo之后串口无打印，wifi模块也无响应。程序卡死在初始化中。这是因RTL8710模组的固件太老需要更新最新的固件便可以使用。

![image-20211112143546913](https://gitee.com/song_zhi_shu/my-image-host/raw/master/img/image-20211112143546913.png)

## LVGL GUI Guider 切换到仿真

lv_style_reset 接口替换成 lv_style_init

## LVGL 使能字体

C:\Users\songz\Desktop\wio\Weather_Terminal\.pio\libdeps\seeed_wio_terminal\Seeed_Arduino_lvgl\src\src\lv_conf_internal.h

![image-20211129230746096](https://gitee.com/song_zhi_shu/my-image-host/raw/master/img/image-20211129230746096.png)

## 图片使能

![image-20211218231114818](https://gitee.com/song_zhi_shu/my-image-host/raw/master/img/image-20211218231114818.png)

## LVGL 时间片调度

wio arduino 下没找到硬件定时器的API，所以我们通过获取**micros(void)**或者**millis( void )** 来进行定时。

* micros 获取系统运行的微秒数
* millis 获取系统运行的毫秒数

然后改写我们的loop函数如下所示。

```c++
void loop() {

#if (LVGL == 1)  
    lv_task_handler(); /* let the GUI do its work */
#endif    
    
    timer_tick_now = micros();
    if((timer_tick_now - timer_tick_last) >= 1000)       //1ms
    {
        lv_tick_handler();
        timer_tick_last = micros(); 
    }
}
```



![00](https://gitee.com/song_zhi_shu/my-image-host/raw/master/img/00.BMP)

![image-20211210214925857](https://gitee.com/song_zhi_shu/my-image-host/raw/master/img/image-20211210214925857.png)



## PIO 编译wio 提示SPI.h找不到

![image-20211215224314043](https://gitee.com/song_zhi_shu/my-image-host/raw/master/img/image-20211215224314043.png)

默认情况下我们包含的是wio的标准arduino库，鬼知道这里是怎么链接的。。。。就是找不到。WTF，调试了半天发现在`main.cpp` 中包含"SPI.h"即可。

![image-20211215224546558](https://gitee.com/song_zhi_shu/my-image-host/raw/master/img/image-20211215224546558.png)
