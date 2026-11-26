#include <bits/stdc++.h>
#define MAXSIZE 5000
using namespace std;

// ==================== 数据结构定义 ====================

typedef struct stockdetail {
    string date;
    double openprice;
    double closeprice;
    double highest;
    double lowest;
    double volume;
    double turnover;
    string changerate;
    string risefallrate;
    double risefallamount;
} stockdetail;

typedef struct stock {
    string stockcode;
    string indusname;
    string sname;
    string briname;
    string pricate;
    string seccate;
    string listexchange;
    string wholecompany;
    string launchdate;
    string provin;
    string city;
    string legalper;
    string addr;
    string url;
    string email;
    string callnum;
    string mainbus;
    string scopeofbus;

    stockdetail* detail = nullptr;
    int detail_count = 0;
} stock;

typedef struct {
    stock* elem = nullptr;   // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
    int length = 0;
} SqList;

typedef struct BSTNode {
    stock data;
    struct BSTNode* lchild;
    struct BSTNode* rchild;
} BSTNode, *BSTree;

// ==================== 初始化顺序表 ====================

void InitList(SqList& L) {
    // elem[0] 闲置，有效下标为 1 ~ MAXSIZE
    L.elem = new stock[MAXSIZE + 1];
    L.length = 0;
}

// ==================== 释放顺序表 ====================

void FreeList(SqList& L) {
    if (L.elem == nullptr) {
        L.length = 0;
        return;
    }

    // 每日价格数组由顺序表统一管理
    for (int i = 1; i <= L.length; i++) {
        delete[] L.elem[i].detail;
        L.elem[i].detail = nullptr;
    }

    delete[] L.elem;
    L.elem = nullptr;
    L.length = 0;
}

// ==================== 释放二叉排序树 ====================

// BST 结点中的 detail 与顺序表共享。
// 此处只释放树结点，不重复释放 detail。
void FreeBSTree(BSTree& T) {
    if (T == nullptr) {
        return;
    }

    FreeBSTree(T->lchild);
    FreeBSTree(T->rchild);

    delete T;
    T = nullptr;
}

// ==================== 读取每日价格信息 ====================

void ReadDetail(stock& s, string detailDir) {
    string filename = detailDir + "/" + s.stockcode + ".txt";
    ifstream file(filename);

    s.detail = nullptr;
    s.detail_count = 0;

    if (!file.is_open()) {
        return;
    }

    vector<stockdetail> records;
    string line;

    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.empty()) {
            continue;
        }

        // 原文件以分号分隔
        for (char& c : line) {
            if (c == ';') {
                c = ' ';
            }
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

    if (s.detail_count == 0) {
        return;
    }

    s.detail = new stockdetail[s.detail_count];

    for (int i = 0; i < s.detail_count; i++) {
        s.detail[i] = records[i];
    }
}

// ==================== 读取股票基本信息 ====================

void ReadFile(SqList& L, string infoFilename, string detailDir) {
    ifstream infile(infoFilename);

    if (!infile.is_open()) {
        return;
    }

    char s[5000];
    bool flag = false;

    // 股票记录使用 # 分隔
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
            if (!ss.empty() && ss.back() == '\r') {
                ss.pop_back();
            }

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

        if (temp.stockcode.empty()) {
            continue;
        }

        if (L.length >= MAXSIZE) {
            break;
        }

        ReadDetail(temp, detailDir);
        L.elem[++L.length] = temp;
    }

    infile.close();
}

// ==================== 初始化二叉排序树 ====================

void InitBSTree(BSTree& T) {
    T = nullptr;
}

// ==================== 学生函数声明 ====================
// 插入股票结点
void InsertBST(BSTree& T, stock e);

// 查找指定英文名称的股票
BSTNode* SearchBST(BSTree& T, string sname);

// 计算二叉排序树成功查找的平均查找长度
double GetASL(BSTree& T, int count);


/**
 *
 * typedef struct stockdetail {
 *     string date;
 *     double openprice;
 *     double closeprice;
 *     double highest;
 *     double lowest;
 *     double volume;
 *     double turnover;
 *     string changerate;
 *     string risefallrate;
 *     double risefallamount;
 * } stockdetail;
 *
 * typedef struct stock {
 *     string stockcode;
 *     string indusname;
 *     string sname;
 *     string briname;
 *     string pricate;
 *     string seccate;
 *     string listexchange;
 *     string wholecompany;
 *     string launchdate;
 *     string provin;
 *     string city;
 *     string legalper;
 *     string addr;
 *     string url;
 *     string email;
 *     string callnum;
 *     string mainbus;
 *     string scopeofbus;
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
 * typedef struct BSTNode {
 *     stock data;
 *     struct BSTNode* lchild;
 *     struct BSTNode* rchild;
 * } BSTNode, *BSTree;
 */

