#include <iostream>
#include "SETTINGS.h"
#include "Quad.h"
#include "Util.h"
#include "Renderer.h"
using namespace std;



Quad::Quad(Anim_v2 posAnim_in, Anim_v2 sizeAnim_in, Anim_v2 anchorAnim_in, Anim_f rotAnim_in, Anim_f opacityAnim_in, VEC3 color_in){
	posAnim = posAnim_in;
	sizeAnim = sizeAnim_in;
	anchorAnim = anchorAnim_in;
	rotAnim = rotAnim_in;
	opacityAnim = opacityAnim_in;
	color = color_in;
}

bool Quad::isVisible(float x, float y, float t){
	// Set status data
	pos = v2_animate(posAnim, t);
	size = v2_animate(sizeAnim, t);
	anchor = v2_animate(anchorAnim, t);
	rot = f_animate(rotAnim, t);
	opacity = f_animate(opacityAnim, t);

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

VEC3 Quad::getColor(float x, float y, float t){
	// Right now everything is just a solid color
	return color;
}