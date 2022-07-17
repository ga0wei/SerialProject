#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    interfaceInitial();

    this->mySerialPort=new QSerialPort(this);
    labelConnectStatus=new QLabel("Connetion Status:Disconnected");
    labelConnectStatus->setMinimumWidth(500);
    ui->statusbar->addWidget(labelConnectStatus);
    this->resize(1400,800);
    ui->spinBoxFontSize->setValue(ui->plainTextEdit->font().pointSize());
    ui->tabWidget->setCurrentIndex(0);
    connect(mySerialPort,SIGNAL(readyRead()),this,SLOT(serialPortReadSlot()));
}


void MainWindow::closeEvent(QCloseEvent *event)
{

    if(mySerialPort->isOpen())
    {
        QMessageBox::StandardButton ret=
                QMessageBox::question(this,tr("注意"),tr("当前串口正在使用，是否继续退出?"),
                                      QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Cancel);
        if(ret==QMessageBox::Ok){
            mySerialPort->close();
            event->accept();
        }
        else{
            event->ignore();
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,"关于串口调试助手",
                       "Powered by Qt.\nDesigned by Gaowei.\nDate:2022-7.");
}

void MainWindow::interfaceInitial()
{
    //串口
    QList<QSerialPortInfo> portInfos=QSerialPortInfo::availablePorts();
    ui->comboBoxSerialPort->clear();
    foreach(auto str,portInfos)
    {
        ui->comboBoxSerialPort->addItem(str.portName());
    }

    //波特率
    QStringList baudList;
    baudList<<"1200"<<"2400"<<"4800"<<"9600"<<"19200"<<"38400"<<"57600"<<"115200";
    ui->comboBoxBaud->clear();
    ui->comboBoxBaud->addItems(baudList);
    ui->comboBoxBaud->setCurrentIndex(3);

    //数据位
    QStringList dataBitList;
    dataBitList<<"5"<<"6"<<"7"<<"8";
    ui->comboBoxDateNum->clear();
    ui->comboBoxDateNum->addItems(dataBitList);
    ui->comboBoxDateNum->setCurrentIndex(3);

    //校验位
    QStringList checkBitList;
    checkBitList<<"NoParity"<<"EvenParity"<<"OddParity"<<"SpaceParity"<<"MarkParity";
    ui->comboBoxCheckBit->clear();
    ui->comboBoxCheckBit->addItems(checkBitList);
    ui->comboBoxCheckBit->setCurrentIndex(0);


    //停止位
    QStringList stopBitList;
    stopBitList<<"1"<<"1.5"<<"2";
    ui->comboBoxStopBit->clear();
    ui->comboBoxStopBit->addItems(stopBitList);
    ui->comboBoxStopBit->setCurrentIndex(0);
}



void MainWindow::on_pushButtonSerialOperation_clicked(bool checked)
{
    //串口没有打开，现在打开
    if(checked){
        QSerialPort::BaudRate baudRate;
        QSerialPort::DataBits databits;
        QSerialPort::StopBits stopBits;
        QSerialPort::Parity checkBits;
        //波特率
        switch (ui->comboBoxBaud->currentIndex()) {
        case 0: baudRate=QSerialPort::Baud1200;break;
        case 1: baudRate=QSerialPort::Baud2400;break;
        case 2: baudRate=QSerialPort::Baud4800;break;
        case 3: baudRate=QSerialPort::Baud9600;break;
        case 4: baudRate=QSerialPort::Baud19200;break;
        case 5: baudRate=QSerialPort::Baud38400;break;
        case 6: baudRate=QSerialPort::Baud57600;break;
        case 7: baudRate=QSerialPort::Baud115200;break;
        default:baudRate=QSerialPort::Baud9600;break;
        }
        //数据位
        switch (ui->comboBoxDateNum->currentIndex()) {
        case 0: databits=QSerialPort::Data5;break;
        case 1: databits=QSerialPort::Data6;break;
        case 2: databits=QSerialPort::Data7;break;
        case 3: databits=QSerialPort::Data8;break;
        default:databits=QSerialPort::Data8;break;
        }
        //校验位
        switch (ui->comboBoxCheckBit->currentIndex()) {
        case 0: checkBits=QSerialPort::NoParity;break;
        case 1: checkBits=QSerialPort::EvenParity;break;
        case 2: checkBits=QSerialPort::OddParity;break;
        case 3: checkBits=QSerialPort::SpaceParity;break;
        case 4: checkBits=QSerialPort::MarkParity;break;
        case 5: checkBits=QSerialPort::UnknownParity;break;
        default:checkBits=QSerialPort::NoParity;break;
        }
        //停止位
        switch (ui->comboBoxCheckBit->currentIndex()) {
        case 0: stopBits=QSerialPort::OneStop;break;
        case 1: stopBits=QSerialPort::OneAndHalfStop;break;
        case 2: stopBits=QSerialPort::TwoStop;break;
        default:stopBits=QSerialPort::OneStop;break;
        }
        //串口连接属性
        this->mySerialPort->setPortName(ui->comboBoxSerialPort->currentText());
        this->mySerialPort->setBaudRate(baudRate);
        this->mySerialPort->setDataBits(databits);
        this->mySerialPort->setStopBits(stopBits);
        this->mySerialPort->setParity(checkBits);

        if(mySerialPort->open(QIODevice::ReadWrite)==true){
            ui->pushButtonSerialOperation->setText(tr("关闭串口"));
            qDebug()<<"串口打开成功";
            labelConnectStatus->setText("Connetion Status:Connected");
            QMessageBox::information(this,"串口已经打开","串口已经打开");
            ui->pushButtonSend->setEnabled(true);
        }else{
            QMessageBox::information(this,"串口打开失败","串口打开失败");
            qDebug()<<"串口打开失败";
            ui->pushButtonSerialOperation->setChecked(false);
        }
    }
    //串口已经打开，现在关闭
    else{
        mySerialPort->close();
        ui->pushButtonSend->setEnabled(false);
        labelConnectStatus->setText("Connetion Status:Disconnected");
        ui->pushButtonSerialOperation->setText(tr("打开串口"));
        QMessageBox::information(this,"串口已经关闭","串口已经关闭");
        qDebug()<<"串口关闭成功";
    }
}



void MainWindow::serialPortReadSlot()
{
    //从缓冲区中读取数据
    QByteArray buffer = mySerialPort->readAll();
    QString showText;
    QString prefix;
    if(!buffer.isEmpty()){
        if(ui->checkBoxTime->isChecked()){
            prefix=QTime::currentTime().toString();
        }
        if(ui->checkBoxShowSendData->isChecked()){
            prefix+="<<";
        }
        if(ui->checkBoxHexReceiveShow->isChecked()){
            buffer=buffer.toHex(' ').toUpper();
        }
        if(prefix.isEmpty()){
            showText=buffer;
        }else{
            showText=prefix+" :" +buffer;
        }
        ui->plainTextEdit->appendPlainText(showText);
        qDebug()<<(showText);
        qDebug()<<"__________________________";
        qDebug()<<mySerialPort->baudRate();
        qDebug()<<mySerialPort->stopBits();
        qDebug()<<mySerialPort->parity();
        qDebug()<<mySerialPort->portName();
        qDebug()<<mySerialPort->dataBits();
        qDebug()<<"__________________________";
    }
}


void MainWindow::on_pushButton_clicked()
{
    ui->plainTextEdit->clear();
}

void MainWindow::on_pushButtonSend_clicked()
{
    QString text=ui->textEdit->toPlainText();
    if(text.isEmpty()) return;
    if(ui->checkBoxHexSendShow->isChecked())
    {//现在16
        this->textEditHex=ui->textEdit->toPlainText();
        switch (ui->comboBoxNextLine->currentIndex()) {
        case 0:this->textEdit=this->textEdit.replace("0A","0D 0A");break;
        case 1:this->textEdit=this->textEdit.replace("0A","0D");break;
        case 2:
        default:break;
        }
        QStringList list=this->textEditHex.split(' ');
        textEdit.clear();
        foreach(auto str,list){
            textEdit.append(str.toInt(nullptr,16));
        }
    }else{//现在时10
        this->textEdit=ui->textEdit->toPlainText();
        switch (ui->comboBoxNextLine->currentIndex()) {
        case 0:this->textEdit=this->textEdit.replace("\n","\r\n");break;
        case 1:this->textEdit=this->textEdit.replace('\n','\r');break;
        case 2:
        default:break;
        }
        this->textEditHex=this->textEdit.toUtf8().toHex(' ').toUpper();
    }
    qDebug()<<"16进制："<<this->textEditHex;
    qDebug()<<"utf8："<<this->textEdit;
    mySerialPort->write(textEdit.toUtf8());
    QString prefix;
    QString showText;
    if(ui->checkBoxShowSendData->isChecked()){
        prefix=">>";
    }
    else
    {
        return;
    }
    if(ui->checkBoxTime->isChecked()){
        prefix=QTime::currentTime().toString()+prefix;
    }
    if(ui->checkBoxHexReceiveShow->isChecked())
    {
        showText=textEditHex;
    }else{
        showText=textEdit;
    }
    if(!prefix.isEmpty()){
        showText=prefix+" :"+showText;
    }
    ui->plainTextEdit->appendPlainText(showText);
}

void MainWindow::on_checkBoxHexSendShow_toggled(bool checked)
{
    if(checked)
    {//现在时10进制,改16进制
        this->textEdit=ui->textEdit->toPlainText();


        switch (ui->comboBoxNextLine->currentIndex()) {
        case 0:this->textEdit=this->textEdit.replace("\n","\r\n");break;
        case 1:this->textEdit=this->textEdit.replace('\n','\r');break;

        case 2:
        default:break;
        }






        this->textEditHex=this->textEdit.toUtf8().toHex(' ').toUpper();
        ui->textEdit->setText(textEditHex);
    }else{//现在时16进制,改10进制
        this->textEditHex=ui->textEdit->toPlainText();


        switch (ui->comboBoxNextLine->currentIndex()) {

        case 0:this->textEdit=this->textEdit.replace("0A","0D 0A");break;
        case 1:this->textEdit=this->textEdit.replace("0A","0D");break;

        case 2:
        default:break;

        }

        QStringList list=this->textEditHex.split(' ');
        textEdit.clear();
        foreach(auto str,list){
            textEdit.append(str.toInt(nullptr,16));
        }
        ui->textEdit->setText(textEdit);
    }
    qDebug()<<"16进制："<<this->textEditHex;
    qDebug()<<"utf8："<<this->textEdit;
}


void MainWindow::on_actionFileSave_triggered()
{
    QTextDocument *doc=ui->plainTextEdit->document();
    if(doc->isEmpty())
    {
        QMessageBox::critical(this,tr("警告"),
                              "内容为空，无法保存");
        return;
    }
    QString fileName=QFileDialog::getSaveFileName(this,tr("保存文件"),QCoreApplication::applicationDirPath(),
                                                  tr("文本文件(*.txt)"));
    if(fileName.isEmpty())
    {
        return;
    }
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QString str=ui->plainTextEdit->toPlainText();
        QByteArray array=str.toUtf8();
        file.write(array,array.length());
        ui->tabWidget->setCurrentIndex(0);
        file.close();

        QMessageBox::information(this,tr("保存成功"),
                                 fileName);
    }
}

void MainWindow::on_spinBoxFontSize_valueChanged(int arg1)
{
    QFont font=ui->plainTextEdit->font();
    font.setPointSize(arg1);
    ui->plainTextEdit->setFont(font);
}
void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    QColor color;
    switch(index)
    {
    default:
    case 0:color=Qt::black;break;
    case 1:color=Qt::blue;break;
    case 2:color=Qt::red;break;
    case 3:color=Qt::green;break;
    }
    QPalette plt=ui->plainTextEdit->palette();
    plt.setColor(QPalette::Text,color);
    ui->plainTextEdit->setPalette(plt);
}
