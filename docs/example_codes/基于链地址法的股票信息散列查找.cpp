#include<bits/stdc++.h>
#define m 400
using namespace std;

typedef struct stockdetail{
	string date;//日期 
	double openprice;//开盘价
	double closeprice;//收盘价
	double highest;//最高价
	double lowest;//最低价
	double volume;//成交量
	double turnover;//成交额
	string changerate;//换手率
	string risefallrate;//涨跌幅
	double risefallamount;//涨跌额
}stockdetail;//股票的价格信息 
typedef struct stock{
	string stockcode;//股票代码
	string indusname;//股票简称
	string sname;//英文名 
	string briname;//行业编码
	string pricate;//一级门类
	string seccate;//二级门类
	string listexchange;//上市交易所
	string wholecompany;//公司全程
	string launchdate;//上市日期
	string provin;//省份
	string city;//城市
	string legalper;//法人
	string addr;//地址
	string url;//网址
	string email;//邮箱
	string callnum;//电话
	string mainbus;//主营业务
	string scopeofbus;//经营范围
	stockdetail *detail;//定义一个stockdetail类型的数组用来存放该股票每一日的相关价格信息  
	int detail_count;//detail数组的长度 
}stock;

typedef struct LNode {
    stock data;                  // 股票信息
    struct LNode* next;         // 指向下一级结点
} LNode, *LinkList;

