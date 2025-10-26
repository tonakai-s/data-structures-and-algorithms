#include "stdlib.h"
#include "stdio.h"

struct LinkedQueue {
    struct Node* head;
    struct Node* tail;
};

struct Node {
    char* value;
    struct Node* next;
};

struct LinkedQueue new() {
    struct LinkedQueue queue = { .head = NULL, .tail = NULL};
    return queue;
}

void enqueue(struct LinkedQueue* queue, char* item) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->value = item;

    if(queue->tail == NULL) {
        queue->head = queue->tail = node;
        return;
    }
    if(queue->tail == queue->head) {
        node->next = queue->head;
        queue->tail = node;
        return;
    }
    node->next = queue->tail;
    queue->tail = node;
    return;
}

void traverse(struct LinkedQueue* queue) {
    if(queue->tail == NULL) {
        printf("Empty queue");
        return;
    }

    struct Node* node = queue->tail;
    printf("TAIL-->\n");
    printf("%s\n", node->value);
    while(node->next != NULL) {
        node = node->next;
        printf("%s\n", node->value);
    }
    printf("-->HEAD\n");
}

int size(struct LinkedQueue* queue) {
    if(queue->tail == NULL) {
        return 0;
    }
    if(queue->tail == queue->head) {
        return 1;
    }
    int size = 1;
    struct Node* node = queue->tail;
    while(node->next != NULL) {
        size += 1;
        node = node->next;
    }
    return size;
}

void dealloc(struct LinkedQueue* queue) {
    if(queue->tail == NULL) {
        return;
    }

    struct Node* node = queue->tail;
    while(node->next != NULL) {
        struct Node* tmp = node->next;
        free(node);
        node = tmp;
    }
    free(node);
}

int main() {
    struct LinkedQueue queue = new();
    enqueue(&queue, "First item added");
    enqueue(&queue, "Second item added");
    enqueue(&queue, "Third item added");
    traverse(&queue);
    printf("Size: %d\n", size(&queue));

    printf("Deallocating");
    // If manually allocated memory is not freed at the end, it's a memory leak.
    dealloc(&queue);
    return 0;
}