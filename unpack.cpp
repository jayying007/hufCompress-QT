#include "Unpack.h"
#include<iostream>
#include<QMessageBox>
using namespace std;
Unpack::Unpack()
{
    //ctor
    HaveName = new int[CharSize1];
    HaveTimes = new int[CharSize1];

    memset(HaveTimes,0,sizeof(int)*CharSize1);
    memset(HaveName,0,sizeof(int)*CharSize1);
}
int Unpack::ReadFile(string fileName)
{
    inputName = fileName;
    if((read=fopen(fileName.data(),"rb"))==NULL){
        QMessageBox::information(NULL, "路径错误~~",
                                 "貌似出了点问题~~找不到文件！");
        return 0;
    }
    char ch;
    int step=0,t=0,k=0;
    HaveNum=preLen=0;
    type="";
    while(!feof(read)&&step!=4)
    {
        if(step!=3)ch=fgetc(read);
        switch(step)
        {
            //0.读取后缀名
            case 0:
                if(ch=='\n'){
                    step++;
                }
                else{
                    type[k++]=ch;
                }
                break;
            //1.计算preLen
            case 1:
                if(ch=='\n'){
                    step++;
                }
                else{
                    preLen = preLen*10 + ch-'0';
                }
                break;
            //2.计算编码大小
            case 2:
                if(ch=='\n'){
                    step++;
                }
                else{
                    HaveNum = HaveNum*10 + ch-'0';
                }
                break;
            //3.读取编码方式
            case 3:
                if(t==HaveNum){
                    step++;
                }
                else{
                    for(int i=0;i<HaveNum;i++)
                    {
                        //代表字符
                        ch=fgetc(read);
                        HaveName[i]=ch;
                        //代表空格
                        ch = fgetc(read);
                        while((ch=fgetc(read))&&ch!='\n')
                        {
                            HaveTimes[i]=HaveTimes[i]*10+ch-'0';

                        }
                        t++;
                    }
                }
                break;
            case 4:
                break;
            default:
                break;
        }
    }
    BuildCode();
    return 1;
}
void Unpack::BuildCode()
{
    Huffman tree(HaveTimes,HaveNum);
    tree.compile();
    //生成编码信息
    CodeInfo = new string[HaveNum];
    for(int i=0;i<HaveNum;i++)
        CodeInfo[i]=tree.getCode(i);
    for(int i=0;i<2*HaveNum;i++)
        packTree[i]=tree.getNode(i);
}
//十进制转二进制
string Unpack::TranToBit(int aim)
{
    string a;
    int sum=aim;
    char temp[10];
    int i=0;
    while(sum)
    {
        temp[i++]=sum%2+'0';
        sum/=2;
    }
    for(;i<8;i++)temp[i]='0';
    temp[i]='\0';
    for(int i=7;i>=0;i--)a+=temp[i];
    return a;
}
void Unpack::Write(string fileName)
{
    FILE *fp;
    if((fp=fopen(fileName.data(),"wb"))==NULL){
        QMessageBox::information(NULL, "路径错误~~",
                                 "貌似出了点问题~~输出路径错误！");
    }
    //01二进制串
    string code="";
    int one = preLen/50;
    if(one==0)one=1;
    //源文件字符数
    int charNum=0;
    char cRead;
    while(!feof(read)){
        cRead=fgetc(read);
        code+=TranToBit(cRead+128);
        while(code.length()>20&&charNum<preLen)
        {
            int start = 2*HaveNum-2;
            while(packTree[start].lchild!=-1&&packTree[start].rchild!=-1)
            {
                if(code[0]=='0')start=packTree[start].lchild;
                else start=packTree[start].rchild;
                code=code.substr(1);
            }
            int ch=HaveName[start];
            fputc(ch,fp);
            charNum++;
            if(charNum%one==0){
                CurrentNum=100*charNum/preLen;
                emit PackCurrent(CurrentNum);
            }
        }
    }
    while(charNum<preLen)
    {
        int start = 2*HaveNum-2;
        while(packTree[start].lchild!=-1&&packTree[start].rchild!=-1)
        {
            if(code[0]=='0')start=packTree[start].lchild;
            else start=packTree[start].rchild;
            code=code.substr(1);
        }
        int ch=HaveName[start];
        fputc(ch,fp);
        charNum++;
        if(charNum%one==0){
            CurrentNum=100*charNum/preLen;
            emit PackCurrent(CurrentNum);
        }
    }
    fclose(read);
    fclose(fp);
}
void Unpack::setOutName(string fileName)
{
    outPutName=fileName;
}
void Unpack::start()
{
    Write(outPutName);
    //解压完成
    emit PackCurrent(100);
}
Unpack::~Unpack()
{
    //dtor
    delete HaveName;
    delete HaveTimes;
    delete CodeInfo;
}
