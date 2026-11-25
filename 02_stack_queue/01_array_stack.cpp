#include<iostream>
using namespace std;

const int MAXSIZE = 100;

//定义顺序栈
struct SqStack {
    int data[MAXSIZE]; //存储栈元素
    int top;           //栈顶指针
    //初始化栈
    SqStack(){
    top = -1;//栈顶指针初始化为-1，表示栈为空
 }
};
//入栈
bool push(SqStack&s,int x){
    if(s.top == MAXSIZE -1){
        return false;
    }else{
        s.data[++s.top] = x;
        return true;    
    }
}
//出栈
bool pop(SqStack&s,int&x){
    if(s.top == -1){
        //cout<<"栈空"<<endl;
        return false;
    }
    x = s.data[s.top--];
    s.top--;
    return true;
}
//判断栈满
bool full(SqStack&s){
    return s.top == MAXSIZE - 1;
}
//判断栈空
bool empty(SqStack&s){
    return s.top == -1;
}

//置空栈
void clear(SqStack&s){
    s.top = -1;
}
//主函数
int main(){
    SqStack s;
    int x = -1;    

    
    //测试入栈
    push(s, 10);
    push(s, 20);
    push(s, 30);
cout<<"依次入栈：10，20，30"<<endl;
//测试 栈空和栈满
cout<<"栈是否为空(0为非空，1为空)："<<empty(s)<<endl;
cout<<"栈是否为满(0为非满，1为满)："<<full(s)<<endl;
//测试出栈
if(pop(s,x)){
    cout<<"出栈元素为："<<x<<endl;
}
clear(s);
cout << "置空后栈是否为空(0为非空，1为空)：" << empty(s) << endl;

}

