
#include <iostream>
using namespace std;

const int MAXSIZE = 100;

// 定义数组队列
struct SqQueue
{
    int data[MAXSIZE];
    int front;  // 队头下标
    int rear;   // 队尾下标

    // 初始化队列
    SqQueue()
    {
        front = 0;
        rear = 0;
    }
};

// a. 入队
bool enqueue(SqQueue &q, int x)
{
    if (q.rear == MAXSIZE)
    {
        return false;
    }

    q.data[q.rear] = x;
    q.rear++;
    return true;
}

// b. 出队
bool dequeue(SqQueue &q, int &x)
{
    if (q.front == q.rear)
    {
        return false;
    }

    x = q.data[q.front];
    q.front++;
    return true;
}

// c. 判断队满
bool full(SqQueue &q)
{
    return q.rear == MAXSIZE;
}

// d. 判断队空
bool empty(SqQueue &q)
{
    return q.front == q.rear;
}

// e. 置空队列
void clear(SqQueue &q)
{
    q.front = 0;
    q.rear = 0;
}

int main()
{
    SqQueue q;
    int x;

    // 测试入队
    enqueue(q, 1);
    enqueue(q, 10);
    enqueue(q, 100);

    cout << "依次入队：1、10、100" << endl;
    
    // 测试队空和队满
    cout << "队列是否为空(0为非空，1为空)："
         << empty(q) << endl;

    cout << "队列是否为满(0为未满，1为满)："
         << full(q) << endl;

    // 测试出队
    if (dequeue(q, x))
    {
        cout << "出队元素为：" << x << endl;
    }

    // 测试置空
    clear(q);

    cout << "置空后队列是否为空："
         << empty(q) << endl;

    return 0;
}
