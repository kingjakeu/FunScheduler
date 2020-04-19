typedef struct Node {
    pid_t pid;
    int nice;
    double vruntime;
    struct Node *next;
}Node;

typedef struct Queue {
    Node *front;
}Queue;

Node* initNode(pid_t pid, int nice, double vruntime);
int isEmpty(Queue *q);
void enQueue(Queue *q, Node *newNode);
Node* deQueue(Queue *q);
int calVruntime(Node *n);
void terminate();
void handler(int sig);
