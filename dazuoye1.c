#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    char id[20];
    char name[50];
    char major[50];
    char class[10];
    float scores[3];
} Student;

void Input(Student *students, int *count) {
    printf("请输入学号：");
    scanf("%s", students->id);
    printf("请输入姓名：");
    scanf("%s", students->name);
    printf("请输入专业：");
    scanf("%s", students->major);
    printf("请输入班级：");
    scanf("%s", students->class);
    for (int a = 0; a < 3; a++) {
        printf("请输入第%d门成绩：(0~100) ", a + 1);
        if (scanf("%f", &students->scores[a]) != 1) { // 如果输入不是浮点数
            printf("\033[31m输入无效，请输入数字！\033[0m\n");
            --a;
            while (getchar() != '\n')
                continue;
        } else if (students->scores[a] < 0 || students->scores[a] > 100) { // 限定成绩范围
            printf("\033[31m成绩范围应该在0～100之间，请重新输入！\033[0m\n");
            --a;
            continue;
        }
    }
    (*count)++;
}

void Output(Student *students, int count) {
    if (count == 0) {
        printf("没有学生信息\n");
        return;
    }
    printf("\n学生信息如下:\n");
    for (int i = 0; i < count; i++) {
        printf("学号:\033[33m %s\033[0m", students[i].id);
        printf(" 姓名:\033[33m %s\033[0m", students[i].name);
        printf(" 专业:\033[33m %s\033[0m", students[i].major);
        printf(" 班级:\033[33m %s\033[0m", students[i].class);
        for (int j = 0; j < 3; j++) {
            printf(" 第%d门成绩:\033[33m %.1f\033[0m", j + 1, students[i].scores[j]);
        }
        printf("\n");
    }
}

void Save(Student *students, int count) {
    FILE *fp = fopen("student_info.txt", "w"); // "w"表示写入模式
    for (int i = 0; i < count; i++) {
        fprintf(fp, "学号: %s", students[i].id);
        fprintf(fp, " 姓名: %s", students[i].name);
        fprintf(fp, " 专业: %s", students[i].major);
        fprintf(fp, " 班级: %s", students[i].class);
        for (int j = 0; j < 3; j++) {
            fprintf(fp, " 第%d门成绩: %.1f", j + 1, students[i].scores[j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    printf("\033[34m学生信息已保存到student_info.txt文件中\033[0m\n");
}

void Fetch(Student *students) {
    char searchId[20];
    printf("请输入要查询学生的学号：\n");
    scanf("%s", searchId);
    FILE *fp = fopen("student_info.txt", "r");
    if (fp == NULL) {
        printf("\033[31m无法打开文件 student_info.txt\033[0m\n");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        Student t;
        sscanf(line, "学号: %s 姓名: %s 专业: %s 班级: %s 第1门成绩: %f 第2门成绩: %f 第3门成绩: %f",
               t.id, t.name, t.major, t.class, &t.scores[0], &t.scores[1], &t.scores[2]);
        if (strcmp(t.id, searchId) == 0) {
            printf("该考号学生信息为：\n");
            printf("学号:\033[33m %s\033[0m", t.id);
            printf(" 姓名:\033[33m %s\033[0m", t.name);
            printf(" 专业:\033[33m %s\033[0m", t.major);
            printf(" 班级:\033[33m %s\033[0m", t.class);
            for (int j = 0; j < 3; j++) {
                printf(" 第%d门成绩:\033[33m %.1f\033[0m", j + 1, t.scores[j]);
            }
            printf("\n");
            fclose(fp);
            return;
        }
    }
    printf("\033[31m未找到该学号学生的信息！\033[0m\n");
    fclose(fp);
}

void Max(Student *students) {
    printf("\033[31m请输入要查询第几门（1～3）课程最高分\033[0m\n");
    int searchCo;
    scanf("%d", &searchCo);
    if (searchCo < 1 || searchCo > 3) {
        printf("\033[31m课程编号无效！\033[0m\n");
        return;
    }
    FILE *fp = fopen("student_info.txt", "r");
    if (fp == NULL) {
        printf("\033[31m无法打开文件 student_info.txt\033[0m\n");
        return;
    }
    float maxScore = -1;
    char maxName[50];
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        Student f;
        sscanf(line, "学号: %s 姓名: %s 专业: %s 班级: %s 第1门成绩: %f 第2门成绩: %f 第3门成绩: %f",
               f.id, f.name, f.major, f.class, &f.scores[0], &f.scores[1], &f.scores[2]);
        if (f.scores[searchCo - 1] > maxScore) {
            maxScore = f.scores[searchCo - 1];
            strcpy(maxName, f.name);
        }
    }
    if (maxScore == -1) {
        printf("\033[31m未找到该课程的成绩信息！\033[0m\n");
    } else {
        printf("第%d门课程的最高分是\033[33m %.1f\033[0m, 学生姓名是\033[33m %s\033[0m\n", searchCo, maxScore, maxName);
        printf("\n");
    }
    fclose(fp);
}

float getAverageScore(Student *student) {
    float total = 0;
    for (int i = 0; i < 3; i++) {
        total += student->scores[i];
    }
    return total / 3.0;
}

void Sort_select(Student *students, int count) {
    char major[50];
    printf("请输入要排序的专业：");
    scanf("%s", major);
    // 创建一个临时数组存储指定专业的学生
    Student *majorStudents = malloc(count * sizeof(Student));
    int majorCount = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].major, major) == 0) {
            majorStudents[majorCount++] = students[i];
        }
    }
    // 对指定专业的学生进行排序
    for (int i = 0; i < majorCount - 1; i++) {
        for (int j = i + 1; j < majorCount; j++) {
            if (getAverageScore(&majorStudents[i]) > getAverageScore(&majorStudents[j])) {
                Student temp = majorStudents[i];
                majorStudents[i] = majorStudents[j];
                majorStudents[j] = temp;
            }
        }
    }
    // 输出排序后的学生信息
    printf("\033[34m按总平均成绩由低到高排序后的学生信息（专业：%s）:\033[0m\n", major);
    for (int i = 0; i < majorCount; i++) {
        printf("学号:\033[33m %s\033[0m", majorStudents[i].id);
        printf(" 姓名:\033[33m %s\033[0m", majorStudents[i].name);
        printf(" 专业:\033[33m %s\033[0m", majorStudents[i].major);
        printf(" 班级:\033[33m %s\033[0m", majorStudents[i].class);
        for (int j = 0; j < 3; j++) {
            printf(" 第%d门成绩:\033[33m %.1f\033[0m", j + 1, majorStudents[i].scores[j]);
        }
        printf(" 平均成绩:\033[33m %.1f\033[0m\n", getAverageScore(&majorStudents[i]));
    }
    free(majorStudents);
}