/**
 * 按股票的英文名把记录 e 插入二叉排序树（按 sname 比较，更小走左、更大走右）。
 *
 * @param T            （输出参数）树的根指针，插入后可能改变
 * @param e            待插入的股票记录
 */
void InsertBST(BSTree& T, stock e) {

}

/**
 * 在二叉排序树中查找英文名为 sname 的股票。
 *
 * @param T            树的根指针
 * @param sname        待查找的股票英文名
 * @return 找到返回对应结点指针；未找到返回 nullptr。
 */
BSTNode* SearchBST(BSTree& T, string sname) {

}

/**
 * 计算二叉排序树成功查找的平均查找长度。
 *
 * 某个结点的查找长度等于它所在的层数（根结点为 1），ASL 就是所有结点查找长度的平均值。
 *
 * @param T            树的根指针
 * @param count        树中结点总数
 * @return 返回 ASL = 各结点查找长度之和 / count。
 */
double GetASL(BSTree& T, int count) {

}


// ==================== 构建二叉排序树 ====================

// 按顺序表中的股票顺序依次插入
void buildTree(BSTree& T, SqList L) {
    for (int i = 1; i <= L.length; i++) {
        InsertBST(T, L.elem[i]);
    }
}


// ==================== 输出股票信息 ====================

void Print(BSTNode* T, ostream& out) {
    if (T == nullptr) {
        return;
    }

    const stock& s = T->data;

    out << "股票代码: " << s.stockcode << endl;
    out << "股票简称: " << s.indusname << endl;
    out << "英文名: " << s.sname << endl;
    out << "行业编码: " << s.briname << endl;
    out << "一级门类: " << s.pricate << endl;
    out << "二级门类: " << s.seccate << endl;
    out << "上市交易所: " << s.listexchange << endl;
    out << "公司全称: " << s.wholecompany << endl;
    out << "上市日期: " << s.launchdate << endl;
    out << "省份: " << s.provin << endl;
    out << "城市: " << s.city << endl;
    out << "法人: " << s.legalper << endl;
    out << "地址: " << s.addr << endl;
    out << "网址: " << s.url << endl;
    out << "邮箱: " << s.email << endl;
    out << "电话: " << s.callnum << endl;
    out << "主营业务: " << s.mainbus << endl;
    out << "经营范围: " << s.scopeofbus << endl;

    for (int i = 0; i < s.detail_count; i++) {
        const stockdetail& d = s.detail[i];

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



// ==================== 主函数 ====================

int main() {
    BSTree T = nullptr;

    InitBSTree(T);

    SqList L;
    InitList(L);

    // 保留原实验的数据路径
    string infoFilename ="stocks.txt";
    string detailDir ="detail";

    // ---------- 1. 读取股票信息 ----------
    ReadFile(L, infoFilename, detailDir);

    // ---------- 2. 根据原始数据构建二叉排序树 ----------
    buildTree(T, L);

    // ---------- 3. 从文件读取查询英文名称 ----------
    ifstream fin("input.txt");

    if (!fin.is_open()) {
        FreeBSTree(T);
        FreeList(L);
        return 1;
    }

    string sname;
    getline(fin, sname);

    if (!sname.empty() && sname.back() == '\r') {
        sname.pop_back();
    }

    fin.close();

    // ---------- 4. 打开结果文件 ----------
    ofstream fout("result.txt");

    if (!fout.is_open()) {
        FreeBSTree(T);
        FreeList(L);
        return 1;
    }

    // ---------- 5. 查找并输出 ----------
    BSTNode* node = SearchBST(T, sname);

    if (node != nullptr) {
        Print(node, fout);

        double ASL = GetASL(T, L.length);

        fout << "ASL为：" << fixed << setprecision(2) << ASL << endl;
    } else {
        fout << "查找失败" << endl;
    }

    // ---------- 6. 释放资源 ----------
    fout.close();

    FreeBSTree(T);
    FreeList(L);

    return 0;
}

