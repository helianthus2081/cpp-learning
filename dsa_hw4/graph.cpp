// 数据结构实验四：图的遍历与最小生成树

#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

constexpr int INF = 1'000'000'000;

struct Edge {
    int u, v;   // 0-based
    int w;      // 权值；无权图的边权记为 1
};

struct GraphData {
    int n = 0;
    bool directed = false;
    bool weighted = false;
    vector<Edge> edges;
};

// ---------------------------------------------------------------------------
// 一、邻接矩阵存储
// ---------------------------------------------------------------------------
class AdjMatrixGraph {
public:
    explicit AdjMatrixGraph(const GraphData& g) : n_(g.n), directed_(g.directed) {
        mat_.assign(n_, vector<int>(n_, INF));
        for (int i = 0; i < n_; ++i) mat_[i][i] = 0;
        for (const auto& e : g.edges) {
            mat_[e.u][e.v] = e.w;
            if (!directed_) mat_[e.v][e.u] = e.w;
        }
    }

    // 1.1 深度优先遍历（从 start 出发，start 为 0-based）
    void dfs(int start) const {
        vector<bool> visited(n_, false);
        cout << "DFS : ";
        dfsRecursive(start, visited);
        cout << '\n';
    }

    // 对非连通图：按顶点编号从小到大访问各个连通分量
    void dfsAll() const {
        vector<bool> visited(n_, false);
        cout << "DFS(全图) : ";
        for (int i = 0; i < n_; ++i)
            if (!visited[i]) dfsRecursive(i, visited);
        cout << '\n';
    }

