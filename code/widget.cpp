/****************************************************
 * @Copyright © 2024 高博. All rights reserved.
 *
 * @FileName: widget.cpp
 * @Identifier: widget
 * @Brief: SCNU 2024年9月 编译原理课程项目
 * @Module Function:任务1：输入程序设计语言正则表达式，NFA，DFA，最小化DFA，词法分析程序，输出单词编码LEX文件
 *                  任务2：输入文法规则，LR1DFA,LALR1DFA,LALR1分析表，对LEX文件做语法分析，输出语法树
 *                  任务3：以minic作为输入，能够
 *
 * @Current Version: 1.0
 * @Author: 高博
 * @Modifier: 高博
 * @Finished Date: 2024/12/04
 *
 * @Version History:
 * - 1.0 2024/12/04 高博: 完成所有代码的编写
 ****************************************************/

#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QInputDialog>
#include <QFile>
#include <QStack>
#include <QString>
#include <QDebug>
#include <qdebug.h>
#include <QTextStream>
#include <QDebug>
#include <QFileDialog>
#include <QTextCodec>
#include <QMessageBox>
#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <unordered_map>
#include <queue>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include<QProcess>
using namespace std;

// 词法分析 EPSILON定义
const char regexEpsilon = '#';


/*
* @brief 模拟trim函数，去除特殊特殊字符串开头的转义符号，\+ -> +      \* -> *
*/
string myTrim(const string& str)
{
    if (str.empty() || str == "\\n")
    {
        return str;
    }
    string ret(str);
    ret.erase(0, ret.find_first_not_of("\\"));
    return ret;
}

// 正则表达式前缀
QVector<QString> prefixList = {"keywords", "operators", "operators_name",
                         "letter", "digit", "id", "num", "comment", "_final"};
// 正则表达式与对应的命名
QMap<QString, QString>regexMap;

// 生成NFA和DFA的正则表达式
string finalRegex;

// 关键词合集
set<string> keyWords;
// 操作符号map
map<string,string> opMap;

// 注释符号集合，0为开始符号，1为结束符号
string commentLetters[2];

// 全局结点计数器
int nodeCount = 0;

// 是否忽略大小写（默认不忽略）
bool isLowerCase = false;


// lex编码相关
int keywordsStartIndex = 1; // keywords编码
int operatorsStartIndex = 200;// operators编码
int numberIndex = 100;   // number编码
int idIndex = 101;       // id编码

QMap<QString, int> keywordsLexCode; //keywords的lex映射
QMap<QString, int> operatorsLexCode; //operators的lex映射
QMap<int, QString> lexcode2str; // lex编码到字符串的转换



Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}



/*
* @brief 获取关键词列表
*/
void getKeyWords() {
    keywordsLexCode.clear();
    QString keyRegex = regexMap["keywords"];
    QStringList keys = keyRegex.split("|");
    int i = keywordsStartIndex;
    for(QString key : keys){
        keywordsLexCode[key] = i++;
        keyWords.insert(key.toStdString());
    }
}

// 符号->字符串
map<char, string> char2string = { {(char)1,"digit"}, {(char)2, "letter"},{(char)3, "\\+"},{(char)4, "\\|"},
                                  {(char)5, "\\("},{(char)6, "\\)"},{(char)7, "\\*"},{(char)16, "\\?"},
                                  {(char)17, "\\["},{(char)18, "\\]"},{ (char)19,"\\~"}, {(char)20,"\\n"}};
// 字符串->符号
map<string, char> string2char = { {"digit",(char)1}, {"letter", (char)2 },{"\\+", (char)3 },{"\\|", (char)4 },{ "\\(", (char)5},{ "\\)" ,(char)6},{"\\*" ,(char)7},{ "\\?",(char)16},{"\\[", (char)17},{"\\]", (char)18 }, {"\\~", (char)19},{"\\n",(char)20} };

// 字符统计集合，用于展示在NFA、DFA图的表头
set<char> nfaCharSet;
set<char> dfaCharSet;

//获取操作符号的名称
string getOpName() {
    QString regex1 = regexMap["operators"];
    QString regex2 = regexMap["operators_name"];
    QStringList op = regex1.split("|");
    QStringList opName = regex2.split("|");
    if (op.size() != opName.size()) {
        return "操作符和操作符名称个数不一致！";
    }

    // 记录相关的lex编码
    operatorsLexCode.clear();
    int idx = operatorsStartIndex;
    for (int i = 0; i < op.size(); i++) {
        QString cur_op = op[i];
        std::string stdop = cur_op.toStdString();
        if(string2char.find(stdop) != string2char.end()){
            cur_op = QString::fromStdString(myTrim(stdop));
        }
        operatorsLexCode[cur_op] = idx++;
        opMap[op[i].toStdString()] = opName[i].toStdString();
    }
    qDebug() << "op complete!";

    //使用map映射关系替换正则表达式中的符号
    for (const auto& mPair : string2char) {
        auto it = opMap.find(mPair.first);
        if (it != opMap.end()) {
            // 创建一个新的pair，其中first是mPair.second，second是原始值
            std::pair<std::string, std::string> newPair(std::string(1, mPair.second), it->second);
            // 替换原来的元素
            opMap.erase(it); // 先删除旧的元素
            opMap.insert(newPair); // 插入新的元素
        }
    }

    return "";
}

/*
* @brief 获取注释符号
*/
string getCommentSymbol() {

    QString& regex = regexMap["comment"];
    QStringList t = regex.split("~");


    if (t.size() != 2) return "注释输入格式错误";
    commentLetters[0] = t[0].toStdString();
    commentLetters[1] = t[1].toStdString();
    regex = t[0] + "~*" + t[1];
    return "";
}

/*
* @brief 判断是否是除特殊符号外的字符
*/
bool isChar(char c)
{
    if (!(c == '+' || c == '[' || c == ']' || c == '|' || c == '*' || c == '(' || c == ')' || c == '?' || c == '@'))
        return true;
    return false;
}

/*
* @brief 判断是否需要添加连接符号@
*/
bool IsNeedConaction(char ch1, char ch2){
    if ((isChar(ch1) && isChar(ch2))
        || (isChar(ch1) && ch2 == '(')
        || (ch1 == ')' && isChar(ch2))
        || (ch1 == ')' && ch2 == '(')
        || (ch1 == '*' && ch2 != ')' && ch2 != '|' && ch2 != '?')
        || (ch1 == '?' && ch2 != ')')
        || (ch1 == '+' && ch2 != ')'))
        return true;
    else return false;

}

/*
* @brief 处理正则表达式中的+符号，并且在多字符的符号(如)中间添加@以便后续处理
*/
QString processRegex(QString regex)
{
    string regexStd = regex.toStdString();
    qDebug() << "enter handleRegex: " << regex;

    // 处理中括号 [a-c] -> a|b|c
    string result;
    bool insideBrackets = false;
    string currentString;

    for (char c : regexStd) {
        if (c == ' ') continue;
        if (c == '[') {
            insideBrackets = true;
            currentString.push_back('(');
        }
        else if (c == ']') {
            insideBrackets = false;
            currentString.push_back(')');
            result += currentString;
            currentString.clear();
        }
        else if (insideBrackets) {
            if (currentString.length() > 1) {
                currentString.push_back('|');
            }
            currentString.push_back(c);
        }
        else {
            result.push_back(c);
        }
    }

    regexStd = result;

    //处理+号 a+ -> a a*
    for (int i = 0; i < regexStd.size(); i++)
    {
        if (regexStd[i] == '+')
        {
            int kcount = 0;
            int j = i;
            do
            {
                j--;
                if (regexStd[j] == ')')
                {
                    kcount++;
                }
                else if (regexStd[j] == '(')
                {
                    kcount--;
                }
            } while (kcount != 0);
            string str1 = regexStd.substr(0, j);
            string kstr = regexStd.substr(j, i - j);
            string str2 = regexStd.substr(i + 1, (regexStd.size() - i));
            regexStd = str1 + kstr + kstr + "*" + str2;
        }
    }

    //处理连接字符: ab -> a@b
    for (int i = 0; i < regexStd.size() - 1; i++)
    {
        if (IsNeedConaction(regexStd[i], regexStd[i+1]))
        {
            string str1 = regexStd.substr(0, i + 1);
            string str2 = regexStd.substr(i + 1, (regexStd.size() - i));
            str1 += "@";
            regexStd = str1 + str2;
        }

    }
    return QString::fromStdString(regexStd);
}

/*
* @brief 检查输入的正则表达式的赋值
*/
QStringList checkPrefix(QStringList lines){

    for(int i = 0; i < prefixList.size(); i++){
        QString pre = prefixList[i];
        regexMap[pre] = "";
    }


    QVector<bool> prefixFlagArray(9, false);  //全0数组,用于判断是否prefixlist的元素都得到赋值
    for(int i = 0; i < lines.size(); i++ ){
        QString& line = lines[i];

        QString prefix = line.mid(0, line.indexOf('='));
        int idx = prefixList.indexOf(prefix);
        if(idx == -1)
            return QStringList(); // 没有规定的输入，返回空List
        else{
            regexMap[prefix] = line.mid(line.indexOf('=') + 1);
            prefixFlagArray[idx] = true;
        }
    }

    //判断是否所有前缀都有赋值
    for(int i = 0; i < prefixFlagArray.size(); i++)
        if(prefixFlagArray[i] == false)
            return QStringList(); //没有全部都赋值，返回空

//    for(auto key : regexMap.keys()){
//        qDebug() << "key:" << key << "value:" << regexMap[key];
//    }
    return lines;
}



/*
* @brief 用先前赋值好的变量对下划线开头的_final进行替换
*/
QString replaceFinal(){
    int final_idx = prefixList.indexOf("_final");
    QString finalRegex = regexMap[prefixList[final_idx]];
//    qDebug() << finalRegex;

    finalRegex.remove("keywords"); //keywords已经专门处理过，此处不做处理
    if(finalRegex.startsWith("|"))finalRegex.remove(0, 1); // 删除‘|’
    finalRegex.replace("operators", "(" + regexMap["operators"] + ")");
    finalRegex.replace("id", "(" + regexMap["id"] + ")");
    finalRegex.replace("num", "(" + regexMap["num"] + ")");
    finalRegex.replace("comment", "(" + regexMap["comment"] + ")");

    // 使用map映射关系替换正则表达式中的符号
    for (const auto& pair : string2char) {
        finalRegex.replace(QString::fromStdString(pair.first), QString(pair.second));
    }
//    qDebug() << finalRegex;

    return finalRegex;

}


/*
 * 将字符串到lex编码的映射反过来
 */

void reverseLexCode(){
    lexcode2str.clear();
    lexcode2str[numberIndex] = "num";
    lexcode2str[idIndex] = "id";
    for(QString key : keywordsLexCode.keys()){
        int code = keywordsLexCode[key];
        lexcode2str[code] = key;
    }
    for(QString key : operatorsLexCode.keys()){
        int code = operatorsLexCode[key];
        lexcode2str[code] = key;
    }
    qDebug() << "reverse complere";

}




/*
* @brief 正则表达式处理的主函数
*/
string processInput(QString allRegex, bool isLowerCase) {

    // 不区分大小写的话，全部转为小写
    if (isLowerCase) {
        allRegex = allRegex.toLower();
    }
    // 去除空格
    allRegex.replace(" ","");
    // 将文本内容按行分割成一个字符串列表
    QStringList lines = allRegex.split("\n");

    //去除多余的换行
    for(QString line : lines){
        if(line == "")lines.removeOne(line);
    }

    //检查输入的前缀,即‘=’左边被赋值的对象
    lines = checkPrefix(lines);
    if(lines.size() == 0){
        //检验输入
        return "输入的正则表达式不符合规范，请点击查看规则后重新输入！";
    }
//    qDebug() << "ok";

    getKeyWords();  // 获取关键词
    string res = getCommentSymbol();  // 获取注释开始符号和结束符号后
    res = getOpName(); //获取操作符和操作符的名字
    reverseLexCode();


    // 拼接生成NFA和DFA图的正则表达式
    QString replacedFinal = replaceFinal(); //用已经命名的字符串替换_final中的字符串

    finalRegex = replacedFinal.toStdString();

    finalRegex = processRegex(replacedFinal).toStdString(); //进一步处理

    return res;
}

struct nfaNode; //结构体声明

//NFA图的边
struct nfaEdge
{
    char c; // 边上的字符
    nfaNode* next; //指向目标
};
//NFA图的结点
struct nfaNode
{
    bool isStart;   // 是否是初态
    bool isEnd; // 是否是终态
    int id; // 结点id
    vector<nfaEdge> edges;  // 可以到达的节点数组
    nfaNode() {
        id = nodeCount++;
        isStart = false;
        isEnd = false;
    }
    nfaNode(int id){
        this->id = id;
        isStart = false;
        isEnd = false;
    }
};

// NFA图
struct nfaGraph
{
    nfaNode* start;  //初态指针
    nfaNode* end;  //终态指针
    nfaGraph() {}
    nfaGraph(nfaNode* s, nfaNode* e)
    {
        start = s;
        end = e;
    }
};

/*
* @brief 由字符创建NFA节点
*/
nfaGraph fromSymbol(char ch) {
    nfaNode* start = new nfaNode();
    nfaNode* end = new nfaNode();

    start->isStart = true;
    end->isEnd = true;

    nfaEdge new_edge;
    new_edge.c = ch;
    new_edge.next = end;
    start->edges.push_back(new_edge);

    nfaGraph newNfa(start, end); //创建一个nfa图

    // 存入全局nfa字符set
    nfaCharSet.insert(ch);
    // 存入全局dfa字符set
    dfaCharSet.insert(ch);

    return newNfa;
}


// NFA @连接运算

nfaGraph setConcat(nfaGraph nfa1, nfaGraph nfa2) {

    // nfa1的终态指向nfa2的初态
    nfa1.end->isEnd = false;
    nfa2.start->isStart = false;

    nfaEdge newedge;
    newedge.c = regexEpsilon; //epsilon转换
    newedge.next = nfa2.start;
    nfa1.end->edges.push_back(newedge);

    nfaGraph newNfa;
    newNfa.start = nfa1.start;
    newNfa.end = nfa2.end;

    return newNfa;
}

// NFA | 选择运算
nfaGraph setUnions(nfaGraph nfa1, nfaGraph nfa2) {
    nfaNode* startNode = new nfaNode();
    nfaNode* endNode = new nfaNode();

    startNode->isStart = true;
    endNode->isEnd = true;

    // 连接startNode与nfa1和nfa2的初态
    nfaEdge edge1;
    edge1.c = regexEpsilon;
    edge1.next = nfa1.start;
    startNode->edges.push_back(edge1);
    nfa1.start->isStart = false;

    nfaEdge edge2;
    edge2.c = regexEpsilon;
    edge2.next = nfa2.start;
    startNode->edges.push_back(edge2);
    nfa2.start->isStart = false;    // 初态结束


    // 连接endNode与nfa1和nfa2的终态
    nfa1.end->isEnd = false;
    nfa2.end->isEnd = false;

    nfaEdge edge3;
    edge3.c = regexEpsilon;
    edge3.next = endNode;
    nfa1.end->edges.push_back(edge3);

    nfaEdge edge4;
    edge4.c = regexEpsilon;
    edge4.next = endNode;
    nfa2.end->edges.push_back(edge4);

    nfaGraph newNfa{ startNode , endNode };

    return newNfa;
}

// brief NFA * 闭包运算

nfaGraph setClosure(nfaGraph nfa1) {
    nfaNode* startNode = new nfaNode();
    nfaNode* endNode = new nfaNode();

    startNode->isStart = true;
    endNode->isEnd = true;

    // 连接新初态与nfa1初态
    nfaEdge edge1;
    edge1.c = regexEpsilon;
    edge1.next = nfa1.start;
    startNode->edges.push_back(edge1);
    nfa1.start->isStart = false;    // 初态结束

    // 连接新初态和新终态
    nfaEdge edge2;
    edge2.c = regexEpsilon;
    edge2.next = endNode;
    startNode->edges.push_back(edge2);


    nfa1.end->isEnd = false;

    // 连接nfa1的终态和nfa1终态
    nfaEdge edge3;
    edge3.c = regexEpsilon;
    edge3.next = nfa1.start;
    nfa1.end->edges.push_back(edge3);

    //连接nfa1的终态和新终态
    nfaEdge edge4;
    edge4.c = regexEpsilon;
    edge4.next = endNode;
    nfa1.end->edges.push_back(edge4);

    nfaGraph nfa{ startNode,endNode };

    return nfa;
}

// NFA ? 可选运算
nfaGraph setOption(nfaGraph nfa1) {
    nfaNode* start = new nfaNode();
    nfaNode* end = new nfaNode();

    start->isStart = true;
    end->isEnd = true;

    // 把新的初态与nfa1的初态连接起来
    nfaEdge edge1;
    edge1.c = regexEpsilon;
    edge1.next = nfa1.start;
    start->edges.push_back(edge1);
    nfa1.start->isStart = false;    // 初态结束

    // 把新的初态与新的终止状态连接起来
    nfaEdge edge2;
    edge2.c = regexEpsilon;
    edge2.next = end;
    start->edges.push_back(edge2);

    // 把nfa1的终止状态与新的终止状态连接起来
    nfa1.end->isEnd = false;

    nfaEdge edge3;
    edge3.c = regexEpsilon;
    edge3.next = end;
    nfa1.end->edges.push_back(edge3);

    nfaGraph nfa(start, end);

    return nfa;
}

//获取运算符号的优先级
int getPriority(char op) {
    if (op == '*' || op == '?' || op == '+')  return 3;
    else if (op == '@') return 2;
    else if (op == '|')  return 1;
    else  return 0;  // 默认情况下，将其它字符的优先级设为0
}

/*
* @brief 结构体，转换表中一行的数据
*/
struct nfaTableNode
{
    bool isStart;  //是否是初态
    bool isEnd; //是否是终态
    int id;
    map<char, set<int>> m;  // 对应字符能到达的状态
    nfaTableNode()
    {
        this->isStart = false;
        this->isEnd = false;
    }
};


unordered_map<int, nfaTableNode> nfaTable; //nfa状态转换表
// statusTable插入顺序记录，方便后续输出
vector<int> insertion_vec;


// nfa初态集合
set<int> nfaStartSet;

//nfa终态集合
set<int> nfaEndSet;

