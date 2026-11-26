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

  stockdetail *detail = nullptr;
  int detail_count = 0;
} stock;

typedef struct {
  stock *elem; // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
  int length;
} SqList;

// ==================== 初始化顺序表 ====================

// 沿用原程序从下标 1 开始存储的方式
void InitList(SqList &L) {
  L.elem = new stock[MAXSIZE + 1];
  L.length = 0;
}

// ==================== 释放顺序表 ====================

void FreeList(SqList &L) {
  if (L.elem != nullptr) {
    for (int i = 1; i <= L.length; i++) {
      delete[] L.elem[i].detail;
      L.elem[i].detail = nullptr;
    }

    delete[] L.elem;
  }

  L.elem = nullptr;
  L.length = 0;
}

// ==================== 读取股票每日价格 ====================

void ReadDetail(stock &s, string detailDir) {
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

    // 原文件使用分号分隔
    for (char &c : line) {
      if (c == ';') {
        c = ' ';
      }
    }

    stockdetail d{};
    stringstream ss(line);

    if (ss >> d.date >> d.openprice >> d.closeprice >> d.highest >> d.lowest >>
        d.volume >> d.turnover >> d.changerate >> d.risefallrate >>
        d.risefallamount) {
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

void ReadFile(SqList &L, string infoFilename, string detailDir) {
  ifstream infile(infoFilename);

  if (!infile.is_open()) {
    return;
  }

  string segment;
  bool flag = false;

  while (getline(infile, segment, '#')) {
    stock temp;
    stringstream streams(segment);
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

// ==================== 保存股票基本信息 ====================

void SaveFile(SqList &L, string filename) {
  ofstream outfile(filename);

  if (!outfile.is_open()) {
    return;
  }

  for (int i = 1; i <= L.length; i++) {
    const stock &s = L.elem[i];

    if (i > 1) {
      outfile << "#\n";
    }

    outfile << "CODE:" << s.stockcode << "\n";
    outfile << "股票简称:" << s.indusname << "\n";
    outfile << "英文名:" << s.sname << "\n";
    outfile << "机构名称:" << s.wholecompany << "\n";
    outfile << "上市交易所:" << s.listexchange << "\n";
    outfile << "行业编码:" << s.briname << "\n";
    outfile << "行业大类:" << s.pricate << "\n";
    outfile << "行业二级类:" << s.seccate << "\n";
    outfile << "上市日期:" << s.launchdate << "\n";
    outfile << "省份:" << s.provin << "\n";
    outfile << "城市:" << s.city << "\n";
    outfile << "法人:" << s.legalper << "\n";
    outfile << "地址:" << s.addr << "\n";
    outfile << "网址:" << s.url << "\n";
    outfile << "邮箱:" << s.email << "\n";
    outfile << "电话:" << s.callnum << "\n";
    outfile << "主营业务:" << s.mainbus << "\n";
    outfile << "经营范围:" << s.scopeofbus << "\n";
  }

  outfile.close();
}

// ==================== 学生需要补全的函数 ====================

// 根据股票简称查找股票，修改公司全称，
// 返回包含修改后信息的新 stock 对象。
stock *ModifyStock(SqList &L, string indusname, string newcompany);

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
 *     stock* elem;   // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
 *     int length;
 * } SqList;
 */

/**
 * 按简称找到股票，把它的公司全称改为 newcompany，并返回修改后的股票对象。
 *
 * @param L            股票顺序表（输出参数：对应记录的 wholecompany 会被修改）
 * @param indusname    待修改股票的简称
 * @param newcompany   修改后的公司全称
 * @return 成功返回一个 new 出来的 stock*；失败返回 nullptr。
 */
stock *ModifyStock(SqList &L, string indusname, string newcompany) {
  for (int i = 1; i <= L.length; i++) {
    if (L.elem[i].indusname == indusname) {
      L.elem[i].wholecompany = newcompany;
      return new stock(L.elem[i]);
    }
  }
  return nullptr;
}

// ==================== 检查修改结果 ====================

bool CheckStock(SqList &L, string indusname, string newcompany) {
  for (int i = 1; i <= L.length; i++) {
    if (L.elem[i].indusname == indusname) {
      return L.elem[i].wholecompany == newcompany;
    }
  }

  return false;
}

// ==================== 输出股票信息 ====================

// 将原来的 cout 改为 ostream
void Print(stock *s, ostream &out) {
  if (s == nullptr) {
    return;
  }

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
    const stockdetail &d = s->detail[i];

    out << "日期: " << d.date << fixed << setprecision(3)
        << " 开盘价: " << d.openprice << " 收盘价: " << d.closeprice
        << " 最高价: " << d.highest << " 最低价: " << d.lowest
        << " 成交量: " << d.volume << " 成交额: " << d.turnover
        << " 换手率: " << d.changerate << " 涨跌幅: " << d.risefallrate
        << " 涨跌额: " << d.risefallamount << endl;
  }
}

// ==================== 主函数 ====================

int main() {
  SqList L;
  InitList(L);

  // 保留原有数据文件名称
  string originInfoFilename = "stocks.txt";
  string newInfoFilename = "new_stocks.txt";
  string detailDir = "detail";

  // 1. 读取原始股票信息
  ReadFile(L, originInfoFilename, detailDir);

  // 2. 从 input.txt 读取股票简称和新公司名称
  ifstream fin("input.txt");
  string indusname, newcompany;
  if (!fin.is_open() || !(fin >> indusname >> newcompany)) {
    FreeList(L);
    return 1;
  }
  fin.close();

  // 3. 打开输出文件
  ofstream fout("result.txt");
  if (!fout.is_open()) {
    FreeList(L);
    return 1;
  }

  // 4. 调用学生修改函数
  stock *s = ModifyStock(L, indusname, newcompany);

  if (s != nullptr) {
    Print(s, fout);
    delete s;
  } else {
    fout << "修改失败" << endl;
  }

  fout.close();
  FreeList(L);

  return 0;
}
