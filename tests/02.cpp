#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
using namespace std;
namespace fs = std::filesystem;

#define MAXSIZE 5000

// ==================== 数据结构定义 ====================

struct stockdetail {
  string date;
  double openprice = 0;
  double closeprice = 0;
  double highest = 0;
  double lowest = 0;
  double volume = 0;
  double turnover = 0;
  string changerate;
  string risefallrate;
  double risefallamount = 0;
};

struct stock {
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
};

struct SqList {
  stock *elem = nullptr; // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
  int length = 0;
};

// ==================== 初始化与释放 ====================

// 使用下标 1 ~ MAXSIZE 存储股票
void InitList(SqList &L) {
  L.elem = new stock[MAXSIZE + 1];
  L.length = 0;
}

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

// ==================== 辅助函数 ====================

// 去除 Windows 文本行尾的 '\r'
string stripCR(string s) {
  if (!s.empty() && s.back() == '\r') {
    s.pop_back();
  }
  return s;
}

// 提取冒号后面的字段内容
string afterColon(const string &line) {
  size_t pos = line.find(':');

  if (pos != string::npos) {
    return line.substr(pos + 1);
  }

  pos = line.find("：");

  if (pos != string::npos) {
    return line.substr(pos + 3);
  }

  return line;
}

// ==================== 读取每日行情 ====================

void ReadDetail(stock &s, const string &detailDir) {
  fs::path filename = fs::path(detailDir) / (s.stockcode + ".txt");

  ifstream fin(filename, ios::binary);

  if (!fin.is_open()) {
    s.detail = nullptr;
    s.detail_count = 0;
    return;
  }

  vector<stockdetail> records;
  string line;

  while (getline(fin, line)) {
    line = stripCR(line);

    if (line.empty()) {
      continue;
    }

    // 原文件使用分号分隔字段
    for (char &c : line) {
      if (c == ';') {
        c = ' ';
      }
    }

    stringstream ss(line);
    stockdetail d;

    if (ss >> d.date >> d.openprice >> d.closeprice >> d.highest >> d.lowest >>
        d.volume >> d.turnover >> d.changerate >> d.risefallrate >>
        d.risefallamount) {
      records.push_back(d);
    }
  }

  fin.close();

  s.detail_count = static_cast<int>(records.size());

  if (s.detail_count == 0) {
    s.detail = nullptr;
    return;
  }

  s.detail = new stockdetail[s.detail_count];

  for (int i = 0; i < s.detail_count; i++) {
    s.detail[i] = records[i];
  }
}

// ==================== 读取股票基本信息 ====================

void ReadFile(SqList &L, const string &infoFilename, const string &detailDir) {

  ifstream fin(fs::path(infoFilename), ios::binary);

  if (!fin.is_open()) {
    throw runtime_error("Cannot open stock information file");
  }

  string segment;

  // 每支股票之间使用 # 分隔
  while (getline(fin, segment, '#')) {

    stringstream ss(segment);
    vector<string> fields;
    string line;

    while (getline(ss, line)) {
      line = stripCR(line);

      if (line.empty()) {
        continue;
      }

      fields.push_back(afterColon(line));
    }

    if (fields.empty()) {
      continue;
    }

    if (fields.size() < 18) {
      throw runtime_error("Invalid stock record");
    }

    if (L.length >= MAXSIZE) {
      throw runtime_error("Too many stocks");
    }

    stock s;

    s.stockcode = fields[0];
    s.indusname = fields[1];
    s.sname = fields[2];
    s.wholecompany = fields[3];
    s.listexchange = fields[4];
    s.briname = fields[5];
    s.pricate = fields[6];
    s.seccate = fields[7];
    s.launchdate = fields[8];
    s.provin = fields[9];
    s.city = fields[10];
    s.legalper = fields[11];
    s.addr = fields[12];
    s.url = fields[13];
    s.email = fields[14];
    s.callnum = fields[15];
    s.mainbus = fields[16];
    s.scopeofbus = fields[17];

    // 读取该股票的每日价格信息
    ReadDetail(s, detailDir);

    L.elem[++L.length] = std::move(s);
  }

  fin.close();
}

// ==================== 保存股票基本信息 ====================

void SaveFile(const SqList &L, const string &filename) {

  ofstream fout(fs::path(filename), ios::binary);

  if (!fout.is_open()) {
    throw runtime_error("Cannot write stock information file");
  }

  for (int i = 1; i <= L.length; i++) {

    const stock &s = L.elem[i];

    if (i > 1) {
      fout << "#\n";
    }

    fout << "CODE:" << s.stockcode << "\n";
    fout << "股票简称:" << s.indusname << "\n";
    fout << "英文名:" << s.sname << "\n";
    fout << "机构名称:" << s.wholecompany << "\n";
    fout << "上市交易所:" << s.listexchange << "\n";
    fout << "行业编码:" << s.briname << "\n";
    fout << "行业大类:" << s.pricate << "\n";
    fout << "行业二级类:" << s.seccate << "\n";
    fout << "上市日期:" << s.launchdate << "\n";
    fout << "省份:" << s.provin << "\n";
    fout << "城市:" << s.city << "\n";
    fout << "法人:" << s.legalper << "\n";
    fout << "地址:" << s.addr << "\n";
    fout << "网址:" << s.url << "\n";
    fout << "邮箱:" << s.email << "\n";
    fout << "电话:" << s.callnum << "\n";
    fout << "主营业务:" << s.mainbus << "\n";
    fout << "经营范围:" << s.scopeofbus << "\n";
  }

  fout.close();
}

