
#include <bits/stdc++.h>
#define MAXSIZE 5000
using namespace std;

// ==================== 数据结构定义 ====================

// 股票每日价格信息
typedef struct stockdetail {
    string date;             // 日期
    double openprice;        // 开盘价
    double closeprice;       // 收盘价
    double highest;          // 最高价
    double lowest;           // 最低价
    double volume;           // 成交量
    double turnover;         // 成交额
    string changerate;       // 换手率
    string risefallrate;     // 涨跌幅
    double risefallamount;   // 涨跌额
} stockdetail;

// 股票基本信息
typedef struct stock {
    string stockcode;        // 股票代码
    string indusname;        // 股票简称
    string sname;            // 英文名
    string briname;          // 行业编码
    string pricate;          // 一级门类
    string seccate;          // 二级门类
    string listexchange;     // 上市交易所
    string wholecompany;     // 公司全称
    string launchdate;       // 上市日期
    string provin;           // 省份
    string city;             // 城市
    string legalper;         // 法人
    string addr;             // 地址
    string url;              // 网址
    string email;            // 邮箱
    string callnum;          // 电话
    string mainbus;          // 主营业务
    string scopeofbus;       // 经营范围

    stockdetail* detail = nullptr;
    int detail_count = 0;
} stock;

// 顺序表
typedef struct {
    stock* elem = nullptr;   // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
    int length = 0;
} SqList;

// 字典树结点
typedef struct TNode {
    // 指向以当前路径为完整英文名称的股票
    stock* herbPtr;

    // 按字符的 ASCII 码（0~127）索引
    struct TNode* child[128];   // 按字符 ASCII 码索引，覆盖英文名中出现的全部字符
} TNode, *TrieTree;

// ==================== 初始化顺序表 ====================

void InitList(SqList& L) {
    // elem[0]闲置，使用下标1~MAXSIZE
    L.elem = new stock[MAXSIZE + 1];
    L.length = 0;
}

// ==================== 释放顺序表 ====================

void FreeList(SqList& L) {
    if (L.elem == nullptr) return;

    for (int i = 1; i <= L.length; i++) {
        delete[] L.elem[i].detail;
        L.elem[i].detail = nullptr;
    }

    delete[] L.elem;
    L.elem = nullptr;
    L.length = 0;
}

// ==================== 初始化字典树结点 ====================

TNode* InitTNode() {
    TNode* p = new TNode;

    p->herbPtr = nullptr;

    for (int i = 0; i < 128; i++) {
        p->child[i] = nullptr;
    }

    return p;
}

// ==================== 读取每日价格信息 ====================

void ReadDetail(stock& s, string detailDir) {

    string filename = detailDir + "/" + s.stockcode + ".txt";
    ifstream file(filename);

    s.detail = nullptr;
    s.detail_count = 0;

    if (!file.is_open()) return;

    vector<stockdetail> records;
    string line;

    while (getline(file, line)) {

        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        if (line.empty()) continue;

        // 原始文件使用分号分隔
        for (char& c : line) {
            if (c == ';') c = ' ';
        }

        stockdetail d{};
        stringstream ss(line);

        if (ss >> d.date
               >> d.openprice
               >> d.closeprice
               >> d.highest
               >> d.lowest
               >> d.volume
               >> d.turnover
               >> d.changerate
               >> d.risefallrate
               >> d.risefallamount) {
            records.push_back(d);
        }
    }

    file.close();

    s.detail_count = (int)records.size();

    if (s.detail_count == 0) return;

    s.detail = new stockdetail[s.detail_count];

    for (int i = 0; i < s.detail_count; i++) {
        s.detail[i] = records[i];
    }
}

// ==================== 读取股票基本信息 ====================

