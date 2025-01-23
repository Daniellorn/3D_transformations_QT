#include "ekran.h"

#include <QPainter>


using namespace math;

Ekran::Ekran(QWidget *parent)
    : QWidget{parent}, m_translateXValue(0), m_translateYValue(0),
    m_rotationValueX(0), m_rotationValueY(0), m_rotationValueZ(0),
    m_scaleXValue(1), m_scaleYValue(1)
{

    m_canvas = QImage(900, 700, QImage::Format_RGB32);
    m_canvas.fill(0);

    setMinimumSize(1100, 800);

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

    setupUI();
    draw3D(0, 0, 0, 0, 0, 1, 1);
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


void Ekran::draw3D(float translationX, float translationY, float radianX, float radianY, float radianZ, float scaleX, float scaleY)
{
    m_canvas.fill(0);

    float AspectRatio = m_canvas.width() / (float)m_canvas.height();

    math::mat4 projection(1.0f);
    projection = math::mat4::perspective(math::mat4::radians(45.0f), AspectRatio, 0.1f, 100.0f);

    //math::mat4 toZero(1.0f);
    //toZero = math::mat4::translation(toZero, vec3());

    math::mat4 model1(1.0f);
    model1 = math::mat4::translation(model1, vec3(3.0f, 2.5f, 3.0f));
    //model1 = math::mat4::translation(model1, vec3(-m_canvas.width()/2.0f, -m_canvas.height()/2.0f, 3.0f));

    math::mat4 model2(1.0f);
    model2 = rotationMatrix(radianX, radianY, radianZ);

    math::mat4 model3(1.0f);
    model3 = math::mat4::translation(model3, vec3(translationX, translationY, 1.0f));

    math::mat4 model(1.0f);

    model = model3 * model1 * model2;

    math::mat4 view(1.0f);
    view = math::mat4::translation(view, vec3(0.0f, 0.0f, 1.0f));


    math::mat4 view2(1.0f);
    view2 = math::mat4::scaleXY(view, vec3{ m_canvas.width() / 2.0f, m_canvas.height() / 2.0f, 1.0f});

    view = view2 * view;

    math::mat4 ClipSpaceMatrix(1.0f);
    ClipSpaceMatrix = projection * view * model;


    std::array<Triangle, 12> projTriangle;


    int i = 1;
    for (const auto& tri: m_triangles)
    {
        Triangle triProj;
        Triangle triTrans = tri;
        Triangle test = tri;


        triTrans.triangle[0] = model * triTrans.triangle[0];
        triTrans.triangle[1] = model * triTrans.triangle[1];
        triTrans.triangle[2] = model * triTrans.triangle[2];


        triProj.triangle[0] = projection * triTrans.triangle[0];
        triProj.triangle[1] = projection * triTrans.triangle[1];
        triProj.triangle[2] = projection * triTrans.triangle[2];
//
//
        //triProj.triangle[0] = view * triTrans.triangle[0];
        //triProj.triangle[1] = view * triTrans.triangle[1];
        //triProj.triangle[2] = view * triTrans.triangle[2];

       // //qDebug() << "Trojkat nr:" <<
       // //    " x1:" << triProj.triangle[0].x <<
       // //    " y1:" << triProj.triangle[0].y <<
       // //    " x2:" << triProj.triangle[1].x <<
       // //    " y2:" << triProj.triangle[1].y <<
       // //    " x3:" << triProj.triangle[2].x <<
       // //    " y3:" << triProj.triangle[2].y;
//
//
       //triProj.triangle[0].x += 1.0f;
       //triProj.triangle[0].y += 1.0f;
//
       //triProj.triangle[1].x += 1.0f;
       //triProj.triangle[1].y += 1.0f;
//
       //triProj.triangle[2].x += 1.0f;
       //triProj.triangle[2].y += 1.0f;
//
//
       //triProj.triangle[0].x *= 0.25f * m_canvas.width();
       //triProj.triangle[0].y *= 0.25f * m_canvas.height();
//
       //triProj.triangle[1].x *= 0.25f * m_canvas.width();
       //triProj.triangle[1].y *= 0.25f * m_canvas.height();
//
       //triProj.triangle[2].x *= 0.25f * m_canvas.width();
       //triProj.triangle[2].y *= 0.25f * m_canvas.height();
//
       triProj.triangle[0] = view * triProj.triangle[0];
       triProj.triangle[1] = view * triProj.triangle[1];
       triProj.triangle[2] = view * triProj.triangle[2];

       //triProj.triangle[0] = model * triProj.triangle[0];
       //triProj.triangle[1] = model * triProj.triangle[1];
       //triProj.triangle[2] = model * triProj.triangle[2];


        //triTrans.triangle[0] = projection * triTrans.triangle[0];
        //triTrans.triangle[1] = projection * triTrans.triangle[1];
        //triTrans.triangle[2] = projection * triTrans.triangle[2];

       test.triangle[0] = ClipSpaceMatrix * test.triangle[0];
       test.triangle[1] = ClipSpaceMatrix * test.triangle[1];
       test.triangle[2] = ClipSpaceMatrix * test.triangle[2];


        //drawTriangle(m_canvas, triProj.triangle[0].x, triProj.triangle[0].y,
        //                       triProj.triangle[1].x, triProj.triangle[1].y,
        //                       triProj.triangle[2].x, triProj.triangle[2].y);

        drawTriangle(m_canvas, test.triangle[0].x, test.triangle[0].y,
                               test.triangle[1].x, test.triangle[1].y,
                               test.triangle[2].x, test.triangle[2].y);
        i++;

    }



    //drawLineBresenham(m_canvas, 0, 0, 400, 400);

    update();

}

void Ekran::setupUI()
{
    m_mainLayout = new QHBoxLayout(this);

    m_leftPanel = new QWidget(this);
    m_leftPanel->setMinimumSize(1000, 700);
    m_mainLayout->addWidget(m_leftPanel);

    //m_spacer = new QSpacerItem(5, 100, QSizePolicy::Expanding, QSizePolicy::Expanding);
    //m_mainLayout->addItem(m_spacer);

    m_rightPanel = new QWidget(this);
    m_rightLayout = new QVBoxLayout(m_rightPanel);
    m_rightPanel->setMinimumSize(300, 750);

    m_translationLabel = new QLabel("Translation", this);
    m_rightLayout->addWidget(m_translationLabel);

    m_translateXSlider = new QSlider(Qt::Horizontal, this);
    m_translateXSlider->setRange(-m_canvas.width()/100, m_canvas.width()/100);
    m_translateXSlider->setMinimumWidth(0);
    m_translateXSlider->setValue(0);
    m_translateYSlider = new QSlider(Qt::Horizontal, this);
    m_translateYSlider->setRange(-m_canvas.height()/100, m_canvas.height()/100);
    m_translateYSlider->setValue(0);
    m_rightLayout->addWidget(m_translateXSlider);
    m_rightLayout->addWidget(m_translateYSlider);

    m_rotateLabel = new QLabel("Rotation", this);
    m_rightLayout->addWidget(m_rotateLabel);

    m_rotateSliderX = new QSlider(Qt::Horizontal, this);
    m_rotateSliderX->setRange(0, 360);
    m_rightLayout->addWidget(m_rotateSliderX);

    m_rotateSliderY = new QSlider(Qt::Horizontal, this);
    m_rotateSliderY->setRange(0, 360);
    m_rightLayout->addWidget(m_rotateSliderY);

    m_rotateSliderZ = new QSlider(Qt::Horizontal, this);
    m_rotateSliderZ->setRange(0, 360);
    m_rightLayout->addWidget(m_rotateSliderZ);

    m_scalingLabel = new QLabel("Scale", this);
    m_rightLayout->addWidget(m_scalingLabel);

    m_scaleXSpinBox = new QDoubleSpinBox(this);
    m_scaleXSpinBox->setRange(0.1, 10.0);
    m_scaleXSpinBox->setValue(1.0);
    m_scaleXSpinBox->setDecimals(2);
    m_scaleXSpinBox->setSingleStep(0.1);

    m_scaleYSpinBox = new QDoubleSpinBox(this);
    m_scaleYSpinBox->setRange(0.1, 10.0);
    m_scaleYSpinBox->setValue(1.0);
    m_scaleYSpinBox->setDecimals(2);
    m_scaleYSpinBox->setSingleStep(0.1);

    m_rightLayout->addWidget(m_scaleXSpinBox);
    m_rightLayout->addWidget(m_scaleYSpinBox);

    m_resetButton = new QPushButton("Reset", this);
    m_rightLayout->addWidget(m_resetButton);

    m_rightPanel->setLayout(m_rightLayout);
    m_mainLayout->addWidget(m_rightPanel);

    setLayout(m_mainLayout);

    connect(m_translateXSlider, &QSlider::valueChanged, this, &Ekran::onTranslateXChanged);
    connect(m_translateYSlider, &QSlider::valueChanged, this, &Ekran::onTranslateYChanged);
    connect(m_rotateSliderX, &QSlider::valueChanged, this, &Ekran::onRotationXChanged);
    connect(m_rotateSliderY, &QSlider::valueChanged, this, &Ekran::onRotationYChanged);
    connect(m_rotateSliderZ, &QSlider::valueChanged, this, &Ekran::onRotationZChanged);
    connect(m_scaleXSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Ekran::onScaleXChanged);
    connect(m_scaleYSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Ekran::onScaleYChanged);


    connect(m_resetButton, &QPushButton::clicked, this, &Ekran::onButtonClicked);
}

math::mat4 Ekran::rotationMatrix(float radianX, float radianY, float radianZ)
{
    math::mat4 RMatrix(1.0f);

    math::mat4 rotX(1.0f);
    rotX = math::mat4::rotationX(rotX, radianX);

    math::mat4 rotY(1.0f);
    rotY = math::mat4::rotationY(rotY, radianY);

    math::mat4 rotZ(1.0f);
    rotZ = math::mat4::rotationZ(rotZ, radianZ);

    RMatrix = rotZ * rotY * rotX;

    return RMatrix;
}

void Ekran::onTranslateXChanged(int value)
{
    m_translateXValue = value;

    draw3D(m_translateXValue, m_translateYValue, m_rotationValueX,  m_rotationValueY,  m_rotationValueZ, m_scaleXValue, m_scaleYValue);
}

void Ekran::onTranslateYChanged(int value)
{
    m_translateYValue = value;

     draw3D(m_translateXValue, m_translateYValue, m_rotationValueX,  m_rotationValueY,  m_rotationValueZ, m_scaleXValue, m_scaleYValue);
}

void Ekran::onRotationXChanged(int value)
{
    m_rotationValueX = value * M_PI / 180;

     draw3D(m_translateXValue, m_translateYValue, m_rotationValueX,  m_rotationValueY,  m_rotationValueZ, m_scaleXValue, m_scaleYValue);
}

void Ekran::onRotationYChanged(int value)
{
    m_rotationValueY = value * M_PI / 180;

     draw3D(m_translateXValue, m_translateYValue, m_rotationValueX,  m_rotationValueY,  m_rotationValueZ, m_scaleXValue, m_scaleYValue);
}

void Ekran::onRotationZChanged(int value)
{
    m_rotationValueZ = value * M_PI / 180;

     draw3D(m_translateXValue, m_translateYValue, m_rotationValueX,  m_rotationValueY,  m_rotationValueZ, m_scaleXValue, m_scaleYValue);
}

void Ekran::onScaleXChanged(double value)
{
    m_scaleXValue = (float)value;

}

void Ekran::onScaleYChanged(double value)
{
    m_scaleYValue = (float)value;
}


void Ekran::onButtonClicked()
{
    m_translateXValue = 0;
    m_translateYValue = 0;
    m_rotationValueX = 0;
    m_rotationValueY = 0;
    m_rotationValueZ = 0;
    m_scaleXValue = 1.0f;
    m_scaleYValue = 1.0f;


    m_translateXSlider->setValue(0);
    m_translateYSlider->setValue(0);
    m_rotateSliderX->setValue(0);
    m_rotateSliderY->setValue(0);
    m_rotateSliderZ->setValue(0);
    m_scaleXSpinBox->setValue(1.0);
    m_scaleYSpinBox->setValue(1.0);

}
