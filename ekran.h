#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QDoubleSpinBox>



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


    void draw3D(float translationX, float translationY, float radianX, float radianY, float radianZ,
                float scaleX, float scaleY, float scaleZ);
    void setupUI();


    math::mat4 rotationMatrix(float radianX, float radianY, float radianZ);


private:

    std::array<Triangle, 12> m_triangles;
    QImage m_canvas;
    QImage m_temp;



    QHBoxLayout* m_mainLayout;
    QWidget* m_leftPanel;
    QWidget* m_rightPanel;
    QVBoxLayout* m_rightLayout;

    QSlider* m_translateXSlider;
    QSlider* m_translateYSlider;
    QSlider* m_rotateSliderX;
    QSlider* m_rotateSliderY;
    QSlider* m_rotateSliderZ;
    QDoubleSpinBox* m_scaleXSpinBox;
    QDoubleSpinBox* m_scaleYSpinBox;
    QDoubleSpinBox* m_scaleZSpinBox;

    QLabel* m_translationLabel;
    QLabel* m_rotateLabel;
    QLabel* m_scalingLabel;

    QPushButton* m_resetButton;

    QSpacerItem* m_spacer;


    float m_translateXValue;
    float m_translateYValue;
    float m_rotationValueX;
    float m_rotationValueY;
    float m_rotationValueZ;
    float m_scaleXValue;
    float m_scaleYValue;
    float m_scaleZValue;


private slots:
    void onTranslateXChanged(int value);
    void onTranslateYChanged(int value);
    void onRotationXChanged(int value);
    void onRotationYChanged(int value);
    void onRotationZChanged(int value);
    void onScaleXChanged(double value);
    void onScaleYChanged(double value);
    void onScaleZChanged(double value);

    void onButtonClicked();



signals:
};

#endif // EKRAN_H
