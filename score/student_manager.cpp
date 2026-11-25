#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <windows.h>

using namespace std;

//========================
// 学生结构体
//========================
struct Student
{
    string id;          // 学号
    string name;        // 姓名
    string sex;         // 性别
    string cls;         // 班级
    double average;     // 算术平均分
    double gpa;         // 平均学分绩点
};

//========================
// 全局变量
//========================
Student stu[300];
int count = 0;

//========================
// 读取文件
//========================
void loadFile()
{
    ifstream fin("grades.txt");

    if (!fin)
    {
        cout << "文件打开失败！" << endl;
        return;
    }

    string line;

    // 跳过前两行
    getline(fin, line);
    getline(fin, line);

    while (getline(fin, line))
    {
        Student temp;

        stringstream ss(line);

        // 用来接收不要的数据
        string majorID, majorName;
        int year;
        double totalScore;
        int courseNum;
        double totalCredit;
        double getCredit;
        int avgRank;
        double weightAvg;
        int weightRank;
        int gpaRank;
        int failNum;
        int examNum;

        ss >> temp.id
           >> temp.name
           >> temp.sex
           >> temp.cls
           >> majorID
           >> majorName
           >> year
           >> totalScore
           >> courseNum
           >> totalCredit
           >> getCredit
           >> temp.average
           >> avgRank
           >> weightAvg
           >> weightRank
           >> temp.gpa
           >> gpaRank
           >> failNum
           >> examNum;

        stu[count++] = temp;
    }

    fin.close();
}

//========================
// 显示全部学生
//========================
void showStudent()
{
    cout << left
         << setw(15) << "学号"
         << setw(10) << "姓名"
         << setw(15) << "班级"
         << setw(10) << "平均分"
         << setw(8) << "GPA"
         << endl;

    for (int i = 0; i < count; i++)
    {
        cout << left
             << setw(15) << stu[i].id
             << setw(10) << stu[i].name
             << setw(15) << stu[i].cls
             << setw(10) << stu[i].average
             << setw(8) << stu[i].gpa
             << endl;
    }
}//========================
// 第一组重载：查询学生
//========================
void findStudent(string name)
{
    for(int i = 0; i < count; i++)
    {
        if(stu[i].name == name)
        {
            cout << "\n找到该学生！" << endl;
            cout << "姓名：" << stu[i].name << endl;
            cout << "班级：" << stu[i].cls << endl;
            cout << "GPA：" << stu[i].gpa << endl;
            return;
        }
    }

    cout << "没有找到该学生！" << endl;
}

void findStudent()
{
    string name;

    cout << "请输入姓名：";
    cin >> name;

    findStudent(name);
}

//========================
// GPA冒泡排序
//========================
void bubbleSort()
{
    for(int i = 0; i < count - 1; i++)
    {
        for(int j = 0; j < count - 1 - i; j++)
        {
            if(stu[j].gpa < stu[j + 1].gpa)
            {
                Student temp = stu[j];
                stu[j] = stu[j + 1];
                stu[j + 1] = temp;
            }
        }
    }

    cout << "排序完成！" << endl;
}

//========================
// 第二组重载：修改学生GPA
//========================
void modifyStudent(string name)
{
    for(int i = 0; i < count; i++)
    {
        if(stu[i].name == name)
        {
            cout << "原GPA：" << stu[i].gpa << endl;

            cout << "请输入新的GPA：";
            cin >> stu[i].gpa;

            cout << "修改成功！" << endl;
            return;
        }
    }

    cout << "没有找到该学生！" << endl;
}

void modifyStudent()
{
    string name;

    cout << "请输入姓名：";
    cin >> name;

    modifyStudent(name);
}//========================
// 第三组重载：插入学生
//========================
void insertStudent(Student s)
{
    string name;

    cout << "请输入要插入到哪个学生前面的姓名：";
    cin >> name;

    int pos = count;

    for(int i = 0; i < count; i++)
    {
        if(stu[i].name == name)
        {
            pos = i;
            break;
        }
    }

    // 顺序表后移
    for(int i = count; i > pos; i--)
    {
        stu[i] = stu[i - 1];
    }

    stu[pos] = s;
    count++;

    cout << "插入成功！" << endl;
}

void insertStudent()
{
    Student s;

    cout << "请输入学号：";
    cin >> s.id;

    cout << "请输入姓名：";
    cin >> s.name;

    cout << "请输入性别：";
    cin >> s.sex;

    cout << "请输入班级：";
    cin >> s.cls;

    cout << "请输入平均分：";
    cin >> s.average;

    cout << "请输入GPA：";
    cin >> s.gpa;

    insertStudent(s);
}

//========================
// 第四组重载：删除学生
//========================
void deleteStudent(string name)
{
    for(int i = 0; i < count; i++)
    {
        if(stu[i].name == name)
        {
            // 顺序表前移
            for(int j = i; j < count - 1; j++)
            {
                stu[j] = stu[j + 1];
            }

            count--;

            cout << "删除成功！" << endl;
            return;
        }
    }

    cout << "没有找到该学生！" << endl;
}

void deleteStudent()
{
    string name;

    cout << "请输入姓名：";
    cin >> name;

    deleteStudent(name);
}

//========================
// 主函数
//========================
int main()
{
    loadFile();

    int choice;

    do
    {
        cout << "\n========== 学生成绩管理系统 ==========" << endl;
        cout << "1. 显示全部学生" << endl;
        cout << "2. 查询学生" << endl;
        cout << "3. GPA排序" << endl;
        cout << "4. 修改学生GPA" << endl;
        cout << "5. 插入学生" << endl;
        cout << "6. 删除学生" << endl;
        cout << "0. 退出" << endl;
        cout << "请选择：";
        cin >> choice;

        switch(choice)
        {
        case 1:
            showStudent();
            break;

        case 2:
            findStudent();
            break;

        case 3:
            bubbleSort();
            showStudent();
            break;

        case 4:
            modifyStudent();
            break;

        case 5:
            insertStudent();
            break;

        case 6:
            deleteStudent();
            break;

        case 0:
            cout << "程序结束！" << endl;
            break;

        default:
            cout << "输入有误，请重新输入！" << endl;
        }

    }while(choice != 0);

    return 0;
}