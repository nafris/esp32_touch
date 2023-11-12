#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include <vector>
#include "ESP32_SPI_9341.h"
#define SD_SCK 18
#define SD_MISO 19
#define SD_MOSI 23
#define SD_CS 5

//#define LIGHT_ADC 34

int led_pin[3] = {17, 4, 16};


LGFX lcd;

static int colors[] = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_YELLOW};

int i = 0;
long runtime_0 = 0;
long runtime_1 = 0;

void touch_calibration()
{
    lcd.fillScreen(TFT_YELLOW);

    lcd.setTextColor(TFT_BLACK);
    lcd.setTextSize(2);
    lcd.setCursor(70, 110);
    lcd.println("SCREEN");
    lcd.setCursor(70, 150);
    lcd.println("CALIBRATION");

    // タッチを使用する場合、キャリブレーションを行います。画面の四隅に表示される矢印の先端を順にタッチしてください。
    std::uint16_t fg = TFT_WHITE;
    std::uint16_t bg = TFT_BLACK;
    if (lcd.isEPD())
        std::swap(fg, bg);
    lcd.calibrateTouch(nullptr, fg, bg, std::max(lcd.width(), lcd.height()) >> 3);
}


extern "C"{
    void app_main();
}
void app_main() {
    //pinMode(led_pin[0], OUTPUT);
    //pinMode(led_pin[1], OUTPUT);
    //pinMode(led_pin[2], OUTPUT);

    // pinMode(LCD_BL, OUTPUT);
    // digitalWrite(LCD_BL, HIGH);

    lcd.init();
  
    // lcd.setRotation(0);
    vTaskDelay(100);
    touch_calibration();
    while(1){
        if ((esp_timer_get_time() / 1000 - runtime_0) > 1000)
        {
            //led_set(i);
            lcd.fillScreen(colors[i++]);

            if (i > 3)
            {
                i = 0;
            }

            runtime_0 = esp_timer_get_time() / 1000;
        }

        if ((esp_timer_get_time() / 1000 - runtime_1) > 300)
        {
                    int poss[2] = {0, 0};
            lcd.getTouch(&poss[0], &poss[1]);
            printf("%d %d \n",poss[0], poss[1]);
            lcd.setCursor(10, 10);
            lcd.printf("%d  %d", poss[0], poss[1]);
            //int adc_value = analogRead(LIGHT_ADC);
            //printf("ADC:%d\n", adc_value);
        /* if (adc_value > 50)
                lcd.setBrightness(50);
            else
            lcd.setBrightness(255);*/
            runtime_1 = esp_timer_get_time() / 1000;
        }
        vTaskDelay(10);
    }
}
