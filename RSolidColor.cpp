#include <iostream>
#include "SETTINGS.h"
#include "Renderer.h"
#include "RSolidColor.h"

RSolidColor::RSolidColor(){

}

VEC4 RSolidColor::getColor(float u, float v){
	return {1,1,1,1};
}

