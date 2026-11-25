# C++ 作业一：成绩单程序详细讲解

> 题目要求：  
> 设计成绩单对应的抽象数据类型；利用构造函数、对象实例化；读取成绩单数据；以列表形式输出成绩单内容。

---

# 1. 完整代码（详细注释版）

```cpp
#include <iostream>      // 输入输出：cout、cin、endl
#include <fstream>       // 文件操作：ifstream
#include <sstream>       // 字符串流：stringstream
#include <string>        // 字符串类型：string
#include <iomanip>       // 格式化输出：setw、fixed、setprecision
#include <windows.h>     // Windows API，用于设置控制台 UTF-8 编码

using namespace std;

// 定义 Student 类
// class 可以理解为“自己定义一种新的数据类型”
// 这里 Student 就代表“学生”这种类型
class Student
{
private:
    // private：私有成员
    // 这些变量只能在 Student 类内部直接访问
    string id;          // 学号
    string name;        // 姓名
    string className;   // 班级
    double gpa;         // 平均学分绩点

public:
    // 构造函数
    // 作用：创建 Student 对象时，给成员变量赋初值
    // 构造函数特点：
    // 1. 函数名和类名完全相同
    // 2. 没有返回值类型，连 void 都不能写
    // 3. 创建对象时会自动调用
    Student(string i, string n, string c, double g)
    {
        id = i;          // 把参数 i 赋给成员变量 id
        name = n;        // 把参数 n 赋给成员变量 name
        className = c;   // 把参数 c 赋给成员变量 className
        gpa = g;         // 把参数 g 赋给成员变量 gpa
    }

    // 成员函数：输出学生信息
    void show()
    {
        // left：左对齐
        // setw(18)：设置当前输出字段宽度为 18
        // fixed：使用定点小数形式
        // setprecision(3)：小数点后保留 3 位

        cout << left << setw(18) << id
             << setw(12) << name
             << setw(18) << className
             << fixed << setprecision(3) << gpa
             << endl;
    }
};

int main()
{
    // 设置 Windows 控制台输出编码为 UTF-8
    // 主要是为了减少中文乱码问题
    SetConsoleOutputCP(CP_UTF8);

    // 创建文件输入流对象 fin，并打开 grades.txt
    ifstream fin("grades.txt");

    // 判断文件是否打开成功
    // !fin 表示文件流无效，也就是打开失败
    if (!fin)
    {
        cout << "File open error!" << endl;

        // return 1 表示程序异常结束
        return 1;
    }

    // line：保存文件中的一整行
    // id：学号
    // name：姓名
    // gender：性别
    // className：班级
    string line, id, name, gender, className;

    // temp：临时变量，用来接收我们不需要的列
    string temp;

    // gpa：平均学分绩点
    double gpa;

    // grades.txt 前两行是标题和表头
    // getline(fin, line) 表示从文件 fin 中读取一整行，存到 line 中
    // 这里读取后不使用，所以相当于“跳过”
    getline(fin, line);
    getline(fin, line);

    // 输出表头
    cout << "ID\t\tName\t\tClass\t\tGPA" << endl;

    // 只要还能从文件中成功读取一行，就继续循环
    while (getline(fin, line))
    {
        // 把这一整行字符串放进字符串流 ss
        // 之后就可以像 cin 一样用 >> 一个一个读取数据
        stringstream ss(line);

        // 读取前 4 列
        // 第1列：学号
        // 第2列：姓名
        // 第3列：性别
        // 第4列：班级
        ss >> id >> name >> gender >> className;

        // 第5～15列不是本题需要输出的数据
        // 一共有 11 列，所以循环读取 11 次
        // 每次都读进 temp，然后直接丢弃
        for (int i = 0; i < 11; i++)
        {
            ss >> temp;
        }

        // 第16列是平均学分绩点
        ss >> gpa;

        // 创建 Student 对象 stu
        // 这里叫“对象实例化”
        // 创建对象时会自动调用 Student 的构造函数
        Student stu(id, name, className, gpa);

        // 调用 stu 对象的 show() 成员函数
        // 输出当前学生的信息
        stu.show();
    }

    // 文件使用结束，关闭文件
    fin.close();

    // return 0 表示程序正常结束
    return 0;
}
```

---

# 2. 程序整体在做什么？

这个程序可以概括成 4 步：

