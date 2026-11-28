
#include <bits/stdc++.h>
#include <cstddef>
#define MAXSIZE 5000
#define m 3
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

typedef struct Sqlist {
  stock *elem = nullptr; // elem[1] ~ elem[length] 为有效数据，elem[0] 不用
  int length = 0;
} SqList;

// 3阶B-树结点
typedef struct BTNode {
  int keynum;                // 关键字个数（1 ≤ keynum ≤ m-1）
  struct BTNode *parent;     // 双亲指针
  string K[m + 1];           // K[1..keynum] 有效
  struct BTNode *ptr[m + 1]; // ptr[0..keynum] 有效
  stock *recptr[m + 1];      // recptr[1..keynum] 有效，与 K 一一对应
} BTNode, *BTree;

// B-树查找结果
typedef struct {
  BTNode *pt; // 成功：关键字所在结点；失败：应插入的叶结点
  int i;   // 成功：1~pt->keynum（关键字序号）；失败：1~pt->keynum+1（插入位置）
  int tag; // 1 查找成功，0 查找失败
} Result;

// ==================== 初始化顺序表 ====================

void InitList(SqList &L) {
  // 下标0闲置，实际使用1~MAXSIZE
  L.elem = new stock[MAXSIZE + 1];
  L.length = 0;
}

// ==================== 释放顺序表 ====================

void FreeList(SqList &L) {
  if (L.elem == nullptr)
    return;

  for (int i = 1; i <= L.length; i++) {
    delete[] L.elem[i].detail;
    L.elem[i].detail = nullptr;
  }

  delete[] L.elem;
  L.elem = nullptr;
  L.length = 0;
}

// ==================== 读取每日行情 ====================

