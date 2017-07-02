/* 
 * This is a implementation of a B-Tree structure capable of insertions and print of its contents
 * for an avaliation work for data structures class in Universidade Federal of Fronteira Sul (UFFS)
 * it uses a preemtive split, so its order must be a even number
 * Author: Maurício Catanio;
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ORDER 4									//By definition order = 2*t
#define MAX_SUB_TREES ORDER
#define MAX_KEYS (ORDER-1)
#define MIDDLE_KEY_ARRAY ((MAX_KEYS/2))			//The middle position in the key array
#define MIDDLE_SUB_ARRAY (ORDER/2)
#define MIN_SUB_TREES (ORDER/2)
#define MIN_KEYS (MIN_SUB_TREES-1)


/* STRUCTURES */
typedef struct Node {
	int *key;							//Array of keys or data
	struct Node *parent;				//superior page
	struct Node **sub;					//Sub-Tree pointers array
	int num_keys;						//Current number of keys in the page
	bool isLeaf;						//true = leaf node, false = internal node
} Node;

typedef struct BTree {
	struct Node *root;
} BTree;

/* END STRUCTURES */

/* AUXILIARY FUNCTION HEADERS */
Node *createNode(Node *new);
int numOFSubTrs (Node *node);
bool isFull(Node *node);
int insertAndSort(Node *node, int key);

void insertOption(BTree *tree);
void printOption(BTree *tree);
void printPage(Node *node);
void printTree(Node *node);
/* END OF AUXILIARY FUNCTION HEADERS */


/* BTREE FUNCTION HEADERS */
BTree *initialize();
void promote(Node *node);
void split(Node *node);
void insertKey(Node *node, int key);
/* END OF BTREE FUNCTION HEADERS */

void main() {
	BTree *tree = initialize();
	int option;


		do{
			system("cls||clear");
			printf("\nB-Tree\n0- Exit\n1- Insert\n2- Show Tree\n");
			scanf("%d", &option);

			if ( option == 1 ) {
				insertOption(tree);
			}

			else if ( option == 2){
				printOption(tree);
			}

			else if ( option == 0) return;
	}while (1);
}

/* AUXILIARY FUNCTIONS  */
Node *createNode(Node *new) {					//Creates an empty node 

	new = (Node *) malloc (sizeof(Node));			
	new->key = (int *) malloc ((ORDER-1)*sizeof(int));								
	new->num_keys = 0;
	new->isLeaf = true;
	new->parent = NULL;
	new->sub = (Node **) malloc (MAX_SUB_TREES*sizeof (Node *));
	for (int i = 0; i < MAX_SUB_TREES; i++) {
		new->sub[i] = NULL;
	}
	return new;
}

int numOFSubTrs (Node *node) {					//Returns the current number of sub-tree pointers
	return (node->num_keys) + 1;
}

bool isFull(Node *node) {						//Verify if the page is full
	if (node->num_keys == MAX_KEYS ) {
		return true;
	}
	else return false;
}
int insertAndSort(Node *node, int key) {
		int aux, left_shifts = 0;
		node->key[node->num_keys] = key;		//insert in the first non-used position of the array
		
		for(int i = node->num_keys; i > 0; i--) {	//little insertion sort
			if (node->key[i] < node->key[i-1]) {
				aux = node->key[i-1];
				node->key[i-1] = node->key[i];
				node->key[i] = aux;
				left_shifts++;
			}
		}

		node->num_keys++;							//adds one to the total amount of keys stored on node after the adition
		return left_shifts;
}

void insertOption(BTree *tree) {
	int key;
	printf("Value of the key to be inserted: ");
	scanf("%d", &key);

	if ( tree->root == NULL) {								//if it is an empty tree
		
		Node *new = createNode(new);
		insertKey(new, key);
		tree->root = new;
	}
	else {
		insertKey(tree->root, key);							
		if (tree->root->parent != NULL) {					//after inserting in a non-empty tree, check if the
			Node *aux;										//tree root is the actual root;
			for (aux = tree->root; aux->parent != NULL; aux = aux->parent);
			tree->root = aux;
		}
	}
}


void printOption(BTree *tree) {
	printTree(tree->root);
}
void printPage(Node *node) {
		for (int i = 0; i < node->num_keys; i++) {
		printf(" %d ", node->key[i]);
	}
}

