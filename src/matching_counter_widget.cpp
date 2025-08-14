#include <QFileDialog>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QtCharts>
#include <iostream>

#include "ui_matching_counter_widget.h"
#include "matching_counter_widget.h"
#include "filter_widget.h"
#include "matching_counter.h"
#include "sheet.h"


MatchingCounterWidget::MatchingCounterWidget(std::shared_ptr<order_analyse::Sheet> const& main_sheet,
                                             QWidget *parent) : QWidget(parent), ui(new Ui::MatchingCounterWidget),sheet(main_sheet) {
    ui->setupUi(this);
    show_sheet();
}

MatchingCounterWidget::~MatchingCounterWidget() {
    delete ui;
}


bool MatchingCounterWidget::file_check() {
    if (sheet == NULL || sheet->rows().size() == 0 ||
        sheet->rows()[0][0] == "Error:Bad_Sheet!") {
        QMessageBox::warning(this, "错误", "请先打开文件");
        return false;
    }
    return true;
}

void MatchingCounterWidget::show_sheet() {
    ui->table->setUpdatesEnabled(false);
    ui->table->clear();
    ui->table->setRowCount(sheet->rows().size());
    ui->table->setColumnCount(sheet->headers().size());

    QStringList headers;
    for (const auto &str : sheet->headers()) {
        headers.append(QString::fromStdString(str));
    }
    ui->table->setHorizontalHeaderLabels(headers);

    int i = 0;
    for (const auto &row : sheet->rows()) {
        int j = 0;
        for (const auto &cell : row) {
            QTableWidgetItem *item =
                new QTableWidgetItem(QString::fromStdString(cell));
            ui->table->setItem(i, j, item);
            j++;
        }
        i++;
    }
    ui->table->setUpdatesEnabled(true);
    ui->table->resizeColumnsToContents();
}

void MatchingCounterWidget::on_filte_clicked()
{
    if (sheet == NULL || sheet->rows().size() == 0 ||
        sheet->rows()[0][0] == "Error:Bad_Sheet!") {
        QMessageBox::warning(this, "错误", "请先生成match count表");
        return;
    }
    std::string left = ui->left->text().toStdString();
    std::string right = ui->right->text().toStdString();
    std::string header = sheet->headers()[0];
    order_analyse::MatchingCounter counter;
    std::shared_ptr<order_analyse::Sheet>temp (new order_analyse::Sheet(
        counter.Filter_LT(*sheet, header, left, right)));
    if (temp->headers()[0] == "Error:Bad_Sheet!") {
        QMessageBox::warning(this, "错误", "请检查表头输入");
        return;
    }
    FilterWidget *w = new FilterWidget(temp);
    w->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动删除
    w->setWindowTitle("筛选结果");
    w->show();
}

