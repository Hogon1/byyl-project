/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QTabWidget *tabWidget_2;
    QWidget *tab_6;
    QTabWidget *tabWidget;
    QWidget *tab_5;
    QPlainTextEdit *plainTextEdit_2;
    QPushButton *pushButton_6;
    QCheckBox *checkBox;
    QPushButton *pushButton;
    QPushButton *pushButton_7;
    QTextBrowser *textBrowser_2;
    QLabel *label_7;
    QWidget *NFAWidget;
    QTableWidget *NFAtable;
    QWidget *DFAWidget;
    QTableWidget *DFAtable;
    QWidget *tab;
    QTableWidget *minDFAtable;
    QWidget *tab_3;
    QPlainTextEdit *sampleEdit;
    QPushButton *openSampleButton;
    QPushButton *saveSampleButton;
    QLabel *label_14;
    QWidget *tab_2;
    QTextBrowser *codeBrowser;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QLabel *label_15;
    QLabel *label_17;
    QTextBrowser *textBrowser_5;
    QWidget *tab_4;
    QTextBrowser *lexBrewser;
    QPushButton *openLexButton;
    QLabel *label_16;
    QTextBrowser *lexCodeBrowser;
    QLabel *label_18;
    QLabel *label_19;
    QTextBrowser *textBrowser;
    QWidget *tab_7;
    QTextBrowser *textBrowser_4;
    QTabWidget *tabWidget_3;
    QWidget *tab_16;
    QPlainTextEdit *plainTextEdit;
    QPushButton *analyze;
    QPushButton *saveFile;
    QPushButton *openFile;
    QWidget *tab_8;
    QTextBrowser *textBrowser_3;
    QWidget *tab_9;
    QTableWidget *firstTable;
    QTableWidget *followTable;
    QWidget *tab_11;
    QTableWidget *LR1DFAGraphTable;
    QWidget *tab_12;
    QTableWidget *LALR1DFAGraphTable;
    QWidget *tab_13;
    QTableWidget *LALR1AnalyzeTable_2;
    QWidget *tab_14;
    QTabWidget *tabWidget_4;
    QWidget *tab_15;
    QPlainTextEdit *codePlainTextEdit;
    QPushButton *codeAnalyzeBtn;
    QLabel *label_3;
    QPlainTextEdit *codeSemanticPlainTextEdit;
    QLabel *label_4;
    QPushButton *openCodeFileBtn;
    QPushButton *openSemanticBtn;
    QWidget *tab_17;
    QTableWidget *codeAnalyzeTable;
    QWidget *tab_18;
    QTextBrowser *simpleTreeBrowser;
    QWidget *tab_19;
    QTreeWidget *codeTreeWidget;
    QWidget *tab_20;
    QTabWidget *tabWidget_5;
    QWidget *tab_21;
    QLabel *label_6;
    QPushButton *openLexFileBtn;
    QPushButton *AnalyzeLex;
    QLabel *label_8;
    QPushButton *openLexSemanticBtn;
    QPlainTextEdit *lexEdit;
    QPlainTextEdit *lexSemanticEdit;
    QWidget *tab_10;
    QTableWidget *lexAnalyzeTable;
    QWidget *tab_22;
    QTextBrowser *lexTreeBroswer;
    QWidget *tab_23;
    QTreeWidget *lexTreeWidget;
    QLabel *label_9;
    QLabel *label_10;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(1504, 830);
        tabWidget_2 = new QTabWidget(Widget);
        tabWidget_2->setObjectName(QString::fromUtf8("tabWidget_2"));
        tabWidget_2->setGeometry(QRect(10, 160, 1461, 661));
        QFont font;
        font.setFamily(QString::fromUtf8("\347\255\211\347\272\277"));
        font.setPointSize(13);
        tabWidget_2->setFont(font);
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        tabWidget = new QTabWidget(tab_6);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(370, 10, 1051, 601));
        tabWidget->setFont(font);
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        plainTextEdit_2 = new QPlainTextEdit(tab_5);
        plainTextEdit_2->setObjectName(QString::fromUtf8("plainTextEdit_2"));
        plainTextEdit_2->setGeometry(QRect(10, 200, 791, 351));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Consolas"));
        font1.setPointSize(14);
        plainTextEdit_2->setFont(font1);
        pushButton_6 = new QPushButton(tab_5);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(820, 170, 191, 51));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font2.setPointSize(12);
        pushButton_6->setFont(font2);
        checkBox = new QCheckBox(tab_5);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(840, 40, 141, 41));
        checkBox->setFont(font2);
        checkBox->setTristate(false);
        pushButton = new QPushButton(tab_5);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(820, 450, 191, 61));
        pushButton->setFont(font2);
        pushButton_7 = new QPushButton(tab_5);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setGeometry(QRect(820, 300, 191, 61));
        pushButton_7->setFont(font2);
        textBrowser_2 = new QTextBrowser(tab_5);
        textBrowser_2->setObjectName(QString::fromUtf8("textBrowser_2"));
        textBrowser_2->setGeometry(QRect(10, 10, 791, 131));
        label_7 = new QLabel(tab_5);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 160, 491, 31));
        QFont font3;
        font3.setFamily(QString::fromUtf8("\347\255\211\347\272\277"));
        font3.setPointSize(15);
        label_7->setFont(font3);
        tabWidget->addTab(tab_5, QString());
        NFAWidget = new QWidget();
        NFAWidget->setObjectName(QString::fromUtf8("NFAWidget"));
        NFAtable = new QTableWidget(NFAWidget);
        NFAtable->setObjectName(QString::fromUtf8("NFAtable"));
        NFAtable->setGeometry(QRect(10, 10, 1021, 541));
        tabWidget->addTab(NFAWidget, QString());
        DFAWidget = new QWidget();
        DFAWidget->setObjectName(QString::fromUtf8("DFAWidget"));
        DFAtable = new QTableWidget(DFAWidget);
        DFAtable->setObjectName(QString::fromUtf8("DFAtable"));
        DFAtable->setGeometry(QRect(10, 10, 1021, 541));
        DFAtable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        DFAtable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        DFAtable->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        tabWidget->addTab(DFAWidget, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        minDFAtable = new QTableWidget(tab);
        minDFAtable->setObjectName(QString::fromUtf8("minDFAtable"));
        minDFAtable->setGeometry(QRect(10, 10, 1021, 541));
        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        sampleEdit = new QPlainTextEdit(tab_3);
        sampleEdit->setObjectName(QString::fromUtf8("sampleEdit"));
        sampleEdit->setGeometry(QRect(10, 80, 811, 451));
        sampleEdit->setFont(font);
        openSampleButton = new QPushButton(tab_3);
        openSampleButton->setObjectName(QString::fromUtf8("openSampleButton"));
        openSampleButton->setGeometry(QRect(840, 150, 161, 61));
        QFont font4;
        font4.setFamily(QString::fromUtf8("\351\234\236\351\271\234\346\226\207\346\245\267"));
        font4.setPointSize(12);
        openSampleButton->setFont(font4);
        saveSampleButton = new QPushButton(tab_3);
        saveSampleButton->setObjectName(QString::fromUtf8("saveSampleButton"));
        saveSampleButton->setGeometry(QRect(840, 280, 161, 51));
        saveSampleButton->setFont(font4);
        label_14 = new QLabel(tab_3);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(30, 30, 901, 41));
        label_14->setFont(font);
        tabWidget->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        codeBrowser = new QTextBrowser(tab_2);
        codeBrowser->setObjectName(QString::fromUtf8("codeBrowser"));
        codeBrowser->setGeometry(QRect(10, 160, 1021, 381));
        QFont font5;
        font5.setFamily(QString::fromUtf8("\347\255\211\347\272\277"));
        font5.setPointSize(12);
        codeBrowser->setFont(font5);
        pushButton_2 = new QPushButton(tab_2);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(760, 20, 251, 51));
        pushButton_2->setFont(font);
        pushButton_3 = new QPushButton(tab_2);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(760, 90, 251, 51));
        pushButton_3->setFont(font);
        label_15 = new QLabel(tab_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(30, 60, 791, 31));
        QFont font6;
        font6.setFamily(QString::fromUtf8("\351\234\236\351\271\234\346\226\207\346\245\267"));
        font6.setPointSize(13);
        label_15->setFont(font6);
        label_17 = new QLabel(tab_2);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(30, 100, 661, 31));
        label_17->setFont(font6);
        textBrowser_5 = new QTextBrowser(tab_2);
        textBrowser_5->setObjectName(QString::fromUtf8("textBrowser_5"));
        textBrowser_5->setGeometry(QRect(20, 10, 731, 141));
        textBrowser_5->setFont(font5);
        tabWidget->addTab(tab_2, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        lexBrewser = new QTextBrowser(tab_4);
        lexBrewser->setObjectName(QString::fromUtf8("lexBrewser"));
        lexBrewser->setGeometry(QRect(20, 80, 411, 471));
        lexBrewser->setFont(font5);
        openLexButton = new QPushButton(tab_4);
        openLexButton->setObjectName(QString::fromUtf8("openLexButton"));
        openLexButton->setGeometry(QRect(830, 330, 191, 41));
        openLexButton->setFont(font5);
        label_16 = new QLabel(tab_4);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(830, 200, 201, 61));
        label_16->setFont(font);
        lexCodeBrowser = new QTextBrowser(tab_4);
        lexCodeBrowser->setObjectName(QString::fromUtf8("lexCodeBrowser"));
        lexCodeBrowser->setGeometry(QRect(450, 80, 351, 471));
        lexCodeBrowser->setFont(font5);
        label_18 = new QLabel(tab_4);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(170, 50, 91, 31));
        label_18->setFont(font);
        label_19 = new QLabel(tab_4);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(460, 50, 361, 31));
        label_19->setFont(font);
        tabWidget->addTab(tab_4, QString());
        textBrowser = new QTextBrowser(tab_6);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(10, 10, 351, 591));
        tabWidget_2->addTab(tab_6, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName(QString::fromUtf8("tab_7"));
        textBrowser_4 = new QTextBrowser(tab_7);
        textBrowser_4->setObjectName(QString::fromUtf8("textBrowser_4"));
        textBrowser_4->setGeometry(QRect(10, 10, 351, 591));
        tabWidget_3 = new QTabWidget(tab_7);
        tabWidget_3->setObjectName(QString::fromUtf8("tabWidget_3"));
        tabWidget_3->setGeometry(QRect(370, 10, 1061, 611));
        tabWidget_3->setFont(font);
        tab_16 = new QWidget();
        tab_16->setObjectName(QString::fromUtf8("tab_16"));
        plainTextEdit = new QPlainTextEdit(tab_16);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 10, 831, 541));
        plainTextEdit->setFont(font);
        analyze = new QPushButton(tab_16);
        analyze->setObjectName(QString::fromUtf8("analyze"));
        analyze->setGeometry(QRect(870, 380, 171, 51));
        QFont font7;
        font7.setFamily(QString::fromUtf8("\351\234\236\351\271\234\346\226\207\346\245\267"));
        font7.setPointSize(14);
        analyze->setFont(font7);
        saveFile = new QPushButton(tab_16);
        saveFile->setObjectName(QString::fromUtf8("saveFile"));
        saveFile->setGeometry(QRect(870, 100, 171, 51));
        saveFile->setFont(font7);
        openFile = new QPushButton(tab_16);
        openFile->setObjectName(QString::fromUtf8("openFile"));
        openFile->setGeometry(QRect(870, 240, 171, 51));
        openFile->setFont(font7);
        tabWidget_3->addTab(tab_16, QString());
        tab_8 = new QWidget();
        tab_8->setObjectName(QString::fromUtf8("tab_8"));
        textBrowser_3 = new QTextBrowser(tab_8);
        textBrowser_3->setObjectName(QString::fromUtf8("textBrowser_3"));
        textBrowser_3->setGeometry(QRect(10, 10, 1031, 551));
        textBrowser_3->setFont(font);
        tabWidget_3->addTab(tab_8, QString());
        tab_9 = new QWidget();
        tab_9->setObjectName(QString::fromUtf8("tab_9"));
        firstTable = new QTableWidget(tab_9);
        firstTable->setObjectName(QString::fromUtf8("firstTable"));
        firstTable->setGeometry(QRect(20, 10, 501, 541));
        QFont font8;
        font8.setFamily(QString::fromUtf8("\347\255\211\347\272\277"));
        font8.setPointSize(11);
        firstTable->setFont(font8);
        followTable = new QTableWidget(tab_9);
        followTable->setObjectName(QString::fromUtf8("followTable"));
        followTable->setGeometry(QRect(540, 10, 501, 541));
        followTable->setFont(font8);
        tabWidget_3->addTab(tab_9, QString());
        tab_11 = new QWidget();
        tab_11->setObjectName(QString::fromUtf8("tab_11"));
        LR1DFAGraphTable = new QTableWidget(tab_11);
        LR1DFAGraphTable->setObjectName(QString::fromUtf8("LR1DFAGraphTable"));
        LR1DFAGraphTable->setGeometry(QRect(10, 10, 1031, 541));
        tabWidget_3->addTab(tab_11, QString());
        tab_12 = new QWidget();
        tab_12->setObjectName(QString::fromUtf8("tab_12"));
        LALR1DFAGraphTable = new QTableWidget(tab_12);
        LALR1DFAGraphTable->setObjectName(QString::fromUtf8("LALR1DFAGraphTable"));
        LALR1DFAGraphTable->setGeometry(QRect(10, 10, 1031, 541));
        tabWidget_3->addTab(tab_12, QString());
        tab_13 = new QWidget();
        tab_13->setObjectName(QString::fromUtf8("tab_13"));
        LALR1AnalyzeTable_2 = new QTableWidget(tab_13);
        LALR1AnalyzeTable_2->setObjectName(QString::fromUtf8("LALR1AnalyzeTable_2"));
        LALR1AnalyzeTable_2->setGeometry(QRect(10, 10, 1021, 541));
        tabWidget_3->addTab(tab_13, QString());
        tab_14 = new QWidget();
        tab_14->setObjectName(QString::fromUtf8("tab_14"));
        tabWidget_4 = new QTabWidget(tab_14);
        tabWidget_4->setObjectName(QString::fromUtf8("tabWidget_4"));
        tabWidget_4->setGeometry(QRect(10, 10, 1031, 551));
        tab_15 = new QWidget();
        tab_15->setObjectName(QString::fromUtf8("tab_15"));
        codePlainTextEdit = new QPlainTextEdit(tab_15);
        codePlainTextEdit->setObjectName(QString::fromUtf8("codePlainTextEdit"));
        codePlainTextEdit->setGeometry(QRect(10, 60, 381, 441));
        codePlainTextEdit->setFont(font5);
        codeAnalyzeBtn = new QPushButton(tab_15);
        codeAnalyzeBtn->setObjectName(QString::fromUtf8("codeAnalyzeBtn"));
        codeAnalyzeBtn->setGeometry(QRect(800, 350, 201, 51));
        codeAnalyzeBtn->setFont(font);
        label_3 = new QLabel(tab_15);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(60, 0, 301, 61));
        label_3->setFont(font);
        codeSemanticPlainTextEdit = new QPlainTextEdit(tab_15);
        codeSemanticPlainTextEdit->setObjectName(QString::fromUtf8("codeSemanticPlainTextEdit"));
        codeSemanticPlainTextEdit->setGeometry(QRect(410, 60, 381, 441));
        codeSemanticPlainTextEdit->setFont(font5);
        label_4 = new QLabel(tab_15);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(500, 10, 211, 41));
        label_4->setFont(font);
        openCodeFileBtn = new QPushButton(tab_15);
        openCodeFileBtn->setObjectName(QString::fromUtf8("openCodeFileBtn"));
        openCodeFileBtn->setGeometry(QRect(800, 140, 201, 51));
        openCodeFileBtn->setFont(font);
        openSemanticBtn = new QPushButton(tab_15);
        openSemanticBtn->setObjectName(QString::fromUtf8("openSemanticBtn"));
        openSemanticBtn->setGeometry(QRect(800, 240, 201, 51));
        openSemanticBtn->setFont(font);
        tabWidget_4->addTab(tab_15, QString());
        tab_17 = new QWidget();
        tab_17->setObjectName(QString::fromUtf8("tab_17"));
        codeAnalyzeTable = new QTableWidget(tab_17);
        codeAnalyzeTable->setObjectName(QString::fromUtf8("codeAnalyzeTable"));
        codeAnalyzeTable->setGeometry(QRect(10, 10, 1001, 491));
        tabWidget_4->addTab(tab_17, QString());
        tab_18 = new QWidget();
        tab_18->setObjectName(QString::fromUtf8("tab_18"));
        simpleTreeBrowser = new QTextBrowser(tab_18);
        simpleTreeBrowser->setObjectName(QString::fromUtf8("simpleTreeBrowser"));
        simpleTreeBrowser->setGeometry(QRect(10, 10, 1001, 491));
        QFont font9;
        font9.setFamily(QString::fromUtf8("Consolas"));
        font9.setPointSize(13);
        simpleTreeBrowser->setFont(font9);
        tabWidget_4->addTab(tab_18, QString());
        tab_19 = new QWidget();
        tab_19->setObjectName(QString::fromUtf8("tab_19"));
        codeTreeWidget = new QTreeWidget(tab_19);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        codeTreeWidget->setHeaderItem(__qtreewidgetitem);
        codeTreeWidget->setObjectName(QString::fromUtf8("codeTreeWidget"));
        codeTreeWidget->setGeometry(QRect(10, 10, 1001, 491));
        codeTreeWidget->setFont(font8);
        tabWidget_4->addTab(tab_19, QString());
        tabWidget_3->addTab(tab_14, QString());
        tab_20 = new QWidget();
        tab_20->setObjectName(QString::fromUtf8("tab_20"));
        tabWidget_5 = new QTabWidget(tab_20);
        tabWidget_5->setObjectName(QString::fromUtf8("tabWidget_5"));
        tabWidget_5->setGeometry(QRect(10, 10, 1031, 551));
        tabWidget_5->setFont(font);
        tab_21 = new QWidget();
        tab_21->setObjectName(QString::fromUtf8("tab_21"));
        label_6 = new QLabel(tab_21);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(140, 20, 131, 31));
        label_6->setFont(font);
        openLexFileBtn = new QPushButton(tab_21);
        openLexFileBtn->setObjectName(QString::fromUtf8("openLexFileBtn"));
        openLexFileBtn->setGeometry(QRect(800, 140, 201, 51));
        openLexFileBtn->setFont(font);
        AnalyzeLex = new QPushButton(tab_21);
        AnalyzeLex->setObjectName(QString::fromUtf8("AnalyzeLex"));
        AnalyzeLex->setGeometry(QRect(800, 350, 201, 51));
        AnalyzeLex->setFont(font);
        label_8 = new QLabel(tab_21);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(500, 10, 191, 41));
        label_8->setFont(font);
        openLexSemanticBtn = new QPushButton(tab_21);
        openLexSemanticBtn->setObjectName(QString::fromUtf8("openLexSemanticBtn"));
        openLexSemanticBtn->setGeometry(QRect(800, 240, 201, 51));
        openLexSemanticBtn->setFont(font);
        lexEdit = new QPlainTextEdit(tab_21);
        lexEdit->setObjectName(QString::fromUtf8("lexEdit"));
        lexEdit->setGeometry(QRect(10, 60, 381, 441));
        lexEdit->setFont(font5);
        lexSemanticEdit = new QPlainTextEdit(tab_21);
        lexSemanticEdit->setObjectName(QString::fromUtf8("lexSemanticEdit"));
        lexSemanticEdit->setGeometry(QRect(410, 60, 381, 441));
        lexSemanticEdit->setFont(font5);
        tabWidget_5->addTab(tab_21, QString());
        label_8->raise();
        AnalyzeLex->raise();
        openLexFileBtn->raise();
        label_6->raise();
        openLexSemanticBtn->raise();
        lexEdit->raise();
        lexSemanticEdit->raise();
        tab_10 = new QWidget();
        tab_10->setObjectName(QString::fromUtf8("tab_10"));
        lexAnalyzeTable = new QTableWidget(tab_10);
        lexAnalyzeTable->setObjectName(QString::fromUtf8("lexAnalyzeTable"));
        lexAnalyzeTable->setGeometry(QRect(10, 10, 1001, 491));
        QFont font10;
        font10.setFamily(QString::fromUtf8("\347\255\211\347\272\277"));
        font10.setPointSize(10);
        lexAnalyzeTable->setFont(font10);
        tabWidget_5->addTab(tab_10, QString());
        tab_22 = new QWidget();
        tab_22->setObjectName(QString::fromUtf8("tab_22"));
        lexTreeBroswer = new QTextBrowser(tab_22);
        lexTreeBroswer->setObjectName(QString::fromUtf8("lexTreeBroswer"));
        lexTreeBroswer->setGeometry(QRect(10, 10, 1001, 491));
        lexTreeBroswer->setFont(font9);
        tabWidget_5->addTab(tab_22, QString());
        tab_23 = new QWidget();
        tab_23->setObjectName(QString::fromUtf8("tab_23"));
        lexTreeWidget = new QTreeWidget(tab_23);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(0, QString::fromUtf8("1"));
        lexTreeWidget->setHeaderItem(__qtreewidgetitem1);
        lexTreeWidget->setObjectName(QString::fromUtf8("lexTreeWidget"));
        lexTreeWidget->setGeometry(QRect(10, 10, 1001, 491));
        tabWidget_5->addTab(tab_23, QString());
        tabWidget_3->addTab(tab_20, QString());
        tabWidget_2->addTab(tab_7, QString());
        label_9 = new QLabel(Widget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(180, 50, 1211, 51));
        QFont font11;
        font11.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font11.setPointSize(23);
        font11.setBold(true);
        font11.setWeight(75);
        label_9->setFont(font11);
        label_10 = new QLabel(Widget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(380, 112, 671, 31));
        QFont font12;
        font12.setPointSize(15);
        label_10->setFont(font12);

        retranslateUi(Widget);

        tabWidget_2->setCurrentIndex(1);
        tabWidget->setCurrentIndex(0);
        tabWidget_3->setCurrentIndex(7);
        tabWidget_4->setCurrentIndex(0);
        tabWidget_5->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
