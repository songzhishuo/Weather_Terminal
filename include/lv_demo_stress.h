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
void set_time(char hour,char min);
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_STRESS_H*/
