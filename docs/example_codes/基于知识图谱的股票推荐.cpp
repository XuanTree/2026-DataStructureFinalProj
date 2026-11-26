
#include <bits/stdc++.h>
#define MVNum 30000
using namespace std;

/*================ 数据结构定义 ================*/

// 定义关系数组，用于表示边的类型
string Relationship[] = {
    "所属机构", "上市于", "具有行业编码",
    "属于行业大类", "属于行业二级类",
    "位于省份", "位于城市",
    "具有注册地址", "主营业务为"
};

// 定义顶点数组，用于表示实体类型和内容
string Entity[] = {
    "股票简称", "机构", "上市交易所",
    "行业编码", "行业大类", "行业二级类",
    "省份", "城市", "地址", "主营业务"
};

// 定义边结点结构体
typedef struct ArcNode {
    int adjvex;                     // 该边所指向顶点的位置
    int relationship;               // 表示边的类型，即关系的类型，对应为数组下标
    struct ArcNode* nextarc;        // 下一条边
} ArcNode;

// 定义顶点结点结构体
typedef struct VNode {
    int entity;                     // 表示顶点的类型，即实体的类型，对应为数组下标
    string info;                    // 表示顶点的内容，即实体的内容
    ArcNode* firstarc;              // 指向第一条依附该顶点的边的指针
} VNode, AdjList[MVNum];

// 定义图的结构体
typedef struct {
    AdjList vertices;               // 邻接表
    int vexnum, arcnum;             // 图的当前顶点数和边数
} ALGraph;


/*================ 实体和关系定位函数 ================*/

int LocateEntity(string str) {
    // 返回str在Entity数组中的位置
    int i = 0;
    for (; i < 10; i++)
        if (str == Entity[i])
            return i;

    return -1;
}

int LocateRelationship(string str) {
    // 返回str在Relationship数组中的位置
    int i = 0;
    for (; i < 9; i++)
        if (str == Relationship[i])
            return i;

    return -1;
}


/*================ 初始化邻接表 ================*/

void InitALGraph(ALGraph& G) {
    // 初始化顶点数和边数为0
    G.vexnum = 0;
    G.arcnum = 0;
}


/*================ 创建顶点邻接表 ================*/

void CreateAdjList(ALGraph& G, string filename) {
    // 打开输入文件，准备从中读取数据
    ifstream pf(filename);

    if (!pf) {
        perror("Load");
        exit(0);
    }

    string s1, s2, line;

    // 循环读取文件中的每一行数据
    while (getline(pf, line)) {
        stringstream ss(line);
        ss >> s1 >> s2;

        // 将读取的实体名称存入顶点信息
        G.vertices[G.vexnum].info = s1;

        // 定位实体在图中的位置
        G.vertices[G.vexnum].entity = LocateEntity(s2);

        // 为新的顶点初始化第一条边指针为NULL
        G.vertices[G.vexnum++].firstarc = NULL;
    }

    // 关闭文件
    pf.close();
}


/*================ 查找顶点位置 ================*/

int LocateVex(ALGraph& G, string str) {
    // 返回str在AdjList中的位置
    for (int i = 0; i < G.vexnum; i++)
        if (G.vertices[i].info == str)
            return i;

    return -1;
}


/*================ 创建无向图 ================*/

void CreateUDG(ALGraph& G, string filename, ofstream& fout) {
    // 打开关系文件
    ifstream pf(filename);

    if (!pf) {
        perror("Load");
        exit(0);
    }

    string v1, v2, v3, line;

    // 逐行读取关系数据
    while (getline(pf, line)) {
        stringstream ss(line);
        ss >> v1 >> v2 >> v3;

        // 查找关系两端的顶点
        int i = LocateVex(G, v1);
        int j = LocateVex(G, v3);

        // 检查顶点是否存在
        if (i == -1 || j == -1) {
            fout << "顶点未找到: "
                 << v1 << " 或 " << v3 << endl;
            continue;
        }

        // 查找关系类型
        int rel = LocateRelationship(v2);

        if (rel == -1) {
            fout << "关系未定义: "
                 << v2 << endl;
            continue;
        }

        // 创建第一条边
        ArcNode* p1 = new ArcNode;
        p1->adjvex = j;
        p1->relationship = rel;
        p1->nextarc = G.vertices[i].firstarc;
        G.vertices[i].firstarc = p1;

        // 创建反向边
        ArcNode* p2 = new ArcNode;
        p2->adjvex = i;
        p2->relationship = rel;
        p2->nextarc = G.vertices[j].firstarc;
        G.vertices[j].firstarc = p2;
    }

    // 关闭文件
    pf.close();
}

/*================ 学生函数声明 ================*/

// 根据实体查询股票
void SearchByEntity(
    ALGraph& G,
    const string& entityName,
    ofstream& fout
);

/**
 *
 * typedef struct ArcNode {
 *     int adjvex;                     // 该边所指向顶点的位置
 *     int relationship;               // 表示边的类型，即关系的类型，对应为数组下标
 *     struct ArcNode* nextarc;        // 下一条边
 * } ArcNode;
 *
 * typedef struct VNode {
 *     int entity;                     // 表示顶点的类型，即实体的类型，对应为数组下标
 *     string info;                    // 表示顶点的内容，即实体的内容
 *     ArcNode* firstarc;              // 指向第一条依附该顶点的边的指针
 * } VNode, AdjList[MVNum];
 *
 * typedef struct {
 *     AdjList vertices;               // 邻接表
 *     int vexnum, arcnum;             // 图的当前顶点数和边数
 * } ALGraph;
 */

/**
 * 从实体 entityName 出发，在知识图谱中查找具有该信息的股票，并按固定顺序输出推荐序列。
 *
 * @param G            知识图谱
 * @param entityName   查询的实体内容
 * @param fout         （输出参数）结果输出流，函数会写入推荐序列
 */
void SearchByEntity(
    ALGraph& G,
    const string& entityName,
    ofstream& fout
){
    
}


/*================ 主函数 ================*/

int main() {
    ALGraph G;

    // 初始化图
    InitALGraph(G);

    // 打开输入输出文件
    ifstream fin("input.txt");
    ofstream fout("result.txt");

    if (!fin.is_open() || !fout.is_open()) {
        return 1;
    }

    // 从实体文件创建顶点邻接表
    CreateAdjList(G, "entity.txt");

    // 从关系文件创建无向图
    CreateUDG(G, "relation.txt", fout);

    // 从input.txt读取需要查询的实体名称
    string entityToSearch;
    fin >> entityToSearch;

    // 查询与目标实体相连的股票及其相关信息
    SearchByEntity(G, entityToSearch, fout);

    // 关闭输入输出文件
    fin.close();
    fout.close();

    return 0;
}
