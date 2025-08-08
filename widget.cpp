#include<fstream>
#include <QMessageBox>
#include <QFileDialog>

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
    delete ui;
}

void Widget::show_sheet(){
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
    for (const auto& row : sheet->rows()) {
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
        show_sheet();

}

void Widget::on_count_clicked()
{

  if(sheet==NULL||sheet->rows().size()==0||sheet->rows()[0][0]=="Error:Bad_Sheet!")
    {QMessageBox::warning(this,"错误","请先打开文件");return;}
    QString header = ui->headers->text();
    order_analyse::DistinctCounter counter;
    std::string result=counter.Calculate(*sheet,std::vector<std::string>{header.toStdString()}).rows()[0][0];
    QMessageBox::information(this,"结果","共"+QString::fromStdString(result)+"个"+header);
}


void Widget::on_matching_count_clicked()
{
    if(sheet==NULL||sheet->rows().size()==0||sheet->rows()[0][0]=="Error:Bad_Sheet!")
    {QMessageBox::warning(this,"错误","请先打开文件");
    return;}
    QString headers = ui->headers->text();
    order_analyse::MatchingCounter counter;
    order_analyse::CsvDealer spliter;
    order_analyse::Sheet *temp=new order_analyse::Sheet(counter.Calculate(*sheet,spliter.SplitRow(headers.toStdString())));
    if(temp->rows().size()==0||temp->rows()[0][0]=="Error:Bad_Sheet!")
    {QMessageBox::warning(this,"错误","请检查表头输入");
        delete temp;
        return;}
    delete sheet;
    sheet=temp;
    Widget::show_sheet();
}


void Widget::on_flite_clicked()
{
   if(sheet==NULL||sheet->rows().size()==0||sheet->rows()[0][0]=="Error:Bad_Sheet!")
    {QMessageBox::warning(this,"错误","请先生成match count表");
   return;}
    std::string left = ui->left->text().toStdString();
    std::string right = ui->right->text().toStdString();
    std::string header = sheet->headers()[0];
    order_analyse::MatchingCounter counter;
    order_analyse::Sheet *temp=new order_analyse::Sheet (counter.Fliter_LT(*sheet,header,left,right));
    if(temp->rows().size()==0||temp->rows()[0][0]=="Error:Bad_Sheet!")
    {QMessageBox::warning(this,"错误","请检查表头输入");
        delete temp;
        return;}
    delete sheet;
    sheet=temp;
    Widget::show_sheet();
}

