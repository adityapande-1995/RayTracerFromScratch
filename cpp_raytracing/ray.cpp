#include <iostream>
#include <random>
#include <math.h>
#include "vec3.h"
#include "ray.h"

// *************** Random number init
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0, 1);

// Random in unit sphere      -------- 
vec3 random_in_unit_sphere(){
  float t1 = dis(gen); float t2 = dis(gen); float t3 = dis(gen);
  vec3 p = vec3(t1,t2,t3)*2.0 - vec3(1,1,1);

  while ( dot(p,p) >= 1.0   ){
    float t1 = dis(gen);
    float t2 = dis(gen);
    float t3 = dis(gen);
    p = vec3(t1,t2,t3)*2.0 - vec3(1,1,1);
  }
  return p;
}

//******* Reflect function  ----------  
vec3 reflect(vec3 v, vec3 n){ return v - n*dot(v,n)*2.0; }

//******** Refract function */
bool refract(vec3 v, vec3 n, float ni_over_nt, vec3& refracted){
  vec3 uv = unit_vector(v);
  float dt = dot(uv ,n);
  float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1 - dt*dt);
  if (discriminant > 0){
    refracted = (v - n*dt)*ni_over_nt - n*sqrt(discriminant);
    return true;
  }
  else{
    return false;
  }
}

float schlick(float cosine, float ref_idx){
  float r0 = (1-ref_idx)/(1+ref_idx);
  r0 = r0*r0;
  return r0 + (1-r0)*pow((1-cosine),5);
}

//**********/ Ray class     -----------  
ray::ray(vec3 a, vec3 b){ this->A = a ; this->B = b; }
vec3 ray::origin(){ return this->A ;}
vec3 ray::direction(){ return this->B;  }
vec3 ray::point_at_parameter(float t){ return this->A + this->B*t; }

//**********/ hit record class --------- 
hit_record::hit_record(float t, vec3 p, vec3 normal, gen_material material ){
    this->t = t; this->p = p; this->normal = normal ; this->material = material;
}

//*****************************************/ Materials -----
// Gen materials class    ---------- 
gen_material::gen_material(vec3 a, std::string name, float fuzz){ this->albedo = a; this->name = name;
  if (fuzz > 1.0){
    this->fuzz = 1.0 ;
  }
  else{
    this->fuzz = fuzz;
  }
}
// Scatter function
struct material_return scatter(gen_material& material, ray r_in, hit_record rec, vec3 attenuation, ray scattered){
  if (material.name == "lambert"){
    vec3 target = rec.p + rec.normal + random_in_unit_sphere();
    scattered = ray(rec.p , target - rec.p);
    attenuation = material.albedo;
    struct material_return temp;
    temp.status = true; temp.att = attenuation ; temp.scat = scattered;
    return temp;
    }

  if (material.name == "metal"){
    vec3 reflected = reflect( unit_vector( r_in.direction() ), rec.normal );
    scattered = ray(rec.p , reflected + random_in_unit_sphere()*material.fuzz);
    attenuation = material.albedo;

    struct material_return temp;
    temp.status = ( dot( scattered.direction() , rec.normal) > 0 ) ; temp.att = attenuation ; temp.scat = scattered;
    return temp;
  }

  if (material.name == "dielectric"){
    vec3 outward_normal;
    vec3 reflected = reflect(r_in.direction(), rec.normal);
    float ni_over_nt;
    attenuation = vec3(1.0, 1.0, 1.0);
    vec3 refracted;
    float reflect_prob ; float cosine;
    if ( dot(r_in.direction(), rec.normal) > 0){
      outward_normal = -rec.normal;
      ni_over_nt = material.fuzz;
      cosine = material.fuzz*dot(r_in.direction(), rec.normal)/r_in.direction().length(); 
    }else{
      outward_normal = rec.normal;
      ni_over_nt = 1.0 / material.fuzz ;
      cosine = -dot(r_in.direction(), rec.normal)/r_in.direction().length(); 
    }


    if ( refract(r_in.direction(), outward_normal, ni_over_nt, refracted) ){
      reflect_prob = schlick(cosine, material.fuzz);
    }
    else{
      scattered = ray( rec.p,reflected );
      reflect_prob = 1.0;
    }

    if (dis(gen) < reflect_prob){
      scattered = ray( rec.p,reflected );
    }else{
      scattered = ray( rec.p,refracted );
    }

    struct material_return temp;
    temp.status= true; temp.att = attenuation ; temp.scat = scattered;
    return temp;

  }

