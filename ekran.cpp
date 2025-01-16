#include "ekran.h"

#include <QPainter>


using namespace math;

Ekran::Ekran(QWidget *parent)
    : QWidget{parent}
{

    m_canvas = QImage(1920, 1080, QImage::Format_RGB32);
    m_canvas.fill(0);

    m_triangles = {
            // przód
            Triangle(math::vec3(0.0f, 0.0f, 0.0f), math::vec3(0.0f, 1.0f, 0.0f), math::vec3(1.0f, 1.0f, 0.0f)),
            Triangle(math::vec3(0.0f, 0.0f, 0.0f), math::vec3(1.0f, 1.0f, 0.0f), math::vec3(1.0f, 0.0f, 0.0f)),

            // prawa
            Triangle(math::vec3(1.0f, 0.0f, 0.0f), math::vec3(1.0f, 1.0f, 0.0f), math::vec3(1.0f, 1.0f, 1.0f)),
            Triangle(math::vec3(1.0f, 0.0f, 0.0f), math::vec3(1.0f, 1.0f, 1.0f), math::vec3(1.0f, 0.0f, 1.0f)),

            // tył
            Triangle(math::vec3(1.0f, 0.0f, 1.0f), math::vec3(1.0f, 1.0f, 1.0f), math::vec3(0.0f, 1.0f, 1.0f)),
            Triangle(math::vec3(1.0f, 0.0f, 1.0f), math::vec3(0.0f, 1.0f, 1.0f), math::vec3(0.0f, 0.0f, 1.0f)),

            // lewo
            Triangle(math::vec3(0.0f, 0.0f, 1.0f), math::vec3(0.0f, 1.0f, 1.0f), math::vec3(0.0f, 1.0f, 0.0f)),
            Triangle(math::vec3(0.0f, 0.0f, 1.0f), math::vec3(0.0f, 1.0f, 0.0f), math::vec3(0.0f, 0.0f, 0.0f)),

            // góra
            Triangle(math::vec3(0.0f, 1.0f, 0.0f), math::vec3(0.0f, 1.0f, 1.0f), math::vec3(1.0f, 1.0f, 1.0f)),
            Triangle(math::vec3(0.0f, 1.0f, 0.0f), math::vec3(1.0f, 1.0f, 1.0f), math::vec3(1.0f, 1.0f, 0.0f)),

            // dół
            Triangle(math::vec3(1.0f, 0.0f, 1.0f), math::vec3(0.0f, 0.0f, 1.0f), math::vec3(0.0f, 0.0f, 0.0f)),
            Triangle(math::vec3(1.0f, 0.0f, 1.0f), math::vec3(0.0f, 0.0f, 0.0f), math::vec3(1.0f, 0.0f, 0.0f))
    };

    draw3D();
}

void Ekran::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    //m_temp = m_canvas;

    p.drawImage(0, 0, m_canvas);


}

void Ekran::drawLineBresenham(QImage& img, int x1, int y1, int x2, int y2)
{
    if (std::abs(y2 - y1) == 0) //pozioma
    {
        if (x2 < x1)
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        for (int x = x1; x <= x2; x++)
        {
            drawPixel(img, x, y1, 0x00FF00);
        }
    }
    else if (std::abs(x2 - x1) == 0) //pionowa
    {
        if (y2 < y1)
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }

        for (int y = y1; y <= y2; y++)
        {
            drawPixel(img, x1, y, 0x00FF00);
        }
    }
    else if (std::abs(y2 - y1) <= std::abs(x2 - x1))
    {
        if (x1 > x2)
        {
            std::swap(x1,x2);
            std::swap(y1, y2);
        }

        int dx = x2 - x1;
        int dy = y2 - y1;

        int m = (dy < 0) ? -1 : 1;
        dy *= m;
        int y = y1;
        int D = 2 * dy - dx;

        for (int x = x1; x <= x2; x++)
        {
            drawPixel(img, x, y, 0x00FF00);
            if (D > 0)
            {
                y += m;
                D -= 2 * dx;
            }
            D += 2 * dy;
        }
    }
    else
    {
        if (y1 > y2)
        {
            std::swap(x1,x2);
            std::swap(y1, y2);
        }

        int dx = x2 - x1;
        int dy = y2 - y1;

        int m = (dx < 0) ? -1 : 1;
        dx *= m;

        int x = x1;
        int D = 2 * dx - dy;

        for (int y = y1; y <= y2; y++)
        {
            drawPixel(img, x, y, 0x00FF00);
            if (D > 0)
            {
                x += m;
                D -= 2 * dy;
            }
            D += 2 * dx;
        }
    }

    update();
}

void Ekran::drawPixel(QImage &img, int x, int y, int h_color)
{
    if (x < 0 || x >= img.width() || y < 0 || y >= img.height()) return;

    uchar* line = img.scanLine(y);
    line[4*x] = h_color & 0xFF; //blue
    line[4*x + 1] = (h_color >> 8) & 0xFF; //green
    line[4*x + 2] = (h_color >> 16) & 0xFF; //red
    line[4*x + 3] = 255; // alpha
}

void Ekran::drawTriangle(QImage &img, int x1, int y1, int x2, int y2, int x3, int y3)
{
    drawLineBresenham(img, x1, y1, x2, y2);
    drawLineBresenham(img, x2, y2, x3, y3);
    drawLineBresenham(img, x3, y3, x1, y1);
}


