#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QtGui>
#include <QTextCodec>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class ConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();
    QString getUserName();//得到用户名
    QString getPassword();//得到密码
    QString getHostName();//得到主机地址
public slots:
    virtual void accept();
private:
    QLineEdit* userNameEdit;//用户名
    QLineEdit* passwordEdit;//密码
    QLineEdit* hostNameEdit;//主机地址
};

#endif // CONFIGDIALOG_H
