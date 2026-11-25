#include <iostream>
#include <string>
#include <algorithm>
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
};

//顺序类表
class SeqList {
private:
    static const int MAXSIZE = 100;   // 数组容量
    student_element data[MAXSIZE];    // 以数组作为线性表的存储结构
    int length;                       // 当前人数

public:
    SeqList()
    {
        length = 0;
        loadFromFile(DATA_FILE);   // 读成绩表，写构造函数
    }

    //a. 读成绩表
    bool loadFromFile(const string& filename)
    {
        ifstream fin(filename.c_str());
        if (!fin) { cout << "[错误] 打不开文件：" << filename << "\n"; return false; }
        length = 0;
        string line;
        getline(fin, line);   // 第一行是表头（学号 姓名 …）舍去
        while (getline(fin, line) && length < MAXSIZE) {
            if (line.empty()) continue;   // 空行跳过
            istringstream iss(line);
            student_element s;
            if (!(iss >> s.no >> s.name)) continue;
            bool ok = true;
            for (int i = 0; i < COURSE_NUM; ++i)
                if (!(iss >> s.course_score[i])) { ok = false; break; }
            if (!ok) continue;
            iss >> s.jidian >> s.index;   // 读学分绩点、排名
            data[length++] = s;
        }
        fin.close();
        return true;
    }

    int size() const { return length; }

    //显示成绩表
    void show() const
    {
        cout << "\n===== 班级学期成绩表（数组，共 " << length << " 人）=====\n";
        printHeader();
        for (int i = 0; i < length; ++i)
            printRecord(data[i].no, data[i].name, data[i].course_score,
                        data[i].jidian, data[i].index);
        cout << "\n";
    }

    //a.按学号或姓名查找，返回下标；找不到返回 -1
    int find(const string& key) const
    {
        for (int i = 0; i < length; ++i)
            if (data[i].no == key || data[i].name == key) return i;
        return -1;
    }

    //b. 按某门课程成绩排序（降序）
    bool sortByCourse(int c)   // c = 1..6
    {
        if (c < 1 || c > COURSE_NUM) return false;
        stable_sort(data, data + length,
                    [c](const student_element& a, const student_element& b) {
                        return a.course_score[c - 1] > b.course_score[c - 1];
                    });
        cout << "已按《" << courseNames[c - 1] << "》成绩从高到低排序。\n";
        return true;
    }

    //c. 新来一位同学插入到指定同学的前面或后面
    bool insert(const string& targetKey, bool before, const student_element& s)
    {
        if (length >= MAXSIZE) { cout << "[失败] 表已满。\n"; return false; }
        int pos = find(targetKey);
        if (pos < 0) { cout << "[失败] 找不到同学 " << targetKey << "。\n"; return false; }
        string targetInfo = data[pos].no + " " + data[pos].name;
        int insertPos = before ? pos : pos + 1;
        // 从后往前，把 insertPos 及之后的元素整体后移一位
        for (int i = length; i > insertPos; --i) data[i] = data[i - 1];
        data[insertPos] = s;
        ++length;
        cout << "已把 " << s.no << " " << s.name << " 插入到 " << targetInfo
             << " 的" << (before ? "前面" : "后面") << "。\n";
        return true;
    }

    //d. 删除某位同学
    bool remove(const string& key)
    {
        int pos = find(key);
        if (pos < 0) { cout << "[失败] 找不到同学 " << key << "。\n"; return false; }
        string info = data[pos].no + " " + data[pos].name;
        // 后面的元素整体前移，覆盖掉被删的那一位
        for (int i = pos; i < length - 1; ++i) data[i] = data[i + 1];
        --length;
        cout << "已删除：" << info << "。\n";
        return true;
    }

    // e. 修改某位同学某门课程的成绩
    bool modifyScore(const string& key, int c, int newScore)
    {
        if (c < 1 || c > COURSE_NUM) { cout << "[失败] 课程号非法。\n"; return false; }
        if (newScore < 0 || newScore > 100) { cout << "[失败] 成绩应在 0~100。\n"; return false; }
        int pos = find(key);
        if (pos < 0) { cout << "[失败] 找不到同学 " << key << "。\n"; return false; }
        int old = data[pos].course_score[c - 1];
        data[pos].course_score[c - 1] = newScore;
        cout << "已修改 " << data[pos].no << " " << data[pos].name << " 的《"
             << courseNames[c - 1] << "》成绩：" << old << " -> " << newScore << "。\n";
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
        return s;
    }
};

//菜单
static void menu()
{
    cout << "\n===== 实验一 1. 数组（顺序表）班级学期成绩表 =====\n";
    cout << "  1. 显示成绩表\n";
    cout << "  2. 按某门课程成绩排序\n";
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
    SeqList list;   // 构造函数里已读好成绩表
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
            int c = inputCourseNo();
            if (c > 0) { list.sortByCourse(c); list.show(); }
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
