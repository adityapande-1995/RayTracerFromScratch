#!python3
from vec3 import *
import copy
from random import random

def random_in_unit_sphere():
    p = vec3(random(), random(), random())*2.0 - vec3(1,1,1)
    while dot(p,p) >= 1.0 :
        p = vec3(random(), random(), random())*2.0 - vec3(1,1,1)

    return p

def reflect(v,n):
    return v - dot(v,n)*n*2.0

class ray:
    def __init__(self, a, b):
        self.A = a ; self.B = b

    def origin(self):
        return self.A

    def direction(self):
        return self.B

    def point_at_parameter(self,t):
        return self.A + self.B*t

# Material class
class lambertian:
    def __init__(self, a):
        self.albedo = copy.deepcopy(a)

    def scatter(self, r_in, rec, attenuation, scattered):
        target = rec.p + rec.normal + random_in_unit_sphere()
        scattered = ray(rec.p , target - rec.p)
        attenuation = copy.deepcopy( self.albedo )
        return True # return scattered, attenuation ?



# World objects and tracing

class hit_record:
    def __init__(self, t=0 ,p=0 , normal=vec3(0,0,0), material=None):
        self.t = t ; self.p = p ; self.normal = normal ; self.material = material

class sphere:
    def __init__(self, center, radius):
        self.center = center ; self.radius = radius

    def hit(self, r, t_min, t_max, rec):
        oc = r.origin() - self.center
        a = dot(r.direction(), r.direction())
        b = dot(oc, r.direction())
        c = dot(oc, oc) - self.radius**2
        discriminant = b**2 - a*c
        if discriminant > 0:
            temp = (-b -(b**2 -a*c)**0.5)/a
            if temp < t_max and temp > t_min :
                rec.t = temp
                rec.p = r.point_at_parameter(rec.t)
                rec.normal = (rec.p - self.center)/self.radius
                return True, rec

            temp = (-b + (b**2 -a*c)**0.5)/a
            if temp < t_max and temp > t_min :
                rec.t = temp
                rec.p = r.point_at_parameter(rec.t)
                rec.normal = (rec.p - self.center)/self.radius
                return True, rec

        return False, rec

class hitable_list:
    def __init__(self, l, n):
        self.List = l ; self.List_size = n

    def hit(self, r, t_min, t_max, rec):
        temp_rec = hit_record()
        hit_anything = False
        closest_so_far = t_max
        for i in range(self.List_size):
            ifhit , temp_rec = self.List[i].hit( r,t_min, closest_so_far, temp_rec )
            if ifhit:
                hit_anything = True
                closest_so_far = temp_rec.t
                rec = copy.deepcopy(temp_rec)

        return hit_anything, rec

# Camera class
class camera:
    def __init__(self):
        self.lower_left_corner = vec3(-2.0, -1.0, -1.0)
        self.horizontal = vec3(4.0, 0.0, 0.0)
        self.vertical = vec3(0.0, 2.0, 0.0)
        self.origin = vec3(0.0, 0.0, 0.0)

    def get_ray(self,u,v):
        return ray(self.origin, self.lower_left_corner + self.horizontal*u + self.vertical*v)

