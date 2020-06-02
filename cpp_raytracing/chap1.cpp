#include <iostream>
#include <math.h>
#include <random>
#include "vec3.h"
#include "ray.h"

// To generate random numbers
std::random_device rd2;
std::mt19937 gen2(rd2());
std::uniform_real_distribution<> dis2(0, 1);

vec3 color(ray r , hitable_list world, int depth){
  hit_record rec = hit_record();

  struct sp_return lol1 = world.hit(r, 0.0, 100000, rec);
  bool ifhit = lol1.status; rec = lol1.rec;

  if (ifhit){
    ray scattered = ray();
    vec3 attenuation = vec3();

    struct material_return lol2 = scatter(rec.material , r, rec, attenuation, scattered);
    attenuation = lol2.att ; scattered = lol2.scat;
    if ( depth < 50 && lol2.status){
      return attenuation*color(scattered , world, depth+1);
    }
    else{
      return vec3(0,0,0);
    }
  }
  else{
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5*(unit_direction.y() + 1.0 );
    return vec3(1.0, 1.0, 1.0)*(1.0 - t) + vec3(0.5, 0.7, 1.0)*t;
  }
}

int main(){
  int nx = 200*3 ;
  int ny = 100*3 ;
  int ns = 4 ;

  std::cout << "P3\n" << nx << " " << ny << "\n255\n" ;
  camera cam = camera();

  std::vector<sphere> objects;
  objects.push_back( sphere(vec3(0,0,-1), 0.5, gen_material(vec3(0.8,0.3,0.3), "metal") )  );
  objects.push_back( sphere(vec3(0,-100.5,-1), 100.0, gen_material(vec3(0.8,0.8,0.0), "lambert") )  );
  objects.push_back( sphere(vec3(1,0,-1.25), 0.5, gen_material(vec3(0.8,0.6,0.2), "metal") )  );
  objects.push_back( sphere(vec3(-1,0,-1.25), 0.5, gen_material(vec3(0.8,0.8,0.8), "metal") )  );

  hitable_list world = hitable_list(objects, 4);

  float u,v; vec3 col;
  ray r; vec3 p;
  float gamma = 0.7;

  for (int j = ny - 1 ; j > -1 ; j--){
    for (int i = 0 ; i < nx ; i++){
      col = vec3(0,0,0);
      // Antialiasing loop
      for (int s = 0 ; s < ns ; s++ ){
        u = (i + dis2(gen2))/nx;
        v = (j + dis2(gen2))/ny;
        r = cam.get_ray(u,v);
        p = r.point_at_parameter(2.0);
        col = col + color(r,world,0);
      }

      col = col/float(ns);

      int ir = (255.99*pow(col[0], gamma));
      int ig = (255.99*pow(col[1], gamma));
      int ib = (255.99*pow(col[2], gamma));

      std::cout<< ir << " " << ig << " " << ib << std::endl;
    }
  }

  return 0;
}