void ReadDetail(stock &s, string detailDir) {
  string filename = detailDir + "/" + s.stockcode + ".txt";
  ifstream file(filename);

  s.detail = nullptr;
  s.detail_count = 0;

  if (!file.is_open())
    return;

  vector<stockdetail> records;
  string line;

  while (getline(file, line)) {
    if (!line.empty() && line.back() == '\r')
      line.pop_back();

    if (line.empty())
      continue;

    // 保留原文件以分号分隔的格式
    for (char &c : line) {
      if (c == ';')
        c = ' ';
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

  if (s.detail_count == 0)
    return;

  s.detail = new stockdetail[s.detail_count];

  for (int i = 0; i < s.detail_count; i++) {
    s.detail[i] = records[i];
  }
}

// ==================== 读取股票基本信息 ====================

void ReadFile(SqList &L, string infoFilename, string detailDir) {
  ifstream infile(infoFilename);

  if (!infile.is_open())
    return;

  string line;
  stock temp;
  bool hasData = false;
  bool firstLine = true;

  // 保存读取完成的一条股票记录
  auto appendStock = [&]() {
    if (!hasData)
      return;

    if (!temp.stockcode.empty() && L.length < MAXSIZE) {
      ReadDetail(temp, detailDir);
      L.elem[++L.length] = temp;
    }

    temp = stock();
    hasData = false;
  };

  while (getline(infile, line)) {

    // 去掉UTF-8 BOM
    if (firstLine) {
      if (line.size() >= 3 && (unsigned char)line[0] == 0xEF &&
          (unsigned char)line[1] == 0xBB && (unsigned char)line[2] == 0xBF) {
        line.erase(0, 3);
      }
      firstLine = false;
    }

    // 兼容Windows换行
    if (!line.empty() && line.back() == '\r')
      line.pop_back();

    // 股票记录之间使用#分隔
    if (line == "#") {
      appendStock();
      continue;
    }

    // 只按照第一个英文冒号分隔字段
    size_t pos = line.find(':');

    if (pos == string::npos)
      continue;

    string key = line.substr(0, pos);
    string value = line.substr(pos + 1);

    if (key == "CODE")
      temp.stockcode = value;
    else if (key == "股票简称")
      temp.indusname = value;
    else if (key == "英文名")
      temp.sname = value;
    else if (key == "机构名称")
      temp.wholecompany = value;
    else if (key == "上市交易所")
      temp.listexchange = value;
    else if (key == "行业编码")
      temp.briname = value;
    else if (key == "行业大类")
      temp.pricate = value;
    else if (key == "行业二级类")
      temp.seccate = value;
    else if (key == "上市日期")
      temp.launchdate = value;
    else if (key == "省份")
      temp.provin = value;
    else if (key == "城市")
      temp.city = value;
    else if (key == "法人")
      temp.legalper = value;
    else if (key == "地址")
      temp.addr = value;
    else if (key == "网址")
      temp.url = value;
    else if (key == "邮箱")
      temp.email = value;
    else if (key == "电话")
      temp.callnum = value;
    else if (key == "主营业务")
      temp.mainbus = value;
    else if (key == "经营范围")
      temp.scopeofbus = value;

    hasData = true;
  }

  // 处理文件末尾没有#的情况
  appendStock();

  infile.close();
}

// ==================== 创建B-树结点 ====================

BTNode *CreateNode() {
  BTNode *p = new BTNode;

  p->keynum = 0;
  p->parent = nullptr;

  for (int i = 0; i <= m; i++) {
    p->K[i] = "";
    p->ptr[i] = nullptr;
    p->recptr[i] = nullptr;
  }

  return p;
}

void InsertKey(BTNode *q, int i, string x, stock *rec, BTNode *ap);
void SplitNode(BTNode *q, string &x, stock *&rec, BTNode *&ap);
void NewRoot(BTree &T, string x, stock *rec, BTNode *ap);
int CountKeys(BTNode *p);
void CollectDepth(BTNode *p, int depth, long long &sum);

// ==================== 学生函数声明 ====================

// 在B-树中查找指定关键字
Result SearchBTree(BTree T, string key);

// 在B-树中插入股票记录
bool InsertBTree(BTree &T, string key, stock *rec);

// 计算成功查找的平均查找长度
double GetASL(BTree T);

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
 *     stock* elem = nullptr;   // elem[1] ~ elem[length] 为有效数据，elem[0]
 * 不用 int length = 0; } SqList;
 *
 * typedef struct BTNode {
 *     int keynum;                 // 关键字个数（1 ≤ keynum ≤ m-1）
 *     struct BTNode* parent;      // 双亲指针
 *     string K[m + 1];            // K[1..keynum] 有效
 *     struct BTNode* ptr[m + 1];  // ptr[0..keynum] 有效
 *     stock* recptr[m + 1];       // recptr[1..keynum] 有效，与 K 一一对应
 * } BTNode, *BTree;
 *
 * typedef struct {
 *     BTNode* pt;                 // 成功：关键字所在结点；失败：应插入的叶结点
 *     int i;                      //
 * 成功：1~pt->keynum（关键字序号）；失败：1~pt->keynum+1（插入位置） int tag;
 * // 1 查找成功，0 查找失败 } Result;
 */

/**
 * 在 3 阶 B 树中查找关键字 key。
 *
 * @param T            B 树根指针
 * @param key          待查找的股票英文名（关键字）
 * @return 返回 Result：
 *         tag=1：查找成功，pt 为关键字所在结点，i 为该关键字在结点中的序号（1 ~
 * pt->keynum）； tag=0：查找失败，pt 为查找结束时的叶结点，i 为应插入的位置（1
 * ~ pt->keynum + 1）。
 */
Result SearchBTree(BTree T, string key) {
  if (!T) {
    return Result{.pt = nullptr, .i = 0, .tag = 0};
  }
  if (key.empty()) {
    return Result{.pt = nullptr, .i = 0, .tag = 0};
  }

  int i = 0;
  bool isFound = false;
  BTree tempT = T;
  BTNode *nonEmpty = nullptr;

  while (tempT != nullptr && !isFound) {
    i = 1;
    while (i <= tempT->keynum && tempT->K[i] < key) {
      i++;
    }
    if (i <= tempT->keynum && tempT->K[i] == key) {
      isFound = true;
    } else {
      nonEmpty = tempT;
      tempT = tempT->ptr[i - 1];
    }

    if (isFound)
      return Result{.pt = tempT, .i = i, .tag = 1};
  }
  return Result{.pt = nonEmpty, .i = i, .tag = 0};
}
/**
 * 把关键字 key 及其对应的股票记录 rec 插入 3 阶 B 树，必要时分裂结点。
 *
 * @param T            （输出参数）B 树根指针，根分裂时会改变
 * @param key          待插入的关键字（股票英文名）
 * @param rec          该关键字对应的股票记录指针
 * @return 插入成功返回 true；关键字已存在返回 false。
 */
bool InsertBTree(BTree &T, string key, stock *rec) {
  if (key.empty())
    return false;
  if (!rec)
    return false;
  if (T == nullptr) { // 如果树是空树，直接插入进去
    NewRoot(T, key, rec, nullptr);
    return true;
  }

  if (SearchBTree(T, key).tag == 1) {
    return false; // 关键字已经存在，不需要插入，返回false;
  }

  BTNode *q = SearchBTree(T, key).pt;   // 插入的叶子结点
  int position = SearchBTree(T, key).i; // 插入的位置信息
  BTNode *ap = nullptr;

  while (true) {
    InsertKey(q, position, key, rec, ap);
    if (q->keynum < m)
      return true; // 没有上溢出，直接结束;

    SplitNode(q, key, rec, ap);
    BTNode *parent = q->parent;

    if (parent == nullptr) {
      NewRoot(T, key, rec, ap);
      return true;
    }

    int pos = 0;
    while (pos <= parent->keynum && parent->ptr[pos] != q)
      pos++;

    position = pos + 1;
    q = parent;
  }
  return false;
}

/**
 * 计算 B 树成功查找的平均查找长度。
 *
 * @param T            B 树根指针
 * @return 返回 ASL。
 */
double GetASL(BTree T) {
  if (T == nullptr) {
    return 0.0f;
  }

  long long sum = 0;
  CollectDepth(T, 1, sum); // 根记为第1层;

  int n = CountKeys(T);
  if (n == 0)
    return 0.0f;

  return static_cast<double>(sum) / n;
}

// ==================== 结点内部插入关键字 ====================
void InsertKey(BTNode *q, int i, string x, stock *rec, BTNode *ap) {

  // 将第i个位置及其后的关键字和指针后移
  for (int j = q->keynum; j >= i; j--) {
    q->K[j + 1] = q->K[j];
    q->recptr[j + 1] = q->recptr[j];
    q->ptr[j + 1] = q->ptr[j];
  }

  // 插入新关键字和对应记录
  q->K[i] = x;
  q->recptr[i] = rec;
  q->ptr[i] = ap;

  if (ap != nullptr)
    ap->parent = q;

  q->keynum++;
}

// ==================== 分裂B-树结点 ====================
void SplitNode(BTNode *q, string &x, stock *&rec, BTNode *&ap) {

  // 3阶B-树：最多2个关键字
  // 临时达到3个关键字时进行分裂
  int mid = (m + 1) / 2;

  // 创建右兄弟结点
  ap = CreateNode();
  ap->parent = q->parent;

  // 复制右半部分关键字
  for (int j = mid + 1; j <= m; j++) {
    ap->K[j - mid] = q->K[j];
    ap->recptr[j - mid] = q->recptr[j];
  }

  // 复制右半部分子树
  for (int j = mid; j <= m; j++) {
    ap->ptr[j - mid] = q->ptr[j];

    if (ap->ptr[j - mid] != nullptr)
      ap->ptr[j - mid]->parent = ap;
  }

  ap->keynum = m - mid;
  q->keynum = mid - 1;

  // 中间关键字上升到双亲
  x = q->K[mid];
  rec = q->recptr[mid];
}

// ==================== 创建新根结点 ====================
void NewRoot(BTree &T, string x, stock *rec, BTNode *ap) {

  BTNode *root = CreateNode();

  root->keynum = 1;
  root->K[1] = x;
  root->recptr[1] = rec;

  root->ptr[0] = T;
  root->ptr[1] = ap;

  if (T != nullptr)
    T->parent = root;

  if (ap != nullptr)
    ap->parent = root;

  T = root;
}

// ==================== 构建B-树 ====================
// 按股票在原文件中的顺序依次插入
BTree BuildBTree(SqList &L) {
  BTree T = nullptr;
  for (int i = 1; i <= L.length; i++) {
    InsertBTree(T, L.elem[i].sname, &L.elem[i]);
  }
  return T;
}

// ==================== 统计B-树关键字数量 ====================
int CountKeys(BTNode *p) {
  if (p == nullptr)
    return 0;
  int sum = p->keynum;
  for (int i = 0; i <= p->keynum; i++) {
    sum += CountKeys(p->ptr[i]);
  }
  return sum;
}

// ==================== 累计关键字所在层数 ====================

// 根结点位于第1层
void CollectDepth(BTNode *p, int depth, long long &sum) {
  if (p == nullptr)
    return;

  // 同一结点内所有关键字的层数相同
  for (int i = 1; i <= p->keynum; i++) {
    sum += depth;
  }

  // 递归遍历所有子树
  for (int i = 0; i <= p->keynum; i++) {
    CollectDepth(p->ptr[i], depth + 1, sum);
  }
}

// ==================== 输出股票信息 ====================

// 将原来的cout替换为ostream
void Print(stock *s, ostream &out) {
  if (s == nullptr)
    return;

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

// ==================== 释放B-树 ====================

// B-树只保存股票记录指针
// 股票记录由顺序表负责释放
void FreeBTree(BTree &T) {
  if (T == nullptr)
    return;

  for (int i = 0; i <= T->keynum; i++) {
    FreeBTree(T->ptr[i]);
  }

  delete T;
  T = nullptr;
}

int main() {

  // ---------- 1. 初始化顺序表 ----------
  SqList L;
  InitList(L);

  // 保留原程序的文件路径
  string infoFilename = "stocks.txt";
  string detailDir = "detail";

  // ---------- 2. 读取股票数据 ----------
  ReadFile(L, infoFilename, detailDir);

  // ---------- 3. 构建3阶B-树 ----------
  BTree T = BuildBTree(L);

  // ---------- 4. 从input.txt读取股票英文名称 ----------
  ifstream fin("input.txt");
  if (!fin.is_open()) {
    FreeBTree(T);
    FreeList(L);
    return 1;
  }
  string key;
  getline(fin, key);
  if (!key.empty() && key.back() == '\r')
    key.pop_back();
  fin.close();

  // ---------- 5. 打开结果文件 ----------
  ofstream fout("result.txt");
  if (!fout.is_open()) {
    FreeBTree(T);
    FreeList(L);
    return 1;
  }

  // ---------- 6. B-树查找 ----------
  Result r = SearchBTree(T, key);
  if (r.tag == 1) {
    // 输出查找到的股票完整信息
    Print(r.pt->recptr[r.i], fout);
    // 计算成功查找的平均查找长度
    double ASL = GetASL(T);
    fout << "ASL为：" << fixed << setprecision(2) << ASL << endl;
  } else {
    fout << "查找失败" << endl;
  }

  // ---------- 7. 释放资源 ----------
  fout.close();

  FreeBTree(T);
  FreeList(L);

  return 0;
}
