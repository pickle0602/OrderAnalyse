#include<fstream>
#include <QMessageBox>
#include <QFileDialog>
#include <QtCharts>
#include <QGraphicsScene>

#include "widget.h"
#include "./ui_widget.h"
#include "sheet.h"
#include "csv_dealer.h"
#include "distinct_counter.h"
#include "matching_counter.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_widget->setSelectionBehavior(QAbstractItemView::SelectRows);

}

Widget::~Widget()
{
    if(sheet!=NULL)
        delete sheet;
    if(showing_sheet!=NULL)
        delete showing_sheet;
    delete ui;
}

void Widget::show_sheet(){
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
    for (const auto& row : showing_sheet->rows()) {
        int j = 0;
        for (const auto& cell : row) {
            QTableWidgetItem *item = new QTableWidgetItem(QString::fromStdString(cell));
            ui->table_widget->setItem(i, j, item);
            j++;
        }
        i++;
    }
    ui->table_widget->setUpdatesEnabled(true);
    ui->table_widget->resizeColumnsToContents();
}

bool Widget::file_check(){
    if(sheet==NULL||sheet->rows().size()==0||sheet->rows()[0][0]=="Error:Bad_Sheet!")
    {QMessageBox::warning(this,"错误","请先打开文件");
        return false;}
    return true;
}

void Widget::on_open_clicked()
{
    QString path = ui->lineEdit->text();

    std::ifstream stream(path.toStdString());
    if (!stream.is_open()) {
        QMessageBox::warning(this, "错误", "无法打开文件: " + path);
        return;
    }
        order_analyse::CsvDealer dealer;
        if(sheet!=NULL)
            delete sheet;
        sheet =new order_analyse::Sheet (dealer.InPut(stream));
        if(showing_sheet!=NULL)
            delete showing_sheet;
        showing_sheet=sheet;
        show_sheet();

}

void Widget::on_count_clicked()
{
    if(!file_check())
        return;
    QString header = ui->headers->text();
    order_analyse::DistinctCounter counter;
    std::string result=counter.Calculate(*sheet,std::vector<std::string>{header.toStdString()}).rows()[0][0];
    QMessageBox::information(this,"结果","共"+QString::fromStdString(result)+"个"+header);
}


void Widget::on_matching_count_clicked()
{
    if(!file_check())
        return;
    QString headers = ui->headers->text();
    order_analyse::MatchingCounter counter;
    order_analyse::CsvDealer spliter;
    order_analyse::Sheet *temp=new order_analyse::Sheet(counter.Calculate(*sheet,spliter.SplitRow(headers.toStdString())));
    if(temp->rows().size()==0||temp->rows()[0][0]=="Error:Bad_Sheet!")
    {QMessageBox::warning(this,"错误","请检查表头输入");
        delete temp;
        return;}
    delete showing_sheet;
    showing_sheet=temp;
    Widget::show_sheet();
}


void Widget::on_flite_clicked()
{
   if(sheet==NULL||showing_sheet->rows().size()==0||showing_sheet->rows()[0][0]=="Error:Bad_Sheet!")
    {QMessageBox::warning(this,"错误","请先生成match count表");
   return;}
    std::string left = ui->left->text().toStdString();
    std::string right = ui->right->text().toStdString();
    std::string header = showing_sheet->headers()[0];
    order_analyse::MatchingCounter counter;
    order_analyse::Sheet *temp=new order_analyse::Sheet (counter.Fliter_LT(*showing_sheet,header,left,right));
    if(temp->rows().size()==0||temp->rows()[0][0]=="Error:Bad_Sheet!")
    {QMessageBox::warning(this,"错误","请检查表头输入");
        delete temp;
        return;}
    delete showing_sheet;
    showing_sheet=temp;
    Widget::show_sheet();
}


void Widget::on_ENhist_clicked()
{
    if(!file_check())
        return;

}


void Widget::on_EN_clicked()
{
    if(!file_check())
        return;
}

void Widget::show_chart(){
    // 1. 获取或创建场景
    QGraphicsScene *scene =ui->bar_chart->scene();
    if (!scene) {
        scene = new QGraphicsScene(ui->bar_chart);
        ui->bar_chart->setScene(scene);
    }

    // 2. 创建柱状图数据
    QBarSeries *series = new QBarSeries();
    QBarSet *set = new QBarSet("销售额");
    *set << 1200 << 2400 << 1800 << 3200;
    series->append(set);

    // 3. 创建图表并配置
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("季度销售额");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // 4. 设置坐标轴
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append({"Q1", "Q2", "Q3", "Q4"});
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 4000);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // 5. 将图表添加到场景
    chart->setPos(0, 0);  // 设置图表在场景中的位置
    scene->addItem(chart); // 关键步骤：将图表作为图形项加入
}



