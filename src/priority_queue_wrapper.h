
#ifdef __cplusplus
extern "C" {
#endif

    typedef struct pq_node {
	long double priority;
	unsigned long long lastuse;
	char* key;
    } pq_node;
    
    void pq_find_and_remove(void *v_pq, pq_node* to_find);
    void pq_insert(void *v_pq, pq_node* to_add);
    pq_node* pq_get_lowest(void *v_pq);
    void* create_queue();
    pq_node* pq_make_node( char* key, long double priority, unsigned long long lastuse );
    void pq_free_node( pq_node* );
    void pq_erase_queue( void* v_pq );
    
#ifdef __cplusplus
}
#endif
