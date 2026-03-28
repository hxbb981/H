
#include"RBTree.h"
class mymap{
	struct mapkof{
		const K&operator()(const pair<K,v> kv ){
			return kv.first
		}
	
	}
	public:
		typedef typename RBTree<K,pair<K,v>,mapkof>::Interator iterator;
		typedef typename RBTree<K,pair<K,v>,mapkof>::constInterator constiterator;
		iterator begin(){
			return Map.Begin();
		}
		interator end(){
			return Map.end();
		}
		constinterator begin()const{
			return Map.begin();
		}
		constinterator end()const{
			return Map.end();
		}
		pair<interator,bool> insert(pair<K,v> kv){
			return Map.insert(kv);
		}


	


	private:
		RBTree<K,pair<K,v>,mapkof> Map;




}

