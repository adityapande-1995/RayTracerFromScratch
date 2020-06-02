#!python3

class vec3:
    def __init__(self, e0, e1, e2):
        self.e = [e0,e1,e2]

    def x(self):
        return self.e[0]
    def y(self):
        return self.e[1]
    def z(self):
        return self.e[2]
    def r(self):
        return self.e[0]
    def g(self):
        return self.e[1]
    def b(self):
        return self.e[2]

    # Overlaod unary operators
    def __neg__(self,other):
        return vec3(-self.e[0], -self.e[1], -self.e[2])
    def __getitem__(self, key):
        return self.e[key]

    # Overload binary operators
    def __add__(self, other):
        return vec3( self.e[0] + other.e[0], self.e[1] + other.e[1], self.e[2] + other.e[2] )
    def __sub__(self, other):
        return vec3( self.e[0] - other.e[0], self.e[1] - other.e[1], self.e[2] - other.e[2] )
    def __mul__(self, other):
        if type(other) != float :
            return vec3( self.e[0]*other.e[0], self.e[1]*other.e[1], self.e[2]*other.e[2] )
        else:
            return vec3( self.e[0]*other, self.e[1]*other, self.e[2]*other )

    def __truediv__(self, other):
        if type(other) != float:
            return vec3( self.e[0]/other.e[0], self.e[1]/other.e[1], self.e[2]/other.e[2] )
        else:
            return vec3( self.e[0]/other, self.e[1]/other, self.e[2]/other )


    # Other methods
    def length(self):
        return (self.e[0]*self.e[0] + self.e[1]*self.e[1] + self.e[2]*self.e[2])**0.5
    def squared_length(self):
        return self.e[0]*self.e[0] + self.e[1]*self.e[1] + self.e[2]*self.e[2]

    def make_unit_vector(self):
        pass


# Dot and cross products
def dot(v1, v2):
    return v1.e[0]*v2.e[0] + v1.e[1]*v2.e[1] + v1.e[2]*v2.e[2]

def cross(v1, v2):
    tx = v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]
    ty = -(v1.e[0]*v2.e[2] - v1.e[2]*v2.e[0])
    tz = v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]
    return vec3(tx, ty, tz)

def unit_vector(v):
    return v/v.length()




