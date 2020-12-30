#include<iostream>
#include<ctime>
#include<cstdlib>
#include<vector>
#include<queue>
#include<set>
#define RBT LinearTable
#define p(i) std::cout<<i<<std::endl;
#define _p(i,j) std::cout<<i<<" "<<j<<std::endl;
#define __p(i,j,k) std::cout<<i<<" "<<j<<" "<<k<<std::endl;
namespace DS{

    //红黑树结点
    class node{
        public:
            node();
            node(const node& ne);

            bool isNIL();//是否为NIL结点
            void NIL();//将其设为NIL结点

            //以下是属性
            unsigned num=0;//统计子树结点个数
            int val;
            node* father=nullptr;
            node* lchild=nullptr;
            node* rchild=nullptr;
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

            int& operator*();
            RBT_iter operator++();
            RBT_iter operator++(int);
            RBT_iter operator--();
            RBT_iter operator--(int);
            const RBT_iter& operator=(const RBT_iter& n_iter);
            bool operator==(const RBT_iter& n_iter);

            node* get_pointer();
            const node* get_pointer() const;
            void write_pointer(node* ne);

            friend void swap(RBT_iter iterA, RBT_iter iterB);
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
            void push_front(int num,int elem);
            void pop_front();
            void pop_front(int pop_size);
            
            int back();
            void push_back(int elem);
            void push_back(int num,int elem);
            void pop_back();
            void pop_back(int pop_size);

            unsigned int size();
            bool empty();

            int& operator[](unsigned index);

            void swap(int indexA,int indexB);
            void swap(RBT& nrbt);
            void refresh(RBT_iter& nroot,RBT_iter& nbegin,RBT_iter& nend,unsigned int& nsize);

            void clear();

            RBT_iter begin();
            RBT_iter end();
            RBT_iter root();
            RBT_iter insert(RBT_iter iter,int elem);//在某迭代器的位置插入
            RBT_iter erase(RBT_iter iter);//删除迭代器指向的元素

        private:
            RBT_iter _root;
            RBT_iter _begin;
            RBT_iter _end;

            size_t _size=0U;
            
            const int RED=0;
            const int BLACK=1;

            void ipolish(node* ne,node* avoid=nullptr);//insert_polish
            void epolish(node* ne);//erase_polish
            void leftro(node* ne,node* avoid=nullptr);//左旋操作
            void rightro(node* ne,node* avoid=nullptr);//右旋操作
            void countnum(node* ne,node* avoid=nullptr);
            void destroynode(node* ne,node* father=nullptr);
    };
    
    void swap(RBT_iter& iterA, RBT_iter& iterB);
}

//以下是node类的具体实现
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

    void node::NIL(){
        val=0;
        lchild=rchild=nullptr;
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

    const node* RBT_iter::get_pointer() const {
        return pointer;
    }

    int& RBT_iter::operator*(){
        return pointer->val;
    }

    RBT_iter RBT_iter::operator++(){
        if(pointer->rchild->isNIL()){//如果右孩子是空结点，那么就上去
            while(pointer->father->rchild==pointer){
                pointer=pointer->father;
            }
            pointer=pointer->father;
        }
        else{
            pointer=pointer->rchild;
            while(!pointer->lchild->isNIL()){//去右子树的左下角
                pointer=pointer->lchild;
            }
        }
        return *this;
    }

    RBT_iter RBT_iter::operator++(int){//后置
        node* ne=pointer;
        ++(*this);
        return RBT_iter(ne);
    }

    RBT_iter RBT_iter::operator--(){
        if(pointer->isNIL()||pointer->lchild->isNIL()){//如果左孩子是空结点，那么就上去
            while(pointer->father->lchild==pointer){//如果是父节点的左孩子，那就一直往上跳
                pointer=pointer->father;
            }
            pointer=pointer->father;
        }
        else{
            pointer=pointer->lchild;
            while(!pointer->rchild->isNIL()){//左子树非空去左子树右下角
                pointer=pointer->rchild;
            }
        }
        return *this;
    }

    RBT_iter RBT_iter::operator--(int){
        node* ne=pointer;
        --(*this);
        return RBT_iter(ne);
    }

    const RBT_iter& RBT_iter::operator=(const RBT_iter& n_iter){
        pointer=n_iter.pointer;
        return *this;
    }

    void RBT_iter::write_pointer(node* ne){
        pointer=ne;
    }

    bool RBT_iter::operator==(const RBT_iter& n_iter){
        return this->pointer==n_iter.get_pointer();
    }
}

