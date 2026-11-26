
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

// 日期-成交量记录
typedef struct DailyVolume {
    string date;             // 交易日期
    double volume;           // 当日行业总成交量
} DailyVolume;

// 股票顺序表
typedef struct {
    stock* elem = nullptr;   // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
    int length = 0;
} SqList;

// 日期-成交量顺序表
typedef struct {
    DailyVolume* elem = nullptr;   // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
    int length = 0;
} SqListDailyVolume;

/*================ 初始化股票顺序表 ================*/

void InitList(SqList& L) {
    // 使用下标1~MAXSIZE
    L.elem = new stock[MAXSIZE + 1];
    L.length = 0;
}

/*================ 释放股票顺序表 ================*/

void FreeList(SqList& L) {
    if (L.elem == nullptr) return;

    // 先释放每支股票的每日交易详情数组
    for (int i = 1; i <= L.length; i++) {
        delete[] L.elem[i].detail;
        L.elem[i].detail = nullptr;
    }

    delete[] L.elem;
    L.elem = nullptr;
    L.length = 0;
}

/*================ 初始化日期-成交量顺序表 ================*/

void InitDailyVolumeList(SqListDailyVolume& DL) {
    // 使用下标1~MAXSIZE
    DL.elem = new DailyVolume[MAXSIZE + 1];
    DL.length = 0;
}

/*================ 释放日期-成交量顺序表 ================*/

void FreeDailyVolume(SqListDailyVolume& DL) {
    delete[] DL.elem;
    DL.elem = nullptr;
    DL.length = 0;
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

        // 原始数据以分号分隔
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

    char segmentBuffer[5000];
    bool isFirstSegment = false;

    // 股票记录使用#分隔
    while (infile.getline(
        segmentBuffer, sizeof(segmentBuffer), '#')) {

        stock temp;
        stringstream segmentStream(segmentBuffer);
        string lineContent;
        int fieldType = 0;

        if (isFirstSegment) {
            getline(segmentStream, lineContent, '\n');
        }

        isFirstSegment = true;

        while (getline(segmentStream, lineContent, '\n')) {

            if (!lineContent.empty() &&
                lineContent.back() == '\r') {
                lineContent.pop_back();
            }

            switch (fieldType) {

                case 0:
                    temp.stockcode = lineContent.substr(5);
                    break;

                case 1:
                    temp.indusname = lineContent.substr(13);
                    break;

                case 2:
                    temp.sname = lineContent.substr(10);
                    break;

                case 3:
                    temp.wholecompany = lineContent.substr(13);
                    break;

                case 4:
                    temp.listexchange = lineContent.substr(16);
                    break;

                case 5:
                    temp.briname = lineContent.substr(13);
                    break;

                case 6:
                    temp.pricate = lineContent.substr(13);
                    break;

                case 7:
                    temp.seccate = lineContent.substr(16);
                    break;

                case 8:
                    temp.launchdate = lineContent.substr(13);
                    break;

                case 9:
                    temp.provin = lineContent.substr(7);
                    break;

                case 10:
                    temp.city = lineContent.substr(7);
                    break;

                case 11:
                    temp.legalper = lineContent.substr(7);
                    break;

                case 12:
                    temp.addr = lineContent.substr(7);
                    break;

                case 13:
                    temp.url = lineContent.substr(7);
                    break;

                case 14:
                    temp.email = lineContent.substr(7);
                    break;

                case 15:
                    temp.callnum = lineContent.substr(7);
                    break;

                case 16:
                    temp.mainbus = lineContent.substr(13);
                    break;

                case 17:
                    temp.scopeofbus = lineContent.substr(13);
                    break;

                default:
                    break;
            }

            fieldType++;
        }

        // 跳过无效股票记录
        if (temp.stockcode.empty()) continue;

        // 防止顺序表越界
        if (L.length >= MAXSIZE) break;

        ReadDetail(temp, detailDir);

        // 按原始顺序存入股票顺序表
        L.elem[++L.length] = temp;
    }

    infile.close();
}

/*================ 学生函数声明 ================*/

// 将两个有序子表归并为一个有序子表
void Merge( SqListDailyVolume& DL, DailyVolume* tempArray,
    int left, int mid, int right, int& kcn, int& rmn);

// 递归归并排序
void MergeSort( SqListDailyVolume& DL, DailyVolume* tempArray,
    int left, int right, int& kcn, int& rmn);

// 按行业统计每日成交量并排序
void MergeSort(SqListDailyVolume& DL, int& kcn, int& rmn);

