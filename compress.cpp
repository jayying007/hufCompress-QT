#include "Compress.h"
#include<iostream>
#include<QMessageBox>
using namespace std;
Compress::Compress()
{
    CharNum = new int[CharSize];
    HaveName = new int[CharSize];
    HaveTimes = new int[CharSize];
    //ctor
    memset(CharNum,0,sizeof(int)*CharSize);
    memset(HaveName,0,sizeof(int)*CharSize);
    memset(HaveTimes,0,sizeof(int)*CharSize);
}
char Compress::TranToNum(string data)
{
    int sum=0,len=data.length();
    for(int i=0;i<len;i++)
        sum = sum + (data[i]-'0')*pow(2,len-i-1);
    char aim = sum;
    return aim-128;
}
void Compress::CountNum()
{
    HaveNum=0;
    for(int i=0;i<CharSize;i++)
    {
        if(CharNum[i])
        {
            HaveTimes[HaveNum]=CharNum[i];
            HaveName[HaveNum]=i-128;
            HaveNum++;
        }
    }
}
void Compress::BuildCode()
{
    Huffman tree(HaveTimes,HaveNum);
    tree.compile();
    //生成编码信息
    CodeInfo = new string[HaveNum];
    for(int i=0;i<HaveNum;i++)
        CodeInfo[i]=tree.getCode(i);
}
int Compress::ReadFile(string fileName)
{
    inputName = fileName;
    //文件指针
    FILE* fp;
    //以二进制方式打开图像
    if ( (fp=fopen(fileName.data(), "rb" ))==NULL )
    {
        QMessageBox::information(NULL, "路径错误~~",
                                 "貌似出了点问题~~找不到文件！");
        return 0;
    }
    //获取数据总长度
    fseek(fp, 0, SEEK_END);
    length=ftell(fp);
    rewind(fp);
    if(length==0){
        QMessageBox::information(NULL, "徒劳无获~~",
                                 "emmm,空文件就没必要压缩了~~！");
        fclose(fp);
        return 0;
    }
    char ch;
    while(!feof(fp))
    {
        ch = fgetc(fp);
        CharNum[ch+128]++;
    }
    //统计各字符权值
    CountNum();
    //编码
    BuildCode();
	fclose(fp);
    return 1;
}
string NumtoStr(int n)
{
    string a="";
    char num[100];
    int i=0;
    while(n)
    {
        num[i]=n%10+'0';
        n/=10;
        i++;
    }
    for(int j=i-1;j>=0;j--)a+=num[j];
    return a;
}
int Compress::binSearch(int k,int a[],int l,int d)
{
    if(l>d)return -1;
    int r=(l+d)/2;
    if(a[r]==k)return r;
    else if(a[r]>k)binSearch(k,a,l,r-1);
    else binSearch(k,a,r+1,d);
    //return -1;
}
void Compress::Write(string fileName)
{
    FILE *fp;
    if((fp=fopen(fileName.data(),"wb"))==NULL){
        QMessageBox::information(NULL, "路径错误~~",
                                 "貌似出了点问题~~输出路径或格式错误！");
    }
    //后缀名
    type="";
    int l=inputName.length();
    int k=l-1;
    while(inputName[k]!='.'&&inputName[k]!='/')k--;
    if(inputName[k]=='.'){
        type=inputName.substr(k,l);
    }
    for(int i=0;i<type.length();i++){
        fputc(type[i],fp);
    }
    fputc('\n',fp);

    string temp;
    //文件大小
    temp=NumtoStr(length);
    l=temp.length();
    for(int i=0;i<l;i++)
        fputc(temp[i],fp);
    fputc('\n',fp);
    //编码数量
    temp=NumtoStr(HaveNum);
    l=temp.length();
    for(int i=0;i<l;i++)
        fputc(temp[i],fp);
    fputc('\n',fp);
    //编码格式   ASCII值---空格---权值---换行
    for(int i=0;i<HaveNum;i++)
    {
        fputc(HaveName[i],fp);
        fputc(' ',fp);
        temp = NumtoStr(HaveTimes[i]);
        l=temp.length();
        for(int i=0;i<l;i++)
            fputc(temp[i],fp);
        fputc('\n',fp);
    }
    int one=length/50,i=0;
    if(one==0)one=1;
    string FinalCode = "";
    CurrentNum=0;
    FILE *read;
    read=fopen(inputName.data(),"rb");
    char ch;
    while(!feof(read))
    {
        i++;
        ch=fgetc(read);
        int j=binSearch(ch,HaveName,0,HaveNum-1);
        FinalCode+=CodeInfo[j];
        while(FinalCode.length()>=8)
        {
            string temp=FinalCode.substr(0,8);
            char value = TranToNum(temp);
            fputc(value,fp);
            FinalCode=FinalCode.substr(8);
        }
        if(i%one==0){
            CurrentNum=100*i/length;
            emit PressCurrent(CurrentNum);
        }
    }
    if(FinalCode.length()){
        while(FinalCode.length()%8)FinalCode+="0";
        char value = TranToNum(FinalCode);
        fputc(value,fp);
    }
    fclose(read);
    fclose(fp);
}
void Compress::setOutName(string fileName)
{
    outputName=fileName;
}
void Compress::start()
{
    Write(outputName);
    //压缩成功
    emit PressCurrent(100);
}
Compress::~Compress()
{
    //dtor
    delete CharNum;
    delete HaveName;
    delete HaveTimes;
    delete CodeInfo;
}
