#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int pid;
    char letter;
    double vruntime;
    struct Node *next;
}Node;

typedef struct Queue {
    Node *front;
}Queue;

Node* initNode(int pid, char letter, double vruntime) {
    Node* newNode = (Node *)malloc(sizeof(Node));
    newNode->next = NULL;
    newNode->pid = pid;
    newNode->letter = letter;
    newNode->vruntime = vruntime;
    return newNode;
}

int isEmpty(Queue *q) {
    if(q->front == NULL) return 1;
    else return 0;
}

void enQueue(Queue *q, Node *newNode) {
    if(q->front == NULL){
        q->front = newNode;
        return;
    }
    Node *tempNode = q->front;
    Node *prevTempNode = NULL;
    
    while(tempNode != NULL){
        if(newNode->vruntime < tempNode->vruntime){
            if(prevTempNode == NULL){
                q->front = newNode;
            }else{
                prevTempNode->next = newNode;
            }
            newNode->next = tempNode;
            return;
        }
        prevTempNode = tempNode;
        tempNode = tempNode->next;
    }
    prevTempNode->next = newNode;
}

Node* deQueue(Queue *q) {
    Node* tempNode = q->front;
    
    if(tempNode != NULL){
        q->front = tempNode->next;
    }
    tempNode->next = NULL;
    return tempNode;
}

int main(int argc, char *argv[]) {
    Queue* que = (Queue *)malloc(sizeof(Queue));
    int i = 0;
    for(i=0; i<5; i++){
        enQueue(que, initNode(i, 65+i, 5-i));
    }
    
    for(i=0; i<10; i++){
        Node *node = deQueue(que);
        node->vruntime += 2;
        enQueue(que, node);
    }
    
    return -1;
}
