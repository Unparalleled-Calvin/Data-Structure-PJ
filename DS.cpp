//本分支是第3个版本了..
//版本1和版本2共同的缺点是由于哨兵结点的存在，导致内存有一半是浪费的，
//版本1的由于多次new、delete，导致时间浪费比较严重。
//版本2在每个红黑树内维护了一个数组和queue用于存放内存，在扩容时的批量复制和批量压队列都带来了灾难性的时间和空间后果
//导致当数据量比较大时和版本1时间持平，空间略大，实在是不划算。
//想到其实根本没必要像vector一样扩容+复制
//得益于杜宇轩的全局内存池的启发，利用一个全局的对象统一管理所有的rbt的内存，而不需要为每个rbt单独开一个池子（造成浪费）
//每次开定长的空间，用以减少复制的浪费，使得新开一个空间时，queue的容量不超过那个定长
#include<iostream>
#include<ctime>
#include<cstdlib>
#include<vector>
#include<queue>
#define RBT LinearTable
#define p(i) std::cout<<i<<std::endl;
#define _p(i,j) std::cout<<i<<" "<<j<<std::endl;
#define __p(i,j,k) std::cout<<i<<" "<<j<<" "<<k<<std::endl;
#define BLACK 1
#define RED 0
namespace DS{
    //红黑树结点
    class node{
        public:
            node();
            node(const node& ne);

            node& operator=(node& ne);

            bool isNIL();//是否为NIL结点
            void NIL();//将其设为NIL结点

            //以下是属性
            unsigned num=0;//统计子树结点个数
            int val;
            unsigned father=-1;
            unsigned lchild=-1;
            unsigned rchild=-1;
            bool color;
    };

    //红黑树迭代器
    class RBT_iter{
        public:
            RBT_iter();
            RBT_iter(unsigned ne);
            RBT_iter(const RBT_iter& n_iter);//拷贝构造函数

            int& operator*();
            RBT_iter operator++();
            RBT_iter operator++(int);
            RBT_iter operator--();
            RBT_iter operator--(int);
            const RBT_iter& operator=(const RBT_iter& n_iter);

            //以下是随机访问迭代器的相关方法
            const RBT_iter& operator+=(int n);
            bool operator==(const RBT_iter& n_iter);
            bool operator==(const RBT_iter& n_iter) const;
            bool operator!=(const RBT_iter& n_iter);
            bool operator!=(const RBT_iter& n_iter) const;
            bool operator>=(const RBT_iter& n_iter);
            bool operator>=(const RBT_iter& n_iter) const;
            bool operator<=(const RBT_iter& n_iter);
            bool operator<=(const RBT_iter& n_iter) const;
            bool operator>(const RBT_iter& n_iter);
            bool operator>(const RBT_iter& n_iter) const;
            bool operator<(const RBT_iter& n_iter);
            bool operator<(const RBT_iter& n_iter) const;
            friend RBT_iter operator+(RBT_iter ne,int offset);
            friend RBT_iter operator-(RBT_iter ne,int offset);

            int get_pointer();
            const int get_pointer() const;
            void write_pointer(int ne);

            void swap(RBT_iter& n_iter);

            unsigned get_index();
            unsigned get_index() const;
        private:
            unsigned pointer;
    };

    //红黑树
    class RBT{
        public:
            RBT();//构造函数
            RBT(int n,int val);//构造长度为n的线性表
            RBT(const RBT_iter& ib,const RBT_iter& ie);//从begin迭代器到end迭代器
            RBT(int* ab,int* ae);
            ~RBT();//析构函数

            int front();
            void push_front(int val);
            void push_front(int num,int val);
            void pop_front();
            void pop_front(int pop_size);
            
            int back();
            void push_back(int val);
            void push_back(int num,int val);
            void pop_back();
            void pop_back(int pop_size);

            unsigned size();
            bool empty();

            int& operator[](unsigned index);

            void swap(int indexA,int indexB);
            void swap(RBT& nrbt);

            void clear();

            RBT_iter begin();
            RBT_iter end();
            RBT_iter root();
            RBT_iter insert(RBT_iter iter,int val);//在某迭代器的位置插入
            RBT_iter erase(RBT_iter iter);//删除迭代器指向的元素

            RBT_iter get_iter(int index);
            
        private:
        
            RBT_iter _root;
            RBT_iter _begin;
            RBT_iter _end;

            size_t _size=0U;

