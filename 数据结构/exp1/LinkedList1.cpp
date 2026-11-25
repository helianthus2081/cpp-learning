#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

#ifdef _WIN32
#include <windows.h>
#endif


const int COURSE_NUM = 6;
const string DATA_FILE = "scores.txt";

const string courseNames[COURSE_NUM] = {
    "Java", "C++", "工程图学", "军事理论", "近代史", "毛中特"
};

void printHeader() {
    cout << "学号  姓名  ";
    for (int i = 0; i < COURSE_NUM; i++)
        cout << courseNames[i] << "  ";
    cout << "学分绩点  排名" << endl;
}

void printRecord(string no, string name, const int score[COURSE_NUM],
                 float jidian, int rank) {
    cout << no << "  " << name;
    for (int i = 0; i < COURSE_NUM; i++)
        cout << "  " << score[i];
    cout << "  " << jidian << "  " << rank << endl;
}

int inputCourseNo() {
    cout << "请输入课程号（";
    for (int i = 0; i < COURSE_NUM; i++)
        cout << (i + 1) << "=" << courseNames[i] << " ";
    cout << "）：";
    int c;
    cin >> c;
    if (c < 1 || c > COURSE_NUM) {
        cout << "课程号非法。" << endl;
        return -1;
    }
    return c;
}


//数据元素
struct student_element {
    string no;                        // 学号
    string name;                      // 姓名
    int    course_score[COURSE_NUM];  // 6 门课程成绩
    float  jidian;                    // 学分绩点
    int    index;                     // 排名
    struct student_element* next;     // 指针域
};

//单链表类
class LinkedList {
private:
    student_element* head;   // 附加头结点，head->next 指向第一个数据元素
    int length;              // 结点个数

    // 新建一个结点，内容照抄 s，next 置空
    student_element* makeNode(const student_element& s) const
    {
        student_element* p = new student_element;
        p->no = s.no;
        p->name = s.name;
        for (int i = 0; i < COURSE_NUM; ++i) p->course_score[i] = s.course_score[i];
        p->jidian = s.jidian;
        p->index = s.index;
        p->next = NULL;
        return p;
    }

    // 释放所有结点
    void destroy()
    {
        student_element* p = head->next;
        while (p != NULL) {
            student_element* q = p->next;
            delete p;
            p = q;
        }
        head->next = NULL;
        length = 0;
    }

    // 按学号  或姓名查找，返回该结点；找不到返回 NULL
    student_element* findNode(const string& key) const
    {
        for (student_element* p = head->next; p != NULL; p = p->next)
            if (p->no == key || p->name == key) return p;
        return NULL;
    }

    // 找某结点的前驱（用于插到前面、删除）
    student_element* findPrev(student_element* t) const
    {
        for (student_element* p = head; p != NULL && p->next != NULL; p = p->next)
            if (p->next == t) return p;
        return NULL;
    }

public:
    LinkedList() : length(0)
    {
        head = new student_element;   // 附加头结点
        head->next = NULL;
        loadFromFile(DATA_FILE);      // 读成绩表，完成构造函数
    }
    ~LinkedList() { destroy(); delete head; }

    // a. 读成绩表，尾插法建表
    bool loadFromFile(const string& filename)
    {
        ifstream fin(filename.c_str());
        if (!fin) { cout << "[错误] 打不开文件：" << filename << "\n"; return false; }
        destroy();
        string line;
        getline(fin, line);   // 第一行是表头
        student_element* tail = head;
        while (getline(fin, line)) {
            if (line.empty()) continue;   // 空行跳过
            istringstream iss(line);
            student_element s;
            if (!(iss >> s.no >> s.name)) continue;
            bool ok = true;
            for (int i = 0; i < COURSE_NUM; ++i)
                if (!(iss >> s.course_score[i])) { ok = false; break; }
            if (!ok) continue;
            iss >> s.jidian >> s.index;   // 读学分绩点、排名
            student_element* node = makeNode(s);
            tail->next = node;            // 尾插
            tail = node;
            ++length;
        }
        fin.close();
        return true;
    }

    int size() const { return length; }

    // 显示成绩表
    void show() const
    {
        cout << "\n===== 班级学期成绩表（单链表，共 " << length << " 人）=====\n";
        printHeader();
        for (student_element* p = head->next; p != NULL; p = p->next)
            printRecord(p->no, p->name, p->course_score, p->jidian, p->index);
        cout << "\n";
    }

    // b. 查找指定同学，并显示这一条
    void findAndShow(const string& key) const
    {
        student_element* p = findNode(key);
        if (p == NULL) { cout << "[结果] 未找到 " << key << "。\n"; return; }
        printHeader();
        printRecord(p->no, p->name, p->course_score, p->jidian, p->index);
        cout << "\n";
    }

