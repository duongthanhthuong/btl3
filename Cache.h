#ifndef CACHE_H
#define CACHE_H

#include "main.h"


class ReplacementPolicy {
	protected:
	int count;
	Elem** arr;
	
	public:
	virtual int insert(Elem* e,int idx) = 0;//insert e into arr[idx] if idx != -1 else into the position by replacement policy
	virtual void access(int idx) = 0;//idx is index in the cache of the accessed element 
	virtual int remove() = 0; 
	virtual void print() = 0;
	
        void change(int index,Data*cont,bool sync){
                this->arr[index]->data = cont;
                this->arr[index]->sync = sync;
        }
	bool isFull() {return count == MAXSIZE;}
	bool isEmpty() {return count == 0;}
        int size(){return this->count;} // them vao
        void resize(int s){ this->count = s;} // them vao
	Elem* getValue(int idx){return arr[idx];}
	void replace(int idx,Elem* e) {
		delete arr[idx];
		access(idx);
		arr[idx] = e;
	}
};

class SearchEngine {
	public:
	virtual int search(int key) = 0; // -1 if not found
	virtual void insert(int key,int idx) = 0;
	virtual	void deleteNode(int key) = 0;
	virtual void print(ReplacementPolicy* r) = 0;
};
template <class T >
class IList
{
 public :
virtual void add (T e) = 0;
virtual void add ( int index , T e) = 0;
virtual T removeAt (int index ) = 0;
 virtual bool removeItem (T item ) = 0;
 virtual bool empty () = 0;
 virtual int size () = 0;
 virtual void clear () = 0;
 virtual T get (int index ) = 0;
 virtual void set ( int index , T e) = 0;
 virtual int indexOf (T item ) = 0;
 virtual bool contains ( T item ) = 0;

 };
template <class T >
class DLinkedList : public IList<T>
{
public:
        class Node; // Forward declaration

protected:
        Node *head;
        Node *tail;
        int count;

public:
        DLinkedList() : head(NULL), tail(NULL), count(0){};
        virtual void add(T element)
        {
                // chua co phan tu nao
                if (this->count == 0)
                {
                        Node *tmp = new Node(element);
                        this->head = this->tail = tmp;
                        this->count++;
                }
                // th co phan tu
                else
                {
                        Node *tmp = new Node(element);
                        this->tail->next = tmp;
                        this->tail = tmp;
                        this->count++;
                }
        }

        virtual void add(int index, T element)
        {
                // check index
                if (index < 0 || index > this->count)
                        throw std ::out_of_range(" index error");
                // execution
                else
                {
                        // chua co phan tu nao
                        if (this->count == 0)
                        {
                                this->add(element);
                        }
                        // add o dau
                        else if (index == 0)
                        {
                                Node *tmp = new Node(element);
                                this->head->prev = tmp;
                                tmp->next = this->head;
                                this->head = tmp;
                                this->count++;
                        }
                        // add o cuoi
                        else if (index == this->count)
                        {
                                this->add(element);
                        }
                        //add o giua
                        else
                        {
                                Node *tmp = new Node(element);
                                Node *tmp_index = this->head;
                                // chay toi cho index
                                for (int i = 0; i < index; i++)
                                {
                                        tmp_index = tmp_index->next;
                                }
                                tmp->prev = tmp_index->prev;
                                tmp_index->prev->next = tmp;
                                tmp->next = tmp_index;
                                tmp_index->prev = tmp;
                                this->count++;
                        }
                }
        }

        virtual  T removeAt(int index)
        {
                // check index
                if (index < 0 || index >= this->count)
                        throw std ::out_of_range(" index error");
                else
                {
                        T element = 0;
                        // list co 1 phan tu
                        if (this->count == 1)
                        {
                                Node *tmp = this->head;
                                this->head = nullptr;
                                this->tail = nullptr;
                                element = tmp->data;
                                delete tmp;
                                this->count--;
                        }
                        // xoa o dau
                        else if (index == 0)
                        {
                                Node *tmp = this->head;
                                this->head = this->head->next;
                                this->head->prev = nullptr;
                                tmp->next = nullptr;
                                element = tmp->data;
                                delete tmp;
                                this->count--;
                        }
                        // xoa o cuoi
                        else if (index == this->count - 1)
                        {
                                Node *tmp = this->tail;
                                this->tail = this->tail->prev;
                                this->tail->next = nullptr;
                                tmp->prev = nullptr;
                                element = tmp->data;
                                delete tmp;
                                this->count--;
                        }
                        // xoa o giua
                        else
                        {
                                Node *tmp = this->head;
                                for (int i = 0; i < index; i++)
                                {
                                        tmp = tmp->next;
                                }
                                tmp->prev->next = tmp->next;
                                tmp->next->prev = tmp->prev;
                                tmp->next = nullptr;
                                tmp->prev = nullptr;
                                element = tmp->data;
                                delete tmp;
                                this->count--;
                        }
                        return element;
                }
        }

        virtual bool removeItem(T item)
        {
                // list khong co phan tu nao
                if (this->count == 0)
                        return false;
                else
                {
                        Node *tmp = this->head;
                        for (int i = 0; i < this->count; i++)
                        {
                                if (tmp->data == item)
                                {
                                        this->removeAt(i);
                                }
                                return true;
                        }

                        return false;
                }
        }

        virtual bool empty()
        {

                return this->count == 0;
        }

        virtual int size()
        {

                return this->count;
        }

        virtual T get(int index)
        {
                // check index
                if (index < 0 || index >= this->count)
                        throw std ::out_of_range(" index error");
                else
                {
                        Node *tmp = this->head;
                        for (int i = 0; i < index; i++)
                        {
                                tmp = tmp->next;
                        }
                        return tmp->data;
                }
        }

