#include <bits/stdc++.h>
#define MAXSIZE 10000
using namespace std;

/*================ 学生函数声明 ================*/

// 基于BF算法将输入的模式串与文本进行匹配
int BF(const char* S, const char* T, int pos);
//实体识别函数
bool EntityRecognition(const char* S, const char* T, ofstream& fout);


/**
 * 从主串 S 的 pos 位置开始，用 BF（朴素模式匹配）查找模式串 T。
 *
 * @param S            主串（非结构化文本）
 * @param T            模式串（规则中的字面文字）
 * @param pos          开始匹配的位置
 * @return 匹配成功返回起始下标；失败返回 -1。
 */
int BF(const char* S, const char* T, int pos){

}

/**
 * 按规则 T 从文本 S 中识别出所有实体，并把实体逐行写入输出流 fout。
 *
 * @param S            非结构化文本
 * @param T            规则串，'*' 表示一个汉字的通配符
 * @param fout         （输出参数）结果输出流，函数会写入匹配到的实体
 * @return 匹配到至少一个实体返回 true；否则返回 false（由主函数输出「无匹配」）。
 */
bool EntityRecognition(const char* S, const char* T, ofstream& fout){

}


int main() {
    char keyword[100];
    char text[10000];

    ifstream fin("input.txt");
    ofstream fout("result.txt");

    if (!fin.is_open() || !fout.is_open()) {
             return 1;
    }

    // 从文本文件读取包含空格的内容
    fin.getline(keyword, 100);
    fin.getline(text, 10000);

    if (!EntityRecognition(text, keyword, fout))
        fout << "无匹配" << endl;

    fin.close();
    fout.close();

    return 0;
}
