#ifndef MATRIX_H
#define MATRIX_H

#include <array>

#include "vec.h"

namespace math {

class mat4
{
public:

    mat4();
    mat4(float value);

    float& operator()(int row, int col);
    const float& operator()(int row, int col) const;

    mat4 operator*(const mat4& other);
    vec4 operator*(const vec4& vector);
    vec3 operator*(const vec3& vector);
    mat4& operator=(const mat4& other) = default;

    float* Data();
    const float* Data() const;

    static mat4 translation(const mat4& matrix, const vec3& vector);
    static mat4 rotationX(const mat4& matrix, float radian);
    static mat4 rotationY(const mat4& matrix, float radian);
    static mat4 rotationZ(const mat4& matrix, float radian);
    static mat4 scaleXY(const mat4& matrix, const vec3& vector);
    static mat4 perspective(float radians, float proportion, float n , float f);

    static float radians(float angle);

    int getColNum() const;
    int getRowNum() const;


private:

    std::array<float, 16> m_matrix;
    int m_columns;
    int m_rows;
};


}


#endif // MATRIX_H
