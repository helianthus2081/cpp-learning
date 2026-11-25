#include<iostream>
using namespace std;

#include <iostream>
using namespace std;

//定义单链表结点
struct link_node
{
    int data;
    link_node *next;
};

//定义链队列
struct link_queue
{
    link_node *front;  //队头指针
    link_node *rear;   //队尾指针

    //初始化空队列
    link_queue()
    {
        front = nullptr;
        rear = nullptr;
    }

    //非空队列的析构
    ~link_queue()
    {
        link_node *p;

        while (front != nullptr)
        {
            p = front;
            front = front->next;
            delete p;
        }

        rear = nullptr;
    }
};

//入队列
void enqueue(link_queue &q, int x)
{
    link_node *p = new link_node;
    p->data = x;
    p->next = nullptr;

    //如果原队列为空
    if (q.front == nullptr)
    {
        q.front = p;
        q.rear = p;
    }
    else
    {
        q.rear->next = p;
        q.rear = p;
    }
}

//出队列
bool dequeue(link_queue &q, int &x)
{
    if (q.front == nullptr)
    {
        return false;
    }

    link_node *p = q.front;
    x = p->data;
    q.front = q.front->next;

    //如果出队后队列为空
    if (q.front == nullptr)
    {
        q.rear = nullptr;
    }

    delete p;
    return true;
}

//判断队列是否为空
bool empty(link_queue &q)
{
    return q.front == nullptr;
}

int main()
{
    link_queue q;
    int x;

    //测试入队列
    enqueue(q, 2);
    enqueue(q, 20);
    enqueue(q, 200);

    cout << "依次入队：2、20、200" << endl;

    //测试队列是否为空
    cout << "队列是否为空(0为非空，1为空)："
         << empty(q) << endl;

    //测试出队列
    if (dequeue(q, x))
    {
        cout << "出队元素为：" << x << endl;
    }

    cout << "出队后队列是否为空："
         << empty(q) << endl;

    return 0;
}