            void ipolish(int ne,int avoid=-1);//insert_polish
            void epolish(int ne);//erase_polish
            void leftro(int ne,int avoid=-1);//左旋操作
            void rightro(int ne,int avoid=-1);//右旋操作
            void countnum(int ne,int avoid=-1);
            void destroynode(int ne,int father=-1);
    };

    class POOL{//内存池类
        public:
            POOL(int capacity_size=256U);
            ~POOL();
            unsigned alloc();
            void revert(unsigned index);
            node& operator[](unsigned index);
        private:
            std::vector<node*> memory;
            std::queue<unsigned> aval;
            unsigned capacity;
    };

    //友元函数
    RBT_iter operator+(RBT_iter ne,int offset);
    RBT_iter operator-(RBT_iter ne,int offset);

    POOL pool;
}

//以下是node类的具体实现
namespace DS{
    node::node(){
        NIL();
        father=-1;
    }

    node::node(const node& ne){//复制构造函数和属性有关，改属性一定要改这个！
        num=ne.num;
        val=ne.val;
        father=ne.father;
        lchild=ne.lchild;
        rchild=ne.rchild;
        color=ne.color;
    }

    node& node::operator=(node& ne){
        num=ne.num;//统计子树结点个数
        val=ne.val;
        father=ne.father;
        lchild=ne.lchild;
        rchild=ne.rchild;
        color=ne.color;
        return *this;
    }

    void node::NIL(){
        val=0;
        lchild=rchild=-1;
        color=BLACK;
    }

    bool node::isNIL(){
        return color&&(lchild==rchild);//只有NIL的左右结点相等为-1
    }
}

//以下是RBT_iterator类的具体实现
namespace DS{
    RBT_iter::RBT_iter(){
        pointer=-1;
    }

    RBT_iter::RBT_iter(unsigned ne){
        pointer=ne;
    }

    RBT_iter::RBT_iter(const RBT_iter& n_iter){
        pointer=n_iter.pointer;
    }
    
    int RBT_iter::get_pointer(){
        return pointer;
    }

    const int RBT_iter::get_pointer() const {
        return pointer;
    }

    int& RBT_iter::operator*(){
        return pool[pointer].val;
    }

    RBT_iter RBT_iter::operator++(){
        if(pool[pointer].isNIL()) return *this;//如果是NIL，也就是一开始的情况，这里考虑一下鲁棒性
        if(pool[pool[pointer].rchild].isNIL()){//如果右孩子是空结点，那么就上去
            while(pool[pointer].father!=-1&&pool[pool[pointer].father].rchild==pointer){
                pointer=pool[pointer].father;
            }//向上跳直到是父亲的左孩子
            if(pool[pointer].father==-1){//说明应该指向最后一个结点，只需跳往右下角
                while(pool[pointer].rchild!=-1)
                    pointer=pool[pointer].rchild;
            }
            else{
                pointer=pool[pointer].father;
            }
        }
        else{
            pointer=pool[pointer].rchild;
            while(!pool[pool[pointer].lchild].isNIL()){//去右子树的左下角
                pointer=pool[pointer].lchild;
            }
        }
        return *this;
    }

    RBT_iter RBT_iter::operator++(int){//后置
        int ne=pointer;
        ++(*this);
        return RBT_iter(ne);
    }

    RBT_iter RBT_iter::operator--(){
        if(pool[pointer].isNIL()||pool[pool[pointer].lchild].isNIL()){//如果自己是空或左孩子是空结点，那么就上去
            while(pool[pointer].father!=-1&&pool[pool[pointer].father].lchild==pointer){//如果是父节点的左孩子，那就一直往上跳
                pointer=pool[pointer].father;
            }
            if(pool[pointer].father==-1){//说明应该是左下角的位置，去_begin
                while(!pool[pool[pointer].lchild].isNIL())
                    pointer=pool[pointer].rchild;
            }
            else{
                pointer=pool[pointer].father;
            }
        }
        else{
            pointer=pool[pointer].lchild;
            while(!pool[pool[pointer].rchild].isNIL()){//左子树非空去左子树右下角
                pointer=pool[pointer].rchild;
            }
        }
        return *this;
    }

    RBT_iter RBT_iter::operator--(int){
        int ne=pointer;
        --(*this);
        return RBT_iter(ne);
    }

    const RBT_iter& RBT_iter::operator=(const RBT_iter& n_iter){
        pointer=n_iter.pointer;
        return *this;
    }

