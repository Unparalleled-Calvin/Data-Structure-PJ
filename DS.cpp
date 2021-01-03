//本文件将尝试不使用动态new，而是使用固定new+queue来存储可用的结点
#include<iostream>
#include<ctime>
#include<cstdlib>
#include<algorithm>
#include<cstring>
#include<queue>
#define RBT LinearTable
#define RED 0
#define BLACK 1
#define p(i) std::cout<<i<<std::endl;
#define _p(i,j) std::cout<<i<<" "<<j<<std::endl;
#define __p(i,j,k) std::cout<<i<<" "<<j<<" "<<k<<std::endl;
namespace DS{
    //红黑树结点
    class node{
        public:
            node();
            node(const node& ne);

            node& operator=(node& ne);

            bool isNIL();//是否为NIL结点
            void NIL();//将其设为NIL结点
            bool isNIL() const;//是否为NIL结点

            //以下是属性
            unsigned num=0;//统计子树结点个数
            int val;
            unsigned father=-1U;
            unsigned lchild=-1U;
            unsigned rchild=-1U;
            bool color;
    };

    //红黑树迭代器
    class RBT_iter{
        public:
            RBT_iter();
            RBT_iter(unsigned ne,node* _pool);
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
            bool operator!=(const RBT_iter& n_iter);
            bool operator>=(const RBT_iter& n_iter);
            bool operator<=(const RBT_iter& n_iter);
            bool operator>(const RBT_iter& n_iter);
            bool operator<(const RBT_iter& n_iter);
            friend RBT_iter operator+(RBT_iter ne,unsigned offset);
            friend RBT_iter operator-(RBT_iter ne,unsigned offset);

            unsigned get_pointer();
            const unsigned get_pointer() const;
            void write_pointer(unsigned ne);
            node* get_pool();
            const node* get_pool() const; 
            void write_pool(node* _pool);

            void swap(RBT_iter& n_iter);

            unsigned get_index();
            unsigned get_index() const;

        private:
            unsigned pointer;
            node* pool;
    };

    class const_RBT_iter{
        public:
            const_RBT_iter();
            const_RBT_iter(unsigned ne,const node* _pool);
            const_RBT_iter(const const_RBT_iter& n_iter);//拷贝构造函数
            const_RBT_iter(const RBT_iter& n_iter);

            const int& operator*();
            const_RBT_iter operator++();
            const_RBT_iter operator++(int);
            const_RBT_iter operator--();
            const_RBT_iter operator--(int);

            //以下是随机访问迭代器的相关方法
            const RBT_iter& operator+=(int n);
            bool operator==(const const_RBT_iter& n_iter);
            bool operator!=(const const_RBT_iter& n_iter);
            bool operator>=(const const_RBT_iter& n_iter);
            bool operator<=(const const_RBT_iter& n_iter);
            bool operator>(const const_RBT_iter& n_iter);
            bool operator<(const const_RBT_iter& n_iter);
            friend const_RBT_iter operator+(const_RBT_iter ne,unsigned offset);
            friend const_RBT_iter operator-(const_RBT_iter ne,unsigned offset);

            unsigned get_pointer();
            const unsigned get_pointer() const;
            
            const node* get_pool();

            unsigned get_index();
            unsigned get_index() const;

        private:
            unsigned pointer;
            const node* pool;
    };

    //红黑树
    class RBT{
        public:
            RBT();//构造函数
            RBT(unsigned n,int val);//构造长度为n的线性表
            RBT(const RBT& nrbt);//拷贝构造函数
            RBT(RBT &&nrbt);//移动构造函数
            RBT(const RBT_iter& ib,const RBT_iter& ie);//从begin迭代器到end迭代器
            RBT(const const_RBT_iter& ib,const const_RBT_iter& ie);//常量迭代器
            RBT(int* ab,int* ae);//指针构造
            ~RBT();//析构函数

