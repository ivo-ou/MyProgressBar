#include "myprogressbar.h"
#include <QPainter>
#include <math.h>
#include <QtMath>
#include <QDebug>
#define EPS 1e-7 //判断浮点数是否位于0的一个很小的邻域内[-EPS,EPS]内

MyProgressBar::MyProgressBar( QWidget* parent )
    : QWidget( parent )
    , m_barStyle( barStyle::Round )
    , m_textPosition( DataPosition::Outer )
    , m_min( 0 )
    , m_max( 100 )
    , m_value( 25 )
    , m_outlinePenWidth( 1 )
    , m_dataPenWidth( 20 )
    , m_decimals( 0 )
    , m_showValue( true )
    , m_baseColor( QColor( 1, 255, 255, 100 ) )
    , m_valueColor( QColor( "#F5A515" ) )
    , m_borderColor( QColor( 1, 255, 255, 100 ) )
    , m_textColor( QColor( "#F5A515" ) )
    , m_InnerDiaOuterDiaRate( 0.9 )
    , m_startAngel( 90 )
    , m_height( 50 )
    , m_roundpercent( 50 )
    , m_margin( qMakePair( 2, 2 ) )
    , m_isTransition( true )
{
    m_transitionColor.push_back( qMakePair( 25, QColor( "red" ) ) );
    m_transitionColor.push_back( qMakePair( 75, QColor( "yellow" ) ) );
    m_transitionColor.push_back( qMakePair( 100, QColor( "green" ) ) );
}

void MyProgressBar::setBarStyle( const barStyle style )
{
    if ( style != m_barStyle )
    {
        m_barStyle = style;
        update();
    }
}

void MyProgressBar::setOutlinePenWidth( const int penWidth )
{
    if ( !( fabs( penWidth - m_outlinePenWidth ) <= EPS ) )
    {
        m_outlinePenWidth = penWidth;
        update();
    }
}

void MyProgressBar::setDataPenWidth( const int penWidth )
{
    if ( !( fabs( penWidth - m_dataPenWidth ) <= EPS ) )
    {
        m_dataPenWidth = penWidth;
        update();
    }
}

void MyProgressBar::setBaseColor( const QColor base )
{
    m_baseColor = base;
    update();
}
void MyProgressBar::setValueColor( const QColor value )
{
    m_valueColor = value;
    update();
}
void MyProgressBar::setBorderColor( const QColor border )
{
    m_borderColor = border;
    update();
}
void MyProgressBar::setTextColor( const QColor text )
{
    m_textColor = text;
    update();
}

void MyProgressBar::setDataPos( const DataPosition pos )
{
    if ( m_textPosition != pos )
    {
        m_textPosition = pos;
        update();
    }
}

void MyProgressBar::setInnerOuterDiameterRate( const double rate )
{
    if ( !( fabs( rate - m_InnerDiaOuterDiaRate ) <= EPS ) )
    {
        m_InnerDiaOuterDiaRate = rate;
        update();
    }
}

void MyProgressBar::setStartAngel( const double angle )
{
    if ( !( fabs( angle - m_startAngel ) <= EPS ) )
    {
        m_startAngel = angle;
        update();
    }
}

void MyProgressBar::setRange( const double min, const double max )
{
    m_min = min;
    m_max = max;

    if ( m_max < m_min )
    {
        qSwap( m_max, m_min );
    }

    if ( m_value < m_min )
    {
        m_value = m_min;
    }
    else if ( m_value > m_max )
    {
        m_value = m_max;
    }

    update();
}

void MyProgressBar::setMax( const double max )
{
    setRange( m_min, max );
}

void MyProgressBar::setMin( const double min )
{
    setRange( min, m_max );
}

void MyProgressBar::setvalue( const double value )
{
    if ( !( fabs( m_value - value ) <= EPS ) )
    {
        if ( value < m_min )
        {
            m_value = m_min;
        }
        else if ( value > m_max )
        {
            m_value = m_max;
        }
        else
        {
            m_value = value;
        }
        update();
    }
}

void MyProgressBar::setDecimals( const int decimals )
{
    m_decimals = decimals;
    update();
}

/* 仅在进度条样式为Line有效*/
void MyProgressBar::setHeight( const double height )
{
    m_height = height;
    update();
}
void MyProgressBar::setRoundA( const int roundA )
{
    m_roundpercent = roundA;
    update();
}
void MyProgressBar::setMargin( const QPair< double, double > margin )
{
    m_margin = margin;
    update();
}