    void RBT_iter::write_pointer(int ne){
        pointer=ne;
    }

    bool RBT_iter::operator==(const RBT_iter& n_iter){
        return pointer==n_iter.pointer;
    }

    bool RBT_iter::operator==(const RBT_iter& n_iter) const{
        return pointer==n_iter.pointer;
    }

    bool RBT_iter::operator!=(const RBT_iter& n_iter){
        return !(*this==n_iter);
    }

    bool RBT_iter::operator!=(const RBT_iter& n_iter) const{
        return !(*this==n_iter);
    }

    bool RBT_iter::operator>=(const RBT_iter& n_iter){
        return this->get_index()>=n_iter.get_index();
    }

    bool RBT_iter::operator>=(const RBT_iter& n_iter) const{
        return this->get_index()>=n_iter.get_index();
    }

    bool RBT_iter::operator<=(const RBT_iter& n_iter){
        return this->get_index()<=n_iter.get_index();
    }

    bool RBT_iter::operator<=(const RBT_iter& n_iter) const{
        return this->get_index()<=n_iter.get_index();
    }

    bool RBT_iter::operator>(const RBT_iter& n_iter){
        return *this>=n_iter&&*this!=n_iter;
    }

    bool RBT_iter::operator>(const RBT_iter& n_iter) const{
        RBT_iter temp=*this;
        return temp>n_iter;
    }

    bool RBT_iter::operator<(const RBT_iter& n_iter){
        return *this<=n_iter&&*this!=n_iter;
    }

    bool RBT_iter::operator<(const RBT_iter& n_iter) const{
        RBT_iter temp=*this;
        return temp<n_iter;
    }

    void RBT_iter::swap(RBT_iter& n_iter){
        std::swap(pointer,n_iter.pointer);
    }

    unsigned RBT_iter::get_index(){//自下而上logn遍历到n获取索引
        unsigned fa,temp=pointer,ret;
        ret=pool[pointer].isNIL()?-1+(pool[pointer].father==-1):pool[pool[pointer].lchild].num;//防止出现size==0的情况
        while((fa=pool[temp].father)!=-1){
            if(temp==pool[fa].rchild){//如果是父节点的右孩子
                ret+=pool[pool[fa].lchild].num+1;
            }
            temp=fa;
        }
        return ret;
    }

    unsigned RBT_iter::get_index() const{
        RBT_iter temp=*this;
        return temp.get_index();
    }

}

//以下是RBT类的具体实现
namespace DS{
    RBT::RBT(){
        _size=0U;
        _end=_begin=_root=RBT_iter(pool.alloc());
    }

    RBT::RBT(int n,int val){
        _size=0U;
        _end=_begin=_root=RBT_iter(pool.alloc());
        for(int i=0;i<n;i++)
            this->push_back(val);//朴素地尾插n个val。
    }

    RBT::RBT(const RBT_iter& ib,const RBT_iter& ie){
        _size=0U;
        _end=_begin=_root=RBT_iter(pool.alloc());
        for(RBT_iter temp=ib;!(temp==ie);++temp){
            this->push_back(*temp);
        }
    }

    RBT::RBT(int* ab,int* ae){
        _size=0U;
        _end=_begin=_root=RBT_iter(pool.alloc());
        for(int* temp=ab;temp<ae;temp++){
            push_back(*temp);
        }
    }

    RBT::~RBT(){
        clear();
        pool.revert(_root.get_pointer());
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
        return pool[_begin.get_pointer()].val;
    }
    
    void RBT::push_front(int val){
        insert(_begin,val);
    }

    void RBT::push_front(int num,int val){
        for(int i=0;i<num;i++){
            insert(_begin,val);
        }
    }

    void RBT::pop_front(){
        if(_size)
            erase(_begin==_root?_begin:_begin++);
    }

    void RBT::pop_front(int pop_size){
        while(_size&&pop_size--)
            erase(_begin==_root?_begin:_begin++);
    }

    int RBT::back(){
        if(_size==0) std::cerr<<"Empty container!"<<std::endl;
        return pool[pool[_end.get_pointer()].father].val;
    }

    void RBT::push_back(int val){
        insert(_end,val);
    }

    void RBT::push_back(int num,int val){
        for(int i=0;i<num;i++){
            insert(_end,val);
        }
    }

    void RBT::pop_back(){
        if(_size)
            erase(--_end);
    }