            int front();
            void push_front(int val);
            void push_front(unsigned num,int val);
            void pop_front();
            void pop_front(unsigned pop_size);
            
            int back();
            void push_back(int val);
            void push_back(unsigned num,int val);
            void pop_back();
            void pop_back(unsigned pop_size);

            unsigned size();
            bool empty();

            int& operator[](unsigned index);
            const int& operator[](unsigned) const;

            void swap(unsigned indexA,unsigned indexB);
            void swap(RBT& nrbt);
            void refresh(RBT_iter& nroot,RBT_iter& nbegin,RBT_iter& nend,unsigned& nsize);

            void clear();

            RBT_iter begin();
            const_RBT_iter begin() const ;
            RBT_iter end();
            const_RBT_iter end() const;
            RBT_iter root();
            const_RBT_iter root() const;
            RBT_iter insert(RBT_iter iter,int val);//在某迭代器的位置插入
            RBT_iter insert(RBT_iter iter,unsigned isize,int val);//批量插入
            RBT_iter erase(RBT_iter iter);//删除迭代器指向的元素

            RBT_iter get_iter(unsigned index);
            void setpool();
        private:
        
            RBT_iter _root;
            RBT_iter _begin;
            RBT_iter _end;

            unsigned _size=0U;
            unsigned capacity;
            const unsigned defcapacity=4U;//默认初始空间

            void ipolish(unsigned ne,unsigned avoid=-1);//insert_polish
            void epolish(unsigned ne);//erase_polish
            void leftro(unsigned ne,unsigned avoid=-1);//左旋操作
            void rightro(unsigned ne,unsigned avoid=-1);//右旋操作
            void countnum(unsigned ne,unsigned avoid=-1);
            void destroynode(unsigned ne,unsigned father=-1);

            unsigned alloc();//从内存池中分配空间
            void revert(unsigned ne);//将空间归还内存池

            std::queue<unsigned> aval;
            node* pool;
    };

    //友元函数
    RBT_iter operator+(RBT_iter ne,unsigned offset);
    RBT_iter operator-(RBT_iter ne,unsigned offset);
    const_RBT_iter operator+(const_RBT_iter ne,unsigned offset);
    const_RBT_iter operator-(const_RBT_iter ne,unsigned offset);
}

//以下是node类的具体实现
namespace DS{
    node::node(){
        NIL();
        father=-1U;
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
        lchild=rchild=-1U;
        color=BLACK;
    }

    bool node::isNIL(){
        return color&&(lchild==rchild);//只有NIL的左右结点相等为-1U
    }

    bool node::isNIL() const {
        return color&&(lchild==rchild);
    }
}

//以下是RBT_iter类的具体实现
namespace DS{
    RBT_iter::RBT_iter(){
        pointer=-1U;
        pool=nullptr;
    }

    RBT_iter::RBT_iter(unsigned ne,node* _pool){
        pointer=ne;
        pool=_pool;
    }

    RBT_iter::RBT_iter(const RBT_iter& n_iter){
        pointer=n_iter.pointer;
        pool=n_iter.pool;
    }

    node* RBT_iter::get_pool(){
        return pool;
    }

    const node* RBT_iter::get_pool() const {
        return pool;
    }

    void RBT_iter::write_pool(node* _pool){
        pool=_pool;
    }

    unsigned RBT_iter::get_pointer(){
        return pointer;
    }

    const unsigned RBT_iter::get_pointer() const {
        return pointer;
    }

    int& RBT_iter::operator*(){
        return pool[pointer].val;
    }