void MyProgressBar::setTransition( const bool yes )
{
    m_isTransition = yes;
    update();
}
void MyProgressBar::setTransitionColor( const QVector< QPair< int, QColor > > transitionColor )
{
    m_transitionColor = transitionColor;
    update();
}

QVector< QPair< int, QColor > > MyProgressBar::getTransitionColor()
{
    return m_transitionColor;
}

double MyProgressBar::getMax()
{
    return m_max;
}

double MyProgressBar::getMin()
{
    return m_min;
}

double MyProgressBar::getvalue()
{
    return m_value;
}

void MyProgressBar::drawbase( QPainter& p, const QRectF& baseRect, double outerDiameter )
{
    p.save();
    QPen pen;
    pen.setColor( m_borderColor );
    pen.setWidth( m_outlinePenWidth );
    p.setPen( pen );
    switch ( m_barStyle )
    {
    case barStyle::Round:
    {
        QPainterPath path;
        path.addEllipse( baseRect.center(), outerDiameter / 2, outerDiameter / 2 );                                                           /* 外圈圆形*/
        path.addEllipse( baseRect.center(), ( outerDiameter / 2 ) * m_InnerDiaOuterDiaRate, ( outerDiameter / 2 ) * m_InnerDiaOuterDiaRate ); /* 内圈圆形*/

        p.setBrush( m_baseColor );
        p.drawPath( path );
        break;
    }
    case barStyle::Pie:
    {
        p.setBrush( QBrush( m_baseColor ) );
        p.drawEllipse( baseRect.center(), outerDiameter / 2, outerDiameter / 2 );
        break;
    }

    case barStyle::Line:
    {
        double offset_y = 0;
        double offset_x = 0;
        double offset_width = 0;
        QFont f;
        f.setFamily( "Noto Sans CJK SC Medium" );
        f.setPixelSize( m_dataPenWidth );
        QFontMetrics fm( f );
        // 当进度值显示在左右
        if ( m_textPosition >= 5 )
        {
            double textWidth = fm.width( tr( "%1% " ).arg( m_max ) ); // 计算最大进度值所占像素值
            if ( m_textPosition == 5 )
                offset_x = textWidth;
            else
                offset_width = textWidth;
        }
        else if ( 3 == m_textPosition || m_textPosition == 4 )
        {
            double textHeight = fm.ascent() + fm.descent();
            if ( m_textPosition == 3 )
                offset_y = textHeight;
            else
                offset_y = -textHeight;
        }
        QPainterPath path;
        path.addRoundRect( baseRect.topLeft().x() + offset_x, ( baseRect.center().y() - m_height / 2 ) + offset_y, outerDiameter, m_height, m_roundpercent );
        p.setBrush( QBrush( m_baseColor ) );
        p.drawPath( path );
        break;
    }
    case barStyle::Battery:
    {
        double offset_y = 0;
        double offset_x = 0;
        double offset_width = 0;
        QFont f;
        f.setFamily( "Noto Sans CJK SC Medium" );
        f.setPixelSize( m_dataPenWidth );
        QFontMetrics fm( f );
        // 当进度值显示在左右
        if ( m_textPosition >= 5 )
        {
            double textWidth = fm.width( tr( "%1% " ).arg( m_max ) ); // 计算最大进度值所占像素值
            if ( m_textPosition == 5 )
                offset_x = textWidth;
            else
                offset_width = textWidth;
        }
        else if ( 3 == m_textPosition || m_textPosition == 4 )
        {
            double textHeight = fm.ascent() + fm.descent();
            if ( m_textPosition == 3 )
                offset_y = textHeight;
            else
                offset_y = -textHeight;
        }
        double x = baseRect.topLeft().x() + offset_x;
        double y = ( baseRect.center().y() - m_height / 2 ) + offset_y;
        double connerSize = ( m_height / 2 ) * m_roundpercent / 100;
        int headRound = 5;
        QRectF batteryRect( x, y, outerDiameter - m_height / 6, m_height );
        QPainterPath path;
        // 左上角圆角
        path.moveTo( batteryRect.left() + connerSize, batteryRect.top() );
        path.arcTo( batteryRect.left(), batteryRect.top(), 2 * connerSize, 2 * connerSize, 90.0, 90.0 );
        //左下角圆角
        path.lineTo( batteryRect.left(), batteryRect.bottom() - connerSize );
        path.arcTo( batteryRect.left(), batteryRect.bottom() - 2 * connerSize, 2 * connerSize, 2 * connerSize, 180.0, 90.0 );

        //右下角圆角
        path.lineTo( batteryRect.right() - connerSize, batteryRect.bottom() );
        path.arcTo( batteryRect.right() - 2 * connerSize, batteryRect.bottom() - 2 * connerSize, 2 * connerSize, 2 * connerSize, 270.0, 90.0 );

        //        // 右上角圆角
        path.lineTo( batteryRect.right(), batteryRect.top() + connerSize );
        path.arcTo( batteryRect.right() - 2 * connerSize, batteryRect.top(), 2 * connerSize, 2 * connerSize, 0.0, 90.0 );

        path.lineTo( batteryRect.left() + connerSize, batteryRect.top() );

        p.setBrush( m_baseColor );
        p.drawPath( path );

        QPainterPath headPath;
        // 电池头部左下圆角
        //        headPath.moveTo(batteryRect.right(), batteryRect.bottom() - m_height/3 + headRound)
        headPath.moveTo( batteryRect.right(), batteryRect.bottom() - m_height / 3 + headRound );
        headPath.arcTo( batteryRect.right(), batteryRect.bottom() - m_height / 3, 2 * headRound, 2 * headRound, 180.0, -90.0 );
        // 电池头部右下圆角
        headPath.lineTo( batteryRect.right() + m_height / 6 - headRound, batteryRect.bottom() - m_height / 3 );
        headPath.arcTo( batteryRect.right() + m_height / 6 - 2 * headRound, batteryRect.bottom() - m_height / 3 - 2 * headRound, 2 * headRound, 2 * headRound, 270.0, 90.0 );

        //        // 电池头部右上角
        headPath.lineTo( batteryRect.right() + m_height / 6, batteryRect.top() + m_height / 3 + headRound );
        headPath.arcTo( batteryRect.right() + m_height / 6 - 2 * headRound, batteryRect.top() + m_height / 3, 2 * headRound, 2 * headRound, 0.0, 90.0 );
        //        // 电池头部左上角
        headPath.lineTo( batteryRect.right() + headRound, batteryRect.top() + m_height / 3 );
        headPath.arcTo( batteryRect.right(), batteryRect.top() + m_height / 3 - 2 * headRound, 2 * headRound, 2 * headRound, 270.0, -90.0 );
        p.setBrush( m_borderColor );
        p.drawPath( headPath );
        break;
    }
    }
    p.restore();
}

