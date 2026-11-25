#include <bits/stdc++.h>
#define MAXSIZE 5000
using namespace std;

// ==================== 数据结构定义 ====================

// 股票每日价格信息
typedef struct stockdetail {
  string date;           // 日期
  double openprice;      // 开盘价
  double closeprice;     // 收盘价
  double highest;        // 最高价
  double lowest;         // 最低价
  double volume;         // 成交量
  double turnover;       // 成交额
  string changerate;     // 换手率
  string risefallrate;   // 涨跌幅
  double risefallamount; // 涨跌额
} stockdetail;

// 股票基本信息
typedef struct stock {
  string stockcode;    // 股票代码
  string indusname;    // 股票简称
  string sname;        // 英文名
  string briname;      // 行业编码
  string pricate;      // 一级门类
  string seccate;      // 二级门类
  string listexchange; // 上市交易所
  string wholecompany; // 公司全称
  string launchdate;   // 上市日期
  string provin;       // 省份
  string city;         // 城市
  string legalper;     // 法人
  string addr;         // 地址
  string url;          // 网址
  string email;        // 邮箱
  string callnum;      // 电话
  string mainbus;      // 主营业务
  string scopeofbus;   // 经营范围

  stockdetail *detail = nullptr; // 每日价格信息数组
  int detail_count = 0;          // 每日价格记录数量
} stock;

// 顺序表
typedef struct SqList {
  stock *elem =
      nullptr;    // 股票数组，elem[1] ~ elem[length] 为有效数据，elem[0] 不用
  int length = 0; // 股票数量
} SqList;

// ==================== 函数声明 ====================

void InitList(SqList &L);
void ReadDetail(stock &s, string detailDir);
void ReadFile(SqList &L, string infoFilename, string detailDir);

// 去掉行尾的 '\r' / '\n'（detail 文件是 CRLF 换行）
static void StripCR(string &s) {
  while (!s.empty() && (s.back() == '\r' || s.back() == '\n')) {
    s.pop_back();
  }
}

// 把一个字段（字段名, 字段值）写入 stock 的对应成员
static void SetField(stock &s, const string &key, const string &val) {
  if (key == "CODE") {
    s.stockcode = val;
  } else if (key == "股票简称") {
    s.indusname = val;
  } else if (key == "英文名") {
    s.sname = val;
  } else if (key == "机构名称") {
    s.wholecompany = val;
  } else if (key == "上市交易所") {
    s.listexchange = val;
  } else if (key == "行业编码") {
    s.briname = val;
  } else if (key == "行业大类") {
    s.pricate = val;
  } else if (key == "行业二级类") {
    s.seccate = val;
  } else if (key == "上市日期") {
    s.launchdate = val;
  } else if (key == "省份") {
    s.provin = val;
  } else if (key == "城市") {
    s.city = val;
  } else if (key == "法人") {
    s.legalper = val;
  } else if (key == "地址") {
    s.addr = val;
  } else if (key == "网址") {
    s.url = val;
  } else if (key == "邮箱") {
    s.email = val;
  } else if (key == "电话") {
    s.callnum = val;
  } else if (key == "主营业务") {
    s.mainbus = val;
  } else if (key == "经营范围") {
    s.scopeofbus = val;
  }
}

/**
 * 初始化顺序表：为 elem 分配数组并把 length 置 0。
 *
 * @param L            （输出参数）待初始化的顺序表
 */
void InitList(SqList &L) {
  // elem[0] 不使用，最多可存 MAXSIZE-1 条记录（本数据 300 条）
  L.elem = new stock[MAXSIZE];
  L.length = 0;
}

/**
 * 读取单支股票的每日行情，填入 s.detail 和 s.detail_count。
 *
 * @param s            （输出参数）要填充的股票记录
 * @param detailDir    每日行情文件所在目录（detail）
 */
void ReadDetail(stock &s, string detailDir) {
  s.detail = nullptr;
  s.detail_count = 0;

  // 文件名 = 目录 + "/" + 股票代码 + ".txt"，如 detail/cn_688018.txt
  ifstream fin(detailDir + "/" + s.stockcode + ".txt");
  if (!fin.is_open()) { // 文件不存在：detail_count 保持 0
    return;
  }

  vector<stockdetail> buf;
  string line;
  while (getline(fin, line)) {
    StripCR(line);
    if (line.empty()) { // 跳过空行（含文件末尾的空行）
      continue;
    }

    // 一行 10 个字段，用半角分号 ';' 分隔
    vector<string> f;
    string item;
    stringstream ss(line);
    while (getline(ss, item, ';')) {
      f.push_back(item);
    }
    if (f.size() < 10) { // 字段不足，跳过该残缺行
      continue;
    }

    stockdetail d;
    d.date = f[0];
    d.openprice = stod(f[1]);
    d.closeprice = stod(f[2]);
    d.highest = stod(f[3]);
    d.lowest = stod(f[4]);
    d.volume = stod(f[5]);
    d.turnover = stod(f[6]);
    d.changerate = f[7];
    d.risefallrate = f[8];
    d.risefallamount = stod(f[9]);
    buf.push_back(d);
  }

  s.detail_count = (int)buf.size();
  if (s.detail_count > 0) {
    s.detail = new stockdetail[s.detail_count];
    for (int i = 0; i < s.detail_count; i++) {
      s.detail[i] = buf[i];
    }
  }
}

