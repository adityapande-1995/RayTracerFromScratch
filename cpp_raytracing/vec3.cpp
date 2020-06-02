#include <iostream>
#include <math.h>
#include "vec3.h"

// Class vec3 methods

// Constructors
vec3::vec3() {};
vec3::vec3(float e0, float e1, float e2){
    this->e[0] = e0 ; this->e[1] = e1 ; this->e[2] = e2;
};

// Basic access
float vec3::x(){ return this->e[0]; };
float vec3::y(){ return this->e[1]; };
float vec3::z(){ return this->e[2]; };

float vec3::r(){ return this->e[0]; };
float vec3::g(){ return this->e[1]; };
float vec3::b(){ return this->e[2]; };

// Overloading unary operators
vec3 vec3::operator-(){ return vec3( -this->e[0] , -this->e[1], -this->e[2] ); };
float vec3::operator[](int i){ return this->e[i]; };

// Overloading binary operators
vec3 vec3::operator+(vec3 other){
    return vec3( this->e[0] + other[0], this->e[1] + other[1], this->e[2] + other[2] );
};
vec3 vec3::operator-(vec3 other){
    return vec3( this->e[0] - other[0], this->e[1] - other[1], this->e[2] - other[2] );
};
vec3 vec3::operator*(vec3 other){
    return vec3( this->e[0]*other[0], this->e[1]*other[1], this->e[2]*other[2] );
};
vec3 vec3::operator*(float other){
    return vec3( this->e[0]*other, this->e[1]*other, this->e[2]*other );
};
vec3 vec3::operator/(vec3 other){
    return vec3( this->e[0]/other[0], this->e[1]/other[1], this->e[2]/other[2] );
};
vec3 vec3::operator/(float other){
    return vec3( this->e[0]/other, this->e[1]/other, this->e[2]/other );
};

//Other methods
float vec3::length(){
    return sqrt(this->e[0]*this->e[0] +  this->e[1]*this->e[1] + this->e[2]*this->e[2]);
};
float vec3::squared_length(){
    return (this->e[0]*this->e[0] +  this->e[1]*this->e[1] + this->e[2]*this->e[2]);
};

// print
void vec3::print(){
    std::cout << this->e[0] << " " << this->e[1] << " " << this->e[2] << std::endl;
};

//*****************
// Dot and cross products
float dot(vec3 v1, vec3 v2){ return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2] ; };
vec3 cross(vec3 v1, vec3 v2){
  float tx = v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1];
  float ty = -(v1.e[0]*v2.e[2] - v1.e[2]*v2.e[0]);
  float tz = v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0];
  return vec3(tx, ty, tz);
};

// Unit vector
vec3 unit_vector(vec3 v){ return v/v.length() ;  };