    // 1.2 广度优先遍历
    void bfs(int start) const {
        vector<bool> visited(n_, false);
        queue<int> q;
        cout << "BFS : ";
        visited[start] = true;
        q.push(start);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            cout << u + 1 << ' ';
            for (int v = 0; v < n_; ++v) {
                if (mat_[u][v] != INF && !visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        cout << '\n';
    }

    void bfsAll() const {
        vector<bool> visited(n_, false);
        cout << "BFS(全图) : ";
        for (int s = 0; s < n_; ++s) {
            if (visited[s]) continue;
            queue<int> q;
            visited[s] = true;
            q.push(s);
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                cout << u + 1 << ' ';
                for (int v = 0; v < n_; ++v) {
                    if (mat_[u][v] != INF && !visited[v]) {
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }
        }
        cout << '\n';
    }

    // 供 Prim 算法读取边权：无权边返回 INF
    int weight(int u, int v) const { return mat_[u][v]; }
    int vertexCount() const { return n_; }

    void printMatrix(const GraphData& src) const {
        cout << "\n邻接矩阵（∞ 表示无边）：\n    ";
        for (int j = 1; j <= n_; ++j) cout << j << '\t';
        cout << '\n';
        for (int i = 0; i < n_; ++i) {
            cout << i + 1 << "\t";
            for (int j = 0; j < n_; ++j) {
                if (mat_[i][j] == INF) cout << "∞\t";
                else cout << mat_[i][j] << '\t';
            }
            cout << '\n';
        }
        if (!src.weighted) cout << "（无权图，矩阵中的 1 仅表示存在边）\n";
    }

private:
    void dfsRecursive(int u, vector<bool>& visited) const {
        visited[u] = true;
        cout << u + 1 << ' ';
        for (int v = 0; v < n_; ++v) {
            if (mat_[u][v] != INF && !visited[v]) dfsRecursive(v, visited);
        }
    }

    int n_;
    bool directed_;
    vector<vector<int>> mat_;
};

// ---------------------------------------------------------------------------
// 二、邻接表存储
// ---------------------------------------------------------------------------
class AdjListGraph {
public:
    explicit AdjListGraph(const GraphData& g) : n_(g.n), directed_(g.directed) {
        adj_.assign(n_, {});
        for (const auto& e : g.edges) {
            adj_[e.u].push_back({e.v, e.w});
            if (!directed_) adj_[e.v].push_back({e.u, e.w});
        }
        // 按邻接点编号排序，使两种存储结构的遍历序列可直接对比
        for (auto& list : adj_) {
            sort(list.begin(), list.end(),
                 [](const pair<int, int>& a, const pair<int, int>& b) { return a.first < b.first; });
        }
    }

    // 2.1 深度优先遍历
    void dfs(int start) const {
        vector<bool> visited(n_, false);
        cout << "DFS : ";
        dfsRecursive(start, visited);
        cout << '\n';
    }

    void dfsAll() const {
        vector<bool> visited(n_, false);
        cout << "DFS(全图) : ";
        for (int i = 0; i < n_; ++i)
            if (!visited[i]) dfsRecursive(i, visited);
        cout << '\n';
    }

    // 2.2 广度优先遍历
    void bfs(int start) const {
        vector<bool> visited(n_, false);
        queue<int> q;
        cout << "BFS : ";
        visited[start] = true;
        q.push(start);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            cout << u + 1 << ' ';
            for (const auto& [v, w] : adj_[u]) {
                (void)w;
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        cout << '\n';
    }

    void bfsAll() const {
        vector<bool> visited(n_, false);
        cout << "BFS(全图) : ";
        for (int s = 0; s < n_; ++s) {
            if (visited[s]) continue;
            queue<int> q;
            visited[s] = true;
            q.push(s);
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                cout << u + 1 << ' ';
                for (const auto& [v, w] : adj_[u]) {
                    (void)w;
                    if (!visited[v]) {
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }
        }
        cout << '\n';
    }

    void printList() const {
        cout << "\n邻接表：\n";
        for (int u = 0; u < n_; ++u) {
            cout << u + 1 << " -> ";
            if (adj_[u].empty()) cout << "(空)";
            for (size_t i = 0; i < adj_[u].size(); ++i) {
                if (i) cout << " -> ";
                cout << adj_[u][i].first + 1 << '(' << adj_[u][i].second << ')';
            }
            cout << '\n';
        }
    }

private:
    void dfsRecursive(int u, vector<bool>& visited) const {
        visited[u] = true;
        cout << u + 1 << ' ';
        for (const auto& [v, w] : adj_[u]) {
            (void)w;
            if (!visited[v]) dfsRecursive(v, visited);
        }
    }

    int n_;
    bool directed_;
    vector<vector<pair<int, int>>> adj_;
};

// ---------------------------------------------------------------------------
// 三、最小生成树
// ---------------------------------------------------------------------------

// Prim 算法：基于邻接矩阵，从 1 号顶点出发；非连通时返回最小生成森林
vector<Edge> prim(const AdjMatrixGraph& g, int& components) {
    const int n = g.vertexCount();
    vector<int> minDist(n, INF), parent(n, -1);
    vector<bool> inTree(n, false);
    vector<Edge> result;
    components = 0;
    minDist[0] = 0;

    for (int cnt = 0; cnt < n; ++cnt) {
        int u = -1;
        for (int i = 0; i < n; ++i)
            if (!inTree[i] && (u == -1 || minDist[i] < minDist[u])) u = i;

        if (minDist[u] == INF) {          // 当前连通分量已处理完，开启新的分量
            ++components;
            for (int i = 0; i < n; ++i)
                if (!inTree[i]) { minDist[i] = 0; parent[i] = -1; break; }
            --cnt;                        // 本轮未加入顶点，重新选择
            continue;
        }

        inTree[u] = true;
        if (parent[u] != -1) result.push_back({parent[u], u, minDist[u]});

        for (int v = 0; v < n; ++v) {
            int w = g.weight(u, v);
            if (!inTree[v] && w != INF && w < minDist[v]) {
                minDist[v] = w;
                parent[v] = u;
            }
        }
    }
    ++components;
    return result;
}

// Kruskal 算法：按权值排序 + 并查集，作为对照实现
struct DSU {
    vector<int> p, r;
    explicit DSU(int n) : p(n), r(n, 0) { iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (r[a] < r[b]) swap(a, b);
        p[b] = a;
        if (r[a] == r[b]) ++r[a];
        return true;
    }
};

vector<Edge> kruskal(const GraphData& g) {
    vector<Edge> edges = g.edges;
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) { return a.w < b.w; });
    DSU dsu(g.n);
    vector<Edge> result;
    for (const auto& e : edges) {
        if (dsu.unite(e.u, e.v)) {
            result.push_back(e);
            if ((int)result.size() == g.n - 1) break;
        }
    }
    return result;
}

void printMST(const vector<Edge>& tree, bool isForest, int components) {
    if (tree.empty()) {
        cout << "没有可用边。\n";
        return;
    }
    if (isForest) cout << "图不连通，输出的是最小生成森林（" << components << " 个连通分量）：\n";
    else cout << "最小生成树：\n";
    int sum = 0;
    for (const auto& e : tree) {
        cout << "  " << e.u + 1 << " -- " << e.v + 1 << "  (权值 " << e.w << ")\n";
        sum += e.w;
    }
    cout << "总权值 = " << sum << '\n';
}

// ---------------------------------------------------------------------------
// 图的构造与交互
// ---------------------------------------------------------------------------
GraphData sampleUndirected() {
    GraphData g;
    g.n = 6;
    g.weighted = true;
    g.edges = {{0,1,6},{0,2,3},{0,3,5},{1,3,2},{1,4,4},
               {2,3,1},{2,4,7},{2,5,4},{3,4,3},{3,5,6},{4,5,2}};
    return g;
}

GraphData sampleDirected() {
    GraphData g;
    g.n = 6;
    g.directed = true;
    g.edges = {{0,1,1},{0,2,1},{1,3,1},{1,4,1},{2,5,1},{3,5,1},{4,5,1}};
    return g;
}

GraphData inputGraph() {
    GraphData g;
    cout << "请输入顶点数 n：";
    cin >> g.n;
    cout << "是否为有向图？(y/n)：";
    char c; cin >> c;
    g.directed = (c == 'y' || c == 'Y');
    cout << "是否为带权图？(y/n)：";
    cin >> c;
    g.weighted = (c == 'y' || c == 'Y');
    int m;
    cout << "请输入边数 m：";
    cin >> m;
    cout << "请依次输入每条边（顶点编号从 1 开始）：\n";
    for (int i = 0; i < m; ++i) {
        int u, v, w = 1;
        cout << "  边 " << i + 1 << "：";
        cin >> u >> v;
        if (g.weighted) cin >> w;
        if (u < 1 || u > g.n || v < 1 || v > g.n || u == v) {
            cout << "  顶点编号非法或为自环，已跳过该边。\n";
            continue;
        }
        g.edges.push_back({u - 1, v - 1, w});
    }
    return g;
}

int readStart(int n) {
    int s;
    cout << "请输入起始顶点编号 (1~" << n << ")：";
    cin >> s;
    if (s < 1 || s > n) { cout << "编号非法，默认从 1 号顶点出发。\n"; s = 1; }
    return s - 1;
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    GraphData g;
    cout << "========== 数据结构实验四：图的遍历与最小生成树 ==========\n";
    cout << "第一步：选择要处理的图\n";
    cout << "  1. 示例无向带权图（6 个顶点，用于验证 MST；MST 总权值为 11）\n";
    cout << "  2. 示例有向无权图（6 个顶点，用于验证遍历）\n";
    cout << "  3. 自定义图\n";
    cout << "请选择 (1/2/3)：";
    int choice;
    cin >> choice;
    if (choice == 1) g = sampleUndirected();
    else if (choice == 2) g = sampleDirected();
    else g = inputGraph();

    if (g.n <= 0) { cout << "顶点数非法，程序结束。\n"; return 0; }

    AdjMatrixGraph mg(g);
    AdjListGraph lg(g);

    cout << "\n当前图：" << g.n << " 个顶点，" << g.edges.size() << " 条边，"
         << (g.directed ? "有向" : "无向") << "，"
         << (g.weighted ? "带权" : "无权") << '\n';
    mg.printMatrix(g);
    lg.printList();

    while (true) {
        cout << "\n---------- 操作菜单 ----------\n";
        cout << "  1. 邻接矩阵：深度优先遍历 DFS\n";
        cout << "  2. 邻接矩阵：广度优先遍历 BFS\n";
        cout << "  3. 邻接表：深度优先遍历 DFS\n";
        cout << "  4. 邻接表：广度优先遍历 BFS\n";
        cout << "  5. 全图遍历（覆盖非连通分量：DFS + BFS）\n";
        cout << "  6. Prim 算法求最小生成树\n";
        cout << "  7. Kruskal 算法求最小生成树（对照）\n";
        cout << "  0. 退出\n";
        cout << "请选择：";
        cin >> choice;
        if (choice == 0) break;

        switch (choice) {
        case 1: { int s = readStart(g.n); cout << "[邻接矩阵] "; mg.dfs(s); break; }
        case 2: { int s = readStart(g.n); cout << "[邻接矩阵] "; mg.bfs(s); break; }
        case 3: { int s = readStart(g.n); cout << "[邻接表]   "; lg.dfs(s); break; }
        case 4: { int s = readStart(g.n); cout << "[邻接表]   "; lg.bfs(s); break; }
        case 5:
            cout << "[邻接矩阵] "; mg.dfsAll();
            cout << "[邻接表]   "; lg.dfsAll();
            cout << "[邻接矩阵] "; mg.bfsAll();
            cout << "[邻接表]   "; lg.bfsAll();
            break;
        case 6:
        case 7: {
            if (g.directed) { cout << "最小生成树只针对无向图，当前图为有向图。\n"; break; }
            if (!g.weighted) { cout << "当前为无权图，所有边权均为 1，仍可计算生成树。\n"; }
            if (choice == 6) {
                int comp = 0;
                auto tree = prim(mg, comp);
                printMST(tree, comp > 1, comp);
            } else {
                auto tree = kruskal(g);
                printMST(tree, (int)tree.size() != g.n - 1,
                         g.n - (int)tree.size());
            }
            break;
        }
        default: cout << "无效选择，请重新输入。\n";
        }
    }
    cout << "实验演示结束。\n";
    return 0;
}