    void RBT::pop_back(int pop_size){
        while(_size&&pop_size--){
            erase(--_end);
        }
    }

    RBT_iter RBT::insert(RBT_iter iter,int val){
        unsigned now;
        unsigned lc=pool.alloc();
        unsigned rc=pool.alloc();
        if(_size==0U){//仅为根节点
            now=_root.get_pointer();
            pool[now].lchild=lc;
            pool[now].rchild=rc;
            pool[now].val=val;
            pool[lc].father=pool[rc].father=now;
            _begin=_root=RBT_iter(now);
            _end=RBT_iter(rc);
        }
        else{//你只管插入，调整的事交给polish
            if(pool[iter.get_pointer()].isNIL()){
                now=iter.get_pointer();
            }
            else{
                now=pool[iter.get_pointer()].lchild;
                while(!pool[now].isNIL()){//一路向右
                    now=pool[now].rchild;
                }
            }
            if(iter==_begin){
                _begin.write_pointer(now);
            }
            if(iter==_end){
                _end.write_pointer(rc);
            }
            pool[now].color=RED;
            pool[now].lchild=lc;
            pool[now].rchild=rc;
            pool[now].val=val;
            pool[lc].father=pool[rc].father=now;
            ipolish(now,now);
        }
        int temp=now;
        do{
            pool[temp].num+=1;
        }while((temp=pool[temp].father)!=-1);//调完结构之后再调数目
        _size++;
        iter.write_pointer(now);
        return iter;
    }

    RBT_iter RBT::erase(RBT_iter iter){
        if(_size==0U) return _begin;
        int ne=iter.get_pointer();
        if(iter==_end&&pool[iter.get_pointer()].isNIL()){//考虑是直接erase(_end)的情况
            this->pop_back();
            RBT_iter temp(_end);
            return --temp;
        }
        if(pool[pool[ne].lchild].isNIL()&&pool[pool[ne].rchild].isNIL()){//左右结点都为NIL
            if(pool[ne].color==RED){//该结点若为红色，直接删除
                pool.revert(pool[ne].lchild);
                pool.revert(pool[ne].rchild);
                pool[ne].NIL();
            }
            else{//该结点为黑色
                pool.revert(pool[ne].lchild);
                pool.revert(pool[ne].rchild);
                pool[ne].NIL();
                epolish(ne);
            }
            int temp=ne;
            do{
                pool[temp].num-=1;
            }while((temp=pool[temp].father)!=-1);//erase结点时先不动num，左右旋转照常计数，最后一起自底向上更新一遍。
            _size--;
            return RBT_iter(pool[ne].father);
        }
        else if((!pool[pool[ne].lchild].isNIL())&&(!pool[pool[ne].rchild].isNIL())){//两子女均不为空结点
            RBT_iter n_iter=iter;
            ++n_iter;
            int _ne=n_iter.get_pointer();
            std::swap(pool[ne].val,pool[_ne].val);//交换该结点与下一结点，重新调用erase函数
            erase(n_iter);//
            return iter;//还指向它，与它无关
        }
        else{//有一个子女非空这个子女必为红色
            int temp;
            temp=pool[pool[ne].lchild].isNIL()?pool[ne].rchild:pool[ne].lchild;
            pool[ne].val=pool[temp].val;
            if(_end.get_pointer()==ne) _end.write_pointer(pool[ne].rchild);//这个是为了应对pop_back先将end--的情况
            else if(_end.get_pointer()==pool[pool[ne].rchild].rchild) _end.write_pointer(pool[ne].rchild);//正常删除涉及_end的情况
            if(pool[_begin.get_pointer()].father==ne) _begin.write_pointer(ne);//这个是为了应对pop_front先将front--的情况
            else if(_begin.get_pointer()==pool[ne].lchild) _begin.write_pointer(ne);//正常涉及_front的情况
            pool.revert(pool[temp].lchild);
            pool.revert(pool[temp].rchild);
            pool[temp].NIL();
            do{
                pool[temp].num-=1;
            }while((temp=pool[temp].father)!=-1);//顺序不对，在delete之前就应该处理好
            _size--;
            return iter;
        }
    }

