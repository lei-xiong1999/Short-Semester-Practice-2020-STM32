#ifndef _MYOBJECT_H
#define _MYOBJECT_H

#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"
#include "ssd1306_i2c.h"
#include "app.h"
#include "ssd1306.h"
#include "gui.h"


typedef struct Vertex{
  int8_t location[3];
}MyVertex;
typedef struct Edge{
  int8_t connection[2];
}MyEdge;
typedef struct Object{
  int numv;
  int nume;
  MyVertex* v;
  MyEdge* e;
  float quat[4];
  float offset[3];
}MyObject;


#endif