```text
打开成绩文件
    ↓
一行一行读取学生数据
    ↓
提取学号、姓名、班级、GPA
    ↓
创建 Student 对象并输出
```

更完整一点：

```text
grades.txt
    ↓
ifstream 打开文件
    ↓
getline() 读取一整行
    ↓
stringstream 拆分这一行
    ↓
读取 id / name / className / gpa
    ↓
Student stu(...)
    ↓
stu.show()
    ↓
打印学生信息
```

---

# 3. 头文件详细讲解

C++ 中：

```cpp
#include <...>
```

可以理解为：

> “我要使用某个已经写好的工具库。”

如果没有包含对应的头文件，很多功能就无法使用。

---

## 3.1 `<iostream>`

```cpp
#include <iostream>
```

`iostream` 是 Input Output Stream 的缩写。

主要负责：

- 标准输入
- 标准输出

常用内容：

```cpp
cin
cout
endl
```

例如：

```cpp
cout << "Hello" << endl;
```

输出：

```text
Hello
```

---

## 3.2 `<fstream>`

```cpp
#include <fstream>
```

`fstream` 负责文件输入输出。

常见类型：

```cpp
ifstream
ofstream
fstream
```

其中：

### `ifstream`

Input File Stream

用于：

> 从文件读取数据。

本程序中：

```cpp
ifstream fin("grades.txt");
```

意思是：

> 创建一个叫 `fin` 的文件输入流，并打开 `grades.txt`。

---

### `ofstream`

Output File Stream

用于：

> 向文件写数据。

例如：

```cpp
ofstream fout("result.txt");
fout << "Hello";
```

---

### `fstream`

既可以读，也可以写。

目前这份作业只需要掌握：

```cpp
ifstream
```

---

# 3.3 `<sstream>`

```cpp
#include <sstream>
```

`sstream` 表示 String Stream，也就是“字符串流”。

最重要的是：

```cpp
stringstream
```

本程序：

```cpp
stringstream ss(line);
```

假设：

```cpp
line = "252219605505 张三 男 软件工程2522 90 85 3.725";
```

原本这只是一个完整字符串。

创建：

```cpp
stringstream ss(line);
```

之后，就可以：

```cpp
ss >> id;
ss >> name;
ss >> gender;
```

相当于不断从这一行中取数据。

可以理解为：

```text
一整行字符串
    ↓
stringstream
    ↓
一个一个拆开
```

---

# 3.4 `<string>`

```cpp
#include <string>
```

用于字符串类型：

```cpp
string
```

例如：

```cpp
string name = "张三";
```

本程序中：

```cpp
string id;
string name;
string className;
```

都是字符串。

---

## 为什么学号使用 `string`？

虽然学号看起来是数字，例如：

```text
252219605505
```

但学号本质上是：

> 编号，而不是需要进行加减乘除的数字。

因此用：

```cpp
string id;
```

比：

```cpp
int id;
```

更合理。

而且有些编号可能以 `0` 开头，用整数会丢失开头的 `0`。

---

# 3.5 `<iomanip>`

```cpp
#include <iomanip>
```

用于格式化输入输出。

本程序中使用：

```cpp
left
setw()
fixed
setprecision()
```

---

## `left`

```cpp
cout << left;
```

表示：

> 后面的内容左对齐。

---

## `setw()`

例如：

```cpp
setw(18)
```

表示：

> 设置当前数据输出区域的宽度。

例如：

```cpp
cout << left << setw(10) << "ABC";
```

效果类似：

```text
ABC
```

后面自动留出空格。

它主要用于让表格对齐。

---

## `fixed`

```cpp
fixed
```

表示：

> 用普通小数形式输出浮点数。

---

## `setprecision(3)`

和 `fixed` 一起使用：

```cpp
fixed << setprecision(3)
```

表示：

> 小数点后保留 3 位。

例如：

```text
3.7
```

输出：

```text
3.700
```

例如：

```text
3.72568
```

输出：

```text
3.726
```

---

# 3.6 `<windows.h>`

```cpp
#include <windows.h>
```

这是 Windows 系统提供的头文件。

本程序中：

```cpp
SetConsoleOutputCP(CP_UTF8);
```

来自 Windows API。

作用：

> 把控制台输出代码页设置为 UTF-8。

主要是为了处理中文乱码。

