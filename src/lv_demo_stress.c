/**
 * @file lv_demo_stress.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_demo_stress.h"

/*********************
 *      DEFINES
 *********************/
#define TIME_STEP   50

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void auto_del(lv_obj_t * obj, uint32_t delay);
static void obj_test_task_cb(lv_task_t *);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t * main_page;
static lv_obj_t * ta;
static lv_style_t style_slow_trans;
static const char * mbox_btns[] = {"Ok", "Cancel", ""};
static uint32_t mem_free_start = 0;
/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
typedef struct
{
    lv_chart_series_t * home1_chart_tem_0;      /*高温 曲线*/
    lv_chart_series_t * home1_chart_tem_1;      /*低温 曲线*/
}lv_chart_t;



typedef struct
{
    lv_obj_t* home;
    lv_obj_t* home_cont_top;
    lv_obj_t* home_labeldate;
    lv_obj_t* home_cont_down;
    lv_obj_t* home_cont_master;
    lv_obj_t* home_label_time;
    lv_obj_t *home_img_weather_sun;


	lv_obj_t *home1;
	lv_obj_t *home1_cont_1;
	lv_obj_t *home1_cont_top;
	lv_obj_t *home1_cont_down;
	lv_obj_t *home1_chart_tem;              /*折线图*/

    lv_chart_t home1_chart_line;             /*chart 控件 中折线*/
}lv_ui;

LV_IMG_DECLARE(_100_alpha_100x100);

static lv_ui test;              /*label 全局定义*/

// lv_chart_series_t * home1_chart_tem_0;
// lv_chart_series_t * home1_chart_tem_1;

void lv_set_time(char *time)
{
    lv_label_set_text(test.home_label_time, time);
}

void lv_set_date(char *date)
{
    lv_label_set_text(test.home_labeldate, date);
}

void change_page(char page)         /*页面切换*/
{
    lv_obj_clean(lv_scr_act());
    if(page == 0)
    {
        demo_test1(&test);    
        lv_scr_load(test.home1);
    }
    else if(page == 1)
    {
        demo_test(&test);
        lv_scr_load(test.home);
    }
}

