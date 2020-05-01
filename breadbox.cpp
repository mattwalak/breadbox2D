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


int main(int argc, char ** argv){
	int xRes = 1920;
	int yRes = 1080;
	float * pixels = new float[xRes*yRes*3];
	for(int i = 0; i < xRes*yRes*3; i++){
		pixels[i] = 255.0f * ((float)i/(xRes*yRes*3));
	}

	writePPM("oven/output.ppm", xRes, yRes, pixels);
}