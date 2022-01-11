#include "progressbar.h"
#include "ui_progressbar.h"
#include <QDebug>
#include <QColorDialog>

progressbar::progressbar( QWidget* parent )
    : QWidget( parent )
    , ui( new Ui::progressbar )
    , progressbarTimer( nullptr )
    , valueStep( 1 )
{
    ui->setupUi( this );
    progressbarTimer = new QTimer( this );
    progressbarTimer->setInterval( 1000 );
    ui->lineEdit_value_step->setText( QString::number( valueStep ) );
    ui->widget_progressbar->setvalue( 0 );
    ui->lineEdit_value->setText( "0" );
    connect( progressbarTimer, &QTimer::timeout, this, &progressbar::updateValue );
    connect( ui->pushButton_start, SIGNAL( clicked() ), progressbarTimer, SLOT( start() ) );
    connect( ui->pushButton_pause, SIGNAL( clicked() ), progressbarTimer, SLOT( stop() ) );
    connect( ui->pushButton_set_value, &QPushButton::clicked, this, &progressbar::setValue );
    connect( ui->pushButton_set_value_step, &QPushButton::clicked, this, &progressbar::setValueStep );
    connect( ui->spinBox_reflash_speed, static_cast< void ( QSpinBox::* )( int ) >( &QSpinBox::valueChanged ), [=]( int value ) {
        progressbarTimer->setInterval( value );
        if ( progressbarTimer->isActive() )
            progressbarTimer->start();
    } );
}

progressbar::~progressbar()
{
    delete ui;
}

void progressbar::updateValue()
{
    bool bOk;
    double value = ui->lineEdit_value->text().toDouble( &bOk );
    if ( bOk )
    {
        if ( value >= ui->widget_progressbar->getMax() )
        {
            progressbarTimer->stop();
            ui->widget_progressbar->setvalue( 100 );
            return;
        }
        ui->widget_progressbar->setvalue( value + valueStep );
        ui->lineEdit_value->setText( QString::number( value + valueStep ) );
    }
}

void progressbar::setValue()
{
    bool bOk;
    double value = ui->lineEdit_value->text().toDouble( &bOk );
    if ( bOk )
        ui->widget_progressbar->setvalue( value );
}

void progressbar::setValueStep()
{
    bool bOk;
    double value = ui->lineEdit_value_step->text().toDouble( &bOk );
    if ( bOk )
        valueStep = value;
}

void progressbar::on_pushButton_set_range_clicked()
{
    double max = ui->lineEdit_range_max->text().toDouble();
    double min = ui->lineEdit_range_min->text().toDouble();
    ui->widget_progressbar->setRange( min, max );
}

void progressbar::on_comboBox_progressBar_style_currentIndexChanged( int index )
{
    ui->widget_progressbar->setBarStyle( barStyle( index ) );
    ui->stackedWidget_canshu->setCurrentIndex( index );
}

void progressbar::on_pushButton_set_inner_outer_rate_clicked()
{
    double rate = ui->lineEdit_inner_outer_rate->text().toDouble();
    ui->widget_progressbar->setInnerOuterDiameterRate( rate );
}

void progressbar::on_pushButton_set_start_angel_clicked()
{
    double angel = ui->lineEdit_start_angel->text().toDouble();
    ui->widget_progressbar->setStartAngel( angel );
}

void progressbar::on_spinBox_outline_width_editingFinished()
{
    ui->widget_progressbar->setOutlinePenWidth( ui->spinBox_outline_width->value() );
}

void progressbar::on_spinBox_text_size_editingFinished()
{
    ui->widget_progressbar->setDataPenWidth( ui->spinBox_text_size->value() );
}

void progressbar::on_comboBox_text_position_currentIndexChanged( int index )
{
    ui->widget_progressbar->setDataPos( DataPosition( index ) );
}

void progressbar::on_pushButton_base_color_clicked()
{
    QColorDialog dlg( this );
    QColor color;
    color = dlg.getColor( Qt::red, this, tr( "颜色对话框" ), QColorDialog::ShowAlphaChannel );
    ui->widget_progressbar->setBaseColor( color );
    QPalette palette = ui->label_base_color->palette();
    palette.setColor( ui->label_base_color->backgroundRole(), color );
    ui->label_base_color->setAutoFillBackground( true );
    ui->label_base_color->setPalette( palette );
}

void progressbar::on_pushButton_value_color_clicked()
{
    QColorDialog dlg( this );
    QColor color;
    color = dlg.getColor( Qt::red, this, tr( "颜色对话框" ), QColorDialog::ShowAlphaChannel );
    ui->widget_progressbar->setValueColor( color );
    QPalette palette = ui->label_value_color->palette();
    palette.setColor( ui->label_value_color->backgroundRole(), color );
    ui->label_value_color->setAutoFillBackground( true );
    ui->label_value_color->setPalette( palette );
}

void progressbar::on_pushButton_border_color_clicked()
{
    QColorDialog dlg( this );
    QColor color;
    color = dlg.getColor( Qt::red, this, tr( "颜色对话框" ), QColorDialog::ShowAlphaChannel );
    ui->widget_progressbar->setBorderColor( color );
    QPalette palette = ui->label_border_color->palette();
    palette.setColor( ui->label_border_color->backgroundRole(), color );
    ui->label_border_color->setAutoFillBackground( true );
    ui->label_border_color->setPalette( palette );
}

void progressbar::on_pushButton_text_color_clicked()
{
    QColorDialog dlg( this );
    QColor color;
    color = dlg.getColor();
    ui->widget_progressbar->setTextColor( color );
    QPalette palette = ui->label_text_color->palette();
    palette.setColor( ui->label_text_color->backgroundRole(), color );
    ui->label_text_color->setAutoFillBackground( true );
    ui->label_text_color->setPalette( palette );
}

