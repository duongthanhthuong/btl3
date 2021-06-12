#include "main.h"
Cache::Cache(SearchEngine* s,ReplacementPolicy* r):rp(r),s_engine(s) {}
Cache::~Cache(){
	//delete rp;
	//delete s_engine;
}
Data* Cache::read(int addr) { // Done
    if (this->s_engine->search(addr) == -1) return NULL; // khong tim thay 
    else{
        return this->rp->getValue(this->s_engine->search(addr))->data; // tim thay va tra ve data
    }
}
Elem* Cache::put(int addr, Data* cont) {
    // tao ra phan tu moi put vao cache
    Elem *newElem = new Elem(addr,cont,true);
    // cache day;
    if (this->rp->isFull()){
        int idxremove = this->rp->remove(); // muc duoc dua ra khoi cache
        Elem *removeElem = this->rp->getValue(idxremove);
        this->rp->insert(newElem,idxremove); // insert vao vi tri vua dc dx khoi cache
        this->s_engine->deleteNode(removeElem->addr); // xoa muc duoc dx ra ngoai trong cong cu tim kiem
        this->s_engine->insert(addr,idxremove); // insert vao cong cu tim kiem
        return removeElem;
    }
    else{ // cache chua day
        this->rp->insert(newElem,this->rp->size());
        this->s_engine->insert(addr,this->rp->size()); // insert vao cong cu tim kiem
        this->rp->resize(this->rp->size()+1); // tang kich co cache len 1;
        
    }
    return NULL;
}
Elem* Cache::write(int addr, Data* cont) {
    // tim dia chi addr trong cache
    int index = this->s_engine->search(addr);
    if (index!=-1){ // neu tim duoc
        this->rp->change(index,cont,false);
    }
    else{ // neu khong tim dc
        // tao ra phan tu moi put vao cache
    Elem *newElem = new Elem(addr,cont,false);
    // cache day;
    if (this->rp->isFull()){
        int idxremove = this->rp->remove(); // muc duoc dua ra khoi cache
        Elem *removeElem = this->rp->getValue(idxremove);
        this->rp->insert(newElem,idxremove); // insert vao vi tri vua dc dx khoi cache
        this->s_engine->deleteNode(removeElem->addr); // xoa muc duoc dx ra ngoai trong cong cu tim kiem
        this->s_engine->insert(addr,idxremove); // insert vao cong cu tim kiem
        return removeElem;
    }
    else{ // cache chua day
        this->rp->insert(newElem,this->rp->size());
        this->s_engine->insert(addr,this->rp->size()); // insert vao cong cu tim kiem
        this->rp->resize(this->rp->size()+1); // tang kich co cache len 1;
        
    }
    }
    return NULL;
}
void Cache::printRP() {
    //cout<<"Print replacement\n";
    this->rp->print();
}
void Cache::printSE() {
    //cout<< "Print search buffer\n";
    this->s_engine->print(this->rp);
}
int AVL::getHeightRec(Node *node){
    if (node == NULL)
            return 0;
        int lh = this->getHeightRec(node->pLeft);
        int rh = this->getHeightRec(node->pRight);
        return (lh > rh ? lh : rh) + 1;
}
int   AVL::getHeight(){
    return this->getHeightRec(this->root);
}
Node* AVL::rotateLeft(Node *&root){
    Node *tmp = root->pRight;
    root->pRight = tmp->pLeft;
    tmp->pLeft = root;
    return tmp;
}
Node* AVL::rotateRight(Node *&root){
    Node *tmp = root->pLeft;
        root->pLeft = tmp->pRight;
        tmp->pRight = root;
        return tmp;
}
Node* AVL::leftbalance(Node *&root, bool &taller){
    Node* lefttree = root->pLeft;
        // left of left
        if (lefttree->balance == LH){
            root = rotateRight(root);
            root->balance = EH;
            root->pRight->balance = EH;
            taller = false;
        }
        else{ // right to left
            Node* righttree = lefttree->pRight;
            if(righttree->balance == EH)
            {
                root->balance = EH ; ///////////////////////////////////////////////
                lefttree->balance = EH ;
            }
            else if(righttree->balance == LH){
                lefttree->balance = EH;
                root->balance = EH;
            }
            else{
                lefttree->balance = LH ;
                root->balance = EH;
            }
            righttree->balance = EH;
            root->pLeft = rotateLeft(lefttree);
            root = rotateRight(root);
            taller = false ;
        }
        return root;
}
Node* AVL::rightbalance(Node *&root, bool &taller){
    Node *righttree= root->pRight;
        if(righttree->balance == RH)
        {
            root= rotateLeft(root);
            root->balance = EH ;
            root->pLeft->balance = EH;
            taller = false;
        }
        else{
            Node *lefttree = righttree->pLeft ;
            if (lefttree->balance == EH)
            {
                righttree->balance = EH;
                root->balance = EH;
            }
            else if (lefttree->balance == RH)
            {
                righttree->balance = EH;
                root->balance = LH;
            }
            else{
                righttree->balance = RH;
                root->balance = EH;
            }
            root->pRight = rotateRight(righttree);
            lefttree->balance = EH;
            root = rotateLeft(root);
            taller = false;
        }
        return root;
}
void  AVL::insertRec(Node *&root , int value,int i, bool &taller){
    
    // tree is empty
        if (root==nullptr)
        {
            root = new Node(value,i);
            taller = true;
        }
        else
        {
            // add in lefttree
            if (value < root->data)
            {
                insertRec(root->pLeft, value,i, taller);
                if (taller){
                    if (root->balance== LH) root = leftbalance(root, taller);
                    else if (root->balance == RH){
                        root->balance= EH;
                        taller = false;
                    }
                    else
                    {
                        root->balance= LH;
                    }
                }
            }
            else
            {
                insertRec(root->pRight,value,i,taller);
                if (taller){
                    if (root->balance == RH){
                        root = rightbalance(root,taller);
                    }
                    else if (root->balance == LH){
                        root->balance = EH;
                        taller = false;
                    }
                    else root->balance = RH;
                }
            }
        }
}
void  AVL::insert(int key,int i){
    bool taller = false;
    //cout << " gia tri insert vao la : " << key <<"\n";
    insertRec(this->root,key,i,taller);
}

