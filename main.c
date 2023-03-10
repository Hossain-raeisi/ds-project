#include <stdio.h>
#include <malloc.h>
#include <string.h>


// UTILS ---------------------------------------------------------------------------------------------------------------
#define for_each(element, list) \
    for(ListElement *element = list->first; element != NULL; element = (ListElement *)element->next)

int MAX_VALUE = 100;
int S = -2;

int max(int a, int b) {
    return (a > b) ? a : b;
}

int compareInt(int *a, int *b) {
    if (*a == *b)
        return 0;
    else if (*a > *b)
        return 1;

    return -1;
}

typedef struct {
    int phase2;
} Context;

Context *NewContext(int phase2);

Context *NewContext(int phase2) {
    Context *context = (Context *) malloc(sizeof(Context));

    context->phase2 = phase2;

    return context;
}

float minArray(float *array, int size) {
    float minValue = array[0];

    for (int i = 0; i < size; i++) {
        if (array[i] < minValue)
            minValue = array[i];
    }

    return minValue;
}

void **build2DArray(int a, int b, int elementSize) {
    void **matrix = (void **) malloc(a * sizeof(int *));
    for (int i = 0; i < a; i++) {
        matrix[i] = (void *) malloc(b * elementSize);
    }
    return matrix;
}

int equalsIntPtrInt(int *intPtr, int normalInt) {
    return *intPtr == normalInt;
}

