#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <windows.h>
using namespace std;

class Student
{
private:
    string id;          // 学号
    string name;        // 姓名
    string className;   // 班级
    double gpa;         // 平均学分绩点

public:
    // 构造函数
    Student(string i, string n, string c, double g)
    {
        id = i;
        name = n;
        className = c;
        gpa = g;
    }

    // 输出学生信息
    void show()
    {
        cout << left << setw(18) << id
             << setw(12) << name
             << setw(18) << className
             << fixed << setprecision(3) << gpa << endl;
    }
};

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    ifstream fin("grades.txt");

    if (!fin)
    {
        cout << "File open error!" << endl;
        return 1;
    }

    string line, id, name, gender, className;
    string temp;
    double gpa;

    // 跳过标题和表头
    getline(fin, line);
    getline(fin, line);

    cout << "ID\t\tName\t\tClass\t\tGPA" << endl;

    while (getline(fin, line))
    {
        stringstream ss(line);

        // 前4列：学号、姓名、性别、班级
        ss >> id >> name >> gender >> className;

        // 跳过第5～15列
        for (int i = 0; i < 11; i++)
            ss >> temp;

        // 第16列：平均学分绩点
        ss >> gpa;

        Student stu(id, name, className, gpa);
        stu.show();
    }

    fin.close();
    return 0;
}