//生成nfa的状态转换表
void generateNFAMatrix(nfaGraph& nfa)
{
    QStack<nfaNode*> nfaStack;
    set<nfaNode*> visited; // 已被访问的节点集合

    // 初态
    nfaNode* start = nfa.start;
    nfaTableNode startTableNode;
    startTableNode.isStart = true;  // 初态
    startTableNode.id = start->id;
    nfaTable[start->id] = startTableNode;
    insertion_vec.push_back(start->id);
    nfaStartSet.insert(start->id);

    nfaStack.push(start);

    while (!nfaStack.empty()) {
        nfaNode* currentNode = nfaStack.top();
        nfaStack.pop();
        visited.insert(currentNode);

        for (nfaEdge edge : currentNode->edges) {
            char transitionChar = edge.c;
            nfaNode* nextNode = edge.next;

            // 记录状态转换信息
            nfaTable[currentNode->id].m[transitionChar].insert(nextNode->id);

            // 如果下一个状态未被访问，将其加入堆栈
            if (visited.find(nextNode) == visited.end()) {
                nfaStack.push(nextNode);

                // 记录状态信息
                nfaTableNode nextStatus;
                nextStatus.id = nextNode->id;
                if (nextNode->isStart) {
                    nextStatus.isStart = true; //初态
                    nfaStartSet.insert(nextStatus.id);
                }
                else if (nextNode->isEnd) {
                    nextStatus.isEnd = true; //终态
                    nfaEndSet.insert(nextStatus.id);
                }
                nfaTable[nextNode->id] = nextStatus;
                // 记录插入顺序（排除终态）
                if (!nextNode->isEnd)
                {
                    insertion_vec.push_back(nextNode->id);
                }
            }
        }
    }

    // 顺序表才插入终态
    nfaNode* endNode = nfa.end;
    insertion_vec.push_back(endNode->id);
}



//正则表达式转NFA入口
nfaGraph regex2NFA(string regex)
{
    // 双栈法，创建两个栈opStack（运算符栈）,nfaStack（nfa图栈）
    QStack<char> opStack;
    QStack<nfaGraph> nfaStack;

    for (char c : regex)
    {
        switch (c)
        {
        case ' ': // 空格跳过
            break;
        case '(':
            opStack.push(c);
            break;
        case ')':
            while (!opStack.empty() && opStack.top() != '(')
            {
                // 处理栈顶运算符，构建NFA图，并将结果入栈
                char op = opStack.top();
                opStack.pop();

                if (op == '|') {
                    // 处理并构建"|"运算符
                    nfaGraph nfa2 = nfaStack.top();
                    nfaStack.pop();
                    nfaGraph nfa1 = nfaStack.top();
                    nfaStack.pop();

                    // 创建新的NFA，表示nfa1 | nfa2
                    nfaGraph resultNFA = setUnions(nfa1, nfa2);
                    nfaStack.push(resultNFA);
                }
                else if (op == '@') {
                    // 处理并构建"."运算符
                    nfaGraph nfa2 = nfaStack.top();
                    nfaStack.pop();
                    nfaGraph nfa1 = nfaStack.top();
                    nfaStack.pop();

                    // 创建新的NFA，表示nfa1 . nfa2
                    nfaGraph resultNFA = setConcat(nfa1, nfa2);
                    nfaStack.push(resultNFA);
                }
            }
            if (opStack.empty())
            {
                qDebug() << "括号未闭合，请检查正则表达式！";
                exit(-1);
            }
            else
            {
                opStack.pop(); // 弹出(
            }
            break;
        case '|':
        case '@':
            // 处理运算符 | 和 @
            while (!opStack.empty() && (opStack.top() == '|' || opStack.top() == '@') &&
                getPriority(opStack.top()) >= getPriority(c))
            {
                char op = opStack.top();
                opStack.pop();

                // 处理栈顶运算符，构建NFA图，并将结果入栈
                if (op == '|') {
                    // 取出两个结点执行untion
                    nfaGraph nfa2 = nfaStack.top();
                    nfaStack.pop();
                    nfaGraph nfa1 = nfaStack.top();
                    nfaStack.pop();
                    nfaGraph resultNFA = setUnions(nfa1, nfa2);
                    nfaStack.push(resultNFA);
                }
                else if (op == '@') {
                    // 执行concat运算
                    nfaGraph nfa2 = nfaStack.top();
                    nfaStack.pop();
                    nfaGraph nfa1 = nfaStack.top();
                    nfaStack.pop();
                    nfaGraph resultNFA = setConcat(nfa1, nfa2);
                    nfaStack.push(resultNFA);
                }
            }
            opStack.push(c);
            break;
        case '?':
        case '*':
            //两种闭包
            // 从nfaStack弹出NFA，应用相应的闭包操作，并将结果入栈
            if (!nfaStack.empty()) {
                nfaGraph nfa = nfaStack.top();
                nfaStack.pop();
                if (c == '?') {
                    // option运算
                    nfaGraph resultNFA = setOption(nfa);
                    nfaStack.push(resultNFA);
                }
                else if (c == '*') {
                    // clouser运算
                    nfaGraph resultNFA = setClosure(nfa);
                    nfaStack.push(resultNFA);
                }
            }
            else {
                qDebug() << "error: no closure or option";
                exit(-1);
            }
            break;
        default:
            nfaGraph nfa = fromSymbol(c);  //字符创建nfanode
            nfaStack.push(nfa);
            break;
        }

    }

    // 处理栈中剩余的运算符
    while (!opStack.empty())
    {
        char op = opStack.top();
        opStack.pop();

        if (op == '|' || op == '@')
        {
            // 处理并构建运算符 | 和 .
            if (nfaStack.size() < 2)
            {
                qDebug() << "error: op not enough! " << op << "！";
                exit(-1);
            }
            //取出两个节点
            nfaGraph nfa2 = nfaStack.top();
            nfaStack.pop();
            nfaGraph nfa1 = nfaStack.top();
            nfaStack.pop();

            if (op == '@')
            {
                // 执行concat运算
                nfaGraph resultNFA = setConcat(nfa1, nfa2);
                nfaStack.push(resultNFA);
            }
            else if (op == '|')
            {
                // 执行union运算
                nfaGraph resultNFA = setUnions(nfa1, nfa2);
                nfaStack.push(resultNFA);
            }
        }
        else
        {
            qDebug() << "error " << op << "！";
            exit(-1);
        }
    }

    // 最终的NFA图在nfaStack的顶部
    qDebug() << "NFA complete";
    nfaGraph result = nfaStack.top(); //栈顶元素是最终的图

    generateNFAMatrix(result); //构建转换表

    return result;
}

// dfa终态集合
set<int> dfaFinalSet;
// dfa非终态集合
set<int> dfaNonFinalStateSet;

// dfa节点
struct dfaNode
{
    bool isStart; // 是否是初态
    bool isEnd; // 是否是终态
    set<int> nfaStatesSet; // 该DFA状态包含的NFA状态的集合
    map<char, set<int>> transitions; // 字符到下一状态的映射
    dfaNode() {
        this->isStart = false;
        this->isEnd = false;
    }
};



int startStaus;// 初态状态号


// first = true : 有初态， second=true : 终态
pair<bool, bool> isSetHasStartOrEnd(set<int>& statusSet)
{
    bool start = false, end = false;
    for (auto element : nfaStartSet) {
        if (statusSet.count(element) > 0) {
            start = true;
            break;
        }
    }

    for (auto element : nfaEndSet) {
        if (statusSet.count(element) > 0) {
            end = true;
            break;
        }
    }

    return {start, end};
}


// dfa状态去重集
set<set<int>> dfaStateSet;

// dfa最终结果
vector<dfaNode> dfaGraphMatrix;

// set对应序号MAP
map<set<int>, int> dfa2numberMap;

/*
* @brif 找到所有通过epsilon可以到达的集合节点
*/
set<int> epsilonClosure(int id)
{
    set<int> epsilonResSet{ id };
    QStack<int> stack;
    stack.push(id);

    while (!stack.empty())
    {
        int current = stack.top();
        stack.pop();

        set<int> eClosure = nfaTable[current].m[regexEpsilon];
        for (auto t : eClosure)
        {
            if (epsilonResSet.find(t) == epsilonResSet.end())
            {
                epsilonResSet.insert(t);
                stack.push(t);
            }
        }
    }

    return epsilonResSet;
}

// 找到NFA所有通过特定字符ch可以到达的集合节点
set<int> nfaCharClosure(int id, char ch)
{
    set<int> otherResult{};
    set<int> processed;
    stack<int> stack;
    stack.push(id);

    while (!stack.empty())
    {
        int current = stack.top();
        stack.pop();

        if (processed.find(current) != processed.end())
            continue;

        processed.insert(current);

        set<int> otherClosure = nfaTable[current].m[ch];
        for (auto o : otherClosure)
        {
            auto tmp = epsilonClosure(o);
            otherResult.insert(tmp.begin(), tmp.end());
            stack.push(o);
        }
    }

    return otherResult;
}


/*
* @brif NFA到DFA的转换
*/
void NFA2DFA(nfaGraph& nfa)
{
    int statusCnt = 1;// 记录DFA状态计数
    auto start = nfa.start; // 获得NFA图的起始位置
    auto startId = start->id;   // 获得起始编号
    dfaNode startDFANode;
    startDFANode.nfaStatesSet = epsilonClosure(startId); // 初始闭包

    // 判断DFA初始节点是否包含NFA的初始节点
    pair<bool, bool> temp_pair = isSetHasStartOrEnd(startDFANode.nfaStatesSet);
    startDFANode.isStart = temp_pair.first, startDFANode.isEnd = temp_pair.second;

    deque<set<int>> newStateQue{}; //待处理的新DFA节点dqueue
    // 记录NFA状态集合到DFA编号的映射关系
    dfa2numberMap[startDFANode.nfaStatesSet] = 1;
    startStaus = 1;
    if (temp_pair.second) {
       // 如果初始闭包包含NFA的终态，加入DFA终态集合
        dfaFinalSet.insert(statusCnt++);
    }
    else
    {
        // 不包含终态，加入DFA非终态集合
        dfaNonFinalStateSet.insert(statusCnt++);
    }
    // 遍历所有输入字符，计算初始节点的状态转移
    for (auto ch : dfaCharSet)
    {
        set<int> closureOfCurCh{}; //当前符号可以转移到的集合
        for (int id : startDFANode.nfaStatesSet)
        {
            set<int> tmp = nfaCharClosure(id, ch);
            closureOfCurCh.insert(tmp.begin(), tmp.end());
        }
        if (closureOfCurCh.empty())
        {
            // 如果闭包为空，跳过该符号
            continue;
        }
        int beforeSize = dfaStateSet.size();
        dfaStateSet.insert(closureOfCurCh);
        int afterSize = dfaStateSet.size();
        startDFANode.transitions[ch] = closureOfCurCh;
        // 前后大小不一致，为新状态
        if (afterSize > beforeSize)
        {
            dfa2numberMap[closureOfCurCh] = statusCnt;// 为新状态分配编号
            newStateQue.push_back(closureOfCurCh);// 新状态入队列
            if (isSetHasStartOrEnd(closureOfCurCh).second) {
                 // 如果新状态包含NFA终态，加入DFA终态集合
                dfaFinalSet.insert(statusCnt++);
            }
            else
            {
                 // 不包含NFA终态，加入非终态集合
                dfaNonFinalStateSet.insert(statusCnt++);
            }

        }

    }
    dfaGraphMatrix.push_back(startDFANode);

     // 遍历所有新状态队列，处理每个新状态的转移关系
    while (!newStateQue.empty())
    {
        set<int> queFrontStatus = newStateQue.front();//队首
        newStateQue.pop_front();
        dfaNode DFANode;
        DFANode.nfaStatesSet = queFrontStatus;  // 该节点状态集合
        DFANode.isStart = isSetHasStartOrEnd(queFrontStatus).first, DFANode.isEnd = isSetHasStartOrEnd(queFrontStatus).second;

        for (char ch : dfaCharSet)
        {

            set<int> closureOfCurCh{};
            for (int id : queFrontStatus)
            {
                set<int> tmp = nfaCharClosure(id, ch); //转移后的闭包
                closureOfCurCh.insert(tmp.begin(), tmp.end());
            }
            if (closureOfCurCh.empty())
            {
                // 闭包为空，跳过
                continue;
            }
            int beforeSize = dfaStateSet.size();
            dfaStateSet.insert(closureOfCurCh);
            int afterSize = dfaStateSet.size();
            // 不管一不一样都是该节点这个字符的状态
            DFANode.transitions[ch] = closureOfCurCh;
            // 如果大小不一样，证明是新状态
            if (afterSize > beforeSize)
            {
                // 大小不一致
                dfa2numberMap[closureOfCurCh] = statusCnt;
                newStateQue.push_back(closureOfCurCh);
                if (isSetHasStartOrEnd(closureOfCurCh).second) {
                    dfaFinalSet.insert(statusCnt++);
                }
                else
                {
                    dfaNonFinalStateSet.insert(statusCnt++);
                }

            }

        }
        dfaGraphMatrix.push_back(DFANode);

    }
}



// {isStart, isEnd}
pair<bool, bool> minSetHasStartOrEnd(set<int>& statusSet)
{
    //{start, end}
    bool start = false, end = false;
    string result = "";
    if (statusSet.count(startStaus) > 0) {
        start = true;
        result += "-";
    }

    for (const int& element : dfaFinalSet) {
        if (statusSet.count(element) > 0) {
            end = true;
            break;  // 可能会有多个终态同时包含，我们只要一个
        }
    }
    return pair<bool, bool> {start, end};
}

// dfa最小化节点
struct minDfaNode
{
    bool isStart;
    bool isEnd;
    int id;
    map<char, int> transitions; // 字符到下一状态的映射
    minDfaNode();
};
//最小化dfa的最终结果
vector<minDfaNode> dfaMinTable;

// 分割集合
vector<set<int>> divideVector;
// 存下标
map<int, int> dfaMinMap;


/*
* @brif 根据字符 ch 将状态集合 node 分成两个子集合
*/
void splitByChar(int i, char ch)
{
    set<int> splitRes;
    auto& node = divideVector[i];
    int originalIndex = -2;

    for (auto state : node)
    {
        int curNum;
        if (dfaGraphMatrix[state - 1].transitions.find(ch) == dfaGraphMatrix[state - 1].transitions.end())
        {
            // 如果当前状态没有通过字符 ch 的有效转移，划分编号设为 -1（代表空集）
            curNum = -1;
        }
        else
        {
            // 根据字符 ch 找到下一个状态
            int nextStateId = dfa2numberMap[dfaGraphMatrix[state - 1].transitions[ch]];
            curNum = dfaMinMap[nextStateId];
        }

        if (originalIndex == -2)    // 初始下标
        {
            originalIndex = curNum;
        }
        else if (curNum != originalIndex)   // 如果下标不同，就是有问题，需要分出来
        {
            splitRes.insert(state);
        }
    }

    // 从当前状态集合中移除已分离的状态
    for (int state : splitRes) {
        node.erase(state);
    }

    // 如果有新的子集产生，将其加入 divideVector 中
    if (!splitRes.empty())
    {
        divideVector.push_back(splitRes);
        // 同时更新下标
        for (auto a : splitRes)
        {
            dfaMinMap[a] = divideVector.size() - 1;
        }
    }
}


//DFA最小化
void DFAminimize()
{
    divideVector.clear();
    dfaMinMap.clear();

    // 存入非终态、终态集合
    if (dfaNonFinalStateSet.size() != 0)
    {
        divideVector.push_back(dfaNonFinalStateSet);
    }
    // 初始化map
    for (int t : dfaNonFinalStateSet)
    {
        dfaMinMap[t] = divideVector.size() - 1;
    }

    divideVector.push_back(dfaFinalSet);

    for (int t : dfaFinalSet)
    {
        dfaMinMap[t] = divideVector.size() - 1;
    }

    int flag = 1; //循环标志

    while (flag)
    {
        flag = 0;
        int beforeSize = divideVector.size();

        for (int i = 0; i < beforeSize; i++)
        {

            // 逐个字符尝试分割状态集合
            for (char ch : dfaCharSet)
            {
                splitByChar(i, ch);
            }
        }
        int afterSize = divideVector.size();

        if (afterSize > beforeSize)
        {
             // 如果划分数量增加，说明需要继续循环
            flag = 1;
        }
    }

    for (int cnt = 0; cnt < divideVector.size(); cnt++)
    {
        set<int>& curSet = divideVector[cnt];
        minDfaNode node;
        pair<bool, bool> startOrEnd = minSetHasStartOrEnd(curSet);
        node.isStart = startOrEnd.first;
        node.isEnd = startOrEnd.second;
        node.id = cnt;
        for (char ch : dfaCharSet)
        {
            if (curSet.size() == 0)
            {
                // 特殊处理空集合
                node.transitions[ch] = -1;
                continue;
            }
            int i = *(curSet.begin()); // 获取当前划分的一个状态
            if (dfaGraphMatrix[i - 1].transitions.find(ch) == dfaGraphMatrix[i - 1].transitions.end())
            {
                node.transitions[ch] = -1;   // 空集特殊判断
                continue;
            }
             // 获取通过字符 ch 转移后的状态编号，并找到对应的最小化划分编号
            int next_state = dfa2numberMap[dfaGraphMatrix[i - 1].transitions[ch]];
            int thisNum = dfaMinMap[next_state];    // 这个状态下标
            node.transitions[ch] = thisNum;
        }
        dfaMinTable.push_back(node);
    }

    qDebug() << "minDFA complete";
}






//将集合中的数据转换为string类型进行输出
string set2Str(set<int> s)
{
    string result;

    for (int i : s) {
        result.append(to_string(i));
        result.append(",");
    }

    if (result.size() != 0)
        result.pop_back(); //弹出最后一个逗号

    return result;
}

bool findInChar2Stdstr(char str){
    for(auto pair : char2string){
        if(str == pair.first){
            return true;
        }
    }
    return false;
}