void Sort_bubble(Student *students, int count) {
    char major[50];
    printf("请输入要排序的专业：");
    scanf("%s", major);
    Student *majorStudents = malloc(count * sizeof(Student));
    int majorCount = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].major, major) == 0) {
            majorStudents[majorCount++] = students[i];
        }
    }
    for (int i = 0; i < majorCount - 1; i++) {
        for (int j = 0; j < majorCount - 1 - i; j++) {
            if (getAverageScore(&majorStudents[j]) < getAverageScore(&majorStudents[j + 1])) {
                Student temp = majorStudents[j];
                majorStudents[j] = majorStudents[j + 1];
                majorStudents[j + 1] = temp;
            }
        }
    }
    printf("\033[34m按总平均成绩由高到低排序后的学生信息（专业：%s）:\033[0m\n", major);
    for (int i = 0; i < majorCount; i++) {
        printf("学号:\033[33m %s\033[0m", majorStudents[i].id);
        printf(" 姓名:\033[33m %s\033[0m", majorStudents[i].name);
        printf(" 专业:\033[33m %s\033[0m", majorStudents[i].major);
        printf(" 班级:\033[33m %s\033[0m", majorStudents[i].class);
        for (int j = 0; j < 3; j++) {
            printf(" 第%d门成绩:\033[33m %.1f\033[0m", j + 1, majorStudents[i].scores[j]);
        }
        printf(" 平均成绩:\033[33m %.1f\033[0m\n", getAverageScore(&majorStudents[i]));
    }
    free(majorStudents);
}

