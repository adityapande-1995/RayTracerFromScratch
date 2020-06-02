#!python3
from ray import *
from vec3 import *

def color(r , world, depth):
    rec = hit_record()
    ifhit, rec = world.hit(r, 0.0, 10000, rec)
    if ifhit:
        scattered = ray()
        attenuation = vec3(0,0,0)
        flag, attenuation, scattered = rec.material.scatter(r, rec, attenuation, scattered)
        if depth < 50 and flag :
            return attenuation*color(scattered, world, depth+1)
        else:
            return vec3(0,0,0)

    else:
        unit_direction = unit_vector( r.direction() )
        t = 0.5*( unit_direction.y() + 1.0  )
        return vec3(1.0, 1.0, 1.0)*(1.0 - t) + vec3(0.5, 0.7, 1.0)*t

if __name__ == '__main__':
    nx = 200*3
    ny = 100*3
    ns = 4 # Antialising samples per pixel

    print("P3")
    print(nx," ",ny,"\n255")

    cam = camera()

    temp_list = [sphere(vec3(0,0,-1), 0.5, lambertian(vec3(0.8,0.3,0.3)  ) )    ,
                 sphere(vec3(0,-100.5,-1), 100.0, lambertian(vec3(0.8,0.8,0.0)  )   ),
                 sphere(vec3(1,0,-1.25), 0.5, metal(vec3(0.8,0.6,0.2))   ),
                 sphere(vec3(-1,0,-1.25), 0.5, metal(vec3(0.8,0.8,0.8)  )   )]


    world = hitable_list(temp_list, len(temp_list))

    for j in range(ny-1, -1, -1):
        for i in range(0, nx, 1):
            col = vec3(0,0,0)
            # Antialising loop
            for s in range(0,ns,1):
                u = (i + random())/nx
                v = (j + random())/ny
                r = cam.get_ray(u,v)
                p = r.point_at_parameter(2.0)
                col = col + color(r,world,0)

            col = col/float(ns)

            gamma = 0.7
            ir = int(255.99* (col[0])**gamma )
            ig = int(255.99* (col[1])**gamma )
            ib = int(255.99* (col[2])**gamma )

            print(ir," ",ig," ",ib)