    RBT_iter RBT_iter::operator++(){
        if(pool[pointer].isNIL()) return *this;//如果是NIL，也就是一开始的情况，这里考虑一下鲁棒性
        if(pool[pool[pointer].rchild].isNIL()){//如果右孩子是空结点，那么就上去
            while(pool[pointer].father!=-1U&&pool[pool[pointer].father].rchild==pointer){
                pointer=pool[pointer].father;
            }//向上跳直到是父亲的左孩子
            if(pool[pointer].father==-1U){//说明应该指向最后一个结点，只需跳往右下角
                while(pool[pointer].rchild!=-1U)
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
        unsigned ne=pointer;
        ++(*this);
        return RBT_iter(ne,pool);
    }

    RBT_iter RBT_iter::operator--(){
        if(pool[pointer].isNIL()||pool[pool[pointer].lchild].isNIL()){//如果自己是空或左孩子是空结点，那么就上去
            while(pool[pointer].father!=-1U&&pool[pool[pointer].father].lchild==pointer){//如果是父节点的左孩子，那就一直往上跳
                pointer=pool[pointer].father;
            }
            if(pool[pointer].father==-1U){//说明应该是左下角的位置，去_begin
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
        unsigned ne=pointer;
        --(*this);
        return RBT_iter(ne,pool);
    }

    const RBT_iter& RBT_iter::operator=(const RBT_iter& n_iter){
        pointer=n_iter.pointer;
        pool=n_iter.pool;
        return *this;
    }

    void RBT_iter::write_pointer(unsigned ne){
        pointer=ne;
    }

    bool RBT_iter::operator==(const RBT_iter& n_iter){
        return pointer==n_iter.pointer&&pool==n_iter.pool;
    }

    bool RBT_iter::operator!=(const RBT_iter& n_iter){
        return !(*this==n_iter);
    }

    bool RBT_iter::operator>=(const RBT_iter& n_iter){
        return this->get_index()>=n_iter.get_index()&&pool==n_iter.pool;
    }

    bool RBT_iter::operator<=(const RBT_iter& n_iter){
        return this->get_index()<=n_iter.get_index()&&pool==n_iter.pool;
    }

    bool RBT_iter::operator>(const RBT_iter& n_iter){
        return *this>=n_iter&&*this!=n_iter;
    }

    bool RBT_iter::operator<(const RBT_iter& n_iter){
        return *this<=n_iter&&*this!=n_iter;
    }

    void RBT_iter::swap(RBT_iter& n_iter){
        std::swap(pool,n_iter.pool);
        std::swap(pointer,n_iter.pointer);
    }

    unsigned RBT_iter::get_index(){//自下而上logn遍历到n获取索引
        unsigned fa,temp=pointer,ret;
        ret=pool[pointer].isNIL()?-1U+(pool[pointer].father==-1U):pool[pool[pointer].lchild].num;//防止出现size==0的情况
        while((fa=pool[temp].father)!=-1U){
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

//以下是const_RBT_iter类的具体实现
namespace DS{
    const_RBT_iter::const_RBT_iter(unsigned ne,const node* _pool){
        pointer=ne;
        pool=_pool;
    }

    const_RBT_iter::const_RBT_iter(const const_RBT_iter& n_iter){
        pointer=n_iter.pointer;
        pool=n_iter.pool;
    }

    const_RBT_iter::const_RBT_iter(const RBT_iter& n_iter){
        pointer=n_iter.get_pointer();
        pool=n_iter.get_pool();
    }

    const int& const_RBT_iter::operator*(){
        return pool[pointer].val;
    }
    const_RBT_iter const_RBT_iter::operator++(){
        if(pool[pointer].isNIL()) return *this;//如果是NIL，也就是一开始的情况，这里考虑一下鲁棒性
        if(pool[pool[pointer].rchild].isNIL()){//如果右孩子是空结点，那么就上去
            while(pool[pointer].father!=-1U&&pool[pool[pointer].father].rchild==pointer){
                pointer=pool[pointer].father;
            }//向上跳直到是父亲的左孩子
            if(pool[pointer].father==-1U){//说明应该指向最后一个结点，只需跳往右下角
                while(pool[pointer].rchild!=-1U)
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
    const_RBT_iter const_RBT_iter::operator++(int){
        unsigned ne=pointer;
        ++(*this);
        return const_RBT_iter(ne,pool);
    }
    const_RBT_iter const_RBT_iter::operator--(){
        if(pool[pointer].isNIL()||pool[pool[pointer].lchild].isNIL()){//如果自己是空或左孩子是空结点，那么就上去
            while(pool[pointer].father!=-1U&&pool[pool[pointer].father].lchild==pointer){//如果是父节点的左孩子，那就一直往上跳
                pointer=pool[pointer].father;
            }
            if(pool[pointer].father==-1U){//说明应该是左下角的位置，去_begin
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
    const_RBT_iter const_RBT_iter::operator--(int){
        unsigned ne=pointer;
        --(*this);
        return const_RBT_iter(ne,pool);
    }

    bool const_RBT_iter::operator==(const const_RBT_iter& n_iter){
        return pointer==n_iter.pointer&&pool==n_iter.pool;
    }

    bool const_RBT_iter::operator!=(const const_RBT_iter& n_iter){
        return !(*this==n_iter);
    }

    bool const_RBT_iter::operator>=(const const_RBT_iter& n_iter){
        return this->get_index()>=n_iter.get_index()&&pool==n_iter.pool;
    }

    bool const_RBT_iter::operator<=(const const_RBT_iter& n_iter){
        return this->get_index()<=n_iter.get_index()&&pool==n_iter.pool;
    }

    bool const_RBT_iter::operator>(const const_RBT_iter& n_iter){
        return *this>=n_iter&&*this!=n_iter;
    }

    bool const_RBT_iter::operator<(const const_RBT_iter& n_iter){
        return *this<=n_iter&&*this!=n_iter;
    }

    const node* const_RBT_iter::get_pool(){
        return pool;
    }

    unsigned const_RBT_iter::get_pointer(){
        return pointer;
    }
    const unsigned const_RBT_iter::get_pointer() const{
        return pointer;
    }

    unsigned const_RBT_iter::get_index(){
        unsigned fa,temp=pointer,ret;
        ret=pool[pointer].isNIL()?-1U+(pool[pointer].father==-1U):pool[pool[pointer].lchild].num;//防止出现size==0的情况
        while((fa=pool[temp].father)!=-1U){
            if(temp==pool[fa].rchild){//如果是父节点的右孩子
                ret+=pool[pool[fa].lchild].num+1;
            }
            temp=fa;
        }
        return ret;
    }

    unsigned const_RBT_iter::get_index() const {
        const_RBT_iter temp=*this;
        return temp.get_index();
    }
}

//以下是RBT类的具体实现
namespace DS{
    RBT::RBT(){
        _size=0U;
        capacity=defcapacity;
        pool=new node[capacity];
        for(unsigned i=0;i<capacity;i++)
            aval.push(i);
        _end=_begin=_root=RBT_iter(alloc(),pool);
    }

    RBT::RBT(unsigned n,int val){
        capacity=int(1.5*n);//将容量置为1.5倍n，不直接用alloc扩容
        pool=new node[capacity];
        for(unsigned i=0;i<capacity;i++)
            aval.push(i);
        _end=_begin=_root=RBT_iter(alloc(),pool);
        for(unsigned i=0;i<n;i++)
            this->push_back(val);//朴素地尾插n个val。
    }

    RBT::RBT(const RBT& nrbt){
        capacity=nrbt.capacity;
        pool=new node[capacity];
        _begin=nrbt._begin;
        _begin.write_pool(pool);
        _end=nrbt._end;
        _end.write_pool(pool);
        _root=nrbt._root;
        _root.write_pool(pool);
        aval=nrbt.aval;
        memcpy(pool,nrbt.pool,capacity*sizeof(node));
    }

    RBT::RBT(RBT &&nrbt){
        aval=std::move(nrbt.aval);
        pool=nrbt.pool;
        _begin=nrbt._begin;
        _end=nrbt._end;
        _root=nrbt._root;
        _size=nrbt._size;
        capacity=nrbt.capacity;
        nrbt.setpool();
    }

    RBT::RBT(const RBT_iter& ib,const RBT_iter& ie){
        capacity=int(1.5*(ie.get_index()-ib.get_index()));//未实现！！觉得不需要重载这个操作符，没有实际意义，在这里搞一下就行了，以后再搞
        for(unsigned i=0;i<capacity;i++)
            aval.push(i);
        _end=_begin=_root=RBT_iter(alloc(),pool);
        for(RBT_iter temp=ib;!(temp==ie);++temp){
            this->push_back(*temp);
        }
    }

    RBT::RBT(const const_RBT_iter& ib,const const_RBT_iter& ie){
        capacity=int(1.5*(ie.get_index()-ib.get_index()));//未实现！！觉得不需要重载这个操作符，没有实际意义，在这里搞一下就行了，以后再搞
        for(unsigned i=0;i<capacity;i++)
            aval.push(i);
        _end=_begin=_root=RBT_iter(alloc(),pool);
        for(const_RBT_iter temp=ib;!(temp==ie);++temp){
            this->push_back(*temp);
        }
    }

    RBT::RBT(int* ab,int* ae){
        capacity=int(1.5*(ae-ab));
        pool=new node[capacity];
        for(unsigned i=0;i<capacity;i++)
            aval.push(i);
        _end=_begin=_root=RBT_iter(alloc(),pool);
        for(int* temp=ab;temp<ae;temp++){
            this->push_back(*temp);
        }
    }

    RBT::~RBT(){
        delete[] pool;
    }

    unsigned RBT::alloc(){//空间分配函数
        if(aval.empty()){
            unsigned _capacity=int(capacity*1.5);//1.5倍扩容方式
            node* _pool=new node[_capacity];
            memcpy(_pool,pool,capacity*sizeof(node));
            delete[] pool;
            for(unsigned i=capacity;i<_capacity;i++)
                aval.push(i);
            pool=_pool;
            capacity=_capacity;
            _begin.write_pool(pool);
            _end.write_pool(pool);
            _root.write_pool(pool);
        }
        unsigned ret=aval.front();
        aval.pop();
        return ret;
    }

    void RBT::revert(unsigned ne){//归还空间
        pool[ne].NIL();
        aval.push(ne);
    }

    RBT_iter RBT::begin(){
        return _begin;
    }

    const_RBT_iter RBT::begin() const {
        return _begin;
    }

    RBT_iter RBT::end(){
        return _end;
    }

    const_RBT_iter RBT::end() const {
        return _end;
    }

    RBT_iter RBT::root(){
        return _root;
    }

    const_RBT_iter RBT::root() const {
        return _root;
    }

    int RBT::front(){
        if(_size==0U) std::cerr<<"Empty container!"<<std::endl;
        return pool[_begin.get_pointer()].val;
    }
    
    void RBT::push_front(int val){
        insert(_begin,val);
    }

    void RBT::push_front(unsigned num,int val){
        for(unsigned i=0;i<num;i++){
            insert(_begin,val);
        }
    }

    void RBT::pop_front(){
        if(_size)
            erase(_begin==_root?_begin:_begin++);
    }

    void RBT::pop_front(unsigned pop_size){
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

    void RBT::push_back(unsigned num,int val){
        for(unsigned i=0;i<num;i++){
            insert(_end,val);
        }
    }

    void RBT::pop_back(){
        if(_size)
            erase(--_end);
    }

    void RBT::pop_back(unsigned pop_size){
        while(_size&&pop_size--){
            erase(--_end);
        }
    }

    RBT_iter RBT::insert(RBT_iter iter,int val){
        unsigned now;
        unsigned lc=alloc();
        int rc=alloc();
        iter.write_pool(pool);
        if(_size==0U){//仅为根节点
            now=_root.get_pointer();
            pool[now].lchild=lc;
            pool[now].rchild=rc;
            pool[now].val=val;
            pool[lc].father=pool[rc].father=now;
            _begin=_root=RBT_iter(now,pool);
            _end=RBT_iter(rc,pool);
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
        unsigned temp=now;
        do{
            pool[temp].num+=1;
        }while((temp=pool[temp].father)!=-1U);//调完结构之后再调数目
        _size++;
        iter.write_pointer(now);
        return iter;
    }

    RBT_iter RBT::insert(RBT_iter iter,unsigned isize,int val){
        for(unsigned i=0;i<isize;i++){
            iter=insert(iter,val);
        }
        return iter;
    }

    RBT_iter RBT::erase(RBT_iter iter){
        if(_size==0U) return _begin;
        unsigned ne=iter.get_pointer();
        if(iter==_end&&pool[iter.get_pointer()].isNIL()){//考虑是直接erase(_end)的情况
            this->pop_back();
            RBT_iter temp(_end);
            return --temp;
        }
        if(pool[pool[ne].lchild].isNIL()&&pool[pool[ne].rchild].isNIL()){//左右结点都为NIL
            if(pool[ne].color==RED){//该结点若为红色，直接删除
                revert(pool[ne].lchild);
                revert(pool[ne].rchild);
                pool[ne].NIL();
            }
            else{//该结点为黑色
                revert(pool[ne].lchild);
                revert(pool[ne].rchild);
                pool[ne].NIL();
                epolish(ne);
            }
            unsigned temp=ne;
            do{
                pool[temp].num-=1;
            }while((temp=pool[temp].father)!=-1U);//erase结点时先不动num，左右旋转照常计数，最后一起自底向上更新一遍。
            _size--;
            return RBT_iter(pool[ne].father,pool);
        }
        else if((!pool[pool[ne].lchild].isNIL())&&(!pool[pool[ne].rchild].isNIL())){//两子女均不为空结点
            RBT_iter n_iter=iter;
            ++n_iter;
            unsigned _ne=n_iter.get_pointer();
            std::swap(pool[ne].val,pool[_ne].val);//交换该结点与下一结点，重新调用erase函数
            erase(n_iter);//
            return iter;//还指向它，与它无关
        }
        else{//有一个子女非空这个子女必为红色
            unsigned temp;
            temp=pool[pool[ne].lchild].isNIL()?pool[ne].rchild:pool[ne].lchild;
            pool[ne].val=pool[temp].val;
            if(_end.get_pointer()==ne) _end.write_pointer(pool[ne].rchild);//这个是为了应对pop_back先将end--的情况
            else if(_end.get_pointer()==pool[pool[ne].rchild].rchild) _end.write_pointer(pool[ne].rchild);//正常删除涉及_end的情况
            if(pool[_begin.get_pointer()].father==ne) _begin.write_pointer(ne);//这个是为了应对pop_front先将front--的情况
            else if(_begin.get_pointer()==pool[ne].lchild) _begin.write_pointer(ne);//正常涉及_front的情况
            revert(pool[temp].lchild);
            revert(pool[temp].rchild);
            pool[temp].NIL();
            do{
                pool[temp].num-=1;
            }while((temp=pool[temp].father)!=-1U);//顺序不对，在delete之前就应该处理好
            _size--;
            return iter;
        }
    }

    void RBT::ipolish(unsigned ne,unsigned avoid){//用于insert的结构调整
        if(pool[ne].father==-1U){//要调整的节点为根节点
            pool[ne].color=BLACK;
        }
        else if(pool[pool[ne].father].color==RED){//新节点的父节点为红色
            unsigned tempfather=pool[ne].father;
            unsigned grandfather=pool[tempfather].father;
            unsigned uncle;
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

    void RBT::epolish(unsigned ne){//用于erase的结构调整
        if(pool[ne].father==-1U){//情况一，n是新的根
            pool[ne].color=BLACK;
        }
        else if(pool[pool[ne].father].lchild==ne){//该结点是左孩子
            unsigned brother=pool[pool[ne].father].rchild;
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
            unsigned brother=pool[pool[ne].father].lchild;
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

    void RBT::leftro(unsigned ne,unsigned avoid){//左旋
        unsigned rc=pool[ne].rchild;
        unsigned tempfather=pool[ne].father;
        if(tempfather!=-1U)//排除根节点的情况
            if(ne==pool[tempfather].lchild)
                pool[tempfather].lchild=rc;
            else
                pool[tempfather].rchild=rc;
        else
            _root.write_pointer(rc);
        pool[rc].father=tempfather;
        unsigned rclc=pool[rc].lchild;
        pool[rc].lchild=ne;
        pool[ne].father=rc;
        pool[ne].rchild=rclc;
        pool[rclc].father=ne;
        countnum(ne,avoid);
        countnum(rc,avoid);
    }

    void RBT::rightro(unsigned ne,unsigned avoid){//右旋
        unsigned lc=pool[ne].lchild;
        unsigned tempfather=pool[ne].father;
        if(tempfather!=-1U)//排除根节点的情况
            if(ne==pool[tempfather].lchild)
                pool[tempfather].lchild=lc;
            else
                pool[tempfather].rchild=lc;
        else
            _root.write_pointer(lc);
        pool[lc].father=tempfather;
        unsigned lcrc=pool[lc].rchild;
        pool[lc].rchild=ne;
        pool[ne].father=lc;
        pool[ne].lchild=lcrc;
        pool[lcrc].father=ne;
        countnum(ne,avoid);
        countnum(lc,avoid);
    }

    void RBT::countnum(unsigned ne,unsigned avoid){
        if(!pool[ne].isNIL())
            pool[ne].num=pool[pool[ne].lchild].num+pool[pool[ne].rchild].num+(ne!=avoid);
    }

    void RBT::destroynode(unsigned ne,unsigned father){//递归删除一整颗子树
        if(father==-1U){//说明是clear()
            delete[] pool;
            capacity=defcapacity;
            pool=new node[capacity];
            std::queue<unsigned> empty;
            aval=empty;
            for(unsigned i=0;i<capacity;i++)
                aval.push(i);
            _root=_begin=_end=RBT_iter(alloc(),pool);//默认将0作为根
            return;
        }
        if(pool[ne].lchild!=-1U) destroynode(pool[ne].lchild);
        if(pool[ne].rchild!=-1U) destroynode(pool[ne].rchild);
        if(pool[ne].father!=father) revert(ne);//否则
        else{
            _size-=pool[ne].num;
            pool[ne].NIL();
            if(father==-1U){
                _begin.write_pointer(ne);
                _end.write_pointer(ne);
            }
        }
    }

    RBT_iter RBT::get_iter(unsigned index){
        return _begin+index;
    }

    void RBT::setpool(){
        pool=nullptr;
    }

    int& RBT::operator[](unsigned index){//随机访问
        index++;
        unsigned temp=0;
        unsigned ne=_root.get_pointer();
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

    const int& RBT::operator[](unsigned index) const{
        index++;
        unsigned temp=0;
        unsigned ne=_root.get_pointer();
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

    void RBT::swap(unsigned indexA,unsigned indexB){
        if(indexA>=0&&indexA<_size&&indexB>=0&&indexB<_size)
            std::swap((*this)[indexA],(*this)[indexB]);
    }

    void RBT::swap(RBT& nrbt){
        nrbt.refresh(_root,_begin,_end,_size);
        aval.swap(nrbt.aval);
        std::swap(pool,nrbt.pool);
        std::swap(capacity,nrbt.capacity);

    }

    void RBT::refresh(RBT_iter& nroot,RBT_iter& nbegin,RBT_iter& nend,unsigned int& nsize){
        _root.swap(nroot);
        _begin.swap(nbegin);
        _end.swap(nend);
        std::swap(nsize,_size);
    }

    unsigned RBT::size(){
        return _size;
    }

    bool RBT::empty(){
        return !_size;
    }
}

//以下是友元函数的具体实现
namespace DS{
    RBT_iter operator+(RBT_iter ne,unsigned offset){
        unsigned temp=ne.get_pointer();
        unsigned _index=ne.get_index()+offset+1;
        unsigned temp_sum=0;
        node* pool=ne.pool;
        if(offset<60){//两种情况，如果所需步数小于60，那么就用朴素实现
            for(unsigned i=0;i<offset;i++)
                ++ne;
            return ne;
        }
        while(pool[temp].father!=-1U)//否则先找对应index
            temp=pool[temp].father;//找到root
        if(_index>pool[temp].num){
            while(pool[temp].rchild!=-1U)
                temp=pool[temp].rchild;//一路向右直到_end的位置
            return RBT_iter(temp,pool);
        }
        else{//否则到那个位置
            while(true){
                if(_index>temp_sum+pool[pool[temp].lchild].num){
                    if(_index==temp_sum+pool[pool[temp].lchild].num+1) return RBT_iter(temp,pool);
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

    RBT_iter operator-(RBT_iter ne,unsigned offset){
        unsigned temp=ne.get_pointer();
        unsigned _index=ne.get_index()-offset+1;
        unsigned temp_sum=0;
        node* pool=ne.pool;
        if(offset<60){
            for(unsigned i=0;i<offset;i++)
                --ne;
            return ne;
        }
        while(pool[temp].father!=-1U)
            temp=pool[temp].father;//找到root
        if(_index<1){
            while(!pool[pool[temp].lchild].isNIL())
                temp=pool[temp].rchild;//一路向左至_begin
            return RBT_iter(temp,pool);
        }
        else{//否则到那个位置
            while(true){
                if(_index>temp_sum+pool[pool[temp].lchild].num){
                    if(_index==temp_sum+pool[pool[temp].lchild].num+1) return RBT_iter(temp,pool);
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

    const_RBT_iter operator+(const_RBT_iter ne,unsigned offset){
        unsigned temp=ne.get_pointer();
        unsigned _index=ne.get_index()+offset+1;
        unsigned temp_sum=0;
        const node* pool=ne.pool;
        if(offset<60){//两种情况，如果所需步数小于60，那么就用朴素实现
            for(unsigned i=0;i<offset;i++)
                ++ne;
            return ne;
        }
        while(pool[temp].father!=-1U)//否则先找对应index
            temp=pool[temp].father;//找到root
        if(_index>pool[temp].num){
            while(pool[temp].rchild!=-1U)
                temp=pool[temp].rchild;//一路向右直到_end的位置
            return const_RBT_iter(temp,pool);
        }
        else{//否则到那个位置
            while(true){
                if(_index>temp_sum+pool[pool[temp].lchild].num){
                    if(_index==temp_sum+pool[pool[temp].lchild].num+1) return const_RBT_iter(temp,pool);
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

    const_RBT_iter operator-(const_RBT_iter ne,unsigned offset){
        unsigned temp=ne.get_pointer();
        unsigned _index=ne.get_index()-offset+1;
        unsigned temp_sum=0;
        const node* pool=ne.pool;
        if(offset<60){
            for(unsigned i=0;i<offset;i++)
                --ne;
            return ne;
        }
        while(pool[temp].father!=-1U)
            temp=pool[temp].father;//找到root
        if(_index<1){
            while(!pool[pool[temp].lchild].isNIL())
                temp=pool[temp].rchild;//一路向左至_begin
            return const_RBT_iter(temp,pool);
        }
        else{//否则到那个位置
            while(true){
                if(_index>temp_sum+pool[pool[temp].lchild].num){
                    if(_index==temp_sum+pool[pool[temp].lchild].num+1) return const_RBT_iter(temp,pool);
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