#ifndef HUFFMAN_H
#define HUFFMAN_H

#include<iostream>
#include<cstring>
#include<string>
using namespace std;
const int MaxSize=512;
struct Node
{
    int weight;
    int lchild,rchild,parent;
};

class Huffman
{
    public:
        Huffman(int a[],int n);
        //打印哈夫曼树
        void print();
        //外部调用哈夫曼编码
        void compile();
        //获取编码信息
        string getCode(int i);
        Node getNode(int i);
        virtual ~Huffman();

    protected:

    private:
        //初始化哈夫曼树
        void inital();
        //选取权值最小结点
        void SelectMin(int &i1,int &i2);
        //哈夫曼树生成
        void HuffmanTree();
        //哈夫曼编码
        void compile(Node node,string s,int index);
        Node tree[MaxSize];
        //叶子结点数
        int NodeSize;
        //各点对应编码
        string code[MaxSize];
};

#endif // HUFFMAN_H
