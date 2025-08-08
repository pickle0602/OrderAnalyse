#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QProcess>
#include"sheet.h"
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

    void on_flite_clicked();

private:
    Ui::Widget *ui;
    void show_sheet();
    order_analyse::Sheet *sheet=NULL;
};
#endif // WIDGET_H
