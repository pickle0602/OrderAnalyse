#include <QFileDialog>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QtCharts>
#include <fstream>

#include "ui_chart_widget.h"
#include "main_widget.h"
#include "chart_widget.h"
#include "csv_dealer.h"
#include "distinct_counter.h"
#include "matching_counter.h"
#include "sheet.h"


ChartWidget::ChartWidget(std::shared_ptr<order_analyse::Sheet> const& main_sheet,QWidget *parent) : QWidget(parent), ui(new Ui::ChartWidget),sheet(main_sheet) {
    ui->setupUi(this);

}

ChartWidget::~ChartWidget() {

    delete ui;
}


bool ChartWidget::file_check() {
    if (sheet == NULL || sheet->rows().size() == 0 ||
        sheet->rows()[0][0] == "Error:Bad_Sheet!") {
        QMessageBox::warning(this, "错误", "请先打开文件");
        return false;
    }
    return true;
}



void ChartWidget::show_chart(std::string title, std::string bar_name, const std::vector<int>& values) {
    // 1. 清理旧图表
    QLayoutItem* child;
    while ((child = ui->bar_chart_layout->takeAt(0))) {
        if (child->widget()) {
            child->widget()->setParent(nullptr); // 解除关联
            child->widget()->hide();
        }
        delete child;
    }

    std::vector<int> displayValues= values;

    // 3. 创建图表
    QChart* chart = new QChart();
    QBarSeries* series = new QBarSeries();
    series->setBarWidth(1);
    series->setUseOpenGL(true); // 启用OpenGL加速
    // ui->bar_chart->setRenderHint(QPainter::Antialiasing, false);
    QBarSet* set = new QBarSet(QString::fromStdString(bar_name));
    set->setColor(QColor::fromHsv(255, 255, 255));
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

void ChartWidget::on_EQ_clicked()
{
    if (!file_check()) return;
    std::map<std::string,int> Accumulator;
    std::vector<int> values;
    size_t o_index=sheet->index("Order ID(M)");
    size_t q_index=sheet->index("Item Count(M)");
    if(o_index==-1||q_index==-1){
        QMessageBox::warning(this,"错误","表格数据格式错误");
        return;
    }
    for(const auto& row:sheet->rows()){
        try{
            Accumulator[row[o_index]]+=std::stoi(row[q_index]);
        }
        catch (std::invalid_argument&){
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

void ChartWidget::on_ENhist_clicked() {
    if (!file_check()) return;
    order_analyse::MatchingCounter counter;
    order_analyse::Sheet temp=counter.Calculate(*sheet,std::vector<std::string>{"Order ID(M)","Item Code(M)"});
    std::vector<int> values;
    for(const auto& row:temp.rows()){
        try{
            values.emplace_back(stoi(row[1]));
        }
        catch (std::invalid_argument&){
            QMessageBox::warning(this,"错误","表格数据格式错误");
            return;
        }
    }
    std::sort(values.begin(), values.end(), std::greater<int>());
    std::vector<int> result;
    int i=values[0];
    int j=0;
    for(auto const& val:values){
        while(i!=val){
            result.emplace_back(j);
            j=0;
            i--;
        }
        j++;
    }
    result.emplace_back(j);
    std::reverse(result.begin(), result.end());
    show_chart("EN-hist","E-Frequency",result);
}

void ChartWidget::on_EN_clicked() {
    if (!file_check()) return;
    order_analyse::MatchingCounter counter;
    order_analyse::Sheet temp=counter.Calculate(*sheet,std::vector<std::string>{"Order ID(M)","Item Code(M)"});
    std::vector<int> values;
    for(const auto& row:temp.rows()){
        try{
            values.emplace_back(stoi(row[1]));
        }
        catch (std::invalid_argument&){
            QMessageBox::warning(this,"错误","表格数据格式错误");
            return;
        }
    }
    std::sort(values.begin(), values.end(), std::greater<int>());
    show_chart("EN","N",values);
}


void ChartWidget::on_EQhist_clicked()
{
    if (!file_check()) return;
    std::map<std::string,int> Accumulator;
    std::vector<int> values;
    size_t o_index=sheet->index("Order ID(M)");
    size_t q_index=sheet->index("Item Count(M)");
    if(o_index==-1||q_index==-1){
        QMessageBox::warning(this,"错误","表格数据格式错误");
        return;
    }
    for(const auto& row:sheet->rows()){
        try{
            Accumulator[row[o_index]]+=std::stoi(row[q_index]);
        }
        catch (std::invalid_argument&){
            QMessageBox::warning(this,"错误","表格数据格式错误");
            return;
        }
    }
    for(const auto& entry: Accumulator){
        values.emplace_back(entry.second);
    }
    std::sort(values.begin(), values.end(), std::greater<int>());
    std::vector<int> result;
    int i=values[0];
    int j=0;
    for(auto const& val:values){
        while(i!=val){
            result.emplace_back(j);
            j=0;
            i--;
        }
        j++;
    }
    result.emplace_back(j);
    std::reverse(result.begin(), result.end());
    show_chart("EQ-hist","E-Frequency",result);
}


void ChartWidget::on_IK_clicked()
{
    if (!file_check()) return;
    order_analyse::MatchingCounter counter;
    order_analyse::Sheet temp=counter.Calculate(*sheet,std::vector<std::string>{"Item Code(M)","Order ID(M)"});
    std::vector<int> values;
    for(const auto& row:temp.rows()){
        try{
            values.emplace_back(stoi(row[1]));
        }
        catch (std::invalid_argument&){
            QMessageBox::warning(this,"错误","表格数据格式错误");
            return;
        }
    }
    std::sort(values.begin(), values.end(), std::greater<int>());
    show_chart("IK","K",values);
}


void ChartWidget::on_IQ_clicked()
{
    if (!file_check()) return;
    std::map<std::string,int> Accumulator;
    std::vector<int> values;
    size_t i_index=sheet->index("Item Code(M)");
    size_t q_index=sheet->index("Item Count(M)");
    if(i_index==-1||q_index==-1){
        QMessageBox::warning(this,"错误","表格数据格式错误");
        return;
    }
    for(const auto& row:sheet->rows()){
        try{
            Accumulator[row[i_index]]+=std::stoi(row[q_index]);
        }
        catch (std::invalid_argument&){
            QMessageBox::warning(this,"错误","表格数据格式错误");
            return;
        }
    }
    for(const auto& entry: Accumulator){
        values.emplace_back(entry.second);
    }
    std::sort(values.begin(), values.end(), std::greater<int>());
    show_chart("IQ","Q",values);
}