// 词法分析程序生成letter/digit/comment的switch语句
// charList: 状态集合中有效字符的列表
// codeStr: 保存生成的 switch 语句字符串
// idx: 当前 DFA 状态在 MinDfaGraphVec 中的索引
// flag: 标记是否需要追加状态更新代码
// 如果检测到注释（"~"）字符，则返回 true；否则返回 false
bool getLexCodeCase(QList<QString> tmpList, QString& codeStr, int idx, bool flag)
{
    bool commentFlag = false; // 判断是否遇到注释符号
    for (int i = 0; i < tmpList.size(); i++)
    {
        QString tmpKey = tmpList[i];
        char ch = tmpKey.toUtf8().constData()[0];// 将字符转换为 ASCII 编码
        if (dfaMinTable[idx].transitions[ch] == -1) {
            // 如果当前字符无法通过 DFA 的转移，跳过
            continue;
        }
        // letter
        qDebug() << string2char["letter"];
        if (tmpKey == string2char["letter"])
        {
//            for (int j = 0; j < 26; j++)
//            {
//                codeStr += "\t\t\tcase \'" + QString(char('a' + j)) + "\':\n";
//                codeStr += "\t\t\tcase \'" + QString(char('A' + j)) + "\':\n";
//            }
//            codeStr.chop(1);// 去除换行符
//            if (flag)codeStr += "inId = true; ";
        }
        // digit
        else if (tmpKey == string2char["digit"])
        {

//            for (int j = 0; j < 10; j++)
//            {
//                codeStr += "\t\t\tcase \'" + QString::number(j) + "\':\n";
//            }
//            codeStr.chop(1);// 去除换行符
//            if (flag)codeStr += "inNum = true; ";
        }
        // comment
        else if (tmpKey == "~")
        {
            commentFlag = true;
            continue;
        }
        else {
            QString keyRes = tmpKey;
            if (char2string.find(ch) != char2string.end()) {
                keyRes = QString::fromStdString(myTrim(char2string[ch]));
            }
            codeStr += "\t\t\tcase \'" + keyRes + "\':";
        }

        if (flag && tmpKey != string2char["digit"] && tmpKey != string2char["letter"]) {
            codeStr += "current_state = " + QString::number(dfaMinTable[idx].transitions[ch]) + "; ";
        }
        if(tmpKey != string2char["digit"] && tmpKey != string2char["letter"])
            codeStr += "break;\n";
    }
    return commentFlag;
}


//@brief 让用户输入文件名后缀
QString getUserInput() {
    bool ok;
    QString input = QInputDialog::getText(
        nullptr,
        "提示",
        "请输入要读取的sample文件后缀(如tny , minic)",
        QLineEdit::Normal,
        "tny",
        &ok
    );

    if (ok && !input.isEmpty()) {
        // 用户点击确定，并且输入不为空
        return input;
    } else {
        // 用户取消输入或输入为空时返回空字符串
        return QString();
    }
}

/*
* @brif 生成词法分析代码
*/
QString generateLexerCode(QString filePath, QString sampleType)
{
    QString lexCode;

    //头文件
    lexCode +=
        "#include<stdio.h>\n"
        "#include<stdlib.h>\n"
        "#include<string.h>\n"
        "#include<stdbool.h>\n";

    // 所有关键字
    lexCode += "//所有关键字\n";
    lexCode += "const char* keywordSet[" + QString::number((int)keyWords.size()) + QString::fromStdString("] = {");
    for (const auto& tmp : keyWords)
    {
        lexCode += QString::fromStdString("\"" + tmp + "\",");
    }
    lexCode.chop(1);
    lexCode += " };\n";

    // 符号映射，用于将operators与operators_name对应
    lexCode += "//对应op与operators_name\n";
    lexCode += "static struct\n";
    lexCode += "{\n";
    lexCode += "\tconst char* str;\n";
    lexCode += "\tconst char* value;\n";
    lexCode += "} reservedWords[" + QString::number((int)opMap.size()) + QString::fromStdString("] = {");
    for (const auto& tmp : opMap)
    {
        char first = tmp.first[0];
        string keyRes = tmp.first;

        if (char2string.find(first) != char2string.end()) {
            // 判断是否是正则表达式符号专用符号，如果是，需要转换回正常符号
            keyRes = myTrim(char2string[first]);
        }
        lexCode += QString::fromStdString("{\"" + keyRes + "\",\"" + tmp.second + "\"},");
    }
    lexCode.chop(1);
    lexCode += " };\n";



    // lex编码映射
    lexCode += "//lex编码的映射\n"
               "static struct{\n"
               "\tconst char* op;\n"
               "\tconst int code;\n"
               "} keywordLexCode[" + QString::number((int)keywordsLexCode.size()) + QString::fromStdString("]={");
               for(const QString key : keywordsLexCode.keys()){
                   int id = keywordsLexCode[key];
                   lexCode += "{\"" + key + "\"," + QString::number(id) + "},";
               }
     lexCode.chop(1);
     lexCode += " };\n";


    lexCode += "static struct{\n"
               "\tconst char* op;\n"
               "\tconst int code;\n"
               "} opLexCode[" + QString::number((int)operatorsLexCode.size()) + QString::fromStdString("]={");
               for(const QString op : operatorsLexCode.keys()){
                   int id = operatorsLexCode[op];
                   lexCode += "{\"" + op + "\"," + QString::number(id) + "},";
               }
     lexCode.chop(1);
     lexCode += " };\n";



    // 词法分析器必需函数
    lexCode +=
        "//是否是关键字keywords\n"
        "bool isKeyWords(const char* str) {\n"
        "\tint i = 0;\n"
        "\tfor (i = 0; i < "+ QString::number((int)keyWords.size()) + "; i++) {\n"
        "\t\tif (!strcmp(str,keywordSet[i])) {\n"
        "\t\t\treturn true;\n"
        "\t\t}\n"
        "\t}\n"
        "\treturn false;\n"
        "}\n"
        "//追加字符\n"
        "void append(char* str, char tmp) {\n"
        "\tint len = strlen(str);\n"
        "\tstr[len] = tmp;\n"
        "\tstr[len + 1] = '\\0';\n"
        "}\n"
        "\n"
        "//是否是保留字符\n"
        "const char* getReversedWord(const char* str) {\n"
        "\tint i;"
        "\tfor (i = 0; i < " + QString::number((int)opMap.size()) + "; ++i) {\n"
        "\t\tif (strcmp(reservedWords[i].str, str) == 0) {\n"
        "\t\t\treturn reservedWords[i].value;\n"
        "\t\t}\n"
        "\t}\n"
        "\treturn NULL;\n"
        "}\n"
        "//判断是不是数字\n"
        "bool isDigit(char ch){\n"
        "\tif(ch >= '0' && ch <= '9') return true;\n"
        "\telse return false;\n"
        "}\n"
        "//判断是否是字母\n"
        "bool isAlpha(char ch){\n"
        "\tif((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))return true;\n"
        "\telse return false;\n"
        "}\n";


    //分析代码
    lexCode += "void lexicalAnalysis(FILE* input_file,FILE* output_file) {\n"
        "\tchar current_char = fgetc(input_file);\n";
    if (isLowerCase) {
        lexCode += "\tif (isupper(current_char)) {\n"
            "\t\tcurrent_char = tolower(current_char);\n"
            "\t}\n";
    }

    lexCode += "\tif (current_char == ' ' || current_char == '\\n' || current_char == '\\t'){\n"
        "\t\treturn;\n"
        "\t}\n"
        "\tungetc(current_char, input_file);\n"
        "\tint current_state = 0;\n"
        "\tbool flag = false;\n"
        "\tbool inId = false;\n"
        "\tbool inNum = false;\n"
        "\tbool isInComment = false;\n"
        "\tchar cur_line[1024];\n"
        "\tcur_line[0] = '\\0';\n"
        "\twhile (!flag) {\n"
        "\t\tcurrent_char = fgetc(input_file);//获取下一个字符\n"
        "\t\tswitch (current_state) {\n";

    //使用讲稿方法二生成词法分析代码
    for (auto i = 0; i < dfaMinTable.size(); i++)
    {
        if (dfaMinTable[i].transitions.size() != 0) {
            //当前case有转移
            bool hasDigit = false, hasLetter = false;
            QList<QString> cur_char_List;
            for (const auto& pair : dfaMinTable[i].transitions) {
                cur_char_List.append(QString(pair.first));
                if(dfaMinTable[i].transitions[string2char["letter"]] != -1 && dfaMinTable[i].transitions.find(string2char["letter"]) != dfaMinTable[i].transitions.end())
                    hasLetter = true;
                if(dfaMinTable[i].transitions[string2char["digit"]] != -1 && dfaMinTable[i].transitions.find(string2char["digit"]) != dfaMinTable[i].transitions.end())
                    hasDigit = true;
//                if(string2char.find("letter") != string2char.end() && dfaMinTable[i].transitions[string2char["letter"] != -1])hasLetter = true;
//                if(string2char.find("digit") != string2char.end() && dfaMinTable[i].transitions[string2char["digit"] != -1])hasDigit = true;
            }
            lexCode += "\t\tcase " + QString::number(dfaMinTable[i].id) + ": {\n";
            if(hasDigit){
                lexCode += "\t\t\tif(isDigit(current_char)) {\n"
                        "\t\t\t\tinNum = true; current_state = " + QString::number(dfaMinTable[i].transitions[string2char["digit"]]) + "; break;\n"
                        "\t\t\t}\n";
            }
            if(hasLetter){
                lexCode += "\t\t\t";
                if(hasDigit)lexCode += "else ";
                lexCode += "if(isAlpha(current_char)){\n"
                           "\t\t\t\tinId = true; current_state = " +  QString::number(dfaMinTable[i].transitions[string2char["letter"]])+ "; break;\n"
                           "\t\t\t}\n";
            }

            lexCode += "\t\t\tswitch (current_char) {\n";
            if (getLexCodeCase(cur_char_List, lexCode, i, 1))
                lexCode += "\t\t\tdefault:current_state = " + QString::number(dfaMinTable[i].transitions['~']) + "; isInComment = true; break;\n";
            lexCode += "\t\t\t}\n";
            lexCode += "\t\t\tbreak;\n";
            lexCode += "\t\t}\n";
        }
    }
    lexCode += "\t\t}\n";
    lexCode += "\t\tappend(cur_line, current_char);\n";


    // 终态集合
    QList<int> stateList;
    for (const minDfaNode& node : dfaMinTable) {
        if(node.isEnd) {
            stateList.append(node.id);
        }
    }

    lexCode += "\t\tif (";
    for (int i = 0; i < stateList.size(); i++)
    {

        int num = stateList[i];
        bool hasLetter = false, hasDigit = false;
        if(dfaMinTable[num].transitions[string2char["letter"]] != -1 && dfaMinTable[num].transitions.find(string2char["letter"]) != dfaMinTable[num].transitions.end())
            hasLetter = true;
        if(dfaMinTable[num].transitions[string2char["digit"]] != -1 && dfaMinTable[num].transitions.find(string2char["digit"]) != dfaMinTable[i].transitions.end())
            hasDigit = true;
        if (i)lexCode += "\t\telse if (";
        lexCode += "current_state ==" + QString::number(num) + ") {\n";
        //要提前读一个字符判断是不是真的到终态，因为到了终态不一定是真正的终态
        lexCode += "\t\t\tcurrent_char = fgetc(input_file);\n";
        if(hasDigit){
            lexCode += "\t\t\tif(isDigit(current_char)){} \n";
        }
        if(hasLetter){
            lexCode += "\t\t\t";
            if(hasDigit)lexCode += "else ";
            lexCode += "if(isAlpha(current_char)){}\n";
        }
        lexCode += "\t\t\t";
        if(hasDigit || hasLetter) lexCode += "else ";
        lexCode += "switch (current_char) {\n";
        QList<QString> tmpList;
        for (const auto& pair : dfaMinTable[i].transitions) {
            tmpList.append(QString(pair.first));
        }


        getLexCodeCase(tmpList, lexCode, num, 0);
        lexCode += "\t\t\tdefault: {\n";
        lexCode += "\t\t\t\tflag=true;\n";
        if (tmpList.contains("letter"))
            lexCode += "\t\t\t\tinId = true;\n";
        lexCode += "\t\t\t}\n"
            "\t\t\t}\n"
            "\t\t\tungetc(current_char, input_file);\n"
            "\t\t}\n";
    }
    lexCode += "\t}\n";

    //为适配解码增加Keyword:前缀，数字Digit:前缀，ID:标识符前缀 Operaotr:专用符号
    lexCode +=
        "\tif (isKeyWords(cur_line)) {\n"
        "\t\tint i ;for (i = 0; i < "+ QString::number(keywordsLexCode.size()) +"; i++) {\n"
        "\t\t\tif (strcmp(cur_line, keywordLexCode[i].op) == 0) {\n"
        "\t\t\t\tfprintf(output_file, \"%d \\n\", keywordLexCode[i].code);\n"
        "\t\t\t\tprintf(\"%d \\n\" , keywordLexCode[i].code);\n"
        "\t\t\t\treturn;}\n"
        "\t\t}\n"
        "\t}\n"

        "\tif (inId) {\n"
        "\t\tfprintf(output_file, \""+ QString::number(idIndex) + ":%s \\n\", cur_line);\n"
        "\t\tprintf(\""+ QString::number(idIndex) + ":%s \\n\", cur_line);\n"
        "\t\treturn;\n"
        "\t}\n"

        "\tif (inNum) {\n"
        "\t\tfprintf(output_file, \"" +  QString::number(numberIndex) +":%s \\n\", cur_line);\n"
        "\t\tprintf(\""+ QString::number(numberIndex) + ":%s \\n\", cur_line);\n"
        "\t\treturn;\n"
        "\t}\n"

        "\tif (!isInComment) {\n"
        "\t\tint i; for(i = 0; i < "+ QString::number(operatorsLexCode.size()) + "; i++){\n"
        "\t\t\tif (strcmp(cur_line, opLexCode[i].op) == 0) {\n"
        "\t\t\t\tfprintf(output_file, \"%d \\n\", opLexCode[i].code);\n"
        "\t\t\t\tprintf(\"%d \\n\", opLexCode[i].code);\n"
        "\t\t\t\treturn;\n"
        "\t\t\t}\n"
        "\t\t}\n"
        "\t}\n"
        "};\n";

    //主函数
    QFileInfo fileInfo(filePath);
    QString t_tmpFilePath = fileInfo.path();
    lexCode += "int main(int argc, char* argv[]) {\n"
        "\tFILE* input_file = fopen(\"" + filePath + "/sample." + sampleType + "\", \"r\");\n"
        "\tif (input_file == NULL) {\n"
        "\t\tprintf(\"Failed to open input file\");\n"
        "\t\treturn 1;\n"
        "\t}\n";

    lexCode +=
        "\tFILE* output_file = fopen(\"" + filePath + "/sample.lex" + "\", \"w\");\n"
        "\tif (output_file == NULL) {\n"
        "\t\tprintf(\"Failed to open output file\");\n"
        "\t\tfclose(input_file);\n"
        "\t\treturn 1;\n"
        "\t}\n";

    lexCode += "\tchar c;\n"
        "\twhile ((c=fgetc(input_file)) != EOF) {\n"
        "\t\tungetc(c, input_file);\n"
        "\t\tlexicalAnalysis(input_file, output_file);\n"
        "\t}\n"
        "\tfprintf(output_file, \"EOF:EOF\");\n"
        "\tprintf(\"EOF:EOF\");\n"
        "\tfclose(input_file);\n"
        "\tfclose(output_file);\n"
        "\treturn 0;\n"
        "}";

    return lexCode;
}


/*
* @brief 初始化
* 用于清空全局变量和所有框中的内容
*/
void clearGlobalVarible() {
    keyWords.clear();
    finalRegex.clear();
    commentLetters->clear();
    nodeCount = 0;
    nfaCharSet.clear();
    dfaCharSet.clear();
    nfaTable.clear();
    insertion_vec.clear();
    nfaStartSet.clear();
    nfaEndSet.clear();
    dfaStateSet.clear();
    dfaFinalSet.clear();
    dfaNonFinalStateSet.clear();
    dfaMinTable.clear();
    divideVector.clear();
    dfaMinMap.clear();
    dfaGraphMatrix.clear();
    nfaCharSet.insert(regexEpsilon); // 放入epsilon
}

/*
* @brief 清空table内容
*/
void cleartable(QTableWidget* table){
    table->clearContents(); // 清除表格中的数据
    table->setRowCount(0); // 清除所有行
    table->setColumnCount(0); // 清除所有列
    table->setAlternatingRowColors(true); //换行切换颜色
}

void cleartable(QTextBrowser* broswer){
    broswer->clear();
}
void cleartable(QPlainTextEdit* edit){
    edit->clear();
}

/*
* @brief 由nfatable展示NFA
*/
void showNFA(QTableWidget* nfatable)
{
    // 设置列数
    int colCnt = 2 + nfaCharSet.size();
    nfatable->setColumnCount(colCnt);

    map<char, int> headerCharNum;

    // 设置表头
    QStringList headerLabels;
    headerLabels << "标志" << "NFA节点编号";
    int headerCount = 3;
    for (const auto& ch : nfaCharSet) {
        if (char2string.find(ch) != char2string.end()) {
            headerLabels << QString::fromStdString(myTrim(char2string[ch]));
        }
        else {
            if (ch == '~') {
                //注释情况
                char tt = commentLetters[1][0];
                string commentEnd = commentLetters[1];
                if (char2string.find(tt) != char2string.end()) {
                    commentEnd = myTrim(char2string[tt]);
                }
                headerLabels << QString::fromStdString("注释内容，非" + commentEnd);
            }
            else {
                headerLabels << QString(ch);
            }
        }

        headerCharNum[ch] = headerCount++;
    }
    nfatable->setHorizontalHeaderLabels(headerLabels);

    // 设置行数
    int rowCount = nfaTable.size();
    nfatable->setRowCount(rowCount);

   // 表格数据
    int row = 0;
    for (auto id : insertion_vec) {
        const nfaTableNode& node = nfaTable[id];

        // Flag 列
        QString flagStr = "";
        if(node.isStart)flagStr += "-";  //初态
        if(node.isEnd)flagStr += "+"; //终态
        nfatable->setItem(row, 0, new QTableWidgetItem(flagStr));

        // ID 列
        nfatable->setItem(row, 1, new QTableWidgetItem(QString::number(node.id)));

        // TransitionChar 列
        int col = 2;
        for (const auto& transitionEntry : node.m) {
            string resutlt = set2Str(transitionEntry.second);

            // 放到指定列数据
            nfatable->setItem(row, headerCharNum[transitionEntry.first] - 1, new QTableWidgetItem(QString::fromStdString(resutlt)));
            col++;
        }

        row++;
    }

    // 调整列宽
    nfatable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


    // 显示表格
    nfatable->show();

}