Node* AVL::deleteRightBalance(Node *&root, bool &shorter){
    //cout<< "Right balance call \n";
        if(root->balance == LH){
            //cout<< "root is LH \n";
            root->balance = EH;
        }
        else if(root->balance == EH){
            //cout<< "root is EH \n";
            root->balance = RH;
            shorter = false ;
        }
        else{
            //cout<< "root is RH \n";
            Node *righttree= root->pRight;
            if (righttree->balance == LH)
            {
                Node *lefttree= righttree->pLeft;
                if (lefttree->balance== LH){
                    root->balance = EH;
                    righttree->balance = RH;
                }
                else if (lefttree->balance== RH){
                    root->balance = LH;
                    righttree->balance = EH;
                }
                else{ 
                    root->balance = EH;//LH;          /////////////////////////////////////////////////////////////////////////////////////////////////
                    righttree->balance = EH;
                }
                lefttree->balance = EH;
                root->pRight= rotateRight(righttree);
                root = rotateLeft(root);

            }
            else{
                    if (righttree->balance == EH){
                    root->balance = RH;
                    righttree->balance = LH;
                    shorter = false;
                    }
                    else if (righttree->balance == RH){
                    root->balance = EH;
                    righttree->balance = EH;
                    }
                    root= rotateLeft(root);
            }
            

        }
        return root;
}
Node* AVL::deleteLeftBalance(Node *&root, bool &shorter){
    //cout<< "Left balance call \n";
        if (root->balance == RH){
            //cout<< "root is RH \n";
            root->balance = EH;
        }
        else if (root->balance == EH){
            //cout<< "root is EH \n";
            root->balance = LH;
            shorter = false;
        }
        else{
            //cout<< "root is LH \n";
            Node *lefttree = root->pLeft;
            if (lefttree->balance == RH){
                Node *righttree = lefttree->pRight;
                if(righttree->balance == EH)
                {
                    root->balance = EH;//RH ;                 /////////////////////////////////////////////////////////////////////////
                    lefttree->balance = EH ;
                }
                else if(righttree->balance == LH){
                    lefttree->balance = EH;
                    root->balance = RH;
                }
                else{ // RH
                    lefttree->balance = LH ;
                    root->balance = EH;
                }
                righttree->balance = EH;
                root->pLeft = rotateLeft(lefttree);
                root = rotateRight(root);
            }
            else{
                if (lefttree->balance == LH){
                    //cout<< "lefttree is LH \n";
                    root->balance = EH;
                    lefttree->balance = EH;
                }
                else{ // EH
                    root->balance = LH;
                    lefttree->balance = RH;
                    shorter = false;
                }
                root= rotateRight(root);
            }
        }
        return root;
}
Node* AVL::deleteNodeRec(Node *&root, int value, bool &success, bool &shorter){
    // tree is empty
        if (root == nullptr){
            success = false;
            shorter = false;
            //return root;
        }
        else{
            if (value < root->data){
                //cout<< " nho hon : left tree\n";
                root->pLeft= deleteNodeRec(root->pLeft,value,success,shorter);
                if (shorter){
                    root = deleteRightBalance(root,shorter);    
                }
            }
            else if (value > root->data){
                //cout<< " lon hon : right tree\n";
                root->pRight=deleteNodeRec(root->pRight,value,success,shorter);
                if (shorter){
                    root = deleteLeftBalance(root,shorter);    
                }
            }
            else{
                //cout<< " bang : xoa node \n";
                Node *deleteNode= root;
                if (root->pLeft == nullptr)
                {
                    //cout<< " left == null : xoa \n";
                    Node *newRoot=root->pRight;
                    //cout<< "1\n";
                    success= true;
                    shorter=true;
                    //cout<< "2\n";
                    delete deleteNode;
                    //cout<< "3\n";
                    return newRoot;
                }
                else if (root->pRight == nullptr){
                    Node *newRoot=root->pLeft;
                    success= true;
                    shorter=true;
                    delete deleteNode;
                    return newRoot;
                }
                else{
                    Node *maxleft= root->pLeft;
                    while (maxleft->pRight != nullptr){
                        maxleft = maxleft->pRight;
                    }
                    root->data = maxleft->data;
                    root->pLeft=deleteNodeRec(root->pLeft,maxleft->data,success,shorter);
                    if (shorter){
                        root = deleteRightBalance(root,shorter);
                    }
                }
            }
            
        }
        return root;
}
void  AVL::deleteNode(int key){
    //TODO
        bool shorter= false;
        bool success = false;
        //cout << "gia tri bi remove la : "<< key << "\n";
        this->root=deleteNodeRec(this->root,key,success,shorter);
}

