#include "skip_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>

static inline void adding_link_deal(skip_list_node_t *node, skip_list_node_t *pre)
{
    skip_list_node_t *cur = pre->next;
    pre->next = node;
    node->next = cur;
}

static inline void deleting_link_deal(skip_list_node_t *node, skip_list_node_t *pre)
{
    skip_list_node_t *cur = pre->next;
    pre->next = cur->next;
    free(cur);
}

static void delete_line(skip_list_node_t *node)
{
    skip_list_node_t *tmp = NULL;
    while (node)
    {
        tmp = node->next;
        free(node);
        node = tmp;
    }
}
void update_index(int node_len, skip_list_t *list)
{
    skip_list_node_t *node = NULL;
    if (node_len > list->max_level)
    {
        node_len = (node_len > MAX_SEARCH_INDEX) ? MAX_SEARCH_INDEX : node_len;
        for (int i = 0; i < node_len - list->max_level; i++)
        {
            node = list->level_index;
            list->level_index = (skip_list_node_t *)malloc(sizeof(skip_list_node_t));
            list->level_index->down = node;
            list->level_index->next = NULL;
            list->level_index->key = -1;
        }
        list->max_level = node_len;
    }
    else if (node_len < list->max_level)
    {
        int tmp = list->max_level;
        while (list->level_index->down && (node_len++ < tmp))
        {
            node = list->level_index;
            list->level_index = node->down;
            delete_line(node);
            list->max_level--;
        }
        
    }
}

int rand_creat(int min, int max)
{
    HCRYPTPROV hcp;

    unsigned char rdata[4];
    if (!CryptAcquireContextA(&hcp, NULL, "Microsoft Base Cryptographic Provider v1.0", PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
		return -1;		
	}
    CryptGenRandom(hcp, 4, rdata);

    int out = *(int *)rdata;
    out = out % (max - min + 1) + min;
    return out;
}

skip_list_t *creat_skip_list(void)
{
    skip_list_t *list;
    list = (skip_list_t *)malloc(sizeof(skip_list_t));
    list->level_index = (skip_list_node_t *)malloc(sizeof(skip_list_node_t));
    list->max_level = 1;
    list->level_index->next = NULL;
    list->level_index->down = NULL;
    list->level_index->key = -1;
    return list;
}

skip_list_node_t *find_node(int key, skip_list_t *list)
{
    skip_list_node_t *node = list->level_index;
    while (node)
    {
        //printf("key:%d, node->key:%d, node:%x, node->down:%x, node->next:%x\n", key, node->key, node, node->down, node->next);
        if (!node->next)
        {
            node = node->down;
        }
        else if (node->next->key == key)
        {
            //printf("key:%d,node->down:%x, node->next:%x\n", key, node->down, node->next);
            return node;
        }
        else if (node->next->key > key)
        {
            //printf("key:%d,node->down:%x, node->next:%x\n", key, node->down, node->next);
            node = node->down;
        }
        else
        {
            //printf("key:%d,node->down:%x, node->next:%x\n", key, node->down, node->next);
            node = node->next;
        }
    }
    return NULL;
}

int add_node(int key, skip_list_t *list)
{
    // if find the same key in list refuse to add node
    if (find_node(key, list) != NULL)
    {
        return -1;
    }

    int len = 1;
    skip_list_node_t *cur = (skip_list_node_t *)malloc(sizeof(skip_list_node_t));
    skip_list_node_t *top = cur;
    cur->key = key;
    cur->down = NULL;
    cur->next = NULL;
    while ((rand_creat(0, 3)) == 0 && len < MAX_SEARCH_INDEX)
    {
        // printf("creat\n");
        cur->down = (skip_list_node_t *)malloc(sizeof(skip_list_node_t));
        cur = cur->down;
        cur->key = key;
        cur->next = NULL;
        cur->down = NULL;
        len++;
    }

    //update maxlen of list
    if (len > list->max_level)
    {
        update_index(len, list);
    }
    
    int tmp = list->max_level - len;
    cur = list->level_index;
    while (tmp--)
    {
        cur = cur->down;
    }
    
    
    while (top && cur)
    {
        if (cur->next == NULL || (cur->key < key && cur->next->key > key))
        {
            adding_link_deal(top, cur);
            top = top->down;
            cur = cur->down;
        }
        else
        {
            cur = cur->next;
        }
    }

    return 0;
}

int delete_node(int key, skip_list_t *list)
{
    skip_list_node_t *node = find_node(key, list);
    skip_list_node_t *tmp = NULL;
    if (node == NULL)
    {
        return -1;
    }
    
    int size = 0;
    while (node)
    {
        if (node->next == NULL)
        {
            node = node->down;
            continue;
        }
        
        if (node->next->key == key)
        {
            tmp = node->next->next;
            free(node->next);
            node->next = tmp;
            node = node->down;
            size++;
        }
        else
        {
            node = node->next;
        }
    }
    if (size >= list->max_level)
    {
        node = list->level_index;
        while (!node->next && node->down)
        {
            node = node->down;
            size--;
        }
        size = (size < 1) ? (1) : (size);
        update_index(size, list);
    }

    return 0;
}

int change_node_key(int key, int new_key, skip_list_t *list)
{
    if(!delete_node(key, list))
    {
        return -1;
    }
    return add_node(new_key, list);
}

void clear_list(skip_list_t *list)
{
    skip_list_node_t *node = list->level_index;
    while (node)
    {
        list->level_index = list->level_index->down;
        delete_line(node);
        node = list->level_index;
    }
    free(list);
}




