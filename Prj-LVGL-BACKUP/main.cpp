#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Arduino.h>
#include <SPI.h>


TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
static lv_disp_buf_t disp_buf;

//static lv_color_t buf[LV_HOR_RES_MAX * 10];
static lv_color_t buf[LV_HOR_RES_MAX * 10];

int screenWidth  = 240;
int screenHeight = 240;


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
  
  lv_obj_t *label = lv_label_create( lv_scr_act(),NULL);
  lv_label_set_text( label, "Hello Arduino! (V8.0.X)" );
  lv_obj_set_pos(label,50,50);

}

void loop() {
  lv_task_handler(); 
  /* let the GUI do its work */
}

