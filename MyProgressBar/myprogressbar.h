#ifndef MyProgressBar_H
#define MyProgressBar_H

#include <QWidget>

enum barStyle
{
    Round,
    Pie,
    Battery,
    Line
};

enum DataPosition
{
    Hide = 0,
    Inner,
    Outer,
    top,
    bottom,
    Left,
    Right
};

class MyProgressBar : public QWidget
{
    Q_OBJECT
public:
    MyProgressBar( QWidget* parent );
    void setBarStyle( const barStyle style );      /* 设置进度条样式*/
    void setOutlinePenWidth( const int penWidth ); /* 设置轮廓线宽度*/
    void setDataPenWidth( const int penWidth );    /* 设置文字粗细*/
    void setBaseColor( const QColor base );        /* 设置背景颜色*/
    void setValueColor( const QColor value );      /* 设置进度条颜色*/
    void setBorderColor( const QColor border );    /* 设置轮廓颜色*/
    void setTextColor( const QColor text );        /* 设置进度值颜色*/
    void setDataPos( const DataPosition pos );     /* 设置进度值位置*/
    void setRange( const double min, const double max ); /* 设置进度条范围*/
    void setMax( const double max );                     /* 设置进度条最大值*/
    void setMin( const double min );                     /* 设置进度条最小值*/
    void setvalue( const double value );                 /* 设置当前进度值*/
    void setDecimals( const int decimals );              /* 设置当前进度精度*/

    /* 仅在进度条样式为Round有效*/
    void setInnerOuterDiameterRate( const double rate ); /* 设置内圈外圈比例*/
    void setStartAngel( const double pos );              /* 设置起始角度*/

    /* 仅在进度条样式为Line有效*/
    void setHeight(const double height);                /* 设置直线进度条高度*/
    void setRoundA(const int roundA);                /* 设置圆角*/
    void setMargin(const QPair<double, double> margin); /* 设置边距*/

    /* 仅在进度条样式为Batte有效*/
    void setTransition(const bool yes);                 /*是否显示阶段颜色*/
    void setTransitionColor(const QVector<QPair<int, QColor> > transitionColor);      /* 设置阶段颜色*/
    QVector<QPair<int, QColor> > getTransitionColor();      /* 获取阶段颜色*/


    double getMax();   /* 获取进度条最大值*/
    double getMin();   /* 获取进度条最小值*/
    double getvalue(); /* 获取当前进度值*/

protected:
    void paintEvent( QPaintEvent* );
    void drawbase( QPainter& p, const QRectF& baseRect, double outerDiameter );                   /* 画出基本图形*/
    void drawValue( QPainter& p, const QRectF& baseRect, double arcStep, double outerDiameter );  /* 画出进度值图形*/
    void drawText( QPainter& p, const QRectF& baseRect, double arcLength, double outerDiameter ); /* 画出进度值文字*/

private:
    barStyle m_barStyle;
    DataPosition m_textPosition;
    double m_min;
    double m_max;
    double m_value;
    int m_outlinePenWidth;
    int m_dataPenWidth;
    int m_decimals;
    bool m_showValue;
    QColor m_baseColor;
    QColor m_valueColor;
    QColor m_borderColor;
    QColor m_textColor;
    /* 圆盘 圆环参数*/
    double m_InnerDiaOuterDiaRate;
    double m_startAngel;
    /* 直线 电池参数*/
    double m_height;
    int m_roundpercent;
    QPair<double, double> m_margin;

    /* 电池参数*/
    bool m_isTransition;
    QVector<QPair<int, QColor> > m_transitionColor;
};

#endif // MyProgressBar_H
