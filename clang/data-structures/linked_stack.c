#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "assert.h"

struct LinkedStack {
    struct Frame* top;
};

struct Frame {
    char* value;
    struct Frame* next;
};

struct LinkedStack new() {
    struct LinkedStack stack;
    stack.top = NULL;
    return stack;
}

void push(struct LinkedStack* stack, char* item) {
    struct Frame* frame = (struct Frame*)malloc(sizeof(struct Frame));
    frame->value = item;
    frame->next = NULL;

    if(stack->top == NULL) {
        stack->top = frame;
        return;
    }

    frame->next = stack->top;
    stack->top = frame;
}

char* peek(struct LinkedStack* stack) {
    if(stack->top == NULL) {
        return NULL;
    }

    return stack->top->value;
}

char* pop(struct LinkedStack* stack) {
    if(stack->top == NULL) {
        return NULL;
    }

    char* popped = stack->top->value;
    stack->top = stack->top->next;
    return popped;
}

int size(struct LinkedStack* stack) {
    if(stack->top == NULL) {
        return 0;
    }

    int size = 0;
    struct Frame* frame = stack->top;
    while(frame != NULL) {
        size += 1;
        frame = frame->next;
    }
    return size;
}

void print(struct LinkedStack* stack) {
    if(stack->top == NULL) {
        printf("Stack is empty");
        return;
    }

    struct Frame* frame = stack->top;
    printf("%s\n", frame->value);
    while(frame->next != NULL) {
        frame = frame->next;
        printf("%s\n", frame->value);
    }
}

int main() {
    struct LinkedStack stack = new();

    printf("Peeked: %s\n", peek(&stack));
    printf("Popped: %s\n", peek(&stack));
    push(&stack, "First add");
    push(&stack, "Second add");
    push(&stack, "Third add");
    print(&stack);
    printf("Size: %d\n", size(&stack));
    printf("Peeked: %s\n", peek(&stack));
    printf("Peeked: %s\n", peek(&stack));
    printf("Size: %d\n", size(&stack));
    printf("Popped: %s\n", pop(&stack));
    printf("Size: %d\n", size(&stack));
    printf("Peeked: %s\n", peek(&stack));

    return 0;
}