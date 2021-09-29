#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "onvifmodels/models.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void initConnect();
private slots:
    void slot_pushButton_connectCamera_clicked();

    void slot_pushButton_up_pressed();

    void slot_pushButton_down_pressed();

    void slot_pushButton_reset_pressed();

    void slot_pushButton_left_pressed();

    void slot_pushButton_right_pressed();

    void slot_pushButton_zoomIn_pressed();

    void slot_pushButton_zoomOut_pressed();

    void slot_pushButton_upleft_pressed();

    void slot_pushButton_upright_pressed();

    void slot_pushButton_downLeft_pressed();

    void slot_pushButton_downRight_pressed();

    void slot_PTZ_control_button_released();

    void on_pushButton_createPreset_clicked();

    void on_pushButton_gotoPreset_clicked();

    void on_pushButton_getPreset_clicked();

private:
    Ui::MainWindow *ui;
    QString m_userName{""};
    QString m_passWord{""};
    QString m_PTZAddress{""};
    MediaProfile m_selectedMedia;
};

#endif // MAINWINDOW_H
