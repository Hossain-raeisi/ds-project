#include <stdio.h>
#include <malloc.h>
#include <string.h>


// UTILS ---------------------------------------------------------------------------------------------------------------
#define for_each(element, list) \
    for(ListElement *element = list->first; element != NULL; element = (ListElement *)element->next)

int max(int a, int b) {
    return (a > b) ? a : b;
}

// DOUBLE LINKED LIST --------------------------------------------------------------------------------------------------

typedef struct {
    void *content;
    struct ListElement *next;
    struct ListElement *before;
} ListElement;

ListElement *NewListElement(void *content, ListElement *next, ListElement *before);

ListElement *NewListElement(void *content, ListElement *next, ListElement *before) {
    ListElement *listElement = (ListElement *) malloc(sizeof(ListElement));

    listElement->content = content;
    listElement->next = (struct ListElement *) next;
    listElement->before = (struct ListElement *) before;

    return listElement;
}

typedef struct {
    ListElement *first;
    ListElement *last;
    int size;
} DoubleLinkedList;

DoubleLinkedList *NewDoubleLinkedList();

void InsertToList(DoubleLinkedList *doubleLinkedList, void *value);

ListElement *iterOnList(ListElement *first, int iterations);

ListElement *ListPop(DoubleLinkedList *doubleLinkedList, void *value);

ListElement *ListPopIndex(DoubleLinkedList *doubleLinkedList, int index);

void RemoveElement(DoubleLinkedList *doubleLinkedList, ListElement *listElement);

ListElement *SearchList(DoubleLinkedList *doubleLinkedList, int (*equals)(void *, int), int id);

void InsertToSortedList(DoubleLinkedList *doubleLinkedList, void *value, int (*compare)(void *, void *));

ListElement *getBiggestSmallerElement(DoubleLinkedList *doubleLinkedList, void *value, int (*compare)(void *, void *));


DoubleLinkedList *NewDoubleLinkedList() {
    DoubleLinkedList *list = (DoubleLinkedList *) malloc(sizeof(DoubleLinkedList));

    list->size = 0;
    list->first = NULL;
    list->last = NULL;

    return list;
}

