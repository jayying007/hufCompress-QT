#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "compress.h"
#include "unpack.h"
#include <QFileDialog>
#include <QMessageBox>
#include <string>
#include <QTextCodec>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //设置标题，图标，大小，背景颜色
    ui->setupUi(this);
    setWindowTitle("来压~~快活压~~~");
    setWindowIcon(QIcon(":/image/icon.png"));
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(215,219,228));
    setPalette(pal);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::ErrorFile()
{
    QMessageBox::information(NULL, "你的操作貌似有点问题~~",
                             "输入或输出文件有误,请仔细检查~");
}
//压缩输入路径选择
void MainWindow::on_choosePath1_clicked()
{
    QString aFile=QFileDialog::getOpenFileName(this,"选择要压缩的文件","",
                                 "所有文件(*.*)");
    if (aFile.isEmpty())
       return;
    ui->ComPath->setText(aFile);
    //自动补全输出路径
    if(ui->ComToPath->text().isEmpty()){
        int l=aFile.length();
        int j=l-1;
        while(aFile[j]!='.'&&aFile[j]!='/')j--;
        QString toPath;
        if(aFile[j]=='/')toPath=aFile+".huff";
        else{
            for(int i=0;i<=j;i++)toPath[i]=aFile[i];
            toPath+="huff";
        }
        ui->ComToPath->setText(toPath);
    }
}
//压缩输出路径选择
void MainWindow::on_Savepath1_clicked()
{
    QString aFile=QFileDialog::getSaveFileName(this,"导出路径","",
                              "huff文件(*.huff)");
    if (aFile.isEmpty())
       return;
    ui->ComToPath->setText(aFile);
}
//Qstring与stirng类型转换
string tostr(QString s)
{
    return string(s.toLocal8Bit());
}
QString toqstr(string s)
{
    return QString(QString::fromLocal8Bit(s.c_str()));
}
//执行压缩
void MainWindow::on_Compress_clicked()
{
    if(ui->ComPath->text().isEmpty()
            ||ui->ComToPath->text().isEmpty()){
        ErrorFile();
        return;
    }
    press = new Compress;
    QString str = ui->ComPath->text();
    string inpath=tostr(str);
    str = ui->ComToPath->text();
    string outpath = tostr(str);
    if(outpath[outpath.length()-1]=='/'){
        ErrorFile();
        return;
    }
    if(outpath.substr(outpath.length()-5,outpath.length())!=".huff"){
        outpath+=".huff";
    }
    press->setOutName(outpath);

    if(!press->ReadFile(inpath)){
        return;
    }

    QObject::connect(press,SIGNAL(PressCurrent(int)),this,SLOT(progress(int)));
    press->start();
    QMessageBox::information(NULL, "操作正确~~",
                             "~~理论上，文件压缩完成了！");
    ui->ComPath->setText("");
    ui->ComToPath->setText("");
    ui->progress->setValue(0);
}
void MainWindow::progress(int n)
{
    ui->progress->setValue(n);
}
void MainWindow::on_choosePath2_clicked()
{
    QString aFile=QFileDialog::getOpenFileName(this,"选择要解压的文件","",
                                 "huff文件(*.huff)");
    if (aFile.isEmpty())
       return;
    ui->PackPath->setText(aFile);
    //自动补全输出路径、后缀名
    if(ui->PackToPath->text().isEmpty()){
        QString str = ui->PackPath->text();
        string path = tostr(str);
        FILE *fp;
        if((fp = fopen(path.data(),"rb"))==NULL){
            return;
        }
        string temp="";
        char ch=fgetc(fp);
        while(ch!='\n'){
            temp+=ch;
            ch=fgetc(fp);
        }
        fclose(fp);
        path = path.substr(0,path.length()-5) + temp;
        ui->PackToPath->setText(toqstr(path));
    }
}

void MainWindow::on_SavePath2_clicked()
{
    QString aFile=QFileDialog::getSaveFileName(this,"导出路径","",
                              "所有文件(*.*)");
    if (aFile.isEmpty())
       return;
    ui->PackToPath->setText(aFile);

}
//执行解压
void MainWindow::on_Unpack_clicked()
{
    if(ui->PackPath->text().isEmpty()
            ||ui->PackToPath->text().isEmpty()){
        ErrorFile();
        return;
    }
    pack = new Unpack;
    QString str = ui->PackPath->text();
    string inpath = tostr(str);
    QString out = ui->PackToPath->text();
    string outpath = tostr(out);
    pack->setOutName(outpath);

    if(!pack->ReadFile(inpath)){
        return;
    }

    QObject::connect(pack,SIGNAL(PackCurrent(int)),this,SLOT(progress(int)));
    pack->start();
    QMessageBox::information(NULL, "操作正确~~",
                             "~~理论上，文件解压完成了！");
    ui->PackPath->setText("");
    ui->PackToPath->setText("");
    ui->progress->setValue(0);
}

void MainWindow::on_Show_currentChanged(int index)
{
    if(index==1){
        ui->ComPath->setText("");
        ui->ComToPath->setText("");
    }
    else if(index==0){
        ui->PackPath->setText("");
        ui->PackToPath->setText("");
    }
}
