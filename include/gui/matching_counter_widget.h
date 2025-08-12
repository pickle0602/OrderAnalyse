#ifndef MATCHING_COUNTER_WIDGET_H
#define MATCHING_COUNTER_WIDGET_H

#include <QWidget>
#include <QProcess>
#include <memory>

#include"sheet.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MatchingCounterWidget;
}
QT_END_NAMESPACE

class MatchingCounterWidget : public QWidget
{
    Q_OBJECT
public:
    MatchingCounterWidget(std::shared_ptr<order_analyse::Sheet> const& main_sheet,QWidget *parent = nullptr);
    ~MatchingCounterWidget();

private slots:


    void on_filte_clicked();

private:
    Ui::MatchingCounterWidget *ui;
    std::shared_ptr<order_analyse::Sheet> sheet;

    bool file_check();
    void show_sheet();

};
#endif // MATCHING_COUNTER_WIDGET_H
