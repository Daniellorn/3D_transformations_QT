#include "matrix.h"

#include <cmath>
#include <numbers>

#include <QDebug>

namespace math {


mat4::mat4(): m_columns(4), m_rows(4)
{
    m_matrix.fill(0);
}

mat4::mat4(float value): m_columns(4), m_rows(4)
{
    m_matrix.fill(0.0f);

    for (int i = 0; i < 4; i++)
    {
        m_matrix[4 * i + i] = value;
    }
}

float& mat4::operator()(int row, int col)
{
    return m_matrix[row * m_columns + col];
}

const float& mat4::operator()(int row, int col) const
{
    return m_matrix[row * m_columns + col];
}

mat4 mat4::operator*(const mat4& other)
{
    mat4 result;

    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_columns; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                result(i, j) += (*this)(i, k) * other(k, j);
            }
        }
    }

    return result;
}

vec4 mat4::operator*(const vec4& vector)
{
    vec4 result;

    result.x = (*this)(0, 0) * vector.x + (*this)(0, 1) * vector.y + (*this)(0, 2) * vector.z + (*this)(0, 3);
    result.y = (*this)(1, 0) * vector.x + (*this)(1, 1) * vector.y + (*this)(1, 2) * vector.z + (*this)(1, 3);
    result.z = (*this)(2, 0) * vector.x + (*this)(2, 1) * vector.y + (*this)(2, 2) * vector.z + (*this)(2, 3);
    result.w = (*this)(3, 0) * vector.x + (*this)(3, 1) * vector.y + (*this)(3, 2) * vector.z + (*this)(3, 3);

    if (result.w != 0)
    {
        result.x /= result.w;
        result.y /= result.w;
        result.z /= result.w;
    }


    return result;
}


vec3 mat4::operator*(const vec3& vector)
{
    vec3 result;

    result.x = (*this)(0, 0) * vector.x + (*this)(0, 1) * vector.y + (*this)(0, 2) * vector.z + (*this)(0, 3);
    result.y = (*this)(1, 0) * vector.x + (*this)(1, 1) * vector.y + (*this)(1, 2) * vector.z + (*this)(1, 3);
    result.z = (*this)(2, 0) * vector.x + (*this)(2, 1) * vector.y + (*this)(2, 2) * vector.z + (*this)(2, 3);
    float w  = (*this)(3, 0) * vector.x + (*this)(3, 1) * vector.y + (*this)(3, 2) * vector.z + (*this)(3, 3);

    if (w != 0)
    {
        result.x /= w;
        result.y /= w;
        result.z /= w;
    }

    return result;
}

float* mat4::Data()
{
    return m_matrix.data();
}

const float* mat4::Data() const
{
    return m_matrix.data();
}

mat4 mat4::translation(const mat4& matrix, const vec3& vector)
{
    mat4 result = matrix;

    result(0, 3) = vector.x;
    result(1, 3) = vector.y;
    result(2, 3) = vector.z;
    result(3, 3) = 1.0f;

    return result;
}

mat4 mat4::rotationX(const mat4& matrix, float radian)
{
    mat4 result = matrix;

    result(1, 1) = std::cos(radian);
    result(1, 2) = -std::sin(radian);
    result(2, 1) = std::sin(radian);
    result(2, 2) = std::cos(radian);

    return result;
}

mat4 mat4::rotationY(const mat4& matrix, float radian)
{
    mat4 result = matrix;

    result(0, 0) = std::cos(radian);
    result(0, 2) = std::sin(radian);
    result(2, 0) = -std::sin(radian);
    result(2, 2) = std::cos(radian);

    return result;
}

mat4 mat4::rotationZ(const mat4& matrix, float radian)
{
    mat4 result = matrix;

    result(0, 0) = std::cos(radian);
    result(0, 1) = -std::sin(radian);
    result(1, 0) = std::sin(radian);
    result(1, 1) = std::cos(radian);

    return result;
}

mat4 mat4::scaleXY(const mat4& matrix, const vec3& vector)
{
    mat4 result = matrix;

    result(0, 0) = vector.x;
    result(1, 1) = vector.y;
    result(2, 2) = vector.z;
    result(3, 3) = 1.0f;

    return result;
}

mat4 mat4::perspective(float radians, float AspectRatio, float n, float f)
{
    mat4 result(1.0f);

    result(0, 0) = 1.0f / (AspectRatio * std::tan(radians / 2.0f));
    result(1, 1) = 1.0f / std::tan(radians / 2);
    result(2, 2) = f / (f - n);
    result(2, 3) = -(f*n) / (f - n);
    result(3, 2) =  1.0f;
    result(3, 3) = 0.0f;


    //result(0, 0) = 2.0f / 900;
    //result(1, 1) = 2.0f / 700;
    //result(2, 2) = -2.0f/(f - n);
    //result(2, 3) = -(f*n) / (f - n);
    //result(3, 2) =  0.0f;
    //result(3, 3) = 0.0f;

    return result;
}

float mat4::radians(float angle)
{
    return angle * std::numbers::pi_v<float> / 180;
}

int mat4::getColNum() const
{
    return m_columns;
}

int mat4::getRowNum() const
{
    return m_rows;
}

}