void industryDailyVolume( SqList& L, SqListDailyVolume& DL,
    string briname, int& kcn, int& rmn);


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
 * typedef struct DailyVolume {
 *     string date;             // 交易日期
 *     double volume;           // 当日行业总成交量
 * } DailyVolume;
 *
 * typedef struct {
 *     stock* elem = nullptr;   // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
 *     int length = 0;
 * } SqList;
 *
 * typedef struct {
 *     DailyVolume* elem = nullptr;   // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
 *     int length = 0;
 * } SqListDailyVolume;
 */

/**
 * 把 DL[left..mid] 和 DL[mid+1..right] 两个有序子表归并成一个有序表，并统计比较和移动次数。
 *
 * @param DL           （输出参数）存放待归并数据的顺序表，归并后该区间有序
 * @param tempArray    临时数组，用于辅助归并
 * @param left         左子表起始下标
 * @param mid          左子表结束下标
 * @param right        右子表结束下标
 * @param kcn          （输出参数）比较次数
 * @param rmn          （输出参数）移动次数
 */
void Merge(
    SqListDailyVolume& DL,
    DailyVolume* tempArray,
    int left,
    int mid,
    int right,
    int& kcn,
    int& rmn
) {


}

/**
 * 对 DL[left..right] 递归进行归并排序（按成交量升序）。
 *
 * @param DL           （输出参数）待排序的顺序表
 * @param tempArray    临时数组
 * @param left         区间左端
 * @param right        区间右端
 * @param kcn          （输出参数）比较次数
 * @param rmn          （输出参数）移动次数
 */
void MergeSort(
    SqListDailyVolume& DL,
    DailyVolume* tempArray,
    int left,
    int right,
    int& kcn,
    int& rmn
) {

}

/**
 * 汇总指定行业所有股票的每日成交量，按日期累加后按成交量升序排序，结果写入 DL。
 *
 * @param L            股票顺序表
 * @param DL           （输出参数）汇总并排序后的日期-成交量表
 * @param briname      行业编码
 * @param kcn          （输出参数）比较次数
 * @param rmn          （输出参数）移动次数
 */
void industryDailyVolume(
    SqList& L,
    SqListDailyVolume& DL,
    string briname,
    int& kcn,
    int& rmn
) {

   
}


/*================ 归并排序入口函数 ================*/

// 系统提供临时数组的创建和释放
void MergeSort(
    SqListDailyVolume& DL,
    int& kcn,
    int& rmn
) {

    if (DL.length <= 1) return;

    DailyVolume* tempArray = new DailyVolume[MAXSIZE + 1];

    // 调用学生实现的递归归并排序
    MergeSort( DL, tempArray, 1, DL.length, kcn, rmn);

    delete[] tempArray;
}


/*================ 主函数 ================*/

int main() {

    // ---------- 1. 初始化股票顺序表 ----------
    SqList L;
    InitList(L);

    // ---------- 2. 初始化日期-成交量顺序表 ----------
    SqListDailyVolume DL;
    InitDailyVolumeList(DL);

    // ---------- 3. 读取股票数据 ----------
    string infoFile = "stocks.txt";
    string detailDir ="detail";
    ReadFile(L, infoFile, detailDir);

    // ---------- 4. 从input.txt读取行业编码 ----------
    ifstream fin("input.txt");
    if (!fin.is_open()) {
        FreeDailyVolume(DL);
        FreeList(L);
        return 1;
    }
    string briname;
    fin >> briname;
    fin.close();

    // ---------- 5. 统计并排序行业每日成交量 ----------
    int kcn = 0;
    int rmn = 0;
    industryDailyVolume(
        L, DL, briname, kcn, rmn
    );

    // ---------- 6. 打开结果文件 ----------
    ofstream fout("result.txt");
    if (!fout.is_open()) {
        FreeDailyVolume(DL);
        FreeList(L);
        return 1;
    }

    // ---------- 7. 输出排序统计 ----------
    fout << "总的关键字比较次数KCN为：" << kcn << endl;
    fout << "总的记录移动次数RMN为：" << rmn << endl;

    // ---------- 8. 输出排序后的日期和成交量 ----------
    for (int i = 1; i <= DL.length; i++) {
        fout << DL.elem[i].date << " " << fixed << setprecision(3)
             << DL.elem[i].volume << endl;
    }

    // ---------- 9. 释放资源 ----------
    fout.close();

    FreeDailyVolume(DL);
    FreeList(L);

    return 0;
}