void printTree(Node *node) {
	if (node == NULL){
		printf(" ");
		return;
	}
	int i;
	printf("(");
	for (i = 0; i < node->num_keys; i++) {
		printTree(node->sub[i]);
		printf("%d", node->key[i]);
	}
	printTree(node->sub[i]);
	printf(")");
}
/* END OF AUXILIARY FUNCTIONS */

/* BTREE FUNCTIONS */
BTree *initialize() {
	BTree *tree = (BTree *) malloc (sizeof(BTree));
	tree->root = NULL;
	return tree;
}

void promote(Node *node);//three cases of promotion: left, middle, right
void split(Node *node); //Creates two new pages with half width-1 of the node passed
void insertKey(Node *node, int key){
//check if the courent page is leaf
	//if it is, insert
	//if it isnt, travel through the adequate path
//after the insertion, check if it's full
	//being full, split and promote the middle key

	
	//search and insertion part

	if (node->isLeaf) {								//check if the courrent node(page) is a leaf node
		insertAndSort(node, key);
	}
	
	else {											//iftsn't, travel throught the adequate path
		if (key < node->key[0]){
			insertKey(node->sub[0], key);
		}
		else{
			for (int i = node->num_keys-1; i >= 0; i--){
				if (key > node->key[i]){
					insertKey(node->sub[i+1], key);
					break;
				}
			}
		}
	}


	//upgrade part
	//the courrent node is splitted and stays only with the left-values, creates a new separeted right-node
	if (node->num_keys == MAX_KEYS) {
		

		Node *right = createNode(right);		//create a new page for the splitted node
		right->parent = node->parent;

		printf("pre-upgrade\nNode = ");
		printPage(node);
		if (node->parent != NULL ) {
			printf("\nparent = ");
			printPage(node->parent);
		} else printf("\nempty parent");
		printf("\n--------\n");
		if (node->parent != NULL) {					//if the superior node already exists
			Node *aux;
			int left_shifts;
			left_shifts =  insertAndSort(node->parent, node->key[MIDDLE_KEY_ARRAY]);
			node->parent->sub[node->parent->num_keys] = right;

			for (int i = 0; i < left_shifts; i++ ) {
				aux = node->parent->sub[node->parent->num_keys-i];
				node->parent->sub[node->parent->num_keys-i] = node->parent->sub[node->parent->num_keys-i-1];
				node->parent->sub[node->parent->num_keys-i-1] = aux;
			}
		}
		else {										//if superiorNode == NULL - creates a new node for root;
			Node *new_root = createNode(new_root);
			insertAndSort(new_root, node->key[MIDDLE_KEY_ARRAY]);		//new_root recives the middle key of the full node
			new_root->isLeaf = false;
			new_root->sub[0] = node;
			new_root->sub[1] = right;

			node->parent = new_root;
			right->parent = new_root;
		}
		for (int i = MIDDLE_KEY_ARRAY+1; i < MAX_KEYS; i++){			//coppy the last half keys to the new right page
			right->key[i%(MIDDLE_KEY_ARRAY+1)] = node->key[i];			//that solution uses a modular of the middle vector as the total amount of space for the right new node
		}
		if (node->isLeaf == false) {							//That treats the case of the new nodes not being leafs
			right->isLeaf = false;

			for (int i = MIDDLE_SUB_ARRAY; i < MAX_SUB_TREES; i++){			//coppy the last half pointers to the new right page
				right->sub[i%(MIDDLE_SUB_ARRAY)] = node->sub[i];		
				node->sub[i]->parent = right;								//that repairs the parent pointer of the now-separated pages
			}
			for (int i = MIDDLE_SUB_ARRAY; i < MAX_SUB_TREES; i++){			//turns into NULL pointers the second half of the left page
				node->sub[i] = NULL;		
			}
		}
		node->num_keys = MIDDLE_KEY_ARRAY;
		right->num_keys = MIDDLE_KEY_ARRAY;

		printf("post-upgrade\nleft= ");
		printPage(node);
		printf("\nright = ");
		printPage(right);
		printf("\nLeft parent = ");
		printPage(node->parent);
		printf("\nright parent = ");
		printPage(right->parent);
		printf("\n\n\n");


	}
}
/* END OF BTREE FUNCTIONS  */
