#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QProcess>
#include <QtCharts>
#include"sheet.h"

// QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    void on_open_clicked();

    void on_count_clicked();

    void on_matching_count_clicked();

    void on_filte_clicked();

    void on_ENhist_clicked();

    void on_EN_clicked();


    void on_EQ_clicked();

    void on_EQhist_clicked();

    void on_IK_clicked();

    void on_IQ_clicked();

private:
    Ui::Widget *ui;
    order_analyse::Sheet *sheet=NULL;
    order_analyse::Sheet *showing_sheet=NULL;

    bool file_check();
    void show_chart(std::string title,std::string bar_name,const std::vector<int>& values);
    void show_sheet();

};
#endif // WIDGET_H