/*
* @brief 由dfatable展示DFA
*/
void showDFA(QTableWidget* dfatable)
{

    // 设置列数
    int colCnt = 2 + dfaCharSet.size(); // 默认两列：Flag 和 状态集合
    dfatable->setColumnCount(colCnt);

    // 字符和第X列存起来对应
    map<char, int> headerCharNum;

    // 设置表头
    QStringList headerLabels;
    headerLabels << "标志" << "状态集合";
    int headerCount = 3;
    for (const auto& ch : dfaCharSet) {
        if (char2string.find(ch) != char2string.end()) {
            headerLabels << QString::fromStdString(myTrim(char2string[ch]));
        }
        else {
            if (ch == '~') {
                char tt = commentLetters[1][0];
                string res = commentLetters[1];
                if (char2string.find(tt) != char2string.end()) {
                    res = myTrim(char2string[tt]);
                }
                headerLabels << QString::fromStdString("注释内容，非" + res);
            }
            else {
                headerLabels << QString(ch);
            }
        }

        headerCharNum[ch] = headerCount++;
    }
    dfatable->setHorizontalHeaderLabels(headerLabels);
    // 设置行数
    int rowCount = dfaGraphMatrix.size();
    dfatable->setRowCount(rowCount);

    // 填充数据
    int row = 0;
    for (auto& dfaNode : dfaGraphMatrix) {

        // Flag 列
        QString flagStr;
        if(dfaNode.isStart)flagStr += "-";  //初态
        if(dfaNode.isEnd)flagStr += "+"; //终态
        dfatable->setItem(row, 0, new QTableWidgetItem(flagStr));

        // 状态集合 列
        dfatable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString("{" + set2Str(dfaNode.nfaStatesSet) + "}")));

        // 状态转换 列
        int col = 2;
        for (const auto& transitionEntry : dfaNode.transitions) {
            string re = set2Str(transitionEntry.second);

            // 放到指定列数据
            dfatable->setItem(row, headerCharNum[transitionEntry.first] - 1, new QTableWidgetItem(QString::fromStdString("{" + re + "}")));
            col++;
        }

        row++;
    }

    // 调整列宽
    dfatable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // 显示表格
    dfatable->show();
}

/*
* @brief 由mindfatable展示MinDFA
*/
void showMinDFA(QTableWidget* mindfatable)
{
    mindfatable->clearContents(); // 清除表格中的数据
    mindfatable->setRowCount(0); // 清除所有行
    mindfatable->setColumnCount(0); // 清除所有列

    // 设置列数
    int colCnt = 2 + dfaCharSet.size(); // 默认两列：Flag 和 状态集合
    mindfatable->setColumnCount(colCnt);

    // 字符和第X列存起来对应
    map<char, int> headerCharNum;

    // 设置表头
    QStringList headerLabels;
    headerLabels << "标志" << "dfa节点编号";
    int headerCount = 3;
    for (const auto& ch : dfaCharSet) {
        if (char2string.find(ch) != char2string.end()) {
            headerLabels << QString::fromStdString(myTrim(char2string[ch]));
        }
        else {
            if (ch == '~') {
                char tt = commentLetters[1][0];
                string res = commentLetters[1];
                if (char2string.find(tt) != char2string.end()) {
                    res = myTrim(char2string[tt]);
                }
                headerLabels << QString::fromStdString("注释内容，非" + res);
            }
            else {
                headerLabels << QString(ch);
            }
        }

        headerCharNum[ch] = headerCount++;
    }
    mindfatable->setHorizontalHeaderLabels(headerLabels);

    // 设置行数
    int rowCount = dfaMinTable.size();
    mindfatable->setRowCount(rowCount);

    // 填充数据
    int row = 0;
    for (auto& dfaNode : dfaMinTable) {

        // Flag 列
        QString flagStr;
        if(dfaNode.isStart)flagStr += "-";  //初态
        if(dfaNode.isEnd)flagStr += "+"; //终态
        mindfatable->setItem(row, 0, new QTableWidgetItem(flagStr));

        // 状态集合 列
        mindfatable->setItem(row, 1, new QTableWidgetItem(QString::number(dfaNode.id)));

        // 状态转换 列
        int col = 2;
        for (const auto& transitionEntry : dfaNode.transitions) {
            // 放到指定列数据
            mindfatable->setItem(row, headerCharNum[transitionEntry.first] - 1, new QTableWidgetItem(transitionEntry.second == -1 ? QString::fromStdString("") : QString::number(transitionEntry.second)));
            col++;
        }

        row++;
    }

    // 调整列宽
    mindfatable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // 显示表格
    mindfatable->show();
}

// 输出提示信息
void warnmsg(QString warnStr){
    QMessageBox warningBox;
    warningBox.setWindowTitle("警告");
    warningBox.setText(warnStr);
    warningBox.setIcon(QMessageBox::Warning);
    warningBox.setStandardButtons(QMessageBox::Ok);

    // 显示警示框
    warningBox.exec();
    return;
}

/*
* @brief 开始分析
*/
void Widget::on_pushButton_clicked(){
    // 清空全局变量
    clearGlobalVarible();

    //清空所有控件内容
    cleartable(ui->NFAtable);
    cleartable(ui->DFAtable);
    cleartable(ui->minDFAtable);
    cleartable(ui->codeBrowser);
    cleartable(ui->sampleEdit);
    cleartable(ui->lexBrewser);
    cleartable(ui->lexCodeBrowser);

    // 拿到所有的正则表达式
    QString input = ui->plainTextEdit_2->toPlainText();

    isLowerCase = ui->checkBox->isChecked();
    qDebug() <<"是否区分大小写："<< isLowerCase;

    string result = processInput(input, isLowerCase);
    // 如果字符串不为空就是报错了，退出
    if (!result.empty()) {
        QMessageBox::critical(this, "错误信息", QString::fromStdString(result));
        return;
    }

    //正则表达式转换成NFA图
    nfaGraph nfa = regex2NFA(finalRegex);
    //展示nfa表格
    showNFA(ui->NFAtable);

    // NFA转DFA
    NFA2DFA(nfa);
    showDFA(ui->DFAtable);

    DFAminimize();
    showMinDFA(ui->minDFAtable);

    QMessageBox::about(this, "提示", "分析成功！请点击其余按钮查看结果！");
}


/*
* @brief 词法分析器代码生成提示
*/
void msgLEX(){
    QMessageBox msgBox;
    msgBox.setText("请先选择词法分析程序的保存路径，保存后，请将sample文件保存到该路径");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    if (msgBox.exec() != QMessageBox::Ok) {
        // 用户点击了"取消"，直接返回
        return;
    }
}


// lexer.c文件的保存路径
QString lexerPath;

/*
* @brief 生成词法分析代码
*/
void Widget::on_pushButton_2_clicked()
{
    lexerPath.clear();
    msgLEX(); //给出交互提示
//    QString srcFilePath;
    QString t_filePath=QFileDialog::getExistingDirectory(this,QDir::currentPath());

    if(t_filePath.isEmpty())
        return;
    else
        lexerPath=t_filePath;

    QString type = getUserInput();//让用户输入sample文件后缀
    if(type == QString()){
        // 用户没有输入，报错
        warnmsg("请输入正确的文件后缀！");
        return;
    }

    QString res = generateLexerCode(lexerPath, type);//生成词法分析程序
    ui->codeBrowser->setText(res); //展示源码

    qDebug() << "lexCode complete!";

    //保存词法分析程序源码
    QFile tgtFile(lexerPath+"/lexer.c");
    if(!tgtFile.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Truncate))
    {
        QMessageBox::warning(NULL, "文件", "文件打开/写入失败");
        return;
    }
    QTextStream outputFile(&tgtFile);
    outputFile<< res;
    tgtFile.close();
}

// 编译运行词法分析程序 lexer.c，路径为已经选定的lexerPath
void Widget::on_pushButton_3_clicked()
{
    if(lexerPath.isEmpty()){
        //还没有生成词法分析代码
        warnmsg("还没有生成词法分析代码，请先进行分析！");
        return;
    }
    qDebug() << lexerPath;

    QTextBrowser* lexBrowser = this->ui->lexBrewser;
    lexBrowser->clear();


    //设置生成的可执行文件路径
    QString exePath =lexerPath + "/lexer.exe";
    QString cFilePath = lexerPath + "/lexer.c";

    // 调用编译器（g++）进行编译
    QProcess compiler;
    QStringList arguments;
    arguments << cFilePath << "-o" << exePath; // g++ temp.cpp -o temp.exe

    compiler.start("g++", arguments);
    if (!compiler.waitForFinished()) {
        lexBrowser->setText("错误：编译器执行失败");
        return;
    }

    // 获取编译器输出
    QString compileOutput = compiler.readAllStandardOutput();
    QString compileError = compiler.readAllStandardError();

    // 显示编译结果
    if (!compileError.isEmpty()) {
        lexBrowser->setText("编译错误：\n" + compileError);
    }

    //运行生成的可执行文件并捕获其输出
    QProcess program;
    program.setProcessChannelMode(QProcess::MergedChannels);  // 合并标准输出与标准错误流
    program.setReadChannel(QProcess::StandardOutput);          // 设置读取标准输出流
    program.start(exePath);
    if (!program.waitForFinished()) {
        warnmsg("运行失败！");
        return;
    }
    QString programOutput = program.readAllStandardOutput();
    lexBrowser->setText(programOutput);

    //保存lex文件
    QString lexFilePath = lexerPath + "/sample.lex";
    QFile file(lexFilePath);
    // 使用WriteOnly模式确保内容被写入
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "无法打开文件:" << lexFilePath;
        return;
    }

    QTextStream out2(&file);
    out2 << programOutput; // 将QString内容写入文件
    file.close();

    QMessageBox::about(this, "提示", "词法分析程序运行成功!请选择\"lex\"选项查看词法分析结果！");


    //展示lex编码表
    ui->lexCodeBrowser->clear();
    QString displayText;
    // 遍历 QMap，将所有的键值对转化为字符串
    for (auto it = lexcode2str.begin(); it != lexcode2str.end(); ++it) {
        displayText.append(QString("Lex Code: %1, String: %2\n")
                           .arg(it.key())  // 键（lex编码）
                           .arg(it.value()));  // 值（对应字符串）
    }

    // 假设 ui->lexCodeBrowser 是 QTextBrowser 控件
    ui->lexCodeBrowser->setText(displayText);


}



/*
* @brif 上传正则表达式文件
*/
void Widget::on_pushButton_6_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::homePath(), tr("文本文件 (*.txt);;所有文件 (*.*)"));

    if (!filePath.isEmpty())
    {
        ifstream inputFile;
        QTextCodec* code = QTextCodec::codecForName("GB2312");

        string selectedFile = code->fromUnicode(filePath.toStdString().c_str()).data();
        inputFile.open(selectedFile.c_str(), ios::in);
        if (!inputFile) {
            QMessageBox::critical(this, "错误信息", "导入错误！无法打开文件，请检查路径和文件是否被占用！");
            cerr << "Error opening file." << endl;
        }
        // 读取文件内容并显示在 plainTextEdit_2
        stringstream buffer;
        buffer << inputFile.rdbuf();
        QString fileContents = QString::fromStdString(buffer.str());
        ui->plainTextEdit_2->setPlainText(fileContents);
    }

}
/*
* @brif 保存正则表达式文本
*/
void Widget::on_pushButton_7_clicked()
{
    // 保存结果到文本文件
    QString saveFilePath = QFileDialog::getSaveFileName(this, tr("保存结果文件"), QDir::homePath(), tr("文本文件 (*.txt)"));
    if (!saveFilePath.isEmpty() && !ui->plainTextEdit_2->toPlainText().isEmpty()) {
        QFile outputFile(saveFilePath);
        if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&outputFile);
            stream << ui->plainTextEdit_2->toPlainText();
            outputFile.close();
            QMessageBox::about(this, "提示", "导出成功！");
        }
    }
    else if (ui->plainTextEdit_2->toPlainText().isEmpty())
    {
        QMessageBox::warning(this, tr("提示"), tr("输入框为空，请重试！"));
    }
}


/*
* @brief 保存sample文件
*/
void Widget::on_saveSampleButton_clicked()
{
    QString defaultFileName = "sample"; // 默认文件名
    QString filter = tr("Tny files (*.tny);;MiniC files (*.minic)"); // 限制文件类型

    // 设置默认文件名和文件类型过滤器
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
        QDir::currentPath() + "/" + defaultFileName, filter);

    if(fileName.isEmpty()) {
        QMessageBox::warning(this, "警告", "未输入文件名");
    } else {
        qDebug() << "Selected file:" << fileName << endl;
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QMessageBox::about(NULL, "文件", "文件无法打开");
        } else {
            QString fileContent = ui->sampleEdit->toPlainText();
            QTextStream out(&file);
            out.setCodec("UTF-8");
            out << fileContent;
            file.close();
        }
    }
}


/*
* @brief 打开sample文件
*/
void Widget::on_openSampleButton_clicked()
{
    QString defaultFileName = "sample"; // 默认文件名
    QString filter = tr("Tny files (*.tny);;MiniC files (*.minic)"); // 限制文件类型


    // 设置默认文件名和文件类型过滤器
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
        QDir::currentPath() + "/" + defaultFileName, filter);

    if(fileName.isEmpty()){
        QMessageBox::warning(this, "警告", "请选择一个文件");
    }
    else{
        qDebug() << fileName << endl;
        QFile file(fileName); // 打开文件
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QMessageBox::about(NULL, "文件", "文件无法打开");
        }
        QTextStream in(&file);
        in.setCodec("UTF-8"); // 设置编码为UTF-8
        QString fileContent = in.readAll();
        qDebug() << fileContent;
        ui->sampleEdit->setPlainText(fileContent);
        file.close();
    }
}

/*
* @brief 打开lex文件
*/
void Widget::on_openLexButton_clicked()
{
    QString defaultFileName = "output.lex"; // 默认文件名更改为output.lex
    QString filter = tr("Lex files (*.lex)"); // 限制文件类型为.lex

    // 设置默认文件名和文件类型过滤器
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
        QDir::currentPath() + "/" + defaultFileName, filter);

    if(fileName.isEmpty()){
        QMessageBox::warning(this, "警告", "请选择一个文件");
    }
    else{
        qDebug() << fileName << endl;
        QFile file(fileName); // 打开文件
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QMessageBox::about(NULL, "文件", "文件无法打开");
        }
        else {
            QTextStream in(&file);
            in.setCodec("UTF-8"); // 设置编码为UTF-8
            QString fileContent = in.readAll();
            qDebug() << fileContent;
            ui->lexBrewser->setPlainText(fileContent);
            file.close();
        }
    }
}

minDfaNode::minDfaNode() {
    this->isEnd = false;
    this->isStart = false;
}


// int类型的二维数组，判断 [rule.id][rule.right]的编码
QVector<QVector<int>> treeAction;

// 全局文法变量
string grammarStr;

struct myrule{
    QString left;
    QVector<QString> right;
    int rid; // 规则编号
    //( exp )  right有3个元素：(,exp,)
    myrule(QString l,  QVector<QString> r){
        left = l, right = r;
    }
    myrule(){
        left = "";
        QVector<QString> r;
        right = r;
    }
};
QVector<myrule> myrules;
QSet<QString> terminal, nonterminal;


// 是否是文法规则中的运算符，如果是，则一定为终结符
bool isOperator(QString letter){
    QSet<QString> opSet = {"+", "-", "*", "/", "%", "^", "(",")",
                           "<", ">", "=", ":=", "<=", ">=", "!=", "<>",
                           "==", ";", "[", "]", "{", "}", ","};
    for(QString op : opSet){
        if(op == letter)
            return true;
    }
    return false;
}

// 是否是字母
bool isalpha(QChar ch){
    if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
        return true;
    else return false;
}
//是否是数字
bool isdigit(QChar ch){
    if(ch >= '0' && ch <= '9')
        return true;
    return false;
}

// 文法中的每一项是否是一个标识符
// 本人定义的标识符：只能由字母和'-'组成
bool isIdentifier(QString letter){
    for(QChar ch : letter){
        if(!isalpha(ch) && ch != "-")
            return false;
    }
    return true;
}


/*
* @brif 预处理，在特定符号两端添加空格
*/
void preProcess(){
    // 找到并替换 replacedSymbols 中的符号，在两端添加空格
    QSet<QString> replacedSymbols = {"->", "|", "(", ")", "[", "]", "{", "}" };
    for(QString reSym : replacedSymbols){
        size_t pos = 0;
        while ((pos = grammarStr.find(reSym.toStdString(), pos)) != std::string::npos) {
            string replaceStr = " " + reSym.toStdString() +" ";
            if(reSym.size() == 2)
                grammarStr.replace(pos, 2, replaceStr);
            else
                grammarStr.replace(pos, 1, replaceStr);
            pos += 4;  // 移动到新的位置，避免无限循环
        }
    }
}
void processInput(QTextBrowser* broswer){
    vector<string> lines;
    istringstream iss(grammarStr);
    string line;

    // 防止中间有换行符
    while (getline(iss, line))
    {
        if (!line.empty())
        {
            lines.push_back(line);
        }
    }

    for (const auto& rule : lines){
        bool isfirst = true;
        istringstream is(rule);
        string s;
        QVector<QString> tempright;
        QString templeft;
        while(is >> s){
            QString letter =  QString::fromStdString(s);
            if(letter == '|'){
                //遇到多个规则了，需要将|拆开
                //先保存上一段规则
                myrules.push_back(myrule(templeft, tempright));
                //清空，准备重新收集右边规则
                tempright.clear();
            }
            else if(isfirst){
                //开头的第一个，一定是非终结符
                nonterminal.insert(letter);
                if(terminal.contains(letter))terminal.remove(letter); // 如果被判定为终结符，则要删除这个符号
                templeft = letter;
                isfirst = false;
                //跳过箭头
                is.ignore(numeric_limits<streamsize>::max(), '-');
                is.ignore(numeric_limits<streamsize>::max(), '>');
            }
            else if(isOperator(letter)){
                //如果是运算符号的话
                tempright.push_back(letter);
                terminal.insert(letter); //终结符
            }
            else if(isIdentifier(letter)){
                //遇到标识符，先当成终结符
                if(!nonterminal.contains(letter))
                    terminal.insert(letter);
                tempright.push_back(letter);
            }
            else if(letter == "@"){
                //遇到空串epsilon
                tempright.push_back(letter);
            }
        }
        //一行遍历完毕，保存规则。
        myrules.push_back(myrule(templeft, tempright));
    }
    //默认新增一个文法开始符号
    QVector<QString> tempright;
    tempright.push_back(myrules[0].left);
    myrule newrule(QString("S'"), tempright);
    myrules.insert(0, newrule);
    nonterminal.insert(myrules[0].left);

    QString alltext = "";
    for(int i = 0; i < myrules.size(); i++){
        myrules[i].rid = i;
        QString showline = QString::number(i)  + ':' + myrules[i].left + " -> ";
        for(QString letter : myrules[i].right){
            showline += letter + ' ';
        }
        alltext += showline + '\n';
    }

    broswer->setText(alltext);
}

