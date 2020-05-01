#include <iostream>
#include "SETTINGS.h"
using namespace std;
#ifndef RENDERER_H
#define RENDERER_H

class Renderer{
public:
	virtual VEC4 getColor(float u, float v) = 0;
};

#endif