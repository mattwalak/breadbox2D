#include <iostream>
using namespace std;
#ifndef VIDELEMENT_H
#define VIDELEMENT_H

class VidElement{
	int layer;
public:
	void setLayer(int layer_in){layer = layer_in;}
	int getLayer(){return layer;}
	virtual bool isVisible(float x, float y, float t) = 0;
};

#endif