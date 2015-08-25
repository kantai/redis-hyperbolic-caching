#include <string.h>
#include <stdio.h>
#include <set>
#include "priority_queue_wrapper.h"

struct pq_node_compare {
    bool operator() (const pq_node* a, const pq_node* b) const{
	if (a->priority < b->priority)
	    return true;
	if (a->priority == b->priority){
	    if (a->lastuse < b->lastuse)
		return true;
	    if (a->lastuse == b->lastuse){
		if (strcmp(a->key, b->key) != 0)
		    printf("!!");
	    }
	    return (a->lastuse == b->lastuse && 
		    strcmp(a->key, b->key) < 0);
	}
	return false;
    }
};

typedef std::set<pq_node*, pq_node_compare>  pq_set;

extern "C" void pq_find_and_remove(void *v_pq,
				pq_node* to_find){
    pq_set* pq = (pq_set*) v_pq;
    pq->erase(to_find);
}

extern "C" void pq_insert(void *v_pq,
		       pq_node* to_add){
    pq_set* pq = (pq_set*) v_pq;
    pq->insert(to_add);
}

// get the lowest priority node, and remove it from the queue.
extern "C" pq_node* pq_get_lowest(void *v_pq){
    pq_set* pq = (pq_set*) v_pq;
    pq_node* rval = *pq->begin();
    
    pq_find_and_remove(pq, rval);
    return rval;
}

extern "C" void* create_queue(){
    pq_set* pq = new pq_set();
    
    return (void *) pq;
}

extern "C" pq_node* pq_make_node( char* key, long double priority, unsigned long long lastuse ){
    pq_node* pq = new pq_node;
    pq->key = key;
    pq->priority = priority;
    pq->lastuse = lastuse;
    return pq;
}

extern "C" void pq_free_node( pq_node* pq ) {
    delete pq;
}

extern "C" void pq_erase_queue( void* v_pq ) {
    pq_set* pq = (pq_set*) v_pq;
    pq->erase(pq->begin(), pq->end());
}
