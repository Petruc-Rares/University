#ifndef TRIE_H
#define TRIE_H
#include "stdint.h"

typedef struct TrieNode TrieNode;

struct TrieNode {
    // The Trie Node Structure
    // Each node has N children, starting from the root
    // and a flag to check if it's a leaf node
    struct route_table_entry *routing_info;
    TrieNode* child_left;
    TrieNode* child_right;
};

extern TrieNode* make_trienode(struct route_table_entry *routing_info);
 
extern TrieNode* insert_trie(TrieNode* root, struct route_table_entry *r);

 
/*
 Returns a pointer (eg. &rtable[i]) to the best matching route
 for the given dest_ip. Or NULL if there is no matching route.
*/
extern struct route_table_entry* search_trie(TrieNode* root, uint32_t destIp);

#endif