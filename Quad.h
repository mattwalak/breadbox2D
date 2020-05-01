#include <iostream>
#include "VidElement.h"
using namespace std;

class Quad : public VidElement{
public:
	Quad();
	int result();
	bool isVisible(float x, float y, float t);
};