//以下是RBT类的具体实现
namespace DS{
    RBT::RBT(){
        _size=0U;
        _end=_begin=_root=RBT_iter(new node);
    }

    RBT::~RBT(){
        if(_size)
            destroynode(_root.get_pointer());
        delete _root.get_pointer();
    }

    RBT_iter RBT::begin(){
        return _begin;
    }

    RBT_iter RBT::end(){
        return _end;
    }

    RBT_iter RBT::root(){
        return _root;
    }

    int RBT::front(){
        if(_size==0U) std::cerr<<"Empty container!"<<std::endl;
        return _begin.get_pointer()->val;
    }
    
    void RBT::push_front(int elem){
        insert(_begin,elem);
    }

    void RBT::push_front(int num,int elem){
        for(int i=0;i<num;i++)
            this->push_front(elem);
    }

    void RBT::pop_front(){
        if(_size)
            erase(_begin==_root?_begin:_begin++);
    }

    void RBT::pop_front(int pop_size){
        while(_size&&pop_size--){
            this->pop_front();
        }
    }

    int RBT::back(){
        if(_size==0) std::cerr<<"Empty container!"<<std::endl;
        return _end.get_pointer()->father->val;
    }

    void RBT::push_back(int elem){
        insert(_end,elem);
    }

    void RBT::push_back(int num,int elem){
        for(int i=0;i<num;i++)
            this->push_back(elem);
    }

    void RBT::pop_back(){
        if(_size)
            erase(--_end);
    }

    void RBT::pop_back(int pop_size){
        while(_size&&pop_size--){
            this->pop_back();
        }
    }

    RBT_iter RBT::insert(RBT_iter iter,int elem){
        node* now;
        node* lc=new node;
        node* rc=new node;
        if(_size==0U){//仅为根节点
            now=_root.get_pointer();
            now->lchild=lc;
            now->rchild=rc;
            now->val=elem;
            lc->father=rc->father=now;
            _begin=_root=RBT_iter(now);
            _end=RBT_iter(rc);
        }
        else{//你只管插入，调整的事交给polish
            // p(iter.get_pointer()->father->val);
            if(iter.get_pointer()->isNIL()){
                now=iter.get_pointer();
            }
            else{
                now=iter.get_pointer()->lchild;
                while(!now->isNIL()){//一路向右
                    now=now->rchild;
                }
            }
            if(iter==_begin){
                _begin.write_pointer(now);
            }
            if(iter==_end){
                _end.write_pointer(rc);
            }
            now->color=RED;
            now->lchild=lc;
            now->rchild=rc;
            now->val=elem;
            lc->father=rc->father=now;
            ipolish(now,now);
        }
        node* temp=now;
        do{//当temp->father==nullptr会停下
            (temp->num)++;
        }while(temp=temp->father);//调完结构之后再调数目
        _size++;
        iter.write_pointer(now);
        return iter;
    }

    RBT_iter RBT::erase(RBT_iter iter){
        if(_size==0U) return _begin;
        node* ne=iter.get_pointer();
        if(ne->lchild->isNIL()&&ne->rchild->isNIL()){//左右结点都为NIL
            if(ne->color==RED){//该结点若为红色，直接删除
                delete ne->lchild;
                delete ne->rchild;
                ne->NIL();
            }
            else{//该结点为黑色
                delete ne->lchild;
                delete ne->rchild;
                ne->NIL();
                epolish(ne);
            }
            node* temp=ne;
            do{
                temp->num--;
            }while(temp=temp->father);//erase结点时先不动num，左右旋转照常计数，最后一起自底向上更新一遍。
            _size--;
            return RBT_iter(ne->father);
        }
        else if((!ne->lchild->isNIL())&&(!ne->rchild->isNIL())){//两子女均不为空结点
            RBT_iter n_iter=iter;
            ++n_iter;
            node* _ne=n_iter.get_pointer();
            std::swap(ne->val,_ne->val);//交换该结点与下一结点，重新调用erase函数
            erase(n_iter);//
            return iter;//还指向它，与它无关
        }
        else{//有一个子女非空这个子女必为红色
            node* temp;
            temp=ne->lchild->isNIL()?ne->rchild:ne->lchild;
            ne->val=temp->val;
            if(_end.get_pointer()==ne) _end.write_pointer(ne->rchild);//这个是为了应对pop_back先将end--的情况
            else if(_end.get_pointer()==ne->rchild->rchild) _end.write_pointer(ne->rchild);//正常删除涉及_end的情况
            if(_begin.get_pointer()->father==ne) _begin.write_pointer(ne);//这个是为了应对pop_front先将front--的情况
            else if(_begin.get_pointer()==ne->lchild) _begin.write_pointer(ne);//正常涉及_front的情况
            delete temp->lchild;
            delete temp->rchild;
            temp->NIL();
            do{
                temp->num--;
            }while(temp=temp->father);//顺序不对，在delete之前就应该处理好
            _size--;
            return iter;
        }
    }