#if QT_CONFIG(tooltip)
        plainTextEdit_2->setToolTip(QCoreApplication::translate("Widget", "<html><head/><body><p><br/></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        plainTextEdit_2->setPlainText(QString());
        plainTextEdit_2->setPlaceholderText(QString());
        pushButton_6->setText(QCoreApplication::translate("Widget", "\344\270\212\344\274\240\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217", nullptr));
        checkBox->setText(QCoreApplication::translate("Widget", "\345\277\275\347\225\245\345\244\247\345\260\217\345\206\231\345\214\272\345\210\253", nullptr));
        pushButton->setText(QCoreApplication::translate("Widget", "\345\274\200\345\247\213\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217\345\210\206\346\236\220", nullptr));
        pushButton_7->setText(QCoreApplication::translate("Widget", "\344\277\235\345\255\230\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217", nullptr));
        textBrowser_2->setHtml(QCoreApplication::translate("Widget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'\347\255\211\347\272\277','\347\255\211\347\272\277'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt;\">\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217\350\276\223\345\205\245\350\247\204\345\210\231\357\274\232</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt;\">1. \346\234\254\351\241\271\347\233\256</span><span style=\" font-family:'SimSun'; font-size:12pt; font-weight:600;\">\346\224\257\346\214\201\345\221\275\345"
                        "\220\215\346\223\215\344\275\234</span><span style=\" font-family:'SimSun'; font-size:12pt;\">\357\274\214\350\276\223\345\205\245\344\270\255</span><span style=\" font-family:'SimSun'; font-size:12pt; font-weight:600;\">\345\277\205\351\241\273\345\214\205\345\220\253</span><span style=\" font-family:'SimSun'; font-size:12pt;\">\350\277\231\344\272\233\345\221\275\345\220\215\357\274\232operators,operators_name,letter,digit,num,comment,_final\345\205\266\344\270\255</span><span style=\" font-family:'SimSun'; font-size:12pt; font-weight:600;\">\345\212\240\344\270\213\345\210\222\347\272\277\347\232\204_final</span><span style=\" font-family:'SimSun'; font-size:12pt;\">\350\241\250\347\244\272\350\257\245\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217\351\234\200\350\246\201\347\224\237\346\210\220DFA\345\233\276</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt"
                        ";\">2. \346\211\200\346\234\211\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217\344\270\255\350\276\223\345\205\245\344\270\255\345\217\252\350\203\275\346\234\211\344\270\200\344\270\252_final</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt;\">3. \346\234\254\351\241\271\347\233\256\346\224\257\346\214\201\347\232\204\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217\350\277\220\347\256\227\347\254\246\345\217\267\357\274\232\350\275\254\344\271\211\347\254\246\345\217\267()\343\200\201\350\277\236\346\216\245\343\200\201 \351\200\211\346\213\251(|)\343\200\201 \\n \351\227\255\345\214\205(*)\343\200\201 \346\255\243\351\227\255\345\214\205\357\274\210+\357\274\211\343\200\201 [ ]\343\200\201 \345\217\257\351\200\211\357\274\210?\357\274\211\343\200\201 \346\213\254\345\217\267 ( )</span></p></body></html>", nullptr));
        label_7->setText(QCoreApplication::translate("Widget", "\350\257\267\345\234\250\344\273\245\344\270\213\347\252\227\345\217\243\350\276\223\345\205\245\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217:", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QCoreApplication::translate("Widget", "\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(NFAWidget), QCoreApplication::translate("Widget", "NFA", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(DFAWidget), QCoreApplication::translate("Widget", "DFA", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("Widget", "\346\234\200\345\260\217\345\214\226DFA", nullptr));
        openSampleButton->setText(QCoreApplication::translate("Widget", "\346\211\223\345\274\200sample\346\226\207\344\273\266", nullptr));
        saveSampleButton->setText(QCoreApplication::translate("Widget", "\344\277\235\345\255\230sample\346\226\207\344\273\266", nullptr));
        label_14->setText(QCoreApplication::translate("Widget", "\350\257\264\346\230\216\357\274\232sample\346\226\207\344\273\266\346\230\257\350\257\215\346\263\225\345\210\206\346\236\220\347\250\213\345\272\217\346\211\200\350\246\201\345\210\206\346\236\220\347\232\204\345\257\271\350\261\241\357\274\214\346\202\250\345\217\257\344\273\245\345\234\250\350\277\231\351\207\214\347\274\226\350\276\221\345\271\266\344\277\235\345\255\230sample\346\226\207\344\273\266\343\200\202", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("Widget", "\347\274\226\350\276\221/\344\277\235\345\255\230sample\346\226\207\344\273\266", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Widget", "\347\224\237\346\210\220\345\271\266\344\277\235\345\255\230\350\257\215\346\263\225\345\210\206\346\236\220\346\272\220\347\240\201", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Widget", "\347\274\226\350\257\221\350\277\220\350\241\214\344\273\243\347\240\201", nullptr));
        label_15->setText(QString());
        label_17->setText(QString());
        textBrowser_5->setHtml(QCoreApplication::translate("Widget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'\347\255\211\347\272\277','\347\255\211\347\272\277','\347\255\211\347\272\277'; font-size:12pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'\351\234\236\351\271\234\346\226\207\346\245\267'; font-size:13pt;\">\350\257\264\346\230\216\357\274\2321. \346\202\250\351\234\200\350\246\201\345\205\210\351\200\211\346\213\251\346\272\220\347\240\201\344\277\235\345\255\230\344\275\215\347\275\256\357\274\214\350\276\223\345\205\245\346\226\207\344\273\266\345\220\216\347\274\200\345\220\215\357\274\214\346\211\215\345\217\257\344\273\245\346\265\217\350\247\210\346\272\220\347\240\201\357\274\2332. \344\277\235"
                        "\345\255\230\346\272\220\347\240\201\345\220\216\357\274\214\346\211\215\345\217\257\344\273\245\347\274\226\350\257\221\350\277\220\350\241\214\346\272\220\347\240\201\357\274\214\346\263\250\346\204\217\351\234\200\350\246\201\345\260\206sample\346\226\207\344\273\266\344\277\235\345\255\230\345\210\260\347\233\270\345\220\214\347\232\204\350\267\257\345\276\204\344\270\213\357\274\2333. \345\246\202\346\236\234\350\277\220\350\241\214\346\210\220\345\212\237\357\274\214\350\277\220\350\241\214\347\232\204\347\273\223\346\236\234\345\260\206\345\261\225\347\244\272\345\234\250&quot;lex&quot;\351\200\211\351\241\271\345\215\241\344\270\255\343\200\202</span></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("Widget", "\350\257\215\346\263\225\345\210\206\346\236\220\346\272\220\347\250\213\345\272\217", nullptr));
        openLexButton->setText(QCoreApplication::translate("Widget", "\346\211\223\345\274\200lex\346\226\207\344\273\266", nullptr));
        label_16->setText(QCoreApplication::translate("Widget", "\346\202\250\344\271\237\345\217\257\344\273\245\347\202\271\345\207\273\346\214\211\351\222\256\n"
"\346\211\213\345\212\250\346\211\223\345\274\200lex\346\226\207\344\273\266", nullptr));
        label_18->setText(QCoreApplication::translate("Widget", "lex\347\274\226\347\240\201", nullptr));
        label_19->setText(QCoreApplication::translate("Widget", "lex\347\274\226\347\240\201\345\257\271\347\205\247\350\241\250\357\274\210\347\224\261\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217\347\224\237\346\210\220\357\274\211", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("Widget", "lex", nullptr));
        textBrowser->setHtml(QCoreApplication::translate("Widget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'\347\255\211\347\272\277'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt;\">\344\275\277\347\224\250\346\255\245\351\252\244</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt;\">1.\350\257\267\345\205\210\346\237\245\347\234\213\345\271\266\351\230\205\350\257\273</span><span style=\" font-family:'SimSun'; font-size:12pt; font-weight:600;\">\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217\347\232\204\350"
                        "\276\223\345\205\245\350\247\204\345\210\231</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt;\">2.\350\257\267\346\211\213\345\212\250\350\276\223\345\205\245\346\255\243\345\210\231\350\241\250\350\276\276\346\210\226\346\211\223\345\274\200\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217\346\226\207\344\273\266\357\274\214\345\220\214\346\227\266\345\217\257\344\273\245\346\240\271\346\215\256\350\257\255\350\250\200\347\211\271\346\200\247\351\200\211\346\213\251\346\230\257\345\220\246&quot;\345\277\275\347\225\245\345\244\247\345\260\217\345\206\231&quot;\343\200\202</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt;\">3.\347\202\271\345\207\273&quot;\345\274\200\345\247\213\345\210\206\346\236\220&quot;</span></p>\n"
"<"
                        "p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt;\">4.\345\210\206\346\236\220\345\256\214\346\257\225\345\220\216\357\274\214\345\206\215\347\202\271\345\207\273\345\205\266\344\273\226\346\214\211\351\222\256\346\237\245\347\234\213\347\273\223\346\236\234\357\274\232</span><span style=\" font-family:'SimSun'; font-size:12pt; font-weight:600;\">&quot;NFA&quot;,&quot;DFA&quot;,&quot;\346\234\200\345\260\217\345\214\226DFA&quot;</span><span style=\" font-family:'SimSun'; font-size:12pt;\">\357\274\214\344\273\245\344\270\212\344\270\211\344\270\252\345\210\206\346\236\220\347\273\223\346\236\234\345\235\207\344\273\245\347\212\266\346\200\201\350\275\254\346\215\242\350\241\250\347\232\204\345\275\242\345\274\217\345\221\210\347\216\260</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font"
                        "-family:'SimSun'; font-size:12pt;\">5.\351\200\211\346\213\251&quot;</span><span style=\" font-family:'SimSun'; font-size:12pt; font-weight:600;\">\350\257\215\346\263\225\345\210\206\346\236\220\346\272\220\347\250\213\345\272\217</span><span style=\" font-family:'SimSun'; font-size:12pt;\">&quot;\351\200\211\351\241\271\357\274\214\347\202\271\345\207\273&quot;\344\277\235\345\255\230\346\272\220\347\240\201&quot;\346\214\211\351\222\256\357\274\214\346\272\220\347\240\201\345\260\206\344\273\245lexer.c\346\226\207\344\273\266\344\277\235\345\255\230\345\210\260\346\214\207\345\256\232\350\267\257\345\276\204\344\270\213\343\200\202</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt;\">6.\351\200\211\346\213\251&quot;sanple&quot;\351\200\211\351\241\271\357\274\214\347\224\250\346\210\267\345\217\257\344\273\245\345\234\250\350\257\245\347\252\227\345\217\243\344\270\213\347"
                        "\274\226\350\276\221\346\210\226\346\211\223\345\274\200\346\237\245\347\234\213sample\346\226\207\344\273\266\357\274\214\345\220\214\346\227\266\351\234\200\350\246\201\345\210\266\345\256\232\346\226\207\344\273\266\345\220\216\347\274\200</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt;\">7.\347\224\261\344\272\216\350\257\215\346\263\225\345\210\206\346\236\220\346\272\220\347\250\213\345\272\217\344\270\255\345\206\205\347\275\256\344\272\206\346\226\207\344\273\266\350\257\273\345\206\231\347\232\204\344\273\243\347\240\201\357\274\214\345\233\240\346\255\244\345\234\250&quot;\350\257\215\346\263\225\345\210\206\346\236\220\346\272\220\347\250\213\345\272\217&quot;\351\200\211\351\241\271\344\270\255\357\274\214\351\234\200\350\246\201\347\224\250\346\210\267\346\211\213\345\212\250\350\276\223\345\205\245\350\246\201\345\210\206\346\236\220\347\232\204sample\346\226"
                        "\207\344\273\266\347\232\204\345\220\216\347\274\200\357\274\214\345\217\257\344\273\245\350\276\223\345\205\245tny\346\210\226minic\343\200\202\346\272\220\347\240\201\344\277\235\345\255\230\345\256\214\346\257\225\345\220\216\357\274\214\347\202\271\345\207\273</span><span style=\" font-family:'SimSun'; font-size:12pt; font-weight:600;\">&quot;\347\274\226\350\257\221\350\277\220\350\241\214\346\272\220\347\240\201</span><span style=\" font-family:'SimSun'; font-size:12pt;\">&quot;\345\215\263\345\217\257\345\257\271\350\277\220\350\241\214\350\257\215\346\263\225\345\210\206\346\236\220\347\250\213\345\272\217\343\200\202\347\255\211\345\276\205\344\270\200\346\256\265\346\227\266\351\227\264\345\220\216\357\274\214\345\207\272\347\216\260\345\274\271\347\252\227\357\274\214\346\217\220\347\244\272\350\277\220\350\241\214\345\256\214\346\257\225\357\274\214\345\217\257\345\234\250&quot;</span><span style=\" font-family:'SimSun'; font-size:12pt; font-weight:600;\">lex</span><span style=\" font-family:'SimSu"
                        "n'; font-size:12pt;\">&quot;\351\200\211\351\241\271\344\270\213\347\233\264\346\216\245\346\237\245\347\234\213lex\346\226\207\344\273\266\347\232\204\345\206\205\345\256\271\357\274\214\345\215\263\345\217\257\346\237\245\347\234\213\345\257\271sample\346\226\207\344\273\266\347\232\204\346\211\200\346\234\211</span><span style=\" font-family:'SimSun'; font-size:12pt; font-weight:600;\">\345\215\225\350\257\215\347\274\226\347\240\201</span></p></body></html>", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_6), QCoreApplication::translate("Widget", "\344\273\273\345\212\2411 \350\257\215\346\263\225\345\210\206\346\236\220", nullptr));
        textBrowser_4->setHtml(QCoreApplication::translate("Widget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'\347\255\211\347\272\277'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt;\">\344\275\277\347\224\250\346\255\245\351\252\244\357\274\232</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt;\">1.\350\257\267\345\205\210\350\276\223\345\205\245\346\210\226\346\211\223\345\274\200BNF\346\226\207\346\263\225\350\247\204\345\210\231\357\274\214\347\204\266\345\220\216\347\202\271\345\207\273&quot;\345\274\200\345\247\213\345\210\206\346\236"
                        "\220\346\226\207\346\263\225&quot;\357\274\214\350\200\220\345\277\203\347\255\211\345\276\205\345\210\206\346\236\220\346\210\220\345\212\237\347\232\204\345\274\271\347\252\227\345\207\272\347\216\260\357\274\214\347\202\271\345\207\273&quot;\347\241\256\345\256\232&quot;</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt;\">2.\347\204\266\345\220\216\345\217\257\344\273\245\346\265\217\350\247\210\345\205\266\344\273\226\351\200\211\351\241\271</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt;\"> 2.1&quot;\346\224\271\350\277\233\346\226\207\346\263\225&quot;\357\274\232\345\260\206\346\211\200\346\234\211\346\226\207\346\263\225\346\214\211 | \350\277\233\350\241\214\345\210\206\345\211\262\345\271\266\345\242\236\345\271\277\346\226\207"
                        "\346\263\225</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt;\"> 2.2&quot;</span><span style=\" font-family:'SimSun'; font-size:12pt; font-weight:600;\">FIRST&quot;\345\222\214&quot;FOLLOW&quot;</span><span style=\" font-family:'SimSun'; font-size:12pt;\">\357\274\232\345\261\225\347\244\272\346\211\200\346\234\211\351\235\236\347\273\210\347\273\223\347\254\246\345\217\267\347\232\204FIRST\345\222\214FOLLOW\351\233\206\345\220\210</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt;\"> 2.3</span><span style=\" font-family:'SimSun'; font-size:12pt; font-weight:600;\">&quot;LR(1)DFA&quot;\345\222\214LALR(1)</span><span style=\" font-family:'SimSun'; font-size:12pt;\">\357\274\232\345\210\206\345\210\253\345\261\225\347\244\272LR(1)\345\222\214"
                        "LALR(1)\347\232\204DFA\345\233\276\357\274\214\345\246\202\346\236\234\346\226\207\346\263\225</span><span style=\" font-family:'SimSun'; font-size:12pt; font-weight:600;\">\351\235\236LALR(1)\346\226\207\346\263\225\345\260\206\344\274\232\345\207\272\347\216\260\346\217\220\347\244\272</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt;\">3.\345\234\250&quot;\346\272\220\347\250\213\345\272\217\350\257\255\346\263\225\345\210\206\346\236\220\344\270\255&quot;\351\200\211\351\241\271\357\274\232</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt;\"> 3.1&quot;\346\272\220\347\250\213\345\272\217&quot;\357\274\232\350\276\223\345\205\245</span><span style=\" font-family:'SimSun'; font-size:12pt; font-weight:600;\">BNF\346\226\207\346\263\225\345"
                        "\257\271\345\272\224\347\232\204\346\272\220\347\250\213\345\272\217</span><span style=\" font-family:'SimSun'; font-size:12pt;\">\345\222\214\347\274\226\345\206\231\345\245\275\347\232\204\350\257\255\344\271\211\347\274\226\347\240\201\357\274\214\347\202\271\345\207\273&quot;\345\274\200\345\247\213\350\257\255\346\263\225\345\210\206\346\236\220&quot;\357\274\214\346\237\245\347\234\213\345\205\266\345\256\203\345\255\220\351\200\211\351\241\271</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt;\"> 3.1&quot;</span><span style=\" font-family:'SimSun'; font-size:12pt; font-weight:600;\">\350\207\252\345\272\225\345\220\221\344\270\212\351\200\220\345\217\245\345\210\206\346\236\220</span><span style=\" font-family:'SimSun'; font-size:12pt;\">&quot;\345\261\225\347\244\272\344\272\206\346\211\200\346\234\211\350\257\255\345\217\245\347\232\204</span><span style=\" font-fam"
                        "ily:'SimSun'; font-size:12pt; font-weight:600;\">\350\257\255\346\263\225\345\210\206\346\236\220\350\277\207\347\250\213</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt;\"> 3.2&quot;</span><span style=\" font-family:'SimSun'; font-size:12pt; font-weight:600;\">\350\257\255\346\263\225\346\240\221(\346\226\207\346\234\254\345\275\242\345\274\217)&quot;\345\222\214&quot;\350\257\255\346\263\225\346\240\221(\346\240\221\345\275\242)</span><span style=\" font-family:'SimSun'; font-size:12pt;\">&quot;\345\210\206\345\210\253\351\200\232\350\277\207\346\226\207\346\234\254\345\275\242\345\274\217\345\222\214\346\240\221\345\236\213\346\216\247\344\273\266\344\270\244\347\247\215\346\226\271\345\274\217\345\261\225\347\244\272\350\257\255\346\263\225\345\210\206\346\236\220\350\277\207\347\250\213\344\270\255\347\232\204</span><span style=\" font-family:'SimSun'; font-size:12pt;"
                        " font-weight:600;\">\350\257\255\346\263\225\346\240\221</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:12pt;\">4.\345\234\250&quot;</span><span style=\" font-family:'SimSun'; font-size:12pt; font-weight:600;\">lex\346\226\207\344\273\266\350\257\255\346\263\225\345\210\206\346\236\220</span><span style=\" font-family:'SimSun'; font-size:12pt;\">&quot;\351\200\211\351\241\271\357\274\214\350\276\223\345\205\245\346\210\226\346\211\223\345\274\200BNF\346\226\207\346\263\225\345\257\271\345\272\224\347\232\204\346\272\220\347\250\213\345\272\217\347\232\204lex\346\226\207\344\273\266\345\222\214\347\274\226\345\206\231\345\245\275\347\232\204\350\257\255\344\271\211\347\274\226\347\240\201\357\274\214\347\202\271\345\207\273&quot;\345\274\200\345\247\213\345\210\206\346\236\220lex&quot;\357\274\214\351\232\217\345\220\216\346\237\245\347\234\213\345\205\266\344\273\226\351\200\211"
                        "\351\241\271\357\274\214\350\257\255\346\263\225\346\240\221\347\232\204\351\200\211\351\241\271\345\220\2143</span></p></body></html>", nullptr));
        analyze->setText(QCoreApplication::translate("Widget", "\345\274\200\345\247\213\345\210\206\346\236\220\346\226\207\346\263\225", nullptr));
        saveFile->setText(QCoreApplication::translate("Widget", "\344\277\235\345\255\230\346\226\207\346\263\225\350\247\204\345\210\231", nullptr));
        openFile->setText(QCoreApplication::translate("Widget", "\346\211\223\345\274\200\346\226\207\346\263\225\350\247\204\345\210\231", nullptr));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tab_16), QCoreApplication::translate("Widget", "\350\276\223\345\205\245\346\226\207\346\263\225", nullptr));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tab_8), QCoreApplication::translate("Widget", "\346\224\271\350\277\233\346\226\207\346\263\225", nullptr));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tab_9), QCoreApplication::translate("Widget", "FIRST\345\222\214FOLLOW", nullptr));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tab_11), QCoreApplication::translate("Widget", "LR(1)DFA", nullptr));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tab_12), QCoreApplication::translate("Widget", "LALR(1)DFA", nullptr));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tab_13), QCoreApplication::translate("Widget", "LALR(1)Graph", nullptr));
        codeAnalyzeBtn->setText(QCoreApplication::translate("Widget", "\345\274\200\345\247\213\350\257\255\346\263\225\345\210\206\346\236\220", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "\346\226\207\346\263\225\345\257\271\345\272\224\347\232\204\346\272\220\347\250\213\345\272\217(\344\270\215\350\203\275\346\234\211\346\263\250\351\207\212)", nullptr));
        label_4->setText(QCoreApplication::translate("Widget", "\346\226\207\346\263\225\345\257\271\345\272\224\347\232\204\350\257\255\344\271\211\347\274\226\347\240\201", nullptr));
        openCodeFileBtn->setText(QCoreApplication::translate("Widget", "\346\211\223\345\274\200\346\272\220\347\250\213\345\272\217\346\226\207\344\273\266", nullptr));
        openSemanticBtn->setText(QCoreApplication::translate("Widget", "\346\211\223\345\274\200\350\257\255\344\271\211\347\274\226\347\240\201\346\226\207\344\273\266", nullptr));
        tabWidget_4->setTabText(tabWidget_4->indexOf(tab_15), QCoreApplication::translate("Widget", "\346\272\220\347\250\213\345\272\217", nullptr));
        tabWidget_4->setTabText(tabWidget_4->indexOf(tab_17), QCoreApplication::translate("Widget", "\350\207\252\345\272\225\345\220\221\344\270\212\351\200\220\345\217\245\345\210\206\346\236\220", nullptr));
        tabWidget_4->setTabText(tabWidget_4->indexOf(tab_18), QCoreApplication::translate("Widget", "\350\257\255\346\263\225\346\240\221(\346\226\207\346\234\254\345\275\242\345\274\217)", nullptr));
        tabWidget_4->setTabText(tabWidget_4->indexOf(tab_19), QCoreApplication::translate("Widget", "\350\257\255\346\263\225\346\240\221\357\274\210\346\240\221\345\275\242\357\274\211", nullptr));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tab_14), QCoreApplication::translate("Widget", "\346\272\220\347\250\213\345\272\217\350\257\255\346\263\225\345\210\206\346\236\220", nullptr));
        label_6->setText(QCoreApplication::translate("Widget", "lex\346\226\207\344\273\266\345\206\205\345\256\271", nullptr));
        openLexFileBtn->setText(QCoreApplication::translate("Widget", "\346\211\223\345\274\200lex\346\226\207\344\273\266", nullptr));
        AnalyzeLex->setText(QCoreApplication::translate("Widget", "\345\274\200\345\247\213\350\257\255\346\263\225\345\210\206\346\236\220", nullptr));
        label_8->setText(QCoreApplication::translate("Widget", "lex\345\257\271\345\272\224\347\232\204\350\257\255\344\271\211\347\274\226\347\240\201", nullptr));
        openLexSemanticBtn->setText(QCoreApplication::translate("Widget", "\346\211\223\345\274\200\350\257\255\344\271\211\347\274\226\347\240\201\346\226\207\344\273\266", nullptr));
        tabWidget_5->setTabText(tabWidget_5->indexOf(tab_21), QCoreApplication::translate("Widget", "\346\211\223\345\274\200lex", nullptr));
        tabWidget_5->setTabText(tabWidget_5->indexOf(tab_10), QCoreApplication::translate("Widget", "\350\207\252\345\272\225\345\220\221\344\270\212\351\200\220\345\217\245\345\210\206\346\236\220", nullptr));
        tabWidget_5->setTabText(tabWidget_5->indexOf(tab_22), QCoreApplication::translate("Widget", "\350\257\255\346\263\225\346\240\221(\346\226\207\346\234\254\345\275\242\345\274\217)", nullptr));
        tabWidget_5->setTabText(tabWidget_5->indexOf(tab_23), QCoreApplication::translate("Widget", "\350\257\255\346\263\225\346\240\221\357\274\210\346\240\221\345\275\242\357\274\211", nullptr));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tab_20), QCoreApplication::translate("Widget", "lex\346\226\207\344\273\266\350\257\255\346\263\225\345\210\206\346\236\220", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_7), QCoreApplication::translate("Widget", "\344\273\273\345\212\2412 \350\257\255\346\263\225\345\210\206\346\236\220", nullptr));
        label_9->setText(QCoreApplication::translate("Widget", "\345\215\216\345\215\227\345\270\210\350\214\203\345\244\247\345\255\246\350\256\241\347\256\227\346\234\272\345\255\246\351\231\242 \347\274\226\350\257\221\345\216\237\347\220\206\350\257\276\347\250\213\351\241\271\347\233\256 2024-2025\345\255\246\345\271\264\347\254\254\344\270\200\345\255\246\346\234\237", nullptr));
        label_10->setText(QCoreApplication::translate("Widget", "\345\210\266\344\275\234\357\274\232\351\253\230\345\215\232  \345\255\246\345\217\267\357\274\23220222131028  \346\214\207\345\257\274\350\200\201\345\270\210\357\274\232\351\273\204\347\205\234\345\273\211", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