    void RBT::ipolish(int ne,int avoid){//用于insert的结构调整
        if(pool[ne].father==-1){//要调整的节点为根节点
            pool[ne].color=BLACK;
        }
        else if(pool[pool[ne].father].color==RED){//新节点的父节点为红色
            int tempfather=pool[ne].father;
            int grandfather=pool[tempfather].father;
            int uncle;
            if(tempfather==pool[grandfather].lchild){//父节点在左边
                if(ne==pool[tempfather].lchild){//新节点在左边
                    uncle=pool[grandfather].rchild;
                    if(pool[uncle].color==BLACK){//叔父节点为黑色
                        pool[tempfather].color=BLACK;
                        pool[grandfather].color=RED;
                        rightro(grandfather,avoid);
                    }
                    else{//叔父节点为红色
                        pool[tempfather].color=BLACK;
                        pool[uncle].color=BLACK;
                        pool[grandfather].color=RED;
                        ipolish(grandfather,avoid);
                    }
                }
                else{//新节点在右边
                    uncle=pool[grandfather].rchild;
                    if(pool[uncle].color==BLACK){//叔父节点为黑色
                        leftro(tempfather,avoid);
                        pool[ne].color=BLACK;
                        pool[grandfather].color=RED;
                        rightro(grandfather,avoid);
                    }
                    else{//叔父节点为红色
                        pool[tempfather].color=BLACK;
                        pool[uncle].color=BLACK;
                        pool[grandfather].color=RED;
                        ipolish(grandfather,avoid);
                    }
                }
            }
            else{//父节点在右边
                if(ne==pool[tempfather].rchild){//新节点在右边
                    uncle=pool[grandfather].lchild;
                    if(pool[uncle].color==BLACK){//叔父节点为黑色
                        pool[tempfather].color=BLACK;
                        pool[grandfather].color=RED;
                        leftro(grandfather,avoid);
                    }else{//叔父节点为红色
                        pool[tempfather].color=BLACK;
                        pool[uncle].color=BLACK;
                        pool[grandfather].color=RED;
                        ipolish(grandfather,avoid);
                    }
                }
                else{//新节点在左边
                    uncle=pool[grandfather].lchild;
                    if(pool[uncle].color==BLACK){//叔父节点为黑色
                        rightro(tempfather,avoid);
                        pool[ne].color=BLACK;
                        pool[grandfather].color=RED;
                        leftro(grandfather,avoid);
                    }
                    else{//叔父节点为红色
                        pool[tempfather].color=BLACK;
                        pool[uncle].color=BLACK;
                        pool[grandfather].color=RED;
                        ipolish(grandfather,avoid);
                    }
                }
            }
        }
    }

    void RBT::epolish(int ne){//用于erase的结构调整
        if(pool[ne].father==-1){//情况一，n是新的根
            pool[ne].color=BLACK;
        }
        else if(pool[pool[ne].father].lchild==ne){//该结点是左孩子
            int brother=pool[pool[ne].father].rchild;
            if(pool[pool[ne].father].color==RED&&pool[pool[brother].lchild].color==BLACK&&pool[pool[brother].rchild].color==BLACK){//情况二，父节点为红，兄弟结点两孩子为黑
                pool[pool[ne].father].color=BLACK;
                pool[brother].color=RED;
            }
            else if(pool[pool[ne].father].color==BLACK&&pool[brother].color==BLACK&&pool[pool[brother].lchild].color==BLACK&&pool[pool[brother].rchild].color==BLACK){//情况六，需要递归删除
                pool[brother].color=RED;
                epolish(pool[ne].father);
            }
            else if(pool[brother].color==BLACK){//兄弟结点为黑色，此时必定下面有一红
                if(pool[pool[brother].lchild].color==RED){//情况四，兄弟结点左孩子为红色
                    pool[brother].color=RED;
                    pool[pool[brother].lchild].color=BLACK;
                    rightro(brother);
                    brother=pool[brother].father;
                }
                pool[pool[brother].rchild].color=BLACK;//情况三，兄弟结点右孩子为红
                std::swap(pool[brother].color,pool[pool[brother].father].color);
                leftro(pool[brother].father);//不知道该不该是nullptr再看
            }
            else{//情况五，兄弟结点为红
                pool[brother].color=BLACK;
                pool[pool[brother].father].color=RED;
                leftro(pool[brother].father);//操作使ne有一个红爸爸和一个黑兄弟，可以对其重新调整
                epolish(ne);
            }            
        }
        else{//该结点是右孩子
            int brother=pool[pool[ne].father].lchild;
            if(pool[pool[ne].father].color==RED&&pool[pool[brother].lchild].color==BLACK&&pool[pool[brother].rchild].color==BLACK){//情况二，父节点为红，兄弟结点两孩子为黑
                pool[pool[ne].father].color=BLACK;
                pool[brother].color=RED;
            }
            else if(pool[pool[ne].father].color==BLACK&&pool[brother].color==BLACK&&pool[pool[brother].lchild].color==BLACK&&pool[pool[brother].rchild].color==BLACK){//情况六，需要递归删除
                pool[brother].color=RED;
                epolish(pool[ne].father);
            }
            else if(pool[brother].color==BLACK){//兄弟结点为黑色，此时必定下面有一红
                if(pool[pool[brother].rchild].color==RED){//情况四，兄弟结点左孩子为红色
                    pool[brother].color=RED;
                    pool[pool[brother].rchild].color=BLACK;
                    leftro(brother);
                    brother=pool[brother].father;
                }
                pool[pool[brother].lchild].color=BLACK;//情况三，兄弟结点右孩子为红
                std::swap(pool[brother].color,pool[pool[brother].father].color);
                rightro(pool[brother].father);//不知道该不该是nullptr再看
            }
            else{//情况五，兄弟结点为红
                pool[brother].color=BLACK;
                pool[pool[brother].father].color=RED;
                rightro(pool[brother].father);//操作使ne有一个红爸爸和一个黑兄弟，可以对其重新调整
                epolish(ne);
            }
        }
    }