    void RBT::ipolish(node* ne,node* avoid){//用于insert的结构调整
        if(ne->father==nullptr){//要调整的节点为根节点
            ne->color=BLACK;
        }
        else if((ne->father)->color==RED){//新节点的父节点为红色
            node* tempfather=ne->father;
            node* grandfather=tempfather->father;
            node* uncle;
            if(tempfather==grandfather->lchild){//父节点在左边
                if(ne==tempfather->lchild){//新节点在左边
                    uncle=grandfather->rchild;
                    if(uncle->color==BLACK){//叔父节点为黑色
                        tempfather->color=BLACK;
                        grandfather->color=RED;
                        rightro(grandfather,avoid);
                    }
                    else{//叔父节点为红色
                        tempfather->color=BLACK;
                        uncle->color=BLACK;
                        grandfather->color=RED;
                        ipolish(grandfather,avoid);
                    }
                }
                else{//新节点在右边
                    uncle=grandfather->rchild;
                    if(uncle->color==BLACK){//叔父节点为黑色
                        leftro(tempfather,avoid);
                        ne->color=BLACK;
                        grandfather->color=RED;
                        rightro(grandfather,avoid);
                    }
                    else{//叔父节点为红色
                        tempfather->color=BLACK;
                        uncle->color=BLACK;
                        grandfather->color=RED;
                        ipolish(grandfather,avoid);
                    }
                }
            }
            else{//父节点在右边
                if(ne==tempfather->rchild){//新节点在右边
                    uncle=grandfather->lchild;
                    if(uncle->color==BLACK){//叔父节点为黑色
                        tempfather->color=BLACK;
                        grandfather->color=RED;
                        leftro(grandfather,avoid);
                    }else{//叔父节点为红色
                        tempfather->color=BLACK;
                        uncle->color=BLACK;
                        grandfather->color=RED;
                        ipolish(grandfather,avoid);
                    }
                }
                else{//新节点在左边
                    uncle=grandfather->lchild;
                    if(uncle->color==BLACK){//叔父节点为黑色
                        rightro(tempfather,avoid);
                        ne->color=BLACK;
                        grandfather->color=RED;
                        leftro(grandfather,avoid);
                    }
                    else{//叔父节点为红色
                        tempfather->color=BLACK;
                        uncle->color=BLACK;
                        grandfather->color=RED;
                        ipolish(grandfather,avoid);
                    }
                }
            }
        }
    }

    void RBT::epolish(node* ne){//用于erase的结构调整
        if(ne->father==nullptr){//情况一，n是新的根
            ne->color=BLACK;
        }
        else if(ne->father->lchild==ne){//该结点是左孩子
            node* brother=ne->father->rchild;
            if(ne->father->color==RED&&brother->lchild->color==BLACK&&brother->rchild->color==BLACK){//情况二，父节点为红，兄弟结点两孩子为黑
                ne->father->color=BLACK;
                brother->color=RED;
            }
            else if(ne->father->color==BLACK&&brother->color==BLACK&&brother->lchild->color==BLACK&&brother->rchild->color==BLACK){//情况六，需要递归删除
                brother->color=RED;
                epolish(ne->father);
            }
            else if(brother->color==BLACK){//兄弟结点为黑色，此时必定下面有一红
                if(brother->lchild->color==RED){//情况四，兄弟结点左孩子为红色
                    brother->color=RED;
                    brother->lchild->color=BLACK;
                    rightro(brother);
                    brother=brother->father;
                }
                brother->rchild->color=BLACK;//情况三，兄弟结点右孩子为红
                std::swap(brother->color,brother->father->color);
                leftro(brother->father);//不知道该不该是nullptr再看
            }
            else{//情况五，兄弟结点为红
                brother->color=BLACK;
                brother->father->color=RED;
                leftro(brother->father);//操作使ne有一个红爸爸和一个黑兄弟，可以对其重新调整
                epolish(ne);
            }            
        }
        else{//该结点是右孩子
            node* brother=ne->father->lchild;
            if(ne->father->color==RED&&brother->lchild->color==BLACK&&brother->rchild->color==BLACK){//情况二，父节点为红，兄弟结点两孩子为黑
                ne->father->color=BLACK;
                brother->color=RED;
            }
            else if(ne->father->color==BLACK&&brother->color==BLACK&&brother->lchild->color==BLACK&&brother->rchild->color==BLACK){//情况六，需要递归删除
                brother->color=RED;
                epolish(ne->father);
            }
            else if(brother->color==BLACK){//兄弟结点为黑色，此时必定下面有一红
                if(brother->rchild->color==RED){//情况四，兄弟结点左孩子为红色
                    brother->color=RED;
                    brother->rchild->color=BLACK;
                    leftro(brother);
                    brother=brother->father;
                }
                brother->lchild->color=BLACK;//情况三，兄弟结点右孩子为红
                std::swap(brother->color,brother->father->color);
                rightro(brother->father);//不知道该不该是nullptr再看
            }
            else{//情况五，兄弟结点为红
                brother->color=BLACK;
                brother->father->color=RED;
                rightro(brother->father);//操作使ne有一个红爸爸和一个黑兄弟，可以对其重新调整
                epolish(ne);
            }
        }
    }

