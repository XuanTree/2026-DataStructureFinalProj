
#include <bits/stdc++.h>
#define m 400
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

// 开放地址法散列表
typedef struct {
    stock* key;              // 散列表存储数组
    int length;              // 已存储股票数量
} HashTable;

// ==================== 初始化散列表 ====================

void InitHT(HashTable& HT) {
    HT.key = new stock[m];
    HT.length = 0;
}

// ==================== 释放散列表 ====================

void FreeHT(HashTable& HT) {
    if (HT.key == nullptr) return;

    // 释放每支股票的每日价格数组
    for (int i = 0; i < m; i++) {
        delete[] HT.key[i].detail;
        HT.key[i].detail = nullptr;
    }

    delete[] HT.key;
    HT.key = nullptr;
    HT.length = 0;
}

// ==================== 学生函数声明 ====================

// 根据股票英文名称计算散列地址
int Hash(string sname);

// 使用线性探测法插入股票
void HTInsert(HashTable& HT, stock s);

// 在散列表中查找股票英文名称
int SearchHash(HashTable HT, string key);

// 计算散列查找成功时的平均查找长度
double GetASL(HashTable HT);


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
 *     stock* key;              // 散列表存储数组
 *     int length;              // 已存储股票数量
 * } HashTable;
 */

/**
 * 散列函数：根据股票英文名计算它在散列表中的桶下标。
 *
 * 规则：把英文名第 i 个字符（i 从 0 开始）的 i² × ASCII 码累加，再对 397 取余。
 * 散列表表长 m = 400，除数取小于 400 的最大质数 397。
 *
 * @param sname        股票英文名
 * @return 返回散列地址（0 ~ 396）。
 */
int Hash(string sname) {

}

/**
 * 用线性探测法把股票 s 插入散列表。
 *
 * @param HT           （输出参数）散列表，函数会写入元素
 * @param s            待插入的股票
 */
void HTInsert(HashTable& HT, stock s) {

}

/**
 * 在散列表中用线性探测查找英文名为 key 的股票。
 *
 * @param HT           散列表
 * @param key          待查找的股票英文名
 * @return 找到返回散列表下标；未找到返回 -1。
 */
int SearchHash(HashTable HT, string key) {

}

/**
 * 计算开放地址法散列查找成功时的平均查找长度。
 *
 * 对散列表中每个已存储的股票，从它的散列地址开始用线性探测找到它所在的位置，
 * 统计探测次数；ASL 就是所有股票探测次数之和 / 股票总数。
 *
 * @param HT           散列表
 * @return 返回 ASL；表中没有股票时返回 0。
 */
double GetASL(HashTable HT) {

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

        // 原始文件以分号分隔字段
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

void ReadFile(HashTable& HT,
              string infoFilename, string detailDir) {

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

        // 跳过无效记录
        if (temp.stockcode.empty()) continue;

        // 读取该股票每日价格
        ReadDetail(temp, detailDir);

        // 插入散列表
        // 如果散列表已满，释放未插入记录的内存
        if (HT.length >= m) {
            delete[] temp.detail;
            break;
        }

        HTInsert(HT, temp);
    }

    infile.close();
}


// ==================== 输出股票信息 ====================

void Print(const HashTable& HT, int pos, ostream& out) {

    if (HT.key == nullptr || pos < 0 || pos >= m)
        return;

    const stock& s = HT.key[pos];

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

    // ---------- 1. 初始化散列表 ----------
    HashTable HT;
    InitHT(HT);

    // 保留原始股票数据路径
    string infoFilename = "stocks.txt";
    string detailDir = "detail";

    // ---------- 2. 读取股票并建立散列表 ----------
    ReadFile(HT, infoFilename, detailDir);

    // ---------- 3. 从input.txt读取查询英文名 ----------
    ifstream fin("input.txt");
    if (!fin.is_open()) {
        FreeHT(HT);
        return 1;
    }
    string sname;
    getline(fin, sname);
    if (!sname.empty() && sname.back() == '\r')
        sname.pop_back();
    fin.close();

    // ---------- 4. 打开结果文件 ----------
    ofstream fout("result.txt");
    if (!fout.is_open()) {
        FreeHT(HT);
        return 1;
    }

    // ---------- 5. 散列查找 ----------
    int pos = SearchHash(HT, sname);
    if (pos != -1) {
        // 输出股票信息
        Print(HT, pos, fout);
        // 计算成功查找ASL
        double ASL = GetASL(HT);
        fout << "ASL为：" << fixed << setprecision(2) << ASL << endl;
    } else {
        fout << "查找失败" << endl;
    }

    // ---------- 6. 释放资源 ----------
    fout.close();
    FreeHT(HT);

    return 0;
}