void ReadFile(SqList& L, string infoFilename, string detailDir) {

    ifstream infile(infoFilename);

    if (!infile.is_open()) return;

    char s[5000];
    bool flag = false;

    // 原始股票记录使用#分隔
    while (infile.getline(s, sizeof(s), '#')) {

        stock temp;
        stringstream streams(s);
        string ss;
        int type = 0;

        if (flag) {
            getline(streams, ss, '\n');
        }
        flag = true;

        while (getline(streams, ss, '\n')) {

            if (!ss.empty() && ss.back() == '\r')
                ss.pop_back();

            switch (type) {
                case 0:
                    temp.stockcode = ss.substr(5);
                    break;

                case 1:
                    temp.indusname = ss.substr(13);
                    break;

                case 2:
                    temp.sname = ss.substr(10);
                    break;

                case 3:
                    temp.wholecompany = ss.substr(13);
                    break;

                case 4:
                    temp.listexchange = ss.substr(16);
                    break;

                case 5:
                    temp.briname = ss.substr(13);
                    break;

                case 6:
                    temp.pricate = ss.substr(13);
                    break;

                case 7:
                    temp.seccate = ss.substr(16);
                    break;

                case 8:
                    temp.launchdate = ss.substr(13);
                    break;

                case 9:
                    temp.provin = ss.substr(7);
                    break;

                case 10:
                    temp.city = ss.substr(7);
                    break;

                case 11:
                    temp.legalper = ss.substr(7);
                    break;

                case 12:
                    temp.addr = ss.substr(7);
                    break;

                case 13:
                    temp.url = ss.substr(7);
                    break;

                case 14:
                    temp.email = ss.substr(7);
                    break;

                case 15:
                    temp.callnum = ss.substr(7);
                    break;

                case 16:
                    temp.mainbus = ss.substr(13);
                    break;

                case 17:
                    temp.scopeofbus = ss.substr(13);
                    break;

                default:
                    break;
            }

            type++;
        }

        if (temp.stockcode.empty()) continue;

        if (L.length >= MAXSIZE) break;

        ReadDetail(temp, detailDir);

        L.elem[++L.length] = temp;
    }

    infile.close();
}

// ==================== 学生函数声明 ====================

// 构建字典树
TNode* BuildTree(SqList& L);

// 查找前缀对应的字典树结点
TNode* SearchPrefixNode(TNode* root, string prefix);

// 收集指定子树中的所有股票

// 计算字典树成功查找的平均查找长度
double GetASL(SqList& L);


/**
 *
 * typedef struct stockdetail {
 *     string date;             // 日期
 *     double openprice;        // 开盘价
 *     double closeprice;       // 收盘价
 *     double highest;          // 最高价
 *     double lowest;           // 最低价
 *     double volume;           // 成交量
 *     double turnover;         // 成交额
 *     string changerate;       // 换手率
 *     string risefallrate;     // 涨跌幅
 *     double risefallamount;   // 涨跌额
 * } stockdetail;
 *
 * typedef struct stock {
 *     string stockcode;        // 股票代码
 *     string indusname;        // 股票简称
 *     string sname;            // 英文名
 *     string briname;          // 行业编码
 *     string pricate;          // 一级门类
 *     string seccate;          // 二级门类
 *     string listexchange;     // 上市交易所
 *     string wholecompany;     // 公司全称
 *     string launchdate;       // 上市日期
 *     string provin;           // 省份
 *     string city;             // 城市
 *     string legalper;         // 法人
 *     string addr;             // 地址
 *     string url;              // 网址
 *     string email;            // 邮箱
 *     string callnum;          // 电话
 *     string mainbus;          // 主营业务
 *     string scopeofbus;       // 经营范围
 * 
 *     stockdetail* detail = nullptr;
 *     int detail_count = 0;
 * } stock;
 *
 * typedef struct {
 *     stock* elem = nullptr;   // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
 *     int length = 0;
 * } SqList;
 *
 * typedef struct TNode {
 *     // 指向以当前路径为完整英文名称的股票
 *     stock* herbPtr;
 * 
 *     // 按字符的 ASCII 码（0~127）索引
 *     struct TNode* child[128];
 * } TNode, *TrieTree;
 */

/**
 * 根据顺序表 L 中所有股票的英文名构建字典树，每个结点保存一个字符。
 * 每个字符都按它的 ASCII 码作为 child 数组的下标。
 *
 * @param L            股票顺序表
 * @return 返回字典树的根结点指针。
 */
TNode* BuildTree(SqList& L) {

}

/**
 * 在字典树中查找英文前缀 prefix 对应的结点。
 * 每个字符同样按 ASCII 码取 child 下标。
 *
 * @param root         字典树根结点
 * @param prefix       待查找的英文前缀
 * @return 找到返回该前缀对应的结点指针；不存在返回 nullptr。
 */
TNode* SearchPrefixNode(TNode* root, string prefix) {

}

/**
 * 计算字典树成功查找的平均查找长度（所有英文名长度之和 / 股票数）。
 *
 * @param L            股票顺序表
 * @return 返回 ASL。
 */
double GetASL(SqList& L) {

}


// ==================== 输出股票信息 ====================

