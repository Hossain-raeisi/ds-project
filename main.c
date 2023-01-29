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

void addToList(DoubleLinkedList *doubleLinkedList, void *value);

ListElement *iterOnList(ListElement *first, int iterations);

ListElement *ListPop(DoubleLinkedList *doubleLinkedList, void *value);

ListElement *ListPopIndex(DoubleLinkedList *doubleLinkedList, int index);

void RemoveElement(DoubleLinkedList *doubleLinkedList, ListElement *listElement);

ListElement *findElement(DoubleLinkedList *doubleLinkedList, int (*equals)(void *, int), int id);

DoubleLinkedList *NewDoubleLinkedList() {
    DoubleLinkedList *list = (DoubleLinkedList *) malloc(sizeof(DoubleLinkedList));

    list->size = 0;
    list->first = NULL;
    list->last = NULL;

    return list;
}

void addToList(DoubleLinkedList *doubleLinkedList, void *value) {
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
    }

    if (listElement->next != NULL) {
        ((ListElement *) listElement->next)->before = listElement->before;
    }
    doubleLinkedList->size -= 1;
}

ListElement *findElement(DoubleLinkedList *doubleLinkedList, int (*equals)(void *, int), int id) {
    ListElement *element = doubleLinkedList->first;

    for (int i = 0; i < doubleLinkedList->size; i++) {
        if (equals(element->content, id)) {
            return element;
        }
        element = iterOnList(element, 1);
    }

    return NULL;
}

// AVL TREE ---------------------------------------------------------------------------------------------------------

typedef struct {
    void *content;
    struct Node *left;
    struct Node *right;
    int height;
} Node;

Node *NewNode(void *content);

int getNodeHeight(Node *node);

Node *LeftRotate(Node *root);

Node *RightRotate(Node *root);

Node *InsertToTree(Node *root, void *content, int (*compare)(void *, void *));

Node *rotate(Node *root, int (*compare)(void *, void *));

Node *Find(Node *root, char name[], int (*compareByName)(void *, char[]));

Node *TreePop(Node *root, void *content, int (*compare)(void *, void *));

Node *getRightMostNode(Node *root);

Node *getLeftMostNode(Node *root);


Node *NewNode(void *content) {
    Node *node = (Node *) malloc(sizeof(Node));
    node->content = content;
    node->height = 1;
    return node;
}

int getNodeHeight(Node *node) {
    if (node != NULL)
        return node->height;
    return 0;
}

Node *LeftRotate(Node *root) {
    Node *leftNode = (Node *) root->left;
    Node *rightLeftNode = NULL;
    if (leftNode!= NULL)
        rightLeftNode = (Node *) leftNode->right;

    leftNode->right = (struct Node *) root;
    root->left = (struct Node *) rightLeftNode;

    root->height = max(getNodeHeight((Node *) root->left), getNodeHeight((Node *) root->right) + 1);
    leftNode->height = max(getNodeHeight((Node *) leftNode->left), getNodeHeight((Node *) leftNode->right) + 1);

    return leftNode;
}

Node *RightRotate(Node *root) {
    Node *rightNode = (Node *) root->right;
    Node *leftRightNode = NULL;
    if (rightNode != NULL)
        leftRightNode = (Node *) rightNode->left;

    rightNode->left = (struct Node *) root;
    root->right = (struct Node *) leftRightNode;

    root->height = max(getNodeHeight((Node *) root->right), getNodeHeight((Node *) root->left) + 1);
    rightNode->height = max(getNodeHeight((Node *) rightNode->right), getNodeHeight((Node *) rightNode->left) + 1);

    return rightNode;
}

Node *InsertToTree(Node *root, void *content, int (*compare)(void *, void *)) {
    if (root == NULL)
        return (NewNode(content));

    if (compare(root->content, content) == 1)
        root->left = (struct Node *) InsertToTree((Node *) root->left, content, compare);
    else
        root->right = (struct Node *) InsertToTree((Node *) root->right, content, compare);

    root->height = max(getNodeHeight((Node *) root->right), getNodeHeight((Node *) root->left)) + 1;

    return rotate(root, compare);
}

Node *rotate(Node *root, int (*compare)(void *, void *)) {
    int heightDifference = getNodeHeight((Node *) root->left) - getNodeHeight((Node *) root->right);

    if (heightDifference < -1) {
        if (getNodeHeight((Node *)((Node *) root->right)->right) > getNodeHeight((Node *)((Node *) root->right)->left))
            return RightRotate(root);
        root->right = (struct Node *) LeftRotate((Node *) root->right);
        return RightRotate(root);
    } else if (heightDifference > 1) {
        if (getNodeHeight((Node *)((Node *) root->left)->left) > getNodeHeight((Node *)((Node *) root->left)->right))
            return LeftRotate(root);
        root->left = (struct Node *) RightRotate((Node *) root->left);
        return LeftRotate(root);
    }

    return root;
}

