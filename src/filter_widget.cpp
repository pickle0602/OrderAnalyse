#include <QFileDialog>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QtCharts>

#include "ui_filter_widget.h"
#include "filter_widget.h"
#include "matching_counter.h"
#include "sheet.h"


FilterWidget::FilterWidget(std::shared_ptr<order_analyse::Sheet> const& main_sheet,
                                             QWidget *parent) : QWidget(parent), ui(new Ui::FilterWidget),sheet(main_sheet) {
    ui->setupUi(this);
    show_sheet();
}

FilterWidget::~FilterWidget() {
    delete ui;
}


bool FilterWidget::file_check() {
    if (sheet == NULL || sheet->rows().size() == 0 ||
        sheet->rows()[0][0] == "Error:Bad_Sheet!") {
        QMessageBox::warning(this, "错误", "请先打开文件");
        return false;
    }
    return true;
}

void FilterWidget::show_sheet() {
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
