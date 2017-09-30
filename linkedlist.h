/* Doubly Linked List */
typedef struct Node {

	int data;
	struct Node *next;
	struct Node *prev;
} Node;

typedef Node* NodePtr;

void insert_list(NodePtr *head, NodePtr *tail, int data);