void MyProgressBar::drawValue( QPainter& p, const QRectF& baseRect, double arcStep, double outerDiameter )
{
    if ( arcStep / 360 <= EPS )
        return;
    p.save();
    switch ( m_barStyle )
    {
    case barStyle::Round:
    {
        auto center = baseRect.center();
        double middleRound = outerDiameter * ( 1 + m_InnerDiaOuterDiaRate ) / 2; /* 圆心到内外圈中间的距离(半径)*/
        double startPoint = center.x() - middleRound / 2;
        QRectF innerRect( startPoint, startPoint, middleRound, middleRound );
        QPen pen;
        pen.setColor( m_valueColor );
        pen.setWidthF( outerDiameter - middleRound - m_outlinePenWidth );
        pen.setCapStyle( Qt::RoundCap );
        p.setPen( pen );
        p.drawArc( innerRect, static_cast< int >( m_startAngel * 16 ), static_cast< int >( -16 * arcStep ) );
        break;
    }
    case barStyle::Pie:
    {
        // 获取中心点坐标
        QRectF pieRect( baseRect.topLeft(), QSizeF( outerDiameter, outerDiameter ) );
        QPointF centerPoint = baseRect.center();
        pieRect.moveCenter( centerPoint );
        QPainterPath dataPath;
        dataPath.setFillRule( Qt::WindingFill );
        dataPath.moveTo( centerPoint );

        // 逆时针画弧长
        dataPath.arcTo( pieRect, m_startAngel, -arcStep );
        dataPath.lineTo( centerPoint );
        QPen pen;
        pen.setColor( m_borderColor );
        pen.setWidthF( m_outlinePenWidth );
        p.setPen( pen );
        p.setBrush( QBrush( m_valueColor ) );
        p.drawPath( dataPath );
        break;
    }
    case barStyle::Line:
    {
        QPainterPath path;
        path.setFillRule( Qt::WindingFill );
        double lenght = arcStep / 360 * ( outerDiameter - 2 * m_margin.second - 2 * m_outlinePenWidth );

        double offset_y = 0;
        double offset_x = 0;
        double offset_width = 0;
        QFont f;
        f.setFamily( "Noto Sans CJK SC Medium" );
        f.setPixelSize( m_dataPenWidth );
        QFontMetrics fm( f );
        // 当进度值显示在左右
        if ( m_textPosition >= 5 )
        {
            double textWidth = fm.width( tr( "%1% " ).arg( m_max ) ); // 计算最大进度值所占像素值
            if ( m_textPosition == 5 )
                offset_x = textWidth;
            else
                offset_width = textWidth;
        }
        else if ( 3 == m_textPosition || m_textPosition == 4 )
        {
            double textHeight = fm.ascent() + fm.descent();
            if ( m_textPosition == 3 )
                offset_y = textHeight;
            else
                offset_y = -textHeight;
        }

        QRectF lineRect( baseRect.topLeft().x() + m_margin.second + offset_x + m_outlinePenWidth, baseRect.center().y() - m_height / 2 + m_margin.first + offset_y + m_outlinePenWidth, lenght, m_height - 2 * m_margin.first - 2 * m_outlinePenWidth );

        path.addRoundRect( lineRect, m_roundpercent );
        //        QPen pen;
        //        pen.setColor( m_borderColor );
        //        pen.setWidthF( m_outlinePenWidth );
        p.setPen( QPen( QColor( 0, 0, 0, 0 ) ) );
        p.setBrush( QBrush( m_valueColor ) );
        QPainterPath path_mask;
        path_mask.addRoundRect( baseRect.topLeft().x() + m_margin.second + offset_x, ( baseRect.center().y() - m_height / 2 ) + m_margin.first + offset_y - 1, outerDiameter - m_margin.second * 2, m_height - m_margin.first * 2, m_roundpercent ); // 防止进度值超出外框
        p.setClipPath( path_mask );
        p.drawPath( path );
        break;
    }
    case barStyle::Battery:
    {
        QPainterPath path;
        path.setFillRule( Qt::WindingFill );
        double lenght = arcStep / 360 * ( outerDiameter - m_height / 6 - 2 * m_margin.second - 2 * m_outlinePenWidth );

        double offset_y = 0;
        double offset_x = 0;
        double offset_width = 0;
        QFont f;
        f.setFamily( "Noto Sans CJK SC Medium" );
        f.setPixelSize( m_dataPenWidth );
        QFontMetrics fm( f );
        // 当进度值显示在左右
        if ( m_textPosition >= 5 )
        {
            double textWidth = fm.width( tr( "%1% " ).arg( m_max ) ); // 计算最大进度值所占像素值
            if ( m_textPosition == 5 )
                offset_x = textWidth;
            else
                offset_width = textWidth;
        }
        else if ( 3 == m_textPosition || m_textPosition == 4 )
        {
            double textHeight = fm.ascent() + fm.descent();
            if ( m_textPosition == 3 )
                offset_y = textHeight;
            else
                offset_y = -textHeight;
        }

        double x = baseRect.topLeft().x() + m_margin.second + offset_x + m_outlinePenWidth;
        double y = ( baseRect.center().y() - m_height / 2 ) + m_margin.first + offset_y + m_outlinePenWidth;
        QRectF batteryRect( x, y, lenght, m_height - 2 * m_margin.first - 2 * m_outlinePenWidth );
        path.addRoundRect( batteryRect, m_roundpercent );
        //        QPen pen;
        //        pen.setColor( m_borderColor );
        //        pen.setWidthF( m_outlinePenWidth );
        //        p.setPen( pen );
        p.setPen( QPen( QColor( 0, 0, 0, 0 ) ) );
        p.setBrush( QBrush( m_valueColor ) );
        if ( m_isTransition )
        {
            if ( m_value <= m_transitionColor.at( 0 ).first )
                p.setBrush( QBrush( m_transitionColor.at( 0 ).second ) );
            if ( m_value <= m_transitionColor.at( 1 ).first && m_value > m_transitionColor.at( 0 ).first )
                p.setBrush( QBrush( m_transitionColor.at( 1 ).second ) );
            if ( m_value <= m_transitionColor.at( 2 ).first && m_value > m_transitionColor.at( 1 ).first )
                p.setBrush( QBrush( m_transitionColor.at( 2 ).second ) );
        }
        QPainterPath path_mask;
        path_mask.addRoundRect( baseRect.topLeft().x() + m_margin.second + offset_x, ( baseRect.center().y() - m_height / 2 ) + m_margin.first + offset_y - 1, outerDiameter - m_margin.second * 2 - m_height / 6, m_height - m_margin.first * 2, m_roundpercent ); // 防止进度值超出外框
        p.setClipPath( path_mask );
        p.drawPath( path );
        break;
    }
    }
    p.restore();
}

