#include "configdialog.h"

ConfigDialog::ConfigDialog(QWidget* parent) : QDialog(parent)
{
    //三个标签和对应的输入框
    QLabel* userNameLabel = new QLabel(QString::fromLocal8Bit("请输入用户名:"));
    QLabel* passwordLabel = new QLabel(QString::fromLocal8Bit("请输入密码:"));
    QLabel* hostNameLabel = new QLabel(QString::fromLocal8Bit("请输入主机地址:"));
    userNameEdit = new QLineEdit;
    passwordEdit = new QLineEdit;
    hostNameEdit = new QLineEdit;

    //网格布局
    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(userNameLabel, 0, 0, 1, 1);
    gridLayout->addWidget(passwordLabel, 1, 0, 1, 1);
    gridLayout->addWidget(hostNameLabel, 2, 0, 1, 1);
    gridLayout->addWidget(userNameEdit, 0, 1, 1, 1);
    gridLayout->addWidget(passwordEdit, 1, 1, 1, 1);
    gridLayout->addWidget(hostNameEdit, 2, 1, 1, 1);

    //确定按键
    QPushButton* okButton = new QPushButton(QString::fromLocal8Bit("确定"));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(60);
    btnLayout->addWidget(okButton);
    QVBoxLayout *dlgLayout = new QVBoxLayout;
    dlgLayout->setMargin(40);
    dlgLayout->addLayout(gridLayout);
    dlgLayout->addStretch(40);
    dlgLayout->addLayout(btnLayout);
    setLayout(dlgLayout);

    //按钮和接受槽函数关联
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

    //设置大小
    resize(300,200);
}

ConfigDialog::~ConfigDialog()
{

}

QString ConfigDialog::getUserName()
{
    return this->userNameEdit->text();
}

QString ConfigDialog::getPassword()
{
    return this->passwordEdit->text();
}

QString ConfigDialog::getHostName()
{
    return this->hostNameEdit->text();
}

void ConfigDialog::accept()
{
    //输入内容都不为空时，才返回accept
    if (userNameEdit->text() != nullptr
        && passwordEdit->text() != nullptr
        && hostNameEdit->text() != nullptr)
    {
        QDialog::accept();
    }
}
