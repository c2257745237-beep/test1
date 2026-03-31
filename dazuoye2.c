#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct Student{
	char num[15];//学号
	char name[15];//姓名
	char major[10];//专业（computer,software,network）
	int classNo;//班级(1-2)
	int score[3];//3门课的成绩(1-3)
struct Student *next;//指向下一结点的指针
};
typedef struct Student STU;

void CreatList(STU **head) {
    STU *new_student = (STU *)malloc(sizeof(STU));
    printf("请输入学生学号: ");
    scanf("%s", new_student->num);
    printf("请输入学生姓名: ");
    scanf("%s", new_student->name);
    printf("请输入学生专业: ");
    scanf("%s", new_student->major);
    printf("请输入学生班级: ");
    scanf("%d", &new_student->classNo);
    for (int i = 0; i < 3; i++) {
        printf("请输入第%d门课的成绩(0~100): ", i + 1);
        scanf("%d", &new_student->score[i]);
    }
    new_student->next = NULL;
    if (*head == NULL || strcmp(new_student->num, (*head)->num) < 0) {
        new_student->next = *head;
        *head = new_student;
    } else {
        STU *current = *head;
        while (current->next != NULL && strcmp(current->next->num, new_student->num) < 0) {
            current = current->next;
        }
        new_student->next = current->next;
        current->next = new_student;
    }
}

void InsertList(STU **head, STU *new_student) {
    if (*head == NULL || strcmp(new_student->num, (*head)->num) < 0) {
        new_student->next = *head;
        *head = new_student;
    } else {
        STU *current = *head;
        while (current->next != NULL && strcmp(current->next->num, new_student->num) < 0) {
            current = current->next;
        }
        new_student->next = current->next;
        current->next = new_student;
    }
}

void Output(STU *head) {
    STU *current = head;
    while (current != NULL) {
        printf("学号:\033[33m %s\033[0m",current->num);
        printf(" 姓名:\033[33m %s\033[0m",current->name);
        printf(" 专业:\033[33m %s\033[0m",current->major);
        printf(" 班级:\033[33m %d\033[0m",current->classNo);
        for (int j = 0; j < 3; j++) {
            printf(" 第%d门成绩:\033[33m %d\033[0m", j + 1,current->score[j]);
        }
        printf("\n");
        current = current->next;
    }
}

void Save(STU *head) {
    FILE *fp = fopen("student_info(2).txt", "w"); // "w"表示写入模式
    STU *current = head;
    while (current != NULL) {
        fprintf(fp, "学号: %s", current->num);
        fprintf(fp, " 姓名: %s", current->name);
        fprintf(fp, " 专业: %s", current->major);
        fprintf(fp, " 班级: %d", current->classNo);
        for (int j = 0; j < 3; j++) {
            fprintf(fp, " 第%d门成绩: %d", j + 1, current->score[j]);
        }
        fprintf(fp, "\n");
        current = current->next;
    }
    fclose(fp);
    printf("\033[34m学生信息已保存到student_info(2).txt文件中\033[0m\n");
}

void Fetch(STU **head, int index) {
    FILE *fp = fopen("student_info(2).txt", "r");
    if (fp == NULL) {
        printf("\033[34m无法打开文件。\033[0m\n");
        return;
    }
    char line[256];
    int count = 0;
    STU *current = NULL;
    while (fgets(line, sizeof(line), fp)) {
        STU *new_student = (STU *)malloc(sizeof(STU));
        sscanf(line, "学号: %s 姓名: %s 专业: %s 班级: %d 第1门成绩: %d 第2门成绩: %d 第3门成绩: %d",
               new_student->num, new_student->name, new_student->major, &new_student->classNo,
               &new_student->score[0], &new_student->score[1], &new_student->score[2]);
        new_student->next = NULL;
        InsertList(head, new_student);
        if (count == index) {
            current = new_student;
        }
        count++;
    }
    fclose(fp);
    if (current != NULL) {
        printf("序号为 %d 的学生信息:\n", index + 1);
        printf("学号: \033[33m%s\033[0m", current->num);
        printf("姓名: \033[33m%s\033[0m", current->name);
        printf("专业: \033[33m%s\033[0m", current->major);
        printf("班级: \033[33m%d\033[0m", current->classNo);
        for (int j = 0; j < 3; j++) {
            printf("第%d门成绩: \033[33m%d\033[0m", j + 1, current->score[j]);
        }
        printf("\n");
    } else {
        printf("\033[34m文件中没有找到序号为 %d 的学生信息。\033[0m\n", index + 1);
    }
}

void Search_Num(STU *head,const char *num){
    STU *current = head;
    while (current != NULL) {
        if (strcmp(current->num, num) == 0) {
            printf("学号为 %s 的学生信息:\n",num);
            printf("学号: \033[33m%s\033[0m", current->num);
            printf("姓名: \033[33m%s\033[0m", current->name);
            printf("专业: \033[33m%s\033[0m", current->major);
            printf("班级: \033[33m%d\033[0m", current->classNo);
            for (int j = 0; j < 3; j++) {
                printf("第%d门成绩: \033[33m%d\033[0m", j + 1, current->score[j]);
            }
            printf("\n");
            return;
        }
        current = current->next;
    }
    printf("\033[34m未找到学号为 %s 的学生信息。\033[0m\n", num);
}

