#include <bits/stdc++.h>
#define MAXSIZE 5000
using namespace std;

/*================ 数据结构定义 ================*/

// 股票每日交易详情
typedef struct stockdetail {
    string date;             // 交易日期
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

// 股票顺序表
struct SqList {
    stock* elem = nullptr;   // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
    int length = 0;
};

// 待排序记录
// 保存股票指针和目标日期的交易详情下标
struct SortRecord {
    stock* stockPtr = nullptr;
    int detailIndex = -1;
};

// 排序顺序表
struct SortList {
    SortRecord* elem = nullptr;   // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
    int length = 0;
};

/*================ 初始化股票顺序表 ================*/

void InitList(SqList& L) {
    // 下标0闲置，有效下标为1~MAXSIZE
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

/*================ 初始化排序顺序表 ================*/

void InitSortList(SortList& SL) {
    SL.elem = new SortRecord[MAXSIZE + 1];
    SL.length = 0;
}

/*================ 释放排序顺序表 ================*/

void FreeSortList(SortList& SL) {
    // 排序记录不拥有股票数据，只释放自身数组
    delete[] SL.elem;
    SL.elem = nullptr;
    SL.length = 0;
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

    string segment;
    bool firstSegment = true;

    // 股票记录使用#分隔
    while (getline(infile, segment, '#')) {

        // 跳过分隔符后的换行
        if (!firstSegment) {
            size_t pos = segment.find_first_not_of("\r\n");
            if (pos == string::npos) continue;
            segment.erase(0, pos);
        }
        firstSegment = false;

        stock temp;
        stringstream stream(segment);
        string line;
        int fieldType = 0;

        while (getline(stream, line)) {

            if (!line.empty() && line.back() == '\r')
                line.pop_back();

            // 处理UTF-8 BOM
            if (fieldType == 0 &&
                line.size() >= 3 &&
                (unsigned char)line[0] == 0xEF &&
                (unsigned char)line[1] == 0xBB &&
                (unsigned char)line[2] == 0xBF) {
                line.erase(0, 3);
            }

            // 按原始文件的固定字段格式提取
            switch (fieldType) {
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

            fieldType++;
        }

        // 跳过无效股票记录
        if (temp.stockcode.empty()) continue;

        // 防止超过顺序表容量
        if (L.length >= MAXSIZE) break;

        ReadDetail(temp, detailDir);

        L.elem[++L.length] = temp;
    }

    infile.close();
}

/*================ 获取目标日期成交量 ================*/

// 根据排序记录获取对应股票在目标日期的成交量
double GetVolume(const SortRecord& record) {

    if (record.stockPtr == nullptr)
        return 0.0;

    const stock& s = *record.stockPtr;

    if (record.detailIndex < 0 ||
        record.detailIndex >= s.detail_count)
        return 0.0;

    return s.detail[record.detailIndex].volume;
}

/*================ 学生函数声明 ================*/

// 根据指定日期筛选股票
void FilterStocksByDate(
    SqList& L,
    SortList& SL,
    const string& targetDate
);

// 直接插入排序
void InsertSort(
    SortList& SL,
    int& kcn,
    int& rmn
);


/**
 *
 * typedef struct stockdetail {
 *     string date;             // 交易日期
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
 * struct SqList {
 *     stock* elem = nullptr;   // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
 *     int length = 0;
 * };
 *
 * struct SortRecord {
 *     stock* stockPtr = nullptr;
 *     int detailIndex = -1;
 * };
 *
 * struct SortList {
 *     SortRecord* elem = nullptr;   // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
 *     int length = 0;
 * };
 */

/**
 * 筛选出指定日期有交易记录的股票，结果存入排序表 SL。
 *
 * @param L            股票顺序表
 * @param SL           （输出参数）存放筛选结果的排序表
 * @param targetDate   指定日期
 */
void FilterStocksByDate(
    SqList& L,
    SortList& SL,
    const string& targetDate
) {

}

/**
 * 对 SL 按成交量从小到大做直接插入排序，并统计比较和移动次数。
 *
 * @param SL           （输出参数）待排序的排序表，排序后被就地修改
 * @param kcn          （输出参数）关键字比较次数，函数会累加
 * @param rmn          （输出参数）记录移动次数，函数会累加
 */
void InsertSort(
    SortList& SL,
    int& kcn,
    int& rmn
) {

}


/*================ 输出股票信息 ================*/

void PrintRecord(
    const SortRecord& record,
    ostream& out
) {

    if (record.stockPtr == nullptr) return;

    const stock& s = *record.stockPtr;

    if (record.detailIndex < 0 ||
        record.detailIndex >= s.detail_count)
        return;

    const stockdetail& d = s.detail[record.detailIndex];

    out << "股票代码: " << s.stockcode << '\n';
    out << "股票简称: " << s.indusname << '\n';
    out << "英文名: " << s.sname << '\n';
    out << "行业编码: " << s.briname << '\n';
    out << "一级门类: " << s.pricate << '\n';
    out << "二级门类: " << s.seccate << '\n';
    out << "上市交易所: " << s.listexchange << '\n';
    out << "公司全称: " << s.wholecompany << '\n';
    out << "上市日期: " << s.launchdate << '\n';
    out << "省份: " << s.provin << '\n';
    out << "城市: " << s.city << '\n';
    out << "法人: " << s.legalper << '\n';
    out << "地址: " << s.addr << '\n';
    out << "网址: " << s.url << '\n';
    out << "邮箱: " << s.email << '\n';
    out << "电话: " << s.callnum << '\n';
    out << "主营业务: " << s.mainbus << '\n';
    out << "经营范围: " << s.scopeofbus << '\n';

    out << "日期: " << d.date
        << " 成交量: "
        << fixed << setprecision(3)
        << d.volume << '\n';

    out << "----------------------------\n";
}


/*================ 主函数 ================*/

int main() {

    // ---------- 1. 初始化顺序表 ----------
    SqList L;
    InitList(L);
    SortList SL;
    InitSortList(SL);

    // ---------- 2. 读取股票数据 ----------
    string infoFilename = "stocks.txt";
    string detailDir = "detail";
    ReadFile(L, infoFilename, detailDir);

    // ---------- 3. 从input.txt读取指定日期 ----------
    ifstream fin("input.txt");
    if (!fin.is_open()) {
        FreeSortList(SL);
        FreeList(L);
        return 1;
        
    }
    string targetDate;
    fin >> targetDate;
    fin.close();

    // ---------- 4. 筛选目标日期的股票 ----------
    FilterStocksByDate(L, SL, targetDate);

    // ---------- 5. 直接插入排序 ----------
    int kcn = 0;
    int rmn = 0;
    InsertSort(SL, kcn, rmn);

    // ---------- 6. 打开结果文件 ----------
    ofstream fout("result.txt");
    if (!fout.is_open()) {
        FreeSortList(SL);
        FreeList(L);
        return 1;
    }

    // ---------- 7. 输出排序结果 ----------
    for (int i = 1; i <= SL.length; i++) {
        PrintRecord(SL.elem[i], fout);
    }

    // ---------- 8. 输出排序统计 ----------
    fout << "总的关键字比较次数KCN为：" << kcn << endl;
    fout << "总的记录移动次数RMN为：" << rmn << endl;

    // ---------- 9. 释放资源 ----------
    fout.close();
    FreeSortList(SL);
    FreeList(L);

    return 0;
}
