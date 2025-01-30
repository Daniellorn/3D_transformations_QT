#include "ekran.h"

#include <QPainter>


using namespace math;

Ekran::Ekran(QWidget *parent)
    : QWidget{parent}, m_translateXValue(0.0f), m_translateYValue(0.0f),
    m_rotationValueX(0.0f), m_rotationValueY(0.0f), m_rotationValueZ(0.0f),
    m_scaleXValue(1.0f), m_scaleYValue(1.0f), m_scaleZValue(1.0f), m_mode(BackFaceCullingMode::OFF)
{

    m_canvas = QImage(900, 700, QImage::Format_RGB32);
    m_canvas.fill(0);

    m_texture = QImage("Assets/Obrazek1.jpg");

    if (m_texture.isNull())
    {
        qDebug() << "Brak tekstury";
        std::exit(1);
    }

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
    draw3D(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
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

void Ekran::drawTriangle(QImage &img, int x1, int y1, int x2, int y2, int x3, int y3, const QImage &texture)
{
    int x_a = x1;
    int y_a = y1;

    int x_b = x2;
    int y_b = y2;

    int x_c = x3;
    int y_c = y3;

    int x_a_t = 100;
    int y_a_t = 400;
    int x_b_t = 200;
    int y_b_t = 400;
    int x_c_t = 150;
    int y_c_t = 50;

    int W = ((x_b - x_a) * (y_c - y_a)) - ((y_b - y_a) * (x_c - x_a));

    int min_y = std::min({y_a, y_b, y_c});
    int max_y = std::max({y_a, y_b, y_c});
    int min_x = std::min({x_a, x_b, x_c});
    int max_x = std::max({x_a, x_b, x_c});

    PixelColor textureColor;

    for (int y = min_y; y <= max_y; y++)
    {
        for (int x = min_x; x <= max_x; x++)
        {
            int W_v = ((x - x_a) * (y_c - y_a)) - ((x_c - x_a) * (y - y_a));
            int W_w = ((x_b - x_a) * (y - y_a)) - ((x - x_a) * (y_b - y_a));

            float v = W_v / (W * 1.0f);
            float w = W_w / (W * 1.0f);
            float u = 1 - v - w;

            if ((u >= 0 && v >= 0 && w >= 0) && (u <= 1 && v <= 1 && w <= 1))
            {

                float x_t = u * x_a_t + v * x_b_t + w * x_c_t;
                float y_t = u * y_a_t + v * y_b_t + w * y_c_t;

                PixelColor P1 = getPixel(m_texture, std::floor(x_t),
                                         std::ceil(y_t));
                PixelColor P2 = getPixel(m_texture, std::ceil(x_t),
                                         std::ceil(y_t));
                PixelColor P3 = getPixel(m_texture, std::ceil(x_t),
                                         std::floor(y_t));

                PixelColor P4 = getPixel(m_texture, std::floor(x_t),
                                         std::floor(y_t));

                float x_tt = x_t - std::floor(x_t);
                float y_tt = y_t - std::floor(y_t);
                textureColor.R = y_tt * ((1 - x_tt) * P1.R + x_tt * P2.R) + (1 - y_tt) * ((1 - x_tt) * P4.R + x_tt * P3.R);
                textureColor.G = y_tt * ((1 - x_tt) * P1.G + x_tt * P2.G) + (1 - y_tt) * ((1 - x_tt) * P4.G + x_tt * P3.G);
                textureColor.B = y_tt * ((1 - x_tt) * P1.B + x_tt * P2.B) + (1 - y_tt) * ((1 - x_tt) * P4.B + x_tt * P3.B);


                setPixel(m_canvas, x, y, textureColor);
            }
        }
    }
    update();
}


void Ekran::draw3D(float translationX, float translationY, float radianX, float radianY, float radianZ, float scaleX, float scaleY, float scaleZ)
{


    vec3 center(0.0f, 0.0f, 0.0f);
    int totalVertices = 0;
    for (const auto& tri : m_triangles) {
        for (const auto& vertex : tri.triangle) {
            center.x += vertex.x;
            center.y += vertex.y;
            center.z += vertex.z;
            totalVertices++;
        }
    }
    center.x /= totalVertices;
    center.y /= totalVertices;
    center.z /= totalVertices;


    math::mat4 toCenter(1.0f);
    toCenter = math::mat4::translation(toCenter, vec3(-center.x, -center.y, -center.z));

    math::mat4 fromCenter(1.0f);
    fromCenter = math::mat4::translation(fromCenter, vec3(center.x, center.y, center.z));


    m_canvas.fill(0);

    float AspectRatio = m_canvas.width() / (float)m_canvas.height();

    math::mat4 projection(1.0f);
    projection = math::mat4::perspective(math::mat4::radians(45.0f), AspectRatio, 0.1f, 100.0f);

    math::mat4 model1(1.0f);
    math::mat4 model2(1.0f);
    math::mat4 model3(1.0f);
    model1 = math::mat4::translation(model1, vec3(1.0f, 1.0f, 3.0f));
    model2 = math::mat4::scaleXY(model2, vec3{ m_canvas.width() / 2.0f, m_canvas.height() / 2.0f, 1.0f});
    model3 = math::mat4::translation(model3, vec3{(float)m_canvas.width()/ 2, (float)m_canvas.height() / 2, 1.0f});

    math::mat4 modelRotation(1.0f);
    modelRotation = rotationMatrix(radianX, radianY, radianZ);

    math::mat4 modelTranslation(1.0f);
    modelTranslation = math::mat4::translation(modelTranslation, vec3(translationX, translationY, 1.0f));

    math::mat4 modelScale(1.0f);
    modelScale = math::mat4::scaleXY(modelScale, vec3{scaleX, scaleY, scaleZ});

    math::mat4 model(1.0f);
    model = fromCenter * modelTranslation * model2 * model1 * modelScale * modelRotation * toCenter;



    math::mat4 viewTranslation(1.0f);
    viewTranslation = math::mat4::translation(viewTranslation, vec3{ m_canvas.width() / 2.0f, m_canvas.height() / 2.0f, 1.0f});


    math::mat4 viewScale(1.0f);
    viewScale = math::mat4::scaleXY(viewScale, vec3{ m_canvas.width() / 2.0f + 200.0f, m_canvas.height() / 2.0f + 200.0f, 1.0f});

    math::mat4 view(1.0f);
    view = viewTranslation;

    math::mat4 WorldMatrix(1.0f);
    WorldMatrix=    model;




    std::array<Triangle, 12> projTriangle;

    math::vec3 cameraPoint(0.0f, 0.0f, 0.0f);

    for (const auto& tri: m_triangles)
    {
        Triangle triTrans = tri;


        triTrans.triangle[0] = WorldMatrix * triTrans.triangle[0];
        triTrans.triangle[1] = WorldMatrix * triTrans.triangle[1];
        triTrans.triangle[2] = WorldMatrix * triTrans.triangle[2];


        math::vec3 normal, vecA, vecB, cameraVec;

        vecA.x = triTrans.triangle[1].x - triTrans.triangle[0].x;
        vecA.y = triTrans.triangle[1].y - triTrans.triangle[0].y;
        vecA.z = triTrans.triangle[1].z - triTrans.triangle[0].z;

        vecB.x = triTrans.triangle[2].x - triTrans.triangle[0].x;
        vecB.y = triTrans.triangle[2].y - triTrans.triangle[0].y;
        vecB.z = triTrans.triangle[2].z - triTrans.triangle[0].z;


        normal = math::mat4::crossProduct(vecA, vecB);

        cameraVec.x = triTrans.triangle[0].x - cameraPoint.x;
        cameraVec.y = triTrans.triangle[0].y - cameraPoint.y;
        cameraVec.z = triTrans.triangle[0].z - cameraPoint.z;

        float dotProd = math::mat4::dotProduct(normal, cameraVec);

        if (dotProd < 0.0f && m_mode == BackFaceCullingMode::ON)
        {
          // triTrans.triangle[0] = projection * triTrans.triangle[0];
          // triTrans.triangle[1] = projection * triTrans.triangle[1];
          // triTrans.triangle[2] = projection * triTrans.triangle[2];

           triTrans.triangle[0] = projection * toCenter * triTrans.triangle[0];
           triTrans.triangle[1] = projection * toCenter * triTrans.triangle[1];
           triTrans.triangle[2] = projection * toCenter * triTrans.triangle[2];


           drawTriangle(m_canvas, triTrans.triangle[0].x, triTrans.triangle[0].y,
                        triTrans.triangle[1].x, triTrans.triangle[1].y,
                        triTrans.triangle[2].x, triTrans.triangle[2].y, m_texture);
//
//
           // drawTriangle(m_canvas, triTrans.triangle[0].x + m_canvas.width()/2.0f, triTrans.triangle[0].y + m_canvas.height() /2.0f,
           //              triTrans.triangle[1].x + m_canvas.width() / 2.0f, triTrans.triangle[1].y  + m_canvas.height()/ 2.0f,
           //              triTrans.triangle[2].x + m_canvas.width()/2.0f, triTrans.triangle[2].y + m_canvas.height()/2.0f, m_texture);

        }


        if (m_mode == BackFaceCullingMode::OFF)
        {
            triTrans.triangle[0] = projection * toCenter * triTrans.triangle[0];
            triTrans.triangle[1] = projection * toCenter * triTrans.triangle[1];
            triTrans.triangle[2] = projection * toCenter * triTrans.triangle[2];


            drawTriangle(m_canvas, triTrans.triangle[0].x + m_canvas.width()/2.0f, triTrans.triangle[0].y + m_canvas.height() /2.0f,
                         triTrans.triangle[1].x + m_canvas.width() / 2.0f, triTrans.triangle[1].y  + m_canvas.height()/ 2.0f,
                         triTrans.triangle[2].x + m_canvas.width()/2.0f, triTrans.triangle[2].y + m_canvas.height()/2.0f);
        }
    }

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
    m_translateXSlider->setRange(-m_canvas.width() + 200.0f, m_canvas.width() + 200.0f);
    m_translateXSlider->setMinimumWidth(0);
    m_translateXSlider->setValue(0);
    m_translateYSlider = new QSlider(Qt::Horizontal, this);
    m_translateYSlider->setRange(-m_canvas.height() + 200.0f, m_canvas.height() + 200.0f);
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

    m_scaleZSpinBox = new QDoubleSpinBox(this);
    m_scaleZSpinBox->setRange(0.1, 10.0);
    m_scaleZSpinBox->setValue(1.0);
    m_scaleZSpinBox->setDecimals(2);
    m_scaleZSpinBox->setSingleStep(0.1);

    m_rightLayout->addWidget(m_scaleXSpinBox);
    m_rightLayout->addWidget(m_scaleYSpinBox);
    m_rightLayout->addWidget(m_scaleZSpinBox);

    m_backfaceCullingCheckBox = new QCheckBox("Back-face Culling", this);
    m_rightLayout->addWidget(m_backfaceCullingCheckBox);

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
    connect(m_scaleZSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &Ekran::onScaleZChanged);

    connect(m_backfaceCullingCheckBox, &QCheckBox::toggled, this, [this](bool checked)
            {
                m_mode = checked ? BackFaceCullingMode::ON : BackFaceCullingMode::OFF;

                draw3D(m_translateXValue, m_translateYValue, m_rotationValueX,
                     m_rotationValueY,  m_rotationValueZ, m_scaleXValue, m_scaleYValue, m_scaleZValue);

            });


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
    m_translateXValue = (float)value;

    draw3D(m_translateXValue, m_translateYValue, m_rotationValueX,  m_rotationValueY,  m_rotationValueZ,
           m_scaleXValue, m_scaleYValue, m_scaleZValue);
}

void Ekran::onTranslateYChanged(int value)
{
    m_translateYValue = (float)value;

    draw3D(m_translateXValue, m_translateYValue, m_rotationValueX,  m_rotationValueY,  m_rotationValueZ,
           m_scaleXValue, m_scaleYValue, m_scaleZValue);
}

void Ekran::onRotationXChanged(int value)
{
    m_rotationValueX = value * M_PI / 180;

    draw3D(m_translateXValue, m_translateYValue, m_rotationValueX,  m_rotationValueY,  m_rotationValueZ,
           m_scaleXValue, m_scaleYValue, m_scaleZValue);
}

void Ekran::onRotationYChanged(int value)
{
    m_rotationValueY = value * M_PI / 180;

    draw3D(m_translateXValue, m_translateYValue, m_rotationValueX,  m_rotationValueY,  m_rotationValueZ,
           m_scaleXValue, m_scaleYValue, m_scaleZValue);
}

void Ekran::onRotationZChanged(int value)
{
    m_rotationValueZ = value * M_PI / 180;

    draw3D(m_translateXValue, m_translateYValue, m_rotationValueX,  m_rotationValueY,  m_rotationValueZ,
           m_scaleXValue, m_scaleYValue, m_scaleZValue);
}

void Ekran::onScaleXChanged(double value)
{
    m_scaleXValue = (float)value;

    draw3D(m_translateXValue, m_translateYValue, m_rotationValueX,  m_rotationValueY,  m_rotationValueZ,
           m_scaleXValue, m_scaleYValue, m_scaleZValue);
}


void Ekran::onScaleYChanged(double value)
{
    m_scaleYValue = (float)value;

    draw3D(m_translateXValue, m_translateYValue, m_rotationValueX,  m_rotationValueY,  m_rotationValueZ,
           m_scaleXValue, m_scaleYValue, m_scaleZValue);
}


void Ekran::onScaleZChanged(double value)
{
    m_scaleZValue = (float)value;

    draw3D(m_translateXValue, m_translateYValue, m_rotationValueX,  m_rotationValueY,  m_rotationValueZ,
           m_scaleXValue, m_scaleYValue, m_scaleZValue);
}



void Ekran::onButtonClicked()
{
    m_translateXValue = 0.0f;
    m_translateYValue = 0.0f;
    m_rotationValueX = 0;
    m_rotationValueY = 0;
    m_rotationValueZ = 0;
    m_scaleXValue = 1.0f;
    m_scaleYValue = 1.0f;
    m_scaleZValue = 1.0f;


    m_translateXSlider->setValue(0);
    m_translateYSlider->setValue(0);
    m_rotateSliderX->setValue(0);
    m_rotateSliderY->setValue(0);
    m_rotateSliderZ->setValue(0);
    m_scaleXSpinBox->setValue(1.0);
    m_scaleYSpinBox->setValue(1.0);

    draw3D(m_translateXValue, m_translateYValue, m_rotationValueX,
           m_rotationValueY,  m_rotationValueZ, m_scaleXValue, m_scaleYValue, m_scaleZValue);


}

void Ekran::setBackFaceCullingMode()
{
    m_mode = BackFaceCullingMode::ON;

    draw3D(m_translateXValue, m_translateYValue, m_rotationValueX,
           m_rotationValueY,  m_rotationValueZ, m_scaleXValue, m_scaleYValue, m_scaleZValue);


}

void Ekran::setPixel(QImage &img, int x, int y, const PixelColor &color)
{
    int red = color.R;
    int green = color.G;
    int blue = color.B;

    if (x < 0 || x >= img.width() || y < 0 || y >= img.height()) return;

    uchar* line = img.scanLine(y);
    line[4*x] = blue; //blue
    line[4*x + 1] = green; //green
    line[4*x + 2] = red; //red
    //line[4*x + 3] = 255; // alpha
}

PixelColor Ekran::getPixel(const QImage &img, int x, int y) const
{

    if (x < 0 || x >= img.width() || y < 0 || y >= img.height()) return {0,0,0,0};


    const uchar* line = img.scanLine(y);
    uint8_t blue = line[4*x]; //blue
    uint8_t green = line[4*x + 1]; //green
    uint8_t red = line[4*x + 2]; //red
    //uint8_t alpha = line[4*x + 3]; // alpha


    return PixelColor{red, green, blue};
}