/**
 * 从股票基本信息文件读取全部记录及每日行情，按顺序存入顺序表 L。
 *
 * @param L            （输出参数）保存结果的顺序表
 * @param infoFilename 股票基本信息文件名（stocks.txt）
 * @param detailDir    每日行情目录（detail）
 */
void ReadFile(SqList &L, string infoFilename, string detailDir) {
  ifstream fin(infoFilename);
  if (!fin.is_open()) {
    return;
  }

  stock cur; // 当前正在拼装的一条股票记录
  bool hasData = false;
  string line;

  while (getline(fin, line)) {
    StripCR(line);

    if (line == "#") { // 单行 '#' 表示一条记录结束
      if (hasData && L.length + 1 < MAXSIZE) {
        ReadDetail(cur, detailDir); // 基本信息读完后，再读该股票的行情
        L.elem[L.length + 1] = cur; // 1-based：存入 elem[length+1]
        L.length++;
      }
      cur = stock();
      hasData = false;
      continue;
    }

    if (line.empty()) { // 跳过空行
      continue;
    }

    // 只在第一个半角冒号处切分：值里可能还有冒号（如“经营范围:一般项目:...”）
    size_t pos = line.find(':');
    if (pos == string::npos) {
      continue;
    }
    string key = line.substr(0, pos);
    string val = line.substr(pos + 1); // 值中的空格、冒号原样保留
    SetField(cur, key, val);
    hasData = true;
  }

  // 最后一条记录后面没有 '#'
  if (hasData && L.length + 1 < MAXSIZE) {
    ReadDetail(cur, detailDir);
    L.elem[L.length + 1] = cur;
    L.length++;
  }
}

/**
 * 输出顺序表中第 pos 支股票的全部信息。价格、成交量等浮点数保留 3 位小数。
 *
 *   日期: 20251202 开盘价: 9.190 ... 换手率: 0.95 涨跌幅: -1.19 涨跌额: -0.110
 * 其中换手率、涨跌幅在结构体中本就是字符串，按原样输出；其余浮点数保留 3
 * 位小数。
 *
 * @param L            股票顺序表
 * @param pos          股票位置（从 1 开始）
 * @param out          （输出参数）结果输出流
 */
void Print(const SqList &L, int pos, ostream &out) {
  if (pos < 1 || pos > L.length) { // 位置非法
    return;
  }
  const stock &s = L.elem[pos]; // 1-based：第 pos 支即 elem[pos]

  // 基本信息
  out << "股票代码: " << s.stockcode << '\n';
  out << "股票简称: " << s.indusname << '\n';
  out << "英文名: " << s.sname << '\n';
  out << "行业编码: " << s.briname << '\n';
  out << "一级门类: " << s.pricate << '\n'; // 行业大类
  out << "二级门类: " << s.seccate << '\n'; // 行业二级类
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

  // 每日行情：detail_count 为 0 时不输出任何行情行
  out << fixed << setprecision(3);
  for (int i = 0; i < s.detail_count; i++) {
    const stockdetail &d = s.detail[i];
    out << "日期: " << d.date << " 开盘价: " << d.openprice
        << " 收盘价: " << d.closeprice << " 最高价: " << d.highest
        << " 最低价: " << d.lowest << " 成交量: " << d.volume
        << " 成交额: " << d.turnover
        << " 换手率: " << d.changerate   // string：原样输出，如 0.95
        << " 涨跌幅: " << d.risefallrate // string：原样输出，如 -1.19
        << " 涨跌额: " << d.risefallamount << '\n';
  }
}

// ==================== 释放顺序表 ====================

void FreeList(SqList &L) {

  if (L.elem == nullptr) {
    L.length = 0;
    return;
  }

  // 释放每支股票的每日价格数组
  for (int i = 0; i < L.length; i++) {
    delete[] L.elem[i].detail;
    L.elem[i].detail = nullptr;
    L.elem[i].detail_count = 0;
  }

  // 释放股票数组
  delete[] L.elem;
  L.elem = nullptr;
  L.length = 0;
}

// ==================== 文件输出股票信息 ====================

// out 为输出流，可传入任意 ofstream

// ==================== 主函数 ====================

int main() {
  SqList L;
  InitList(L);

  // 原始股票数据文件及详情目录
  string infoFilename = "stocks.txt";
  string detailDir = "detail";

  // 从文件读取查询位置
  ifstream fin("input.txt");
  int pos;
  if (!fin.is_open() || !(fin >> pos)) {
    FreeList(L);
    return 1;
  }
  fin.close();

  // 读取股票数据
  ReadFile(L, infoFilename, detailDir);

  // 将查询结果写入文件
  ofstream fout("result.txt");
  if (!fout.is_open()) {
    FreeList(L);
    return 1;
  }

  // 输出股票信息
  Print(L, pos, fout);

  fout.close();
  FreeList(L);

  return 0;
}