// ==================== 保存每日行情 ====================

void SaveDetail(const SqList &L, const string &detailDir) {

  fs::create_directories(fs::path(detailDir));

  for (int i = 1; i <= L.length; i++) {

    const stock &s = L.elem[i];

    fs::path filename = fs::path(detailDir) / (s.stockcode + ".txt");

    ofstream fout(filename, ios::binary);

    if (!fout.is_open()) {
      throw runtime_error("Cannot write stock detail file");
    }

    fout << fixed << setprecision(3);

    for (int j = 0; j < s.detail_count; j++) {

      const stockdetail &d = s.detail[j];

      fout << d.date << ";" << d.openprice << ";" << d.closeprice << ";"
           << d.highest << ";" << d.lowest << ";" << d.volume << ";"
           << d.turnover << ";" << d.changerate << ";" << d.risefallrate << ";"
           << d.risefallamount;

      if (j < s.detail_count - 1) {
        fout << "\n";
      }
    }

    fout.close();
  }
}

// ==================== 学生函数声明 ====================
// 根据股票简称删除股票
// 删除成功：返回被删除股票的动态对象
// 删除失败：返回 nullptr
stock *DeleteStock(SqList &L, string indusname);

/**
 *
 * struct stockdetail {
 *     string date;
 *     double openprice = 0;
 *     double closeprice = 0;
 *     double highest = 0;
 *     double lowest = 0;
 *     double volume = 0;
 *     double turnover = 0;
 *     string changerate;
 *     string risefallrate;
 *     double risefallamount = 0;
 * };
 *
 * struct stock {
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
 * };
 *
 * struct SqList {
 *     stock* elem = nullptr;   // elem[1] ~ elem[length] 为有效数据，elem[0]
 * 不用 int length = 0;
 * };
 */

/**
 * 按股票简称在顺序表中查找并删除该股票。
 *
 * @param L            股票顺序表（输出参数：删除后其余元素前移，length 减 1）
 * @param indusname    待删除股票的简称
 * @return 成功返回新分配的 stock*（其中 detail 是新拷贝，调用方需释放 detail
 * 和对象本身）；失败返回 nullptr。
 */
stock *DeleteStock(SqList &L, string indusname) {
  for (int i = 1; i <= L.length; i++) {
    if (L.elem[i].indusname == indusname) {
      stock *temp = new stock(L.elem[i]);
      for (int j = i; j < L.length; j++) {
        L.elem[j] = L.elem[j + 1];
      }
      L.length--;

      return temp;
    }
  }
  return nullptr;
}

// ==================== 检查删除结果 ====================

bool CheckStock(const SqList &L, const string &indusname) {

  for (int i = 1; i <= L.length; i++) {
    if (L.elem[i].indusname == indusname) {
      return false;
    }
  }

  return true;
}

// ==================== 输出被删除的股票 ====================

// 输出到文件流，不使用 cout
void Print(const stock *s, ostream &out) {

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

  out << fixed << setprecision(3);

  for (int i = 0; i < s->detail_count; i++) {

    const stockdetail &d = s->detail[i];

    out << "日期: " << d.date << " 开盘价: " << d.openprice
        << " 收盘价: " << d.closeprice << " 最高价: " << d.highest
        << " 最低价: " << d.lowest << " 成交量: " << d.volume
        << " 成交额: " << d.turnover << " 换手率: " << d.changerate
        << " 涨跌幅: " << d.risefallrate << " 涨跌额: " << d.risefallamount
        << endl;
  }
}

// ==================== 主函数 ====================

int main() {

  // 保留原实验的数据文件路径
  const string originInfoFilename = "stocks.txt";
  const string newInfoFilename = "new_stocks.txt";
  const string detailDir = "detail";
  const string newDetailDir = "new_detail";

  SqList L;
  InitList(L);

  try {
    // 1. 从文件读取待删除的股票简称
    ifstream fin("input.txt");
    if (!fin.is_open()) {
      throw runtime_error("Cannot open input.txt");
    }
    string indusname;
    getline(fin, indusname);
    indusname = stripCR(indusname);
    fin.close();

    // 2. 打开结果文件
    ofstream fout("result.txt");
    if (!fout.is_open()) {
      throw runtime_error("Cannot open result.txt");
    }

    // 3. 读取原始股票信息
    ReadFile(L, originInfoFilename, detailDir);

    // 4. 调用学生实现的删除函数
    stock *deleted = DeleteStock(L, indusname);

    if (deleted != nullptr) {
      Print(deleted, fout);
      delete[] deleted->detail;
      delete deleted;
    } else {
      fout << "删除失败" << endl;
    }

    fout.close();

  } catch (const exception &e) {
    cerr << e.what() << endl;
    FreeList(L);
    return 1;
  }

  FreeList(L);
  return 0;
}
