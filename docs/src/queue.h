#ifndef __EVENT_H__
#define __EVENT_H__

#include <stdio.h>
#include <raylib.h>
#include <stdbool.h>

#define VECTOR3_QUEUE_SIZE 100

typedef struct queue_t {
    int front, back, size;
    Vector3 events[VECTOR3_QUEUE_SIZE];
} vector3_queue_t;

void vector3_queue_init(void);
bool vector3_queue_is_empty(void);
bool vector3_queue_is_full(void);
int vector3_queue_enqueue(Vector3 vector);
Vector3 vector3_queue_dequeue(void);
Vector3 vector3_queue_get_element_at(int index);
int vector3_queue_get_size();


static vector3_queue_t queue = {0};

int vector3_queue_get_size(){
    return queue.size;
}

void vector3_queue_init() {
    queue.size = 0;
    queue.back = queue.front = -1;
}

bool vector3_queue_is_empty() {
    return queue.front == -1;
}

bool vector3_queue_is_full() {
    return (queue.front == queue.back + 1) || (queue.front == 0 && queue.back == VECTOR3_QUEUE_SIZE -1);
}

int vector3_queue_enqueue(Vector3 event) {
    if(vector3_queue_is_full()) return -1;
    
    if(queue.front == -1) queue.front = 0;

    queue.back = (queue.back + 1) % VECTOR3_QUEUE_SIZE;

    queue.events[queue.back] = event;
    queue.size++;
    return 0;
}

Vector3 vector3_queue_dequeue() {
    Vector3 return_value = {0};
    if(vector3_queue_is_empty()) return return_value;
    
    return_value = queue.events[queue.front];

    if(queue.front == queue.back)
        queue.back = queue.front = -1;
    else 
        queue.front = (queue.front + 1) % VECTOR3_QUEUE_SIZE;
    queue.size--;
    return return_value;
}

Vector3 vector3_queue_get_element_at(int index){
    return queue.events[(queue.front + index) % VECTOR3_QUEUE_SIZE];
}

#undef VECTOR3_QUEUE_SIZE


#endif

