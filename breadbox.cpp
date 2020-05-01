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
#include "RSolidColor.h"
#include "Util.h"
using namespace std;

vector<VidElement *> vidElements;
int motion_N = 10;
int subPixel_N = 10;
int cores = 1;

int xRes = 1920;
int yRes = 1080;

void buildScene(){
	vidElements.clear();
	vidElements.push_back(new Quad());
	//Sort vidElements by layer here
}


VEC3 getColor(float x, float y, float t){
	VEC3 color = {0,0,0};
	for(int i = 0; i < vidElements.size(); i++){
		if(vidElements[i]->isVisible(x, y, t)){
			VEC4 raw = vidElements[i]->getColor(x, y, t);
			color = {raw[0], raw[1], raw[2]};
			break;
		}
	}
	return color;
}

float * rasterizeScene(int xRes, int yRes, float t){
	float * pixels = new float[xRes*yRes*3];
	for(int y = 0; y < yRes; y++){
		for(int x = 0; x < xRes; x++){
			VEC3 color = getColor(x, y, t);
			pixels[3*(y*xRes + x) + 0] = clamp(color[0]) * 255.0f;
			pixels[3*(y*xRes + x) + 1] = clamp(color[1]) * 255.0f;
			pixels[3*(y*xRes + x) + 2] = clamp(color[2]) * 255.0f;
		}
	}
	return pixels;
}

int main(int argc, char ** argv){
	buildScene();
	float * pixels = rasterizeScene(xRes, yRes, 0);
	writePPM("oven/output.ppm", xRes, yRes, pixels);
}