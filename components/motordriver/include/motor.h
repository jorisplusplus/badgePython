#pragma once
#include "esp_err.h"

typedef struct motor_indent {
    float angle;    //Angle in degrees where indent is to be place
    float cw_range; //Range in degrees where snap starts in clockwise range
    float ccw_range;//Range in degrees where snap starts in counter clockwise range
    float force;    //How much snap force, [0, 1] range 1 is max power
    struct motor_indent *next;
    struct motor_indent *prev;
} motor_indent_t;

esp_err_t motor_init(void);
void calibrate();
float motor_latest_angle();
