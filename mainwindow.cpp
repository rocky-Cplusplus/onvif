#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>
#include <QTimer>
#include <QMessageBox>
#include <deviceonvif/onvifdeviceservice.h>
#include <mediaonvif/onvifmediaservice.h>
#include <ptzonvif/onvifptzservice.h>

static const double motionless = 0.0;
static const double horMotionOneStep = 0.4;
static const double verMotionOneStep = 0.4;
static const double zoomMotionOneStep = 0.3;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initConnect();
#if 0
    OnvifDeviceService a;
    DeviceCapabilities deviceCapabilities = a.GetCapabilities("http://192.168.3.70:2000/onvif/device_service", "admin", "admin");

    OnvifMediaService b;
    QList<MediaProfile> listProfile = b.GetProfiles(deviceCapabilities.mediaXAddr, "admin", "admin");

    if(listProfile.isEmpty())
    {
        return;
    }
    OnvifPTZService c;
    c.ContinuousMove(deviceCapabilities.ptzXAddr, "admin", "admin", listProfile[0].token, 0.2, 0, 0);
    QThread::msleep(2000);
    c.Stop(deviceCapabilities.ptzXAddr, "admin", "admin", listProfile[0].token);
    QThread::msleep(500);
    c.GoToHomePosition(deviceCapabilities.ptzXAddr, "admin", "admin", listProfile[0].token);
#endif

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initConnect()
{
    connect(ui->pushButton_connectCamera,&QPushButton::clicked,this,&MainWindow::slot_pushButton_connectCamera_clicked);
    //八个方向控制
    connect(ui->pushButton_left,&QPushButton::pressed,this,&MainWindow::slot_pushButton_left_pressed);
    connect(ui->pushButton_upleft,&QPushButton::pressed,this,&MainWindow::slot_pushButton_upleft_pressed);
    connect(ui->pushButton_up,&QPushButton::pressed,this,&MainWindow::slot_pushButton_up_pressed);
    connect(ui->pushButton_upright,&QPushButton::pressed,this,&MainWindow::slot_pushButton_upright_pressed);
    connect(ui->pushButton_right,&QPushButton::pressed,this,&MainWindow::slot_pushButton_right_pressed);
    connect(ui->pushButton_downRight,&QPushButton::pressed,this,&MainWindow::slot_pushButton_downRight_pressed);
    connect(ui->pushButton_down,&QPushButton::pressed,this,&MainWindow::slot_pushButton_down_pressed);
    connect(ui->pushButton_downleft,&QPushButton::pressed,this,&MainWindow::slot_pushButton_downLeft_pressed);

     connect(ui->pushButton_reset,&QPushButton::pressed,this,&MainWindow::slot_pushButton_reset_pressed);

    connect(ui->pushButton_left,&QPushButton::released,this,&MainWindow::slot_PTZ_control_button_released);
    connect(ui->pushButton_upleft,&QPushButton::released,this,&MainWindow::slot_PTZ_control_button_released);
    connect(ui->pushButton_up,&QPushButton::released,this,&MainWindow::slot_PTZ_control_button_released);
    connect(ui->pushButton_upright,&QPushButton::released,this,&MainWindow::slot_PTZ_control_button_released);
    connect(ui->pushButton_right,&QPushButton::released,this,&MainWindow::slot_PTZ_control_button_released);
    connect(ui->pushButton_downRight,&QPushButton::released,this,&MainWindow::slot_PTZ_control_button_released);
    connect(ui->pushButton_down,&QPushButton::released,this,&MainWindow::slot_PTZ_control_button_released);
    connect(ui->pushButton_downleft,&QPushButton::released,this,&MainWindow::slot_PTZ_control_button_released);

    //焦距控制
    connect(ui->pushButton_zoomIn,&QPushButton::pressed,this,&MainWindow::slot_pushButton_zoomIn_pressed);
    connect(ui->pushButton_zoomOut,&QPushButton::pressed,this,&MainWindow::slot_pushButton_zoomOut_pressed);

    connect(ui->pushButton_zoomIn,&QPushButton::released,this,&MainWindow::slot_PTZ_control_button_released);
    connect(ui->pushButton_zoomOut,&QPushButton::released,this,&MainWindow::slot_PTZ_control_button_released);


}