QMap<QString, QSet<QString>> firstmap;
QMap<QString, QSet<QString>> followmap;

QSet<QString> getFirst(QString symbol){
    if(nonterminal.contains(symbol))
        //如果是非终结符，那么返回所需要的first集合
        return firstmap[symbol];
    else return QSet<QString>{symbol};//如果是终结符，返回符号自身
}

QMap<QString, QVector<myrule>> formula;

void generate_formula(){
    //对文法进行预处理,产生formula
    for(int i = 0; i < myrules.size(); i++){
        //如果规则左边相同，放到一起
        QString templeft = myrules[i].left;
        QVector<myrule> tempright;
        if(i == myrules.size() - 1){
            //如果走到最后一个了，存了就退出
            tempright.push_back(myrules[i]);
            formula[templeft] = tempright;
            break;
        }
        if(myrules[i].left == myrules[i + 1].left){
            tempright.push_back(myrules[i]);
            while(myrules[i].left == myrules[i + 1].left){
                tempright.push_back(myrules[i + 1]);
                i++;
                if(i == myrules.size() - 1)break;//到头了，退出
            }
        }
        else{
            //没有一样的
            tempright.push_back(myrules[i]);
        }
        //添加到formula
        formula[templeft] = tempright;
    }
}

void initFirst(){
    //对文法进行预处理,产生formula
    generate_formula();

    bool shouldupdate = true;

    while(shouldupdate){
        shouldupdate = false;
        for(QString key : formula.keys()){
            for(myrule rule : formula[key]){
                int cur;
                for(cur = 0; cur < rule.right.size(); cur++){
                    QString rightitem = rule.right[cur];//rightitems就是Y_k
                    QSet<QString> firstSetOfrightitem = getFirst(rightitem);
                    for(QString firstSymbol : firstSetOfrightitem){
                        if(rightitem == '@' || firstmap[key].contains(firstSymbol)){
                            //如果包含epsilon或者已经有这个符号,不用处理
                            continue;
                        }
                        else{
                            //否则说明发生变化，要重新遍历，并将符号插入
                            firstmap[key].insert(firstSymbol);
                            shouldupdate = true;
                        }
                    }
                    if(!firstSetOfrightitem.contains("@")){
                        //如果没有epsilon，那么不需要看Y_k+1了
                        //有epsilon才需要接着看下去
                        break;
                    }
                }
                if(cur == rule.right.size() && !firstmap[key].contains("@")){
                    //这一步处理两种情况：
                    //1.X -> Y_1Y_2...Y_n, 全部遍历完了，每个的first都有epsilon，就要把epsilon加进来
                    //2.文法本身就会产生epsilon
                    firstmap[key].insert("@");
                    shouldupdate = true;
                }
            }
        }
    }

}

void showFirst(QTableWidget* table){
    table->setColumnCount(2);
    QStringList header;
    header << "非终结符" << "First集合";
    table->setHorizontalHeaderLabels(header);
    //设置行数
    table->setRowCount(firstmap.size());
    int row = 0;

    for(auto iter = firstmap.begin(); iter != firstmap.end(); iter++){
        QString key = iter.key();
        QSet<QString> firstset = firstmap[key];
        //key是非终结符，firstset是对应的first集合
        table->setItem(row, 0, new QTableWidgetItem(key));


        QString firstsetString;
        for(QString firstletter : firstset){
            firstsetString += firstletter + ",";
        }
        if(!firstset.isEmpty())
            firstsetString.chop(1);
        table->setItem(row, 1, new QTableWidgetItem(firstsetString));
        row++;
    }
    //动态调整列宽
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}



void initFollow(){
    QString start = myrules[0].left;
    followmap[start].insert("$"); //匹配符号
    bool needUpdate = true;
    while (needUpdate) {
        needUpdate = false;
        for(auto iter = formula.begin(); iter != formula.end(); iter++){
            QString key = iter.key();
            for(myrule rule : formula[key]){
                //假设处理文法A->X1X2X3...Xn
                QVector<QString> ruleright = rule.right;
                for(int i = 0; i < ruleright.size(); ++i){
                    //开始处理规则推导式右边的每一个符号
                    QString Xi = ruleright[i];
                    if(i == ruleright.size() - 1){
                        //如果i走到了推导式最后面(Xn)，follow(Xn)=follow(A)
                        QSet<QString> followOfCur = followmap[key];
                        for(QString symbol : followOfCur){
                            if(followmap[Xi].contains(symbol))
                                continue;
                            followmap[Xi].insert(symbol);
                            needUpdate = true;
                        }
                         continue;
                    }
                    //没有遍历到最后，假设遍历到Xi
                    QSet<QString> firstOfBehind; //follow(Xi+1Xi+2...Xn)
                    int cur = i + 1;
                    for(; cur < ruleright.size(); ++cur){
                        //从i+1开始遍历Xi+1Xi+2...Xn,遍历到的下标记作cur
                        QSet<QString> firstOfCur = getFirst(ruleright[cur]); //first(Xcur)
                        firstOfBehind.unite(firstOfCur);//first(Xcur)全部加到follow(Xi+1Xi+2...Xn)中
                        firstOfBehind.remove("@"); //删除follow(Xi+1Xi+2...Xn)中的epsilon
                        if(!firstOfCur.contains("@")) //如果Xcur不会生成epsilon，结束，不用再往后遍历了
                            break;
                    }
                    //如果cur走到了最后，说明First(Xi+1Xi+2...X_n)含有epsilon，那么把epsilon加入进来
                    if(cur == ruleright.size())
                        firstOfBehind.insert("@");
                    //把follow(Xi+1Xi+2...Xn)加入到follow(Xi)中
                    for(QString symbol : firstOfBehind){
                        //如果follow(Xi)已经有了，或symbol是epsilon，那么久不添加了
                        if(followmap[Xi].contains(symbol) || symbol == "@")
                            continue;
                        followmap[Xi].insert(symbol);
                        needUpdate = true;
                    }
                    //如果First(Xi+1Xi+2...X_n)有epsilon，那么把follow(A)添加到follow(Xi)中
                    if(firstOfBehind.contains("@")){
                        for(QString s : followmap[key]){
                            if(followmap[Xi].contains(s))continue;
                            followmap[Xi].insert(s);
                            needUpdate = true;
                        }
                    }

                }
            }
        }
    }
}