        virtual void set(int index, T element)
        {
                if (index < 0 || index >= this->count)
                        throw std ::out_of_range(" index error");
                else
                {
                        Node *tmp = this->head;
                        for (int i = 0; i < index; i++)
                        {
                                tmp = tmp->next;
                        }
                        tmp->data = element;
                }
        }

        virtual int indexOf(T item) // tra ve idx cua address in cache
        {

                if (this->count == 0)
                        return -1;
                else
                {
                        Node *tmp = this->head;
                        for (int i = 0; i < this->count; i++)
                        {
                                if (tmp->data == item)
                                {
                                        return i;
                                }
                                else
                                        tmp = tmp->next;
                        }

                        return -1;
                }
        }

        virtual bool contains(T item)
        {
                if (this->count == 0)
                        return false;
                else
                {
                        Node *tmp = this->head;
                        for (int i = 0; i < this->count; i++)
                        {
                                if (tmp->data == item)
                                {
                                        return true;
                                }
                                else
                                        tmp = tmp->next;
                        }

                        return false;
                }
        }

        virtual void clear()
        {
                while (this->count != 0)
                {
                        this->removeAt(this->count - 1);
                        this->count--;
                }
                this->head = this->tail = nullptr;
        }

        
public:
        class Node
        {
        public:
                T data; // address = key
                //int idx ; // index trong mang cache
                Node *next;
                Node *prev;

        public:
                Node() : data(0), next(NULL), prev(NULL){};
                Node(T data) : data(data), next(NULL), prev(NULL){};
        };
}; 
template<class T>
class Queue {
protected:
    DLinkedList<T> list;
public:
    Queue() {}
    void push(T item)
    {
        this->list.add(item);
    }
    T pop()
    {
        return this->list.removeAt(0);
    }
    T top()
    {
        return this->list.get(0);
    }
    bool empty()
    {
        return this->list.empty();
    }
    int size()
    {
        return this->list.size();
    }
    void clear()
    {
        this->list.clear();
        delete list;
    }
};
class FIFO : public ReplacementPolicy {
        Queue<int> fifoqueue;
	public:
	FIFO(){count = 0; arr = new Elem*[MAXSIZE]; }
	~FIFO(){}
	int insert(Elem* e,int idx);
	void access(int idx) ;
	int remove();
	void print();
};

class MRU : public ReplacementPolicy {
	public:
	MRU(){}
	~MRU(){}
	int insert(Elem* e,int idx){return 0;}
	void access(int idx){}
	int remove(){return 0;}
	void print(){}
};
class LRU: public MRU {
	public:
	int remove() override {return 0;}
};

class LFU: public ReplacementPolicy {
	public:
	LFU(){}
	~LFU(){}
	int insert(Elem* e,int idx){return 0;}
	void access(int idx){}
	int remove(){return 0;}
	void print(){}
};

/// HASHING //



class Nodehash{
                int data; // address
                int index; // idx trong arr
                bool used; // check o hash nay chua dia chi chua
                //int j; // j in hashing
        public:
                Nodehash(){
                        this->used = false;
                };
                Nodehash(int address, int idx, bool used): data(address),index(idx),used(used){};
                ~Nodehash();
                friend class DBHashing;
};
class DBHashing: public SearchEngine {
	Nodehash *list ;
	int (*h1)(int);
	int (*h2)(int);
        int size;
	public:
	DBHashing(int (*h1)(int),int (*h2)(int),int s){
		list = new Nodehash[MAXSIZE];
		this->h1 = h1;
		this->h2 = h2;
                this->size = s;
		
	}
	~DBHashing(){}
	void insert(int key,int i);
	void deleteNode(int key);
	void print(ReplacementPolicy* q);
	int search(int key);
};
/// AVL ///
enum BalanceValue
{
    LH = -1,
    EH = 0,
    RH = 1
};
class Node
    	{
    		private:
        	int data; // address
		int index; // idx
        	Node *pLeft, *pRight;
        	BalanceValue balance;
        	friend class AVL;

    		public:
        	Node(int value,int i) : data(value),index(i), pLeft(NULL), pRight(NULL), balance(EH) {}
        	~Node() {}
    	};
class AVL : public SearchEngine {
	// khai bao chi tiet lop AVL
	public:
		//class Node;
	private:
		Node *root;
	public:
		AVL() : root(nullptr) {}
		~AVL() {}
		// ham them vao
		///// insert ////
		int getHeightRec(Node*);
		int getHeight();
		Node* rotateLeft(Node*&);
		Node* rotateRight(Node*&);
		Node* leftbalance(Node*&, bool&);
		Node* rightbalance(Node*&, bool&);
		void insertRec(Node*& , int,int, bool&);
		//// deleteNode
		Node* deleteRightBalance(Node *&root, bool &shorter);
		Node* deleteLeftBalance(Node *&root, bool &shorter);
		Node* deleteNodeRec(Node *&root, int value, bool &success, bool &shorter);
		///// searching////
		int searchRec(Node*root,int value);
		///// printing ////
		void printInorderRec(Node*root,ReplacementPolicy* q); // LNR
		void printPreorderRec(Node*root,ReplacementPolicy* q); // NLR
		// ham chuong trinh co san
		void insert(int key,int i);
		void deleteNode(int key);
		void print(ReplacementPolicy* q) {
			cout<<"Print AVL in inorder:\n";
			this->printInorderRec(this->root,q);
			cout<<"Print AVL in preorder:\n";
			this->printPreorderRec(this->root,q);
		}
		int search(int key){return searchRec(this->root, key);}
		
};




#endif