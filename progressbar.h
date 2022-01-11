#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class progressbar;
}

class progressbar : public QWidget
{
    Q_OBJECT

public:
    explicit progressbar(QWidget *parent = nullptr);
    ~progressbar();
protected slots:
    void updateValue();
    void setValue();
    void setValueStep();

private slots:
    void on_pushButton_set_range_clicked();

    void on_comboBox_progressBar_style_currentIndexChanged(int index);

    void on_pushButton_set_inner_outer_rate_clicked();

    void on_pushButton_set_start_angel_clicked();

    void on_spinBox_outline_width_editingFinished();

    void on_spinBox_text_size_editingFinished();

    void on_comboBox_text_position_currentIndexChanged(int index);

    void on_pushButton_base_color_clicked();

    void on_pushButton_value_color_clicked();

    void on_pushButton_border_color_clicked();

    void on_pushButton_text_color_clicked();

    void on_comboBox_text_position_2_currentIndexChanged(int index);

    void on_lineEdit_margin_tb_textChanged(const QString &arg1);

    void on_lineEdit_margin_lr_textChanged(const QString &arg1);

    void on_lineEdit_roundA_textChanged(const QString &arg1);

    void on_lineEdit_line_height_textChanged(const QString &arg1);

    void on_lineEdit_margin_tb_battery_textChanged(const QString &arg1);

    void on_lineEdit_margin_lr_battery_textChanged(const QString &arg1);

    void on_lineEdit_roundA_battery_textChanged(const QString &arg1);

    void on_lineEdit_battery_height_textChanged(const QString &arg1);

    void on_groupBox_transition_toggled(bool arg1);

    void on_lineEdit_transValue_1_textChanged(const QString &arg1);

    void on_lineEdit_transValue_2_textChanged(const QString &arg1);

    void on_lineEdit_transValue_3_textChanged(const QString &arg1);

    void on_pushButton_transValue_1_clicked();

    void on_pushButton_transValue_2_clicked();

    void on_pushButton_transValue_3_clicked();

private:
    Ui::progressbar *ui;
    QTimer *progressbarTimer;
    double valueStep;
};

#endif // PROGRESSBAR_H