void Sort_insert(Student *students, int count) {
    if (count == 0) {
        printf("\033[31m当前没有学生信息，无法排序！\033[0m\n");
        return;
    }
    // 获取新输入学生的专业和班级
    char *major = students[count - 1].major;
    char *class = students[count - 1].class;
    int course;
    printf("请输入要排序的课程编号(1-3)：");
    if (scanf("%d", &course) != 1 || course < 1 || course > 3) {
        printf("\033[31m无效的课程编号！\033[0m\n");
        return;
    }
    course--;
    // 过滤出符合专业和班级的学生
    Student *filteredStudents = malloc(count * sizeof(Student));
    int filteredCount = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].major, major) == 0 && strcmp(students[i].class, class) == 0) {
            filteredStudents[filteredCount++] = students[i];
        }
    }
    // 插入排序
    for (int i = 1; i < filteredCount; i++) {
        Student temp = filteredStudents[i];
        int j = i - 1;
        while (j >= 0 && filteredStudents[j].scores[course] > temp.scores[course]) {
            filteredStudents[j + 1] = filteredStudents[j];
            j--;
        }
        filteredStudents[j + 1] = temp;
    }
    // 输出排序后的学生信息
    if (filteredCount > 0) {
        printf("\033[34m按第%d门成绩由低到高排序后的学生信息（专业：%s 班级：%s）:\033[0m\n", course + 1, major, class);
        for (int i = 0; i < filteredCount; i++) {
            printf("学号:\033[33m %s\033[0m", filteredStudents[i].id);
            printf(" 姓名:\033[33m %s\033[0m", filteredStudents[i].name);
            printf(" 专业:\033[33m %s\033[0m", filteredStudents[i].major);
            printf(" 班级:\033[33m %s\033[0m", filteredStudents[i].class);
            for (int j = 0; j < 3; j++) {
                printf(" 第%d门成绩:\033[33m %.1f\033[0m", j + 1, filteredStudents[i].scores[j]);
            }
            printf("\n");
        }
    } else {
        printf("\033[31m没有符合条件的学生信息！\033[0m\n");
    }
    free(filteredStudents);
}

float Gettotal(Student *student){
    return student->scores[0]+student->scores[1]+student->scores[2];
}

void Search(Student *students,int count){
    char major[50];
    char class[10];
    float total;
    printf("请输入要查找专业：\n");
    scanf("%s",major);
    printf("请输入要查找的班级：\n");
    scanf("%s",class);
    printf("您要查询三门课总分多少分以上的学生？\n");
    scanf("%f",&total);
    if(total<0||total>300){
        printf("\033[31m请输入有效范围0～300!\033[0m\n");
        return;
    }
    Student *majorClass = malloc(count * sizeof(Student));
    int majorCount = 0;
    for (int i = 0; i < count; i++) {
        if (Gettotal(&students[i])>total&&strcmp(class,students[i].class)==0&&strcmp(major,students[i].major)==0) {
            majorClass[majorCount++] = students[i];
        }
    }
    printf("%s 专业 %s 班总分 %.1f 分以上的学生如下：\n",major,class,total);
    for(int j=0;j<majorCount;j++){
    printf("\033[33m%s \033[0m",majorClass[j].name);
    printf("\n");
    }
    free(majorClass);
}

int main(){
Student *students=NULL;
int count=0;
char choice[10];
while(1){
printf("---学生信息管理系统---\n");
printf("\033[35m选择操作：\n");
printf("\033[32m1--输入学生信息\n");
printf("2--输出学生信息\n");
printf("3--保存学生信息\n");
printf("4--搜索学生信息\n");
printf("5--查询最高分学生及成绩\n");
printf("6--对某个专业的学生，按总平均成绩由低到高进行排序\n");
printf("7--对某个专业的学生，按总平均成绩由高到低进行排序\n");
printf("8--插入某个学生,按某门课成绩由低到高进行排序\n");
printf("9--实现专业班级和成绩的综合查找\n");
printf("10--关闭\033[0m\n");
scanf("%s",choice);
float choiceNum=atof(choice);
if(choiceNum==1){
    students=realloc(students,(count+1)*sizeof(Student));//重新分配内存，使其能够容纳更多学生的信息。每次增加一个学生时，数组的大小增加一个Student结构体的大小
    Input(&students[count],&count);
    continue;
}
else if(choiceNum==2){
    Output(students,count);
    continue;
}
else if(choiceNum==3){
    Save(students,count);
    continue;
}
else if(choiceNum==4){
    Fetch(students);
    continue;
}
else if(choiceNum==5){
    Max(students);
    continue;
}
else if(choiceNum==6){
    Sort_select(students,count);
    continue;
}
else if(choiceNum==7){
    Sort_bubble(students,count);
    continue;
}
else if (choiceNum == 8) {
    students = realloc(students, (count + 1) * sizeof(Student));
    Input(&students[count], &count);
    Sort_insert(students, count);
    continue;
}
else if(choiceNum==9){
    Search(students,count);
    continue;
}
else if(choiceNum==10){
    free(students);
    printf("\033[34m已关闭系统\033[0m\n");
    exit(0);    
}else{
    printf("\033[31m请输入正确的命令！\033[0m\n");
    continue;
  }
 }
return 0;
}