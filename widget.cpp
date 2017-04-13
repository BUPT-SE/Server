#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}



void Widget::on_pushButton_clicked()
{
    ui->r1->setText("1");
    ui->r2->setText("2");
    ui->r3->setText("3");
    ui->r4->setText("4");
    ui->s1->setText("中速");
    ui->s2->setText("中速");
    ui->s3->setText("中速");
    ui->s4->setText("中速");
    ui->Room1->setEnabled("false");
    ui->Room2->setEnabled("false");
    ui->Room3->setEnabled("false");
    ui->Room4->setEnabled("false");


}
