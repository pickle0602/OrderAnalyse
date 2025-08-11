#include "widget.h"

#include <QFileDialog>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QtCharts>
#include <fstream>
#include <iostream>//test

#include "./ui_widget.h"
#include "csv_dealer.h"
#include "distinct_counter.h"
#include "matching_counter.h"
#include "sheet.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
  ui->table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->table_widget->setSelectionBehavior(QAbstractItemView::SelectRows);
}

Widget::~Widget() {
  if (sheet != NULL) delete sheet;
  if (showing_sheet != NULL) delete showing_sheet;
  delete ui;
}

void Widget::show_sheet() {
  ui->table_widget->setUpdatesEnabled(false);
  ui->table_widget->clear();
  ui->table_widget->setRowCount(showing_sheet->rows().size());
  ui->table_widget->setColumnCount(showing_sheet->headers().size());

  QStringList headers;
  for (const auto &str : showing_sheet->headers()) {
    headers.append(QString::fromStdString(str));
  }
  ui->table_widget->setHorizontalHeaderLabels(headers);

  int i = 0;
  for (const auto &row : showing_sheet->rows()) {
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

bool Widget::file_check() {
  if (sheet == NULL || sheet->rows().size() == 0 ||
      sheet->rows()[0][0] == "Error:Bad_Sheet!") {
    QMessageBox::warning(this, "错误", "请先打开文件");
    return false;
  }
  return true;
}

void Widget::on_open_clicked() {
  QString path = ui->lineEdit->text();

  std::ifstream stream(path.toStdString());
  if (!stream.is_open()) {
    QMessageBox::warning(this, "错误", "无法打开文件: " + path);
    return;
  }
  order_analyse::CsvDealer dealer;
  if (sheet != NULL) delete sheet;
  sheet = new order_analyse::Sheet(dealer.InPut(stream));
  if (showing_sheet != NULL) delete showing_sheet;
  showing_sheet = sheet;
  show_sheet();
}

void Widget::on_count_clicked() {
  if (!file_check()) return;
  QString header = ui->headers->text();
  order_analyse::DistinctCounter counter;
  std::string result =
      counter.Calculate(*sheet, std::vector<std::string>{header.toStdString()})
          .rows()[0][0];
  QMessageBox::information(
      this, "结果", "共" + QString::fromStdString(result) + "个" + header);
}

void Widget::on_matching_count_clicked() {
  if (!file_check()) return;
  QString headers = ui->headers->text();
  order_analyse::MatchingCounter counter;
  order_analyse::CsvDealer spliter;
  order_analyse::Sheet *temp = new order_analyse::Sheet(
      counter.Calculate(*sheet, spliter.SplitRow(headers.toStdString())));
  if (temp->rows().size() == 0 || temp->rows()[0][0] == "Error:Bad_Sheet!") {
    QMessageBox::warning(this, "错误", "请检查表头输入");
    delete temp;
    return;
  }
  delete showing_sheet;
  showing_sheet = temp;
  Widget::show_sheet();
}

void Widget::on_filte_clicked() {
  if (sheet == NULL || showing_sheet->rows().size() == 0 ||
      showing_sheet->rows()[0][0] == "Error:Bad_Sheet!") {
    QMessageBox::warning(this, "错误", "请先生成match count表");
    return;
  }
  std::string left = ui->left->text().toStdString();
  std::string right = ui->right->text().toStdString();
  std::string header = showing_sheet->headers()[0];
  order_analyse::MatchingCounter counter;
  order_analyse::Sheet *temp = new order_analyse::Sheet(
      counter.Filter_LT(*showing_sheet, header, left, right));
  if (temp->rows().size() == 0 || temp->rows()[0][0] == "Error:Bad_Sheet!") {
    QMessageBox::warning(this, "错误", "请检查表头输入");
    delete temp;
    return;
  }
  delete showing_sheet;
  showing_sheet = temp;
  Widget::show_sheet();
}

void Widget::on_ENhist_clicked() {
  if (!file_check()) return;
}

void Widget::on_EN_clicked() {
  if (!file_check()) return;
  order_analyse::MatchingCounter counter;
  order_analyse::Sheet temp=counter.Calculate(*sheet,std::vector<std::string>{"Order ID(M)","Item Code(M)"});
  std::vector<int> values;
  for(const auto& row:temp.rows()){
      try{
          values.emplace_back(stoi(row[1]));
      }
      catch (std::out_of_range&){
          QMessageBox::warning(this,"错误","表格数据格式错误");
          return;
      }
  }
    std::sort(values.begin(), values.end(), std::greater<int>());
  show_chart("EN","N",values);
}

void Widget::show_chart(std::string title, std::string bar_name, const std::vector<int>& values) {
    // 1. 清理旧图表
    QLayoutItem* child;
    while ((child = ui->bar_chart_layout->takeAt(0))) {
        if (child->widget()) {
            child->widget()->setParent(nullptr); // 解除关联
            child->widget()->hide();
        }
        delete child;
    }

    // 2. 数据采样处理（限制最大显示数量）
    const int MAX_BARS = 500; // 根据实际性能调整
    std::vector<int> displayValues;
    if(values.size() > MAX_BARS) {
        // 等间隔采样
        double step = static_cast<double>(values.size()) / MAX_BARS;
        for(size_t i = 0; i < MAX_BARS; ++i) {
            size_t index = static_cast<size_t>(i * step);
            displayValues.push_back(values[index]);
        }
    } else {
        displayValues = values;
    }

    // 3. 创建图表
    QChart* chart = new QChart();
    QBarSeries* series = new QBarSeries();
    series->setBarWidth(1);
    series->setUseOpenGL(true); // 启用OpenGL加速
    // ui->bar_chart->setRenderHint(QPainter::Antialiasing, false);
    QBarSet* set = new QBarSet(QString::fromStdString(bar_name));
    set->setColor(QColor::fromHsv(240, 255, 200));
    set->setBorderColor(set->color());
    for(auto val : displayValues) {
        *set << val;
    }
    series->append(set);

    // 4. 优化X轴显示
    QValueAxis* axisX = new QValueAxis();
    axisX->setRange(0, displayValues.size());
    axisX->setTickCount(std::min(11, static_cast<int>(displayValues.size()/50 + 2))); // 动态调整刻度数
    axisX->setLabelFormat("%d");

    // 5. 自动调整Y轴范围
    auto maxElem = *std::max_element(displayValues.begin(), displayValues.end());
    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(0, maxElem * 1.1); // 留10%余量

    // 6. 配置图表
    chart->addSeries(series);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->setTitle(QString::fromStdString(title));
    chart->legend()->setVisible(false);

    // 7. 创建视图
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->bar_chart_layout->addWidget(chartView);
}

void Widget::on_EQ_clicked()
{
    if (!file_check()) return;
    order_analyse::MatchingCounter counter;
    order_analyse::Sheet temp=counter.Calculate(*sheet,std::vector<std::string>{"Order ID(M)","Item Count(M)"});
    std::map<std::string,int> Accumulator;
    std::vector<int> values;
    for(const auto& row:temp.rows()){
        try{
            Accumulator[row[0]]+=std::stoi(row[1]);
        }
        catch (std::out_of_range&){
            QMessageBox::warning(this,"错误","表格数据格式错误");
            return;
        }
    }
    for(const auto& entry: Accumulator){
        values.emplace_back(entry.second);
    }
    std::sort(values.begin(), values.end(), std::greater<int>());
    show_chart("EQ","Q",values);

}


void Widget::on_EQhist_clicked()
{

}


void Widget::on_IK_clicked()
{

}


void Widget::on_IQ_clicked()
{

}