注意：

这不是标准 C++ 的内容。

也就是说，在 Linux / macOS 中通常不需要这一句。

---

# 4. `using namespace std;`

```cpp
using namespace std;
```

C++ 标准库中的很多内容实际上属于：

```cpp
std
```

命名空间。

例如完整写法应该是：

```cpp
std::cout
std::string
std::ifstream
```

写了：

```cpp
using namespace std;
```

之后，可以直接写：

```cpp
cout
string
ifstream
```

---

# 5. 类 `class`

程序中：

```cpp
class Student
{
};
```

这里定义了一个新的类型：

```text
Student
```

以前 C++ 自带：

```cpp
int
double
char
string
```

现在我们自己定义：

```cpp
Student
```

可以理解为：

> Student 是“学生模板”。

---

## 类和对象的关系

例如：

```cpp
class Student
```

是：

> 模板。

而：

```cpp
Student stu(...);
```

是：

> 根据模板创建出来的一个具体对象。

可以理解为：

```text
Student 类
    ↓
学生模板
    ↓
创建对象
    ↓
stu
```

类似：

```text
图纸 → 房子
模具 → 产品
Student 类 → stu 对象
```

---

# 6. 抽象数据类型 ADT

老师题目里提到：

> 设计成绩单对应的抽象数据类型。

ADT 是：

> Abstract Data Type，抽象数据类型。

简单理解：

> 把一类数据和操作这些数据的方法组合起来。

本程序：

```cpp
class Student
```

里面包含：

### 数据

```cpp
id
name
className
gpa
```

### 操作

```cpp
Student(...)
show()
```

也就是：

```text
Student
│
├── 数据
│   ├── id
│   ├── name
│   ├── className
│   └── gpa
│
└── 操作
    ├── 构造函数
    └── show()
```

这就是一个最基础的抽象数据类型设计。

---

# 7. `private` 和 `public`

程序：

```cpp
private:
    string id;
    string name;
    string className;
    double gpa;

public:
    Student(...);
    void show();
```

---

## `private`

```cpp
private:
```

表示：

> 下面的内容属于类的私有成员。

外部不能直接访问。

例如：

```cpp
stu.gpa = 100;
```

通常是不允许的，因为：

```cpp
gpa
```

是 private。

---

## `public`

```cpp
public:
```

表示：

> 外部可以使用。

例如：

```cpp
stu.show();
```

可以执行，因为：

```cpp
show()
```

属于 public。

---

## 为什么这样设计？

这体现了面向对象的：

> 封装。

也就是：

```text
内部数据
    ↓
保护起来
    ↓
通过公开的方法操作
```

---

# 8. 成员变量

```cpp
string id;
string name;
string className;
double gpa;
```

这些变量写在类内部，因此叫：

> 成员变量。

一个 Student 对象会拥有自己的一套成员变量。

例如：

```text
stu1
id = 001
name = 张三
gpa = 3.5

stu2
id = 002
name = 李四
gpa = 3.8
```

两个对象之间的数据互不影响。

---

# 9. 构造函数

程序：

```cpp
Student(string i, string n, string c, double g)
{
    id = i;
    name = n;
    className = c;
    gpa = g;
}
```

构造函数是本题非常重要的知识点。

---

## 构造函数的特点

### 1. 名字和类名相同

类：

```cpp
Student
```

构造函数：

```cpp
Student(...)
```

---

### 2. 没有返回值

不能写：

```cpp
void Student(...)
```

也不能写：

```cpp
int Student(...)
```

正确：

```cpp
Student(...)
```

---

### 3. 创建对象时自动调用

当执行：

```cpp
Student stu(id, name, className, gpa);
```

就会自动调用：

```cpp
Student(string i, string n, string c, double g)
```

---

# 10. 构造函数参数到底怎么传？

假设当前：

```cpp
id = "252219605505";
name = "张三";
className = "软件工程2522";
gpa = 3.725;
```

执行：

```cpp
Student stu(id, name, className, gpa);
```

相当于：

```text
id        → i
name      → n
className → c
gpa       → g
```

然后：

```cpp
id = i;
name = n;
className = c;
gpa = g;
```

完成成员变量初始化。

---

# 11. 对象实例化

```cpp
Student stu(id, name, className, gpa);
```

这句话叫：

> 实例化对象。