void MainWindow::slot_pushButton_connectCamera_clicked()
{
    auto ip = ui->lineEdit_IP->text();
    auto port = ui->lineEdit_Port->text();

     m_userName = ui->lineEdit_userName->text();
     m_passWord = ui->lineEdit_PassWord->text();

    OnvifDeviceService a;
    DeviceCapabilities deviceCapabilities = a.GetCapabilities(tr("http://%1:%2/onvif/device_service").arg(ip).arg(port), m_userName, m_passWord);

    OnvifMediaService b;
    QList<MediaProfile> listProfile = b.GetProfiles(deviceCapabilities.mediaXAddr,  m_userName, m_passWord);

    if(listProfile.isEmpty())
    {
        QMessageBox::warning(nullptr,"error","connect failed");
        return;
    }
    m_PTZAddress = deviceCapabilities.ptzXAddr;
    m_selectedMedia = listProfile[0];

     QMessageBox::information(nullptr,"info","connect success");
}

void MainWindow::slot_pushButton_up_pressed()
{
    OnvifPTZService c;
    c.ContinuousMove(m_PTZAddress, m_userName, m_passWord, m_selectedMedia.token, motionless, verMotionOneStep, motionless);
}

void MainWindow::slot_PTZ_control_button_released()
{
    OnvifPTZService c;
    QThread::msleep(200);
    c.Stop(m_PTZAddress, m_userName, m_passWord, m_selectedMedia.token);
}

void MainWindow::slot_pushButton_down_pressed()
{
    OnvifPTZService c;

    c.ContinuousMove(m_PTZAddress, m_userName, m_passWord, m_selectedMedia.token, motionless, -verMotionOneStep, motionless);
}

void MainWindow::slot_pushButton_reset_pressed()
{
    OnvifPTZService c;
    c.GoToHomePosition(m_PTZAddress, m_userName, m_passWord, m_selectedMedia.token);
}

void MainWindow::slot_pushButton_left_pressed()
{
    OnvifPTZService c;
    c.ContinuousMove(m_PTZAddress, m_userName, m_passWord, m_selectedMedia.token, horMotionOneStep, motionless, motionless);
}

void MainWindow::slot_pushButton_right_pressed()
{
    OnvifPTZService c;
    c.ContinuousMove(m_PTZAddress, m_userName, m_passWord, m_selectedMedia.token, -horMotionOneStep, motionless, motionless);

}

void MainWindow::slot_pushButton_zoomIn_pressed()
{
    OnvifPTZService c;
    c.ContinuousMove(m_PTZAddress, m_userName, m_passWord, m_selectedMedia.token, motionless, motionless, zoomMotionOneStep);

}

void MainWindow::slot_pushButton_zoomOut_pressed()
{
    OnvifPTZService c;
    c.ContinuousMove(m_PTZAddress, m_userName, m_passWord, m_selectedMedia.token, motionless, motionless, -zoomMotionOneStep);

}

void MainWindow::slot_pushButton_upleft_pressed()
{
    OnvifPTZService c;
    c.ContinuousMove(m_PTZAddress, m_userName, m_passWord, m_selectedMedia.token, horMotionOneStep, verMotionOneStep, motionless);

}

void MainWindow::slot_pushButton_upright_pressed()
{
    OnvifPTZService c;
    c.ContinuousMove(m_PTZAddress, m_userName, m_passWord, m_selectedMedia.token, -horMotionOneStep, verMotionOneStep, motionless);
}

void MainWindow::slot_pushButton_downLeft_pressed()
{
    OnvifPTZService c;
    c.ContinuousMove(m_PTZAddress, m_userName, m_passWord, m_selectedMedia.token, horMotionOneStep, -verMotionOneStep, motionless);
}

void MainWindow::slot_pushButton_downRight_pressed()
{
    OnvifPTZService c;
    c.ContinuousMove(m_PTZAddress, m_userName, m_passWord, m_selectedMedia.token, -horMotionOneStep, -verMotionOneStep, motionless);
}

void MainWindow::on_pushButton_createPreset_clicked()
{
    OnvifPTZService c;
    c.setPreset(m_PTZAddress, m_userName, m_passWord, m_selectedMedia.token);
}

void MainWindow::on_pushButton_gotoPreset_clicked()
{
    OnvifPTZService c;
    c.gotoPreset(m_PTZAddress, m_userName, m_passWord, m_selectedMedia.token);
}

void MainWindow::on_pushButton_getPreset_clicked()
{
    OnvifPTZService c;
    c.getPreset(m_PTZAddress, m_userName, m_passWord, m_selectedMedia.token);
}
