#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include <QWidget>
#include <QProcess>
#include <memory>

#include"sheet.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWidget;
}
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private slots:

    void on_open_clicked();

    void on_count_clicked();

    void on_matching_count_clicked();

    void on_EQI_clicked();

private:
    Ui::MainWidget *ui;
    std::shared_ptr<order_analyse::Sheet> sheet;
    QString path;

    bool file_check();
    void show_sheet();

};
#endif // MAIN_WIDGET_H
