#pragma once

#include <Arduino.h>

#define BUILTIN_LED 2

void Raise(String message, uint8_t led_pin = BUILTIN_LED);