void progressbar::on_comboBox_text_position_2_currentIndexChanged( int index )
{
    ui->widget_progressbar->setDecimals( index );
}

void progressbar::on_lineEdit_margin_tb_textChanged( const QString& arg1 )
{
    double tb = arg1.toDouble();
    double lr = ui->lineEdit_margin_lr->text().toDouble();
    ui->widget_progressbar->setMargin( qMakePair( tb, lr ) );
}

void progressbar::on_lineEdit_margin_lr_textChanged( const QString& arg1 )
{
    double tb = ui->lineEdit_margin_tb->text().toDouble();
    double lr = arg1.toDouble();
    ui->widget_progressbar->setMargin( qMakePair( tb, lr ) );
}

void progressbar::on_lineEdit_roundA_textChanged( const QString& arg1 )
{
    int roundA = arg1.toInt();
    ui->widget_progressbar->setRoundA( roundA );
}

void progressbar::on_lineEdit_line_height_textChanged( const QString& arg1 )
{
    double height = arg1.toDouble();
    ui->widget_progressbar->setHeight( height );
}

void progressbar::on_lineEdit_margin_tb_battery_textChanged( const QString& arg1 )
{
    double tb = arg1.toDouble();
    double lr = ui->lineEdit_margin_lr_battery->text().toDouble();
    ui->widget_progressbar->setMargin( qMakePair( tb, lr ) );
}

void progressbar::on_lineEdit_margin_lr_battery_textChanged( const QString& arg1 )
{
    double tb = ui->lineEdit_margin_tb_battery->text().toDouble();
    double lr = arg1.toDouble();
    ui->widget_progressbar->setMargin( qMakePair( tb, lr ) );
}

void progressbar::on_lineEdit_roundA_battery_textChanged( const QString& arg1 )
{
    int roundA = arg1.toInt();
    ui->widget_progressbar->setRoundA( roundA );
}

void progressbar::on_lineEdit_battery_height_textChanged( const QString& arg1 )
{
    double height = arg1.toDouble();
    ui->widget_progressbar->setHeight( height );
}

void progressbar::on_groupBox_transition_toggled( bool arg1 )
{
    ui->widget_progressbar->setTransition( arg1 );
}

void progressbar::on_lineEdit_transValue_1_textChanged( const QString& arg1 )
{
    auto colors = ui->widget_progressbar->getTransitionColor();
    QPair< int, QColor > new_color = qMakePair( arg1.toInt(), colors.at( 0 ).second );
    colors.replace( 0, new_color );
    ui->widget_progressbar->setTransitionColor( colors );
}

void progressbar::on_lineEdit_transValue_2_textChanged( const QString& arg1 )
{
    auto colors = ui->widget_progressbar->getTransitionColor();
    QPair< int, QColor > new_color = qMakePair( arg1.toInt(), colors.at( 1 ).second );
    colors.replace( 1, new_color );
    ui->widget_progressbar->setTransitionColor( colors );
}

void progressbar::on_lineEdit_transValue_3_textChanged( const QString& arg1 )
{
    auto colors = ui->widget_progressbar->getTransitionColor();
    QPair< int, QColor > new_color = qMakePair( arg1.toInt(), colors.at( 2 ).second );
    colors.replace( 2, new_color );
    ui->widget_progressbar->setTransitionColor( colors );
}

void progressbar::on_pushButton_transValue_1_clicked()
{
    QColorDialog dlg( this );
    QColor color;
    color = dlg.getColor( Qt::red, this, tr( "颜色对话框" ), QColorDialog::ShowAlphaChannel );
    auto colors = ui->widget_progressbar->getTransitionColor();
    QPair< int, QColor > new_color = qMakePair( colors.at( 0 ).first, color );
    colors.replace( 0, new_color );
    ui->widget_progressbar->setTransitionColor( colors );
    QPalette palette = ui->label_transValue_1->palette();
    palette.setColor( ui->label_transValue_1->backgroundRole(), color );
    ui->label_transValue_1->setAutoFillBackground( true );
    ui->label_transValue_1->setPalette( palette );
}

void progressbar::on_pushButton_transValue_2_clicked()
{
    QColorDialog dlg( this );
    QColor color;
    color = dlg.getColor( Qt::red, this, tr( "颜色对话框" ), QColorDialog::ShowAlphaChannel );
    auto colors = ui->widget_progressbar->getTransitionColor();
    QPair< int, QColor > new_color = qMakePair( colors.at( 1 ).first, color );
    colors.replace( 1, new_color );
    ui->widget_progressbar->setTransitionColor( colors );
    QPalette palette = ui->label_transValue_2->palette();
    palette.setColor( ui->label_transValue_2->backgroundRole(), color );
    ui->label_transValue_2->setAutoFillBackground( true );
    ui->label_transValue_2->setPalette( palette );
}

void progressbar::on_pushButton_transValue_3_clicked()
{
    QColorDialog dlg( this );
    QColor color;
    color = dlg.getColor( Qt::red, this, tr( "颜色对话框" ), QColorDialog::ShowAlphaChannel );
    auto colors = ui->widget_progressbar->getTransitionColor();
    QPair< int, QColor > new_color = qMakePair( colors.at( 2 ).first, color );
    colors.replace( 2, new_color );
    ui->widget_progressbar->setTransitionColor( colors );
    QPalette palette = ui->label_transValue_3->palette();
    palette.setColor( ui->label_transValue_3->backgroundRole(), color );
    ui->label_transValue_3->setAutoFillBackground( true );
    ui->label_transValue_3->setPalette( palette );
}