int   AVL::searchRec(Node*root,int value){
    if (root==nullptr) return -1;
    else
        {
            if (root->data == value) return root->index;
            else if (root->data < value) return searchRec(root->pRight,value);
            else return searchRec(root->pLeft,value);
        }
}

void  AVL::printInorderRec(Node*root,ReplacementPolicy* q){
    if (root!=nullptr)
        {
            printInorderRec(root->pLeft,q);
            //cout<< "root value : " << root->data<<"\n";
            q->getValue(root->index)->print();
            printInorderRec(root->pRight,q);
        }    
}
void  AVL::printPreorderRec(Node*root,ReplacementPolicy* q){
    if (root!=nullptr)
        {
            q->getValue(root->index)->print();
            printPreorderRec(root->pLeft,q);
            printPreorderRec(root->pRight,q);
            
        }    
}

void DBHashing::insert(int key,int i){
    int j = 0;
    while (true)
    {
        int keyhash = (this->h1(key) + j*this->h2(key)) % this->size;
        if (!list[keyhash].used){ // them vao o hien tai neu o hien tai chua su dung
            list[keyhash].data = key;
            list[keyhash].index = i;
            //list[keyhash].j = j;
            list[keyhash].used = true;
            break;
        }
        else{
            j++;
        }
    }
}
int DBHashing::search(int key) {
    int j = 0;
    while (true)
    {
        int keyhash = (this->h1(key) + j*this->h2(key)) % this->size;
        if (list[keyhash].used && list[keyhash].data== key){
            return list[keyhash].index;
        }
        else{
            if (list[keyhash].used== false) return -1; // khong tim thay key
            j++;
        }
    }
    // khong tim thay ??
}
void DBHashing::deleteNode(int key){
    int j = 0;
    while (true)
    {
        int keyhash = (this->h1(key) + j*this->h2(key)) % this->size;
        if (list[keyhash].used && list[keyhash].data== key){
            list[keyhash].used = false;
        }
        else j++;
    }
}
void DBHashing::print(ReplacementPolicy* q){
    cout<< "Prime memory:\n";
    for (int i = 0; i < MAXSIZE; i++)
    {
        /* code */
        if (list[i].used==true){
            q->getValue(list[i].index)->print();
        }
    }
    
}

int     FIFO::insert(Elem* e,int idx){
    // add vi tri cua e vao queue
    this->fifoqueue.push(idx);
    // them vao arr 
    this->arr[idx] = e;
    return 0;
}
void    FIFO::access(int idx) {}
int     FIFO::remove(){
    return this->fifoqueue.pop();
}
void    FIFO::print(){
    while (!this->fifoqueue.empty())
    {
        /* code */
        //cout<< "gia tri dau la " << this->fifoqueue.top()<<"\n";
        this->getValue(this->fifoqueue.pop())->print();
    }
    
}