void demo_test(lv_ui* ui) {

    //Write codes home
    ui->home = lv_obj_create(NULL, NULL);   //lv_scr_act();
    
    //Write codes home_cont_top
    ui->home_cont_top = lv_cont_create(ui->home, NULL);

    //Write style LV_CONT_PART_MAIN for home_cont_top
    static lv_style_t style_home_cont_top_main;
    lv_style_init(&style_home_cont_top_main);

    //Write style state: LV_STATE_DEFAULT for style_home_cont_top_main
    lv_style_set_radius(&style_home_cont_top_main, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_color(&style_home_cont_top_main, LV_STATE_DEFAULT, lv_color_make(0x2f, 0x32, 0x43));
    lv_style_set_bg_grad_color(&style_home_cont_top_main, LV_STATE_DEFAULT, lv_color_make(0x2f, 0x32, 0x43));
    lv_style_set_bg_grad_dir(&style_home_cont_top_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    lv_style_set_bg_opa(&style_home_cont_top_main, LV_STATE_DEFAULT, 255);
    lv_style_set_border_color(&style_home_cont_top_main, LV_STATE_DEFAULT, lv_color_make(0x2f, 0x32, 0x43));
    lv_style_set_border_width(&style_home_cont_top_main, LV_STATE_DEFAULT, 1);
    lv_style_set_border_opa(&style_home_cont_top_main, LV_STATE_DEFAULT, 255);
    lv_style_set_pad_left(&style_home_cont_top_main, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&style_home_cont_top_main, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_top(&style_home_cont_top_main, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_bottom(&style_home_cont_top_main, LV_STATE_DEFAULT, 0);
    lv_obj_add_style(ui->home_cont_top, LV_CONT_PART_MAIN, &style_home_cont_top_main);
    lv_obj_set_pos(ui->home_cont_top, 0, 0);
    lv_obj_set_size(ui->home_cont_top, 320, 100);
    lv_obj_set_click(ui->home_cont_top, false);
    lv_cont_set_layout(ui->home_cont_top, LV_LAYOUT_OFF);
    lv_cont_set_fit(ui->home_cont_top, LV_FIT_NONE);

    //Write style state: LV_STATE_DEFAULT for style_home_cont_top_main
    lv_style_set_radius(&style_home_cont_top_main, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_color(&style_home_cont_top_main, LV_STATE_DEFAULT, lv_color_make(0x2f, 0x32, 0x43));
    lv_style_set_bg_grad_color(&style_home_cont_top_main, LV_STATE_DEFAULT, lv_color_make(0x2f, 0x32, 0x43));
    lv_style_set_bg_grad_dir(&style_home_cont_top_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    lv_style_set_bg_opa(&style_home_cont_top_main, LV_STATE_DEFAULT, 255);
    lv_style_set_border_color(&style_home_cont_top_main, LV_STATE_DEFAULT, lv_color_make(0x2f, 0x32, 0x43));
    lv_style_set_border_width(&style_home_cont_top_main, LV_STATE_DEFAULT, 1);
    lv_style_set_border_opa(&style_home_cont_top_main, LV_STATE_DEFAULT, 255);
    lv_style_set_pad_left(&style_home_cont_top_main, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&style_home_cont_top_main, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_top(&style_home_cont_top_main, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_bottom(&style_home_cont_top_main, LV_STATE_DEFAULT, 0);
    lv_obj_add_style(ui->home_cont_top, LV_CONT_PART_MAIN, &style_home_cont_top_main);
    lv_obj_set_pos(ui->home_cont_top, 0, 0);
    lv_obj_set_size(ui->home_cont_top, 320, 100);
    lv_obj_set_click(ui->home_cont_top, false);
    lv_cont_set_layout(ui->home_cont_top, LV_LAYOUT_OFF);
    lv_cont_set_fit(ui->home_cont_top, LV_FIT_NONE);



    //Write codes home_cont_down
    ui->home_cont_down = lv_cont_create(ui->home, NULL);

    //Write style LV_CONT_PART_MAIN for home_cont_down
    static lv_style_t style_home_cont_1_main;
    lv_style_init(&style_home_cont_1_main);

    //Write style state: LV_STATE_DEFAULT for style_home_cont_1_main
    lv_style_set_radius(&style_home_cont_1_main, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_color(&style_home_cont_1_main, LV_STATE_DEFAULT, lv_color_make(0x2f, 0x32, 0x43));
    lv_style_set_bg_grad_color(&style_home_cont_1_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
    lv_style_set_bg_grad_dir(&style_home_cont_1_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    lv_style_set_bg_opa(&style_home_cont_1_main, LV_STATE_DEFAULT, 255);
    lv_style_set_border_color(&style_home_cont_1_main, LV_STATE_DEFAULT, lv_color_make(0x2f, 0x32, 0x43));
    lv_style_set_border_width(&style_home_cont_1_main, LV_STATE_DEFAULT, 1);
    lv_style_set_border_opa(&style_home_cont_1_main, LV_STATE_DEFAULT, 255);
    lv_style_set_pad_left(&style_home_cont_1_main, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&style_home_cont_1_main, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_top(&style_home_cont_1_main, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_bottom(&style_home_cont_1_main, LV_STATE_DEFAULT, 0);
    lv_obj_add_style(ui->home_cont_down, LV_CONT_PART_MAIN, &style_home_cont_1_main);
    lv_obj_set_pos(ui->home_cont_down, 0, 98);
    lv_obj_set_size(ui->home_cont_down, 320, 142);
    lv_obj_set_click(ui->home_cont_down, false);
    lv_cont_set_layout(ui->home_cont_down, LV_LAYOUT_OFF);
    lv_cont_set_fit(ui->home_cont_down, LV_FIT_NONE);


	//Write codes home_cont_master
	ui->home_cont_master = lv_cont_create(ui->home, NULL);

	//Write style LV_CONT_PART_MAIN for home_cont_master
	static lv_style_t style_home_cont_master_main;
	lv_style_init(&style_home_cont_master_main);

	//Write style state: LV_STATE_DEFAULT for style_home_cont_master_main
	lv_style_set_radius(&style_home_cont_master_main, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_color(&style_home_cont_master_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_home_cont_master_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_home_cont_master_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home_cont_master_main, LV_STATE_DEFAULT, 70);
	lv_style_set_border_color(&style_home_cont_master_main, LV_STATE_DEFAULT, lv_color_make(0x99, 0x99, 0x99));
	lv_style_set_border_width(&style_home_cont_master_main, LV_STATE_DEFAULT, 1);
	lv_style_set_border_opa(&style_home_cont_master_main, LV_STATE_DEFAULT, 255);
	lv_style_set_pad_left(&style_home_cont_master_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_home_cont_master_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_home_cont_master_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_home_cont_master_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->home_cont_master, LV_CONT_PART_MAIN, &style_home_cont_master_main);
	lv_obj_set_pos(ui->home_cont_master, 25, 75);
	lv_obj_set_size(ui->home_cont_master, 265, 150);
	lv_obj_set_click(ui->home_cont_master, false);
	lv_cont_set_layout(ui->home_cont_master, LV_LAYOUT_OFF);
	lv_cont_set_fit(ui->home_cont_master, LV_FIT_NONE);

    //Write codes home_labeldate
    ui->home_labeldate = lv_label_create(ui->home, NULL);
    lv_label_set_text(ui->home_labeldate, "20 Nov 2020");
    lv_label_set_long_mode(ui->home_labeldate, LV_LABEL_LONG_BREAK);
    lv_label_set_align(ui->home_labeldate, LV_LABEL_ALIGN_RIGHT);

    //Write style LV_LABEL_PART_MAIN for home_labeldate
    static lv_style_t style_home_labeldate_main;
    lv_style_init(&style_home_labeldate_main);

    //Write style state: LV_STATE_DEFAULT for style_home_labeldate_main
    lv_style_set_radius(&style_home_labeldate_main, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_color(&style_home_labeldate_main, LV_STATE_DEFAULT, lv_color_make(0x2f, 0x32, 0x43));
    lv_style_set_bg_grad_color(&style_home_labeldate_main, LV_STATE_DEFAULT, lv_color_make(0x2f, 0x32, 0x43));
    lv_style_set_bg_grad_dir(&style_home_labeldate_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    lv_style_set_bg_opa(&style_home_labeldate_main, LV_STATE_DEFAULT, 255);
    lv_style_set_text_color(&style_home_labeldate_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
    lv_style_set_text_font(&style_home_labeldate_main, LV_STATE_DEFAULT, &lv_font_montserrat_16);
    lv_style_set_text_letter_space(&style_home_labeldate_main, LV_STATE_DEFAULT, 2);
    lv_style_set_pad_left(&style_home_labeldate_main, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&style_home_labeldate_main, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_top(&style_home_labeldate_main, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_bottom(&style_home_labeldate_main, LV_STATE_DEFAULT, 0);
    lv_obj_add_style(ui->home_labeldate, LV_LABEL_PART_MAIN, &style_home_labeldate_main);
    lv_obj_set_pos(ui->home_labeldate, 92, 48);
    lv_obj_set_size(ui->home_labeldate, 217, 0);

    //Write codes home_label_time
    ui->home_label_time = lv_label_create(ui->home, NULL);
    lv_label_set_text(ui->home_label_time, "08:00");
    lv_label_set_long_mode(ui->home_label_time, LV_LABEL_LONG_BREAK);
    lv_label_set_align(ui->home_label_time, LV_LABEL_ALIGN_RIGHT);

    //Write style LV_LABEL_PART_MAIN for home_label_time
    static lv_style_t style_home_label_time_main;
    lv_style_init(&style_home_label_time_main);

    //Write style state: LV_STATE_DEFAULT for style_home_label_time_main
    lv_style_set_radius(&style_home_label_time_main, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_color(&style_home_label_time_main, LV_STATE_DEFAULT, lv_color_make(0x2f, 0x32, 0x43));
    lv_style_set_bg_grad_color(&style_home_label_time_main, LV_STATE_DEFAULT, lv_color_make(0x2f, 0x32, 0x43));
    lv_style_set_bg_grad_dir(&style_home_label_time_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    lv_style_set_bg_opa(&style_home_label_time_main, LV_STATE_DEFAULT, 255);
    lv_style_set_text_color(&style_home_label_time_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
    lv_style_set_text_font(&style_home_label_time_main, LV_STATE_DEFAULT, &lv_font_montserrat_32);          // need open LV_FONT_MONTSERRAT_32 define 
    lv_style_set_text_letter_space(&style_home_label_time_main, LV_STATE_DEFAULT, 2);
    lv_style_set_pad_left(&style_home_label_time_main, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&style_home_label_time_main, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_top(&style_home_label_time_main, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_bottom(&style_home_label_time_main, LV_STATE_DEFAULT, 0);
    lv_obj_add_style(ui->home_label_time, LV_LABEL_PART_MAIN, &style_home_label_time_main);
    lv_obj_set_pos(ui->home_label_time, 94, 8);
    lv_obj_set_size(ui->home_label_time, 217, 0);


	//Write codes home_img_weather_sun
	ui->home_img_weather_sun = lv_img_create(ui->home, NULL);

	//Write style LV_IMG_PART_MAIN for home_img_weather_sun
	static lv_style_t style_home_img_weather_sun_main;
	lv_style_init(&style_home_img_weather_sun_main);

	//Write style state: LV_STATE_DEFAULT for style_home_img_weather_sun_main
	lv_style_set_image_recolor(&style_home_img_weather_sun_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_home_img_weather_sun_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_home_img_weather_sun_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->home_img_weather_sun, LV_IMG_PART_MAIN, &style_home_img_weather_sun_main);
	lv_obj_set_pos(ui->home_img_weather_sun, 39, 87);
	lv_obj_set_size(ui->home_img_weather_sun, 100, 100);
	lv_obj_set_click(ui->home_img_weather_sun, true);
	lv_img_set_src(ui->home_img_weather_sun,&_100_alpha_100x100);
	lv_img_set_pivot(ui->home_img_weather_sun, 0,0);
	lv_img_set_angle(ui->home_img_weather_sun, 0);

}

void demo_test1(lv_ui* ui) {

	//Write codes home1
	//ui->home1 = lv_obj_create(NULL, NULL);
    ui->home1 = lv_obj_create(NULL, NULL);//lv_scr_act();

	//Write codes home1_cont_1
	ui->home1_cont_1 = lv_cont_create(ui->home1, NULL);

	//Write style LV_CONT_PART_MAIN for home1_cont_1
	static lv_style_t style_home1_cont_1_main;
	lv_style_init(&style_home1_cont_1_main);

	//Write style state: LV_STATE_DEFAULT for style_home1_cont_1_main
	lv_style_set_radius(&style_home1_cont_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_home1_cont_1_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_home1_cont_1_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_home1_cont_1_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home1_cont_1_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_home1_cont_1_main, LV_STATE_DEFAULT, lv_color_make(0x99, 0x99, 0x99));
	lv_style_set_border_width(&style_home1_cont_1_main, LV_STATE_DEFAULT, 1);
	lv_style_set_border_opa(&style_home1_cont_1_main, LV_STATE_DEFAULT, 255);
	lv_style_set_pad_left(&style_home1_cont_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_home1_cont_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_home1_cont_1_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_home1_cont_1_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->home1_cont_1, LV_CONT_PART_MAIN, &style_home1_cont_1_main);
	lv_obj_set_pos(ui->home1_cont_1, 48, 108);
	lv_obj_set_size(ui->home1_cont_1, 200, 100);
	lv_obj_set_click(ui->home1_cont_1, false);
	lv_cont_set_layout(ui->home1_cont_1, LV_LAYOUT_OFF);
	lv_cont_set_fit(ui->home1_cont_1, LV_FIT_NONE);

	//Write codes home1_cont_top
	ui->home1_cont_top = lv_cont_create(ui->home1, NULL);

	//Write style LV_CONT_PART_MAIN for home1_cont_top
	static lv_style_t style_home1_cont_top_main;
	lv_style_init(&style_home1_cont_top_main);

	//Write style state: LV_STATE_DEFAULT for style_home1_cont_top_main
	lv_style_set_radius(&style_home1_cont_top_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_home1_cont_top_main, LV_STATE_DEFAULT, lv_color_make(0x2f, 0x32, 0x43));
	lv_style_set_bg_grad_color(&style_home1_cont_top_main, LV_STATE_DEFAULT, lv_color_make(0x2f, 0x32, 0x43));
	lv_style_set_bg_grad_dir(&style_home1_cont_top_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home1_cont_top_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_home1_cont_top_main, LV_STATE_DEFAULT, lv_color_make(0x2f, 0x32, 0x43));
	lv_style_set_border_width(&style_home1_cont_top_main, LV_STATE_DEFAULT, 1);
	lv_style_set_border_opa(&style_home1_cont_top_main, LV_STATE_DEFAULT, 255);
	lv_style_set_pad_left(&style_home1_cont_top_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_home1_cont_top_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_home1_cont_top_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_home1_cont_top_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->home1_cont_top, LV_CONT_PART_MAIN, &style_home1_cont_top_main);
	lv_obj_set_pos(ui->home1_cont_top, 0, 0);
	lv_obj_set_size(ui->home1_cont_top, 320, 100);
	lv_obj_set_click(ui->home1_cont_top, false);
	lv_cont_set_layout(ui->home1_cont_top, LV_LAYOUT_OFF);
	lv_cont_set_fit(ui->home1_cont_top, LV_FIT_NONE);

	//Write codes home1_cont_down
	ui->home1_cont_down = lv_cont_create(ui->home1, NULL);

	//Write style LV_CONT_PART_MAIN for home1_cont_down
	static lv_style_t style_home1_cont_down_main;
	lv_style_init(&style_home1_cont_down_main);

	//Write style state: LV_STATE_DEFAULT for style_home1_cont_down_main
	lv_style_set_radius(&style_home1_cont_down_main, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&style_home1_cont_down_main, LV_STATE_DEFAULT, lv_color_make(0x2f, 0x32, 0x43));
	lv_style_set_bg_grad_color(&style_home1_cont_down_main, LV_STATE_DEFAULT, lv_color_make(0x44, 0x3c, 0x3c));
	lv_style_set_bg_grad_dir(&style_home1_cont_down_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_home1_cont_down_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_home1_cont_down_main, LV_STATE_DEFAULT, lv_color_make(0x2f, 0x32, 0x43));
	lv_style_set_border_width(&style_home1_cont_down_main, LV_STATE_DEFAULT, 1);
	lv_style_set_border_opa(&style_home1_cont_down_main, LV_STATE_DEFAULT, 255);
	lv_style_set_pad_left(&style_home1_cont_down_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_home1_cont_down_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&style_home1_cont_down_main, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_home1_cont_down_main, LV_STATE_DEFAULT, 0);
	lv_obj_add_style(ui->home1_cont_down, LV_CONT_PART_MAIN, &style_home1_cont_down_main);
	lv_obj_set_pos(ui->home1_cont_down, 0, 98);
	lv_obj_set_size(ui->home1_cont_down, 320, 142);
	lv_obj_set_click(ui->home1_cont_down, false);
	lv_cont_set_layout(ui->home1_cont_down, LV_LAYOUT_OFF);
	lv_cont_set_fit(ui->home1_cont_down, LV_FIT_NONE);

	// //Write codes home1_chart_tem
	// ui->home1_chart_tem = lv_chart_create(ui->home1, NULL);

	// //Write style LV_CHART_PART_BG for home1_chart_tem
	// static lv_style_t style_home1_chart_tem_bg;
	// lv_style_init(&style_home1_chart_tem_bg);

	// //Write style state: LV_STATE_DEFAULT for style_home1_chart_tem_bg
	// lv_style_set_bg_color(&style_home1_chart_tem_bg, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	// lv_style_set_bg_grad_color(&style_home1_chart_tem_bg, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	// lv_style_set_bg_grad_dir(&style_home1_chart_tem_bg, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	// lv_style_set_bg_opa(&style_home1_chart_tem_bg, LV_STATE_DEFAULT, 60);
	// lv_style_set_pad_left(&style_home1_chart_tem_bg, LV_STATE_DEFAULT, 5);
	// lv_style_set_pad_right(&style_home1_chart_tem_bg, LV_STATE_DEFAULT, 5);
	// lv_style_set_pad_top(&style_home1_chart_tem_bg, LV_STATE_DEFAULT, 5);
	// lv_style_set_pad_bottom(&style_home1_chart_tem_bg, LV_STATE_DEFAULT, 5);
	// lv_obj_add_style(ui->home1_chart_tem, LV_CHART_PART_BG, &style_home1_chart_tem_bg);

	// //Write style LV_CHART_PART_SERIES_BG for home1_chart_tem
	// static lv_style_t style_home1_chart_tem_series_bg;
	// lv_style_init(&style_home1_chart_tem_series_bg);

	// //Write style state: LV_STATE_DEFAULT for style_home1_chart_tem_series_bg
	// lv_style_set_line_color(&style_home1_chart_tem_series_bg, LV_STATE_DEFAULT, lv_color_make(0xe8, 0xe8, 0xe8));
	// lv_style_set_line_width(&style_home1_chart_tem_series_bg, LV_STATE_DEFAULT, 2);
	// lv_style_set_line_opa(&style_home1_chart_tem_series_bg, LV_STATE_DEFAULT, 255);
	// lv_obj_add_style(ui->home1_chart_tem, LV_CHART_PART_SERIES_BG, &style_home1_chart_tem_series_bg);
	// lv_obj_set_pos(ui->home1_chart_tem, 22, 28);
	// lv_obj_set_size(ui->home1_chart_tem, 277, 192);
	// lv_chart_set_type(ui->home1_chart_tem,LV_CHART_TYPE_LINE);
	// lv_chart_set_range(ui->home1_chart_tem,0,100);
	// lv_chart_set_div_line_count(ui->home1_chart_tem, 3, 5);
	// lv_chart_set_point_count(ui->home1_chart_tem, 5);
	// lv_chart_series_t * home1_chart_tem_0 = lv_chart_add_series(ui->home1_chart_tem, lv_color_make(0xff, 0x66, 0x66));
	// lv_chart_set_next(ui->home1_chart_tem, home1_chart_tem_0,10);
	// lv_chart_series_t * home1_chart_tem_1 = lv_chart_add_series(ui->home1_chart_tem, lv_color_make(0x65, 0xec, 0x69));
	// lv_chart_set_next(ui->home1_chart_tem, home1_chart_tem_1,20);


   //Write codes home1_chart_1
    ui->home1_chart_tem = lv_chart_create(ui->home1, NULL);

    //Write style LV_CHART_PART_BG for home1_chart_1
    static lv_style_t style_home1_chart_1_bg;
    lv_style_init(&style_home1_chart_1_bg);

    //Write style state: LV_STATE_DEFAULT for style_home1_chart_1_bg
    lv_style_set_bg_color(&style_home1_chart_1_bg, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
    lv_style_set_bg_grad_color(&style_home1_chart_1_bg, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
    lv_style_set_bg_grad_dir(&style_home1_chart_1_bg, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    lv_style_set_bg_opa(&style_home1_chart_1_bg, LV_STATE_DEFAULT, 255);
    lv_style_set_pad_left(&style_home1_chart_1_bg, LV_STATE_DEFAULT, 40);
    lv_style_set_pad_right(&style_home1_chart_1_bg, LV_STATE_DEFAULT, 5);
    lv_style_set_pad_top(&style_home1_chart_1_bg, LV_STATE_DEFAULT, 10);
    lv_style_set_pad_bottom(&style_home1_chart_1_bg, LV_STATE_DEFAULT, 10);
    lv_obj_add_style(ui->home1_chart_tem, LV_CHART_PART_BG, &style_home1_chart_1_bg);

    //Write style LV_CHART_PART_SERIES_BG for home1_chart_1
    static lv_style_t style_home1_chart_1_series_bg;
    lv_style_init(&style_home1_chart_1_series_bg);

    //Write style state: LV_STATE_DEFAULT for style_home1_chart_1_series_bg
    lv_style_set_line_color(&style_home1_chart_1_series_bg, LV_STATE_DEFAULT, lv_color_make(0xe8, 0xe8, 0xe8));
    lv_style_set_line_width(&style_home1_chart_1_series_bg, LV_STATE_DEFAULT, 2);
    lv_style_set_line_opa(&style_home1_chart_1_series_bg, LV_STATE_DEFAULT, 255);
    lv_obj_add_style(ui->home1_chart_tem, LV_CHART_PART_SERIES_BG, &style_home1_chart_1_series_bg);
    lv_obj_set_pos(ui->home1_chart_tem, 16, 26);
    lv_obj_set_size(ui->home1_chart_tem, 292, 203);
    lv_chart_set_type(ui->home1_chart_tem, LV_CHART_TYPE_LINE);
    lv_chart_set_range(ui->home1_chart_tem, 0, 100);
    lv_chart_set_div_line_count(ui->home1_chart_tem, 3, 3);                           //修改下划线的个数
    //lv_chart_set_series_darking(ui->home1_chart_1, LV_OPA_80);
    lv_chart_set_point_count(ui->home1_chart_tem, 5);
    //lv_chart_series_t* 

	//lv_chart_series_t * home1_chart_tem_0 = lv_chart_add_series(ui->home1_chart_tem, lv_color_make(0xff, 0x66, 0x66));
	ui->home1_chart_line.home1_chart_tem_0 = lv_chart_add_series(ui->home1_chart_tem, lv_color_make(0xff, 0x66, 0x66));
	//lv_chart_set_next(ui->home1_chart_tem, home1_chart_1_0, 30);
	
    //lv_chart_series_t * home1_chart_tem_1 = lv_chart_add_series(ui->home1_chart_tem, lv_color_make(0x65, 0xec, 0x69));
	ui->home1_chart_line.home1_chart_tem_1 = lv_chart_add_series(ui->home1_chart_tem, lv_color_make(0x65, 0xec, 0x69));
	//lv_chart_set_next(ui->home1_chart_tem, home1_chart_tem_1,10);

    lv_chart_set_range(ui->home1_chart_tem, 0, 40);
    lv_chart_set_y_tick_length(ui->home1_chart_tem, 10, 5);
    lv_chart_set_y_tick_texts(ui->home1_chart_tem, "40\n30\n20\n10\n0", 4, LV_CHART_AXIS_DRAW_LAST_TICK);

    /*添加渐变*/
    lv_obj_set_style_local_bg_opa(ui->home1_chart_tem, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, LV_OPA_50); /*Max. opa.*/
    lv_obj_set_style_local_bg_grad_dir(ui->home1_chart_tem, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
    lv_obj_set_style_local_bg_main_stop(ui->home1_chart_tem, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 255);    /*Max opa on the top*/
    lv_obj_set_style_local_bg_grad_stop(ui->home1_chart_tem, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 50);      /*Transparent on the bottom*/


}

void update_time(void* arg)
{
    static char hour = 0, min = 0, sec = 0;
    char time_buf[10] = { 0 };
    //printf("timer down \r\n");

    sec++;
    if (sec >= 60)
    {
        sec = 0;
        min++;
        if (min >= 60)
        {
            min = 0;
            hour++;
            if (hour >= 24)
            {
                hour = 0;
            }
        }


    }
    sprintf(time_buf, "%02d:%02d:%02d", hour, min, sec);
    lv_label_set_text(test.home_label_time, time_buf);
}

void lv_demo_stress(void)
{
    demo_test(&test);
    lv_scr_load(test.home);

    //demo_test(&test);
    //demo_test1(&test);
    //lv_scr_load(test.home1);
    /*创建时钟任务*/
    //lv_task_create(update_time, 1000, LV_TASK_PRIO_LOW, NULL);  // 1秒任务
}

#if 0
void lv_demo_stress(void)
{
    lv_task_create(obj_test_task_cb, TIME_STEP, LV_TASK_PRIO_MID, NULL);

    lv_style_init(&style_slow_trans);
    lv_style_set_bg_opa(&style_slow_trans, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_bg_color(&style_slow_trans, LV_STATE_DISABLED, LV_COLOR_SILVER);
    lv_style_set_transition_prop_1(&style_slow_trans, LV_STATE_DEFAULT, LV_STYLE_BG_COLOR);
    lv_style_set_transition_time(&style_slow_trans, LV_STATE_DEFAULT, TIME_STEP * 10);

    lv_mem_monitor_t mon;
    lv_mem_monitor(&mon);
    mem_free_start = mon.free_size;
}
#endif
/**********************
 *   STATIC FUNCTIONS
 **********************/

static void obj_test_task_cb(lv_task_t * param)
{
    (void) param;    /*Unused*/
    static int16_t state = -1;

    lv_anim_t a;
    lv_obj_t * obj;

    switch(state) {
        case -1:
        {
            lv_res_t res = lv_mem_test();
            if(res != LV_RES_OK) LV_LOG_ERROR("Memory integrity error");

            lv_mem_monitor_t mon;
            lv_mem_monitor(&mon);
            volatile int32_t diff = mem_free_start - mon.free_size;
            LV_LOG_USER("mem leak since start: %d, frag: %3d %%",  diff, mon.frag_pct);
        }
            break;
        case 0:
            /* Holder for all object types */
            main_page = lv_page_create(lv_scr_act(), NULL);
            lv_page_set_anim_time(main_page, TIME_STEP);
            lv_obj_set_size(main_page, LV_HOR_RES / 2, LV_VER_RES);
            lv_page_set_scrl_layout(main_page, LV_LAYOUT_PRETTY_MID);


            obj = lv_btn_create(main_page, NULL);
            lv_obj_set_size(obj, 100, 70);
            obj = lv_label_create(obj, NULL);
            lv_label_set_text(obj, "Multi line\n"LV_SYMBOL_OK LV_SYMBOL_CLOSE LV_SYMBOL_WIFI);
            break;

        case 1:
              obj = lv_tabview_create(lv_scr_act(), NULL);
              lv_obj_set_size(obj, LV_HOR_RES / 2, LV_VER_RES / 2);
              lv_obj_align(obj, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
              {
                  lv_obj_t * t = lv_tabview_add_tab(obj, "First");

                  t = lv_tabview_add_tab(obj, "Second");
                  lv_obj_t * c = lv_cpicker_create(t, NULL);
                  lv_obj_set_size(c,  150, 150);
                  c = lv_led_create(t, NULL);
                  lv_obj_set_pos(c, 160, 20);
                  t = lv_tabview_add_tab(obj, LV_SYMBOL_EDIT " Edit");
                  t = lv_tabview_add_tab(obj, LV_SYMBOL_CLOSE);

                  lv_tabview_set_anim_time(obj, TIME_STEP * 2);
                  lv_tabview_set_tab_act(obj, 1, LV_ANIM_ON);
              }
              auto_del(obj, TIME_STEP * 5 + 30);
              break;

        case 2:
            obj = lv_btn_create(main_page, NULL);
            lv_obj_set_size(obj, 100, 70);

            /*Move to disabled state very slowly*/
            lv_obj_add_style(obj, LV_BTN_PART_MAIN, &style_slow_trans);
            lv_obj_set_state(obj, LV_STATE_DISABLED);

            /*Add an infinite width change animation*/
            lv_anim_init(&a);
            lv_anim_set_var(&a, obj);
            lv_anim_set_time(&a, TIME_STEP * 2);
            lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_width);
            lv_anim_set_values(&a, 100, 200);
            lv_anim_set_playback_time(&a, TIME_STEP * 2);
            lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
            lv_anim_start(&a);

            /*Delete the object a few sec later*/
            auto_del(obj, TIME_STEP * 10);

            obj = lv_label_create(obj, NULL);
            lv_label_set_text_fmt(obj, "Formatted:\n%d %s", 12, "Volt");
            lv_page_focus(main_page, obj, LV_ANIM_ON);
            break;

        case 3:
            ta = lv_textarea_create(lv_scr_act(), NULL);
            lv_obj_align(ta, main_page, LV_ALIGN_OUT_RIGHT_TOP, 10, 10);
            lv_obj_set_height(ta, LV_VER_RES / 4);
            lv_textarea_set_placeholder_text(ta, "The placeholder");
            break;

        case 4:
            obj = lv_btn_create(main_page, NULL);
            lv_obj_set_size(obj, 100, 70);
            lv_obj_set_style_local_value_str(obj, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "Text from\nstyle");
            lv_obj_del_async(obj);  /*Delete on next call of `lv_task_handler` (so not now)*/

            obj = lv_btn_create(main_page, obj);
            lv_btn_set_fit(obj, LV_FIT_TIGHT);
            lv_obj_set_style_local_pattern_image(obj, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_LEFT);
            lv_obj_set_style_local_pattern_opa(obj, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_50);
            lv_obj_set_style_local_pattern_repeat(obj, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, true);
            lv_page_focus(main_page, obj, LV_ANIM_ON);
            break;

        case 5:
            lv_textarea_set_one_line(ta, true);
            break;
        case 6:
            lv_page_set_scrl_layout(main_page, LV_LAYOUT_COLUMN_LEFT);
            break;

        case 7:
            obj = lv_bar_create(main_page, NULL);
            lv_bar_set_range(obj, -1000, 2000);
            lv_bar_set_anim_time(obj, 1000);
            lv_bar_set_value(obj, 1800, LV_ANIM_ON);
            lv_bar_set_start_value(obj, -500, LV_ANIM_ON);

            auto_del(obj, TIME_STEP * 2 + 70);

            obj = lv_slider_create(main_page, NULL);
            lv_bar_set_anim_time(obj, 100);
            lv_bar_set_value(obj, 5000, LV_ANIM_ON);    /*Animate to out of range value*/
            auto_del(obj, TIME_STEP * 5 + 22);

            obj = lv_switch_create(main_page, NULL);
            lv_switch_on(obj, LV_ANIM_ON);

            obj = lv_switch_create(main_page, obj);
            lv_switch_off(obj, LV_ANIM_ON);
            auto_del(obj, 730);
            break;

        case 8:
              obj = lv_win_create(lv_scr_act(), NULL);
              lv_obj_set_size(obj, LV_HOR_RES / 2, LV_VER_RES / 2);
              lv_obj_align(obj, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
              lv_win_set_title(obj, "Window title");
              lv_win_add_btn(obj, LV_SYMBOL_CLOSE);
              lv_win_add_btn(obj, LV_SYMBOL_DOWN);
              auto_del(obj, TIME_STEP * 3 + 5);

              obj = lv_calendar_create(obj, NULL);
              break;
        case 9:
            lv_textarea_set_text(ta, "A very very long text which will should make the text area scrollable"
                    "Here area some dummy sentences to be sure the text area will be really scrollable.");
            break;
        case 10:
            obj = lv_keyboard_create(lv_scr_act(), NULL);
            lv_keyboard_set_mode(obj, LV_KEYBOARD_MODE_TEXT_UPPER);
            lv_anim_init(&a);
            lv_anim_set_var(&a, obj);
            lv_anim_set_values(&a, LV_VER_RES, LV_VER_RES - lv_obj_get_height(obj));
            lv_anim_set_time(&a, TIME_STEP + 3);
            lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
            lv_anim_start(&a);

            auto_del(obj, TIME_STEP * 2 + 18);
            break;

        case 11:
            obj = lv_dropdown_create(main_page, NULL);
            lv_dropdown_set_options(obj, "Zero\nOne\nTwo\nThree\nFour\nFive\nSix\nSeven\nEight");
            lv_dropdown_open(obj);
            lv_dropdown_set_selected(obj, 2);
            auto_del(obj, TIME_STEP * 3 + 11);
            break;

        case 12:
            obj = lv_roller_create(main_page, NULL);
            lv_roller_set_options(obj, "Zero\nOne\nTwo\nThree\nFour\nFive\nSix\nSeven\nEight", LV_ROLLER_MODE_INIFINITE);
            lv_roller_set_anim_time(obj, TIME_STEP);
            lv_roller_set_selected(obj, 2, LV_ANIM_ON);
            auto_del(obj, TIME_STEP * 20 + 22);
            break;

        case 13:
            obj = lv_arc_create(main_page, NULL);
            lv_anim_init(&a);
            lv_anim_set_var(&a, obj);
            lv_anim_set_values(&a, 180, 400);
            lv_anim_set_time(&a, TIME_STEP);
            lv_anim_set_delay(&a, TIME_STEP + 25);
            lv_anim_set_playback_time(&a, TIME_STEP * 2);
            lv_anim_set_repeat_count(&a, 3);
            lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_arc_set_end_angle);
            lv_anim_start(&a);

            obj = lv_linemeter_create(main_page, NULL);
            lv_page_focus(main_page, obj, LV_ANIM_ON);
            lv_linemeter_set_value(obj, 80);
            lv_anim_init(&a);
            lv_anim_set_var(&a, obj);
            lv_anim_set_values(&a, 0, 90);
            lv_anim_set_time(&a, TIME_STEP * 3);
            lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_linemeter_set_angle_offset);
            lv_anim_start(&a);

            auto_del(obj, TIME_STEP * 6 + 30);

            obj = lv_linemeter_create(main_page, obj);
            auto_del(obj, TIME_STEP * 1 + 10);
            break;

        case 14:
            obj = lv_msgbox_create(lv_scr_act(), NULL);
            lv_msgbox_set_text(obj, "Some text on the message box with average length");
            lv_msgbox_add_btns(obj, mbox_btns);
            lv_btnmatrix_set_btn_width(lv_msgbox_get_btnmatrix(obj), 1, 2);
            lv_msgbox_start_auto_close(obj, TIME_STEP * 3);
            lv_obj_align(obj, NULL, LV_ALIGN_IN_RIGHT_MID, -10, 0);
            break;

        case 15:
            lv_textarea_set_one_line(ta, false);
            break;

        case 16:
        {
              lv_obj_t * tv = lv_tileview_create(lv_scr_act(), NULL);
              static lv_point_t valid_pos[] = {{0,0}, {0, 1}, {1,1}};
              lv_tileview_set_valid_positions(tv, valid_pos, 3);
              lv_tileview_set_anim_time(tv, TIME_STEP * 2);
              auto_del(tv, TIME_STEP * 3 + 5);

              obj = lv_label_create(tv, NULL);
              lv_label_set_text(obj, "Tile: 0;0");

              obj = lv_label_create(tv, NULL);
              lv_label_set_text(obj, "Tile: 0;1");
              lv_obj_set_pos(tv, 0, LV_VER_RES);

              obj = lv_label_create(tv, NULL);
              lv_label_set_text(obj, "Tile: 1;1");
              lv_obj_set_pos(tv, LV_HOR_RES, LV_VER_RES);

              lv_tileview_set_tile_act(tv, 1, 1, LV_ANIM_ON);
        }
        break;

        case 17:
        {
            static lv_color_t needle_colors[2];
            needle_colors[0] = LV_COLOR_RED;
            needle_colors[1] = LV_COLOR_BLUE;
            obj = lv_gauge_create(main_page, NULL);
            lv_gauge_set_needle_count(obj, 2, needle_colors);
            lv_gauge_set_value(obj, 0, 30);
            lv_gauge_set_value(obj, 1, 60);
            lv_page_focus(main_page, obj, LV_ANIM_ON);
            auto_del(obj, TIME_STEP * 2 + 16);
        }
            break;
        case 18:
            obj =  lv_list_create(main_page, NULL);
            {
                lv_obj_t * b;
                b = lv_list_add_btn(obj, LV_SYMBOL_OK, "1. Some very long text to scroll");
                auto_del(b, 10);
                lv_list_add_btn(obj, LV_SYMBOL_OK, "2. Some very long text to scroll");
                lv_list_add_btn(obj, LV_SYMBOL_OK, "3. Some very long text to scroll");
                b = lv_list_add_btn(obj, LV_SYMBOL_OK, "4. Some very long text to scroll");
                auto_del(b, TIME_STEP);
                b = lv_list_add_btn(obj, LV_SYMBOL_OK, "5. Some very long text to scroll");
                auto_del(b, TIME_STEP + 90);
                b = lv_list_add_btn(obj, LV_SYMBOL_OK, "6. Some very long text to scroll");
                auto_del(b, TIME_STEP + 10);
                lv_list_focus(lv_list_get_prev_btn(obj, NULL),  LV_ANIM_ON);
            }
            auto_del(obj, TIME_STEP * 5 + 15);

            obj = lv_list_create(lv_scr_act(), obj);
            lv_list_set_layout(obj, LV_LAYOUT_ROW_MID);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_width(obj, LV_HOR_RES);
            lv_list_add_btn(obj, LV_SYMBOL_OK, "1. Horizontal list button");
            lv_list_focus(lv_list_get_prev_btn(obj, NULL),  LV_ANIM_ON);
            lv_list_add_btn(obj, LV_SYMBOL_OK, "2. Horizontal list button");
            lv_list_add_btn(obj, LV_SYMBOL_OK, "3. Horizontal list button");

            auto_del(obj, TIME_STEP * 3 + 85);
            break;

        case 19:
            lv_textarea_set_cursor_pos(ta, 10);
            lv_textarea_set_text(ta, "__INSERTED TEXT" LV_SYMBOL_EDIT"__");
            break;

        case 20:
            lv_page_set_scrl_layout(main_page, LV_LAYOUT_PRETTY_MID);
            break;

        case 21:
            lv_textarea_set_cursor_pos(ta, 0);
            lv_textarea_add_char(ta, '0');
            lv_textarea_add_char(ta, '1');
            lv_textarea_add_char(ta, '2');
            lv_textarea_add_char(ta, '3');
            lv_textarea_add_char(ta, '4');
            lv_textarea_add_char(ta, '5');
            lv_textarea_add_char(ta, '6');
            lv_textarea_add_char(ta, '7');
            lv_textarea_add_char(ta, '8');
            lv_textarea_add_char(ta, '9');
            lv_textarea_add_char(ta, 'A');
            lv_textarea_add_char(ta, 'B');
            lv_textarea_add_char(ta, 'C');
            lv_textarea_add_char(ta, 'D');
            lv_textarea_add_char(ta, 'E');
            lv_textarea_add_char(ta, 'F');
            lv_textarea_add_text(ta, LV_SYMBOL_OK);
            lv_textarea_add_text(ta, LV_SYMBOL_CLOSE);
            lv_textarea_add_text(ta, LV_SYMBOL_COPY);
            lv_textarea_add_text(ta, LV_SYMBOL_SAVE);
            lv_textarea_add_text(ta, LV_SYMBOL_PASTE);
            break;

        case 22:
            obj = lv_spinbox_create(main_page, NULL);
            lv_spinbox_set_digit_format(obj, 6, 3);
            lv_spinbox_set_value(obj, 5678);
            lv_spinbox_set_step(obj, 10);
            lv_spinbox_increment(obj);
            lv_spinbox_increment(obj);
            lv_spinbox_increment(obj);
            lv_spinbox_set_step(obj, 100);
            lv_spinbox_increment(obj);
            lv_spinbox_increment(obj);
            lv_spinbox_set_step(obj, 1);
            lv_spinbox_increment(obj);
            lv_spinbox_increment(obj);
            auto_del(obj, TIME_STEP * 2 + 15);

            lv_page_scroll_ver(main_page, LV_DPI);

            break;

        case 23:
            obj = lv_chart_create(main_page, NULL);
            {
                lv_chart_series_t * s1 = lv_chart_add_series(obj, LV_COLOR_RED);
                lv_chart_set_next(obj, s1, 36);
                lv_chart_set_next(obj, s1, -29);
                lv_chart_set_next(obj, s1, 51);
                lv_chart_set_next(obj, s1, 107);
                lv_chart_set_next(obj, s1, 70);
                lv_chart_set_next(obj, s1, 36);
                lv_chart_set_next(obj, s1, -2);
                lv_chart_set_next(obj, s1, 63);
                lv_chart_set_next(obj, s1, 48);
                lv_chart_set_next(obj, s1, 72);

                auto_del(obj, TIME_STEP * 3);
            }

            lv_page_scroll_ver(main_page, LV_DPI);
            break;

        case 24:
            obj = lv_checkbox_create(main_page, NULL);
            lv_checkbox_set_text(obj, "An option to select");
            auto_del(obj, TIME_STEP * 2 + 20);

            obj = lv_checkbox_create(main_page, obj);
            lv_checkbox_set_checked(obj, true);

            obj = lv_checkbox_create(main_page, obj);
            lv_checkbox_set_disabled(obj);
            auto_del(obj, TIME_STEP * 1 + 60);

            lv_page_scroll_ver(main_page, LV_DPI);

            break;

        case 25:
            lv_textarea_set_cursor_pos(ta, 20);
            {
                uint16_t i;
                for(i = 0; i < 64; i++) {
                    lv_textarea_del_char_forward(ta);
                }
            }
            break;

        case 26:
            lv_textarea_set_one_line(ta, true);
            break;
        case 29:
            lv_page_clean(main_page);
            lv_obj_del(ta);
            ta = NULL;
            break;
        case 31:
            lv_obj_clean(lv_scr_act());
            main_page = NULL;
            state = -2;
            break;
        default:
            break;
    }

    state ++;
}

static void auto_del(lv_obj_t * obj, uint32_t delay)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_time(&a, 0);
    lv_anim_set_delay(&a, delay);
    lv_anim_set_ready_cb(&a, lv_obj_del_anim_ready_cb);
    lv_anim_start(&a);

}



void chart_test(ALL_WEATHER_DATA_T data)
{
    int i = 0;

    lv_chart_series_t *chart_0 = test.home1_chart_line.home1_chart_tem_0;//home1_chart_tem_0;//test.home1_chart_line->home1_chart_tem_0;
    lv_chart_series_t *chart_1 = test.home1_chart_line.home1_chart_tem_1;//home1_chart_tem_1;//test.home1_chart_line->home1_chart_tem_1;
    
    if((chart_0 == NULL) || (chart_1 == NULL))          /*没有获取到指针*/
    {
        return ;                        
    }
    
    for(i = 0;i<5;i++)
    {
        chart_1->points[i] = data.day_weather_data[i].night_temp;
        chart_0->points[i] = data.day_weather_data[i].day_temp;
    }

    lv_chart_refresh(test.home1_chart_tem);
}