拆开：

```cpp
Student
```

表示：

> 类型。

```cpp
stu
```

表示：

> 对象名称。

类似：

```cpp
int a;
```

其中：

```text
int     → 类型
a       → 变量
```

而：

```text
Student → 类型
stu     → 对象
```

---

# 12. 成员函数

```cpp
void show()
```

写在 `Student` 类内部，所以叫：

> 成员函数。

作用：

> 输出当前学生的信息。

调用方式：

```cpp
stu.show();
```

这里的：

```text
.
```

叫：

> 成员访问运算符。

意思是：

> 调用 stu 对象内部的 show()。

---

# 13. `void`

```cpp
void show()
```

`void` 表示：

> 这个函数不返回结果。

例如：

```cpp
void show()
{
    cout << "Hello";
}
```

函数只做事情，不返回值。

---

# 14. `main()` 函数

```cpp
int main()
```

C++ 程序从：

```cpp
main()
```

开始真正执行。

虽然 `Student` 类写在前面，但是那只是：

> 定义 Student 是什么。

真正执行顺序从：

```cpp
main()
```

开始。

---

# 15. 文件读取 `ifstream`

```cpp
ifstream fin("grades.txt");
```

这里：

```cpp
ifstream
```

是文件输入流类型。

```cpp
fin
```

是对象名。

```cpp
"grades.txt"
```

是文件名。

完整意思：

> 创建一个名叫 `fin` 的文件输入流，并打开 grades.txt。

---

# 16. 判断文件是否打开成功

```cpp
if (!fin)
{
    cout << "File open error!" << endl;
    return 1;
}
```

如果文件不存在，或者无法打开：

```cpp
!fin
```

就会成立。

程序输出：

```text
File open error!
```

然后：

```cpp
return 1;
```

退出。

---

## `return 0` 和 `return 1`

通常：

```cpp
return 0;
```

表示：

> 程序正常结束。

而：

```cpp
return 1;
```

通常表示：

> 程序遇到了错误。

---

# 17. `getline()`

程序：

```cpp
getline(fin, line);
```

作用：

> 从文件 `fin` 中读取一整行，保存到 `line`。

例如文件：

```text
学生成绩表
学号 姓名 性别 班级 ...
252219605505 张三 男 软件工程2522 ...
```

第一次：

```cpp
getline(fin, line);
```

读取：

```text
学生成绩表
```

第二次：

```cpp
getline(fin, line);
```

读取：

```text
学号 姓名 性别 班级 ...
```

因为这两行不使用，所以相当于：

> 跳过标题和表头。

---

# 18. `while (getline(fin, line))`

这是文件读取中非常重要的写法：

```cpp
while (getline(fin, line))
```

意思：

> 只要还能成功读取下一行，就继续循环。

例如：

```text
张三
李四
王五
```

程序：

```text
第一次循环 → 张三
第二次循环 → 李四
第三次循环 → 王五
第四次尝试 → 文件结束 → 循环停止
```

---

# 19. `stringstream`

程序：

```cpp
stringstream ss(line);
```

假设：

```cpp
line =
"252219605505 张三 男 软件工程2522 90 85 88 3.725"
```

这时候 `line` 是：

> 一整个字符串。

创建：

```cpp
stringstream ss(line);
```

之后可以：

```cpp
ss >> id;
ss >> name;
ss >> gender;
```

逐个读取。

---

## `stringstream` 和 `cin` 的区别

### `cin`

从键盘读取：

```cpp
cin >> name;
```

---

### `ss`

从字符串中读取：

```cpp
ss >> name;
```

所以：

```text
cin → 从键盘读
ss  → 从字符串读
```

---

# 20. 连续读取多个变量

```cpp
ss >> id >> name >> gender >> className;
```

等价于：

```cpp
ss >> id;
ss >> name;
ss >> gender;
ss >> className;
```

假设数据：

```text
252219605505 张三 男 软件工程2522
```

执行后：

```text
id        = 252219605505
name      = 张三
gender    = 男
className = 软件工程2522
```

---

# 21. `for` 循环跳过不需要的数据

```cpp
for (int i = 0; i < 11; i++)
{
    ss >> temp;
}
```

前面已经读取：

```text
第1列 学号
第2列 姓名
第3列 性别
第4列 班级
```

GPA 在：

```text
第16列
```

