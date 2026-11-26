
#include <bits/stdc++.h>
#define MAXSIZE 5000
using namespace std;

/*================ 数据结构定义 ================*/

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

// 原始股票顺序表
struct SqList {
    stock* elem = nullptr;   // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
    int length = 0;
};

// 筛选结果及堆排序使用的顺序表
// 保存股票指针及对应日期的交易详情下标
typedef struct {
    stock* stockPtr = nullptr;
    int detailIndex = -1;
} HeapRecord;

typedef struct {
    HeapRecord* elem = nullptr;   // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
    int length = 0;
} HeapList;

/*================ 初始化股票顺序表 ================*/

void InitList(SqList& L) {
    // 使用下标1~MAXSIZE
    L.elem = new stock[MAXSIZE + 1];
    L.length = 0;
}

/*================ 释放股票顺序表 ================*/

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

/*================ 初始化堆顺序表 ================*/

void InitHeapList(HeapList& H) {
    H.elem = new HeapRecord[MAXSIZE + 1];
    H.length = 0;
}

/*================ 释放堆顺序表 ================*/

void FreeHeapList(HeapList& H) {
    // HeapRecord只保存指针，不拥有股票数据
    delete[] H.elem;
    H.elem = nullptr;
    H.length = 0;
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

        // 将分号替换为空格
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

void ReadFile(SqList& L,
              string infoFilename,
              string detailDir) {

    ifstream infile(infoFilename);

    if (!infile.is_open()) return;

    char buf[10000];
    bool flag = false;

    while (infile.getline(buf, sizeof(buf), '#')) {

        stock temp;
        stringstream stream(buf);
        string line;

        if (flag) {
            getline(stream, line, '\n');
        }

        flag = true;
        int typ = 0;

        while (getline(stream, line, '\n')) {

            if (!line.empty() && line.back() == '\r')
                line.pop_back();

            switch (typ) {
                case 0:
                    temp.stockcode = line.substr(5);
                    break;
                case 1:
                    temp.indusname = line.substr(13);
                    break;
                case 2:
                    temp.sname = line.substr(10);
                    break;
                case 3:
                    temp.wholecompany = line.substr(13);
                    break;
                case 4:
                    temp.listexchange = line.substr(16);
                    break;
                case 5:
                    temp.briname = line.substr(13);
                    break;
                case 6:
                    temp.pricate = line.substr(13);
                    break;
                case 7:
                    temp.seccate = line.substr(16);
                    break;
                case 8:
                    temp.launchdate = line.substr(13);
                    break;
                case 9:
                    temp.provin = line.substr(7);
                    break;
                case 10:
                    temp.city = line.substr(7);
                    break;
                case 11:
                    temp.legalper = line.substr(7);
                    break;
                case 12:
                    temp.addr = line.substr(7);
                    break;
                case 13:
                    temp.url = line.substr(7);
                    break;
                case 14:
                    temp.email = line.substr(7);
                    break;
                case 15:
                    temp.callnum = line.substr(7);
                    break;
                case 16:
                    temp.mainbus = line.substr(13);
                    break;
                case 17:
                    temp.scopeofbus = line.substr(13);
                    break;
                default:
                    break;
            }

            typ++;
        }

        if (temp.stockcode.empty()) continue;
        if (L.length >= MAXSIZE) break;

        ReadDetail(temp, detailDir);

        L.elem[++L.length] = temp;
    }

    infile.close();
}

/*================ 获取堆记录的成交量 ================*/

// 根据记录中的股票指针和交易详情下标
// 获取目标日期的成交量
double GetVolume(const HeapRecord& record) {
    if (record.stockPtr == nullptr ||
        record.detailIndex < 0 ||
        record.detailIndex >= record.stockPtr->detail_count) {
        return 0.0;
    }

    return record.stockPtr
                 ->detail[record.detailIndex].volume;
}

/*================ 学生函数声明 ================*/

// 按行业编码和交易日期筛选股票
void FilterStocks(
    SqList& L,
    HeapList& H,
    const string& targetInd,
    const string& targetDate
);

// 调整以s为根的子树，使其满足大根堆性质
void HeapAdjust(
    HeapList& H,
    int s,
    int heapSize,
    int& kcn,
    int& rmn
);

// 将无序顺序表构建成大根堆
void CreatHeap(
    HeapList& H,
    int& kcn,
    int& rmn
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
 * struct SqList {
 *     stock* elem = nullptr;   // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
 *     int length = 0;
 * };
 *
 * typedef struct {
 *     stock* stockPtr = nullptr;
 *     int detailIndex = -1;
 * } HeapRecord;
 *
 * typedef struct {
 *     HeapRecord* elem = nullptr;   // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
 *     int length = 0;
 * } HeapList;
 */

/**
 * 按行业编码和日期筛选股票，把符合条件的记录存入堆表 H。
 *
 * @param L            股票顺序表
 * @param H            （输出参数）存放筛选结果的堆表
 * @param targetInd    目标行业编码
 * @param targetDate   目标日期
 */
void FilterStocks(
    SqList& L,
    HeapList& H,
    const string& targetInd,
    const string& targetDate
) {

}

/**
 * 向下调整以 s 为根的子树，使其满足大根性质，并统计比较和移动次数。
 *
 * 比较左右子结点时成交量大的优先，成交量相同时取左子结点；
 * 当前记录与较大子结点的成交量相等时视为不小于，停止下沉。
 *
 * @param H            （输出参数）堆表
 * @param s            要调整的子树根下标
 * @param heapSize     当前堆的元素个数
 * @param kcn          （输出参数）比较次数
 * @param rmn          （输出参数）移动次数
 */
void HeapAdjust(
    HeapList& H,
    int s,
    int heapSize,
    int& kcn,
    int& rmn
) {

}

/**
 * 把无序的堆表 H 建成大根堆，并统计比较和移动次数。
 *
 * @param H            （输出参数）待建堆的顺序表
 * @param kcn          （输出参数）比较次数
 * @param rmn          （输出参数）移动次数
 */
void CreatHeap(
    HeapList& H,
    int& kcn,
    int& rmn
) {

}


/*================ 输出股票信息 ================*/

void PrintHeapRecord(
    const HeapRecord& record,
    ostream& out
) {

    if (record.stockPtr == nullptr) return;

    const stock& s = *record.stockPtr;

    if (record.detailIndex < 0 ||
        record.detailIndex >= s.detail_count) {
        return;
    }

    const stockdetail& d = s.detail[record.detailIndex];

    out << "股票代码: " << s.stockcode << '\n'
        << "股票简称: " << s.indusname << '\n'
        << "行业编码: " << s.briname << '\n'
        << "一级门类: " << s.pricate << '\n'
        << "二级门类: " << s.seccate << '\n'
        << "公司全称: " << s.wholecompany << '\n'
        << "日期: " << d.date
        << " 成交量: " << (long long)d.volume << '\n'
        << "----------------------------\n";
}


/*================ 主函数 ================*/

int main() {

    // ---------- 1. 初始化股票顺序表 ----------
    SqList L;
    InitList(L);

    // ---------- 2. 初始化堆顺序表 ----------
    HeapList H;
    InitHeapList(H);

    // ---------- 3. 读取股票数据 ----------
    string infoFile = "stocks.txt";
    string detailDir = "detail";
    ReadFile(L, infoFile, detailDir);

    // ---------- 4. 从input.txt读取筛选条件 ----------
    ifstream fin("input.txt");

    if (!fin.is_open()) {
        FreeHeapList(H);
        FreeList(L);
        return 1;
    }

    string targetInd;
    string targetDate;

    fin >> targetInd >> targetDate;
    fin.close();

    // ---------- 5. 筛选股票 ----------
    FilterStocks(
        L, H, targetInd, targetDate
    );

    // ---------- 6. 建立大根堆 ----------
    int kcn = 0;
    int rmn = 0;
    CreatHeap(H, kcn, rmn);

    // ---------- 7. 打开结果文件 ----------
    ofstream fout("result.txt");
    if (!fout.is_open()) {
        FreeHeapList(H);
        FreeList(L);
        return 1;
    }

    // ---------- 8. 输出成交量最大的前3支股票 ----------
    int num = 0;
    while (H.length > 0) {
        if (num == 3) break;
        PrintHeapRecord(H.elem[1], fout);
        H.elem[1] = H.elem[H.length--];
        HeapAdjust(H, 1, H.length, kcn, rmn);
        num++;
    }

    // ---------- 9. 输出比较和移动次数 ----------
    fout << "总的关键字比较次数KCN为：" << kcn << endl;
    fout << "总的记录移动次数RMN为：" << rmn << endl;

    // ---------- 10. 释放资源 ----------
    fout.close();

    FreeHeapList(H);
    FreeList(L);

    return 0;
}
