#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>
#include "matrix.h"
#include <compare>

struct Triangle
{
    std::array<math::vec3, 3> triangle;

    Triangle(math::vec3 v1, math::vec3 v2, math::vec3 v3)
    {
        triangle[0] = v1;
        triangle[1] = v2;
        triangle[2] = v3;
    }

    Triangle() : triangle{math::vec3(), math::vec3(), math::vec3()} {}

    //auto operator<=>(const Triangle& other) const = default;

};


struct PixelColor
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t alpha;

    auto operator<=>(const PixelColor& other) const = default;
};


class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:

    void drawLineBresenham(QImage& img, int x1, int y1, int x2, int y2);
    void drawPixel(QImage& img, int x, int y, int h_color);
    void drawTriangle(QImage& img, int x1, int y1, int x2, int y2, int x3, int y3);


    void draw3D();



private:

    std::array<Triangle, 12> m_triangles;
    QImage m_canvas;
    QImage m_temp;


signals:
};

#endif // EKRAN_H
