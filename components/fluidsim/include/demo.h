#ifndef _DEMO1_H_
#define _DEMO1_H_

#include "Adafruit_PixelDust.h"
#include "compositor.h"

class demo {
public:
  demo();
  void selectSim(int option, int flakes);
  void dispSnow();
  void setBuffer(Color* framebuffer);
};

#endif
