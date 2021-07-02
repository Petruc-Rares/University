#include "helpers.h"

int main() {
    Info info;
    info.id = 0;

    Node *node1 = createNode(info);
    info.id = 1;
    Node *node2 = createNode(info);
    info.id = 3;
    Node *node3 = createNode(info);

    Node *list = NULL;
    addFrontNode(&list, node1);
    addFrontNode(&list, node2);
    addFrontNode(&list, node3);

    printList(list);

    deleteNode(&list, node1);
    printList(list);

    deleteNode(&list, node3);
    printList(list);

    deleteNode(&list, node2);
    printList(list);

    info.id = 10;
    Node *node4 = createNode(info);
    addFrontNode(&list, node4);
    printList(list);
  
    deleteNode(&list, node4);
}