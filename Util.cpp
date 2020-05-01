#include <iostream>
#include "SETTINGS.h"
#include "Util.h"
using namespace std;
void writePPM(const string& filename, int& xRes, int& yRes, const float* values)
{
  int totalCells = xRes * yRes;
  unsigned char* pixels = new unsigned char[3 * totalCells];
  for (int i = 0; i < 3 * totalCells; i++)
    pixels[i] = values[i];

  FILE *fp;
  fp = fopen(filename.c_str(), "wb");
  if (fp == NULL)
  {
    cout << " Could not open file \"" << filename.c_str() << "\" for writing." << endl;
    cout << " Make sure you're not trying to write from a weird location or with a " << endl;
    cout << " strange filename. Bailing ... " << endl;
    exit(0);
  }

  fprintf(fp, "P6\n%d %d\n255\n", xRes, yRes);
  fwrite(pixels, 1, totalCells * 3, fp);
  fclose(fp);
  delete[] pixels;
}

VEC2 hadamard(VEC2 a, VEC2 b){
  return {a[0]*b[0], a[1]*b[1]};
}


float clamp(float value)
{
  if (value < 0.0)      return 0.0;
  else if (value > 1.0) return 1.0;
  return value;
}

float exerpFloat(float start, float end, float t){
  clamp(t);
  return start + (end-start)*t;
}

float lerpFloat(float start, float end, float t){
  clamp(t);
  return start + (end-start)*t;
}

VEC2 lerpVec(VEC2 start, VEC2 end, float t){
  clamp(t);
  return start + (end-start)*t;
}

VEC2 smerpVec(VEC2 start, VEC2 end, float t){
  clamp(t);
  t = 3*pow(t,2) - 2*pow(t,2);
  return start + (end-start)*t;
}

float smerpFloat(float start, float end, float t){
  t = 3*pow(t,2) - 2*pow(t,2);
  return start + (end-start)*t;
}

// REQUIRES A 0 keyframe!!!!
float keyframeFloat(vector<float> keyframes, vector<float> values, vector<int> interpolation, float t){
  int i = 0;
  while((i < keyframes.size()) && (t >= keyframes[i])) i++;
  int nextFrame = i;
  if(nextFrame == keyframes.size()) nextFrame--;
  int lastFrame = i-1;
  if(lastFrame < 0) lastFrame = 0;
  float percent;
  if(nextFrame != lastFrame){
    percent = (t-keyframes[lastFrame])/(keyframes[nextFrame]-keyframes[lastFrame]);
  }else{
    percent = 1;
  }

  return lerpFloat(values[lastFrame], values[nextFrame], percent);
}

VEC2 keyframeVec(vector<float> keyframes, vector<VEC2> values, vector<int> interpolation, float t){
  int i = 0;
  while((i < keyframes.size()) && (t >= keyframes[i])) i++;
  int nextFrame = i;
  if(nextFrame == keyframes.size()) nextFrame--;
  int lastFrame = i-1;
  if(lastFrame < 0) lastFrame = 0;
  float percent;
  if(nextFrame != lastFrame){
    percent = (t-keyframes[lastFrame])/(keyframes[nextFrame]-keyframes[lastFrame]);
  }else{
    percent = 1;
  }

  if(interpolation[lastFrame] == 0){
    return lerpVec(values[lastFrame], values[nextFrame], percent);
  }else if(interpolation[lastFrame] == 1){
    return smerpVec(values[lastFrame], values[nextFrame], percent);
  }
  
}

float min(float a, float b){
  if(a < b){
    return a;
  }else{
    return b;
  }
}

float max(float a, float b){
  if(a > b){
    return a;
  }else{
    return b;
  }
}

float area(VEC2 v0, VEC2 v1){
  //VEC2 cross = v0.cross(v1);
  //return cross.norm()/2.0;
  return 5;
}

VEC3 extend(VEC2 in){
  return VEC3(in[0], in[1], 1);
}

VEC2 truncate(VEC3 in){
  return VEC2(in[0], in[1]);
}

VEC2 translate(VEC2 in, VEC2 trans){
  return in + trans;
}

// rot is in DEGREES!!
VEC2 rotate(VEC2 in, float rot, VEC2 anchor){
  float rotRad = rot*PI/180.0f;
  VEC2 result = translate(in, -anchor);
  result = {result[0]*cos(rotRad) - result[1]*sin(rotRad), result[0]*sin(rotRad) + result[1]*cos(rotRad)};
  return translate(result, anchor);
}

VEC2 scale(VEC2 in, VEC2 scale){
  return {in[0]*scale[0], in[1]*scale[1]};
}