    // c. 新来一位同学插入到指定同学的前面或后面（指针操作）
    bool insert(const string& targetKey, bool before, const student_element& s)
    {
        student_element* target = findNode(targetKey);
        if (target == NULL) { cout << "[失败] 找不到同学 " << targetKey << "。\n"; return false; }
        student_element* node = makeNode(s);
        if (before) {
            student_element* prev = findPrev(target);
            node->next = target;   // 新结点指向目标
            prev->next = node;     // 前驱指向新结点
        } else {
            node->next = target->next;   // 新结点指向目标的后继
            target->next = node;         // 目标指向新结点
        }
        ++length;
        cout << "已把 " << s.no << " " << s.name << " 插入到 "
             << target->no << " " << target->name << " 的"
             << (before ? "前面" : "后面") << "。\n";
        return true;
    }

    // d. 删除某位同学
    bool remove(const string& key)
    {
        student_element* target = findNode(key);
        if (target == NULL) { cout << "[失败] 找不到同学 " << key << "。\n"; return false; }
        student_element* prev = findPrev(target);
        string info = target->no + " " + target->name;
        prev->next = target->next;   // 前驱越过被删结点
        delete target;               // 释放结点
        --length;
        cout << "已删除：" << info << "。\n";
        return true;
    }

    // e. 修改某位同学某门课程的成绩
    bool modifyScore(const string& key, int c, int newScore)
    {
        if (c < 1 || c > COURSE_NUM) { cout << "[失败] 课程号非法。\n"; return false; }
        if (newScore < 0 || newScore > 100) { cout << "[失败] 成绩应在 0~100。\n"; return false; }
        student_element* p = findNode(key);
        if (p == NULL) { cout << "[失败] 找不到同学 " << key << "。\n"; return false; }
        int old = p->course_score[c - 1];
        p->course_score[c - 1] = newScore;
        cout << "已修改 " << p->no << " " << p->name << " 的《" << courseNames[c - 1]
             << "》成绩：" << old << " -> " << newScore << "。\n";
        return true;
    }

    // 读入一位新同学的全部信息
    student_element inputNew()
    {
        student_element s;
        cout << "请输入新同学信息（学号 姓名 " << COURSE_NUM << "门成绩，空格分隔）: ";
        cin >> s.no >> s.name;
        for (int i = 0; i < COURSE_NUM; ++i) cin >> s.course_score[i];
        cout << "请输入该同学的学分绩点和排名: ";
        cin >> s.jidian >> s.index;
        s.next = NULL;
        return s;
    }
};

//菜单
static void menu()
{
    cout << "\n===== 实验一 2. 单链表 班级学期成绩表 =====\n";
    cout << "  1. 显示成绩表\n";
    cout << "  2. 查找指定同学\n";
    cout << "  3. 新同学插入到指定同学的前面\n";
    cout << "  4. 新同学插入到指定同学的后面\n";
    cout << "  5. 删除某位同学\n";
    cout << "  6. 修改某位同学某门课程的成绩\n";
    cout << "  0. 退出\n";
    cout << "请选择: ";
}

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);   // 控制台输出按 UTF-8（中文正常显示）
    SetConsoleCP(65001);         // 控制台输入也按 UTF-8（敲中文姓名才能和文件里对上）
#endif
    LinkedList list;   // 构造函数里已建头结点并读好成绩表
    cout << "已从 " << DATA_FILE << " 读入 " << list.size() << " 条记录。\n";

    int choice;
    while (true) {
        menu();
        if (!(cin >> choice)) { cin.clear(); cin.ignore(1024, '\n'); cout << "输入非法。\n"; continue; }
        if (choice == 0) break;
        switch (choice) {
        case 1:
            list.show();
            break;
        case 2: {
            string key;
            cout << "请输入要查找同学的学号或姓名: ";
            cin >> key;
            list.findAndShow(key);
            break;
        }
        case 3:
        case 4: {
            string target;
            cout << "请输入目标同学的学号或姓名: ";
            cin >> target;
            student_element s = list.inputNew();
            if (list.insert(target, choice == 3, s)) list.show();
            break;
        }
        case 5: {
            string key;
            cout << "请输入要删除同学的学号或姓名: ";
            cin >> key;
            if (list.remove(key)) list.show();
            break;
        }
        case 6: {
            string key;
            cout << "请输入同学的学号或姓名: ";
            cin >> key;
            int c = inputCourseNo();
            if (c <= 0) break;
            int v;
            cout << "请输入新的成绩(0-100): ";
            if (!(cin >> v)) { cin.clear(); cin.ignore(1024, '\n'); cout << "成绩非法。\n"; break; }
            if (list.modifyScore(key, c, v)) list.show();
            break;
        }
        default:
            cout << "无此选项。\n";
        }
    }
    cout << "程序结束。\n";
    return 0;
}
