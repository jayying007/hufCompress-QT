#include"Huffman.h"

Huffman::Huffman(int a[],int n)
{
    //ctor
    NodeSize=n;
    inital();
    for(int i=0;i<n;i++)
        tree[i].weight=a[i];
}

Huffman::~Huffman()
{
    //dtor
}
void Huffman::inital()
{
    for(int i=0;i<NodeSize;i++)
    {
        tree[i].lchild=-1;
        tree[i].rchild=-1;
        tree[i].parent=-1;
    }
    for(int i=NodeSize;i<2*NodeSize-1;i++)//即将新增结点，先初始化，且区别于已有结点
    {
        tree[i].lchild=-2;
        tree[i].rchild=-2;
        tree[i].parent=-2;
    }
}
void Huffman::SelectMin(int& i1,int& i2)
{
    int a=-1,b;//先给a,b一个结点的下标
    for(int i=0;i<2*NodeSize-1;i++)
    {
        if(tree[i].parent==-1)
        {
            if(a==-1)a=i;
            else{
                b=i;
                break;
            }
        }
    }
    //让a表示的结点权值更小.
    if(tree[a].weight>tree[b].weight)swap(a,b);

    for(int i=0;i<2*NodeSize-1;i++)
    {
        if(tree[i].parent==-1&&i!=a&&i!=b)
        {
            if(tree[i].weight<tree[a].weight)
            {
                b=a;
                a=i;
            }
            else if(tree[i].weight<tree[b].weight)
            {
                b=i;
            }
        }
    }
    i1=a;i2=b;
}
void Huffman::HuffmanTree()
{
    int i1,i2;
    for(int k=NodeSize;k<2*NodeSize-1;k++)
    {
        SelectMin(i1,i2);
        tree[k].weight=tree[i1].weight+tree[i2].weight;
        tree[k].lchild=i1;
        tree[k].rchild=i2;
        tree[i1].parent=tree[i2].parent=k;
        tree[k].parent=-1;//新增结点，更新parent
    }
}
void Huffman::print()
{
    for(int i=0;i<2*NodeSize-1;i++)
    {
        cout<<tree[i].weight<<" "<<tree[i].parent<<" "<<tree[i].lchild<<" "<<tree[i].rchild;
        cout<<endl;
    }
}
string Huffman::getCode(int i)
{
    return code[i];
}
Node Huffman::getNode(int i)
{
    return tree[i];
}
void Huffman::compile()
{
    HuffmanTree();

//    print();

    compile(tree[2*NodeSize-2],"",2*NodeSize-2);
//    for(int i=0;i<NodeSize;i++)
//        cout<<code[i];
}
void Huffman::compile(Node node,string s,int index)
{
    if(node.lchild==-1)
    {
        code[index]=s;
        return;
    }
    else
    {
        compile(tree[node.lchild],s+"0",node.lchild);
        compile(tree[node.rchild],s+"1",node.rchild);
    }
}