    void RBT::leftro(node* ne,node* avoid){//左旋
        node* rc=ne->rchild;
        node* tempfather=ne->father;
        if(tempfather)//排除根节点的情况
            if(ne==tempfather->lchild)
                tempfather->lchild=rc;
            else
                tempfather->rchild=rc;
        else
            _root.write_pointer(rc);
        rc->father=tempfather;
        node* rclc=rc->lchild;
        rc->lchild=ne;
        ne->father=rc;
        ne->rchild=rclc;
        rclc->father=ne;
        countnum(ne,avoid);
        countnum(rc,avoid);
    }

    void RBT::rightro(node* ne,node* avoid){//右旋
        node* lc=ne->lchild;
        node* tempfather=ne->father;
        if(tempfather)//排除根节点的情况
            if(ne==tempfather->lchild)
                tempfather->lchild=lc;
            else
                tempfather->rchild=lc;
        else
            _root.write_pointer(lc);
        lc->father=tempfather;
        node* lcrc=lc->rchild;
        lc->rchild=ne;
        ne->father=lc;
        ne->lchild=lcrc;
        lcrc->father=ne;
        countnum(ne,avoid);
        countnum(lc,avoid);
    }

    void RBT::countnum(node* ne,node* avoid){
        if(!ne->isNIL())
            ne->num=ne->lchild->num+ne->rchild->num+(ne!=avoid);
    }

    void RBT::destroynode(node* ne,node* father){//递归删除一整颗子树
        if(ne->lchild) destroynode(ne->lchild);
        if(ne->rchild) destroynode(ne->rchild);
        if(ne->father!=father) delete ne;//否则
        else{
            _size-=ne->num;
            ne->NIL();
            if(father==nullptr){
                _begin.write_pointer(ne);
                _end.write_pointer(ne);
            }
        }
    }

    int& RBT::operator[](unsigned index){//随机访问
        index++;
        unsigned temp=0;
        node* ne=_root.get_pointer();
        if(_size&&index<=ne->num&&index>=1)
            while(true){
                if(index>temp+ne->lchild->num){
                    if(index==temp+ne->lchild->num+1) return ne->val;
                    else{
                        temp+=1+ne->lchild->num;
                        ne=ne->rchild;
                    }
                }
                else{
                    ne=ne->lchild;
                }
            }
        std::cerr<<"domain error\n";
        exit(1);
    }

    void RBT::clear(){
        if(_size)
            destroynode(_root.get_pointer());
        _size=0;
    }

    void RBT::swap(int indexA,int indexB){
        if(indexA>=0&&indexA<_size&&indexB>=0&&indexB<_size)
            std::swap((*this)[indexA],(*this)[indexB]);
    }

    void RBT::swap(RBT& nrbt){
        nrbt.refresh(_root,_begin,_end,_size);
    }

    void RBT::refresh(RBT_iter& nroot,RBT_iter& nbegin,RBT_iter& nend,unsigned int& nsize){
        DS::swap(nroot,_root);
        DS::swap(nbegin,_begin);
        DS::swap(nend,_end);
        std::swap(nsize,_size);
    }

    unsigned int RBT::size(){
        return _size;
    }

    bool RBT::empty(){
        return !_size;
    }
}

//以下是友元函数的具体实现
namespace DS{
    void swap(RBT_iter& iterA, RBT_iter& iterB){
        node* temp=iterA.get_pointer();
        iterA.write_pointer(iterB.get_pointer());
        iterB.write_pointer(temp);
    }
}