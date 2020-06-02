#!python3
from ray import *
from vec3 import *
from random import random

def color(r , world):
    rec = hit_record()
    ifhit, rec = world.hit(r, 0.0, 10000, rec)
    if ifhit:
        # Rec should get updated internally
        target = rec.p + rec.normal + random_in_unit_sphere()
        #return vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1)*0.5
        return color( ray(rec.p , target - rec.p), world )*0.5
    else:
        unit_direction = unit_vector( r.direction() )
        t = 0.5*( unit_direction.y() + 1.0  )
        return vec3(1.0, 1.0, 1.0)*(1.0 - t) + vec3(0.5, 0.7, 1.0)*t

if __name__ == '__main__':
    nx = 200*1
    ny = 100*1
    ns = 4 # Antialising samples per pixel

    print("P3")
    print(nx," ",ny,"\n255")

    cam = camera()

    temp_list = [sphere(vec3(0,0,-1), 0.5), sphere(vec3(0,-100.5,-1), 100.0)]
    world = hitable_list(temp_list,2)

    for j in range(ny-1, -1, -1):
        for i in range(0, nx, 1):
            col = vec3(0,0,0)
            # Antialising loop
            for s in range(0,ns,1):
                u = (i + random())/nx
                v = (j + random())/ny
                r = cam.get_ray(u,v)
                p = r.point_at_parameter(2.0)
                col = col + color(r,world)

            col = col/float(ns)

            gamma = 0.7
            ir = int(255.99* (col[0])**gamma )
            ig = int(255.99* (col[1])**gamma )
            ib = int(255.99* (col[2])**gamma )

            print(ir," ",ig," ",ib)