void Delete_Num(STU **head,const char *num){
    STU *current = *head;
    STU *previous = NULL;
    while (current != NULL && strcmp(current->num, num) != 0) {
        previous = current;
        current = current->next;
    }
    if (current == NULL) {
        printf("\033[34m未找到学号为 %s 的学生信息。\033[0m\n", num);
        return;
    }
    if (previous == NULL) {
        *head = current->next;
    } else {
        previous->next = current->next;
    }
    free(current);
    printf("\033[34m学号为 %s 的学生信息已删除。\033[0m\n", num);
}

void Search_score(STU *head, const char *major, int course_index, int score_limit) {
    STU *current = head;
    int found = 0;
    while (current != NULL) {
        if (strcmp(current->major, major) == 0 && current->score[course_index - 1] < score_limit) {
            if (!found) {
                printf("找到以下学生的信息:\n");
                found = 1;
            }
            printf("学号: \033[33m%s\033[0m", current->num);
            printf(" 姓名: \033[33m%s\033[0m", current->name);
            printf(" 专业: \033[33m%s\033[0m", current->major);
            printf(" 班级: \033[33m%d\033[0m", current->classNo);
            for (int j = 0; j < 3; j++) {
                printf(" 第%d门成绩: \033[33m%d\033[0m", j + 1, current->score[j]);
            }
            printf("\n");
        }
        current = current->next;
    }
    if (!found) {
        printf("\033[34m未找到专业为 %s，且第 %d 门课程成绩小于 %d 的学生信息。\033[0m\n", major, course_index, score_limit);
    }
}

void Delete_score(STU **head, const char *major, int classNo, int course_index, int score_limit) {
    STU *current = *head;
    STU *previous = NULL;
    while (current != NULL) {
        if (strcmp(current->major, major) == 0 && current->classNo == classNo && current->score[course_index - 1] < score_limit) {
            if (previous == NULL) {
                *head = current->next;
            } else {
                previous->next = current->next;
            }
            STU *temp = current;
            current = current->next;
            free(temp);
            printf("已删除专业为 %s，班级为 %d，第 %d 门课程成绩小于 %d 的学生信息。\n", major, classNo, course_index, score_limit);
        } else {
            previous = current;
            current = current->next;
        }
    }
}

int main(){
STU *students=NULL;
int count=0;
char choice[10];
while(1){
printf("---学生信息管理系统---\n");
printf("\033[35m选择操作：\n");
printf("\033[32m1--从键盘输入学生信息，创建按学号有序的链表。\n");
printf("2--将学生信息格式化输出。\n");
printf("3--将学生信息存入文件。\n");
printf("4--从文件中按序号读取并输出某个学生的信息。\n");
printf("5--查找指定学号的学生。\n");
printf("6--输入一个学生的信息，将该学生信息插入到链表中的相应位置，并保持链表有序性。\n");
printf("7--从链表中删除指定学号的学生。\n");
printf("8--查找某个专业某门课程的成绩小于某个分数的学生。\n");
printf("9--从链表中删除某个专业某个班级某门课程的成绩小于某个分数的学生。\n");
printf("10--关闭\033[0m\n");
scanf("%s",choice);
float choiceNum=atof(choice);
if(choiceNum==1){
    count++;
    CreatList(&students);
}
else if(choiceNum==2){
    Output(students);
}
else if(choiceNum==3){
    Save(students);
}
else if(choiceNum==4){
    int index;
    printf("请输入要查找的序号：");
    scanf("%d",&index);
    Fetch(&students,index-1);
}
else if(choiceNum==5){
    char num[15];
    printf("请输入要查找的学号：\n");
    scanf("%s",num);
    Search_Num(students,num);
}
else if(choiceNum==6){
    STU *new_student=(STU*)malloc(sizeof(STU));
    printf("请输入学生学号: ");
    scanf("%s", new_student->num);
    printf("请输入学生姓名: ");
    scanf("%s", new_student->name);
    printf("请输入学生专业: ");
    scanf("%s", new_student->major);
    printf("请输入学生班级: ");
    scanf("%d", &new_student->classNo);
        for (int i = 0; i < 3; i++) {
            printf("请输入第%d门课的成绩(0~100): ", i + 1);
            scanf("%d", &new_student->score[i]);
        }
    new_student->next=NULL;
    InsertList(&students,new_student);
}
else if(choiceNum==7){
    char num[15];
    printf("请输入要删除学生的考号：\n");
    scanf("%s",num);
    Delete_Num(&students,num);
}
else if (choiceNum == 8) {
    char major[10];
    int course_index,score_limit;
    printf("请输入查找的专业：");
    scanf("%s",major);
    printf("请输入要查找的课程编号：");
    scanf("%d",&course_index);
    printf("请输入要查找小于多少分的学生：");
    scanf("%d",&score_limit);
    Search_score(students,major,course_index,score_limit);
}
else if(choiceNum==9){
    char major[10];
    int classNo,course_index,score_limit;
    printf("请输入要删除学生的专业：");
    scanf("%s",major);
    printf("请输入要删除学生的班级：");
    scanf("%d",&classNo);
    printf("请输入要删除的课程序号：");
    scanf("%d",&course_index);
    printf("请输入分数限制");
    scanf("%d",&score_limit);
    Delete_score(&students,major,classNo,course_index,score_limit);
}
else if(choiceNum==10){

    printf("\033[34m已关闭系统\033[0m\n");
    exit(0);    
}else{
    printf("\033[31m请输入正确的命令！\033[0m\n");
    continue;
  }
 }
return 0;
}