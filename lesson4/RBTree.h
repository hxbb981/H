#pragma once

#include <iostream>
#include <assert.h>
using namespace std;
enum color
{
	Red,
	Black
};
template <class T>
struct RBTreenode
{
public:
	RBTreenode(T key) : date(key),
						cur(nullptr),
						parent(nullptr),
						Lchild(nullptr),
						Rchild(nullptr),
						_bef(0) {}

private:
	RBTreenode *cur;
	RBTreenode *parent;
	RBTreenode *Lchild;
	RBTreenode *Rchild;
	int _bef;
	T date;
	color _col;
};
// 中序遍历
template <class T, class ref, class ptr>
class RBTreeInterator
{
	typedef RBTreeInterator<class T, class ref, class ptr> self;
	typedef RBTreenode<T> node;
	node *Root;
	node *_node;
	RBTreeInterator(node *root, node *node)
		: Root(root), _node(node) {}
	self operator++()
	{
		node *cur = _node;
		if (cur->Rchild)
		{
			node *min = cur->Rchild;
			while (min->Lchild)
			{
				min = min->Lchild;
			}
			_node = min;
		}
		else
		{
			node *parent = cur->parent;
			while (parent && cur == parent->Rchild)
			{
				cur = parent;
				parent = parent->parent;
			}
			_node = cur;
		}
		return *this;
	}
	self operator--()
	{
		if (_node == nullptr)
		{
			node *most = Root;
			while (most - Rchild)
			{
				most = most->Rchild;
			}
			_node = most;
		}
		else if (_node->Lchild)
		{
			node *most = _node->Lchild;
			while (most)
			{
				most = most->Rchild;
			}
			_node = most;
		}
		else
		{
			node *most = _node->parent;
			node *nnode = _node;
			while (most && most->Lchild == nnode)
			{
				nnode = most;
				most = most->parent;
			}
			_node = most;
		}
		return *this;
	}
	// 引用与地址
	ref operator*()
	{
		return _node->date;
	}
	ptr operator->()
	{
		return &_node->date;
	}
	bool operator==(const self &cur)
	{
		return _node == cur->_node;
	}
};
template <class K, class T, class keyot>
class RBTree
{
	typedef RBTreenode<T> node;
	typedef RBTreeInterator<T, T &, T *> Interator;
	typedef RBTreeInterator<T, const T &, const T *> constInterator;

public:
	RBTreenode() : root(nullptr) {}
	Interator Begin()
	{
		node *cur = root;
		while (cur && cur->Lchild)
		{
			cur = cur->Lchild;
		}
		return Interator(root, cur)
	}

	Interator End()
	{
		return Interator(root, root);
	}
	constInterator Begin() const
	{
		node *cur = root;
		while (cur && cur->Lchild)
		{
			cur = cur->Lchild;
		}
		return constInterator(root, cur)
	}
	constInterator End() const
	{
		return constInterator(root, root);
	}
	pair<Interator, bool> insert(T key)
	{
		if (root == nullptr)
		{
			root = new node(key);
			root->_col = Black;
			return {Interator<root, root>, true};
		}
		keyot ke ;
		node *cur = root;
		node *parent = nullptr;
		else
		{
			while (cur)
			{
				if (ke(cur->date) > ke(key))
				{
					parent = cur;
					cur = cur->Lchild;
				}
				else if (ke(cur->date) <= ke(key))
				{
					parent = cur;
					cur = cur->Rchild;
				}
				else
				{
					assert(false);
				}
			}
			cur = new node(key);
			cur->_col = Red;
			if (ke(cur->date) > ke(key))
			{
				parent->Lchild = cur;
			}
			else
			{
				parent->Rchild = cur;
			}
			return {Interator(root, cur), true};
		}
		// 调整红黑树
		node *gfather = parent->parent;
		node *newnode = cur;
		if (parent == gfather->Lchild)
		{
			// g
			//  p   u
			// c
			while (parent && parent->_col == Red)
			{
				node *uncle = gfather->Rchild;
				if (uncle && uncle->_col == Red)
				{
					parent->_col = uncle->_col = Black;
					gfather->_col = Red;
					cur = gfather;
					parent = cur->parent;
					gfather = parent->parent; // 还需要往上处理
				}
				else
				{
					if (cur = parent->Lchild)
					{
						RotateR(gfather);
						parent->_col = Black;
						gfather->_col = Red; // 不需要再往上处理
					}
					else
					{

						RotateL(parent);
						RotateR(gfather);
						cur->_col = Black;
						gfather->_col = Red;
						break;
					}
				}
			}
		}

		else
		{
			while (parent &&parent->_col = Red)
			{
				if (uncle &&uncle->_col = Red)
				{
					uncle->_col = parent->_col = Black;
					gfather->_col = Red;
					cur = gfather;
					parent = gfather->parent;
					gfather = parent->parent;
				}
				else
				{
					if (cur = parent->Rchild)
					{
						RotateL(gfather)
							parent->_col = Black;
						gfather->_col = Red;
					}
					else
					{
						RotateR(parent);
						RotateL(gfather);
						parent->_col = Black;
						gfather->_col = Red;
					}
					break;
				}
			}
		}
		root->_col = Black;
		return {Interator(cur, root), true};
	}
	bool RotateL(Node *parent)
	{
		Node *Subr = parent->Rchil;
		Node *SubrR = Subr->Lchild;
		// 处理孩子

		Subr->Lchild = parent;
		parent->Rchild = SubrR;
		// 处理母亲,不着急改parent
		if (SubrR)
		{
			SubrR->parent = parent;
		}
		if (Subr == root)
		{
			Subr->parent = nullptr;
		}
		else
		{
			Subr->parent = parent->parent->Rchild;
		}
		parent->parent = Subr;
		Subr->_be = 0;
		parent->_be = 0;
		return true;
	}

