#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_openSampleButton_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_openLexButton_clicked();

    void on_saveSampleButton_clicked();

    void on_openFile_clicked();

    void on_openLexFileBtn_clicked();

    void on_openLexSemanticBtn_clicked();

    void on_AnalyzeLex_clicked();

    void on_saveFile_clicked();

    void on_codeAnalyzeBtn_clicked();

    void on_analyze_clicked();

    void on_pushButton_3_clicked();

//    void on_openCodeFileBtn_2_clicked();

    void on_openCodeFileBtn_clicked();

    void on_openSemanticBtn_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