void Print(stock* s, ostream& out) {

    if (s == nullptr) return;

    out << "股票代码: " << s->stockcode << endl;
    out << "股票简称: " << s->indusname << endl;
    out << "英文名: " << s->sname << endl;
    out << "行业编码: " << s->briname << endl;
    out << "一级门类: " << s->pricate << endl;
    out << "二级门类: " << s->seccate << endl;
    out << "上市交易所: " << s->listexchange << endl;
    out << "公司全称: " << s->wholecompany << endl;
    out << "上市日期: " << s->launchdate << endl;
    out << "省份: " << s->provin << endl;
    out << "城市: " << s->city << endl;
    out << "法人: " << s->legalper << endl;
    out << "地址: " << s->addr << endl;
    out << "网址: " << s->url << endl;
    out << "邮箱: " << s->email << endl;
    out << "电话: " << s->callnum << endl;
    out << "主营业务: " << s->mainbus << endl;
    out << "经营范围: " << s->scopeofbus << endl;

    for (int i = 0; i < s->detail_count; i++) {

        const stockdetail& d = s->detail[i];

        out << "日期: " << d.date
            << fixed << setprecision(3)
            << " 开盘价: " << d.openprice
            << " 收盘价: " << d.closeprice
            << " 最高价: " << d.highest
            << " 最低价: " << d.lowest
            << " 成交量: " << d.volume
            << " 成交额: " << d.turnover
            << " 换手率: " << d.changerate
            << " 涨跌幅: " << d.risefallrate
            << " 涨跌额: " << d.risefallamount
            << endl;
    }
}


// ==================== 释放字典树 ====================

// 字典树结点中的herbPtr指向顺序表中的股票。
// 只释放树结点，不释放股票记录。
void FreeTrie(TNode* root) {

    if (root == nullptr) return;

    for (int i = 0; i < 128; i++) {
        FreeTrie(root->child[i]);
    }

    delete root;
}




// ==================== 收集子树中的股票 ====================

// 平台驱动输出用：把以 node 为根的子树里所有股票指针收集到 s。
// 学生不需要实现这个函数，它只服务于结果输出。
void CollectStocks(TNode* node, stock** s, int& count) {

    if (node == nullptr) return;

    if (node->herbPtr != nullptr) {
        s[count++] = node->herbPtr;
    }

    for (int i = 0; i < 128; i++) {
        if (node->child[i] != nullptr) {
            CollectStocks(node->child[i], s, count);
        }
    }
}


// ==================== 主函数 ====================

int main() {

    // ---------- 1. 初始化顺序表 ----------
    SqList L;
    InitList(L);

    // 保留原始股票数据路径
    string infoFilename = "stocks.txt";
    string detailDir ="detail";

    // ---------- 2. 读取股票数据 ----------
    ReadFile(L, infoFilename, detailDir);

    // ---------- 3. 构建字典树 ----------
    TNode* root = BuildTree(L);

    // ---------- 4. 从input.txt读取查询前缀 ----------
    ifstream fin("input.txt");
    if (!fin.is_open()) {
        FreeTrie(root);
        FreeList(L);
        return 1;
    }
    string prefix;
    getline(fin, prefix);
    if (!prefix.empty() && prefix.back() == '\r')
        prefix.pop_back();
    fin.close();

    // ---------- 5. 查找前缀结点 ----------
    TNode* prefixNode = SearchPrefixNode(root, prefix);

    // ---------- 6. 收集匹配的股票 ----------
    stock** matchedStocks = new stock*[MAXSIZE];

    int count = 0;

    if (prefixNode != nullptr) {
        CollectStocks(prefixNode, matchedStocks, count);
    }

    // ---------- 7. 打开结果文件 ----------
    ofstream fout("result.txt");

    if (!fout.is_open()) {
        delete[] matchedStocks;
        FreeTrie(root);
        FreeList(L);
        return 1;
    }

    // ---------- 8. 输出查询结果 ----------
    if (count > 0) {

        // 输出所有匹配的股票
        for (int i = 0; i < count; i++) {
            Print(matchedStocks[i], fout);
        }

        // 计算并输出ASL
        double ASL = GetASL(L);

        fout << "ASL为：" << fixed << setprecision(2) << ASL << endl;

    } else {

        fout << "查找失败" << endl;
    }

    // ---------- 9. 释放资源 ----------
    fout.close();

    delete[] matchedStocks;
    FreeTrie(root);
    FreeList(L);

    return 0;
}
