#include <iostream>
#include <math.h>
#include <random>
#include <thread>
#include <vector>
#include "vec3.h"
#include "ray.h"

// To generate random numbers
std::random_device rd2;
std::mt19937 gen2(rd2());
std::uniform_real_distribution<> dis2(0, 1);

// class to store pixel values
class pixel{
public:
  int ir = 0; int ig = 0; int ib = 0;
  pixel(){}
  pixel(int ir , int ig, int ib){this->ir = ir ; this->ig = ig ; this->ib = ib;}
};


// Returns pixel color, given a rayand the world
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

// Raytracing calculations
void render(std::vector<pixel> &image  ,hitable_list world, camera cam, int ns, float gamma, int nx, int ny, int ny_start = 0, int ny_end = 0){
  // Check if entire image is needed
  if (ny_start == 0 && ny_end == 0){
    ny_start = ny -1 ; ny_end = 0;
  }

  // Start generating the image
  float u,v; vec3 col;
  ray r; vec3 p;

  int counter = 0;

  for (int j = ny_start ; j >= ny_end  ; j--){
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

      //std::cout<< ir << " " << ig << " " << ib << std::endl;
      pixel pix = pixel(ir,ig,ib);
      //image[counter] = pix;
      image.push_back(pix);
      counter++;
    }
  }
}

// Main
int main(int argc, char** argv){
  int nx = 200*4 ;int ny = 100*4 ; // Image size
  int ns = 20 ; // Antialiasing samples per pixel

  camera cam = camera();

  // List of objects in the world
  std::vector<sphere> objects;
  objects.push_back( sphere(vec3(0,0,-1), 0.5, gen_material(vec3(0.8,0.3,0.3), "lambert", 0) )  );
  objects.push_back( sphere(vec3(0,-100.5,-1), 100.0, gen_material(vec3(0.8,0.8,0.0), "lambert", 0) )  );
  objects.push_back( sphere(vec3(1,0,-1.25), 0.5, gen_material(vec3(0.8,0.6,0.2), "metal", 0.3) )  );
  objects.push_back( sphere(vec3(-1,0,-1.25), 0.5, gen_material(vec3(0.8,0.8,0.8), "metal", 0.1) )  );

  hitable_list world = hitable_list(objects, objects.size());
  float gamma = 0.7;

  // Number of threads to run
  int n_th = 4;

  int start = ny-1 ; int end = start - ny/n_th;
  std::vector<std::vector <pixel>> Images;
  std::vector<std::thread> Threads;

  for (int i = 0; i < n_th ;i++){
    std::vector <pixel> img;
    Images.push_back(img);
  }

  for (int i = 0; i < n_th ;i++){
    Threads.push_back( std::thread (render, std::ref(Images[i]) , world, cam, ns, gamma,  nx, ny, start , end) );
    // Update start and end
    start = end -1 ;
    end = start - ny/n_th;
    if (end < 0){end = 0;}
  }

  for (int i = 0 ; i < Threads.size() ; i++){
    Threads[i].join();
  }

  // Saving the image
  std::cout << "P3\n" << nx << " " << ny << "\n255\n" ;
  for (int i = 0 ; i < n_th ; i++){
    auto image = Images[i];
    for (int j = 0 ; j < image.size() ; j++){ std::cout << image[j].ir << " " << " " << image[j].ig << " " << image[j].ib << std::endl; };
  }

  return 0;
}