	bool RotateR(Node *parent)
	{

		Node *Subl = parent->Lchild;
		Node *SublR = Subl->Rchild;

		Subl->Rchild = parent;
		parent->Lchild = SublR;

		if (SublR)
			SublR->parent = Subl;
		Subl->parent = parent;
		// 与上面连接
		if (parent == root)
		{
			Subl = root;
			Subl->parent = nullptr;
		}
		else
		{

			Subl->parent = parent->parent->Lchild;
		}
		parent->parent = Subl;
		// 更新平衡后的树的?be,只有Subl与parent发生变化
		Subl->_be = 0;
		parent->_be = 0;

		return true;
	} // 并没有改变parent的位置
	node *Find(T key)
	{
		node *cur = root;
		while (cur)
		{
			if (cur->date > key)
			{
				cur = cur->Lchild;
			}
			else if (cur->date < key)
			{
				cur = cur->Rchild;
			}
			else
			{
				return cur;
			}
		}
		return nullptr;
	}
	bool Rcheck()
	{
		node *cur = root;
		int re = 0;
		while (cur)
		{
			if (cur->_col == Black)
			{
				re++;
			}
			cur = cur->Lchild;
		}
		node *cur = root;
		bool really = check(cur, 0, re);
		return really;
	}
	int Treeheight()
	{
		node *cur = root;
		return tall(cur);
	}
	bool RBTprint()
	{
		node *cur = root;
		if (cur == nullptr)
		{
			return true;
		}
		RBTprint(cur->Lchild);
		cout << cur->date << endl;
		RBTprint(cur->Rchild);
	}

private:
	bool RBTprint(node *cur)
	{
		if (cur == nullptr)
		{
			return true;
		}
		RBTprint(cur->Lchild);
		cout << cur->date << endl;
		RBTprint(cur->Rchild);
	}
	bool check(node *cur, int num, int ref)
	{
		// 说明一条路径已经走完了
		node *parent = cur->parent;
		if (cur == nullptr)
		{
			if (num != ref)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		// 检测孩子和自己不好查找，直接检测自己与母亲
		if (cur->_col == Red && cur->_col == Red)
		{
			return false;
		}
		num++;
		return check(cur->Lchild, num, ref) && check(cur->Rchild, num, ref); // 注意只要一个函数递归出来的是false则递归上来的所有函数都是false
	}
	int tall(node *cur)
	{
		if (cur == nullptr)
		{
			return 0;
		}
		int Lheight = tall(cur->Lchild);
		int Rheight = tall(cur->Rchild);
		return Lheight > Rheight ? Lheight++ : Rheight++;
	}
	node *root;
};