void InsertToList(DoubleLinkedList *doubleLinkedList, void *value) {
    ListElement *newElement = NewListElement(value, NULL, doubleLinkedList->last);

    if (doubleLinkedList->last != NULL)
        doubleLinkedList->last->next = (struct ListElement *) newElement;

    doubleLinkedList->last = newElement;
    doubleLinkedList->size += 1;

    if (doubleLinkedList->size == 1) {
        doubleLinkedList->first = newElement;
    }
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

ListElement *getBiggestSmallerElement(DoubleLinkedList *doubleLinkedList, void *value, int (*compare)(void *, void *)) {
    // the given list should be already sorted
    if (compare(doubleLinkedList->first->content, value) == 1)
        return NULL;

    ListElement *element = doubleLinkedList->first;
    while (element->next != NULL && compare(((ListElement *)element->next)->content, value) != 1) {
        element = iterOnList(element, 1);
    }

    return element;
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
                           getNodeHeight((BinaryTreeNode *) root->right) + 1);
        rotate(root, compare);
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

void InsertToHashTable(HashTable **hashTable, void *content, int (*hash)(HashTable *, void *));

void *SearchHashTable(HashTable *hashTable, int (*equals)(void *, int), int id);

void HashTablePop(HashTable **hashTable, int (*equals)(void *, int), int id, int (*hashContent)(HashTable *, void *));

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

void InsertToHashTable(HashTable **hashTable, void *content, int (*hash)(HashTable *, void *)) {
    int hashResult = hash(*hashTable, content);
    InsertToList((*hashTable)->buckets[hashResult], content);
    (*hashTable)->filled += 1;

    if ((*hashTable)->filled >= (*hashTable)->size) {
        *hashTable = doubleHashTableSize(*hashTable, hash);
    }
}

void *SearchHashTable(HashTable *hashTable, int (*equals)(void *, int), int id) {
    int hashResult = hash(hashTable, id);
    ListElement *listElement = SearchList(hashTable->buckets[hashResult], equals, id);
    return listElement->content;
}

void HashTablePop(HashTable **hashTable, int (*equals)(void *, int), int id, int (*hashContent)(HashTable *, void *)) {
    int hashResult = hash(*hashTable, id);
    ListElement *listElement = SearchList((*hashTable)->buckets[hashResult], equals, id);
    RemoveElement((*hashTable)->buckets[hashResult], listElement);
    (*hashTable)->filled -= 1;

    if ((*hashTable)->filled <= (*hashTable)->size * (*hashTable)->fillFactor) {
        *hashTable = halfHashTableSize(*hashTable, hashContent);
    }
}

HashTable *doubleHashTableSize(HashTable *hashTable, int (*hash)(HashTable *, void *)) {
    HashTable *newHashTable = NewHashTable(2 * hashTable->size, hashTable->a, hashTable->b, hashTable->prime,
                                           hashTable->fillFactor);
    for (int i = 0; i < hashTable->size; i++) {
        for_each(element, hashTable->buckets[i]) {
            InsertToHashTable(&newHashTable, element->content, hash);
        }
    }
    return newHashTable;
}

HashTable *halfHashTableSize(HashTable *hashTable, int (*hash)(HashTable *, void *)) {
    HashTable *newHashTable = NewHashTable(hashTable->size / 2, hashTable->a, hashTable->b, hashTable->prime,
                                           hashTable->fillFactor);
    for (int i = 0; i < hashTable->size; i++) {
        for_each(element, hashTable->buckets[i]) {
            InsertToHashTable(&newHashTable, element->content, hash);
        }
    }
    return newHashTable;
}

int hash(HashTable *hashTable, int id) {
    return ((hashTable->a * id + hashTable->b) % hashTable->prime) % hashTable->size;
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

// MODELS --------------------------------------------------------------------------------------------------------------

typedef struct {
    int number;
    char name[30];
    DoubleLinkedList *grades;
} Student;

Student *NewStudent(int number, char name[30]);

int equalsStudent(Student *student, int studentNumber);

int compareStudent(Student *student1, Student *student2);

int compareStudentAndName(Student *student, char name[]);

int compareStudentByName(Student *student1, Student *student2);

void PrintStudent(Student *student);

typedef struct {
    int code;
    char name[10];
    DoubleLinkedList *students;
} Course;

Course *NewCourse(int code, char name[10]);

int equalsCourse(Course *course, int code);

int compareCourse(Course *course1, Course *course2);

int compareCourseAndName(Course *course, char name[]);

int compareCourseByName(Course *course1, Course *course2);

void PrintCourse(Course *course);

typedef struct {
    Student *student;
    Course *course;
    float score;
    int term;
} Grade;

Grade *NewGrade(Student *student, Course *course, float score, int term);

int equalsGradeWithCourseCode(Grade *grade, int courseCode);

int hashStudent(HashTable *hashTable, void *content);

int hashCourse(HashTable *hashTable, void *content);

int AreDirectRelative(Course *course1, Course *course2);

int equalsGraphNodeCourse(GraphNode *graphNode, int courseCode);

int compareGraphNodeCourse(GraphNode *graphNode1, GraphNode *graphNode2);


Student *NewStudent(int number, char name[30]) {
    Student *student = (Student *) malloc(sizeof(Student));

    student->number = number;
    strcpy(student->name, name);
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
        printf("%d %d %0.1f\n", grade->course->code, grade->term, grade->score);
    }
}

Course *NewCourse(int code, char name[10]) {
    Course *course = (Course *) malloc(sizeof(Course));

    course->code = code;
    strcpy(course->name, name);
    course->students = NewDoubleLinkedList();

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
    printf("%d %s %d\n", course->code, course->name, course->students->size);
    for_each(element, course->students) {
        Student *student = (Student *) element->content;
        // TODO: refactor to speed up
        Grade *grade = (Grade *) SearchList(student->grades, (int (*)(void *, int)) equalsGradeWithCourseCode,
                                            course->code)->content;

        printf("%d %d %0.1f\n", student->number, grade->term, grade->score);
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

int hashStudent(HashTable *hashTable, void *content) {
    int id = ((Student *) content)->number;
    return ((hashTable->a * id + hashTable->b) % hashTable->prime) % hashTable->size;
}

int hashCourse(HashTable *hashTable, void *content) {
    int id = ((Course *) content)->code;
    return ((hashTable->a * id + hashTable->b) % hashTable->prime) % hashTable->size;
}

int AreDirectRelative(Course *course1, Course *course2) {
    int commonStudents = 0;

    for_each(element, course1->students) {
        if (SearchList(course2->students, (int (*)(void *, int)) equalsStudent,
                       ((Student *) element->content)->number) != NULL)
            commonStudents += 1;
    }

    if (course1->students->size > 0 && course2->students->size > 0 &&
        commonStudents > 0.5 * course1->students->size && commonStudents > 0.5 * course2->students->size)
        return 1;
    return 0;
}

int equalsGraphNodeCourse(GraphNode *graphNode, int courseCode) {
    return ((Course *) graphNode->content)->code == courseCode;
}

int compareGraphNodeCourse(GraphNode *graphNode1, GraphNode *graphNode2) {
    return compareCourse(graphNode1->content, graphNode2->content);
}


// COMMANDS ------------------------------------------------------------------------------------------------------------

void AddStudent(DoubleLinkedList *studentsList, BinaryTreeNode **studentsTree, HashTable *studentsTable);

void AddCourse(DoubleLinkedList *coursesList, BinaryTreeNode **coursesTree, HashTable *coursesTable);

void AddGrade(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList);

void EditStudent(DoubleLinkedList *studentsList, BinaryTreeNode **studentsTree);

void EditCourse(DoubleLinkedList *coursesList, BinaryTreeNode **coursesTree);

void EditGrade(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList);

void DeleteStudent(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, BinaryTreeNode **studentsTree,
                   HashTable *studentsTable);

void deleteStudentFromCourses(DoubleLinkedList *coursesList, Student *studentList);

void DeleteCourse(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, BinaryTreeNode **coursesTree,
                  HashTable *coursesTable);

void deleteCourseFromStudentGrades(DoubleLinkedList *studentsList, Course *courseList);

void DeleteGrade(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList);

void NumberOfCourses(DoubleLinkedList *studentsList);

void NumberOfStudentsOfCourse(DoubleLinkedList *coursesList);

Grade *findGrade(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, int studentNumber, int courseCode);

void SearchStudentByName(BinaryTreeNode *studentsTree);

void SearchCourseByName(BinaryTreeNode *coursesTree);

void SearchStudentByNumber(HashTable *studentsTable);

void SearchCourseByCode(HashTable *coursesTable);

Graph *BuildCourseRelationGraph(DoubleLinkedList *coursesList);

void IsRelative(Graph *courseGraph);

void AllRelatives(Graph *courseGraph);


void AddStudent(DoubleLinkedList *studentsList, BinaryTreeNode **studentsTree, HashTable *studentsTable) {
    int studentNumber;
    char name[50] = "";

    scanf("%d", &studentNumber);
    scanf("%s", name);

    Student *student = NewStudent(studentNumber, name);
    InsertToList(studentsList, student);
    *studentsTree = InsertToTree(*studentsTree, student, (int (*)(void *, void *)) compareStudentByName);
    InsertToHashTable(&studentsTable, student, hashStudent);
}

void AddCourse(DoubleLinkedList *coursesList, BinaryTreeNode **coursesTree, HashTable *coursesTable) {
    int code;
    char name[10] = "";

    scanf("%d", &code);
    scanf("%s", name);

    Course *course = NewCourse(code, name);
    InsertToList(coursesList, course);
    *coursesTree = InsertToTree(*coursesTree, course, (int (*)(void *, void *)) compareCourseByName);
    InsertToHashTable(&coursesTable, course, hashCourse);
}

void AddGrade(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList) {
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

    InsertToList(student->grades, grade);
    InsertToList(course->students, student);
}

void EditStudent(DoubleLinkedList *studentsList, BinaryTreeNode **studentsTree) {
    int studentNumber;
    char name[50] = "";

    scanf("%d", &studentNumber);
    scanf("%s", name);

    Student *student = (Student *) SearchList(studentsList, (int (*)(void *, int)) equalsStudent,
                                              studentNumber)->content;
    *studentsTree = TreePop(*studentsTree, student, (int (*)(void *, void *)) compareStudentByName);
    memset(student->name, 0, sizeof student->name);
    strcpy(student->name, name);
    *studentsTree = InsertToTree(*studentsTree, student, (int (*)(void *, void *)) compareStudentByName);
}

void EditCourse(DoubleLinkedList *coursesList, BinaryTreeNode **coursesTree) {
    int code;
    char name[10] = "";

    scanf("%d", &code);
    scanf("%s", name);

    Course *course = (Course *) SearchList(coursesList, (int (*)(void *, int)) equalsCourse, code)->content;

    *coursesTree = TreePop(*coursesTree, course, (int (*)(void *, void *)) compareCourseByName);
    memset(course->name, 0, sizeof course->name);
    strcpy(course->name, name);
    *coursesTree = InsertToTree(*coursesTree, course, (int (*)(void *, void *)) compareCourseByName);
}

void EditGrade(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList) {
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
                   HashTable *studentsTable) {
    int studentNumber;
    scanf("%d", &studentNumber);

    ListElement *studentElement = (ListElement *) SearchList(studentsList, (int (*)(void *, int)) equalsStudent,
                                                             studentNumber);

    RemoveElement(studentsList, studentElement);
    *studentsTree = TreePop(*studentsTree, studentElement->content, (int (*)(void *, void *)) compareStudentByName);
    HashTablePop(&studentsTable, (int (*)(void *, int)) equalsStudent, studentNumber, hashStudent);
    deleteStudentFromCourses(coursesList, studentElement->content);
}

void deleteStudentFromCourses(DoubleLinkedList *coursesList, Student *studentList) {
    for_each(element, studentList->grades) {
        Course *course = ((Grade *) element->content)->course;

        ListElement *studentElement = (ListElement *) SearchList(course->students,
                                                                 (int (*)(void *, int)) equalsStudent,
                                                                 studentList->number);
        RemoveElement(course->students, studentElement);
    }
}

void DeleteCourse(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, BinaryTreeNode **coursesTree,
                  HashTable *coursesTable) {
    int courseCode;
    scanf("%d", &courseCode);

    ListElement *courseElement = (ListElement *) SearchList(coursesList, (int (*)(void *, int)) equalsCourse,
                                                            courseCode);

    RemoveElement(coursesList, courseElement);
    *coursesTree = TreePop(*coursesTree, courseElement->content, (int (*)(void *, void *)) compareCourseByName);
    HashTablePop(&coursesTable, (int (*)(void *, int)) equalsCourse, courseCode, hashCourse);
    deleteCourseFromStudentGrades(studentsList, courseElement->content);
}

void deleteCourseFromStudentGrades(DoubleLinkedList *studentsList, Course *courseList) {
    for_each(element, courseList->students) {
        Student *student = (Student *) element->content;
        ListElement *gradeElement = (ListElement *) SearchList(student->grades,
                                                               (int (*)(void *, int)) equalsGradeWithCourseCode,
                                                               courseList->code);
        RemoveElement(student->grades, gradeElement);
    }
}

void DeleteGrade(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList) {
    int studentNumber;
    int courseCode;

    scanf("%d", &studentNumber);
    scanf("%d", &courseCode);

    Student *student = (Student *) SearchList(studentsList, (int (*)(void *, int)) equalsStudent,
                                              studentNumber)->content;
    ListElement *gradeElement = (ListElement *) SearchList(student->grades,
                                                           (int (*)(void *, int)) equalsGradeWithCourseCode,
                                                           courseCode);
    Course *course = ((Grade *) gradeElement->content)->course;
    ListElement *studentElement = (ListElement *) SearchList(course->students, (int (*)(void *, int)) equalsStudent,
                                                             studentNumber);

    RemoveElement(student->grades, gradeElement);
    RemoveElement(course->students, studentElement);

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

    printf("%d\n", course->students->size);
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

void IsRelative(Graph *courseGraph) {
    int courseCode1;
    scanf("%d", &courseCode1);

    int courseCode2;
    scanf("%d", &courseCode2);

    GraphNode *course1Node = (GraphNode *) SearchList(courseGraph->nodes, (int (*)(void *, int)) equalsGraphNodeCourse,
                                                      courseCode1)->content;
    GraphNode *course2Node = (GraphNode *) SearchList(courseGraph->nodes, (int (*)(void *, int)) equalsGraphNodeCourse,
                                                      courseCode2)->content;

    DoubleLinkedList *visited = NewDoubleLinkedList();

    if (IsDescendant(course1Node, course2Node, visited, (int (*)(void *, void *)) compareCourse))
        printf("%s\n", "yes");
    else
        printf("%s\n", "no");

    WhitenNodes(visited);
}

void AllRelatives(Graph *courseGraph) {
    int courseCode;
    scanf("%d", &courseCode);

    GraphNode *courseNode = (GraphNode *) SearchList(courseGraph->nodes, (int (*)(void *, int)) equalsGraphNodeCourse,
                                                     courseCode)->content;

    DoubleLinkedList *allRelatives = GetAllDescendants(courseNode, (int (*)(void *, void *)) compareGraphNodeCourse);
    WhitenNodes(allRelatives);

    printf("%d ", allRelatives->size - 1);
    for_each(element, allRelatives) {
        if (((Course *) ((GraphNode *) element->content)->content)->code != courseCode)
            printf("%d ", ((Course *) ((GraphNode *) element->content)->content)->code);
    }
    printf("\n");
}

// ---------------------------------------------------------------------------------------------------------------------

typedef struct {
    int phase2;
} Context;

Context *NewContext(int phase2);

Context *NewContext(int phase2) {
    Context *context = (Context *) malloc(sizeof(Context));

    context->phase2 = phase2;

    return context;
}

int main() {

    int numberOfCommands, a, b, prime;
    scanf("%d", &numberOfCommands);
    scanf("%d", &a);
    scanf("%d", &b);
    scanf("%d", &prime);

    Context *context = NewContext(prime == 1);

    DoubleLinkedList *studentsList = NewDoubleLinkedList();
    DoubleLinkedList *coursesList = NewDoubleLinkedList();
    BinaryTreeNode *studentsTree = NULL;
    BinaryTreeNode *coursesTree = NULL;
    HashTable *studentsTable = NewHashTable(4, a, b, prime, (float) 0.25);
    HashTable *coursesTable = NewHashTable(4, a, b, prime, (float) 0.25);
    Graph *courseRelationGraph = NULL;

    char command[20];
    for (int i = 0; i < numberOfCommands; i++) {
        scanf("%s", command);
        if (strcmp("ADDS", command) == 0) {
            AddStudent(studentsList, &studentsTree, studentsTable);
        } else if (strcmp("ADDC", command) == 0) {
            AddCourse(coursesList, &coursesTree, coursesTable);
        } else if (strcmp("ADDG", command) == 0) {
            AddGrade(studentsList, coursesList);
        } else if (strcmp("EDITS", command) == 0) {
            EditStudent(studentsList, &studentsTree);
        } else if (strcmp("EDITC", command) == 0) {
            EditCourse(coursesList, &coursesTree);
        } else if (strcmp("EDITG", command) == 0) {
            EditGrade(studentsList, coursesList);
        } else if (strcmp("DELETES", command) == 0) {
            DeleteStudent(studentsList, coursesList, &studentsTree, studentsTable);
        } else if (strcmp("DELETEC", command) == 0) {
            DeleteCourse(studentsList, coursesList, &coursesTree, coursesTable);
        } else if (strcmp("DELETEG", command) == 0) {
            DeleteGrade(studentsList, coursesList);
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
        } else {
            break;
        }
    }

    return 0;
}


