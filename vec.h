#ifndef VEC_H
#define VEC_H


namespace math {

struct vec3
{
    float x;
    float y;
    float z;


    vec3()
    {
        x = 1.0f;
        y = 1.0f;
        z = 1.0f;
    }
    vec3(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
};

struct vec4
{
    float x;
    float y;
    float z;
    float w;

    vec4()
    {
        x = 1.0f;
        y = 1.0f;
        z = 1.0f;
        w = 1.0f;
    }

    vec4(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
        w = 1.0f;
    }

    vec4(vec3 vector, float _w)
    {
        x = vector.x;
        y = vector.y;
        z = vector.z;
        w = _w;
    }


    vec4 operator/=(float value)
    {
        return vec4( (*this).x / value, (*this).y / value, (*this).z / value );
    }
};
}











#endif // VEC_H