所以中间：

```text
第5～15列
```

一共有 11 列。

计算：

```text
15 - 5 + 1 = 11
```

所以循环 11 次。

---

# 22. `temp` 为什么可以一直覆盖？

```cpp
ss >> temp;
```

每次都会覆盖上一次的数据。

例如：

```text
第1次 temp = 90
第2次 temp = 85
第3次 temp = 78
...
```

完全没问题，因为这些数据：

> 本来就不需要保存。

`temp` 只是一个临时“垃圾桶”。

---

# 23. 读取 GPA

跳过 11 列之后：

```cpp
ss >> gpa;
```

正好读取：

```text
第16列 GPA
```

例如：

```text
3.725
```

于是：

```cpp
gpa = 3.725;
```

---

# 24. 创建 Student 对象

数据全部读取完成后：

```cpp
Student stu(id, name, className, gpa);
```

假设：

```text
id = 252219605505
name = 张三
className = 软件工程2522
gpa = 3.725
```

那么创建：

```text
stu
│
├── id = 252219605505
├── name = 张三
├── className = 软件工程2522
└── gpa = 3.725
```

---

# 25. `stu.show()`

```cpp
stu.show();
```

意思：

> 调用 stu 对象自己的 show() 函数。

然后输出：

```text
252219605505      张三        软件工程2522      3.725
```

---

# 26. while 循环中对象的变化

每次循环都会重新创建一个：

```cpp
Student stu(...)
```

例如：

```text
第一次循环：
读取张三
↓
创建 stu
↓
输出张三

第二次循环：
读取李四
↓
创建新的 stu
↓
输出李四

第三次循环：
读取王五
↓
创建新的 stu
↓
输出王五
```

所以有多少学生，就会执行多少次。

---

# 27. `fin.close()`

```cpp
fin.close();
```

表示：

> 关闭 grades.txt 文件。

虽然现代 C++ 在对象销毁时通常会自动关闭文件，但初学阶段主动写：

```cpp
fin.close();
```

逻辑更加清晰。

---

# 28. 程序完整执行流程

```text
main()
  ↓
设置 UTF-8
  ↓
打开 grades.txt
  ↓
判断是否成功
  ↓
跳过标题和表头
  ↓
输出表格标题
  ↓
读取一整行学生数据
  ↓
stringstream 拆分
  ↓
读取学号、姓名、性别、班级
  ↓
跳过第 5～15 列
  ↓
读取 GPA
  ↓
创建 Student 对象
  ↓
调用 stu.show()
  ↓
还有下一行？
  ├── 有 → 继续循环
  └── 没有 → 关闭文件
              ↓
             结束
```

---

# 29. 本题最核心知识点

## ① 类

```cpp
class Student
```

定义一种新的数据类型。

---

## ② 对象

```cpp
Student stu(...);
```

根据类创建一个具体对象。

---

## ③ private

保存类内部数据。

---

## ④ public

提供给外部使用的功能。

---

## ⑤ 构造函数

```cpp
Student(...)
```

创建对象时自动执行。

---

## ⑥ 成员函数

```cpp
void show()
```

对象可以执行的功能。

---

## ⑦ 文件输入流

```cpp
ifstream fin
```

从文件读取数据。

---

## ⑧ getline

```cpp
getline(fin, line)
```

从文件读取一整行。

---

## ⑨ stringstream

```cpp
stringstream ss(line)
```

把一整行数据拆成一个一个字段。

---

## ⑩ 格式化输出

```cpp
setw()
fixed
setprecision()
```

让输出表格更加整齐。

---

# 30. 最重要的三组关系

## 第一组：类和对象

```text
Student
   ↓
学生模板
   ↓
Student stu(...)
   ↓
具体学生对象
```

---

## 第二组：private 和 public

```text
Student
│
├── private
│   ├── id
│   ├── name
│   ├── className
│   └── gpa
│
└── public
    ├── Student(...)
    └── show()
```

---

## 第三组：文件读取流程

```text
grades.txt
    ↓
getline()
    ↓
line
    ↓
stringstream
    ↓
id / name / className / gpa
    ↓
Student stu(...)
    ↓
stu.show()
```

---

# 31. 容易出错的地方

## 1. `className` 不能直接写成 `class`

错误：

```cpp
string class;
```

因为：

