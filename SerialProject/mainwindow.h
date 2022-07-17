#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QFileDialog>
#include<QDebug>
#include <QMainWindow>
#include<QMessageBox>
#include<QTime>
#include <QSerialPort>
#include<QLabel>
#include <QSerialPortInfo>
#include <QSpinBox>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void on_actionAboutQt_triggered();
    void on_actionAbout_triggered();
    void serialPortReadSlot();
    void on_pushButtonSerialOperation_clicked(bool checked);
    void on_pushButton_clicked();
    void on_pushButtonSend_clicked();
    void on_checkBoxHexSendShow_toggled(bool checked);
    void on_actionFileSave_triggered();
    void on_spinBoxFontSize_valueChanged(int arg1);
    void on_comboBox_currentIndexChanged(int index);
private:
    QSpinBox *spinBoxFontSize;
    QLabel *labelFontSize;
    QString textEditHex;
    QString textEdit;
    QLabel *labelConnectStatus;
    void interfaceInitial();
    QSerialPort *mySerialPort;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
