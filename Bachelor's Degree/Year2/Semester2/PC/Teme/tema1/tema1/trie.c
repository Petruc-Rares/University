#include "trie.h"
#include "stddef.h"
#include "stdlib.h"
#include "commonStructures.h"

void int_to_char_array(uint32_t address, char string[]) {
    uint32_t masca = 0x80000000;

    for (int i = 24; i < 32; i++) {
        if ((address & masca) != 0) {
            string[i] = 1;
        } else {
            string[i] = 0;
        }
        masca >>= 1;
    }

    for (int i = 16; i < 24; i++) {
        if ((address & masca) != 0) {
            string[i] = 1;
        } else {
            string[i] = 0;
        }
        masca >>= 1;
    }

    for (int i = 8; i < 16; i++) {
        if ((address & masca) != 0) {
            string[i] = 1;
        } else {
            string[i] = 0;
        }
        masca >>= 1;
    }

    for (int i = 0; i < 8; i++) {
        if ((address & masca) != 0) {
            string[i] = 1;
        } else {
            string[i] = 0;
        }
        masca >>= 1;
    }
    return;
}

int get_mask_size(char mask[]) {
    int mask_size = 32;
    
    for (int i = 31; i >= 0 ; i--) {
        if (mask[i] == 0) {
            mask_size--;
        } else break;
    }
    return mask_size;
} 

TrieNode* make_trienode(struct route_table_entry *routing_info) {
    // Allocate memory for a TrieNode
    TrieNode* root = (TrieNode*) malloc (sizeof(TrieNode));
    root->child_left = NULL;
    root->child_right = NULL;
    root->routing_info = routing_info;
    return root;
}
 
void free_trienode(TrieNode* node) {
    if (node->child_right != NULL) {
        free_trienode(node->child_right);
    }
    
    if (node->child_left != NULL) {
        free_trienode(node->child_left);
    }
    
    free(node->routing_info);
    free(node);
}
 
TrieNode* insert_trie(TrieNode* root, struct route_table_entry *r) {
    TrieNode* temp = root;
 
    char prefix[33];
    int_to_char_array(r->prefix, prefix);

    char mask[33];
    int_to_char_array(r->mask, mask);
    int mask_size = get_mask_size(mask);

    for (int i = 0; i < mask_size; i++) {

        if (prefix[i] == 0) {
            if (temp->child_left == NULL) {
                // daca nu exista, fa-l
                temp->child_left = make_trienode(NULL);
            }
            temp = temp->child_left;
        } else if (prefix[i] == 1) {
            if (temp->child_right == NULL) {
                // daca nu exista, fa-l
                temp->child_right = make_trienode(NULL);
            }
            temp = temp->child_right;
        }
    }
    
    temp->routing_info = r; 
    return root;
}

 
/*
 Returns a pointer (eg. &rtable[i]) to the best matching route
 for the given dest_ip. Or NULL if there is no matching route.
*/
struct route_table_entry* search_trie(TrieNode* root, uint32_t dest_ip) {
    TrieNode* temp = root;
 
    char dest_ip_bits[33];
    int_to_char_array(dest_ip, dest_ip_bits);


    for (int i=0; i < 32; i++) {
        char crtBit = dest_ip_bits[i];

        if (crtBit == 0) {
            if (temp->child_left == NULL) {
                return temp->routing_info;
            } else {
                temp = temp->child_left;
            }
        } else if (crtBit == 1) {
            if (temp->child_right == NULL) {
                return temp->routing_info;
            } else {
                temp = temp->child_right;
            }
        }
    }
    // ajuns aici inseamna ca am ajuns sa coincida toti bitii (masca de 32, mai rar)

    return temp->routing_info;
}