void showFollow(QTableWidget* table){
    table->setColumnCount(2);
    QStringList header;
    header << "非终结符" << "Follow集合";
    table->setHorizontalHeaderLabels(header);
    //设置行数
    table->setRowCount(nonterminal.size());
    int row = 0;


    for(auto iter = nonterminal.begin(); iter != nonterminal.end(); iter++){
        QString key = *iter;
        QSet<QString> followset = followmap[key];
        table->setItem(row, 0, new QTableWidgetItem(key));
        QString followsetString;
        for(QString followletter : followset){
            followsetString += followletter + ",";
        }
        if(!followset.isEmpty())
            followsetString.chop(1);
        table->setItem(row, 1, new QTableWidgetItem(followsetString));
        row++;
    }
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

QVector<QString> transsymbols; //转移符号数组


void generateTransSymbols(){
    for(QString symbol : terminal)
        transsymbols.push_back(symbol);
    for(QString symbol : nonterminal)
        transsymbols.push_back(symbol);
    //去除开始符号
    QString start = myrules[0].left;
    transsymbols.removeOne(start);
}

int getIdBySymbol(QString symbol){
    for(int i = 0; i < transsymbols.size(); i++){
        QString trans = transsymbols[i];
        if(trans == symbol)
            return i;
    }
    return -1;
}

//LR0状态
struct LR0item{
    int rid; //文法规则id
    int rindex; //小数点位置：在第几个字符之前
    bool operator==(const LR0item& item){
        if(rid == item.rid && rindex == item.rindex)
            return true;
        else return false;
    }
    LR0item(int id, int index){
        rid = id; rindex = index;
    }
};

using LR0State = QVector<LR0item>;
using LR0dfa = QVector<LR0State>;


bool stateContains(const LR0State state, const LR0item newitem){
    for(LR0item item : state){
        if(item == newitem)
            return true;
    }
    return false;
}

//求LR0状态的闭包
void LR0closure(LR0State& state){
    bool needUpdate = true;
    while(needUpdate){
        needUpdate = false;
        for(int i = 0; i < state.size(); i++){
            LR0item item = state[i];
            //遍历这个状态下的所有项目A -> αBβ
            QString ruleleft = myrules[item.rid].left; //A
            QVector<QString> ruleright = myrules[item.rid].right;//αBβ
            if(item.rindex == ruleright.size()){
                //A -> αBβ. 走到最后了，跳过
                continue;
            }
            else if(ruleright.size() == 1 && ruleright[0] == "@"){
                //A -> @ 跳过
                continue;
            }
            //A -> α.Bβ
            QString nextsymbol = ruleright[item.rindex]; //B
            if(terminal.contains(nextsymbol))
                continue; //B是终结符，跳过
            //B不是终结符
            for(myrule rule : myrules){
                if(rule.left == nextsymbol){
                    //对于每一个 B -> γ
                    //暂时生成一个新item：B -> .γ
                    LR0item tempitem(rule.rid, 0);
                    if(!stateContains(state, tempitem)){
                        //如果状态state不包含B -> .γ，则将其加入
                        state.push_back(tempitem);
                        needUpdate = true;
                    }
                }
            }
        }
    }
}


//using LR0State = QVector<LR0item>;
//using LR0dfa = QVector<LR0State>;
//QMap<int, QMap<QString, int>> forwards; //状态之间的移进关系
QMap<int, QMap<QString, int>> forwards; //移进关系
//QMap<int, QMap<QString, int>> backwards; //规约关系,第二个int存储的是文法编号

int isNewLR0State(LR0dfa dfa, LR0State newstate){
    //判断是否为dfa图中的新状态
    for(int i = 0; i < dfa.size(); i++){
        LR0State curstate = dfa[i];
        bool flag = true;
        if(newstate.size() == curstate.size()){
            for(LR0item curitem : curstate){
                if(!count(newstate.begin(), newstate.end(), curitem)){
                    //如果newstate不包含了dfa图其中一个state的所有item
                    flag = false;
                }
            }
            if(flag)return i;
        }

    }
    return -1;
}


void testLR0Closure(){
    LR0State teststate;
    LR0item startitem(0, 0);
    teststate.push_back(startitem);
    LR0closure(teststate);

}



LR0dfa generateLR0dfa(){
    LR0dfa dfa;
    //处理开始符号
    LR0State startstate;
    LR0item startitem(0, 0); //文法开始符号的先行符号必为$
    startstate.push_back(startitem);
    dfa.push_back(startstate);
    for(int cur = 0; cur < dfa.size(); cur++){ //cur是当前遍历到的state的编号
        LR0State& curstate = dfa[cur];
        LR0closure(curstate); //求闭包处理
        QMap<QString, QVector<LR0item>> tempforward; //临时存储移进符号以及移进的item
        //遍历状态上的每一个项目
        for(int it = 0; it < curstate.size(); it++){
            LR0item item = curstate[it]; //取出这个项目
            QString key = myrules[item.rid].left;

            if(item.rindex == myrules[item.rid].right.size()){
                //如果项目已经到达末尾，是规约关系
                //不用进行判断
            }
            else{
                //没有到达末尾，是移进状态
                QString raw = myrules[item.rid].right[item.rindex]; //取出这个转移符号
                LR0item tempitem(item.rid, item.rindex); //临时存储这些转移到状态
                tempforward[raw].push_back(tempitem);//把状态暂时存起来
            }

        }
        for(QString raw : tempforward.keys()){
            //取出每个转移符号，对相同的转移符号的item存储到tempstate中
            LR0State tempstate;
            for(LR0item item : tempforward[raw]){
                tempstate.push_back(LR0item(item.rid, item.rindex + 1)); //小数点往后移动一位存入tempstate
            }
            //判断这个移进关系是否已经存在
            if(forwards[cur].contains(raw)){
                //如果存在，
                int target = forwards[cur][raw];
                //查看移进的那个dfa状态中是否包含这个curitem
                for(LR0item item : tempstate){
                    LR0item newitem(item.rid, item.rindex);
                    LR0State nextState = dfa[target];
                    if(!count(nextState.begin(), nextState.end(), newitem)){
                        //这个dfa状态如果不包含curitem，把它加到这个状态里面
                        dfa[target].push_back(newitem);
                    }
                    continue;
                }

            }
            else{
            //如果不存在这个转移关系，则把raw对应的item全部放到一个新state中
                LR0State perhapsNewState;

                for(LR0item newitem : tempstate){
                    perhapsNewState.push_back(newitem);
                }
                //优先判断特殊情况：
                //只有一个A-> @. : 不产生新的state
                if(perhapsNewState.size() == 1 && myrules[perhapsNewState[0].rid].right.size() == 1
                        && myrules[perhapsNewState[0].rid].right[0] == "@" && perhapsNewState[0].rindex == 1)
                    continue;

                LR0closure(perhapsNewState);
                //看dfa列表中有没有一样的dfa状态
                int target = isNewLR0State(dfa, perhapsNewState);
                if(target == -1){
                    //没有一样的的dfa状态,则加入到dfa列表中,并添加移进关系
                    dfa.push_back(perhapsNewState);
                    target = dfa.size() - 1;
                    forwards[cur][raw] = target;
                }
                else{
                    //有现成的dfa状态，添加移进关系
                    forwards[cur][raw] = target;
                }
            }
        }

    }

    return dfa;

}

//LR1的状态
struct LR1item{
    int rid; //文法规则id
    int rindex; //小数点位置：在第几个字符之前
    QSet<QString> lookaheadSet; //先行符号集
    bool operator==(const LR1item& item){
        if(rid == item.rid && rindex == item.rindex && lookaheadSet == item.lookaheadSet)
            return true;
        else return false;
    }
    LR1item(int id, int index, QSet<QString> newset){
        rid = id; rindex = index;lookaheadSet = newset;
    }
};

using LR1State = QVector<LR1item>;
using LR1dfa = QVector<LR1State>;

bool stateContainsLR1(const LR1State state, const LR1item newitem){
    for(LR1item item : state){
        if(item == newitem)
            return true;
    }
    return false;
}

bool itemSameCoreMergeLR1(LR1State& state, const LR1item newitem){
    //如果有核心一样的item,做合并,成功返回true,否则返回false
    for(LR1item& item : state){
        if(item.rid == newitem.rid
                && item.rindex == newitem.rindex
                && item.lookaheadSet != newitem.lookaheadSet)
        {
            //核心相同,做合并
            for(QString symbol : newitem.lookaheadSet){
                if(!item.lookaheadSet.contains(symbol)){
                    item.lookaheadSet.insert(symbol);
                }
            }
            return true;
        }
    }
    return false;
}

QMap<int, QMap<QString, int>> LR1forwards; //移进关系
QMap<int, QMap<QString, int>> LR1backwards; //规约关系,第二个int存储的是文法编号

//求LR1状态的闭包
void LR1closure(LR1State& state){
    bool needUpdate = true;
    while(needUpdate){
        needUpdate = false;
        for(int i = 0; i < state.size(); i++){
            LR1item item = state[i];

            //遍历这个状态下的所有项目A -> αBβ
            QString ruleleft = myrules[item.rid].left; //A
            QVector<QString> ruleright = myrules[item.rid].right;//αBβ
            //特殊情况：A -> @，不能由它生成新的item
            if(ruleright.size() == 1 && ruleright[0] == '@'){
                continue;
            }
            if(item.rindex == ruleright.size()){
                //A -> αBβ. 走到最后了，不会生成新状态，跳过
                continue;
            }
            else if(ruleright.size() == 1 && ruleright[0] == "@"){
                //A -> @ 不会生成新状态，跳过
                continue;
            }
            //A -> α.Bβ
            QString nextsymbol = ruleright[item.rindex]; //B
            if(terminal.contains(nextsymbol))
                continue; //B是终结符，不会由它产生新的item
            //B不是终结符
            for(myrule Brule : myrules){
                if(Brule.left == nextsymbol){
                    //对于每一个 B -> γ
                    //准备暂时生成一个新item：B -> .γ
                    //先获取tempitem的先行符号集
                    QSet<QString> newlookahead;
                    //B -> @, γ本身就是@,生成特殊的item B->.
//                    if(Brule.right.size() == 1 && Brule.right[0] == "@"){

//                        continue;
//                    }

                    if(item.rindex == ruleright.length() - 1){
                        //β为空,即为只有A -> αB,则照抄A -> αBβ的先行符号
                        newlookahead = item.lookaheadSet;
                    }
                    else{
                        //β不为空,求first(β)
                        //如果A -> α.Bβ1β2β3，如果first(β1)为空，先行符号 = 先行符号 ∪ (first(β1) - {@})，再对β2进行相同的处理
                        int nextnextindex = 1;
                        QString nextnextsymbol = ruleright[item.rindex + nextnextindex]; //β1
                        //newlookahead; //A -> α.Bβ1β2β3 的先行符号
                        QSet<QString> backfirst = getFirst(nextnextsymbol); //first(β1);

                        while(backfirst.contains("@") && nextnextindex + item.rindex < ruleright.size()){
                            //first(βi)有epsilon,且不超出数组边界(βi可以是最后一个)
                            backfirst.remove("@");
                            newlookahead = newlookahead.unite(backfirst); //先行符号 = 先行符号 ∪ (first(β1) - {@})
                            nextnextindex++;
                            if(nextnextindex + item.rindex == ruleright.size())break; //没有βi+1了,退出
                            nextnextsymbol = ruleright[item.rindex + nextnextindex]; //βi+1
                            backfirst = getFirst(nextnextsymbol);//first(βi+1)
                        }
                        if(nextnextindex + item.rindex == ruleright.size()){
                            //走到最后了,先行符号 = 先行符号 ∪ A -> α.Bβ1β2β3 的先行符号
                            newlookahead = newlookahead.unite(item.lookaheadSet);
                        }
                        else{
                            //没有走到最后，正常结束
                            newlookahead = newlookahead.unite(backfirst);
                        }
                    }

                    //生成新item
                    LR1item tempitem(Brule.rid, 0, newlookahead);
                    if(!stateContainsLR1(state, tempitem)){
                        //状态state不包含完全一样的B -> .γ, balabala
                        //如果有核心相同的item,做并集.修改
                        if(itemSameCoreMergeLR1(state, tempitem))continue;

                        //如果没有,创建新的
                        state.push_back(tempitem);
                        needUpdate = true;
                    }
                }
            }
        }
    }
}



//void testLR1Closure(){
//    LR1State teststate;
////    LR1item startitem(1, 1, QSet<QString> {"$"}); //A -> ( . A ) , $
//    LR1item startitem(1, 1, QSet<QString> {")"}); //A -> ( . A ) , $
//    teststate.push_back(startitem);
//    LR1closure(teststate);

//}


int isNewLR1State(LR1dfa dfa, LR1State newstate){
    //判断是否为dfa图中的新状态
    for(int i = 0; i < dfa.size(); i++){
        LR1State curstate = dfa[i];
        bool flag = true;
        if(newstate.size() == curstate.size()){
            for(LR1item curitem : curstate){
                if(!count(newstate.begin(), newstate.end(), curitem)){
                    //如果newstate不包含了dfa图其中一个state的所有item
                    flag = false;
                }
            }
            if(flag)return i;
        }

    }
    return -1;
}


LR1dfa generateLR1dfa(){
    LR1dfa dfa;
    //处理开始符号
    LR1State startstate;
    LR1item startitem(0, 0, QSet<QString>{"$"}); //文法开始符号的先行符号必为$
    startstate.push_back(startitem);
    dfa.push_back(startstate);
    for(int cur = 0; cur < dfa.size(); cur++){ //cur是当前遍历到的state的编号
        LR1State& curstate = dfa[cur];
        LR1closure(dfa[cur]); //求闭包处理
        QMap<QString, QVector<LR1item>> tempforward; //临时存储移进符号以及移进的item
        //遍历状态上的每一个项目
        for(int it = 0; it < curstate.size(); it++){
            LR1item item = curstate[it]; //取出这个项目
            QString key = myrules[item.rid].left;

            if(item.rindex == myrules[item.rid].right.size()){
                //如果项目已经到达末尾，是规约关系
                //不判断是否有follow集合的交集
                QSet<QString> followOfItem = followmap[key];
                for(QString element : followOfItem){
                    //对于一个E -> n. ,先看follow(E)中的每个元素都是dfa[cur]的一个规约
                    LR1backwards[cur][element] = curstate[it].rid; // 存储文法编号
                }
            }
            else{
                //没有到达末尾，是移进状态
                QString raw = myrules[item.rid].right[item.rindex]; //取出这个转移符号
                LR1item tempitem(item.rid, item.rindex, item.lookaheadSet); //临时存储这些转移到状态
                tempforward[raw].push_back(tempitem);//把状态暂时存起来
            }

        }
        for(QString raw : tempforward.keys()){
            //取出每个转移符号，对相同的转移符号的item存储到tempstate中
            LR1State tempstate;
            for(LR1item item : tempforward[raw]){
                tempstate.push_back(LR1item(item.rid, item.rindex + 1, item.lookaheadSet)); //小数点往后移动一位存入tempstate
            }
            //判断这个移进关系是否已经存在
            if(LR1forwards[cur].contains(raw)){
                //如果存在，
                int target = LR1forwards[cur][raw];
                //查看移进的那个dfa状态中是否包含这个curitem
                for(LR1item item : tempstate){
                    LR1item newitem(item.rid, item.rindex, item.lookaheadSet);
                    LR1State nextState = dfa[target];
                    if(!count(nextState.begin(), nextState.end(), newitem)){
                        //这个dfa状态如果不包含curitem，把它加到这个状态里面
                        dfa[target].push_back(newitem);
                    }
                    continue;
                }

            }
            else{
            //如果不存在这个转移关系，则把raw对应的item全部放到一个新state中
                LR1State perhapsNewState;

                for(LR1item newitem : tempstate){
                    perhapsNewState.push_back(newitem);
                }
                //优先判断特殊情况：
                //只有一个A-> @. : 不产生新的state
                if(perhapsNewState.size() == 1 && myrules[perhapsNewState[0].rid].right.size() == 1
                        && myrules[perhapsNewState[0].rid].right[0] == "@" && perhapsNewState[0].rindex == 1)
                    continue;

                LR1closure(perhapsNewState);
                //看dfa列表中有没有一样的dfa状态
                int target = isNewLR1State(dfa, perhapsNewState);
                if(target == -1){
                    //没有一样的的dfa状态,则加入到dfa列表中,并添加移进关系
                    dfa.push_back(perhapsNewState);
                    target = dfa.size() - 1;
                    LR1forwards[cur][raw] = target;
                }
                else{
                    //有现成的dfa状态，添加移进关系
                    LR1forwards[cur][raw] = target;
                }
            }
        }

    }
    return dfa;
}


QString getStringState(LR1State state){
    //由状态得到对应的字符串

    QString statestring;//所有项目组成的字符串
    for(LR1item item : state){
        myrule rule = myrules[item.rid];
        QString leftstring = rule.left; //[leftstring -> rightstrting, {aheadstring}]
        QString rightstring = "";
        int i; bool point = false;
        for(i = 0; i < rule.right.size(); i++){
            if(i == item.rindex){
                point = true;
                rightstring += ".";
            }
            rightstring += rule.right[i] + " ";
        }
        if(!point)rightstring += "."; //走到最后
        QString aheadstring = "{";
        for(QString ahead : item.lookaheadSet){
            aheadstring += ahead + "/";
        }
        if(!item.lookaheadSet.isEmpty())aheadstring.chop(1);
        aheadstring += "}";
        QString itemstring = "[" + leftstring + "->" + rightstring + " , " + aheadstring + "]";
        statestring += itemstring + "\n";
    }
    statestring.chop(1); //去掉最后的换行符
    return statestring;
}

//展示lr1dfa图
void showLR1dfa(LR1dfa dfa, QTableWidget* table){
    table->verticalHeader()->setVisible(false); //不显示行号
    //状态编号 文法 转移符号
    QStringList header;
    header << "状态编号" << "文法规则";
    for(QString symbol : transsymbols)
        header << symbol;
    table->setColumnCount(header.size());
    table->setHorizontalHeaderLabels(header);
    //设置行数
    table->setRowCount(dfa.size());
    for(int dfastateid = 0; dfastateid < dfa.size(); dfastateid++){
        LR1State state = dfa[dfastateid];
        table->setItem(dfastateid, 0, new QTableWidgetItem(QString::number(dfastateid)));
        table->setItem(dfastateid, 1, new QTableWidgetItem(getStringState(state)));

        for(auto outerit : LR1forwards){
            for(QString key : outerit.keys()){
                int col = getIdBySymbol(key);
                if(col == -1)continue; //如果是epsilon，则不会进行显示
                col += 2;
                int target = LR1forwards[dfastateid].value(key ,-1);
                if(target == -1)continue; //不存在
                table->setItem(dfastateid, col, new QTableWidgetItem(QString::number(target)));
            }
        }
    }

    //动态调整行宽和列宽
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

}

struct LALR1item{
    int rid;
    int rindex;
    QSet<QString> lookahead;
    LALR1item(int id, int index, QSet<QString> newset){
        rid = id; rindex = index; lookahead = newset;
    }
    LALR1item(int id, int index){
        rid = id; rindex = index;
    }
};

using LALR1State = QVector<LALR1item>;
using LALR1dfa = QVector<LALR1State>;

bool hasItemByCore(LR1State lr1state, LALR1item lalr1item){
    //判断一个lr1state的核心是否包含一个lalr1item
    for(LR1item lr1item : lr1state){
        if(lr1item.rid == lalr1item.rid && lr1item.rindex == lalr1item.rindex)
            return true;
    }
    return false;
}


bool isSameCoreStateLALRandLR1(LALR1State lalrstate, LR1State lr1state){
    //判断一个lalrstate和lr1state的核心是否相同
    bool flag = true;
    if(lalrstate.size() == lr1state.size()){
        for(LALR1item lalr1item : lalrstate){
            if(!hasItemByCore(lr1state, lalr1item)) // 如果lr1state中有一个item的核心与lalr11item不一致，则核心不一致
                flag = false;
        }
        if(flag)return true;
    }
    return false;
}

LALR1State LR0state2LALRstate(LR0State lr0state){
    //由一个LR0state转变为核心相同的LALRstate，默认没有先行符号
    LALR1State lalr1state;
    for(LR0item lr0item : lr0state){
        LALR1item lalr1item(lr0item.rid, lr0item.rindex); //创建没有先行符号的lalrstate
        lalr1state.push_back(lalr1item);
    }
    return lalr1state;

}

void mergeLookForwards(LR1State lr1state, LALR1State& lalr1state){
    //为先行符号做并集
    for(int lr1index = 0; lr1index < lr1state.size(); lr1index++){
        for(int lalr1index = 0; lalr1index < lalr1state.size(); lalr1index++){
            LR1item lr1item = lr1state[lr1index];
            LALR1item& lalr1item = lalr1state[lalr1index];

            if(lr1item.rid == lalr1item.rid
                    && lr1item.rindex == lalr1item.rindex){
                //找到核心相同的item
                lalr1item.lookahead = lalr1item.lookahead.unite(lr1item.lookaheadSet);
            }
        }
    }
}

LALR1dfa mergeLR0andLR1ToLALR(LR1dfa lr1dfa, LR0dfa lr0dfa){
    LALR1dfa lalr1dfa;
    //遍历lr0所有的state，有一个算一个，加到lalrdfa里面
    for(LR0State lr0state : lr0dfa){
        LALR1State  lalr1state =  LR0state2LALRstate(lr0state);
        //对于每一个state，找到lr1中core相同的state，合并先行符号集
        for(LR1State lr1state : lr1dfa){
            if(isSameCoreStateLALRandLR1(lalr1state, lr1state)){
                //核心一致，对先行符号集进行合并
                mergeLookForwards(lr1state, lalr1state);
            }
        }
        lalr1dfa.push_back(lalr1state);
    }
    //LALR的移进关系forwards与LR0相同, 不用管
    return lalr1dfa;
}

//展示lalr1dfa图
void showLALR1dfa(LALR1dfa lalr1dfa, QTableWidget* table){
    table->verticalHeader()->setVisible(false); //不显示行号
    //状态编号 文法 转移符号
    QStringList header;
    header << "状态编号" << "文法规则";
    for(QString symbol : transsymbols){
        header << symbol;
    }
    table->setColumnCount(header.size());
    table->setHorizontalHeaderLabels(header);
    table->setRowCount(lalr1dfa.size());
    for(int dfastateid = 0; dfastateid < lalr1dfa.size(); dfastateid++){
        LALR1State state = lalr1dfa[dfastateid];
        table->setItem(dfastateid, 0, new QTableWidgetItem(QString::number(dfastateid)));
        LR1State templr1state;
        for(LALR1item item : state){
            LR1item tempitem(item.rid, item.rindex, item.lookahead);
            templr1state.push_back(tempitem);
        }
        table->setItem(dfastateid, 1, new QTableWidgetItem(getStringState(templr1state)));

        for(auto outerit : forwards){
            //使用LR0的forwards
            for(QString key : outerit.keys()){
                int col = getIdBySymbol(key);
                if(col == -1)continue; //epsilon不显示
                col += 2;
                int target = forwards[dfastateid].value(key, -1);
                if(target == -1)continue;
                table->setItem(dfastateid, col, new QTableWidgetItem(QString::number(target)));
            }
        }
    }

    //动态调整行宽和列宽
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}


//展示lalr1分析表
void showLALR1AnalyzeTable(LALR1dfa dfa,QTableWidget* table){
    table->verticalHeader()->setVisible(false); //不显示行号
    QStringList header;
    header << "状态编号" << "Action";

    for(int i = 0; i < nonterminal.size() - 1; i++){
        header << " ";
    }
    header << "Goto";
    for(int i = 0; i < terminal.size() - 1; i++){
        header << " ";
    }


    QVector<QString> analyzeSymVec;
    for(QString symbol : terminal){
        analyzeSymVec.push_back(symbol);
    }
    analyzeSymVec.push_back("$");
    for(QString symbol : nonterminal){
        if(symbol == myrules[0].left)continue; //不添加文法开始符号
        analyzeSymVec.push_back(symbol);
    }
    table->setColumnCount(analyzeSymVec.size() + 1);
    table->setRowCount(dfa.size() + 1);
    table->setHorizontalHeaderLabels(header);

    for(int colindex = 0; colindex < analyzeSymVec.size(); colindex++){
        table->setItem(0, colindex + 1, new QTableWidgetItem(analyzeSymVec[colindex]));
    }

    for(int dfastateid = 0; dfastateid < dfa.size(); dfastateid++){
        //遍历所有状态
        table->setItem(dfastateid + 1, 0, new QTableWidgetItem(QString::number(dfastateid)));
        LALR1State state = dfa[dfastateid];  //获取当前state

        //移进item
        for(QMap<QString, int> outerid : forwards){
            for(QString key : outerid.keys()){
                //获取转移关系的key
                int forwardStateId = analyzeSymVec.indexOf(key); //转移符号对应的下标
                int target = forwards[dfastateid].value(key, -1);
                if(target == -1)continue; //不存在这个转移关系
                else{
                    if(nonterminal.contains(key)){
                        table->setItem(dfastateid + 1, forwardStateId + 1, new QTableWidgetItem(QString::number(target)));
                    }
                    else
                        table->setItem(dfastateid + 1, forwardStateId + 1, new QTableWidgetItem("s" + QString::number(target)));
                }

            }
        }

        //规约item
        for(LALR1item item : state){
            myrule rule = myrules[item.rid];
            if(item.rindex == rule.right.size()){
                //规约状态

                //找到对应符号
                for(QString symbol : item.lookahead){
                    int backwardStateId = analyzeSymVec.indexOf(symbol);

                    if(rule.left == myrules[0].left){
                         //文法开始符号的规约状态，一定是ACC S'->S.
                        table->setItem(dfastateid + 1, backwardStateId + 1,
                                       new QTableWidgetItem("Accept"));
                        continue;
                    }
                    QString rulestring = "(" + rule.left + "->";
                    for(QString rightsymbol : rule.right){
                        rulestring += rightsymbol + " ";
                    }
                    rulestring += ")";

                    table->setItem(dfastateid + 1, backwardStateId + 1,
                                   new QTableWidgetItem("r" + rulestring));
                }
            }
        }
    }

    //动态调整行宽和列宽
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

bool checkLALR1confilct(LALR1dfa dfa){
    for(LALR1State state : dfa){
        QVector<LALR1item> itemVec; //记录所有规约item
        for(LALR1item item : state){
            if(item.rindex == myrules[item.rid].right.size()){
                itemVec.push_back(item);
            }
        }
        for(LALR1item item1 : itemVec){
            for(LALR1item item2 : itemVec){
                if(item1.rid == item2.rid && item1.rindex == item2.rindex)continue;
                if(!item1.lookahead.intersect(item2.lookahead).isEmpty()){
                    //如果两个规约item的先行符号集有交集
                    return false;
                }
            }
        }
    }
    return true;
}
void initTable(QTableWidget* table){
    table->clear();
    table->setAlternatingRowColors(true);
}

//全局变量
LALR1dfa lalr1dfa;



// 显示提示信息
void analyzeCompleteMsg(QString msg){
    // 创建一个消息框
    QMessageBox msgBox;
    msgBox.setWindowTitle("提示");
    msgBox.setText(msg + "分析完成，请点击选项框查看结果");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok); // 设置按钮为“确定”

    // 显示消息框
    msgBox.exec();
    return;
}





void Widget::on_openFile_clicked()
{
    ui->plainTextEdit->clear();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),QDir::currentPath(),tr("Files (*.txt)"));

    if(fileName.isEmpty()){
        QMessageBox::warning(this, "警告", "请选择一个文件");
    }
    else{
        QFile file(fileName); // 打开文件
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QMessageBox::about(NULL, "文件", "文件无法打开");
        }
        QTextStream in(&file);
        in.setCodec("UTF-8"); // 设置编码为UTF-8
        QString fileContent = in.readAll();
        ui->plainTextEdit->setPlainText(fileContent);
        file.close();
    }
}

void Widget::on_saveFile_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),QDir::currentPath(),tr("txt (*.txt)"));

    if(fileName.isEmpty())
        QMessageBox::warning(this, "警告", "请输入文件名");
    else{

        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QMessageBox::about(NULL, "文件", "文件无法打开");
        }
        QString fileContent = ui->plainTextEdit->toPlainText();
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << fileContent;
        file.close();
       }
}

// 句子分析中parseStack中的一个节点
struct parseNode{
    QString letter; // 字母
    int lalr_state_id; // lalr1dfa中state的id
    parseNode(){
        this->letter = "";
        this->lalr_state_id = 0;
    }
    parseNode(QString str, int inid){
        this->letter = str;
        this->lalr_state_id = inid;
    }
    QString toQStr(){
        return this->letter + QString::number(this->lalr_state_id);
    }
};


//将分析栈中的内容直观输出
QString stack2QStr(stack<parseNode> stk){
    //为了正常输出，使用另一个栈进行暂存
    stack<parseNode> tempStack;
    QString output;
    while(!stk.empty()){
        parseNode tempnode = stk.top();
        stk.pop();
        tempStack.push(tempnode);
    }

    while(tempStack.size() != 0){
        parseNode tempnode = tempStack.top();
        tempStack.pop();
        output += tempnode.toQStr();
    }
    return output;
}

//将inputQue中的内容直观输出
QString que2QStr(queue<QString> que){
    QString res;
    while(que.size()){
        res += que.front() + " ";
        que.pop();
    }
    return res;
}

// 存储一行lex内容
struct lexRow{
    QString key, value;
    lexRow(QString k, QString v){
        this->key = k, this->value = v;
    }
};

QString que2QStr(queue<lexRow> que){
    QString res;
    while(que.size()){
        res += que.front().value;
        que.pop();
    }
    return res;
}


// 尚未完善
// 判断输入的源程序中的字符串是否为数字
bool isNum(QString str){
    if(nonterminal.contains(str)) return false;

    //正则表达式：num=digit+(.digit+)?
    int state = 0;
    bool isnum = false;
    for(QChar ch : str){
        switch (state) {
            case 0:
                if(isdigit(ch)){
                    isnum = true; return isnum;
                }
        }
    }
    return false;
}