Node *Find(Node *root, char name[], int (*compareByName)(void *, char[])) {
    if (compareByName(root->content, name) == 0)
        return root;

    if (compareByName(root->content, name) == 1)
        return Find((Node *) root->left, name, compareByName);

    return Find((Node *) root->right, name, compareByName);
}

Node *TreePop(Node *root, void *content, int (*compare)(void *, void *)) {
    if (compare(root->content, content) == 0) {
        if (root->right != NULL) {
            Node *newRoot = getLeftMostNode((Node *) root->right);
            root->content = newRoot->content;
            TreePop((Node *) root->right, newRoot->content, compare);
        } else if (root->left != NULL) {
            Node *newRoot = getRightMostNode((Node *) root->left);
            root->content = newRoot->content;
            TreePop((Node *) root->left, newRoot->content, compare);
        } else {
            root = NULL;
        }
    } else if (compare(root->content, content) == 1) {
        Node *leftNode = TreePop((Node *) root->left, content, compare);
        root->left = (struct Node *) leftNode;
    } else {
        Node *rightNode = TreePop((Node *) root->right, content, compare);
        root->right = (struct Node *) rightNode;
    }

    if (root != NULL) {
        root->height = max(getNodeHeight((Node *) root->left), getNodeHeight((Node *) root->right) + 1);
        rotate(root, compare);
    }

    return root;
}

Node *getRightMostNode(Node *root) {
    if (root->right == NULL)
        return root;
    return getRightMostNode((Node *) root->right);
}

