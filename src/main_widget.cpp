#include <QFileDialog>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QtCharts>
#include <fstream>
#include <memory>

#include "ui_main_widget.h"
#include "main_widget.h"
#include "chart_widget.h"
#include "matching_counter_widget.h"
#include "csv_dealer.h"
#include "distinct_counter.h"
#include "matching_counter.h"
#include "sheet.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget) {
  ui->setupUi(this);
  ui->table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->table_widget->setSelectionBehavior(QAbstractItemView::SelectRows);
}

MainWidget::~MainWidget() {
  delete ui;
}

void MainWidget::show_sheet() {
  ui->table_widget->setUpdatesEnabled(false);
  ui->table_widget->clear();
  ui->table_widget->setRowCount(sheet->rows().size());
  ui->table_widget->setColumnCount(sheet->headers().size());

  QStringList headers;
  for (const auto &str : sheet->headers()) {
    headers.append(QString::fromStdString(str));
  }
  ui->table_widget->setHorizontalHeaderLabels(headers);

  int i = 0;
  for (const auto &row : sheet->rows()) {
    int j = 0;
    for (const auto &cell : row) {
      QTableWidgetItem *item =
          new QTableWidgetItem(QString::fromStdString(cell));
      ui->table_widget->setItem(i, j, item);
      j++;
    }
    i++;
  }
  ui->table_widget->setUpdatesEnabled(true);
  ui->table_widget->resizeColumnsToContents();
}

bool MainWidget::file_check() {
  if (sheet == NULL || sheet->rows().size() == 0 ||
      sheet->rows()[0][0] == "Error:Bad_Sheet!") {
    QMessageBox::warning(this, "错误", "请先打开文件");
    return false;
  }
  return true;
}

void MainWidget::on_open_clicked() {
  path = ui->lineEdit->text();

  std::ifstream stream(path.toStdString());
  if (!stream.is_open()) {
    QMessageBox::warning(this, "错误", "无法打开文件: " + path);
    return;
  }
  order_analyse::CsvDealer dealer;
  sheet.reset(new order_analyse::Sheet(dealer.InPut(stream)));
  show_sheet();
}

void MainWidget::on_count_clicked() {
  if (!file_check()) return;
  QString header = ui->headers->text();
  order_analyse::DistinctCounter counter;
  std::string result =
      counter.Calculate(*sheet, std::vector<std::string>{header.toStdString()})
          .rows()[0][0];
  QMessageBox::information(
      this, "结果", "共" + QString::fromStdString(result) + "个" + header);
}

void MainWidget::on_matching_count_clicked() {
  if (!file_check()) return;
  QString headers = ui->headers->text();
  order_analyse::MatchingCounter counter;
  order_analyse::CsvDealer spliter;
  std::shared_ptr<order_analyse::Sheet> temp ( new order_analyse::Sheet(
      counter.Calculate(*sheet, spliter.SplitRow(headers.toStdString()))));
  if (temp->rows().size() == 0 || temp->rows()[0][0] == "Error:Bad_Sheet!") {
    QMessageBox::warning(this, "错误", "请检查表头输入");
    return;
  }
  MatchingCounterWidget *w = new MatchingCounterWidget(temp);
  w->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动删除
  w->setWindowTitle(path+"-统计结果");
  w->show();
}





void MainWidget::on_EQI_clicked()
{
    if (!file_check()) return;
    ChartWidget *w = new ChartWidget(sheet);
    w->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动删除
    w->setWindowTitle(path+"-EQI分析");
    w->show();
}

