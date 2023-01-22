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

Node *RightRotate(Node *root);

Node *LeftRotate(Node *root);

Node *InsertToTree(Node *root, void *content, int (*compare)(void *, void *));

Node *rotate(Node *root, void *content, int (*compare)(void *, void *));

Node *Find(Node *root, void *content, int (*compare)(void *, void *));

Node *TreePop(Node *root, void *content, int (*compare)(void *, void *));

Node *getRightMostNode(Node *root);

Node *getLeftMostNode(Node *root);


Node *NewNode(void *content) {
    Node *node = (Node *) malloc(sizeof(Node));
    node->content = content;
    return node;
}

Node *RightRotate(Node *root) {
    Node *leftNode = root->left;
    Node *rightLeftNode = leftNode->right;

    leftNode->right = (struct Node *) root;
    root->left = (struct Node *) rightLeftNode;

    root->height = max(((Node *) root->left)->height, ((Node *) root->right)->height + 1);
    leftNode->height = max(((Node *) leftNode->left)->height, ((Node *) leftNode->right)->height + 1);

    return leftNode;
}

Node *LeftRotate(Node *root) {
    Node *rightNode = root->right;
    Node *leftRightNode = rightNode->left;

    rightNode->left = (struct Node *) root;
    root->right = (struct Node *) leftRightNode;

    root->height = max(((Node *) root->right)->height, ((Node *) root->left)->height + 1);
    rightNode->height = max(((Node *) rightNode->right)->height, ((Node *) rightNode->left)->height + 1);

    return rightNode;
}

Node *InsertToTree(Node *root, void *content, int (*compare)(void *, void *)) {
    if (root == NULL)
        return (NewNode(content));

    if (compare(root->content, content))
        root->left = (struct Node *) InsertToTree((Node *) root->left, content, compare);
    else
        root->right = (struct Node *) InsertToTree((Node *) root->right, content, compare);

    root->height = max(((Node *) root->right)->height, ((Node *) root->left)->height) + 1;

    return rotate(root, content, compare);
}

Node *rotate(Node *root, void *content, int (*compare)(void *, void *)) {
    int heightDifference = ((Node *) root->left)->height - ((Node *) root->right)->height;

    if (heightDifference > 1) {
        if (compare(((Node *) root->left)->content, content)) {
            return RightRotate(root);
        }
        root->left = (struct Node *) LeftRotate((Node *) root->left);
        return RightRotate(root);
    } else if (heightDifference < -1) {
        if (compare(content, ((Node *) root->left)->content)) {
            return LeftRotate(root);
        }
        root->right = (struct Node *) RightRotate((Node *) root->right);
        return LeftRotate(root);
    }

    return root;
}

Node *Find(Node *root, void *content, int (*compare)(void *, void *)) {
    if (compare(root->content, content) == 0)
        return root;

    if (compare(root->content, content))
        return Find((Node *) root->left, content, compare);

    return Find((Node *) root->right, content, compare);
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
    } else if (compare(root->content, content)) {
        Node *leftNode = TreePop((Node *) root->left, content, compare);
        root->left = (struct Node *) leftNode;
    } else {
        Node *rightNode = TreePop((Node *) root->right, content, compare);
        root->right = (struct Node *) rightNode;
    }

    rotate(root, content, compare); // TODO

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

typedef struct {
    int code;
    char name[10];
    DoubleLinkedList *students;
} Course;

Course *NewCourse(int code, char name[10]);

int equalsCourse(Course *course, int code);

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

void AddStudent(DoubleLinkedList *students);

void AddCourse(DoubleLinkedList *courses);

void AddGrade(DoubleLinkedList *students, DoubleLinkedList *courses);

void EditStudent(DoubleLinkedList *students);

void EditCourse(DoubleLinkedList *courses);

void EditGrade(DoubleLinkedList *students, DoubleLinkedList *courses);

void DeleteStudent(DoubleLinkedList *students, DoubleLinkedList *courses);

void deleteStudentFromCourses(DoubleLinkedList *courses, Student *student);

void DeleteCourse(DoubleLinkedList *students, DoubleLinkedList *courses);

void deleteCourseFromStudentGrades(DoubleLinkedList *students, Course *course);

void DeleteGrade(DoubleLinkedList *students, DoubleLinkedList *courses);

void NumberOfCourses(DoubleLinkedList *students);

void NumberOfStudentsOfCourse(DoubleLinkedList *courses);

Grade *findGrade(DoubleLinkedList *students, DoubleLinkedList *courses, int studentNumber, int courseCode);


void AddStudent(DoubleLinkedList *students) {
    int studentNumber;
    char name[50];

    scanf("%d", &studentNumber);
    scanf("%s", name);

    Student *student = NewStudent(studentNumber, name);
    addToList(students, student);
}

void AddCourse(DoubleLinkedList *courses) {
    int code;
    char name[10];

    scanf("%d", &code);
    scanf("%s", name);

    Course *course = NewCourse(code, name);
    addToList(courses, course);
}

void AddGrade(DoubleLinkedList *students, DoubleLinkedList *courses) {
    int studentNumber;
    int courseCode;
    int semesterCode;
    float score;

    scanf("%d", &studentNumber);
    scanf("%d", &courseCode);
    scanf("%d", &semesterCode);
    scanf("%f", &score);

    Student *student = (Student *) findElement(students, (int (*)(void *, int)) equalsStudent, studentNumber)->content;
    Course *course = (Course *) findElement(courses, (int (*)(void *, int)) equalsCourse, courseCode)->content;

    Grade *grade = NewGrade(student, course, score, semesterCode);

    addToList(student->grades, grade);
    addToList(course->students, student);
}

