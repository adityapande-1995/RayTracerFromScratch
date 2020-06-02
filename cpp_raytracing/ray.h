#ifndef RAY_H
#define RAY_H
#include <string>
#include <vector>

vec3 random_in_unit_sphere();
vec3 reflect(vec3 v, vec3 n);

//**********/ Ray class     
class ray{
 public:
  vec3 A = vec3(0,0,0) ; vec3 B = vec3(0,0,0);
  ray(){}
  ray(vec3 a, vec3 b);
  vec3 origin();
  vec3 direction();
  vec3 point_at_parameter(float t);
};

////*********** Materials start  /
struct material_return{
  bool status = false;
  vec3 att = vec3();
  ray scat = ray();
};

class gen_material{
 public:
  vec3 albedo = vec3(0,0,0);
  std::string name = "none";
  // Constructor
  gen_material(){};
  gen_material(vec3 a, std::string name);
};

// ************ Hit record class
class hit_record{
 public:
  float t = 0;
  vec3 p = vec3(0,0,0);
  vec3 normal = vec3(0,0,0);
  gen_material material = gen_material();
  hit_record(){}
  hit_record(float t, vec3 p, vec3 normal, gen_material material );
};

// Scatter
struct material_return scatter(gen_material& material, ray r_in, hit_record rec, vec3 attenuation, ray scattered);

// ************* World objects
struct sp_return{
  bool status;
  hit_record rec;
};

class sphere{
 public:
  vec3 center; float radius ; gen_material material;
  sphere(vec3 center, float radius, gen_material material);
  struct sp_return hit(ray r, float t_min, float t_max , hit_record rec);
};

class hitable_list{
 public:
  hitable_list( std::vector<sphere> l, int n);
  struct sp_return hit(ray r, float t_min, float t_max , hit_record rec);

  float n;
  std::vector<sphere> List;
};

//***************** Camera class  */

class camera{
 public:
  vec3 lower_left_corner = vec3(-2.0, -1.0, -1.0);
  vec3 horizontal = vec3(4.0, 0.0, 0.0);
  vec3 vertical = vec3(0.0, 2.0, 0.0);
  vec3 origin = vec3(0.0, 0.0, 0.0);

  ray get_ray(float u, float v);
};


#endif
