#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Arduino.h>
#include <SPI.h>

/****************THIS IS SOME BASIC CONFIG FOR TFT LCD 1.3*************/
//define the screen size
int screenWidth  = 240;
int screenHeight = 240;
/* TFT instance */
TFT_eSPI tft = TFT_eSPI(); 
/*define some buf for LVGL*/
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];
// flush the screen  Display flushing 
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
// draw the style 
void test_lvgl();

/*******************THIS IS SOME GLOBAL CONF**************************/
const static uint8_t BG_HIGHSIZE  = 240;
const static uint8_t BG_WIDTHSIZE = 240;

const static uint8_t DIALOG_U_H_SIZE = 80;
const static uint8_t DIALOG_U_W_SIZE = 100;

const static uint8_t DIALOG_D_H_SIZE = 40;
const static uint8_t DIALOG_D_W_SIZE = 100;

/********************************************************************/


void setup() {

  lv_init();
  tft.begin(); /* TFT init */
  lv_disp_buf_init(&disp_buf, buf, NULL,LV_HOR_RES_MAX*10);
  //显示刷新接口
  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.buffer = &disp_buf;
  lv_disp_drv_register(&disp_drv);
  //测试函数-初始化界面
  test_lvgl();

}

void loop() {
  lv_task_handler(); 
  /* let the GUI do its work */
}


/***************绘制函数**********/
//创建LVGL控件
//绘制界面
/********************************/
void test_lvgl()
{
  lv_obj_t *scr = lv_scr_act();
  lv_obj_t *bg    = lv_obj_create(scr,NULL);
  lv_obj_t *label = lv_label_create(scr,NULL);
  lv_obj_t *dialog_up   = lv_obj_create(bg,NULL);
  lv_obj_t *dialog_down = lv_obj_create(bg,NULL);
  lv_obj_t *dialog_label = lv_label_create(dialog_up,NULL);
  lv_obj_t *label_CA = lv_label_create(dialog_down,NULL);
  lv_obj_t *label_CE = lv_label_create(dialog_down,NULL);

  //创建背景控件
  static lv_style_t bg_style;
  lv_style_copy(&bg_style,&lv_style_plain_color);
  lv_obj_set_size(bg,BG_WIDTHSIZE,BG_HIGHSIZE);
  lv_obj_set_pos(bg,0,0);
  bg_style.body.main_color = LV_COLOR_MAKE(205,205,205);
  bg_style.body.grad_color = LV_COLOR_MAKE(205,205,205);
  bg_style.body.opa = 100 ;
  lv_obj_set_style(bg,&bg_style);

  //创建黑色圆角对话框
  static lv_style_t dialogup_style;
  lv_obj_set_size(dialog_up,DIALOG_U_W_SIZE,DIALOG_U_H_SIZE);
  lv_obj_set_pos(dialog_up,70,70);
  lv_style_copy(&dialogup_style,&lv_style_plain_color);
  dialogup_style.body.main_color = LV_COLOR_MAKE(40,42,54);
  dialogup_style.body.grad_color = LV_COLOR_MAKE(40,42,54);
  dialogup_style.body.radius     = 10;
  lv_obj_set_style(dialog_up,&dialogup_style);
  //设置对话框中的文本
  lv_label_set_text(dialog_label,"Do You \nLike it?");
  lv_label_set_long_mode(dialog_label,LV_LABEL_LONG_CROP);
  lv_obj_align(dialog_label,dialog_up,LV_ALIGN_CENTER,0,0);
  //设置白色的矩形框
  static lv_style_t dialogdown_style;
  lv_obj_set_size(dialog_down,DIALOG_D_W_SIZE,DIALOG_D_H_SIZE);
  lv_obj_set_pos(dialog_down,70,140);
  lv_style_copy(&dialogdown_style,&lv_style_plain_color);
  dialogdown_style.body.main_color = LV_COLOR_WHITE;
  dialogdown_style.body.grad_color = LV_COLOR_WHITE;
  lv_obj_set_style(dialog_down,&dialogdown_style);
  //左边的NO用LABEL控件创建 边框为1 中间背景为白色
  static lv_style_t label_CA_style;
  lv_label_set_recolor(label_CA,true);
  lv_obj_set_size(label_CA,40,30);
  lv_label_set_text(label_CA,"#000000 NO #");
  lv_label_set_body_draw(label_CA,true);
  lv_obj_align(label_CA,dialog_down,LV_ALIGN_CENTER,-25,0);
  lv_style_copy(&label_CA_style,&lv_style_plain_color);
  label_CA_style.body.main_color = LV_COLOR_WHITE;
  label_CA_style.body.grad_color = LV_COLOR_WHITE;  
  label_CA_style.body.border.part  =  LV_BORDER_FULL;
  label_CA_style.body.border.width = 1;
  label_CA_style.body.border.color = LV_COLOR_BLACK;
  lv_label_set_style(label_CA,LV_LABEL_STYLE_MAIN,&label_CA_style);

  //创建右边蓝色的YES
  static lv_style_t label_CE_style;
  lv_label_set_recolor(label_CE,true);
  lv_obj_set_size(label_CE,40,30);
  lv_label_set_text(label_CE,"YES");
  lv_label_set_body_draw(label_CE,true);
  lv_obj_align(label_CE,dialog_down,LV_ALIGN_CENTER,25,0);
  lv_style_copy(&label_CE_style,&lv_style_plain_color);
  label_CE_style.body.main_color = LV_COLOR_BLUE;
  label_CE_style.body.grad_color = LV_COLOR_BLUE;  
  lv_label_set_style(label_CE,LV_LABEL_STYLE_MAIN,&label_CE_style);

  //设置头部的文本
  lv_label_set_text(label,"TITLE");
  lv_obj_align(label,NULL,LV_ALIGN_IN_TOP_MID,0,20);
}

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