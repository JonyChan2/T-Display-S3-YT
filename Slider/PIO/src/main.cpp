#include <lvgl.h>
#include <TFT_eSPI.h>
#include <ui.h>
#include <Arduino.h>
#include <Ticker.h>
#include <Bounce2.h>

#define PIN_LCD_BL 38
#define PIN_LCD_RD 9
#define PIN_LCD_RES 5

/*Change to your screen resolution*/
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 170;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf;

TFT_eSPI tft = TFT_eSPI(); /* TFT instance */

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char *buf)
{
  Serial.printf(buf);
  Serial.flush();
}
#endif

void setBrightness(uint8_t value)
{
    static uint8_t level = 0;
    static uint8_t steps = 16;
    if (value == 0) {
        digitalWrite(PIN_LCD_BL, 0);
        delay(3);
        level = 0;
        return;
    }
    if (level == 0) {
        digitalWrite(PIN_LCD_BL, 1);
        level = steps;
        delayMicroseconds(30);
    }
    int from = steps - level;
    int to = steps - value;
    int num = (steps + to - from) % steps;
    for (int i = 0; i < num; i++) {
        digitalWrite(PIN_LCD_BL, 0);
        digitalWrite(PIN_LCD_BL, 1);
    }
    level = value;
}

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

// Button setup
Bounce button1 = Bounce(0, 5);  // Start/Pause button
Bounce button2 = Bounce(14, 5); // Reset button

lv_anim_t focusAnimation; // Global animation object
lv_anim_t breakAnimation; // Global animation object for break

void startFocusAnimation(long remainingTime)
{
  int seconds = (remainingTime % 60000) / 1000;
  int startDegrees = seconds * 6;

  lv_anim_init(&focusAnimation);
  lv_anim_set_var(&focusAnimation, ui_img_focus_sec);
  lv_anim_set_exec_cb(&focusAnimation, (lv_anim_exec_xcb_t)lv_img_set_angle);
  lv_anim_set_values(&focusAnimation, startDegrees * 10, 0);
  lv_anim_set_time(&focusAnimation, remainingTime);
  lv_anim_set_playback_time(&focusAnimation, 0);
  lv_anim_set_repeat_count(&focusAnimation, 1);

  lv_anim_start(&focusAnimation);
}

void switch_page(void)
{
    static uint8_t n;
    n++;
    lv_obj_set_tile_id(ui_Screen1, 0, n % 2, LV_ANIM_ON);
}

void handleButtons();

void setup()
{
  Serial.begin(115200); /* prepare for possible serial debug */

  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");

  lv_init();
  buf = (lv_color_t *)heap_caps_malloc(screenHeight*screenHeight * sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);

#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif
  tft.begin();        /* TFT init */
  tft.setRotation(3); /* Landscape orientation, flipped */
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  lv_indev_drv_register(&indev_drv);

  pinMode(PIN_LCD_BL,OUTPUT);
  setBrightness(8);

  ui_init();
  Serial.println("Setup done");

  button1.attach(0, INPUT_PULLUP); // Setup correct button pin
  button2.attach(14, INPUT_PULLUP);

  // Start the first focus session UI update
  
}

void loop()
{
  lv_timer_handler(); // Let LVGL handle its work
  delay(5);           // Delay to control the loop frequency

  handleButtons();
}

void handleButtons()
{
  button1.update();
  button2.update();

  if (button1.fell())
  { // Detect falling edge (button press)
    switch_page();
  }

  if (button2.fell())
  {
    return ;
  }
}
