
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

// ==================== 初始化顺序表 ====================

void InitList(SqList& L) {
    // elem[0] 闲置，实际使用下标 1 ~ MAXSIZE
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

    delete[] L.elem;
    L.elem = nullptr;
    L.length = 0;
}

// ==================== 读取股票每日价格信息 ====================

void ReadDetail(stock& s, string detailDir) {

    // 构建每日价格文件路径
    string filename = detailDir + "/" + s.stockcode + ".txt";

    ifstream file(filename);

    s.detail = nullptr;
    s.detail_count = 0;

    if (!file.is_open()) {
        return;
    }

    // 统计非空行数量
    int count = 0;
    string line;

    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (!line.empty()) {
            count++;
        }
    }

    s.detail_count = count;

    if (count == 0) {
        file.close();
        return;
    }

    // 为每日价格记录分配空间
    s.detail = new stockdetail[count];

    // 重新定位到文件开头
    file.clear();
    file.seekg(0);

    int index = 0;

    while (getline(file, line)) {

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.empty()) {
            continue;
        }

        // 将原文件中的分号替换为空格
        for (char& c : line) {
            if (c == ';') {
                c = ' ';
            }
        }

        stockdetail detail{};
        stringstream ss(line);

        // 读取每日价格字段
        ss >> detail.date;
        ss >> detail.openprice;
        ss >> detail.closeprice;
        ss >> detail.highest;
        ss >> detail.lowest;
        ss >> detail.volume;
        ss >> detail.turnover;
        ss >> detail.changerate;
        ss >> detail.risefallrate;
        ss >> detail.risefallamount;

        s.detail[index++] = detail;
    }

    file.close();
}

// ==================== 读取股票基本信息 ====================

void ReadFile(SqList& L, string infoFilename, string detailDir) {

    ifstream infile(infoFilename);

    if (!infile.is_open()) {
        return;
    }

    char s[5000];
    bool flag = false;

    // 原始股票记录使用 # 分隔
    while (infile.getline(s, sizeof(s), '#')) {

        stock temp;
        stringstream streams(s);
        string ss;
        int type = 0;

        // 跳过 # 后面的换行
        if (flag) {
            getline(streams, ss, '\n');
        }

        flag = true;

        while (getline(streams, ss, '\n')) {

            // 去除 Windows 行尾的 '\r'
            if (!ss.empty() && ss.back() == '\r') {
                ss.pop_back();
            }

            switch (type) {
                case 0: // CODE
                    temp.stockcode = ss.substr(5);
                    break;

                case 1: // 股票简称
                    temp.indusname = ss.substr(13);
                    break;

                case 2: // 英文名
                    temp.sname = ss.substr(10);
                    break;

                case 3: // 机构名称
                    temp.wholecompany = ss.substr(13);
                    break;

                case 4: // 上市交易所
                    temp.listexchange = ss.substr(16);
                    break;

                case 5: // 行业编码
                    temp.briname = ss.substr(13);
                    break;

                case 6: // 行业大类
                    temp.pricate = ss.substr(13);
                    break;

                case 7: // 行业二级类
                    temp.seccate = ss.substr(16);
                    break;

                case 8: // 上市日期
                    temp.launchdate = ss.substr(13);
                    break;

                case 9: // 省份
                    temp.provin = ss.substr(7);
                    break;

                case 10: // 城市
                    temp.city = ss.substr(7);
                    break;

                case 11: // 法人
                    temp.legalper = ss.substr(7);
                    break;

                case 12: // 地址
                    temp.addr = ss.substr(7);
                    break;

                case 13: // 网址
                    temp.url = ss.substr(7);
                    break;

                case 14: // 邮箱
                    temp.email = ss.substr(7);
                    break;

                case 15: // 电话
                    temp.callnum = ss.substr(7);
                    break;

                case 16: // 主营业务
                    temp.mainbus = ss.substr(13);
                    break;

                case 17: // 经营范围
                    temp.scopeofbus = ss.substr(13);
                    break;

                default:
                    break;
            }

            type++;
        }

        // 跳过无效记录
        if (temp.stockcode.empty()) {
            continue;
        }

        // 防止顺序表越界
        if (L.length >= MAXSIZE) {
            break;
        }

        // 读取对应的每日价格信息
        ReadDetail(temp, detailDir);

        // 从下标 1 开始存储
        L.elem[++L.length] = temp;
    }

    infile.close();
}

// ==================== 学生函数声明 ====================
// 冒泡排序（按英文名升序）
void BubbleSort(SqList& L);

// 在已排序的顺序表中折半查找股票英文名称
// 找到返回下标，否则返回 -1
int BinarySearch(SqList& L, string sname);

// 计算折半查找成功时的平均查找长度
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
 */

/**
 * 在已按英文名升序排列的顺序表中折半查找 sname。
 *
 * @param L            已排序的股票顺序表
 * @param sname        待查找的股票英文名
 * @return 找到返回位序（从 1 开始）；未找到返回 -1。
 */
int BinarySearch(SqList& L, string sname) {

}

/**
 * 计算折半查找成功时的平均查找长度 ASL。
 *
 * @param L            已排序的股票顺序表
 * @return 返回 ASL。
 */
double GetASL(SqList& L) {
    
}

/**
 * 用冒泡排序按股票英文名把顺序表 L 升序排列。
 *
 * @param L    股票顺序表（输出参数：排序后按英文名升序）
 */
void BubbleSort(SqList& L) {

}


// ==================== 输出股票信息 ====================

// 将原来的 cout 改为 ostream
// 输出内容及顺序与原程序一致
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

    string infoFilename = "stocks.txt";
    string detailDir = "detail";

    // ---------- 1. 读取股票数据 ----------
    ReadFile(L, infoFilename, detailDir);

    // ---------- 2. 冒泡排序 ----------
    BubbleSort(L);

    // ---------- 3. 从 input.txt 读取查询英文名 ----------
    ifstream fin("input.txt");
    if (!fin.is_open()) {
        FreeList(L);
        return 1;
    }
    string sname;
    getline(fin, sname);
    if (!sname.empty() && sname.back() == '\r') {
        sname.pop_back();
    }
    fin.close();

    // ---------- 4. 打开 result.txt ----------
    ofstream fout("result.txt");
    if (!fout.is_open()) {
        FreeList(L);
        return 1;
    }

    // ---------- 5. 折半查找 ----------
    int pos = BinarySearch(L, sname);
    if (pos != -1) {
        // 输出查找到的股票完整信息
        Print(L, pos, fout);
        // 计算并输出平均查找长度
        double ASL = GetASL(L);
        fout << "ASL为：" << fixed << setprecision(2) << ASL << endl;
    } else {
        fout << "查找失败" << endl;
    }

    // ---------- 6. 释放资源 ----------
    fout.close();
    FreeList(L);

    return 0;
}
