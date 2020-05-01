#include <iostream>
#include "SETTINGS.h"
#include "Renderer.h"

class RSolidColor : public Renderer{
public:
	RSolidColor();
	VEC4 getColor(float u, float v);
};