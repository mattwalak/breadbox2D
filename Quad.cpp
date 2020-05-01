#include <iostream>
#include "SETTINGS.h"
#include "Quad.h"
#include "Util.h"
#include "Renderer.h"
using namespace std;


VEC2 pos;
float rot;
VEC2 size;
VEC2 anchor;
float opacity;
Renderer * render;



Quad::Quad(Anim_v2 posAnim, Anim_f rotAnim, Anim_v2 sizeAnim, Anim_v2 anchorAnim, Anim_f opacityAnim){
	pos = {960, 540};
	rot = 10;
	size = {100,100};
	anchor = {.5, .5};
}

int Quad::result(){
	return 100;
}

bool Quad::isVisible(float x, float y, float t){
	VEC2 look = {x, y};
	VEC2 max = pos + size/2.0f;
	VEC2 min = pos - size/2.0f;
	VEC2 transAnchor = min + VEC2(size[0]*anchor[0], size[1]*anchor[1]);
	look = rotate(look, -rot, transAnchor);
	if((look[0] > min[0]) && (look[0] < max[0]) && (look[1] > min[1]) && (look[1] < max[1]))
		return true;
	else
		return false;
}

VEC4 Quad::getColor(float x, float y, float t){
	return {1,1,1,1};
}