```cpp
class
```

是 C++ 关键字。

所以使用：

```cpp
string className;
```

---

## 2. 构造函数不能写返回类型

错误：

```cpp
void Student(...)
```

正确：

```cpp
Student(...)
```

---

## 3. `grades.txt` 文件位置

运行：

```cpp
ifstream fin("grades.txt");
```

时，程序会在当前工作目录寻找：

```text
grades.txt
```

如果找不到：

```text
File open error!
```

---

## 4. UTF-8 不等于一定不会乱码

即使文件是 UTF-8：

```cpp
SetConsoleOutputCP(CP_UTF8);
```

也只是设置输出编码。

Windows 终端、编译器源文件编码、字体等都可能影响中文显示。

---

## 5. `setw()` 对中文可能不完全对齐

中文字符在终端中通常显示为双宽字符。

因此：

```cpp
setw()
```

在中英文混合时不一定完全整齐。

这是终端显示宽度的问题，不是你的程序逻辑错误。

---

# 32. 老师如果问：为什么要用 Student 类？

可以回答：

> 因为一个学生包含学号、姓名、班级、GPA 等多个相关数据，使用 Student 类可以把这些数据和输出操作封装在一起，形成一个抽象数据类型，使程序结构更加清晰。

---

# 33. 老师如果问：构造函数有什么用？

可以回答：

> 构造函数用于在创建 Student 对象时初始化学生的学号、姓名、班级和 GPA，而且对象创建时会自动调用。

---

# 34. 老师如果问：为什么用 stringstream？

可以回答：

> getline() 每次读取的是一整行字符串，而 stringstream 可以把这一整行按照空白字符拆开，从而逐个读取学号、姓名、班级等字段。

---

# 35. 老师如果问：为什么使用 while(getline(...))？

可以回答：

> 因为只有当 getline 成功读取到下一行时循环才继续，当文件读取到末尾时 getline 失败，循环自动结束，所以这种写法适合逐行读取文件。

---

# 36. 老师如果问：这道题哪里体现了对象实例化？

代码：

```cpp
Student stu(id, name, className, gpa);
```

这里就是：

> 根据 Student 类创建 stu 对象，也叫对象实例化。

---

# 37. 老师如果问：哪里体现了抽象数据类型？

代码：

```cpp
class Student
```

Student 中同时包含：

```text
数据：
id
name
className
gpa

操作：
Student(...)
show()
```

因此可以看作一个学生抽象数据类型。

---

# 38. 一句话背程序

如果考试前需要快速回忆，可以记：

> 用 `ifstream` 打开成绩文件，用 `getline` 一行一行读取，再用 `stringstream` 拆分字段，取出学号、姓名、班级和 GPA，通过构造函数创建 `Student` 对象，最后调用 `show()` 输出。

---

# 39. 推荐在 Obsidian 中建立的目录

可以放到：

```text
计算机学习
└── C++
    ├── C++基础
    ├── 面向对象
    ├── 文件操作
    └── 作业
        └── 数据结构作业一-成绩单程序.md
```

如果你目前不想分太细，也可以直接：

```text
大学课程
└── 数据结构
    └── 作业
        └── 作业一-成绩单程序详细讲解.md
```

---

# 40. 复习标签

```text
#C++
#数据结构
#面向对象
#class
#构造函数
#文件操作
#stringstream
#课程作业
```

---

# 41. 最后再看一次核心代码

```cpp
// 定义学生类型
class Student
{
private:
    string id;
    string name;
    string className;
    double gpa;

public:
    // 构造函数
    Student(string i, string n, string c, double g)
    {
        id = i;
        name = n;
        className = c;
        gpa = g;
    }

    // 输出函数
    void show()
    {
        cout << left << setw(18) << id
             << setw(12) << name
             << setw(18) << className
             << fixed << setprecision(3) << gpa
             << endl;
    }
};
```

最应该真正理解的是：

```text
class Student
    ↓
定义学生模板

Student(...)
    ↓
构造函数

Student stu(...)
    ↓
实例化对象

stu.show()
    ↓
对象调用成员函数
```

---

> 学习建议：不要背整段代码。  
> 先把 `class → 构造函数 → 对象 → 成员函数` 这一条链弄懂，再理解 `ifstream → getline → stringstream` 这一条文件读取链，这道题就真正掌握了。