  if (material.name == "none"){
    struct material_return temp;
    return temp;
  }

 }

// ****************  World objects
// Sphere
sphere::sphere(vec3 center, float radius, gen_material material){
  this->center = center ; this->radius = radius ; this->material = material;
}
struct sp_return sphere::hit(ray r, float t_min, float t_max , hit_record rec){
  vec3 oc = r.origin() - this->center;
  float a = dot(r.direction(), r.direction());
  float b = dot(oc, r.direction());
  float c = dot(oc, oc) - (this->radius)*(this->radius);
  float discriminant = b*b - a*c;
  if (discriminant > 0){
    float temp;
    temp = (-b -sqrt(b*b -a*c))/a ;
    if (temp < t_max && temp > t_min){
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = (rec.p - this->center)/this->radius;
      rec.material = this->material;

      struct sp_return myresult;
      myresult.status = true ; myresult.rec = rec;
      return myresult;
    }

    temp = (-b +sqrt(b*b -a*c))/a ;
    if (temp < t_max && temp > t_min){
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = (rec.p - this->center)/this->radius;
      rec.material = this->material;

      struct sp_return myresult;
      myresult.status = true ; myresult.rec = rec;
      return myresult;
    }
  }
  struct sp_return myresult;
  myresult.status = false ; myresult.rec = rec;
  return myresult;
}

//********* hitable list */
hitable_list::hitable_list(std::vector<sphere> l, int n){
  this->List = l ; this->n = n;
}
struct sp_return hitable_list::hit(ray r, float t_min, float t_max , hit_record rec){
  bool ifhit;
  hit_record temp_rec = hit_record();
  bool hit_anything = false;
  float closest_so_far = t_max;
  for (int i = 0 ; i < this->n ; i++){
    struct sp_return lol = this->List[i].hit( r,t_min, closest_so_far, temp_rec);
    ifhit = lol.status; temp_rec = lol.rec ;
    if (ifhit){
      hit_anything = true;
      closest_so_far = temp_rec.t;
      rec = temp_rec;
    }
  }

  struct sp_return send_results;
  send_results.status = hit_anything ; send_results.rec = rec;
  return send_results;
}

// ****************  Camera class
camera::camera(float vfov, float aspect){
  float theta = vfov*M_PI/180;
  float half_height = tan(theta/2);
  float half_width = aspect*half_height;
  this->lower_left_corner = vec3(-half_width, -half_height, -1.0);
  this->horizontal = vec3(2.0*half_width, 0.0, 0.0);
  this->vertical = vec3(0.0, 2*half_height, 0.0);
  this->origin = vec3(0.0, 0.0, 0.0);
}
camera::camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect){
  vec3 u,v,w;
  float theta = vfov*M_PI/180;
  float half_height = tan(theta/2);
  float half_width = aspect*half_height;
  this->origin = lookfrom;
  w = unit_vector(lookfrom - lookat);
  u = unit_vector(cross(vup, w));
  v = cross(w,u);
  this->lower_left_corner = vec3(-half_width, -half_height, -1.0);
  this->lower_left_corner = this->origin - u*half_width - v*half_height - w;
  this->horizontal = u*2*half_width;
  this->vertical = v*2*half_height;

}

ray camera::get_ray(float s, float t){
  return ray(this->origin, this->lower_left_corner + this->horizontal*s + this->vertical*t - this->origin);
}

