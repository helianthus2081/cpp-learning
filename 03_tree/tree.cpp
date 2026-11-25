/*
 * 数据结构实验三：二叉树的构造与基本操作（C++ 版）
 * 存储结构：二叉链表
 * 编译示例：g++ binary_tree_exp3.cpp -o binary_tree_exp3
 */

#include <iostream>
#include <string>
#include <new>
#include <cstdlib>
using namespace std;

struct BiTNode {
    char data;
    BiTNode *lchild, *rchild;
    explicit BiTNode(char c) : data(c), lchild(nullptr), rchild(nullptr) {}
};

typedef BiTNode *BiTree;

/* ---------- 1. 由先序 + 中序序列构造二叉树 ---------- */

/* 校验输入：长度一致、元素不重复、两个序列元素集合相同 */
static bool ValidateSequences(const string &pre, const string &ino) {
    if (pre.empty() || pre.size() != ino.size()) return false;

    for (size_t i = 0; i < pre.size(); ++i) {
        for (size_t j = 0; j < i; ++j) {
            if (pre[i] == pre[j]) return false;
            if (ino[i] == ino[j]) return false;
        }
    }

    for (size_t i = 0; i < pre.size(); ++i) {
        if (ino.find(pre[i]) == string::npos) return false;
    }
    return true;
}

/* 递归构造：preIdx 记录当前先序序列处理到的位置 */
static BiTNode *BuildByPreIn(const string &pre, int &preIdx,
                             const string &ino, int inL, int inR) {
    if (inL > inR) return nullptr;

    char rootVal = pre[static_cast<size_t>(preIdx)];
    int k = inL;
    while (k <= inR && ino[static_cast<size_t>(k)] != rootVal) ++k;
    if (k > inR) return nullptr;   /* 输入已校验，理论上不会走到 */

    BiTNode *node = new (nothrow) BiTNode(rootVal);
    if (node == nullptr) {
        cerr << "内存分配失败！" << endl;
        exit(EXIT_FAILURE);
    }
    ++preIdx;

    node->lchild = BuildByPreIn(pre, preIdx, ino, inL, k - 1);
    node->rchild = BuildByPreIn(pre, preIdx, ino, k + 1, inR);
    return node;
}

BiTree CreateBiTree(const string &pre, const string &ino) {
    int idx = 0;
    return BuildByPreIn(pre, idx, ino, 0, static_cast<int>(ino.size()) - 1);
}

/* 释放二叉树（后序释放） */
void DestroyBiTree(BiTree T) {
    if (T == nullptr) return;
    DestroyBiTree(T->lchild);
    DestroyBiTree(T->rchild);
    delete T;
}

/* ---------- 2~4. 三种递归遍历 ---------- */

static void PrintFirst(char c, bool &first) {
    if (!first) cout << " ";
    cout << c;
    first = false;
}

static void PreRec(BiTree T, bool &first) {
    if (T == nullptr) return;
    PrintFirst(T->data, first);
    PreRec(T->lchild, first);
    PreRec(T->rchild, first);
}

static void InRec(BiTree T, bool &first) {
    if (T == nullptr) return;
    InRec(T->lchild, first);
    PrintFirst(T->data, first);
    InRec(T->rchild, first);
}

static void PostRec(BiTree T, bool &first) {
    if (T == nullptr) return;
    PostRec(T->lchild, first);
    PostRec(T->rchild, first);
    PrintFirst(T->data, first);
}

void PreOrder(BiTree T)  { bool first = true; PreRec(T, first);  cout << endl; }
void InOrder(BiTree T)   { bool first = true; InRec(T, first);   cout << endl; }
void PostOrder(BiTree T) { bool first = true; PostRec(T, first); cout << endl; }

/* ---------- 5. 求叶子结点数目 ---------- */

int CountLeaves(BiTree T) {
    if (T == nullptr) return 0;
    if (T->lchild == nullptr && T->rchild == nullptr) return 1;
    return CountLeaves(T->lchild) + CountLeaves(T->rchild);
}

/* ---------- 6. 交换所有结点的左右子树 ---------- */

void SwapLeftRight(BiTree T) {
    if (T == nullptr) return;
    BiTNode *tmp = T->lchild;
    T->lchild = T->rchild;
    T->rchild = tmp;
    SwapLeftRight(T->lchild);
    SwapLeftRight(T->rchild);
}

/* ---------- 7. 求指定结点所在层数（根为第 1 层，不存在返回 0） ---------- */

static int LevelRec(BiTree T, char x, int level) {
    if (T == nullptr) return 0;
    if (T->data == x) return level;

    int leftLevel = LevelRec(T->lchild, x, level + 1);
    if (leftLevel != 0) return leftLevel;
    return LevelRec(T->rchild, x, level + 1);
}

int NodeLevel(BiTree T, char x) {
    return LevelRec(T, x, 1);
}

/* ---------- 主函数：菜单测试 ---------- */

static void PrintMenu() {
    cout << "\n========== 二叉树基本操作 ==========\n"
         << "1. 先序递归遍历\n"
         << "2. 中序递归遍历\n"
         << "3. 后序递归遍历\n"
         << "4. 求叶子结点数目\n"
         << "5. 交换所有结点的左右子树\n"
         << "6. 求指定结点所在层数\n"
         << "0. 退出\n"
         << "请选择：";
}

int main() {
    string pre, ino;

    cout << "请输入先序序列（如 ABDECF）：";
    if (!(cin >> pre)) return 1;
    cout << "请输入中序序列（如 DBEACF）：";
    if (!(cin >> ino)) return 1;

    if (!ValidateSequences(pre, ino)) {
        cout << "输入无效：两序列长度需相同、元素需唯一且集合一致。" << endl;
        return 1;
    }

    BiTree root = CreateBiTree(pre, ino);
    cout << "二叉树构造完成，共 " << pre.size() << " 个结点。" << endl;

    int choice;
    char x;
    while (true) {
        PrintMenu();
        if (!(cin >> choice)) break;

        switch (choice) {
        case 1:
            cout << "先序遍历：";
            PreOrder(root);
            break;
        case 2:
            cout << "中序遍历：";
            InOrder(root);
            break;
        case 3:
            cout << "后序遍历：";
            PostOrder(root);
            break;
        case 4:
            cout << "叶子结点数：" << CountLeaves(root) << endl;
            break;
        case 5:
            SwapLeftRight(root);
            cout << "左右子树已全部交换。交换后的中序遍历：";
            InOrder(root);
            break;
        case 6:
            cout << "请输入要查询的结点字符：";
            if (cin >> x) {
                int lv = NodeLevel(root, x);
                if (lv == 0) cout << "结点 " << x << " 不存在于树中。" << endl;
                else cout << "结点 " << x << " 位于第 " << lv << " 层。" << endl;
            }
            break;
        case 0:
            DestroyBiTree(root);
            cout << "程序结束。" << endl;
            return 0;
        default:
            cout << "无效选择，请重新输入。" << endl;
            break;
        }
    }

    DestroyBiTree(root);
    return 0;
}