void InitList(LinkList* H) {
// 链表初始化
    for (int i = 0; i < m; i++) {
        H[i] = new LNode;
        H[i]->next = NULL;
    }
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
        stringstream ss(line);
        
        // 解析每个字段
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

// ==================== 学生函数声明 ====================
int Hash(string sname);
void ListInsert(LinkList* H, stock s);
int SearchHL(LinkList* H, string key);
double GetASL(LinkList* H, int bucketCount);

int ReadFile(LinkList* H, string infoFilename, string detailDir) {
// 从文件中读取中股票信息，将其按顺序存入L.elem指向的数组中
    ifstream infile(infoFilename);
    char s[5000];
    int count = 0;
    bool flag = false;
    while (infile.getline(s, sizeof s, '#')) {
        stock temp;
        stringstream streams(s);
        string ss;
        int type = 0;
        if (flag)
            getline(streams, ss, '\n');
        flag = true;
        while (getline(streams, ss, '\n')) {
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
        ReadDetail(temp,detailDir);
        ListInsert(H, temp);
        count++;
    }
    infile.close();
    return count;
}

void Print(LinkList T, string sname) {
    // 输出股票信息
    while (T) {
        if (T->data.sname==sname) {
		    cout << "股票代码: " << T->data.stockcode << endl;
		    cout << "股票简称: " << T->data.indusname << endl;
		    cout << "英文名: " << T->data.sname << endl;
		    cout << "行业编码: " << T->data.briname << endl;
		    cout << "一级门类: " << T->data.pricate << endl;
		    cout << "二级门类: " << T->data.seccate << endl;
		    cout << "上市交易所: " << T->data.listexchange << endl;
		    cout << "公司全称: " << T->data.wholecompany << endl;
		    cout << "上市日期: " << T->data.launchdate << endl;
		    cout << "省份: " << T->data.provin << endl;
		    cout << "城市: " << T->data.city << endl;
		    cout << "法人: " << T->data.legalper << endl;
		    cout << "地址: " << T->data.addr << endl;
		    cout << "网址: " << T->data.url << endl;
		    cout << "邮箱: " << T->data.email << endl;
		    cout << "电话: " << T->data.callnum << endl;
		    cout << "主营业务: " << T->data.mainbus << endl;
		    cout << "经营范围: " << T->data.scopeofbus << endl;
		    for (int i = 0; i < T->data.detail_count; i++) {
            	stockdetail d = T->data.detail[i];
            	cout << "日期: " << d.date 
                 << fixed << std::setprecision(3) << " 开盘价: " << d.openprice 
                 << fixed << std::setprecision(3) << " 收盘价: " << d.closeprice 
                 << fixed << std::setprecision(3) << " 最高价: " << d.highest 
                 << fixed << std::setprecision(3) << " 最低价: " << d.lowest 
                 << fixed << std::setprecision(3) << " 成交量: " << d.volume 
                 << fixed << std::setprecision(3) << " 成交额: " << d.turnover 
                 << " 换手率: " << d.changerate 
                 << " 涨跌幅: " << d.risefallrate 
                 << fixed << std::setprecision(3) << " 涨跌额: " << d.risefallamount << endl;
	        }
            break;
        }
        T = T->next;
    }
}


/**
 * typedef struct stockdetail{
 * 	string date;//日期
 * 	double openprice;//开盘价
 * 	double closeprice;//收盘价
 * 	double highest;//最高价
 * 	double lowest;//最低价
 * 	double volume;//成交量
 * 	double turnover;//成交额
 * 	string changerate;//换手率
 * 	string risefallrate;//涨跌幅
 * 	double risefallamount;//涨跌额
 * }stockdetail;
 *
 * typedef struct stock{
 * 	string stockcode;//股票代码
 * 	string indusname;//股票简称
 * 	string sname;//英文名
 * 	string briname;//行业编码
 * 	string pricate;//一级门类
 * 	string seccate;//二级门类
 * 	string listexchange;//上市交易所
 * 	string wholecompany;//公司全程
 * 	string launchdate;//上市日期
 * 	string provin;//省份
 * 	string city;//城市
 * 	string legalper;//法人
 * 	string addr;//地址
 * 	string url;//网址
 * 	string email;//邮箱
 * 	string callnum;//电话
 * 	string mainbus;//主营业务
 * 	string scopeofbus;//经营范围
 * 	stockdetail *detail;//定义一个stockdetail类型的数组用来存放该股票每一日的相关价格信息
 * 	int detail_count;//detail数组的长度
 * }stock;
 *
 * typedef struct LNode {
 *     stock data;                  // 股票信息
 *     struct LNode* next;         // 指向下一级结点
 * } LNode, *LinkList;
 */

/**
 * 链地址法的散列函数：根据股票英文名计算它落在哪个桶。
 *
 * 规则：把英文名第 i 个字符（i 从 0 开始）的 i² × ASCII 码累加，再对 397 取余。
 *
 * @param sname        股票英文名
 * @return 返回桶下标（0 ~ 396）。
 */
int Hash(string sname) {

}

/**
 * 把股票 s 插入散列表 H 中 Hash(s.sname) 对应的桶链表。
 *
 * @param H            （输出参数）散列表，H[i] 是第 i 个桶的头结点
 * @param s            待插入的股票
 */
void ListInsert(LinkList* H, stock s) {

}

/**
 * 先计算 key 的桶下标，再在这个桶的链表中查找英文名为 key 的股票。
 *
 * @param H            散列表，H[i] 是第 i 个桶的头结点
 * @param key          待查找的股票英文名
 * @return 找到返回该股票所在的桶下标；未找到返回 -1。
 */
int SearchHL(LinkList* H, string key) {

}

/**
 * 计算链地址法散列查找成功时的平均查找长度。
 *
 * 成功查找一个记录要比较的次数等于它在所在链中的位序（第 1 个比较 1 次，第 2 个比较 2 次，依此类推），
 * ASL 就是所有记录的比较次数之和除以记录总数。
 *
 * @param H            散列表，H[i] 是第 i 个桶的头结点
 * @param bucketCount  散列表的桶数
 * @return 返回 ASL；表中没有记录时返回 0。
 */
double GetASL(LinkList* H, int bucketCount) {

}


int main(){
    freopen("input.txt","r",stdin);
    freopen("result.txt","w",stdout);
	LinkList H[m];
	InitList(H);
	string infoFilename = "stocks.txt";
	string detailDir = "detail";
	ReadFile(H, infoFilename, detailDir);
	string sname; // 输入要查找的股票英文名称
	getline(cin, sname);
	int pos = SearchHL(H, sname);
	if (pos != -1){
		LinkList p = H[pos]->next; // 查找成功，将p指向单链表头结点
		Print(p, sname);
		double ASL = GetASL(H, m);
		cout << "ASL为：" << fixed << setprecision(2) << ASL << endl;
	}
	else{
		cout << "查找失败" << endl;
	}
	return 0;
}
