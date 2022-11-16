#include "RLEList.h"
#include <stdlib.h>
#include <assert.h>

typedef struct node {
    char letter;
    int times_appeared;
    struct node *next;
} *Node;

struct RLEList_t {
    Node head;
    Node tail;
    int size;
};


//implement the functions here
Node NodeCreate(char letter, int times_appeared, Node next) {
    Node ptr = malloc(sizeof(*ptr));
    if (!ptr) {
        return NULL;
    }
    ptr->letter = letter;
    ptr->times_appeared = times_appeared;
    ptr->next = next;
    return ptr;
}

void NodeDestroy(Node node) {
    free(node);
}


RLEList RLEListCreate() {
    RLEList ptr = malloc(sizeof(*ptr));
    if (!ptr) {
        return NULL;
    }
    ptr->head = NULL;
    ptr->tail = NULL;
    ptr->size = 0;
    return ptr;
}

void RLEListDestroy(RLEList list) {
    while (list->head != NULL) {
        Node tmp = list->head->next;
        NodeDestroy(list->head);
        list->head = tmp;
    }
    free(list);
}

RLEListResult RLEListAppend(RLEList list, char value) {
    if (list == NULL) {
        return RLE_LIST_NULL_ARGUMENT;
    }
    if (list->head == NULL) {
        Node new_node = NodeCreate(value, 1, NULL);
        if (!new_node) {
            return RLE_LIST_OUT_OF_MEMORY;
        }
        list->head = new_node;
        list->tail = new_node;
    } else if (list->tail->letter == value) {
        list->tail->times_appeared++;
    } else {
        Node new_node = NodeCreate(value, 1, NULL);
        if (!new_node) {
            return RLE_LIST_OUT_OF_MEMORY;
        }
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->size++;
    return RLE_LIST_SUCCESS;
}

int RLEListSize(RLEList list) {
    if (!list) {
        return -1;
    }
    return list->size;
}

Node RLEListFindNodeAtIndex(RLEList list, int index) {
    assert(index < 0 || index >= RLEListSize(list));

    Node ptr = list->head;
    int total_prev_letters = 0;
    while (index > total_prev_letters + ptr->times_appeared - 1) {
        ptr = ptr->next;
        total_prev_letters += ptr->times_appeared;
    }
    return ptr;
}

RLEListResult RLEListRemove(RLEList list, int index) {
    if (!list) {
        return RLE_LIST_NULL_ARGUMENT;
    }
    if (index < 0 || index >= RLEListSize(list)) {
        return RLE_LIST_INDEX_OUT_OF_BOUNDS;
    }

    list->size--;

    Node ptr = RLEListFindNodeAtIndex(list, index);
    if (ptr->times_appeared > 1) {
        ptr->times_appeared--;
        return RLE_LIST_SUCCESS;
    }
    if (ptr == list->head) {
        list->head = ptr->next;
    } else {
        Node prev_ptr;
        for (prev_ptr = list->head; prev_ptr->next != ptr; prev_ptr = prev_ptr->next);
        prev_ptr->next = ptr->next;
        if (ptr == list->tail) {
            list->tail = prev_ptr;
        }
    }
    NodeDestroy(ptr);
    return RLE_LIST_SUCCESS;
}

char RLEListGet(RLEList list, int index, RLEListResult *result) {
    if (!list) {
        if (result) {
            *result = RLE_LIST_NULL_ARGUMENT;
        }
        return 0;
    }
    if (index < 0 || index >= RLEListSize(list)) {
        if (result) {
            *result = RLE_LIST_INDEX_OUT_OF_BOUNDS;
        }
        return 0;
    }

    if (result) {
        *result = RLE_LIST_SUCCESS;
    }
    Node ptr = RLEListFindNodeAtIndex(list, index);
    return ptr->letter;
}

char *RLEListExportToString(RLEList list, RLEListResult *result) {
    if (!list) {
        if (result) {
            *result = RLE_LIST_NULL_ARGUMENT;
        }
        return NULL;
    }
    char *string = malloc(sizeof(char) * (RLEListSize(list) + 1));
    if (!string) {
        if (result) {
            *result = RLE_LIST_OUT_OF_MEMORY;
        }
        return NULL;
    }

    int curr_index = 0;
    for (Node ptr = list->head; ptr != NULL; ptr = ptr->next) {
        for (int i = 0; i < ptr->times_appeared; i++) {
            string[curr_index++] = ptr->letter;
        }
    }
    string[RLEListSize(list)] = '\0';

    if (result) {
        *result = RLE_LIST_SUCCESS;
    }
    return string;
}

RLEListResult RLEListMap(RLEList list, MapFunction map_function) {
    if (!list || !map_function) {
        return RLE_LIST_NULL_ARGUMENT;
    }
    for (Node ptr = list->head; ptr != NULL; ptr = ptr->next) {
        ptr->letter = map_function(ptr->letter);
    }
    return RLE_LIST_SUCCESS;
}



