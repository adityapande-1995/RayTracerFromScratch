#ifndef VEC3_H
#define VEC3_H

class vec3{
 public:
  float e[3] = {0,0,0};

  vec3();
  vec3(float e0, float e1, float e2);

  float x();
  float y();
  float z();
  float r();
  float g();
  float b();

  vec3 operator-();
  float operator[](int i);

  vec3 operator+(vec3 other);
  vec3 operator-(vec3 other);
  vec3 operator*(vec3 other);
  vec3 operator*(float other);
  vec3 operator/(vec3 other);
  vec3 operator/(float other);

  float length();
  float squared_length();
  void print();

};

float dot(vec3 v1 , vec3 v2);
vec3 cross(vec3 v1 , vec3 v2);

vec3 unit_vector(vec3 v);

#endif
