#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#define PIN 26          // LEDの信号線をつなぐマイコンのピン番号
#define STRIP_COUNT 52  //つなぐLEDの数

#define BLINK_COUNT 40

#define LED_R 50
#define LED_G 50
#define LED_B 50

#define SNAKE 0
#define FLASH 1
#define MATSURI 2

// ---MODE SELECT---
#define MODE MATSURI

// ---SNAKE---
#define SNAKE_LENGTH 20

// ---MATSURI---
#define MATSURI_PATTERN 2
#define MATSURI_WAIT_TIME 100000
int matsuri_pos = 0;
int matsuri_time = 0;

float e = 2.71828182846;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIP_COUNT, PIN, NEO_GRB + NEO_KHZ800);  //あれこれ初期設定

float pow(float a, int x) {
    if (x == 0) {
        return 1.0;
    }

    float b = a;
    for (int i = 1; i < x; i++) {
        b *= a;
    }
    return b;
}

void setup() {
    strip.begin();
    Serial.begin(9600);
}

void drawSnake(int pos) {
    for (int i = 0; i < SNAKE_LENGTH + 1; i++) {
        if (pos - i > STRIP_COUNT) {
            continue;
        }
        if (pos - i < 0) {
            return;
        }
        float per = (((float)SNAKE_LENGTH - i) * ((float)SNAKE_LENGTH - i) / (((float)SNAKE_LENGTH) * ((float)SNAKE_LENGTH)));
        strip.setPixelColor(pos - i, strip.Color((int)(LED_R * per), (int)(LED_G * per), (int)(LED_B * per)));
    }
}

bool flashLight() {
    for (int j = 0; j < BLINK_COUNT; j++) {
        for (int i = 0; i <= STRIP_COUNT; i++) {
            strip.setPixelColor(
                i,
                strip.Color(
                    (float)LED_R * (((float)BLINK_COUNT - j - 1) / (float)BLINK_COUNT),
                    (float)LED_G * (((float)BLINK_COUNT - j - 1) / (float)BLINK_COUNT),
                    (float)LED_B * (((float)BLINK_COUNT - j - 1) / (float)BLINK_COUNT)));
        }
        delay(5);
        strip.show();  // LEDの設定を反映
        if (Serial.available()) {
            char key = Serial.read();
            if (key == 'f') {
                return true;
            }
        }
    }
    return false;
}

void drawMatsuri() {
    for (int i = 0; i < STRIP_COUNT; i++) {
        bool blink = (i + matsuri_pos) % MATSURI_PATTERN;
        strip.setPixelColor(
            i,
            strip.Color(
                blink ? LED_B : 0,
                blink ? LED_G : 0,
                blink ? LED_B : 0));
    }
    matsuri_pos += 1;
    if (matsuri_pos >= MATSURI_PATTERN) {
        matsuri_pos = 0;
    }
    strip.show();  // LEDの設定を反映
    delay(10);
}

void loop() {
    switch (MODE) {
        case SNAKE:
            for (int i = 0; i <= STRIP_COUNT + SNAKE_LENGTH; i++) {
                // strip.setPixelColor(i, strip.Color(10, 10, 10)); //先ほど初期設定した名前.setPixelColor(設定するLEDの番号, strip.Color(緑の強さ, 赤の強さ, 青の強さ));　ちなみにこの場合は青を最大の明るさで点灯
                drawSnake(i);
                strip.show();  // LEDの設定を反映
                delay(15);
                // strip.setPixelColor(i, strip.Color(0, 0, 0)); //青を消灯
            }
            break;

        case FLASH:
            if (Serial.available()) {
                char key = Serial.read();
                if (key == 'f') {
                    bool res = true;
                    while (res) {
                        res = flashLight();
                    }
                }
            }
            break;

        case MATSURI:
            matsuri_time += 1;
            if (matsuri_time >= MATSURI_WAIT_TIME) {
                matsuri_time = 0;
                drawMatsuri();
            }

        default:
            break;
    }
}