    void RBT::leftro(int ne,int avoid){//左旋
        int rc=pool[ne].rchild;
        int tempfather=pool[ne].father;
        if(tempfather!=-1)//排除根节点的情况
            if(ne==pool[tempfather].lchild)
                pool[tempfather].lchild=rc;
            else
                pool[tempfather].rchild=rc;
        else
            _root.write_pointer(rc);
        pool[rc].father=tempfather;
        int rclc=pool[rc].lchild;
        pool[rc].lchild=ne;
        pool[ne].father=rc;
        pool[ne].rchild=rclc;
        pool[rclc].father=ne;
        countnum(ne,avoid);
        countnum(rc,avoid);
    }

    void RBT::rightro(int ne,int avoid){//右旋
        int lc=pool[ne].lchild;
        int tempfather=pool[ne].father;
        if(tempfather!=-1)//排除根节点的情况
            if(ne==pool[tempfather].lchild)
                pool[tempfather].lchild=lc;
            else
                pool[tempfather].rchild=lc;
        else
            _root.write_pointer(lc);
        pool[lc].father=tempfather;
        int lcrc=pool[lc].rchild;
        pool[lc].rchild=ne;
        pool[ne].father=lc;
        pool[ne].lchild=lcrc;
        pool[lcrc].father=ne;
        countnum(ne,avoid);
        countnum(lc,avoid);
    }

    void RBT::countnum(int ne,int avoid){
        if(!pool[ne].isNIL())
            pool[ne].num=pool[pool[ne].lchild].num+pool[pool[ne].rchild].num+(ne!=avoid);
    }

    void RBT::destroynode(int ne,int father){//递归删除一整颗子树
        if(pool[ne].lchild!=-1) destroynode(pool[ne].lchild);
        if(pool[ne].rchild!=-1) destroynode(pool[ne].rchild);
        if(pool[ne].father!=father) pool.revert(ne);//否则
        else{
            _size-=pool[ne].num;
            pool[ne].NIL();
            if(father==-1){
                _begin.write_pointer(ne);
                _end.write_pointer(ne);
            }
        }
    }

    RBT_iter RBT::get_iter(int index){
        return _begin+index;
    }

