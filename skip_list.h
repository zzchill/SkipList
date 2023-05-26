#ifndef __SKIP_LIST_H__
#define __SKIP_LIST_H__

#include <stdint.h>

#define MAX_SEARCH_INDEX   64
#define CONTAIN_OF(addr, struct_type, item_name)     ((struct_type *)((char *)(addr) - (char*)&(((struct_type *)0)->item_name)))

typedef struct skip_list_node
{
    struct skip_list_node *next;
    struct skip_list_node *down;
    int key;
} skip_list_node_t;


typedef struct skip_list
{
    skip_list_node_t *level_index;           //top of index
    int_fast64_t max_level;
    // void (*add_node)(uint32_t key);
    // void (*delete_node)(uint32_t key);
    // void (*find_node)(uint32_t key);
} skip_list_t;

skip_list_t *creat_skip_list(void);
skip_list_node_t *find_node(int key, skip_list_t *list);
int add_node(int key, skip_list_t *list);
int delete_node(int key, skip_list_t *list);
int change_node_key(int key, int new_key, skip_list_t *list);
void clear_list(skip_list_t *list);
int rand_creat(int min, int max);

#endif