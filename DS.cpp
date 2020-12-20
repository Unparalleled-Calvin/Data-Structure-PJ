#include<iostream>
namespace DS{
    //红黑树结点
    class node{
        public:
            node();
            node(const node& ne);

            bool isNIL();//是否为NIL结点
            void NIL();//将其设为NIL结点

            bool operator<(const node& ne);
            bool operator>(const node& ne);
            bool operator==(const node& ne);
            bool operator!=(const node& ne);
            
            //以下是属性
            unsigned int num;//统计下面结点的个数，不用id，用num，大小自己体现在寻找合适插入位置的过程中

            int val;

            node* father;
            node* lchild;
            node* rchild;

            int color;
        private:
            const int RED=0;
            const int BLACK=1;
    };

    //红黑树迭代器
    class RBT_iter{
        public:
            RBT_iter();
            RBT_iter(node* ne);
            RBT_iter(const RBT_iter& n_iter);//拷贝构造函数

            node operator*();
            RBT_iter& operator++();
            RBT_iter& operator++(int);
            RBT_iter& operator--();
            RBT_iter& operator--(int);
            const RBT_iter& operator=(const RBT_iter& n_iter);

            node* get_pointer();
            void write_pointer(node* ne);
        private:
            node* pointer;
    };

    //红黑树
    class RBT{
        public:
            typedef RBT_iter iterator;
            typedef const RBT_iter const_iterator;
            typedef unsigned int size_t;

            RBT();//构造函数
            ~RBT();//析构函数

            int front();
            void push_front(int elem);
            int pop_front();
            
            int back();
            void push_back(int elem);
            int pop_back();

            size_t size();
            bool empty();

            int operator[](int index);

            void swap(RBT_iter iterA, RBT_iter iterB);
            void swap(int indexa,int indexb);

            void clear();

            RBT_iter begin();
            RBT_iter end();

            RBT_iter insert(RBT_iter iter,int elem);//在某迭代器的位置插入
            
        private:
            RBT_iter begin;
            RBT_iter end;
            RBT_iter root;

            size_t size;
            
            const int RED=0;
            const int BLACK=1;

            node* polish(node* ne);
            node* leftro(node* ne);//左旋操作
            node* rightro(node* ne);//右旋操作
    };
}
//以下是node类方法的具体实现
namespace DS{
    node::node(){
        NIL();
    }

    node::node(const node& ne){//复制构造函数和属性有关，改属性一定要改这个！
        num=ne.num;
        val=ne.val;
        father=ne.father;
        lchild=ne.lchild;
        rchild=ne.rchild;
        color=ne.color;
    }

    bool node::operator<(const node& ne){//用val的值来比较
        return val<ne.val;
    }

    bool node::operator>(const node& ne){//用val的值来比较
        return val>ne.val;
    }

    bool node::operator==(const node& ne){//用val的值来比较
        return val==ne.val;
    }

    bool node::operator!=(const node& ne){//用val的值来比较
        return val!=ne.val;
    }

    void node::NIL(){
        num=0;
        val=0;
        father=lchild=rchild=nullptr;
        color=BLACK;
    }

    bool node::isNIL(){
        return color&&(lchild==rchild);//只有NIL的左右结点才相等为nullptr
    }
}

//以下是RBT_iterator类的具体实现
namespace DS{
    RBT_iter::RBT_iter(){
        pointer=nullptr;
    }

    RBT_iter::RBT_iter(node* ne){
        pointer=ne;
    }

    RBT_iter::RBT_iter(const RBT_iter& n_iter){
        pointer=n_iter.pointer;
    }

    node* RBT_iter::get_pointer(){
        return pointer;
    }

    node RBT_iter::operator*(){
        return *pointer;
    }

    RBT_iter& RBT_iter::operator++(){
        //todo
        return ;
    }

    RBT_iter& RBT_iter::operator++(int){
        //todo
        return *this;
    }

    RBT_iter& RBT_iter::operator--(){
        //todo
        return ;
    }

    RBT_iter& RBT_iter::operator--(int){
        //todo
        return *this;
    }

    const RBT_iter& RBT_iter::operator=(const RBT_iter& n_iter){
        pointer=n_iter.pointer;
        return *this;
    }

    void RBT_iter::write_pointer(node* ne){
        pointer=ne;
    }
}

//以下是RBT类的具体实现
namespace DS{
    RBT::RBT(){
        size=0;
    }

    RBT::~RBT(){
        //todo
    }

    int RBT::front(){
        if(size==0) throw "Empty container!\n";
        return (*begin).val;
    }
    
    void RBT::push_front(int elem){
        insert(begin,elem);
    }

    int RBT::back(){
        if(size==0) throw "Empty container!\n";
        return ((*end).father)->val;
    }

    void RBT::push_back(int elem){
        insert(end,elem);
    }

    RBT_iter RBT::insert(RBT_iter iter,int elem){
        node* now;
        node* lc=new node;
        node* rc=new node;
        if(size==0){//仅为根节点
            now=new node;
            now->lchild=lc;
            now->rchild=rc;
            now->val=elem;
            now->num=1;
            lc->father=rc->father=now;
            begin=root=RBT_iter(now);
            end=RBT_iter(rc);
        }
        else{//你只管插入，调整的事交给polish
            if((*iter).isNIL()){
                now=iter.get_pointer();
            }
            else{
                now=(*iter).lchild;
                while(!now->isNIL()){//一路向右
                    now=now->rchild;
                }
            }
            now->color=RED;
            now->lchild=lc;
            now->rchild=rc;
            now->val=elem;
            now->num=1;
            lc->father=rc->father=now;
            now=polish(now);
        }
        node* temp=now;
        while(temp=temp->father){//当temp->father==nullptr会停下
            (temp->num)++;
        }
        size++;
        iter.write_pointer(now);
        return iter;
    }

    node* RBT::polish(node* ne){//polish的主要任务是：左右旋转
        if(ne->father){//要调整的节点为根节点
            ne->color=BLACK;
        }
        else if((ne->father)->color==RED){//新节点的父节点为红色
            node* tempfather=ne->father;
            node* grandfather=tempfather->father;
            node* uncle;
            //有问题！！！！！！！！！！
            //我怀疑是先判父亲的左右
            if(tempfather==grandfather->lchild){//父节点在左边
                if(ne==tempfather->lchild){//新节点在左边
                    uncle=grandfather->rchild;
                    if(uncle->color==BLACK){//叔父节点为黑色
                        rightro(tempfather);//warning!此时有变动了！
                    }
                    else{//叔父节点为红色
                        tempfather->color=BLACK;
                        uncle->color=BLACK;
                        grandfather->color=RED;
                        polish(grandfather);
                    }
                }
                else{//新节点在右边
                    uncle=grandfather->lchild;
                    if(uncle->color==BLACK){//叔父节点为黑色
                        leftro(tempfather);//warning!此时有变动了！
                        rightro(ne);
                    }
                    else{//叔父节点为红色
                        tempfather->color=BLACK;
                        uncle->color=BLACK;
                        grandfather->color=RED;
                        polish(grandfather);
                    }
                }
            }
            else{
                //todo
            }
        }
        //调完树形结构以后再往上logn调num
        return ne;
    }
}