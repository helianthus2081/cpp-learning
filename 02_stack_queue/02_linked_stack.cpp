#include<iostream>
using namespace std;

//定义单链表结点
struct link_node{
    int data;
    link_node*next;

};
//定义链栈
struct link_stack{
    link_node*top; //栈顶指针
    //初始化栈
    link_stack(){
        top = nullptr; //栈顶指针初始化为空，表示栈为空
    }
//非空的析构
~link_stack(){
    link_node*p;
    while(top !=nullptr){
        p=top;
        top=top->next;
        delete p;
    }
}
};
//入栈
void push(link_stack&s,int x){
    link_node*p=new link_node;
    p->data=x;
    p->next=s.top;
    s.top=p;
}
//出栈
bool pop(link_stack&s,int&x){
    if(s.top==nullptr){
        return false;
    }
    link_node*p=s.top;
    x=p->data;
    s.top=p->next;
    delete p;
    return true;
}
//判断栈空
bool empty(link_stack&s){
    return s.top==nullptr;
}
int main(){
    link_stack s;
    int x;
    push(s, 5);
    push(s, 10);
    push(s, 15);
    cout<<"依次入栈：5，10，15"<<endl;
    //测试栈空
    cout<<"栈是否为空(0为非空，1为空)："<<empty(s)<<endl;
    //测试出栈
    if(pop(s,x)){
        cout<<"出栈元素为："<<x<<endl;

    }
    cout<<"栈是否为空：(0为非空，1为空)："<<empty(s)<<endl;

    return 0;
}

