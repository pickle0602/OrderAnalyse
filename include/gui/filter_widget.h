#ifndef FILTER_WIDGET_H
#define FILTER_WIDGET_H

#include <QWidget>
#include <QProcess>
#include <memory>

#include"sheet.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class FilterWidget;
}
QT_END_NAMESPACE

class FilterWidget : public QWidget
{
    Q_OBJECT
public:
    FilterWidget(std::shared_ptr<order_analyse::Sheet> const& main_sheet,QWidget *parent = nullptr);
    ~FilterWidget();

private slots:


private:
    Ui::FilterWidget *ui;
    std::shared_ptr<order_analyse::Sheet> sheet;

    bool file_check();
    void show_sheet();

};
#endif // FILTER_WIDGET_H
