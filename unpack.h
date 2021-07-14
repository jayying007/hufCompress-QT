#ifndef UNPACK_H
#define UNPACK_H

#include"Huffman.h"
#include<cstring>
#include<cstdio>
#include<cstdlib>
#include<QObject>
//ASCII有256位
const int CharSize1=256;
class Unpack:public QObject
{
    Q_OBJECT
    public:
        Unpack();
        //读文件
        int ReadFile(string fileName);
        //执行解压
        void start();
        void setOutName(string fileName);
        virtual ~Unpack();

    protected:

    private:
        //输入文件指针
        FILE *read;
        //哈夫曼树结构
        Node packTree[2*CharSize1];
        //源文件大小
        int preLen;
        //出现的字符及个数
        int HaveNum;
        int *HaveName;
        int *HaveTimes;
        //当前进度
        int CurrentNum;
        //文件类型，输入，输出文件名
        string type;
        string inputName;
        string outPutName;
        //字符编码信息
        string *CodeInfo;

        //哈夫曼编码
        void BuildCode();
        //数据转为01编码
        void Write(string fileName);
        string TranToBit(int aim);
    signals:
        void PackCurrent(int);
};

#endif // UNPACK_H