// 判断输入的源程序中的字符串是否为变量名，由于minic向tiny兼容，因此正则表达式取minic
bool isId(QString str){
    if(str == "EOF")return false;
    if(nonterminal.contains(str)) return false;
    if(terminal.contains(str))return false;
    // 正则表达式: id=(_|letter)(_|letter|digit)*
    int state = 0;
    bool isid = false;
    for(QChar ch : str){
        switch (state) {
            case 0:
                if(isalpha(ch) || ch == "_"){
                    state = 1;
                    isid = true;
                    break;
                }
                else return false;
            case 1:
                if(isalpha(ch) || ch == "_" || isdigit(ch)){
                    state = 1;
                    isid = true;
                    break;
                }
                else return false;
        }
    }
    return isid;
}





// 判断是否是移进关系，如果是，则返回移进后的id，否则返回-1
int isShift(int lalr1_state_id, QString letter){
    int target = -1;
//    LALR1State lalr1state = lalr1dfa[lalr1_state_id]; //拿到state
    QMap<QString, int> cur_forwords = forwards[lalr1_state_id]; // 当前state的移进关系
    for(QString next_letter : cur_forwords.keys()){
        if(next_letter == letter){
            //找到了移进符号,返回移进后的id
            target = cur_forwords[next_letter];
        }
    }
    return target;
}


// 获取规约规则
myrule getReduceRule(QString letter, int state_id){
    LALR1State state = lalr1dfa[state_id];
    // 收集该state_id下所有的规约rule
    for(LALR1item item : state){
        myrule rule = myrules[item.rid];
        if(item.rindex ==  rule.right.size()){
            //规则右边非空
            if(item.lookahead.contains(letter)){
                // 先行符号集中有letter,找到规约item了
                return rule;
            }
        }
        else if(rule.right.size() == 1 && rule.right[0] == "@"){
            //规则右边为空
            if(item.lookahead.contains(letter))
                return rule;
        }
    }
    return myrule();
}

// 句子分析的一条过程
struct analyzeResLine{
    QString step, stackQStr, queQStr, action;
    analyzeResLine(){
        this->step = "", this->stackQStr = "", queQStr = "", this->action = "";
    }
    void setStepInfo(int num, stack<parseNode> stk, queue<QString> que){
        this->step = QString::number(num);
        this->stackQStr = stack2QStr(stk);
        this->queQStr = que2QStr(que);
    }
    // 重载函数，输入的que改为lexRow结构
    void setStepInfo(int num, stack<parseNode> stk, queue<lexRow> que){
        this->step = QString::number(num);
        this->stackQStr = stack2QStr(stk);
        queue<QString> value_que;
        for(int i = 0; i < que.size(); i++){
            QString value = que.front().key;
            que.pop();
            value_que.push(value);
        }
        this->queQStr = que2QStr(value_que);
    }


    void setAction(QString actionQStr){
        this->action = actionQStr;
    }

};

// 全局变量：句子语法分析结果
QVector<analyzeResLine> codeAnalyzeResLines;

//全局变量：lex文件语法分析结果
QVector<analyzeResLine> lexAnalyzeresLines;


struct treeNode{
    QString label; //标签
    QString value;  //值
    QVector<treeNode*> children;  //孩子节点
    treeNode(){
    }
    treeNode(QString label){
        this->label = label;
    }
    treeNode(QString label, QString value){
        this->label = label, this->value = value;
    }
    void addChild(treeNode* child){
        this->children.push_back(child);
    }
    void addChildren(QVector<treeNode*> children){
        this->children.append(children);
    }
};

//全局变量，源码分析的树根节点指针
treeNode* codeParseRootNode;

//对QVector进行反转
QVector<treeNode*> reserveTreeNode(QVector<treeNode*> vec){
    for(int i = 0; i < vec.size() / 2; i++){ //for循环向下取整
       swap(vec[i], vec[vec.size() - 1 - i]);
    }
    return vec;
}

// 对一个句子进行分析
bool codeParse(QStringList allLinesList){
    bool is_complete = false;
    queue<QString> inputQue;  //输入队列
    stack<parseNode> parseStack; // 分析栈

    //以下两个变量用于生成语法树
    stack<treeNode*> treeNodeStack; // 语法树节点栈
    treeNode* tempTreeNodePtr = new treeNode(); //暂时存储队首元素的指针

    // 初始化栈和队列
    parseStack.push(parseNode("$", 0)); //初始符号和状态0入栈
    for(QString letter : allLinesList){
        inputQue.push(letter);
    }
    inputQue.push("$"); // 结束符号入队列
    int step = 0;

    bool flag = true;
    while(flag){
        analyzeResLine line;
        line.setStepInfo(step, parseStack, inputQue);
        // debug 输出分析信息
        QString analyzeStr = "step:" + QString::number(step) + " stack: " + stack2QStr(parseStack)
                + " queue: " + que2QStr(inputQue) + "  ";


        parseNode stack_top_node = parseStack.top();
        QString que_front_letter = inputQue.front();
        if(tempTreeNodePtr->value == ""){
            // 如果value没有存数据，说明不是id或num，则使用队首的字符串构造一个新的节点，字符串传为label
            tempTreeNodePtr = new treeNode(que_front_letter);
        }

        //判断是否是移进关系，如果是，获得移进的目标，否则得到-1
        int target = isShift(stack_top_node.lalr_state_id, que_front_letter);

        QString action_info;


        if(target != -1){
            //移进
            inputQue.pop(); //弹出队首元素
            //队首元素+移进的目标节点id 入栈
            parseNode push_node = parseNode(que_front_letter, target);
            parseStack.push(push_node);
            action_info = "移进" + QString::number(target);

            //处理语法树节点
            tempTreeNodePtr->label = que_front_letter;
            treeNodeStack.push(tempTreeNodePtr); //语法树节点入栈
            tempTreeNodePtr = new treeNode(que_front_letter); //new:入栈后要new一下
        }
        else if(isId(que_front_letter)){
            // id
            QSet<QString> id_name_set = {"ID", "identifier"};
            for(myrule rule : myrules){
                if(rule.right.size() != 1)continue;
                for(QString rule_right_letter : rule.right){
                    if(id_name_set.contains(rule_right_letter)){
                        tempTreeNodePtr->value = QString(inputQue.front()); // 值存储到语法树节点 //new：QString()
                        tempTreeNodePtr->label = rule_right_letter; //new
                        // 用rule_right_letter替换变量名

                        inputQue.front() = rule_right_letter; //直接修改队首元素
                        action_info = "用" + rule_right_letter + "替换" + que_front_letter;
                        break; //new
                    }
                }
            }
        }
        else if(isNum(que_front_letter)){
            // num
            QSet<QString> num_name_set = {"num", "NUM", "number", "n"}; //num的表现形式是集合中的其中一个元素
            for(myrule rule : myrules){
                if(rule.right.size() != 1)continue; //new
                for(QString rule_right_letter : rule.right){
                    if(num_name_set.contains(rule_right_letter)){
                        tempTreeNodePtr->value = inputQue.front(); // 值存储到语法树节点
                        // 用rule_right_letter替换变量名
                        inputQue.front() = rule_right_letter; //直接修改队首元素
                        action_info = "用" + rule_right_letter + "替换" + que_front_letter;
                        break; //new
                    }
                }
            }
        }
        //普通的规约
        else{
            //拿到规约的规则 left -> right
            myrule reduce_rule = getReduceRule(inputQue.front(), stack_top_node.lalr_state_id);

            //若没有找到对应的规约规则
            if(reduce_rule.left == ""){
                qDebug() << "reduce error!";
                return false;
            }
            // 记录action信息
            action_info = "用" + reduce_rule.left + "->";
            for(QString rightsymbol : reduce_rule.right){
                action_info += rightsymbol + " ";
            }
            action_info += "规约";
            if(reduce_rule.left == myrules[0].left){
                //文法开始符号，接受,不用继续分析了
                action_info = "Accept";
                is_complete = true;
                flag = false;
            }
            if(reduce_rule.right.size() == 1 && reduce_rule.right[0] == "@"){
                // rule: left -> @ ,规则右边是空串
                // parsenode不用操作, 新的语法树节点入栈
                // treeNodeStack以规约规则的左边作为节点label入栈
                treeNodeStack.push(new treeNode(reduce_rule.left));
            }
            else{
                // rule.right 不是 @ 空串, 出栈left.size个元素
                int popsize = reduce_rule.right.size();
                for(int i = 0; i < popsize; i++){
                    parseStack.pop();
                }
                // 操作语法树节点
                bool has_parent = false;
                QVector<treeNode*> temp_node_vec; //存储孩子
                treeNode* parent_node = new treeNode(); //存储父节点
                //debug
                qDebug() << "treeAction:";
                for(int k = 0; k < treeAction[reduce_rule.rid].size(); k++){
                    qDebug() << treeAction[reduce_rule.rid][k];
                }
                for(int j = reduce_rule.right.size() - 1; j >= 0; j-- ){
                    int semantic_op = treeAction[reduce_rule.rid][j];

                    if(semantic_op == 0)treeNodeStack.pop(); //new
                    else if(semantic_op == 1){
                        //作为孩子节点
                        treeNode* tempnode = treeNodeStack.top();
                        treeNodeStack.pop();
                        temp_node_vec.push_back(tempnode);
                    }
                    else if(semantic_op == -1){
                        //作为父节点
                        has_parent = true;
                        treeNode* tempnode = treeNodeStack.top();
                        treeNodeStack.pop();
                        parent_node = tempnode;
                    }
                }
                if(has_parent){
                    qDebug() << "reserve!";
                    //由于出栈时逆序了，所以现在逆序回来
                    temp_node_vec = reserveTreeNode(temp_node_vec);

                    //有父亲节点，全部挂到父结点上
                    parent_node->addChildren(temp_node_vec);
                    treeNodeStack.push(parent_node);
                }
                else{
                    //没有父亲节点，用rule.left新建一个父节点，然后将vec中的节点接到父亲节点之下
                    parent_node->label = reduce_rule.left;
                    temp_node_vec = reserveTreeNode(temp_node_vec);
                    parent_node->addChildren(temp_node_vec);
                    treeNodeStack.push(parent_node);
                }
            }
            // 新元素入栈
            QString trans_letter = reduce_rule.left;
            parseNode top_node = parseStack.top();
            int target = forwards[top_node.lalr_state_id][reduce_rule.left];
            parseStack.push(parseNode(trans_letter, target));
        }
        analyzeStr += action_info;
        qDebug() << analyzeStr;
        step++;


        //保存action信息,将整条分析结果保存到全局变量
        line.setAction(action_info);
        codeAnalyzeResLines.push_back(line);
    }
    if(treeNodeStack.size() != 1){
        // 语法树节点只能保留一个根节点指针
        warnmsg("语法树生成有误，请重试");
        return false;
    }
    else{
        codeParseRootNode = new treeNode();
        codeParseRootNode = treeNodeStack.top(); // 取出栈中最后一个根节点
    }

    return is_complete;
}


//parse函数运行完毕后，剔除中间状态
void removeRedundantStates(treeNode* cur_root){

    bool flag = true;
    while(flag){
        flag = false;
        for(int i = 0; i < cur_root->children.size(); i++){
            treeNode* child = cur_root->children[i];
            removeRedundantStates(child); //先处理他的孩子们

            //遍历整棵树
            if(nonterminal.contains(child->label)){
                flag = true; //发生变化，需要继续遍历
                //如果一个节点的label是nonterminal，则将他的孩子按照原本的顺序全部给到他的父亲
                cur_root->children.remove(i);
                for(int j = 0; j < child->children.size(); j++){
                    cur_root->children.insert(i + j, child->children[j]);
                }

//                cur_root->addChildren(child->children);
//                cur_root->children.remove(i);
            }
        }
    }

    //继续处理他的孩子节点
    for(int i = 0; i < cur_root->children.size(); i++){
        treeNode* child = cur_root->children[i];
        removeRedundantStates(child);
    }


}

//展示句子分析结果
void showAnalysis(QTableWidget* table, QVector<analyzeResLine> analyzeRes){

    table->verticalHeader()->setVisible(false); //不显示行号
    //状态编号 文法 转移符号
    QStringList header;
    header << "步骤" << "分析栈" << "输入队列" << "action";

    //设置第一行表头信息
    table->setColumnCount(header.size());
    table->setHorizontalHeaderLabels(header);
    table->setRowCount(analyzeRes.size());

    for(int row_cnt = 0; row_cnt < analyzeRes.size(); row_cnt++){
        table->setItem(row_cnt, 0, new QTableWidgetItem(analyzeRes[row_cnt].step));
        table->setItem(row_cnt, 1, new QTableWidgetItem(analyzeRes[row_cnt].stackQStr));
        table->setItem(row_cnt, 2, new QTableWidgetItem(analyzeRes[row_cnt].queQStr));
        table->setItem(row_cnt, 3, new QTableWidgetItem(analyzeRes[row_cnt].action));
    }
    //动态调整列宽
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void setSpaceAroundSemicolons(QString& input){
    input.replace(";", " ; ");
}


// 对即将进行语法分析的字符串进行预处理
QStringList preProcessCodeInput(QString codeInput){
    QStringList codeLines = codeInput.split('\n');

    // 输入的源程序整合为一行
    QString allCodeLine;
    for(int i = 0; i < codeLines.size(); i++){
        allCodeLine += codeLines[i] + " ";
    }
    // 在以下特殊符号两边加空格，以免将符号和其他内容读取为一个字符串导致出错
    QSet<QString> special_letter_set = {"(", ")", ";", "[", "]", ","};
    for(QString letter : special_letter_set){
        allCodeLine.replace(letter, " " + letter + " ");
    }

    QStringList allCodeLineList = allCodeLine.split(' '); //以空格为分隔符进行区分
    allCodeLineList.removeAll(""); //删除空字符串
    return allCodeLineList;
}

//判断输入的语义编码与文法规则是否匹配
bool checkSemantic(QStringList semanticList){
    if(semanticList.size() != myrules.size())return false;
    for(int i = 0; i < semanticList.size(); i++){
       QString line = semanticList[i];
       myrule currule = myrules[i];
       // 定位 `->` 左边的字符串
       int arrowIndex = line.indexOf("->");
       QString leftPart = line.left(arrowIndex).trimmed();


       QString middlePart;
       QStringList middlePartList;
       int colonIndex = line.indexOf("\\{");
       // 获取规则右边的字符串
       //new
       if(line.count("\\{") == 1){
           //文法规则右边没有'{'
           middlePart = line.mid(arrowIndex + 2, colonIndex - (arrowIndex + 4)).trimmed();
           middlePartList = middlePart.split(" ", Qt::SkipEmptyParts);// 按空格分割中间部分
       }
       else if(line.count("\\{") == 2){
            colonIndex = line.indexOf("{", colonIndex + 1); //找到二次出现的地方
            middlePart = line.mid(arrowIndex + 2, colonIndex - (arrowIndex + 4)).trimmed();
            middlePartList = middlePart.split(" ", Qt::SkipEmptyParts);// 按空格分割中间部分
       }
       else{
           //error
            qDebug() << "more than 2 '{' exiting!";
       }

       //与文法进行对比
       if(leftPart != currule.left){
           qDebug() << "error line:" + line;
           return false;
       }

       if(middlePartList.size() != currule.right.size()){
           qDebug() << "error line:" + line;
           return false;
       }
       for(int j = 0; j < middlePartList.size(); j++){
           if(middlePartList[j] != currule.right[j]){
               qDebug() << "error line:" + line;
               return false;
           }
       }
    }
    return true;
}

QVector<int> getEncodedNum(QString line, int rulerightsize){
    qDebug() << line;
    //-1:作根节点，1:作为孩子节点,0或没有:不保留信息
    QVector<int> res;




    //new
    // 提取 {} 中的内容
    QRegularExpression regex("\\\\{(.*)\\\\}");
    QRegularExpressionMatch match = regex.match(line);
    if (match.hasMatch()) {
        QString content = match.captured(1); // 获取大括号中的内容
        // 按逗号分割每个键值对
        QStringList pairs = content.split(",", Qt::SkipEmptyParts);
        res.resize(rulerightsize);

        for (const QString& pair : pairs) {
            // 按冒号分割键和值
            QStringList keyValue = pair.split(":", Qt::SkipEmptyParts);
            if (keyValue.size() == 2) {
                int key = keyValue[0].trimmed().toInt();
                int value = keyValue[1].trimmed().toInt();
                if(key > res.size() - 1){
                    qDebug() << "EncodedNum error line:" + line;
                }
                res[key] = value;
            }
        }
    }
    return res;
}



// 处理语义编码
bool processSemanticInput(QString semanticInput){
    treeAction.clear();
    treeAction.resize(myrules.size()); //初始化容量大小

    QStringList semanticList = semanticInput.split("\n");
    semanticList.removeAll(""); //去除空行
    if(!checkSemantic(semanticList)){
        warnmsg("输入的语义编码与文法不匹配，请重新输入!");
        return false;
    }
    for(int i = 0; i < semanticList.size(); i++){
        treeAction[i].resize(myrules[i].right.size());//初始化容量大小
        QString line = semanticList[i];
        treeAction[i] = getEncodedNum(line, myrules[i].right.size());  //获取一行的编码
//        qDebug() << i << "complete";
    }
    qDebug() << "treeAction processing complete!";
    return true;
}

QStringList dfs_text(treeNode* cur_root_node){
    QStringList res;
    //树根信息
    QString root_text = "";
    root_text = cur_root_node->label;
    if(cur_root_node->value != ""){
        root_text += "(" + cur_root_node->value + ")";
    }
    res.push_back(root_text);
    //孩子信息
    for(int i = 0; i < cur_root_node->children.size(); i++){
        QStringList child_text_list = dfs_text(cur_root_node->children[i]);
        //把孩子信息加上去
        for(int j = 0; j < child_text_list.size(); j++){
            res.push_back("| — " + child_text_list[j]);
        }
    }
    return res;


}

void showTreeText(QTextBrowser* browser, treeNode* root){
    // 使用dfs算法进行展示
    QString treeText = QString(); //最终展示内容
    QStringList textList = dfs_text(root);
    for(int i = 0; i < textList.size(); i++){
        treeText += textList[i] + "\n";
    }

    browser->setText(treeText);
}

QTreeWidgetItem* dfsTreeWidget(treeNode* cur_root_node){
    // 根节点的信息
    QString root_text = cur_root_node->label;
    if(cur_root_node->value != "")
        root_text += "(" + cur_root_node->value + ")";

    // 根节点
    QTreeWidgetItem* root_item = new QTreeWidgetItem();
    root_item->setText(0, root_text);

    // 孩子节点
    for(int i = 0; i < cur_root_node->children.size(); i++){
         QTreeWidgetItem* child_item = dfsTreeWidget(cur_root_node->children[i]);
         root_item->addChild(child_item); // 将子节点添加到当前根节点
    }

    return root_item;

}


// Qt treeWidget控件展示
void displayTree(QTreeWidget *treeWidget, treeNode* lexParseRootNode) {

    // 添加根节点
    QTreeWidgetItem *rootItem = dfsTreeWidget(lexParseRootNode);
    treeWidget->addTopLevelItem(rootItem); // 将根节点添加到 QTreeWidget
    treeWidget->expandAll(); // 展开所有节点（可选）
    treeWidget->show(); // 显示树控件
}


// 源程序语法分析： 分析源程序按钮
void Widget::on_codeAnalyzeBtn_clicked()
{
    codeAnalyzeResLines.clear(); //清空分析结果全局变量
    initTable(ui->codeAnalyzeTable); //清空分析表
    ui->simpleTreeBrowser->clear(); // 清空语法树展示内容
    ui->codeTreeWidget->clear(); // 清空语法树展示
    if(myrules.size() == 0)  //尚无BNF文法输入，无法进行分析
        warnmsg("无BNF文法输入，无法进行分析");


    QString codeInput = ui->codePlainTextEdit->toPlainText(); // 获取文法源程序
    QString semanticInput = ui->codeSemanticPlainTextEdit->toPlainText(); // 获取语义编码

    // 对输入的源程序进行预处理
    QStringList allCodeLineList = preProcessCodeInput(codeInput);
    // 对输入的语义编码进行处理
    if(!processSemanticInput(semanticInput)){
        // 识别到编码错误
        return;
    }

    // 进行语法分析，判断是否分析正确
    bool isParseComplete = codeParse(allCodeLineList);
    if(!isParseComplete){
        //分析出错
        warnmsg("源程序输入错误，无法进行完整分析，请检查代码");
        return;
    }
    //去除多余的中间状态
    removeRedundantStates(codeParseRootNode);

    showTreeText(ui->simpleTreeBrowser, codeParseRootNode); //文本形式展示语法树

     //展示逐句语法分析结果
    showAnalysis(ui->codeAnalyzeTable, codeAnalyzeResLines);

    // 使用treeWidget控件进行展示
    displayTree(ui->codeTreeWidget, codeParseRootNode);

    // 弹窗输出提示信息
    analyzeCompleteMsg("源代码");
}

// 源程序语法分析：打开文对应的程序设计语言的源程序
void Widget::on_openCodeFileBtn_clicked()
{
    ui->codePlainTextEdit->clear();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),QDir::currentPath(),tr("Files (*.*)"));

    if(fileName.isEmpty()){
        QMessageBox::warning(this, "警告", "请选择一个文件");
    }
    else{
        QFile file(fileName); // 打开文件
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QMessageBox::about(NULL, "文件", "文件无法打开");
        }
        QTextStream in(&file);
        in.setCodec("UTF-8"); // 设置编码为UTF-8
        QString fileContent = in.readAll();
        ui->codePlainTextEdit->setPlainText(fileContent);
        file.close();
    }
}

