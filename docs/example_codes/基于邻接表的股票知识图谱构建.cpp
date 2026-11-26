
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

/*================ 学生函数声明 ================*/

//读取实体并创建顶点表
void CreateAdjList(ALGraph& G, string filename);
//根据实体名称查找顶点
int LocateVex(ALGraph& G, string str);
//根据三元组建立无向图
void CreateUDG(ALGraph& G, string filename, ofstream& fout);
//遍历邻接表并输出图结构
void PrintGraph(ALGraph& G, ofstream& fout);


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
 * 读取实体文件，建立图的顶点表（每个实体一个顶点）。
 *
 * 平台已经调用 InitALGraph，把 vexnum 和 arcnum 置 0；但不会清空 firstarc。
 * 本函数需要为每个顶点填写 entity、info，并把 firstarc 置为 NULL。
 *
 * @param G            （输出参数）邻接表表示的图
 * @param filename     实体文件名（entity.txt）
 */
void CreateAdjList(ALGraph& G, string filename){
    
}

/**
 * 在图中查找内容等于 str 的顶点。
 *
 * @param G            图
 * @param str          实体内容
 * @return 找到返回顶点下标；未找到返回 -1。
 */
int LocateVex(ALGraph& G, string str){
    
}

/**
 * 读取三元组文件，为每条三元组建立正向和反向两条弧（无向图，头插法）。
 *
 * 实体类型数组 Entity[] 和关系类型数组 Relationship[] 由平台提供，直接用下标访问，不要重复定义。
 *
 * @param G            （输出参数）邻接表表示的图
 * @param filename     三元组文件名（relation.txt）
 * @param fout         （输出参数）输出流，按题意需要输出时写入
 */
void CreateUDG(ALGraph& G, string filename, ofstream& fout){
    
}

/**
 * 遍历邻接表，输出图中全部弧；每行三个字段「实体 关系下标 内容」，用空格分隔，行末不要加空格。
 *
 * 每条三元组都建立了正向、反向两条弧，所以每条三元组会输出两行（正反各一次）。
 * 本数据集共 2700 条三元组，因此输出共 5400 行。
 *
 * @param G            图
 * @param fout         （输出参数）结果输出流
 */
void PrintGraph(ALGraph& G, ofstream& fout){
    
}


/*================ 主函数 ================*/

int main() {
    ALGraph G;

    // 初始化图
    InitALGraph(G);

    // 打开输出文件
    ofstream fout("result.txt");

    if (!fout.is_open()) {
        return 1;
    }

    // 从实体文件创建顶点邻接表
    CreateAdjList(G, "entity.txt");

    // 从关系文件创建无向图
    CreateUDG(G, "relation.txt", fout);

    // 将邻接表写入result.txt
    PrintGraph(G, fout);

    // 关闭输出文件
    fout.close();

    return 0;
}
