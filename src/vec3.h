#ifndef VEC3_HPP
#define VEC3_HPP

#include <stdexcept>

struct Vec2{
    float x;
    float y;
    Vec2 copy(){ return {x,y}; }
    float& operator[](int i){
        switch(i){
            case 0:
                return x;
            case 1:
                return y;
            default:
                throw std::invalid_argument("Vector index out of range");
        }
    }
};

struct Vec3{
    float x;
    float y;
    float z;
    float& operator[](int i){
        switch(i){
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                throw std::invalid_argument("Vector index out of range");
        }
    }
    Vec3 copy(){ return {x,y,z}; }
    float mag(){
        return sqrt(x*x + y*y + z*z);
    }
    float magSq(){
        return x*x + y*y + z*z;
    }
    Vec3& normalize(){
        if(x == 0 && y == 0 && z == 0){
            throw std::invalid_argument("Trying to normalize vector with no length");
        }

        float m = mag();   
        x = x/m;
        y = y/m;
        z = z/m;
        return *this;
    }

    //math operations
    static float dot(const Vec3& a, const Vec3& b){
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }
};

Vec3 operator*(float n, const Vec3& v){
    return {v.x*n, v.y*n, v.z*n};
}

Vec3 operator*(const Vec3& v, float n){
    return {v.x*n, v.y*n, v.z*n};
}

Vec3& operator*=(Vec3& v, float n){
    v.x *= n;
    v.y *= n;
    v.z *= n;
    return v;
}

Vec3 operator+(const Vec3& a, const Vec3& b){
    return {a.x+b.x, a.y*b.y, a.z*b.z};
}

Vec3 operator-(const Vec3& a, const Vec3& b){
    return {a.x-b.x, a.y-b.y, a.z-b.z};
}

#endif