void print2DArray(float **matrix, int a, int b) {
    for (int i = 0; i < a; i++) {
        for (int j = 0; j < b; j++) {
            printf("%0.1f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void InvertCostMatrix(float **matrix, int a, int b) {
    for (int i = 0; i < a; i++) {
        for (int j = 0; j < b; j++) {
            matrix[i][j] = (float) MAX_VALUE - matrix[i][j] - 30;
        }
    }
}

// DOUBLE LINKED LIST --------------------------------------------------------------------------------------------------

typedef struct {
    void *content;
    struct ListElement *next;
    struct ListElement *before;
} ListElement;

typedef struct {
    ListElement *first;
    ListElement *last;
    int size;
} DoubleLinkedList;

ListElement *NewListElement(void *content, ListElement *next, ListElement *before);

DoubleLinkedList *NewDoubleLinkedList();

ListElement *InsertToList(DoubleLinkedList *doubleLinkedList, void *value);

ListElement *iterOnList(ListElement *first, int iterations);

ListElement *ListPop(DoubleLinkedList *doubleLinkedList, void *value);

ListElement *ListPopIndex(DoubleLinkedList *doubleLinkedList, int index);

void RemoveElement(DoubleLinkedList *doubleLinkedList, ListElement *listElement);

ListElement *SearchList(DoubleLinkedList *doubleLinkedList, int (*equals)(void *, int), int id);

void InsertToSortedList(DoubleLinkedList *doubleLinkedList, void *value, int (*compare)(void *, void *));

ListElement *getBiggestSmallerElement(DoubleLinkedList *doubleLinkedList, void *value, int (*compare)(void *, void *));

void InsertIntToSet(DoubleLinkedList *doubleLinkedList, int value);

void InsertToSortedListWithDuplicates(DoubleLinkedList *doubleLinkedList, void *value, int (*compare)(void *, void *));


ListElement *NewListElement(void *content, ListElement *next, ListElement *before) {
    ListElement *listElement = (ListElement *) malloc(sizeof(ListElement));

    listElement->content = content;
    listElement->next = (struct ListElement *) next;
    listElement->before = (struct ListElement *) before;

    return listElement;
}

DoubleLinkedList *NewDoubleLinkedList() {
    DoubleLinkedList *list = (DoubleLinkedList *) malloc(sizeof(DoubleLinkedList));

    list->size = 0;
    list->first = NULL;
    list->last = NULL;

    return list;
}

ListElement *InsertToList(DoubleLinkedList *doubleLinkedList, void *value) {
    ListElement *newElement = NewListElement(value, NULL, doubleLinkedList->last);

    if (doubleLinkedList->last != NULL)
        doubleLinkedList->last->next = (struct ListElement *) newElement;

    doubleLinkedList->last = newElement;
    doubleLinkedList->size += 1;

    if (doubleLinkedList->size == 1) {
        doubleLinkedList->first = newElement;
    }

    return newElement;
}

ListElement *iterOnList(ListElement *first, int iterations) {
    if (iterations == 0) {
        return first;
    }
    return iterOnList((ListElement *) first->next, iterations - 1);
}

ListElement *ListPop(DoubleLinkedList *doubleLinkedList, void *value) {
    ListElement *element = doubleLinkedList->first;
    for (int i = 0; i < doubleLinkedList->size; i++) {
        if (element->content == value) {
            RemoveElement(doubleLinkedList, element);
            return element;
        }
        element = iterOnList(element, 1);
    }
    return NULL;
}

ListElement *ListPopIndex(DoubleLinkedList *doubleLinkedList, int index) {
    ListElement *element = iterOnList(doubleLinkedList->first, index);
    RemoveElement(doubleLinkedList, element);
    return element;
}

void RemoveElement(DoubleLinkedList *doubleLinkedList, ListElement *listElement) {
    if (listElement->before != NULL) {
        ((ListElement *) listElement->before)->next = listElement->next;
    } else {
        doubleLinkedList->first = (ListElement *) listElement->next;
    }

    if (listElement->next != NULL) {
        ((ListElement *) listElement->next)->before = listElement->before;
    } else {
        doubleLinkedList->last = (ListElement *) listElement->before;
    }
    doubleLinkedList->size -= 1;
}

ListElement *SearchList(DoubleLinkedList *doubleLinkedList, int (*equals)(void *, int), int id) {
    ListElement *element = doubleLinkedList->first;

    for (int i = 0; i < doubleLinkedList->size; i++) {
        if (equals(element->content, id)) {
            return element;
        }
        element = iterOnList(element, 1);
    }

    return NULL;
}

void InsertToSortedList(DoubleLinkedList *doubleLinkedList, void *value, int (*compare)(void *, void *)) {
    // if value is already in list returns and won't do anything
    ListElement *biggestSmallerElement = getBiggestSmallerElement(doubleLinkedList, value, compare);
    if (biggestSmallerElement != NULL && compare(biggestSmallerElement->content, value) == 0)
        return;

    ListElement *newElement = NewListElement(value, NULL, NULL);

    if (biggestSmallerElement == NULL) {
        if (doubleLinkedList->size > 0) {
            newElement->next = (struct ListElement *) doubleLinkedList->first;
        }
        doubleLinkedList->first = newElement;
    } else {
        if (biggestSmallerElement->next != NULL) {
            newElement->next = (struct ListElement *) biggestSmallerElement->next;
        }
        biggestSmallerElement->next = (struct ListElement *) newElement;
    }

    doubleLinkedList->size += 1;
}

void InsertToSortedListWithDuplicates(DoubleLinkedList *doubleLinkedList, void *value, int (*compare)(void *, void *)) {
    if (doubleLinkedList->size == 0) {
        InsertToList(doubleLinkedList, value);
        return;
    }

    ListElement *biggestSmallerElement = getBiggestSmallerElement(doubleLinkedList, value, compare);

    ListElement *newElement = NewListElement(value, NULL, biggestSmallerElement);

    if (biggestSmallerElement == NULL) {
        newElement->next = (struct ListElement *) doubleLinkedList->first;
        doubleLinkedList->first = newElement;
    } else {
        if (biggestSmallerElement->next != NULL) {
            newElement->next = (struct ListElement *) biggestSmallerElement->next;
            ((ListElement *) biggestSmallerElement->next)->before = (struct ListElement *) newElement;
        } else {
            doubleLinkedList->last = newElement;
        }

        biggestSmallerElement->next = (struct ListElement *) newElement;
    }

    doubleLinkedList->size += 1;
}

ListElement *getBiggestSmallerElement(DoubleLinkedList *doubleLinkedList, void *value, int (*compare)(void *, void *)) {
    // the given list should be already sorted
    if (compare(doubleLinkedList->first->content, value) == 1)
        return NULL;

    ListElement *element = doubleLinkedList->first;
    while (element->next != NULL && compare(((ListElement *) element->next)->content, value) != 1) {
        element = iterOnList(element, 1);
    }

    return element;
}

void InsertIntToSet(DoubleLinkedList *doubleLinkedList, int value) {
    ListElement *element = SearchList(doubleLinkedList, (int (*)(void *, int)) equalsIntPtrInt, value);
    if (element != NULL)
        return;

    int *placeHolder = (int *) malloc(sizeof(int));
    *placeHolder = value;
    InsertToList(doubleLinkedList, placeHolder);
}

// AVL TREE ---------------------------------------------------------------------------------------------------------

typedef struct {
    void *content;
    struct BinaryTreeNode *left;
    struct BinaryTreeNode *right;
    int height;
} BinaryTreeNode;

BinaryTreeNode *NewBinaryTreeNode(void *content);

int getNodeHeight(BinaryTreeNode *node);

BinaryTreeNode *LeftRotate(BinaryTreeNode *root);

BinaryTreeNode *RightRotate(BinaryTreeNode *root);

BinaryTreeNode *InsertToTree(BinaryTreeNode *root, void *content, int (*compare)(void *, void *));

BinaryTreeNode *rotate(BinaryTreeNode *root, int (*compare)(void *, void *));

BinaryTreeNode *SearchTree(BinaryTreeNode *root, char name[], int (*compareByName)(void *, char[]));

BinaryTreeNode *TreePop(BinaryTreeNode *root, void *content, int (*compare)(void *, void *));

BinaryTreeNode *getRightMostNode(BinaryTreeNode *root);

BinaryTreeNode *getLeftMostNode(BinaryTreeNode *root);


BinaryTreeNode *NewBinaryTreeNode(void *content) {
    BinaryTreeNode *node = (BinaryTreeNode *) malloc(sizeof(BinaryTreeNode));
    node->content = content;
    node->height = 1;
    return node;
}

int getNodeHeight(BinaryTreeNode *node) {
    if (node != NULL)
        return node->height;
    return 0;
}

BinaryTreeNode *LeftRotate(BinaryTreeNode *root) {
    BinaryTreeNode *leftNode = (BinaryTreeNode *) root->left;
    BinaryTreeNode *rightLeftNode = NULL;
    if (leftNode != NULL)
        rightLeftNode = (BinaryTreeNode *) leftNode->right;

    leftNode->right = (struct BinaryTreeNode *) root;
    root->left = (struct BinaryTreeNode *) rightLeftNode;

    root->height = max(getNodeHeight((BinaryTreeNode *) root->left), getNodeHeight((BinaryTreeNode *) root->right) + 1);
    leftNode->height = max(getNodeHeight((BinaryTreeNode *) leftNode->left),
                           getNodeHeight((BinaryTreeNode *) leftNode->right) + 1);

    return leftNode;
}

BinaryTreeNode *RightRotate(BinaryTreeNode *root) {
    BinaryTreeNode *rightNode = (BinaryTreeNode *) root->right;
    BinaryTreeNode *leftRightNode = NULL;
    if (rightNode != NULL)
        leftRightNode = (BinaryTreeNode *) rightNode->left;

    rightNode->left = (struct BinaryTreeNode *) root;
    root->right = (struct BinaryTreeNode *) leftRightNode;

    root->height = max(getNodeHeight((BinaryTreeNode *) root->right), getNodeHeight((BinaryTreeNode *) root->left) + 1);
    rightNode->height = max(getNodeHeight((BinaryTreeNode *) rightNode->right),
                            getNodeHeight((BinaryTreeNode *) rightNode->left) + 1);

    return rightNode;
}

BinaryTreeNode *InsertToTree(BinaryTreeNode *root, void *content, int (*compare)(void *, void *)) {
    if (root == NULL)
        return (NewBinaryTreeNode(content));

    if (compare(root->content, content) == 1)
        root->left = (struct BinaryTreeNode *) InsertToTree((BinaryTreeNode *) root->left, content, compare);
    else
        root->right = (struct BinaryTreeNode *) InsertToTree((BinaryTreeNode *) root->right, content, compare);

    root->height = max(getNodeHeight((BinaryTreeNode *) root->right), getNodeHeight((BinaryTreeNode *) root->left)) + 1;

    return rotate(root, compare);
}

BinaryTreeNode *rotate(BinaryTreeNode *root, int (*compare)(void *, void *)) {
    int heightDifference = getNodeHeight((BinaryTreeNode *) root->left) - getNodeHeight((BinaryTreeNode *) root->right);

    if (heightDifference < -1) {
        if (getNodeHeight((BinaryTreeNode *) ((BinaryTreeNode *) root->right)->right) >
            getNodeHeight((BinaryTreeNode *) ((BinaryTreeNode *) root->right)->left))
            return RightRotate(root);
        root->right = (struct BinaryTreeNode *) LeftRotate((BinaryTreeNode *) root->right);
        return RightRotate(root);
    } else if (heightDifference > 1) {
        if (getNodeHeight((BinaryTreeNode *) ((BinaryTreeNode *) root->left)->left) >
            getNodeHeight((BinaryTreeNode *) ((BinaryTreeNode *) root->left)->right))
            return LeftRotate(root);
        root->left = (struct BinaryTreeNode *) RightRotate((BinaryTreeNode *) root->left);
        return LeftRotate(root);
    }

    return root;
}

BinaryTreeNode *SearchTree(BinaryTreeNode *root, char name[], int (*compareByName)(void *, char[])) {
    if (compareByName(root->content, name) == 0)
        return root;

    if (compareByName(root->content, name) == 1)
        return SearchTree((BinaryTreeNode *) root->left, name, compareByName);

    return SearchTree((BinaryTreeNode *) root->right, name, compareByName);
}

BinaryTreeNode *TreePop(BinaryTreeNode *root, void *content, int (*compare)(void *, void *)) {
    if (compare(root->content, content) == 0) {
        if (root->right != NULL) {
            BinaryTreeNode *newRoot = getLeftMostNode((BinaryTreeNode *) root->right);
            root->content = newRoot->content;
            root->right = (struct BinaryTreeNode *) TreePop((BinaryTreeNode *) root->right, newRoot->content, compare);
        } else if (root->left != NULL) {
            BinaryTreeNode *newRoot = getRightMostNode((BinaryTreeNode *) root->left);
            root->content = newRoot->content;
            root->left = (struct BinaryTreeNode *) TreePop((BinaryTreeNode *) root->left, newRoot->content, compare);
        } else {
            root = NULL;
        }
    } else if (compare(root->content, content) == 1) {
        BinaryTreeNode *leftNode = TreePop((BinaryTreeNode *) root->left, content, compare);
        root->left = (struct BinaryTreeNode *) leftNode;
    } else {
        BinaryTreeNode *rightNode = TreePop((BinaryTreeNode *) root->right, content, compare);
        root->right = (struct BinaryTreeNode *) rightNode;
    }

    if (root != NULL) {
        root->height = max(getNodeHeight((BinaryTreeNode *) root->left),
                           getNodeHeight((BinaryTreeNode *) root->right)) + 1;
        root = rotate(root, compare);
    }

    return root;
}

BinaryTreeNode *getRightMostNode(BinaryTreeNode *root) {
    if (root->right == NULL)
        return root;
    return getRightMostNode((BinaryTreeNode *) root->right);
}

BinaryTreeNode *getLeftMostNode(BinaryTreeNode *root) {
    if (root->left == NULL)
        return root;
    return getLeftMostNode((BinaryTreeNode *) root->left);
}

// HASH TABLE ----------------------------------------------------------------------------------------------------------

typedef struct {
    int a, b;
    int prime;
    int size;
    int filled;
    float fillFactor;
    DoubleLinkedList *buckets[];
} HashTable;

HashTable *NewHashTable(int size, int a, int b, int prime, float fillFactor);

HashTable *InsertToHashTable(HashTable *hashTable, void *content, int (*hash)(HashTable *, void *));

void *SearchHashTable(HashTable *hashTable, int (*equals)(void *, int), int id);

HashTable *
HashTablePop(HashTable *hashTable, int (*equals)(void *, int), int id, int (*hashContent)(HashTable *, void *));

HashTable *doubleHashTableSize(HashTable *hashTable, int (*hash)(HashTable *, void *));

HashTable *halfHashTableSize(HashTable *hashTable, int (*hash)(HashTable *, void *));

int hash(HashTable *hashTable, int id);


HashTable *NewHashTable(int size, int a, int b, int prime, float fillFactor) {
    HashTable *hashTable = (HashTable *) malloc(sizeof(HashTable) + sizeof(DoubleLinkedList *[size]));
    hashTable->a = a;
    hashTable->b = b;
    hashTable->prime = prime;
    hashTable->size = size;
    hashTable->filled = 0;
    hashTable->fillFactor = fillFactor;
    for (int i = 0; i < size; i++) {
        hashTable->buckets[i] = NewDoubleLinkedList();
    }

    return hashTable;
}

HashTable *InsertToHashTable(HashTable *hashTable, void *content, int (*hash)(HashTable *, void *)) {
    if (hashTable->filled >= hashTable->size) {
        hashTable = doubleHashTableSize(hashTable, hash);
    }

    int hashResult = hash(hashTable, content);
    InsertToList(hashTable->buckets[hashResult], content);
    hashTable->filled += 1;

    return hashTable;
}

void *SearchHashTable(HashTable *hashTable, int (*equals)(void *, int), int id) {
    int hashResult = hash(hashTable, id);
    ListElement *listElement = SearchList(hashTable->buckets[hashResult], equals, id);
    return listElement->content;
}

HashTable *
HashTablePop(HashTable *hashTable, int (*equals)(void *, int), int id, int (*hashContent)(HashTable *, void *)) {
    if (hashTable->filled <= hashTable->size * hashTable->fillFactor) {
        hashTable = halfHashTableSize(hashTable, hashContent);
    }

    int hashResult = hash(hashTable, id);
    ListElement *listElement = SearchList(hashTable->buckets[hashResult], equals, id);
    RemoveElement(hashTable->buckets[hashResult], listElement);
    hashTable->filled -= 1;

    return hashTable;
}

HashTable *doubleHashTableSize(HashTable *hashTable, int (*hash)(HashTable *, void *)) {
    HashTable *newHashTable = NewHashTable(2 * hashTable->size, hashTable->a, hashTable->b, hashTable->prime,
                                           hashTable->fillFactor);
    for (int i = 0; i < hashTable->size; i++) {
        for_each(element, hashTable->buckets[i]) {
            newHashTable = InsertToHashTable(newHashTable, element->content, hash);
        }
    }
    return newHashTable;
}

HashTable *halfHashTableSize(HashTable *hashTable, int (*hash)(HashTable *, void *)) {
    HashTable *newHashTable = NewHashTable(hashTable->size / 2, hashTable->a, hashTable->b, hashTable->prime,
                                           hashTable->fillFactor);
    for (int i = 0; i < hashTable->size; i++) {
        for_each(element, hashTable->buckets[i]) {
            newHashTable = InsertToHashTable(newHashTable, element->content, hash);
        }
    }
    return newHashTable;
}

int hash(HashTable *hashTable, int id) {
    return (int) ((((long) hashTable->a * (long) id + (long) hashTable->b) % hashTable->prime) % hashTable->size);
}

// GRAPH ---------------------------------------------------------------------------------------------------------------

typedef struct {
    void *content;
    DoubleLinkedList *children;
    int isVisited;
} GraphNode;

typedef struct {
    DoubleLinkedList *nodes;
} Graph;

GraphNode *NewGraphNode(void *content);

Graph *NewGraph();

void WhitenNodes(DoubleLinkedList *nodes);

DoubleLinkedList *GetAllDescendants(GraphNode *root, int (*compare)(void *, void *));

int IsDescendant(GraphNode *node, GraphNode *root, DoubleLinkedList *visited, int (*compare)(void *, void *));


GraphNode *NewGraphNode(void *content) {
    GraphNode *node = (GraphNode *) malloc(sizeof(GraphNode));

    node->content = content;
    node->children = NewDoubleLinkedList();
    node->isVisited = 0;

    return node;
}

Graph *NewGraph() {
    Graph *graph = (Graph *) malloc(sizeof(Graph));

    graph->nodes = NewDoubleLinkedList();

    return graph;
}

void WhitenNodes(DoubleLinkedList *nodes) {
    for_each(element, nodes) {
        ((GraphNode *) element->content)->isVisited = 0;
    }
}

DoubleLinkedList *GetAllDescendants(GraphNode *root, int (*compare)(void *, void *)) {
    // Note: WhitenNodes should be called on the result

    DoubleLinkedList *descendants = NewDoubleLinkedList();
    InsertToList(descendants, root);
    root->isVisited = 1;

    for_each(childElement, root->children) {
        if (!((GraphNode *) childElement->content)->isVisited) {
            for_each(grandChildElement, GetAllDescendants((GraphNode *) childElement->content, compare)) {
                InsertToSortedList(descendants, grandChildElement->content, compare);
            }
        }
    }

    return descendants;
}

int IsDescendant(GraphNode *node, GraphNode *root, DoubleLinkedList *visited, int (*compare)(void *, void *)) {
    // Note: WhitenNodes should be called on the DoubleLinkedList *visited
    if (root->isVisited)
        return 0;

    root->isVisited = 1;
    InsertToList(visited, root);

    if (compare(root, node) == 0) {
        return 1;
    }

    for_each(element, root->children) {
        if (!((GraphNode *) element->content)->isVisited) {
            if (IsDescendant(node, (GraphNode *) element->content, visited, compare))
                return 1;
        }
    }

    return 0;
}

// HUNGARIAN SOLUTION FOR MIN WEIGHT MATCHING --------------------------------------------------------------------------

/*
 * this implementation is based on
 * https://homes.di.unimi.it/righini/Didattica/OttimizzazioneCombinatoria/MaterialeOC/11%20-%20Min%20cost%20bipartite%20matching.pdf
 * and works only for n*n matrices with positive values
 */
int *SolveAssignmentHungarian(float **weightMatrix, int size, int *mates);

void dualInitialization(float **weight, int size, float *U, float *V);

int primalInitialization(float **weight, int size, float *U, float *V, int *mates, int **relations);

void pathInitialization(float **weight, int size, int *mates, int *labels, float *p, int *pie, float *U, float *V,
                        DoubleLinkedList *L);

void
labelPropagation(float **weight, int size, int *mates, int *labels, float *p, int *pie, DoubleLinkedList *L, int *path);

void aLabelPropagation(float **weight, int size, int *labels, DoubleLinkedList *L, int k);

void
bLabelPropagation(float **weight, int size, int *mates, int *labels, float *p, int *pie, DoubleLinkedList *L, int k);

void dualIteration(float **weight, int size, int *labels, float *p, int *pie, float *U, float *V, DoubleLinkedList *L);

void primalIteration(float **weight, int size, int *mates, int *labels, int *path, int **relations, int *card);


int *SolveAssignmentHungarian(float **weightMatrix, int size, int *mates) {
    float U[size];
    float V[size];

    dualInitialization(weightMatrix, size, U, V);
    int **relations = (int **) build2DArray(size, size, sizeof(int));
    int card = primalInitialization(weightMatrix, size, U, V, mates, (int **) relations);

    while (card < size) {
        int labels[2 * size];
        float p[size];
        int pie[size];
        DoubleLinkedList *L = NewDoubleLinkedList();

        pathInitialization(weightMatrix, size, mates, labels, p, pie, U, V, L);

        int path = -1;
        while (path == -1) {
            while (path == -1 && L->size != 0) {
                labelPropagation(weightMatrix, size, mates, labels, p, pie, L, &path);
            }
            if (path == -1) {
                dualIteration(weightMatrix, size, labels, p, pie, U, V, L);
            }
        }

        primalIteration(weightMatrix, size, mates, labels, &path, relations, &card);
    }

    return mates;
}

void dualInitialization(float **weight, int size, float *U, float *V) {
    for (int i = 0; i < size; i++) {
        U[i] = minArray(weight[i], size);

        for (int j = 0; j < size; j++) {
            weight[i][j] -= U[i];
        }
    }

    for (int i = 0; i < size; i++) {
        float minVal = weight[0][i];
        for (int j = 0; j < size; j++) {
            if (weight[j][i] < minVal)
                minVal = weight[j][i];
        }
        V[i] = minVal;

        for (int j = 0; j < size; j++) {
            weight[j][i] -= V[i];
        }
    }
}

int primalInitialization(float **weight, int size, float *U, float *V, int *mates, int **relations) {
    for (int i = 0; i < 2 * size; i++) {
        mates[i] = -1;
    }

    int card = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (weight[i][j] == 0 && mates[i] == -1 && mates[j + size] == -1) {
                relations[i][j] = 1;
                card += 1;
                mates[i] = j;
                mates[j + size] = i;
            }
        }
    }

    return card;
}

void pathInitialization(float **weight, int size, int *mates, int *labels, float *p, int *pie, float *U, float *V,
                        DoubleLinkedList *L) {

    for (int i = 0; i < 2 * size; i++) {
        labels[i] = -1;
    }

    for (int i = 0; i < size; i++) {
        p[i] = (float) MAX_VALUE;
        pie[i] = -1;
    }

    for (int i = 0; i < size; i++) {
        if (mates[i] != -1)
            continue;

        labels[i] = S;
        InsertIntToSet(L, i);

        for (int j = 0; j < size; j++) {
            if (labels[j + size] != -1)
                continue;

            if (weight[i][j] < p[j]) {
                p[j] = weight[i][j];
                pie[j] = i;
            }
        }
    }
}

void labelPropagation(float **weight, int size, int *mates, int *labels, float *p, int *pie, DoubleLinkedList *L,
                      int *path) {
    int k = *(int *) ListPopIndex(L, 0)->content;

    if (k < size) {
        aLabelPropagation(weight, size, labels, L, k);
    } else {
        if (mates[k] != -1) {
            bLabelPropagation(weight, size, mates, labels, p, pie, L, k);
        } else {
            *path = k;
        }
    }
}

void aLabelPropagation(float **weight, int size, int *labels, DoubleLinkedList *L, int k) {
    for (int j = 0; j < size; j++) {
        if (labels[j + size] == -1 && weight[k][j] == 0) {
            labels[j + size] = k;
            InsertIntToSet(L, j + size);
        }
    }
}

void
bLabelPropagation(float **weight, int size, int *mates, int *labels, float *p, int *pie, DoubleLinkedList *L, int k) {
    if (labels[mates[k]] != -1)
        return;

    labels[mates[k]] = k;
    InsertIntToSet(L, mates[k]);
    for (int j = 0; j < size; j++) {
        if (labels[size + j] != -1)
            continue;

        if (weight[mates[k]][j] < p[j]) {
            p[j] = weight[mates[k]][j];
            pie[j] = mates[k];
        }
    }
}

void dualIteration(float **weight, int size, int *labels, float *p, int *pie, float *U, float *V, DoubleLinkedList *L) {
    float sigma = (float) MAX_VALUE;

    for (int j = 0; j < size; j++) {
        if (labels[j + size] == -1)
            if (p[j] < sigma)
                sigma = p[j];
    }

    for (int i = 0; i < size; i++) {
        if (labels[i] == -1)
            continue;

        for (int j = 0; j < size; j++) {
            weight[i][j] -= sigma;
        }

        U[i] += sigma;
    }

    for (int j = 0; j < size; j++) {
        if (labels[j + size] == -1)
            continue;

        for (int i = 0; i < size; i++) {
            weight[i][j] += sigma;
        }

        V[j] -= sigma;
    }

    for (int j = 0; j < size; j++) {
        if (labels[j + size] != -1)
            continue;

        p[j] -= sigma;
    }

    for (int j = 0; j < size; j++) {
        if (labels[j + size] == -1 && p[j] == 0) {
            labels[j + size] = pie[j];
            InsertIntToSet(L, j + size);
        }
    }
}

void primalIteration(float **weight, int size, int *mates, int *labels, int *path, int **relations, int *card) {
    int j = *path;
    float z;

    do {
        int i = labels[j];

        int j2 = j % size;
        int i2 = i % size;

        mates[j] = i;
        mates[i] = j;
        relations[i2][j2] = 1;
        *card += 1;

        j = labels[i];
        j2 = j % size;

        if (labels[i] != S) {
            relations[i2][j2] = 0;
            *card -= 1;
        }
    } while (j != S);
}

// MODELS --------------------------------------------------------------------------------------------------------------

typedef struct {
    int number;
    char name[30];
    DoubleLinkedList *grades;
} Student;

typedef struct {
    int code;
    char name[10];
    DoubleLinkedList *grades;
} Course;

typedef struct {
    Student *student;
    Course *course;
    float score;
    int term;
    ListElement *studentListElement;
    ListElement *courseListElement;
} Grade;

Student *NewStudent(int number);

int equalsStudent(Student *student, int studentNumber);

int compareStudent(Student *student1, Student *student2);

int compareStudentAndName(Student *student, char name[]);

int compareStudentByName(Student *student1, Student *student2);

void PrintStudent(Student *student);

Course *NewCourse(int code);

int equalsCourse(Course *course, int code);

int compareCourse(Course *course1, Course *course2);

int compareCourseAndName(Course *course, char name[]);

int compareCourseByName(Course *course1, Course *course2);

void PrintCourse(Course *course);

Grade *NewGrade(Student *student, Course *course, float score, int term);

int equalsGradeWithCourseCode(Grade *grade, int courseCode);

int equalsGradeWithStudentNumber(Grade *grade, int studentNumber);

int hashStudent(HashTable *hashTable, void *content);

int hashCourse(HashTable *hashTable, void *content);

int AreDirectRelative(Course *course1, Course *course2);

int equalsGraphNodeCourse(GraphNode *graphNode, int courseCode);

int compareGraphNodeCourse(GraphNode *graphNode1, GraphNode *graphNode2);

int IsStudentBetter(Student *student1, Student *student2);

int equalsGraphNodeStudent(GraphNode *graphNode, int studentNumber);

float *GetCourseAvgScores(Course *course, int minTerm, int maxTerm, float *avgs);

float **BuildStudentTermCourseMatrix(Student *student, DoubleLinkedList *terms, DoubleLinkedList *courses);

void PrintTermCourseSelection(DoubleLinkedList *terms, DoubleLinkedList *courses, int *mates);

void printIntListElements(DoubleLinkedList *doubleLinkedList);


Student *NewStudent(int number) {
    Student *student = (Student *) malloc(sizeof(Student));

    student->number = number;
    scanf("%s", student->name);
    student->grades = NewDoubleLinkedList();

    return student;
}

int equalsStudent(Student *student, int studentNumber) {
    return student->number == studentNumber;
}

int compareStudent(Student *student1, Student *student2) {
    if (student1->number > student2->number) {
        return 1;
    } else if (student1->number < student2->number) {
        return -1;
    } else {
        return 0;
    }
}

int compareStudentAndName(Student *student, char name[]) {
    if (strcmp(student->name, name) >= 1) {
        return 1;
    } else if (strcmp(student->name, name)) {
        return -1;
    } else {
        return 0;
    }
}

int compareStudentByName(Student *student1, Student *student2) {
    return compareStudentAndName(student1, student2->name);
}

void PrintStudent(Student *student) {
    printf("%d %s %d\n", student->number, student->name, student->grades->size);
    for_each(element, student->grades) {
        Grade *grade = (Grade *) element->content;
        if ((int) grade->score == grade->score)
            printf("%d %d %d\n", grade->course->code, grade->term, (int) grade->score);
        else
            printf("%d %d %0.1f\n", grade->course->code, grade->term, grade->score);
    }
}

Course *NewCourse(int code) {
    Course *course = (Course *) malloc(sizeof(Course));

    course->code = code;
    scanf("%s", course->name);
    course->grades = NewDoubleLinkedList();

    return course;
}

int equalsCourse(Course *course, int code) {
    return course->code == code;
}

int compareCourse(Course *course1, Course *course2) {
    if (course1->code > course2->code) {
        return 1;
    } else if (course1->code < course2->code) {
        return -1;
    } else {
        return 0;
    }
}

int compareCourseAndName(Course *course, char name[]) {
    if (strcmp(course->name, name) >= 1) {
        return 1;
    } else if (strcmp(course->name, name)) {
        return -1;
    } else {
        return 0;
    }
}

int compareCourseByName(Course *course1, Course *course2) {
    return compareCourseAndName(course1, course2->name);
}

void PrintCourse(Course *course) {
    printf("%d %s %d\n", course->code, course->name, course->grades->size);
    for_each(element, course->grades) {
        Grade *grade = (Grade *) element->content;

        if ((int) grade->score == grade->score)
            printf("%d %d %d\n", grade->student->number, grade->term, (int) grade->score);
        else
            printf("%d %d %0.1f\n", grade->student->number, grade->term, grade->score);
    }
}

Grade *NewGrade(Student *student, Course *course, float score, int term) {
    Grade *grade = (Grade *) malloc(sizeof(Grade));

    grade->student = student;
    grade->course = course;
    grade->score = score;
    grade->term = term;

    return grade;
}

int equalsGradeWithCourseCode(Grade *grade, int courseCode) {
    return grade->course->code == courseCode;
}

int equalsGradeWithStudentNumber(Grade *grade, int studentNumber) {
    return grade->student->number == studentNumber;
}

int hashStudent(HashTable *hashTable, void *content) {
    return hash(hashTable, ((Student *) content)->number);
}

int hashCourse(HashTable *hashTable, void *content) {
    return hash(hashTable, ((Course *) content)->code);
}

int AreDirectRelative(Course *course1, Course *course2) {
    int commonStudents = 0;

    for_each(element, course1->grades) {
        if (SearchList(course2->grades, (int (*)(void *, int)) equalsGradeWithStudentNumber,
                       ((Grade *) element->content)->student->number) != NULL)
            commonStudents += 1;
    }

    if (course1->grades->size > 0 && course2->grades->size > 0 &&
        commonStudents > 0.5 * course1->grades->size && commonStudents > 0.5 * course2->grades->size)
        return 1;
    return 0;
}

int equalsGraphNodeCourse(GraphNode *graphNode, int courseCode) {
    return ((Course *) graphNode->content)->code == courseCode;
}

int compareGraphNodeCourse(GraphNode *graphNode1, GraphNode *graphNode2) {
    return compareCourse(graphNode1->content, graphNode2->content);
}

int IsStudentBetter(Student *student1, Student *student2) {
    int student1BetterScores = 0;
    int commonCourses = 0;

    for_each(element, student1->grades) {
        Grade *grade1 = (Grade *) element->content;
        ListElement *grade2Element = SearchList(student2->grades, (int (*)(void *, int)) equalsGradeWithCourseCode,
                                                grade1->course->code);
        if (grade2Element != NULL) {
            commonCourses += 1;
            if (grade1->score > ((Grade *) grade2Element->content)->score)
                student1BetterScores += 1;
        }
    }

    if (commonCourses > 0 && student1BetterScores > 0.5 * commonCourses)
        return 1;
    return 0;
}

int equalsGraphNodeStudent(GraphNode *graphNode, int studentNumber) {
    return ((Student *) graphNode->content)->number == studentNumber;
}

float *GetCourseAvgScores(Course *course, int minTerm, int maxTerm, float *avgs) {
    float sumScores[maxTerm - minTerm + 1];
    int countStudents[maxTerm - minTerm + 1];

    for (int i = 0; i < maxTerm - minTerm + 1; i++) {
        sumScores[i] = countStudents[i] = 0;
    }

    for_each(gradeElement, course->grades) {
        Grade *grade = (Grade *) gradeElement->content;
        sumScores[grade->term - minTerm] += grade->score;
        countStudents[grade->term - minTerm] += 1;
    }

    for (int i = 0; i < maxTerm - minTerm + 1; i++) {
        if (sumScores[i] == 0 || countStudents[i] == 0) {
            avgs[i] = 0;
            continue;
        }
        avgs[i] = (float) sumScores[i] / (float) countStudents[i];
    }
    return avgs;
}

float **BuildStudentTermCourseMatrix(Student *student, DoubleLinkedList *terms, DoubleLinkedList *courses) {
    int size = student->grades->size;
    float **scores = (float **) build2DArray(size, size, sizeof(float));

    int minTerm = *(int *) terms->first->content;
    int maxTerm = *(int *) terms->last->content;

    ListElement *courseElement = courses->first;
    for (int i = 0; i < size; i++) {
        float courseAvgs[maxTerm - minTerm + 1];
        GetCourseAvgScores(courseElement->content, minTerm, maxTerm, courseAvgs);

        ListElement *termElement = terms->first;
        for (int j = 0; j < size; j++) {
            scores[j][i] = courseAvgs[*(int *) termElement->content - minTerm];
            termElement = iterOnList(termElement, 1);
        }

        courseElement = iterOnList(courseElement, 1);
    }

    return scores;
}

void PrintTermCourseSelection(DoubleLinkedList *terms, DoubleLinkedList *courses, int *mates) {


    ListElement *termElement = terms->first;

    DoubleLinkedList *currentTermCourses = NewDoubleLinkedList();
    int i = 0;

    while (1) {
        InsertToSortedListWithDuplicates(currentTermCourses,
                                         &((Course *) iterOnList(courses->first, mates[i] % terms->size)->content)->code,
                                         (int (*)(void *, void *)) compareInt);

        if (termElement->next == NULL || (termElement->next != NULL && *(int *) termElement->content !=
                                                                       *(int *) ((ListElement *) termElement->next)->content)) {
            printf("%d ", *(int *) termElement->content);
            printIntListElements(currentTermCourses);

            if (termElement->next == NULL)
                break;

            currentTermCourses->first = NULL;
            currentTermCourses->last = NULL;
            currentTermCourses->size = 0;
        }

        termElement = iterOnList(termElement, 1);
        i += 1;
    }
}

void printIntListElements(DoubleLinkedList *doubleLinkedList) {
    for_each(intElement, doubleLinkedList) {
        printf("%d ", *(int *) intElement->content);
    }
    printf("\n");
}

// COMMANDS ------------------------------------------------------------------------------------------------------------

void
AddStudent(DoubleLinkedList *studentsList, BinaryTreeNode **studentsTree, HashTable **studentsTable, Context *context);

void AddCourse(DoubleLinkedList *coursesList, BinaryTreeNode **coursesTree, HashTable **coursesTable, Context *context);

void AddGrade(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, Context *context);

void EditStudent(DoubleLinkedList *studentsList, BinaryTreeNode **studentsTree, Context *context);

void EditCourse(DoubleLinkedList *coursesList, BinaryTreeNode **coursesTree, Context *context);

void EditGrade(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, Context *context);

void DeleteStudent(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, BinaryTreeNode **studentsTree,
                   HashTable **studentsTable, Context *context);

void deleteStudentFromCourses(DoubleLinkedList *coursesList, Student *student, Context *context);

void DeleteCourse(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, BinaryTreeNode **coursesTree,
                  HashTable **coursesTable, Context *context);

void deleteCourseFromStudentGrades(DoubleLinkedList *studentsList, Course *courseList, Context *context);

void DeleteGrade(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, Context *context);

void NumberOfCourses(DoubleLinkedList *studentsList);

void NumberOfStudentsOfCourse(DoubleLinkedList *coursesList);

Grade *findGrade(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, int studentNumber, int courseCode);

void SearchStudentByName(BinaryTreeNode *studentsTree);

void SearchCourseByName(BinaryTreeNode *coursesTree);

void SearchStudentByNumber(HashTable *studentsTable);

void SearchCourseByCode(HashTable *coursesTable);

Graph *BuildCourseRelationGraph(DoubleLinkedList *coursesList);

void IsRelative(Graph *coursesGraph);

void AllRelatives(Graph *coursesGraph);

Graph *BuildStudentRelationGraph(DoubleLinkedList *studentsList);

void Compare(Graph *studentsGraph);

void MinRisk(DoubleLinkedList *studentsList);


void
AddStudent(DoubleLinkedList *studentsList, BinaryTreeNode **studentsTree, HashTable **studentsTable, Context *context) {
    int studentNumber;

    scanf("%d", &studentNumber);

    Student *student = NewStudent(studentNumber);
    InsertToList(studentsList, student);

    if (context->phase2) {
        *studentsTree = InsertToTree(*studentsTree, student, (int (*)(void *, void *)) compareStudentByName);
        *studentsTable = InsertToHashTable(*studentsTable, student, hashStudent);
    }
}

void
AddCourse(DoubleLinkedList *coursesList, BinaryTreeNode **coursesTree, HashTable **coursesTable, Context *context) {
    int code;

    scanf("%d", &code);

    Course *course = NewCourse(code);
    InsertToList(coursesList, course);
    if (context->phase2) {
        *coursesTree = InsertToTree(*coursesTree, course, (int (*)(void *, void *)) compareCourseByName);
        *coursesTable = InsertToHashTable(*coursesTable, course, hashCourse);
    }

}

void AddGrade(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, Context *context) {
    int studentNumber;
    int courseCode;
    int semesterCode;
    float score;

    scanf("%d", &studentNumber);
    scanf("%d", &courseCode);
    scanf("%d", &semesterCode);
    scanf("%f", &score);

    Student *student = (Student *) SearchList(studentsList, (int (*)(void *, int)) equalsStudent,
                                              studentNumber)->content;
    Course *course = (Course *) SearchList(coursesList, (int (*)(void *, int)) equalsCourse, courseCode)->content;

    Grade *grade = NewGrade(student, course, score, semesterCode);

    ListElement *studentListElement = InsertToList(student->grades, grade);
    ListElement *courseListElement = InsertToList(course->grades, grade);

    grade->courseListElement = courseListElement;
    grade->studentListElement = studentListElement;
}

void EditStudent(DoubleLinkedList *studentsList, BinaryTreeNode **studentsTree, Context *context) {
    int studentNumber;
    char name[50] = "";

    scanf("%d", &studentNumber);

    Student *student = (Student *) SearchList(studentsList, (int (*)(void *, int)) equalsStudent,
                                              studentNumber)->content;

    if (context->phase2)
        *studentsTree = TreePop(*studentsTree, student, (int (*)(void *, void *)) compareStudentByName);
    memset(student->name, 0, sizeof student->name);
    scanf("%s", student->name);

    if (context->phase2)
        *studentsTree = InsertToTree(*studentsTree, student, (int (*)(void *, void *)) compareStudentByName);
}

void EditCourse(DoubleLinkedList *coursesList, BinaryTreeNode **coursesTree, Context *context) {
    int code;
    char name[10] = "";

    scanf("%d", &code);

    Course *course = (Course *) SearchList(coursesList, (int (*)(void *, int)) equalsCourse, code)->content;

    if (context->phase2)
        *coursesTree = TreePop(*coursesTree, course, (int (*)(void *, void *)) compareCourseByName);

    memset(course->name, 0, sizeof course->name);
    scanf("%s", course->name);

    if (context->phase2)
        *coursesTree = InsertToTree(*coursesTree, course, (int (*)(void *, void *)) compareCourseByName);
}

void EditGrade(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, Context *context) {
    int studentNumber;
    int courseCode;
    float score;

    scanf("%d", &studentNumber);
    scanf("%d", &courseCode);
    scanf("%f", &score);

    Grade *grade = findGrade(studentsList, coursesList, studentNumber, courseCode);

    grade->score = score;
}

void DeleteStudent(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, BinaryTreeNode **studentsTree,
                   HashTable **studentsTable, Context *context) {
    int studentNumber;
    scanf("%d", &studentNumber);

    ListElement *studentElement = SearchList(studentsList, (int (*)(void *, int)) equalsStudent,
                                             studentNumber);

    RemoveElement(studentsList, studentElement);
    if (context->phase2) {
        *studentsTree = TreePop(*studentsTree, studentElement->content, (int (*)(void *, void *)) compareStudentByName);
        *studentsTable = HashTablePop(*studentsTable, (int (*)(void *, int)) equalsStudent, studentNumber, hashStudent);
    }
    deleteStudentFromCourses(coursesList, studentElement->content, NULL);
}

void deleteStudentFromCourses(DoubleLinkedList *coursesList, Student *student, Context *context) {
    for_each(element, student->grades) {
        Grade *grade = (Grade *) element->content;
        RemoveElement(grade->course->grades, grade->courseListElement);
    }
}

void DeleteCourse(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, BinaryTreeNode **coursesTree,
                  HashTable **coursesTable, Context *context) {
    int courseCode;
    scanf("%d", &courseCode);

    ListElement *courseElement = SearchList(coursesList, (int (*)(void *, int)) equalsCourse,
                                            courseCode);

    RemoveElement(coursesList, courseElement);
    if (context->phase2) {
        *coursesTree = TreePop(*coursesTree, courseElement->content, (int (*)(void *, void *)) compareCourseByName);
        *coursesTable = HashTablePop(*coursesTable, (int (*)(void *, int)) equalsCourse, courseCode, hashCourse);
    }
    deleteCourseFromStudentGrades(studentsList, courseElement->content, NULL);
}

void deleteCourseFromStudentGrades(DoubleLinkedList *studentsList, Course *courseList, Context *context) {
    for_each(element, courseList->grades) {
        Grade *grade = (Grade *) element->content;
        RemoveElement(grade->student->grades, grade->studentListElement);
    }
}

void DeleteGrade(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, Context *context) {
    int studentNumber;
    int courseCode;

    scanf("%d", &studentNumber);
    scanf("%d", &courseCode);

    Student *student = (Student *) SearchList(studentsList, (int (*)(void *, int)) equalsStudent,
                                              studentNumber)->content;
    ListElement *gradeElement = SearchList(student->grades,
                                           (int (*)(void *, int)) equalsGradeWithCourseCode,
                                           courseCode);
    Grade *grade = (Grade *) gradeElement->content;

    RemoveElement(grade->student->grades, grade->studentListElement);
    RemoveElement(grade->course->grades, grade->courseListElement);
}

void NumberOfCourses(DoubleLinkedList *studentsList) {
    int studentNumber;
    scanf("%d", &studentNumber);

    Student *student = (Student *) SearchList(studentsList, (int (*)(void *, int)) equalsStudent,
                                              studentNumber)->content;

    printf("%d\n", student->grades->size);
}

void NumberOfStudentsOfCourse(DoubleLinkedList *coursesList) {
    int code;
    scanf("%d", &code);

    Course *course = (Course *) SearchList(coursesList, (int (*)(void *, int)) equalsCourse, code)->content;

    printf("%d\n", course->grades->size);
}

Grade *findGrade(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, int studentNumber, int courseCode) {
    Student *student = (Student *) SearchList(studentsList, (int (*)(void *, int)) equalsStudent,
                                              studentNumber)->content;
    Grade *grade = (Grade *) SearchList(student->grades, (int (*)(void *, int)) equalsGradeWithCourseCode,
                                        courseCode)->content;

    return grade;
}

void SearchStudentByName(BinaryTreeNode *studentsTree) {
    char name[50] = "";
    scanf("%s", name);

    Student *student = (Student *) SearchTree(studentsTree, name,
                                              (int (*)(void *, char *)) compareStudentAndName)->content;
    PrintStudent(student);
}

void SearchCourseByName(BinaryTreeNode *coursesTree) {
    char name[10] = "";
    scanf("%s", name);

    Course *course = (Course *) SearchTree(coursesTree, name, (int (*)(void *, char *)) compareCourseAndName)->content;
    PrintCourse(course);
}

void SearchStudentByNumber(HashTable *studentsTable) {
    int studentNumber;
    scanf("%d", &studentNumber);

    Student *student = (Student *) SearchHashTable(studentsTable, (int (*)(void *, int)) equalsStudent, studentNumber);
    printf("%d\n", hash(studentsTable, studentNumber));
    PrintStudent(student);
}

void SearchCourseByCode(HashTable *coursesTable) {
    int courseCode;
    scanf("%d", &courseCode);

    Course *course = (Course *) SearchHashTable(coursesTable, (int (*)(void *, int)) equalsCourse, courseCode);
    printf("%d\n", hash(coursesTable, courseCode));
    PrintCourse(course);
}

Graph *BuildCourseRelationGraph(DoubleLinkedList *coursesList) {
    Graph *courseRelationGraph = NewGraph();

    for_each(courseElement, coursesList) {
        GraphNode *courseNode = NewGraphNode(courseElement->content);
        InsertToList(courseRelationGraph->nodes, courseNode);
    }

    for_each(courseNodeElement, courseRelationGraph->nodes) {
        Course *course1 = (Course *) ((GraphNode *) courseNodeElement->content)->content;
        for_each(otherCourseNodeElement, courseRelationGraph->nodes) {
            Course *course2 = (Course *) ((GraphNode *) otherCourseNodeElement->content)->content;

            if (course1->code != course2->code && AreDirectRelative(course1, course2))
                InsertToList(((GraphNode *) courseNodeElement->content)->children, otherCourseNodeElement->content);
        }
    }

    return courseRelationGraph;
}

void IsRelative(Graph *coursesGraph) {
    int courseCode1;
    scanf("%d", &courseCode1);

    int courseCode2;
    scanf("%d", &courseCode2);

    GraphNode *course1Node = (GraphNode *) SearchList(coursesGraph->nodes, (int (*)(void *, int)) equalsGraphNodeCourse,
                                                      courseCode1)->content;
    GraphNode *course2Node = (GraphNode *) SearchList(coursesGraph->nodes, (int (*)(void *, int)) equalsGraphNodeCourse,
                                                      courseCode2)->content;

    DoubleLinkedList *visited = NewDoubleLinkedList();

    if (IsDescendant(course1Node, course2Node, visited, (int (*)(void *, void *)) compareCourse))
        printf("%s\n", "yes");
    else
        printf("%s\n", "no");

    WhitenNodes(visited);
}

void AllRelatives(Graph *coursesGraph) {
    int courseCode;
    scanf("%d", &courseCode);

    GraphNode *courseNode = (GraphNode *) SearchList(coursesGraph->nodes, (int (*)(void *, int)) equalsGraphNodeCourse,
                                                     courseCode)->content;

    DoubleLinkedList *allRelatives = GetAllDescendants(courseNode, (int (*)(void *, void *)) compareGraphNodeCourse);
    WhitenNodes(allRelatives);

    for_each(element, allRelatives) {
        if (((Course *) ((GraphNode *) element->content)->content)->code != courseCode)
            printf("%d ", ((Course *) ((GraphNode *) element->content)->content)->code);
    }
    printf("\n");
}

Graph *BuildStudentRelationGraph(DoubleLinkedList *studentsList) {
    Graph *studentsRelationGraph = NewGraph();

    for_each(studentElement, studentsList) {
        GraphNode *studentNode = NewGraphNode(studentElement->content);
        InsertToList(studentsRelationGraph->nodes, studentNode);
    }

    for_each(studentNodeElement, studentsRelationGraph->nodes) {
        Student *student1 = (Student *) ((GraphNode *) studentNodeElement->content)->content;
        for_each(otherStudentNodeElement, studentsRelationGraph->nodes) {
            Student *student2 = (Student *) ((GraphNode *) otherStudentNodeElement->content)->content;

            if (student1->number != student2->number && IsStudentBetter(student1, student2))
                InsertToList(((GraphNode *) studentNodeElement->content)->children, otherStudentNodeElement->content);
        }
    }

    return studentsRelationGraph;
}

void Compare(Graph *studentsGraph) {
    int studentNumber1;
    scanf("%d", &studentNumber1);

    int studentNumber2;
    scanf("%d", &studentNumber2);

    GraphNode *student1Node = (GraphNode *) SearchList(studentsGraph->nodes,
                                                       (int (*)(void *, int)) equalsGraphNodeStudent,
                                                       studentNumber1)->content;
    GraphNode *student2Node = (GraphNode *) SearchList(studentsGraph->nodes,
                                                       (int (*)(void *, int)) equalsGraphNodeStudent,
                                                       studentNumber2)->content;

    DoubleLinkedList *visited1 = NewDoubleLinkedList();
    int isStudent1Better = IsDescendant(student1Node, student2Node, visited1, (int (*)(void *, void *)) compareStudent);
    WhitenNodes(visited1);

    DoubleLinkedList *visited2 = NewDoubleLinkedList();
    int isStudent2Better = IsDescendant(student2Node, student1Node, visited2, (int (*)(void *, void *)) compareStudent);
    WhitenNodes(visited2);

    if (isStudent1Better) {
        if (isStudent2Better)
            printf("?\n");
        else
            printf("<\n");
    } else {
        if (isStudent2Better)
            printf(">\n");
        else
            printf("?\n");
    }
}

void MinRisk(DoubleLinkedList *studentsList) {
    int studentNumber;
    scanf("%d", &studentNumber);

    Student *student = (Student *) SearchList(studentsList, (int (*)(void *, int)) equalsStudent,
                                              studentNumber)->content;

    DoubleLinkedList *courses = NewDoubleLinkedList();
    DoubleLinkedList *terms = NewDoubleLinkedList();

    for_each(gradeElement, student->grades) {
        InsertToSortedListWithDuplicates(courses, ((Grade *) gradeElement->content)->course,
                                         (int (*)(void *, void *)) compareCourse);

        int *placeHolder = (int *) malloc(sizeof(int));
        *placeHolder = ((Grade *) gradeElement->content)->term;
        InsertToSortedListWithDuplicates(terms, placeHolder, (int (*)(void *, void *)) compareInt);
    }

    float **termCourseMatrix = BuildStudentTermCourseMatrix(student, terms, courses);

    int size = student->grades->size;

    InvertCostMatrix(termCourseMatrix, size, size);
    int mates[2 * size];
    SolveAssignmentHungarian(termCourseMatrix, size, mates);

    PrintTermCourseSelection(terms, courses, mates);
}

// ---------------------------------------------------------------------------------------------------------------------

int main() {
    int numberOfCommands, a, b, prime;
    scanf("%d", &numberOfCommands);
    scanf("%d", &a);
    scanf("%d", &b);
    scanf("%d", &prime);

    Context *context = NewContext(prime != 1);

    DoubleLinkedList *studentsList = NewDoubleLinkedList();
    DoubleLinkedList *coursesList = NewDoubleLinkedList();
    BinaryTreeNode *studentsTree = NULL;
    BinaryTreeNode *coursesTree = NULL;
    HashTable *studentsTable = NewHashTable(1, a, b, prime, (float) 0.25);
    HashTable *coursesTable = NewHashTable(1, a, b, prime, (float) 0.25);
    Graph *courseRelationGraph = NULL;
    Graph *StudentRelationGraph = NULL;

    char command[20];
    for (int i = 0; i < numberOfCommands; i++) {
        scanf("%s", command);
        if (strcmp("ADDS", command) == 0) {
            AddStudent(studentsList, &studentsTree, &studentsTable, context);
        } else if (strcmp("ADDC", command) == 0) {
            AddCourse(coursesList, &coursesTree, &coursesTable, context);
        } else if (strcmp("ADDG", command) == 0) {
            AddGrade(studentsList, coursesList, context);
        } else if (strcmp("EDITS", command) == 0) {
            EditStudent(studentsList, &studentsTree, context);
        } else if (strcmp("EDITC", command) == 0) {
            EditCourse(coursesList, &coursesTree, context);
        } else if (strcmp("EDITG", command) == 0) {
            EditGrade(studentsList, coursesList, context);
        } else if (strcmp("DELETES", command) == 0) {
            DeleteStudent(studentsList, coursesList, &studentsTree, &studentsTable, context);
        } else if (strcmp("DELETEC", command) == 0) {
            DeleteCourse(studentsList, coursesList, &coursesTree, &coursesTable, context);
        } else if (strcmp("DELETEG", command) == 0) {
            DeleteGrade(studentsList, coursesList, context);
        } else if (strcmp("NUMBERC", command) == 0) {
            NumberOfCourses(studentsList);
        } else if (strcmp("NUMBERS", command) == 0) {
            NumberOfStudentsOfCourse(coursesList);
        } else if (strcmp("SEARCHSN", command) == 0) {
            SearchStudentByName(studentsTree);
        } else if (strcmp("SEARCHCN", command) == 0) {
            SearchCourseByName(coursesTree);
        } else if (strcmp("SEARCHSC", command) == 0) {
            SearchStudentByNumber(studentsTable);
        } else if (strcmp("SEARCHCC", command) == 0) {
            SearchCourseByCode(coursesTable);
        } else if (strcmp("ISRELATIVE", command) == 0) {
            if (courseRelationGraph == NULL)
                courseRelationGraph = BuildCourseRelationGraph(coursesList);
            IsRelative(courseRelationGraph);
        } else if (strcmp("ALLRELATIVE", command) == 0) {
            if (courseRelationGraph == NULL)
                courseRelationGraph = BuildCourseRelationGraph(coursesList);
            AllRelatives(courseRelationGraph);
        } else if (strcmp("COMPARE", command) == 0) {
            if (StudentRelationGraph == NULL)
                StudentRelationGraph = BuildStudentRelationGraph(studentsList);
            Compare(StudentRelationGraph);
        } else if (strcmp("MINRISK", command) == 0) {
            MinRisk(studentsList);
        } else {
            break;
        }
    }

    return 0;
}