void Ekran::draw3D()
{

    float AspectRatio = m_canvas.width() / (float)m_canvas.height();

    math::mat4 projection(1.0f);
    projection = math::mat4::perspective(math::mat4::radians(45.0f), AspectRatio, 0.1f, 100.0f);


    math::mat4 translate(1.0f);
    translate = math::mat4::translation(translate, vec3(-2.0f, 0.0f, 5.0f));


    math::mat4 rot(1.0f);
    rot = math::mat4::rotationY(rot, math::mat4::radians(15.0f));

    math::mat4 rotZ(1.0f);
    rotZ = math::mat4::rotationZ(rotZ, math::mat4::radians(60.0f));

    math::mat4 sc(1.0f);
    sc = math::mat4::scaleXY(sc, vec3(2.0f,0.0f,0.0f));

    math::mat4 rotAll(1.0f);
    rotAll = rot * rotZ;


    std::array<Triangle, 12> projTriangle;


   // qDebug() << m_triangles[0].triangle[1].x << " " << m_triangles[0].triangle[1].y;
//
//
   // m_triangles[0].triangle[1] = rotAll * m_triangles[0].triangle[1];
//
   // qDebug() << m_triangles[0].triangle[1].x << " " << m_triangles[0].triangle[1].y;
//
   // qDebug() << "--------------------------------------------";
//
   // m_triangles[0].triangle[1] = translate * m_triangles[0].triangle[1];
//
   // qDebug() << m_triangles[0].triangle[1].x << " " << m_triangles[0].triangle[1].y;
//
   // qDebug() << "--------------------------------------------";


    int i = 1;
    for (const auto& tri: m_triangles)
    {
        Triangle triProj;
        Triangle triTrans = tri;


        //triTrans.triangle[0].z = tri.triangle[0].z + 3.0f;
        //triTrans.triangle[1].z = tri.triangle[1].z + 3.0f;
        //triTrans.triangle[2].z = tri.triangle[2].z + 3.0f;


        qDebug() << "Trojkat przed rotacja nr:" << i <<
            " x1:" << tri.triangle[0].x <<
            " y1:" << tri.triangle[0].y <<
            " x2:" << tri.triangle[1].x <<
            " y2:" << tri.triangle[1].y <<
            " x3:" << tri.triangle[2].x <<
            " y3:" << tri.triangle[2].y;


       // triTrans.triangle[0] = sc * tri.triangle[0];
       // triTrans.triangle[1] = sc * tri.triangle[1];
       // triTrans.triangle[2] = sc * tri.triangle[2];


        triTrans.triangle[0] = rotAll * triTrans.triangle[0];
        triTrans.triangle[1] = rotAll * triTrans.triangle[1];
        triTrans.triangle[2] = rotAll * triTrans.triangle[2];


         qDebug() << "Trojkat przed mnozeniem nr:" << i <<
             " x1:" << tri.triangle[0].x <<
             " y1:" << tri.triangle[0].y <<
             " x2:" << tri.triangle[1].x <<
             " y2:" << tri.triangle[1].y <<
             " x3:" << tri.triangle[2].x <<
             " y3:" << tri.triangle[2].y;


        triTrans.triangle[0] = translate * triTrans.triangle[0];
        triTrans.triangle[1] = translate * triTrans.triangle[1];
        triTrans.triangle[2] = translate * triTrans.triangle[2];




       // qDebug() << "Trojkat przed mnozeniem nr:" << i <<
       //     " x1:" << tri.triangle[0].x <<
       //     " y1:" << tri.triangle[0].y <<
       //     " x2:" << tri.triangle[1].x <<
       //     " y2:" << tri.triangle[1].y <<
       //     " x3:" << tri.triangle[2].x <<
       //     " y3:" << tri.triangle[2].y;
//

        triProj.triangle[0] = projection * triTrans.triangle[0];
        triProj.triangle[1] = projection * triTrans.triangle[1];
        triProj.triangle[2] = projection * triTrans.triangle[2];


        //qDebug() << "Trojkat nr:" <<
        //    " x1:" << triProj.triangle[0].x <<
        //    " y1:" << triProj.triangle[0].y <<
        //    " x2:" << triProj.triangle[1].x <<
        //    " y2:" << triProj.triangle[1].y <<
        //    " x3:" << triProj.triangle[2].x <<
        //    " y3:" << triProj.triangle[2].y;


        triProj.triangle[0].x += 1.0f;
        triProj.triangle[0].y += 1.0f;

        triProj.triangle[1].x += 1.0f;
        triProj.triangle[1].y += 1.0f;

        triProj.triangle[2].x += 1.0f;
        triProj.triangle[2].y += 1.0f;


        triProj.triangle[0].x *= 0.25f * m_canvas.width();
        triProj.triangle[0].y *= 0.25f * m_canvas.height();

        triProj.triangle[1].x *= 0.25f * m_canvas.width();
        triProj.triangle[1].y *= 0.25f * m_canvas.height();

        triProj.triangle[2].x *= 0.25f * m_canvas.width();
        triProj.triangle[2].y *= 0.25f * m_canvas.height();


        drawTriangle(m_canvas, triProj.triangle[0].x, triProj.triangle[0].y,
                               triProj.triangle[1].x, triProj.triangle[1].y,
                               triProj.triangle[2].x, triProj.triangle[2].y);
        i++;

    }



    //drawLineBresenham(m_canvas, 0, 0, 400, 400);

    update();

}


