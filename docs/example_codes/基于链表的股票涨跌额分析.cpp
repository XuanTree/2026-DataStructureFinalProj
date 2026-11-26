
#include <bits/stdc++.h>
#define MAXSIZE 5000
using namespace std;

/*================ 结构体定义 ================*/

// 股票每日交易详情
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

// 股票基本信息
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

// 顺序表
typedef struct {
    stock* elem = nullptr;   // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
    int length = 0;
} SqList;

// 筛选后的股票链表结点
struct StockNode {
    string stockcode;        // 股票代码
    string indusname;        // 股票简称
    string induscode;        // 行业编码
    string date;             // 交易日期
    double risefallamount;   // 涨跌额
    StockNode* next;         // 后继结点指针
};


/*================ 初始化顺序表 ================*/

void InitList(SqList& L) {
    // elem[0]闲置，有效下标为1~MAXSIZE
    L.elem = new stock[MAXSIZE + 1];
    L.length = 0;
}


/*================ 释放顺序表 ================*/

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


/*================ 读取每日交易详情 ================*/

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


/*================ 读取股票基本信息 ================*/

void ReadFile(SqList& L, string infoFilename, string detailDir) {

    ifstream infile(infoFilename);

    if (!infile.is_open()) return;

    char s[5000];
    bool flag = false;

    // 按#分隔股票记录
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

        // 跳过无效记录
        if (temp.stockcode.empty()) continue;

        // 防止顺序表越界
        if (L.length >= MAXSIZE) break;

        // 读取该股票的每日交易详情
        ReadDetail(temp, detailDir);

        // 按原始顺序存入顺序表
        L.elem[++L.length] = temp;
    }

    infile.close();
}


/*================ 学生函数声明 ================*/

// 根据行业编码和日期筛选股票，
// 将负涨跌额记录放在前面，非负涨跌额记录放在后面。
StockNode* SplitStockListByRiseFall(
    SqList& L,
    const string& induscode,
    const string& targetdate
);

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
 * struct StockNode {
 *     string stockcode;        // 股票代码
 *     string indusname;        // 股票简称
 *     string induscode;        // 行业编码
 *     string date;             // 交易日期
 *     double risefallamount;   // 涨跌额
 *     StockNode* next;         // 后继结点指针
 * };
 */

/**
 * 按行业编码和日期筛选股票交易记录，并按涨跌额正负分成两组：负值在前、非负值在后，组内保持原始顺序。
 *
 * @param L            股票顺序表
 * @param induscode    目标行业编码
 * @param targetdate   目标交易日期
 * @return 返回合并后的单链表头指针（结点由函数 new 出来，调用方负责释放）。
 */
StockNode* SplitStockListByRiseFall(
    SqList& L,
    const string& induscode,
    const string& targetdate
) {

}


/*================ 释放股票链表 ================*/

void FreeStockList(StockNode*& head) {

    while (head != nullptr) {
        StockNode* tmp = head;
        head = head->next;
        delete tmp;
    }

    head = nullptr;
}


/*================ 主函数 ================*/

int main() {

    // ---------- 1. 初始化顺序表 ----------
    SqList L;
    InitList(L);

    // 保留原始股票数据路径
    string infoFilename = "stocks.txt";
    string detailDir = "detail";

    // ---------- 2. 读取股票数据 ----------
    ReadFile(L, infoFilename, detailDir);

    // ---------- 3. 从input.txt读取筛选条件 ----------
    ifstream fin("input.txt");
    if (!fin.is_open()) {
        FreeList(L);
        return 1;
    }
    string induscode;
    string targetdate;
    fin >> induscode >> targetdate;
    fin.close();

    // ---------- 4. 调用学生函数 ----------
    StockNode* head = SplitStockListByRiseFall(
        L, induscode, targetdate
    );

    // ---------- 5. 打开result.txt ----------
    ofstream fout("result.txt");
    if (!fout.is_open()) {
        FreeStockList(head);
        FreeList(L);
        return 1;
    }

    // ---------- 6. 输出筛选结果 ----------
    StockNode* p = head;
    while (p != nullptr) {
        fout << "股票代码: " << p->stockcode << " ";
        fout << "股票简称: " << p->indusname << " ";
        fout << "行业编码：" << p->induscode << " ";
        fout << "日期: " << p->date << " ";
        fout << "涨跌额: " << p->risefallamount << endl;
        p = p->next;
    }

    // ---------- 7. 释放资源 ----------
    fout.close();
    FreeStockList(head);
    FreeList(L);

    return 0;
}
