#pragma once
#include "RBTree.h"
template <class k>
class myset
{
public:
	struct keyofe
	{
		const K &operator()(K key)
		{
			return key;
		}
	} 
	typedef typename RBTree<K, const K, keyofe>::Interator iterator;
	typedef typename RBTree<K, const K, keyofe>::constInterator const_iterator;
	iterator begin()
	{
		return _t.Begin();
	}
	iterator end()
	{
		return _t.End;
	}
	const_iterator begin() const
	{
		return _t.Begin();
	}
	const_iterator() const
	{
		return _t.End;
	}
	RBTree<const k,k,keyofe> _t;
};
