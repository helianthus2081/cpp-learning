/*
 * 数据结构实验三：二叉树的构造与基本操作
 * 存储结构：二叉链表
 * 编译示例：gcc binary_tree_exp3.c -o binary_tree_exp3
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 100

typedef struct BiTNode {
    char data;
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

/* ---------- 1. 由先序 + 中序序列构造二叉树 ---------- */

/* 校验输入：长度一致、元素不重复、两个序列元素集合相同 */
static int ValidateSequences(const char pre[], const char ino[], int n) {
    int i, j, found;

    if (n <= 0 || n >= MAXN) return 0;

    for (i = 0; i < n; ++i) {
        for (j = 0; j < i; ++j) {
            if (ino[i] == ino[j]) return 0;   /* 中序元素必须唯一，否则树不唯一 */
            if (pre[i] == pre[j]) return 0;   /* 先序元素也必须唯一 */
        }
    }

    for (i = 0; i < n; ++i) {
        found = 0;
        for (j = 0; j < n; ++j) {
            if (pre[i] == ino[j]) {
                found = 1;
                break;
            }
        }
        if (!found) return 0;
    }
    return 1;
}

/* 递归构造：preIdx 记录当前先序序列处理到的位置 */
static BiTNode *BuildByPreIn(const char pre[], int *preIdx,
                             const char ino[], int inL, int inR) {
    if (inL > inR) return NULL;

    char rootVal = pre[*preIdx];
    int k = inL;
    while (k <= inR && ino[k] != rootVal) ++k;
    if (k > inR) return NULL;   /* 理论上不会走到，输入已校验 */

    BiTNode *node = (BiTNode *)malloc(sizeof(BiTNode));
    if (node == NULL) {
        fprintf(stderr, "内存分配失败！\n");
        exit(EXIT_FAILURE);
    }
    node->data = rootVal;
    node->lchild = node->rchild = NULL;
    ++(*preIdx);

    node->lchild = BuildByPreIn(pre, preIdx, ino, inL, k - 1);
    node->rchild = BuildByPreIn(pre, preIdx, ino, k + 1, inR);
    return node;
}

BiTree CreateBiTree(const char pre[], const char ino[], int n) {
    int idx = 0;
    return BuildByPreIn(pre, &idx, ino, 0, n - 1);
}

/* 释放二叉树（后序释放） */
void DestroyBiTree(BiTree T) {
    if (T == NULL) return;
    DestroyBiTree(T->lchild);
    DestroyBiTree(T->rchild);
    free(T);
}

/* ---------- 2~4. 三种递归遍历 ---------- */

static void PrintFirst(char c, int *first) {
    if (!(*first)) printf(" ");
    printf("%c", c);
    *first = 0;
}

static void PreRec(BiTree T, int *first) {
    if (T == NULL) return;
    PrintFirst(T->data, first);
    PreRec(T->lchild, first);
    PreRec(T->rchild, first);
}

static void InRec(BiTree T, int *first) {
    if (T == NULL) return;
    InRec(T->lchild, first);
    PrintFirst(T->data, first);
    InRec(T->rchild, first);
}

static void PostRec(BiTree T, int *first) {
    if (T == NULL) return;
    PostRec(T->lchild, first);
    PostRec(T->rchild, first);
    PrintFirst(T->data, first);
}

void PreOrder(BiTree T)  { int first = 1; PreRec(T, &first);  printf("\n"); }
void InOrder(BiTree T)   { int first = 1; InRec(T, &first);   printf("\n"); }
void PostOrder(BiTree T) { int first = 1; PostRec(T, &first); printf("\n"); }

/* ---------- 5. 求叶子结点数目 ---------- */

int CountLeaves(BiTree T) {
    if (T == NULL) return 0;
    if (T->lchild == NULL && T->rchild == NULL) return 1;
    return CountLeaves(T->lchild) + CountLeaves(T->rchild);
}

/* ---------- 6. 交换所有结点的左右子树 ---------- */

void SwapLeftRight(BiTree T) {
    if (T == NULL) return;
    BiTNode *tmp = T->lchild;
    T->lchild = T->rchild;
    T->rchild = tmp;
    SwapLeftRight(T->lchild);
    SwapLeftRight(T->rchild);
}

/* ---------- 7. 求指定结点所在层数（根为第 1 层，不存在返回 0） ---------- */

static int LevelRec(BiTree T, char x, int level) {
    if (T == NULL) return 0;
    if (T->data == x) return level;

    int leftLevel = LevelRec(T->lchild, x, level + 1);
    if (leftLevel != 0) return leftLevel;
    return LevelRec(T->rchild, x, level + 1);
}

int NodeLevel(BiTree T, char x) {
    return LevelRec(T, x, 1);
}

/* ---------- 主函数：菜单测试 ---------- */

static void PrintMenu(void) {
    printf("\n========== 二叉树基本操作 ==========\n");
    printf("1. 先序递归遍历\n");
    printf("2. 中序递归遍历\n");
    printf("3. 后序递归遍历\n");
    printf("4. 求叶子结点数目\n");
    printf("5. 交换所有结点的左右子树\n");
    printf("6. 求指定结点所在层数\n");
    printf("0. 退出\n");
    printf("请选择：");
}

int main(void) {
    char pre[MAXN], ino[MAXN];

    printf("请输入先序序列（如 ABDECF）：");
    if (scanf("%99s", pre) != 1) return 1;
    printf("请输入中序序列（如 DBEACF）：");
    if (scanf("%99s", ino) != 1) return 1;

    int n = (int)strlen(pre);
    if ((int)strlen(ino) != n || !ValidateSequences(pre, ino, n)) {
        printf("输入无效：两序列长度需相同、元素需唯一且集合一致。\n");
        return 1;
    }

    BiTree root = CreateBiTree(pre, ino, n);
    printf("二叉树构造完成，共 %d 个结点。\n", n);

    int choice;
    char x;
    while (1) {
        PrintMenu();
        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
        case 1:
            printf("先序遍历：");
            PreOrder(root);
            break;
        case 2:
            printf("中序遍历：");
            InOrder(root);
            break;
        case 3:
            printf("后序遍历：");
            PostOrder(root);
            break;
        case 4:
            printf("叶子结点数：%d\n", CountLeaves(root));
            break;
        case 5:
            SwapLeftRight(root);
            printf("左右子树已全部交换。交换后的中序遍历：");
            InOrder(root);
            break;
        case 6:
            printf("请输入要查询的结点字符：");
            if (scanf(" %c", &x) == 1) {
                int lv = NodeLevel(root, x);
                if (lv == 0) printf("结点 %c 不存在于树中。\n", x);
                else printf("结点 %c 位于第 %d 层。\n", x, lv);
            }
            break;
        case 0:
            DestroyBiTree(root);
            printf("程序结束。\n");
            return 0;
        default:
            printf("无效选择，请重新输入。\n");
            break;
        }
    }

    DestroyBiTree(root);
    return 0;
}