void MyProgressBar::drawText( QPainter& p, const QRectF& baseRect, double arcLength, double outerDiameter )
{
    p.save();
    QString textToDraw = "%";
    double percent = ( m_value - m_min ) / ( m_max - m_min ) * 100.0;
    textToDraw = QString::number( percent, 'f', m_decimals ) + textToDraw;
    QPointF centerPoint = baseRect.center();
    QFont f;
    f.setFamily( "Noto Sans CJK SC Medium" );
    f.setPixelSize( m_dataPenWidth );
    p.setFont( f );
    p.setPen( QColor( m_textColor ) );
    QFontMetrics fm( f );
    double textWidth = fm.width( textToDraw );
    double textHeight = fm.ascent() + fm.descent();

    switch ( m_barStyle )
    {
    case barStyle::Round:
    case barStyle::Pie:
    {
        double R = 0.0;
        if ( m_textPosition == DataPosition::Outer )
            R = outerDiameter / 2 + m_dataPenWidth + 10;
        else
            R = outerDiameter * m_InnerDiaOuterDiaRate / 2 - m_dataPenWidth - 10;
        double x = (R)*qCos( arcLength * 3.1415926 / 180 ) + centerPoint.x();
        double y = (R)*qSin( arcLength * 3.1415926 / 180 ) + centerPoint.y();

        // 文本绘制从左上角开始，因此将进度值坐标偏移至文本中间，使得进度值能够与进度保持稳定距离
        QRectF textRect( x - textWidth / 2, y - textWidth / 2, textWidth, textHeight );
        p.drawText( textRect, Qt::AlignCenter, textToDraw );
        break;
    }
    case barStyle::Line:
    case barStyle::Battery:
    {
        double x = baseRect.topLeft().x();
        double y = baseRect.center().y() - textHeight / 2;

        // 当进度值显示在左右
        if ( m_textPosition >= 5 )
        {
            if ( m_textPosition == 5 )
                x += 0;
            else
                x += outerDiameter;
        }
        else if ( 3 == m_textPosition || m_textPosition == 4 )
        {
            if ( m_textPosition == 3 )
                y -= m_height / 2;
            else
                y += m_height / 2;
        }

        if ( m_textPosition == DataPosition::Inner || m_textPosition == DataPosition::top || m_textPosition == DataPosition::bottom )
        {
            double lenght = ( arcLength + m_startAngel ) / 360 * outerDiameter;
            x += lenght + 5;
        }
        QRectF textRect( x, y, textWidth, textHeight );
        p.drawText( textRect, Qt::AlignCenter, textToDraw );
        break;
    }
    }
    p.restore();
}