    int& RBT::operator[](unsigned index){//随机访问
        index++;
        unsigned temp=0;
        int ne=_root.get_pointer();
        if(_size&&index<=pool[ne].num&&index>=1)
            while(true){
                if(index>temp+pool[pool[ne].lchild].num){
                    if(index==temp+pool[pool[ne].lchild].num+1) return pool[ne].val;
                    else{
                        temp+=1+pool[pool[ne].lchild].num;
                        ne=pool[ne].rchild;
                    }
                }
                else{
                    ne=pool[ne].lchild;
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
        _root.swap(nrbt._root);
        _begin.swap(nrbt._begin);
        _end.swap(nrbt._end);
        std::swap(nrbt._size,_size);
    }

    unsigned int RBT::size(){
        return _size;
    }

    bool RBT::empty(){
        return !_size;
    }
}

//以下是POOL类的具体实现
namespace DS{
    POOL::POOL(int capacity_size){
        capacity=capacity_size;
    }

    POOL::~POOL(){
        int size=memory.size();
        for(int i=0;i<size;i++)
            delete[] memory[i];
    }

    unsigned POOL::alloc(){
        if(aval.empty()){
            unsigned temp=capacity*memory.size();
            for(unsigned i=0;i<capacity;i++){
                aval.push(i+temp);
            }
            memory.push_back(new node[capacity]);
        }
        int ret=aval.front();
        aval.pop();
        return ret;
    }

    void POOL::revert(unsigned index){
        (*this)[index].NIL();
        aval.push(index);
    }

    node& POOL::operator[](unsigned index){
        return memory[index/capacity][index%capacity];
    }
}

//以下是友元函数的具体实现
namespace DS{
    RBT_iter operator+(RBT_iter ne,int offset){
        unsigned temp=ne.get_pointer();
        unsigned _index=ne.get_index()+offset+1;
        unsigned temp_sum=0;
        if(offset<60){//两种情况，如果所需步数小于60，那么就用朴素实现
            for(int i=0;i<offset;i++)
                ++ne;
            return ne;
        }
        while(pool[temp].father!=-1)//否则先找对应index
            temp=pool[temp].father;//找到root
        if(_index>pool[temp].num){
            while(pool[temp].rchild!=-1)
                temp=pool[temp].rchild;//一路向右直到_end的位置
            return RBT_iter(temp);
        }
        else{//否则到那个位置
            while(true){
                if(_index>temp_sum+pool[pool[temp].lchild].num){
                    if(_index==temp_sum+pool[pool[temp].lchild].num+1) return RBT_iter(temp);
                    else{
                        temp_sum+=1+pool[pool[temp].lchild].num;
                        temp_sum=pool[temp].rchild;
                    }
                }
                else{
                    temp=pool[temp].lchild;
                }
            }
        }
    }

    RBT_iter operator-(RBT_iter ne,int offset){
        unsigned temp=ne.get_pointer();
        unsigned _index=ne.get_index()-offset+1;
        unsigned temp_sum=0;
        if(offset<60){
            for(int i=0;i<offset;i++)
                --ne;
            return ne;
        }
        while(pool[temp].father!=-1)
            temp=pool[temp].father;//找到root
        if(_index<1){
            while(!pool[pool[temp].lchild].isNIL())
                temp=pool[temp].rchild;//一路向左至_begin
            return RBT_iter(temp);
        }
        else{//否则到那个位置
            while(true){
                if(_index>temp_sum+pool[pool[temp].lchild].num){
                    if(_index==temp_sum+pool[pool[temp].lchild].num+1) return RBT_iter(temp);
                    else{
                        temp_sum+=1+pool[pool[temp].lchild].num;
                        temp_sum=pool[temp].rchild;
                    }
                }
                else{
                    temp=pool[temp].lchild;
                }
            }
        }
    }
}

// int main(){
//     DS::RBT rbt;
//     int n,op,temp;
//     std::cin>>n;
//     for(int i=0;i<n;i++){
//         std::cin>>op;
//         switch(op){
//             case 1:
//                 std::cin>>temp;
//                 rbt.push_front(temp);
//                 break;
//             case 2:
//                 std::cin>>temp;
//                 rbt.push_back(temp);
//                 break;
//             case 3:
//                 rbt.pop_front();
//                 break;
//             case 4:
//                 rbt.pop_back();
//                 break;
//             case 5:
//                 int x,y;
//                 std::cin>>x>>y;
//                 if(x<rbt.size()){
//                     rbt[x]+=y;
//                 }
//                 break;
//             case 6:
//                 std::cin>>temp;
//                 if(temp<rbt.size()){
//                     p(rbt[temp]);
//                 }
//                 else{
//                     p("SegmentationFault");
//                 }
//                 break;
//             case 7:
//                 if(rbt.empty()){
//                     p("SegmentationFault");}
//                 else
//                     p(rbt[0]);
//                 break;
//             case 8:
//                 if(rbt.empty()){
//                     p("SegmentationFault");}
//                 else
//                     p(rbt[rbt.size()-1]);
//                 break;
//             case 9:
//                 p(rbt.size());
//                 break;
//             case 10:
//                 rbt.clear();
//                 break;
//         }
//     }
// }