#include <iostream>
#include "SETTINGS.h"
#include "Quad.h"
#include "Util.h"
#include "Renderer.h"
using namespace std;



Quad::Quad(Anim_v2 posAnim_in, Anim_v2 sizeAnim_in, Anim_v2 anchorAnim_in, Anim_f rotAnim_in, Anim_f opacityAnim_in, Anim_f activeAnim_in, Fill fill_in){
	posAnim = posAnim_in;
	sizeAnim = sizeAnim_in;
	anchorAnim = anchorAnim_in;
	rotAnim = rotAnim_in;
	opacityAnim = opacityAnim_in;
	activeAnim = activeAnim_in;
	fill = fill_in;
}

bool Quad::isVisible(float x, float y, float t){
	// Set status data
	pos = v2_animate(posAnim, t);
	size = v2_animate(sizeAnim, t);
	anchor = v2_animate(anchorAnim, t);
	rot = f_animate(rotAnim, t);
	opacity = f_animate(opacityAnim, t);
	active = f_animate(activeAnim, t);

	if(active == 0)
		return false;

	// Perform intersection test
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

/*
// Returns the integral over [x, x+1] [y, y+1]
VEC3 Quad::getAreaIntegralColor(float x, float y, float t){
	// Set status data (You could totally check here to see if t changed at all and save status calculating again)
	pos = v2_animate(posAnim, t);
	size = v2_animate(sizeAnim, t);
	anchor = v2_animate(anchorAnim, t);
	rot = f_animate(rotAnim, t);
	opacity = f_animate(opacityAnim, t);

	VEC2 look = {x, y};
	VEC2 Q = pos + size/2.0f;
	VEC2 Qmin = pos - size/2.0f;
	VEC2 transAnchor = min + VEC2(size[0]*anchor[0], size[1]*anchor[1]);

	look = rotate(look, -rot, transAnchor);

}*/


VEC3 Quad::getColor(float x, float y, float t){
	if(active == 0)
		return {0,0,0};

	// Right now everything is just a solid color
	if(fill.type == 0){
		return fill.color;
	}else if(fill.type == 1){
		VEC2 look = {x, y};
		VEC2 min = pos - size/2.0f;
		VEC2 transAnchor = min + VEC2(size[0]*anchor[0], size[1]*anchor[1]);
		look = rotate(look, -rot, transAnchor);
		float u = (look[1]-min[1])/size[1];
		float v = (look[0]-min[0])/size[0];
		if((u > 1.0f) || (u < 0.0f) || (v > 1.0f) || (v < 0.0f)){
			return {0,0,0};
		}else{
			int x = floor(v*fill.img.xRes);
			int y = floor(u*fill.img.yRes);
			int index = (y*fill.img.xRes + x)*3;
			return {fill.img.pixels[index], 
							fill.img.pixels[index+1],
							fill.img.pixels[index+2]};

		}
	}
}