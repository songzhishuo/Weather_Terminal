/**
 * @file lv_demo_stress.h
 *
 */

#ifndef LV_DEMO_STRESS_H
#define LV_DEMO_STRESS_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"
#include "my_weather.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void lv_demo_stress(void);
void change_page(char page);
void lv_set_time(char *time);
void lv_set_date(char *date);

void lv_set_so2_val(uint16_t date);
void lv_set_pm25_val(uint16_t date);
void lv_set_temp_val(uint16_t date);
void lv_set_weather_icon(uint8_t icon_id);

void chart_test(ALL_WEATHER_DATA_T data);          /*折线绘制测试*/
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_STRESS_H*/
