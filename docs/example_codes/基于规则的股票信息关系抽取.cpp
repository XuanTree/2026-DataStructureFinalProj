
#include <bits/stdc++.h>
#define MAXSIZE 10000
using namespace std;

/*================ 数据结构定义 ================*/

typedef struct {
    string relation;    // 关系名称
    string rule[10];    // 规则数组
} Relation;

/*================ 学生函数声明 ================*/

// 字符串匹配函数
int BF(string t, string s);
//查找实体所有匹配位置
void findAllMatches(string t, string s, int positions[], int &count) ;
//关系抽取函数
bool RelationExtraction(
    char* text,
    char* entity1,
    char* entity2,
    Relation* r,
    ofstream& fout
);


/**
 *
 * typedef struct {
 *     string relation;    // 关系名称
 *     string rule[10];    // 规则数组
 * } Relation;
 */

/**
 * 用 BF（朴素模式匹配）在主串 t 中查找模式串 s。
 *
 * @param t            主串
 * @param s            模式串
 * @return 找到返回起始下标；未找到返回 -1。
 */
int BF(string t, string s) {
    

}

/**
 * 找出模式串 s 在主串 t 中出现的全部位置。
 *
 * @param t            主串
 * @param s            模式串
 * @param positions    （输出参数）存放所有匹配下标的数组
 * @param count        （输出参数）匹配个数，函数会写入
 */
void findAllMatches(string t, string s, int positions[], int &count) {

}

/**
 * 判断两个实体之间是否存在规则表中定义的关系，并把三元组写入输出流 fout。
 *
 * 规则表共 relationCount 组，合法下标为 r[0] ~ r[relationCount-1]。
 * 同一对实体在文本中多次出现且都能匹配规则时，按出现顺序输出所有匹配的三元组。
 *
 * @param text          非结构化文本
 * @param entity1       实体 1
 * @param entity2       实体 2
 * @param r             关系规则表（平台已初始化）
 * @param relationCount 规则组数
 * @param fout          （输出参数）结果输出流，函数会写入三元组
 * @return 存在关系返回 true；否则返回 false。
 */
bool RelationExtraction(
    char* text,
    char* entity1,
    char* entity2,
    Relation* r,
    int relationCount,
    ofstream& fout
) {
  
}


/*================ 关系初始化函数 ================*/

int InitRelation(Relation *r) {
    // 关系初始化函数

    // 初始化第一个关系
    r[0].relation = "属于行业大类";
    r[0].rule[0] = "A行业类别为B";
    r[0].rule[1] = "A属于B";
    r[0].rule[2] = "";

    // 初始化第二个关系
    r[1].relation = "所属机构";
    r[1].rule[0] = "A所属机构为B";
    r[1].rule[1] = "A的公司是B";
    r[1].rule[2] = "A隶属B";
    r[1].rule[3] = "";

    // 初始化第三个关系
    r[2].relation = "具有注册地址";
    r[2].rule[0] = "A位于B";
    r[2].rule[1] = "A坐落于B";
    r[2].rule[2] = "A总部在B";
    r[2].rule[3] = "";

    // 初始化第四个关系
    r[3].relation = "主营业务为";
    r[3].rule[0] = "A包括B";
    r[3].rule[1] = "A专注于B";
    r[3].rule[2] = "A主营业务为B";
    r[3].rule[3] = "";

    // 标记结束
    r[4].relation = "";

    return 4;
}


/*================ 主函数 ================*/

int main() {
    char entity1[100];
    char entity2[100];
    char text[MAXSIZE];

    Relation r[10];
    int relationCount = InitRelation(r);

    // 打开输入输出文件
    ifstream fin("input.txt");
    ofstream fout("result.txt");

    if (!fin.is_open() || !fout.is_open()) {
        return 1;
    }

    // 从文本文件读取两个实体和待识别文本
    fin.getline(entity1, 100);
    fin.getline(entity2, 100);
    fin.getline(text, MAXSIZE);

    // 调用关系抽取函数
    if (!RelationExtraction(text, entity1, entity2, r, relationCount, fout))
        fout << "实体之间不存在关系" << endl;

    fin.close();
    fout.close();

    return 0;
}
