#ifndef COMPRESS_H
#define COMPRESS_H

#include"Huffman.h"
#include<cstring>
#include<string>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<QObject>
//ASCII有256位
const int CharSize=256;
class Compress:public QObject
{
    Q_OBJECT
    public:
        Compress();
        //读文件
        int ReadFile(string fileName);
        //执行压缩
        void start();
        //设置输出文件名
        void setOutName(string fileName);
        virtual ~Compress();
    protected:
    private:
        //文件类型
        string type;
        //输入文件名
        string inputName;
        //输出文件名
        string outputName;
        //每种字符个数
        int *CharNum;
        //出现的字符及个数
        int HaveNum;
        int *HaveName;
        int *HaveTimes;
        //字符编码信息
        string *CodeInfo;
        //数据长度
        int length;
        //当前压缩进度
        int CurrentNum;

        //统计各种字符数
        void CountNum();
        //二进制转十进制
        char TranToNum(string data);
        //哈夫曼编码
        void BuildCode();
        //写入压缩文件
        void Write(string fileName);
        //二分查找法
        int binSearch(int k,int a[],int l,int d);
    signals:
        void PressCurrent(int);
};

#endif // COMPRESS_H