Node *getLeftMostNode(Node *root) {
    if (root->left == NULL)
        return root;
    return getLeftMostNode((Node *) root->left);
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

void PrintCourse(Course *course);

typedef struct {
    Student *student;
    Course *course;
    float score;
    int term;
} Grade;

Grade *NewGrade(Student *student, Course *course, float score, int term);

int equalsGradeWithCourseCode(Grade *grade, int courseCode);

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

void PrintCourse(Course *course) {
    printf("%d %s %d\n", course->code, course->name, course->students->size);
    for_each(element, course->students) {
        Student *student = (Student *) element->content;
        // TODO: refactor to speed up
        Grade *grade = (Grade *) findElement(student->grades, (int (*)(void *, int)) equalsGradeWithCourseCode,
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

// COMMANDS ------------------------------------------------------------------------------------------------------------

void AddStudent(DoubleLinkedList *studentsList, Node **studentsTree);

void AddCourse(DoubleLinkedList *coursesList, Node **coursesTree);

void AddGrade(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList);

void EditStudent(DoubleLinkedList *studentsList);

void EditCourse(DoubleLinkedList *coursesList);

void EditGrade(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList);

void DeleteStudent(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, Node **studentsTree);

void deleteStudentFromCourses(DoubleLinkedList *coursesList, Student *studentList);

void DeleteCourse(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, Node **coursesTree);

void deleteCourseFromStudentGrades(DoubleLinkedList *studentsList, Course *courseList);

void DeleteGrade(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList);

void NumberOfCourses(DoubleLinkedList *studentsList);

void NumberOfStudentsOfCourse(DoubleLinkedList *coursesList);

Grade *findGrade(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, int studentNumber, int courseCode);

void SearchStudentByName(Node *studentsTree);

void SearchCourseByName(Node *coursesTree);

void AddStudent(DoubleLinkedList *studentsList, Node **studentsTree) {
    int studentNumber;
    char name[50];

    scanf("%d", &studentNumber);
    scanf("%s", name);

    Student *student = NewStudent(studentNumber, name);
    addToList(studentsList, student);
    *studentsTree = InsertToTree(*studentsTree, student, (int (*)(void *, void *)) compareStudent);
}

void AddCourse(DoubleLinkedList *coursesList, Node **coursesTree) {
    int code;
    char name[10];

    scanf("%d", &code);
    scanf("%s", name);

    Course *course = NewCourse(code, name);
    addToList(coursesList, course);
    *coursesTree = InsertToTree(*coursesTree, course, (int (*)(void *, void *)) compareCourse);
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

    Student *student = (Student *) findElement(studentsList, (int (*)(void *, int)) equalsStudent,
                                               studentNumber)->content;
    Course *course = (Course *) findElement(coursesList, (int (*)(void *, int)) equalsCourse, courseCode)->content;

    Grade *grade = NewGrade(student, course, score, semesterCode);

    addToList(student->grades, grade);
    addToList(course->students, student);
}

void EditStudent(DoubleLinkedList *studentsList) {
    int studentNumber;
    char name[50];

    scanf("%d", &studentNumber);
    scanf("%s", name);

    Student *student = (Student *) findElement(studentsList, (int (*)(void *, int)) equalsStudent,
                                               studentNumber)->content;

    strcpy(student->name, name);
}

void EditCourse(DoubleLinkedList *coursesList) {
    int code;
    char name[10];

    scanf("%d", &code);
    scanf("%s", name);

    Course *course = (Course *) findElement(coursesList, (int (*)(void *, int)) equalsCourse, code)->content;

    strcpy(course->name, name);
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

void DeleteStudent(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, Node **studentsTree) {
    int studentNumber;
    scanf("%d", &studentNumber);

    ListElement *studentElement = (ListElement *) findElement(studentsList, (int (*)(void *, int)) equalsStudent,
                                                              studentNumber);

    RemoveElement(studentsList, studentElement);
    *studentsTree = TreePop(*studentsTree, studentElement->content, (int (*)(void *, void *)) compareStudent);
    deleteStudentFromCourses(coursesList, studentElement->content);
}

void deleteStudentFromCourses(DoubleLinkedList *coursesList, Student *studentList) {
    for_each(element, studentList->grades) {
        Course *course = ((Grade *) element->content)->course;

        ListElement *studentElement = (ListElement *) findElement(course->students,
                                                                  (int (*)(void *, int)) equalsStudent,
                                                                  studentList->number);
        RemoveElement(course->students, studentElement);
    }
}

void DeleteCourse(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, Node **coursesTree) {
    int courseCode;
    scanf("%d", &courseCode);

    ListElement *courseElement = (ListElement *) findElement(coursesList, (int (*)(void *, int)) equalsCourse,
                                                             courseCode);

    RemoveElement(coursesList, courseElement);
    *coursesTree = TreePop(*coursesTree, courseElement->content, (int (*)(void *, void *)) compareCourse);
    deleteCourseFromStudentGrades(studentsList, courseElement->content);
}

void deleteCourseFromStudentGrades(DoubleLinkedList *studentsList, Course *courseList) {
    for_each(element, courseList->students) {
        Student *student = (Student *) element->content;
        ListElement *gradeElement = (ListElement *) findElement(student->grades,
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

    Student *student = (Student *) findElement(studentsList, (int (*)(void *, int)) equalsStudent,
                                               studentNumber)->content;
    ListElement *gradeElement = (ListElement *) findElement(student->grades,
                                                            (int (*)(void *, int)) equalsGradeWithCourseCode,
                                                            courseCode);
    Course *course = ((Grade *) gradeElement->content)->course;
    ListElement *studentElement = (ListElement *) findElement(course->students, (int (*)(void *, int)) equalsStudent,
                                                              studentNumber);

    RemoveElement(student->grades, gradeElement);
    RemoveElement(course->students, studentElement);

}

void NumberOfCourses(DoubleLinkedList *studentsList) {
    int studentNumber;
    scanf("%d", &studentNumber);

    Student *student = (Student *) findElement(studentsList, (int (*)(void *, int)) equalsStudent,
                                               studentNumber)->content;

    printf("%d\n", student->grades->size);
}

void NumberOfStudentsOfCourse(DoubleLinkedList *coursesList) {
    int code;
    scanf("%d", &code);

    Course *course = (Course *) findElement(coursesList, (int (*)(void *, int)) equalsCourse, code)->content;

    printf("%d\n", course->students->size);
}

Grade *findGrade(DoubleLinkedList *studentsList, DoubleLinkedList *coursesList, int studentNumber, int courseCode) {
    Student *student = (Student *) findElement(studentsList, (int (*)(void *, int)) equalsStudent,
                                               studentNumber)->content;
    Grade *grade = (Grade *) findElement(student->grades, (int (*)(void *, int)) equalsGradeWithCourseCode,
                                         courseCode)->content;

    return grade;
}

void SearchStudentByName(Node *studentsTree) {
    char name[50];
    scanf("%s", name);

    Student *student = (Student *) Find(studentsTree, name, (int (*)(void *, char *)) compareStudentAndName)->content;
    PrintStudent(student);
}

void SearchCourseByName(Node *coursesTree) {
    char name[10];
    scanf("%s", name);

    Course *course = (Course *) Find(coursesTree, name, (int (*)(void *, char *)) compareCourseAndName);
    PrintCourse(course);
}

// ---------------------------------------------------------------------------------------------------------------------

int main() {
    DoubleLinkedList *studentsList = NewDoubleLinkedList();
    DoubleLinkedList *coursesList = NewDoubleLinkedList();
    Node *studentsTree = NULL;
    Node *coursesTree = NULL;

    char command[20];
    while (1) {
        scanf("%s", command);
        if (strcmp("ADDS", command) == 0) {
            AddStudent(studentsList, &studentsTree);
        } else if (strcmp("ADDC", command) == 0) {
            AddCourse(coursesList, &coursesTree);
        } else if (strcmp("ADDG", command) == 0) {
            AddGrade(studentsList, coursesList);
        } else if (strcmp("EDITS", command) == 0) {
            EditStudent(studentsList);
        } else if (strcmp("EDITC", command) == 0) {
            EditCourse(coursesList);
        } else if (strcmp("EDITG", command) == 0) {
            EditGrade(studentsList, coursesList);
        } else if (strcmp("DELETES", command) == 0) {
            DeleteStudent(studentsList, coursesList, &studentsTree);
        } else if (strcmp("DELETEC", command) == 0) {
            DeleteCourse(studentsList, coursesList, &coursesTree);
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
            return 0;
        } else if (strcmp("SEARCHCC", command) == 0) {
            return 0;
        } else {
            break;
        }
    }

    return 0;
}
