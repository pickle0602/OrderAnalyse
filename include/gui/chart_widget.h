#ifndef CHART_WIDGET_H
#define CHART_WIDGET_H

#include <QWidget>
#include <QProcess>
#include <QtCharts>
#include"sheet.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class ChartWidget;
}
QT_END_NAMESPACE

class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    ChartWidget(std::shared_ptr<order_analyse::Sheet> const& main_sheet,QWidget *parent = nullptr);
    ~ChartWidget();

private slots:


    void on_ENhist_clicked();

    void on_EN_clicked();

    void on_EQ_clicked();

    void on_EQhist_clicked();

    void on_IK_clicked();

    void on_IQ_clicked();


private:
    Ui::ChartWidget *ui;
    std::shared_ptr<order_analyse::Sheet> sheet;

    bool file_check();
    void show_chart(std::string title,std::string bar_name,const std::vector<int>& values);

};

#endif // CHART_WIDGET_H
