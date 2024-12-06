#include "idatabase.h"

void IDatabase::ininDatabase()
{
    database = QSqlDatabase::addDatabase("QSQLITE");  //添加 SQL LITE数据库驱动
    QString aFile = "D:/qtproject/Lab3a.db";
    database.setDatabaseName(aFile);  //设置数据库名称

    if (!database.open()) {
        qDebug() << "failed to open database";
    } else
        qDebug() << "open database is ok" << database.connectionName();
}

QString IDatabase::userLogin(QString userName, QString password)
{
    // return "loginOK";
    QSqlQuery query;   //查询出当前记录的所有字段
    query.prepare("select username,password from user where username = :USER");
    query.bindValue(":USER", userName);
    query.exec();
    qDebug() << query.lastQuery() << query.first();

    if (query.first() && query.value("username").isValid()) {
        QString passwd = query.value("password").toString();
        if (passwd == password) {
            qDebug() << "loginOK";
            return "loginOK";
        } else {
            qDebug() << "wrongPassword";
            return "wrongPassword";
        }
    } else {
        qDebug() << "no such user";
        return "wrongUsrname";
    }
}

IDatabase::IDatabase(QObject *parent)
    : QObject{parent}
{
    ininDatabase();
}
