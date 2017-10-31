/* Doubly Linked List */
typedef struct Node {

	int data;
	struct Node *next;
	struct Node *prev;
} Node;

typedef Node* NodePtr;

void insert_tail(NodePtr *head, NodePtr *tail, int data);

void insert_head(NodePtr *head, NodePtr *tail, int data);

void delete_list(NodePtr *head, NodePtr *tail, NodePtr node);
