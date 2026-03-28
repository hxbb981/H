#pragma once
#include<iostream>
#include<assert.h>
using namespace std;

template<class T>
class creatavl{
public:
	AVL(T date):
		key(date),
		parent(nullptr),
		Lchild(nullptr),
		Rchild(nullptr){}
private:
	T key;
	creatavl<T>*parent;
	creatavl<T>*Lchild;
	creatavl<T>*Rchild;
	int _be;
}



template<class T>
class AVL{
typedef creatavl<T> Node;
public:
	AVL():
		root(nullptr){}
	bool insert(int date){
		if(root==nullptr){
			root=new Node(date);
			return true;
		}	
		else{
			Node*cur=root;
			Node*parent=nullptr;
			while(cur){
				if(date<cur->key){
					parent=cur;
					cur=cur->Lchild;
				}
				else if(date>=cur->key){
					parent=cur;
					cur=cur->Rchild;
				}
				else{
					assert(nullptr);
					return false;
				}
				cur=new Node(date);
				if(date<parent->key){
					parent->Lchild=cur;
				}
				else{
					parent->Rchild=cur;
				
				}
				return true;
			}
		}
		//控制平衡，更新自己的平衡因子
		//所谓平衡树，我们就是要保证左右树高度之插不能大于一，则旋转处理
		while(parent){
			if(cur==parent->Lchild)
				parent->_be--;
			else
				parent->_be++;
			if(parent->_be==0){
				break;
			}
			else if(parent->_be==1||parent->_be==-1){
				cur=parent;
				parent=parent->parent;
			}
			else{
				//我们应该旋转控制平衡
				if(parent->_be==-2&&parent->Lchild->_be==-1){
					RotateR(parent);
				}
				else if(parent->_be==2&&parent->Rchild->_be==1){
					RotateL(parent);	
				}
				else if(parent->_be==-2&&parent->Lchild->_be==1){
					RotateLR(parent);
				}
				else if(parent->_be==2&&parent->Rchild->_be==-1){
					RotateRL(parent);
				}
				else{
					assert(false);
				}
			}
		}
	}
		
bool RotateR(Node*parent){

	Node*Subl=parent->Lchild;
	Node*SublR=Subl->Rchild;

	Subl->Rchild=parent;
	parent->Lchild=SublR;

	if(SublR)
		SublR->parent=Subl;
	Subl->parent=parent;
	//与上面连接
	if(parent==root){
		Subl=root;
		Subl->parent=nullptr;
		
	}
	else{
		
		Subl->parent=parent->parent->Lchild;
	}
	parent->parent=Subl;
	//更新平衡后的树的—be,只有Subl与parent发生变化
	Subl->_be=0;
	parent->_be=0;
	
	
	return true;
}
bool RotateL(Node*parent){
	Node*Subr=parent->Rchil;
	Node*SubrR=Subr->Lchild;
	//处理孩子

	Subr->Lchild=parent;
	parent->Rchild=SubrR;
	//处理母亲,不着急改parent
	if(SubrR){
		SubrR->parent=parent;
	}
	if(Subr==root){
		Subr->parent=nullptr;
	}
	else{
		Subr->parent=parent->parent->Rchild;
	}
	parent->parent=Subr;
	Subr->_be=0;
	parent->_be=0;
	return true;
}

bool RotateLR(Node*parent){
	Node*Sublr=parent->Lchild;
	Node*SublrR=Sublr->Rchild;
	//这个be有大用处
	int bf=SublrR->_be;
	//分别进行右旋与左旋
	
	RotateR(Sublr);
	RotateL(parent);

	//更新树的平衡点—be
	
	if(bf==0){
		parent->_be=0;
		Sublr->_be=0;
		SublrR->_be=0
	}
	else if(bf==-1){
		parent->_be=0;
		Sublr->_be=0;
		SublrR->_be=1;
	}
	else if(bf==1){
		parent->_be=-1;
		Sublr->_be=0;
		SublrR->_be=0;
	}
	else{
		assert(false);//习惯
	}
	return true;
}
bool RotateRL(Node*parent){
	Node*Subrl=parent->Rchild;
	Node*SubrlR=Subrl->Lchild;
	//记录SubrlR的平衡因子便于区分
	
	int be=SubrlR->_be;

	//先进行左旋再进行右旋
	
	RotateL(Subrl);
	RotateR(parent);

	//调整平衡因子
	
	if(be==-1){
		parent->_be=-1;
		Subrl->_be=0;
		SubrlR->_be=1;
	}
	else if(be==1){
		parent->_be=-1;
		Subrl->_be=0;
		SubrlR->_be=0;
	}
	else if(be==0){
		parent->_be=0;
		Subrl->_be=0;
		SubrlR->_be=0;
	}
	else{
		assert(false);
	}
}






Node*find(int date){
	Node*cur=root;
	while(cur){
		if(date<cur->key){
			cur=cur->Lchild;
		}
		else if(date>cur->key){

			cur=cur->Rchild;
		}
		else{
			return cur;
		}
	}
	return nullptr;
}



//还未考虑空的情况
bool erase(int date){
	if(!find(date)){
		return false;
	}
	else{
		Node*cur=find(date);
		Node*parent=cur->parent;
		if(cur->Lchild==nullptr&&cur->Rchild!=nullptr){
			if(parent==nullptr){
				root=cur->Rchild;
				root->parent=nullptr;
			}
			if(parent->Lchild==cur){
				parent->Lchild=cur->Rchild;
			}
			else if(parent->Rchild==cur){
				parent->Rchild=cur->Rchild;
			}
		}
		else if(cur->Lchild!=nullptr&&cur->Rchild==nullptr){
			if(parent==nullptr){
				root=cur->Lchild;
				root->parent=nullptr;
			}
			if(parent->Lchild==cur){
				parent->Lchild=cur->Lchild;
			}
			else if(parent-Rchild==cur){
				parent->Rchild=cur->Lchild;
			}
		}
		else if(cur->Lchild==nullptr&&cur->Rchild==nullptr){
			if(parent==nullptr){
				root=nullptr;
			}
			if(parent->Lchild==cur){
				parent->Lchild=nullptr;
			}
			else parent->Rchild=nullptr;
		}
		else if(cur->Lchild!=nullptr&&cur->Rchild==nullptr){
			Node*Lftmin=cur->Rchild;//直接排除删除结点为空的情况
			Node*Lftminparent=cur;
		
			while(Lftmin->Lchild){
				Lftminparent=Lftmin;
				Lftmin=Lftmin->Lchild;
			}
			cur->key=Lftmin->key;
			if(lftminparent->Lchild=lftmin){
				lftminparent->Lchild=lftmin->Rchild;
			}
			else{
				lftminparent->Rchild=lftmin->Rchild;
			}
			delete lftmin;
		}
		return true;
	}
}
//添加每个结点的度
private:
	Node*root;
}




