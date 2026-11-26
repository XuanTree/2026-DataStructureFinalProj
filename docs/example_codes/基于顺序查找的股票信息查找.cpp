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
struct SqList{
    stock* elem = nullptr;   // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
    int length = 0;
} ;

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

    // 释放每支股票的每日价格数组
    for (int i = 1; i <= L.length; i++) {
        delete[] L.elem[i].detail;
        L.elem[i].detail = nullptr;
        L.elem[i].detail_count = 0;
    }

    // 释放股票数组
    delete[] L.elem;
    L.elem = nullptr;
    L.length = 0;
}

void ReadDetail(stock& s, string detailDir) {
    // 构建文件路径
    string filename = detailDir + "/" + s.stockcode + ".txt";
    ifstream file(filename);
    // 先统计行数
    int count = 0;
    string line;
    while (getline(file, line)) {
        if (!line.empty()) count++;
    }
    // 分配内存
    s.detail_count = count;    
    s.detail = new stockdetail[count];
    // 重新读取文件
    file.clear();
    file.seekg(0);
    int index = 0;
    while (getline(file, line)) {
        if (line.empty()) continue;
        // 替换分号为空格
        for (char& c : line) {
            if (c == ';') c = ' ';
        }
        stockdetail detail;
        stringstream segmentStream(line);
        // 解析每个字段
        segmentStream >> detail.date;
        segmentStream >> detail.openprice;
        segmentStream >> detail.closeprice;
        segmentStream >> detail.highest;
        segmentStream >> detail.lowest;
        segmentStream >> detail.volume;
        segmentStream >> detail.turnover;
        segmentStream >> detail.changerate;
        segmentStream >> detail.risefallrate;
        segmentStream >> detail.risefallamount;
        s.detail[index++] = detail;
    }
    file.close();
}

void ReadFile(SqList& L, string infoFilename, string detailDir) {
    // 从文件中读取股票信息，将其按顺序存入L.elem指向的数组中
    ifstream infile(infoFilename);
    char segmentBuffer[5000];  // 存储以'#'分隔的股票信息段
    bool isFirstSegment = false;
    
    while (infile.getline(segmentBuffer, sizeof segmentBuffer, '#')) {
        stock temp;
        stringstream segmentStream(segmentBuffer);
        string lineContent;
        int fieldType = 0;
        
        if (isFirstSegment)
            getline(segmentStream, lineContent, '\n');
        isFirstSegment = true;
        
        while (getline(segmentStream, lineContent, '\n')) {
            if (!lineContent.empty() && lineContent.back() == '\r') lineContent.pop_back();
            switch (fieldType) {
                case 0: // CODE
                    temp.stockcode = lineContent.substr(5);
                    break;
                case 1: // 股票简称
                    temp.indusname = lineContent.substr(13);
                    break;
                case 2: // 英文名
                    temp.sname = lineContent.substr(10);
                    break;
                case 3: // 机构名称
                    temp.wholecompany = lineContent.substr(13);
                    break;
                case 4: // 上市交易所
                    temp.listexchange = lineContent.substr(16);
                    break;
                case 5: // 行业编码
                    temp.briname = lineContent.substr(13);
                    break;
                case 6: // 行业大类
                    temp.pricate = lineContent.substr(13);
                    break;
                case 7: // 行业二级类
                    temp.seccate = lineContent.substr(16);
                    break;
                case 8: // 上市日期
                    temp.launchdate = lineContent.substr(13);
                    break;
                case 9: // 省份
                    temp.provin = lineContent.substr(7);
                    break;
                case 10: // 城市
                    temp.city = lineContent.substr(7);
                    break;
                case 11: // 法人
                    temp.legalper = lineContent.substr(7);
                    break;
                case 12: // 地址
                    temp.addr = lineContent.substr(7);
                    break;
                case 13: // 网址
                    temp.url = lineContent.substr(7);
                    break;
                case 14: // 邮箱
                    temp.email = lineContent.substr(7);
                    break;
                case 15: // 电话
                    temp.callnum = lineContent.substr(7);
                    break;
                case 16: // 主营业务
                    temp.mainbus = lineContent.substr(13);
                    break;
                case 17: // 经营范围
                    temp.scopeofbus = lineContent.substr(13);
                    break; 
                default:
                    break;
            }
            fieldType++;
        }
        ReadDetail(temp, detailDir);
        L.elem[++L.length] = temp;
    }
    infile.close();
}

// ==================== 学生函数声明 ====================

// 顺序查找股票英文名称
// 找到返回下标，否则返回 -1
int SequentialSearch(SqList& L, string sname);

// 计算顺序查找成功时的平均查找长度
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
 * struct SqList{
 *     stock* elem = nullptr;   // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
 *     int length = 0;
 * } ;
 */

/**
 * 在顺序表中从头到尾顺序查找英文名为 sname 的股票。
 *
 * @param L            股票顺序表
 * @param sname        待查找的股票英文名
 * @return 找到返回该股票在表中的位序（从 1 开始）；未找到返回 -1。
 */
int SequentialSearch(SqList& L, string sname) {

}

/**
 * 计算等概率情况下顺序查找成功的平均查找长度 ASL。
 *
 * @param L            股票顺序表
 * @return 返回 ASL = (1+2+...+n)/n。
 */
double GetASL(SqList& L) {

}


// ==================== 输出股票信息 ====================

void Print(const SqList& L, int pos, ostream& out) {

    if (L.elem == nullptr ||
        pos < 1 || pos > L.length) {
        return;
    }

    const stock& s = L.elem[pos];

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

    // 输出每日价格信息
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

    SqList L;
    InitList(L);

    // 保留原程序的股票数据路径
    string infoFilename ="stocks.txt";
    string detailDir ="detail";

    // ---------- 1. 读取股票数据 ----------
    ReadFile(L, infoFilename, detailDir);

    // ---------- 2. 从 input.txt 读取查询英文名 ----------
    ifstream fin("input.txt");
    if (!fin.is_open()) {
        FreeList(L);
        return 1;
    }
    string sname;
    getline(fin, sname);
    // 兼容 Windows 文本文件换行
    if (!sname.empty() && sname.back() == '\r') {
        sname.pop_back();
    }
    fin.close();

    // ---------- 3. 打开 result.txt ----------
    ofstream fout("result.txt");
    if (!fout.is_open()) {
        FreeList(L);
        return 1;
    }

    // ---------- 4. 顺序查找 ----------
    int pos = SequentialSearch(L, sname);
    if (pos != -1) {
        // 输出查找到的股票信息
        Print(L, pos, fout);
        // 计算顺序查找的平均查找长度
        double ASL = GetASL(L);
        fout << "ASL为：" << fixed << setprecision(2) << ASL << endl;
    } else {
        fout << "查找失败" << endl;
    }

    // ---------- 5. 释放资源 ----------
    fout.close();
    FreeList(L);

    return 0;
}