// 源程序语法分析：打开语义编码文件
void Widget::on_openSemanticBtn_clicked()
{
    ui->codeSemanticPlainTextEdit->clear();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),QDir::currentPath(),tr("Files (*.*)"));

    if(fileName.isEmpty()){
        QMessageBox::warning(this, "警告", "请选择一个文件");
    }
    else{
        QFile file(fileName); // 打开文件
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QMessageBox::about(NULL, "文件", "文件无法打开");
        }
        QTextStream in(&file);
        in.setCodec("UTF-8"); // 设置编码为UTF-8
        QString fileContent = in.readAll();
        ui->codeSemanticPlainTextEdit->setPlainText(fileContent);
        file.close();
    }
}



//---------------------------------------------------------------
// 开始分析lex文件
//---------------------------------------------------------------

// 存储一行lex内容
//struct lexRow{
//    QString key, value;
//    lexRow(QString k, QString v){
//        this->key = k, this->value = v;
//    }
//};
//全局变量
QString lexInputContext; //lex文件内容
QString lexSenmanticQStr; //lex对应语义编码
QVector<lexRow> LexVec; //存储lex每一行的内容
treeNode* lexParseRootNode; //树根节点


// 打开lex文件
void Widget::on_openLexFileBtn_clicked()
{
    ui->lexEdit->clear();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),QDir::currentPath(),tr("Files (*.lex)"));

    if(fileName.isEmpty()){
        QMessageBox::warning(this, "警告", "请选择一个文件");
    }
    else{
        QFile file(fileName); // 打开文件
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QMessageBox::about(NULL, "文件", "文件无法打开");
        }
        QTextStream in(&file);
        in.setCodec("UTF-8"); // 设置编码为UTF-8
        QString fileContent = in.readAll();
        ui->lexEdit->setPlainText(fileContent);
        file.close();
    }
}

//打开lex对应的编码文件
void Widget::on_openLexSemanticBtn_clicked()
{
    ui->lexSemanticEdit->clear();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),QDir::currentPath(),tr("Files (*.txt)"));

    if(fileName.isEmpty()){
        QMessageBox::warning(this, "警告", "请选择一个文件");
    }
    else{
        QFile file(fileName); // 打开文件
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QMessageBox::about(NULL, "文件", "文件无法打开");
        }
        QTextStream in(&file);
        in.setCodec("UTF-8"); // 设置编码为UTF-8
        QString fileContent = in.readAll();
        ui->lexSemanticEdit->setPlainText(fileContent);
        file.close();
    }
}

void processLexInput(QStringList lex_input_context){
    for(const QString& line : lex_input_context){
        int colonIndex = line.indexOf(':'); // 查找第一个冒号的位置
        // 如果找到冒号，说明是num或者id
        if (colonIndex != -1) {
            // 以冒号为界分割字符串
            QString key = line.left(colonIndex).trimmed(); // 冒号左侧部分
            QString value = line.mid(colonIndex + 1).trimmed(); // 冒号右侧部分
            if(key == QString::number(numberIndex))key = "NUMBER";
            else if(key == QString::number(idIndex))key = "ID";
            else if(key == "EOF"){}
            else {
                warnmsg("lex input error!");
                return;
            }
            LexVec.push_back(lexRow(key, value)); // 存储到 LexVec
        }
        else{
            int lex_code = line.toInt();
            if(lex_code >= keywordsStartIndex && lex_code <= operatorsStartIndex){
                //keyword编码,如int，double
                QString key = lexcode2str[lex_code];
                LexVec.push_back(lexRow(key, key));
            }
            else if(lex_code >= operatorsStartIndex){
                //operators运算符编码
                QString op = lexcode2str[lex_code];
                QString opname = QString::fromStdString(opMap[op.toStdString()]);
                LexVec.push_back(lexRow(opname, op));
            }
            else{
                warnmsg("lex input error!");
                return;
            }
        }
    }

}

void readLex(){
    LexVec.clear();
    QStringList lexContextList = lexInputContext.split("\n"); //按行分割
    processLexInput(lexContextList);


//    // 解析冒号两边的内容
//    for (const QString& line : lexContextList) {
//        int colonIndex = line.indexOf(':'); // 查找第一个冒号的位置
//        if (colonIndex != -1) { // 如果找到冒号
//            // 以冒号为界分割字符串
//            QString key = line.left(colonIndex).trimmed(); // 冒号左侧部分
//            QString value = line.mid(colonIndex + 1).trimmed(); // 冒号右侧部分
//            LexVec.push_back(lexRow(key, value)); // 存储到 LexVec
//        }
//    }
}



// 对lex文件进行语法分析
bool lexParse(){
   bool is_complete = false;
   queue<lexRow> inputQueue; // 输入队列
   stack<parseNode> parseStack; // 分析栈

   //语法树变量
   stack<treeNode*> treeNodeStack;
   treeNode* tempTreeNodePtr = new treeNode();  //暂存队首元素的指针

   //初始化栈和队列
   parseStack.push(parseNode("EOF", 0));
   for(int i = 0; i < LexVec.size() - 1; i++){
       //避免读取EOF
       lexRow row = LexVec[i];
       inputQueue.push(row);
   }
   inputQueue.push(lexRow("$", "$"));
   if(tempTreeNodePtr->value == ""){
       tempTreeNodePtr = new treeNode();
   }

   bool flag = true;
   int step = 0;
   while(flag){
        QString debug_info;
        parseNode stack_top_node = parseStack.top();

        lexRow que_front_row = inputQueue.front();
        analyzeResLine line;
        line.setStepInfo(step, parseStack, inputQueue);

        QString analyzeStr = "step:" + QString::number(step) + " stack: " + stack2QStr(parseStack)
                + " queue: " + que2QStr(inputQueue) + "  ";
        QString action_info; //action信息
        // 规约:-1,移进:≠-1
        int target = isShift(parseStack.top().lalr_state_id, que_front_row.value); //思考————————————————应该用value
        if(target != -1){
            // 移进
            // 处理分析栈
            inputQueue.pop();
            parseNode parse_push_node = parseNode(que_front_row.value, target);
            parseStack.push(parse_push_node);

            // 处理语法树节点
            treeNode* tree_push_node = new treeNode();
            tree_push_node->label = que_front_row.key;
            if(tempTreeNodePtr->value != ""){
                tree_push_node->value = tempTreeNodePtr->value;
                tempTreeNodePtr = new treeNode(); //移进信息后，要将tempTreeNodePtr进行更新
            }
            treeNodeStack.push(tree_push_node);

            if(isOperator(inputQueue.front().value)){
                tempTreeNodePtr->value = QString(inputQueue.front().value); // 值存储到语法树节点
            }
            action_info = "移进" + QString::number(target);

        }
        else if(isId(que_front_row.value)){
            // id
            QSet<QString> id_name_set = {"ID", "identifier"};
            for(myrule rule : myrules){
                if(rule.right.size() != 1)continue; //new
                for(QString rule_right_letter : rule.right){
                    if(id_name_set.contains(rule_right_letter)){
                        //记录action信息
                        action_info += "用" + rule_right_letter + "替换" + que_front_row.value;

                        // 遇到ID，将value存到tempTreeNodePtr.value中，原本que中的值进行替换，以便后续进行移进
                        tempTreeNodePtr->value = QString(inputQueue.front().value); // 值存储到语法树节点 //new：QString()
                        inputQueue.front().value = rule_right_letter; // 用rule_right_letter替换变量名

                        break; //new
                    }
                }
            }
        }
        else if(isNum(que_front_row.value)){
            // num
            QSet<QString> num_name_set = {"num", "NUM", "number", "n"}; //num的表现形式是集合中的其中一个元素
            for(myrule rule : myrules){
                if(rule.right.size() != 1)continue; //new
                for(QString rule_right_letter : rule.right){
                    if(num_name_set.contains(rule_right_letter)){
                        //记录action信息
                        action_info += "用" + rule_right_letter + "替换" + que_front_row.value;

                        tempTreeNodePtr->value = QString(inputQueue.front().value); // 值存储到语法树节点
                        // 用rule_right_letter替换变量名
                        inputQueue.front().value = rule_right_letter; //直接修改队首元素
                        break;
                    }
                }
            }
        }
        else{
            //规约
            myrule reduce_rule = getReduceRule(que_front_row.value, stack_top_node.lalr_state_id);
            if(reduce_rule.left == ""){
                qDebug() << "reduce error!"; //没有找到规约规则
                return false;
            }
            if(reduce_rule.left == myrules[0].left){
                // 找到文法开始符号，结束
                is_complete = true;
                flag = false;
            }
            if(reduce_rule.right.size() == 1 && reduce_rule.right[0] == "@"){
                // rule: left -> @ ,规则右边是空串
                // parsenode不用操作, 新的语法树节点入栈
                // treeNodeStack以规约规则的左边作为节点label入栈
                treeNodeStack.push(new treeNode(reduce_rule.left));
            }
            if(!(reduce_rule.right.size() == 1 && reduce_rule.right[0] == "@")){
                // rule.right不是@空串，出栈rule.left.size()个元素
                for(int i = 0; i < reduce_rule.right.size(); i++)
                    parseStack.pop();

                //操作语法树节点
                bool has_parent = false;
                QVector<treeNode*> temp_node_vec; //存储孩子
                treeNode* parent_node = new treeNode(); //存储父节点
                for(int j = reduce_rule.right.size() - 1; j >= 0; j-- ){
                    int semantic_op = treeAction[reduce_rule.rid][j];

                    if(semantic_op == 0)treeNodeStack.pop(); //编码为0，舍弃信息
                    else if(semantic_op == 1){
                        //作为孩子节点
                        treeNode* tempnode = treeNodeStack.top();
                        treeNodeStack.pop();
                        temp_node_vec.push_back(tempnode);
                    }
                    else if(semantic_op == -1){
                        //作为父节点
                        has_parent = true;
                        treeNode* tempnode = treeNodeStack.top();
                        treeNodeStack.pop();
                        parent_node = tempnode;
                    }
                }
                if(has_parent){
                    qDebug() << "reserve!";
                    //由于出栈时逆序了，所以现在逆序回来
                    temp_node_vec = reserveTreeNode(temp_node_vec);

                    //有父亲节点，全部挂到父结点上
                    parent_node->addChildren(temp_node_vec);
                    treeNodeStack.push(parent_node);
                }
                else{
                    //没有父亲节点，新建一个父节点，然后将vec中的节点接到父节点下
                    parent_node->label = reduce_rule.left;
                    temp_node_vec = reserveTreeNode(temp_node_vec);
                    parent_node->addChildren(temp_node_vec);
                    treeNodeStack.push(parent_node);
                }
            }
            // 新元素入分析栈
            QString trans_letter = reduce_rule.left;
            parseNode top_node = parseStack.top();
            int target = forwards[top_node.lalr_state_id][reduce_rule.left];
            parseStack.push(parseNode(trans_letter, target));
            //记录action信息
            action_info = "用" + reduce_rule.left + "->";
            for(QString rightletter : reduce_rule.right){
                action_info += rightletter + " ";
            }
            action_info += "规约";

            //规约的规则是文法开始符号，结束
            if(reduce_rule.left == myrules[0].left){
               action_info = "Accept";
               is_complete = true;
               flag = false;
            }

        }
        analyzeStr += " " + action_info;
        line.setAction(action_info);
        //存储语法分析结果
        lexAnalyzeresLines.push_back(line);
//        qDebug() << analyzeStr;
        step++;
   }
   if(treeNodeStack.size() != 1){
       // 语法树节点只能保留一个根节点指针
       warnmsg("语法树生成有误，请重试");
       return false;
   }
   else{
       lexParseRootNode = new treeNode();
       lexParseRootNode = treeNodeStack.top(); // 取出栈中最后一个根节点
   }

   return is_complete;

}





// 开始分析lex文件
void Widget::on_AnalyzeLex_clicked(){

    //清空控件展示
    ui->lexAnalyzeTable->clear();
    ui->lexTreeWidget->clear();
    ui->lexTreeBroswer->clear();

    //清空全局变量
    lexAnalyzeresLines.clear();


    if(myrules.isEmpty()){
        //还没有输入文法
        warnmsg("请先输入文法规则并开始分析！");
        return;
    }


    lexInputContext = ui->lexEdit->toPlainText();
    lexSenmanticQStr = ui->lexSemanticEdit->toPlainText();

    readLex(); //得到lex每一行的内容
    if(!processSemanticInput(lexSenmanticQStr)){//处理并判断语义编码是否正确
        // 编码错误
        warnmsg("文法对应的语义编码错误");
        return;

    }
    if(!lexParse()){
        // 分析lex内容
        warnmsg("语法分析错误，请检查源程序");
        return;
    }
    //展示逐句语法分析结果
   showAnalysis(ui->lexAnalyzeTable, lexAnalyzeresLines);

    // lexParse结束后，去除语法树中多余的中间状态
    removeRedundantStates(lexParseRootNode);

    // 用文本形式展示语法树
    showTreeText(ui->lexTreeBroswer, lexParseRootNode);


    // 尝试treeWidget控件
    displayTree(ui->lexTreeWidget, lexParseRootNode);


    // 弹窗提示信息
    analyzeCompleteMsg("lex文件");

}



//--------------------------------------------------
//开始分析文法规则
//---------------------------------------------------

//清空所有全局变量
void resetAllGlobalVariables(){
    lalr1dfa.clear();
    grammarStr.clear();
    firstmap.clear();
    myrules.clear();
    terminal.clear();
    nonterminal.clear();
    formula.clear();
    followmap.clear();
    forwards.clear();
//    backwards.clear();
    LR1forwards.clear();
    LR1backwards.clear();
    transsymbols.clear();
    treeAction.clear();
    codeAnalyzeResLines.clear();
}

void Widget::on_analyze_clicked()
{
    resetAllGlobalVariables();
    //清除所有内容,设置换行变色
    initTable(ui->firstTable);
    initTable(ui->followTable);
    initTable(ui->LALR1DFAGraphTable);
    initTable(ui->LALR1AnalyzeTable_2);
    initTable(ui->LR1DFAGraphTable);
    initTable(ui->codeAnalyzeTable);


    QString textString = ui->plainTextEdit->toPlainText();

    grammarStr = textString.toStdString();
    // 改进文法
    QTextBrowser* broswer =  ui->textBrowser_3;


    preProcess();
    processInput(broswer);
    initFirst();
    initFollow();


    showFirst(ui->firstTable);
    showFollow(ui->followTable);

    generateTransSymbols();

    LR0dfa lr0dfa = generateLR0dfa();
//    testLR1Closure();
    LR1dfa lr1dfa = generateLR1dfa();
    showLR1dfa(lr1dfa, ui->LR1DFAGraphTable);

    lalr1dfa =  mergeLR0andLR1ToLALR(lr1dfa, lr0dfa);
    showLALR1dfa(lalr1dfa, ui->LALR1DFAGraphTable);
    if(!checkLALR1confilct(lalr1dfa))
        QMessageBox::warning(this, "警告", "非LALR文法");
    else  // 如果不发生冲突
        showLALR1AnalyzeTable(lalr1dfa, ui->LALR1AnalyzeTable_2);
    qDebug() << "analyze finish";

    //弹窗提醒用户计算完成
    analyzeCompleteMsg("文法");

}







