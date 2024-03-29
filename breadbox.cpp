#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <iostream>
#include <fstream>
#include <float.h>
#include "SETTINGS.h"
#include "VidElement.h"
#include "Quad.h"
#include "Renderer.h"
#include "Util.h"
#include "MERSENNE_TWISTER.h"
using namespace std;

vector<VidElement *> vidElements;
bool motion = false;
int motion_N = 1000;
bool subPixel = false;
int subPixel_N = 100;
int cores = 1;

int xRes = 1920;
int yRes = 1080;

MERSENNE_TWISTER m;

void buildScene(){
	vidElements.clear();

	Anim_v2 posAnim;
	posAnim.keys = {0,10};
	posAnim.vals_v2 = {{960, 540}, {1920, 540}};
	posAnim.interps = {lerp};

	Anim_v2 sizeAnim; 
	sizeAnim.keys = {0, 10};
	sizeAnim.vals_v2 = {{100,100}, {200, 200}};
	sizeAnim.interps = {lerp};

	Anim_v2 anchorAnim;
	anchorAnim.keys = {0};
	anchorAnim.vals_v2 = {{.5, .5}};
	anchorAnim.interps = {};

	Anim_f rotAnim;
	rotAnim.keys = {0, 10};
	rotAnim.vals_f = {0, 90};
	rotAnim.interps = {lerp};

	Anim_f activeAnim;
	activeAnim.keys = {5, 8};
	activeAnim.vals_f = {1,0};
	activeAnim.interps = {lerp};

	Anim_f opacityAnim;
	opacityAnim.keys = {0};
	opacityAnim.vals_f = {0};
	opacityAnim.interps = {};

	Fill fill;
	fill.type = 0;
	fill.color = {1,0,0};
	//fill.img = newImage("resources/Loaf.ppm");

	Quad * q = new Quad(posAnim, sizeAnim, anchorAnim, rotAnim, opacityAnim, activeAnim, fill);
	vidElements.push_back(q);
}

void setScene(vector<VidElement *> scene){
	vidElements = scene;
}

void addFirework(VEC2 center, int numParticles, float radius, float time){
	for(int i = 0; i < numParticles; i++){
		float percent = (float)i/numParticles;
		float rad = percent*2.0f*PI;
		VEC2 end = {center[0] + radius*sin(rad), center[1] + radius*cos(rad)};

		Anim_v2 posAnim;
		posAnim.keys = {0,time};
		posAnim.vals_v2 = {center, end};
		posAnim.interps = {lerp};

		Anim_v2 sizeAnim; 
		sizeAnim.keys = {0, time};
		sizeAnim.vals_v2 = {{100,100}, {200, 200}};
		sizeAnim.interps = {lerp};

		Anim_v2 anchorAnim;
		anchorAnim.keys = {0};
		anchorAnim.vals_v2 = {{.5, .5}};
		anchorAnim.interps = {};

		Anim_f rotAnim;
		rotAnim.keys = {0, time};
		rotAnim.vals_f = {0, 360};
		rotAnim.interps = {lerp};

		Anim_f activeAnim;
		activeAnim.keys = {0, time, time+1};
		activeAnim.vals_f = {1,1, 0};
		activeAnim.interps = {lerp};

		Anim_f opacityAnim;
		opacityAnim.keys = {0};
		opacityAnim.vals_f = {0};
		opacityAnim.interps = {};

		Fill fill;
		fill.type = 0;
		fill.color = {1,0,0};

		Quad * q = new Quad(posAnim, sizeAnim, anchorAnim, rotAnim, opacityAnim, activeAnim, fill);
		vidElements.push_back(q);
	}
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VEC3 getColor(float x, float y, float t){
	VEC3 color = {0,0,0};
	for(int i = 0; i < vidElements.size(); i++){
		if(vidElements[i]->isVisible(x, y, t)){
			VEC3 raw = vidElements[i]->getColor(x, y, t);
			color = {raw[0], raw[1], raw[2]};
			break;
		}
	}
	return color;
}

VEC3 getMotionBlurColor(float x, float y, float t){
	VEC3 color = {0,0,0};
	if(!motion){
		color = getColor(x, y, t);
	}else{
		for(int i = 0; i < motion_N; i++){
			float dt = m.rand();
			color += getColor(x, y, t+dt);
		}
		color = color / motion_N;
	}
	return color;
}

float * rasterizeScene(int xRes, int yRes, float t){
	float * pixels = new float[xRes*yRes*3];
	for(int y = 0; y < yRes; y++){
		for(int x = 0; x < xRes; x++){
			VEC3 color = {0,0,0};

			if(!subPixel){
				color = getMotionBlurColor(x, y, t);
			}else{
				for(int i = 0; i < subPixel_N; i++){
					float dx = m.rand();
					float dy = m.rand();
					color += getMotionBlurColor(x+dx, y+dy, t);
				}
				color = color / subPixel_N;
			}			
			
			pixels[3*(y*xRes + x) + 0] = clamp(color[0]) * 255.0f;
			pixels[3*(y*xRes + x) + 1] = clamp(color[1]) * 255.0f;
			pixels[3*(y*xRes + x) + 2] = clamp(color[2]) * 255.0f;
		}
	}
	return pixels;
}

int main(int argc, char ** argv){

	m.seed(123456);
	int startFrame, endFrame;
	if(argc == 1){
		startFrame = 0;
		endFrame = 1;
	}else if(argc == 2){
		startFrame = atoi(argv[1]);
		endFrame = 300;
	}else{
		startFrame = atoi(argv[1]);
		endFrame = atoi(argv[2]);
	}

	addFirework({960, 540}, 10, 500, 30);
	for(int frame_num = startFrame; frame_num < endFrame; frame_num++){
		float * pixels = rasterizeScene(xRes, yRes, frame_num);
		char buffer[256];
    sprintf(buffer, "./oven/frame.%04i.ppm", frame_num);
		writePPM(buffer, xRes, yRes, pixels);
		cout << "Rendered frame " << frame_num << endl;
	}

	
}