void EditStudent(DoubleLinkedList *students) {
    int studentNumber;
    char name[50];

    scanf("%d", &studentNumber);
    scanf("%s", name);

    Student *student = (Student *) findElement(students, (int (*)(void *, int)) equalsStudent, studentNumber)->content;

    strcpy(student->name, name);
}

void EditCourse(DoubleLinkedList *courses) {
    int code;
    char name[10];

    scanf("%d", &code);
    scanf("%s", name);

    Course *course = (Course *) findElement(courses, (int (*)(void *, int)) equalsCourse, code)->content;

    strcpy(course->name, name);
}

void EditGrade(DoubleLinkedList *students, DoubleLinkedList *courses) {
    int studentNumber;
    int courseCode;
    float score;

    scanf("%d", &studentNumber);
    scanf("%d", &courseCode);
    scanf("%f", &score);

    Grade *grade = findGrade(students, courses, studentNumber, courseCode);

    grade->score = score;
}

void DeleteStudent(DoubleLinkedList *students, DoubleLinkedList *courses) {
    int studentNumber;
    scanf("%d", &studentNumber);

    ListElement *studentElement = (ListElement *) findElement(students, (int (*)(void *, int)) equalsStudent,
                                                              studentNumber);

    RemoveElement(students, studentElement);
    deleteStudentFromCourses(courses, studentElement->content);
}

void deleteStudentFromCourses(DoubleLinkedList *courses, Student *student) {
    for_each(element, student->grades) {
        Course *course = ((Grade *) element->content)->course;

        ListElement *studentElement = (ListElement *) findElement(course->students,
                                                                  (int (*)(void *, int)) equalsStudent,
                                                                  student->number);
        RemoveElement(course->students, studentElement);
    }
}

void DeleteCourse(DoubleLinkedList *students, DoubleLinkedList *courses) {
    int courseCode;
    scanf("%d", &courseCode);

    ListElement *courseElement = (ListElement *) findElement(courses, (int (*)(void *, int)) equalsCourse, courseCode);

    RemoveElement(courses, courseElement);
    deleteCourseFromStudentGrades(students, courseElement->content);
}

void deleteCourseFromStudentGrades(DoubleLinkedList *students, Course *course) {
    for_each(element, course->students) {
        Student *student = (Student *) element->content;
        ListElement *gradeElement = (ListElement *) findElement(student->grades,
                                                                (int (*)(void *, int)) equalsGradeWithCourseCode,
                                                                course->code);
        RemoveElement(student->grades, gradeElement);
    }
}

void DeleteGrade(DoubleLinkedList *students, DoubleLinkedList *courses) {
    int studentNumber;
    int courseCode;

    scanf("%d", &studentNumber);
    scanf("%d", &courseCode);

    Student *student = (Student *) findElement(students, (int (*)(void *, int)) equalsStudent, studentNumber)->content;
    ListElement *gradeElement = (ListElement *) findElement(student->grades,
                                                            (int (*)(void *, int)) equalsGradeWithCourseCode,
                                                            courseCode);
    Course *course = ((Grade *) gradeElement->content)->course;
    ListElement *studentElement = (ListElement *) findElement(course->students, (int (*)(void *, int)) equalsStudent,
                                                              studentNumber);

    RemoveElement(student->grades, gradeElement);
    RemoveElement(course->students, studentElement);

}

void NumberOfCourses(DoubleLinkedList *students) {
    int studentNumber;
    scanf("%d", &studentNumber);

    Student *student = (Student *) findElement(students, (int (*)(void *, int)) equalsStudent, studentNumber)->content;

    printf("%d\n", student->grades->size);
}

void NumberOfStudentsOfCourse(DoubleLinkedList *courses) {
    int code;
    scanf("%d", &code);

    Course *course = (Course *) findElement(courses, (int (*)(void *, int)) equalsCourse, code)->content;

    printf("%d\n", course->students->size);
}

Grade *findGrade(DoubleLinkedList *students, DoubleLinkedList *courses, int studentNumber, int courseCode) {
    Student *student = (Student *) findElement(students, (int (*)(void *, int)) equalsStudent, studentNumber)->content;
    Grade *grade = (Grade *) findElement(student->grades, (int (*)(void *, int)) equalsGradeWithCourseCode,
                                         courseCode)->content;

    return grade;
}

// ---------------------------------------------------------------------------------------------------------------------

int main() {
    DoubleLinkedList *students = NewDoubleLinkedList();
    DoubleLinkedList *courses = NewDoubleLinkedList();

    char command[20];
    while (1) {
        scanf("%s", command);
        if (strcmp("ADDS", command) == 0) {
            AddStudent(students);
        } else if (strcmp("ADDC", command) == 0) {
            AddCourse(courses);
        } else if (strcmp("ADDG", command) == 0) {
            AddGrade(students, courses);
        } else if (strcmp("EDITS", command) == 0) {
            EditStudent(students);
        } else if (strcmp("EDITC", command) == 0) {
            EditCourse(courses);
        } else if (strcmp("EDITG", command) == 0) {
            EditGrade(students, courses);
        } else if (strcmp("DELETES", command) == 0) {
            DeleteStudent(students, courses);
        } else if (strcmp("DELETEC", command) == 0) {
            DeleteCourse(students, courses);
        } else if (strcmp("DELETEG", command) == 0) {
            DeleteGrade(students, courses);
        } else if (strcmp("NUMBERC", command) == 0) {
            NumberOfCourses(students);
        } else if (strcmp("NUMBERS", command) == 0) {
            NumberOfStudentsOfCourse(courses);
        } else if (strcmp("SEARCHSN", command) == 0) {

        } else if (strcmp("SEARCHCN", command) == 0) {

        } else if (strcmp("SEARCHSC", command) == 0) {

        } else if (strcmp("SEARCHCC", command) == 0) {

        } else {
            break;
        }
    }

    return 0;
}
