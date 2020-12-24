#include<iostream>
#include<ctime>
#include<cstdlib>
#include<queue>
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

            bool operator<(const node& ne);
            bool operator>(const node& ne);
            bool operator==(const node& ne);
            bool operator!=(const node& ne);
            const node& operator=(const node& ne);

            //以下是属性
            unsigned num=0;//统计下面结点的个数，不用id，用num，大小自己体现在寻找合适插入位置的过程中
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

            node operator*();
            RBT_iter operator++();
            RBT_iter operator++(int);
            RBT_iter operator--();
            RBT_iter operator--(int);
            const RBT_iter& operator=(const RBT_iter& n_iter);
            bool operator==(const RBT_iter& n_iter);

            node* get_pointer();
            const node* get_pointer() const;
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

            int& operator[](unsigned index);

            void swap(RBT_iter iterA, RBT_iter iterB);
            void swap(int indexa,int indexb);

            void clear();

            RBT_iter begin();
            RBT_iter end();
            RBT_iter insert(RBT_iter iter,int elem);//在某迭代器的位置插入
            RBT_iter erase(RBT_iter iter);//删除迭代器指向的元素
            RBT_iter root;

            
        private:
            RBT_iter _begin;
            RBT_iter _end;

            size_t _size=0U;
            
            const int RED=0;
            const int BLACK=1;

            void ipolish(node* ne,node* avoid);//insert_polish
            void epolish(node* ne,node* avoid);//erase_polish
            void leftro(node* ne,node* avoid);//左旋操作
            void rightro(node* ne,node* avoid);//右旋操作
            void countnum(node* ne,node* avoid);

            void destroynode(node* ne);
    };
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

    const node& node::operator=(const node& ne){
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

    node RBT_iter::operator*(){
        return *pointer;
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
    }

    RBT::~RBT(){
        destroynode(root.get_pointer());
    }

    RBT_iter RBT::begin(){
        return _begin;
    }

    RBT_iter RBT::end(){
        return _end;
    }

    int RBT::front(){
        if(_size==0U) throw "Empty container!\n";
        return (*_begin).val;
    }
    
    void RBT::push_front(int elem){
        _begin=insert(_begin,elem);
    }

    int RBT::pop_front(){
        int ret=this->front();
        erase(_begin++);
        return ret;
    }

    int RBT::back(){
        if(_size==0) throw "Empty container!\n";
        return ((*_end).father)->val;
    }

    void RBT::push_back(int elem){
        _end.write_pointer(insert(_end,elem).get_pointer()->rchild);
    }

    int RBT::pop_back(){
        int ret=this->back();
        erase(--_end);
        return ret;
    }

    RBT_iter RBT::insert(RBT_iter iter,int elem){
        node* now;
        node* lc=new node;
        node* rc=new node;
        if(_size==0U){//仅为根节点
            now=new node;
            now->lchild=lc;
            now->rchild=rc;
            now->val=elem;
            lc->father=rc->father=now;
            _begin=root=RBT_iter(now);
            _end=RBT_iter(rc);
            iter.write_pointer(now);
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
            if(iter==_begin){
                _begin.write_pointer(now);
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
        if(_begin==iter)
            iter.write_pointer(now);
        return iter;
    }

    RBT_iter RBT::erase(RBT_iter iter){//同时也要注意调整！！！begin和end
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
                epolish(ne,ne);
            }
            node* temp=ne;
            do{
                temp->num--;
            }while(temp=temp->father);//erase结点时先不动num，左右旋转照常计数，最后一起自底向上更新一遍。
            return RBT_iter(ne->father);
        }
        else if((!ne->lchild->isNIL())&&(!ne->rchild->isNIL())){//两子女均不为空结点
            RBT_iter n_iter=iter;
            ++n_iter;
            node* _ne=n_iter.get_pointer();
            std::swap(ne->val,_ne->val);//交换该结点与下一结点，重新调用erase函数
            erase(n_iter);
            return iter;//还指向它，与它无关
        }
        else{//有一个子女非空
            node* temp;
            temp=ne->lchild->isNIL()?ne->rchild:ne->lchild;
            ne->val=temp->val;
            delete temp->lchild;
            delete temp->rchild;
            temp->NIL();
            do{
                temp->num--;
            }while(temp=temp->father);
            if(_end.get_pointer()==ne) _end.write_pointer(ne->rchild);
            if(_begin.get_pointer()->father==ne) _begin.write_pointer(ne);
            return iter;
        }
    }

    void RBT::ipolish(node* ne,node* avoid){//polish的主要任务是：左右旋转
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

    void RBT::epolish(node* ne,node* avoid){
        if(ne->father==nullptr){
            ne->color=BLACK;
        }
        else if(ne->father->lchild==ne){//该结点是左孩子
            node* brother=ne->father->rchild;
            if(brother->lchild->isNIL()&&brother->rchild->isNIL()){//兄弟结点两个孩子为空
                brother->color=RED;
                ne->father->color=BLACK;
                epolish(ne->father,ne->father);//需要有这个吗？？？？
            }
            else if((!brother->lchild->isNIL())&&(!brother->lchild->isNIL())){//兄弟结点两孩子均不为空
                if(brother->color==RED){//兄弟为红色
                    brother->color=BLACK;
                    brother->lchild->color=RED;
                    leftro(brother->father,nullptr);
                }
                else{
                    std::swap(brother->color,brother->father->color);
                    leftro(brother->father,nullptr);
                }
            }
            else{
                if(!brother->lchild->isNIL()){//兄弟结点左孩子不为空
                    brother->color=RED;
                    brother->lchild->color=BLACK;
                    rightro(brother,nullptr);
                    brother=brother->father;
                }
                brother->rchild->color=BLACK;
                std::swap(brother->color,brother->father->color);
                leftro(brother->father,nullptr);//不知道该不该是nullptr再看
            }
        }
        else{//该结点是右孩子
            node* brother=ne->father->lchild;
            if(brother->lchild->isNIL()&&brother->rchild->isNIL()){//兄弟结点两个孩子为空
                brother->color=RED;
                ne->father->color=BLACK;
                epolish(ne->father,ne->father);//需要有这个吗？？？？
            }
            else if((!brother->lchild->isNIL())&&(!brother->lchild->isNIL())){//兄弟结点两孩子均不为空
                if(brother->color==RED){//兄弟为红色
                    brother->color=BLACK;
                    brother->lchild->color=RED;
                    rightro(brother->father,nullptr);
                }
                else{
                    std::swap(brother->color,brother->father->color);
                    rightro(brother->father,nullptr);
                }
            }
            else{
                if(!brother->rchild->isNIL()){//兄弟结点左孩子不为空
                    brother->color=RED;
                    brother->rchild->color=BLACK;
                    leftro(brother,nullptr);
                    brother=brother->father;
                }
                brother->lchild->color=BLACK;
                std::swap(brother->color,brother->father->color);
                rightro(brother->father,nullptr);//不知道该不该是nullptr再看
            }
        }
    }

    void RBT::leftro(node* ne,node* avoid){
        node* rc=ne->rchild;
        node* tempfather=ne->father;
        if(tempfather)//排除根节点的情况
            if(ne==tempfather->lchild)
                tempfather->lchild=rc;
            else
                tempfather->rchild=rc;
        else
            root.write_pointer(rc);
        rc->father=tempfather;
        node* rclc=rc->lchild;
        rc->lchild=ne;
        ne->father=rc;
        ne->rchild=rclc;
        rclc->father=ne;
        countnum(ne,avoid);
        countnum(rc,avoid);
    }

    void RBT::rightro(node* ne,node* avoid){
        node* lc=ne->lchild;
        node* tempfather=ne->father;
        if(tempfather)//排除根节点的情况
            if(ne==tempfather->lchild)
                tempfather->lchild=lc;
            else
                tempfather->rchild=lc;
        else
            root.write_pointer(lc);
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

    void RBT::destroynode(node* ne){//删除一整颗子树
        if(ne->lchild) destroynode(ne->lchild);
        if(ne->rchild) destroynode(ne->rchild);
        delete ne;
    }

    int& RBT::operator[](unsigned index){//index=0考虑了没有？
        index++;
        unsigned temp=0;
        node* ne=root.get_pointer();
        if(_size&&index<=ne->num&&index>=1)
            while(true){
                // p("debug");
                // printf("%p\n",ne->lchild);
                // __p(temp,ne->val,ne->lchild->num);
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
}

int main(){
    DS::RBT rbt;
    rbt.push_back(1);
    rbt.push_back(2);
    for(int i=0;i<5;i++)
    rbt.insert(rbt.begin(),i);
    for(int i=0;i<7;i++){
        p(rbt[i]);
    }

}
//修改insert、debug erase