void MyProgressBar::paintEvent( QPaintEvent* )
{
    QFont f;
    f.setFamily( "Noto Sans CJK SC Medium" );
    f.setPixelSize( m_dataPenWidth );
    int rectlength = qMin( this->width(), this->height() );
    QRectF baseRect;
    if ( m_barStyle <= 1 )
        baseRect = QRectF( mapTo( this, QPoint( 0, 0 ) ), QSize( rectlength, rectlength ) );
    else
        baseRect = QRectF( mapTo( this, QPoint( 5, 0 ) ), QSize( this->width() - 5, this->height() ) );
    // 外圈直径(非圆模式下为控件长度）
    double outerDiameter = baseRect.width();

    // 当进度值显示在外部时
    if ( m_textPosition == DataPosition::Outer )
    {
        QFontMetrics fm( f );
        double textWidth = fm.width( tr( "%1% " ).arg( m_max ) ); // 计算最大进度值所占像素值
        outerDiameter -= textWidth * 2;                           // 预留进度显示位置
    }
    else if ( m_textPosition == DataPosition::Left || m_textPosition == DataPosition::Right )
    {
        QFontMetrics fm( f );
        double textWidth = fm.width( tr( "%1% " ).arg( m_max ) ); // 计算最大进度值所占像素值
        outerDiameter -= textWidth;                               // 预留进度显示位置
        if ( m_barStyle == barStyle::Battery )
            outerDiameter -= m_height / 6;
    }
    QPainter p( this );
    p.setRenderHints( QPainter::Antialiasing | QPainter::SmoothPixmapTransform );
    //    p.save();
    // 画背景矩形填充透明色
    p.fillRect( baseRect, QBrush( QColor( 0, 0, 0, 0 ) ) );
    drawbase( p, baseRect, outerDiameter );

    // 计算当前步长
    double arcStep = m_value * 360.0 / ( m_max - m_min );
    double arcLength = arcStep - m_startAngel;
    drawValue( p, baseRect, arcStep, outerDiameter );
    if ( m_textPosition != DataPosition::Hide )
        drawText( p, baseRect, arcLength, outerDiameter );
}
