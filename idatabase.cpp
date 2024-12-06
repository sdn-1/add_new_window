#include "idatabase.h"
#include <QUuid>

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

bool IDatabase::initPatientModel()
{
    patientTableModel = new QSqlTableModel(this, database);
    patientTableModel->setTable("patient");
    patientTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit); //数据保存方式： OnManualSubmit , OnRowChange
    patientTableModel->setSort(patientTableModel->fieldIndex("name"), Qt::AscendingOrder); //排序
    if (!(patientTableModel->select()))  //查询数据
        return false;

    thePatientSelection = new QItemSelectionModel(patientTableModel);
    return true;
}

int IDatabase::addNewPatient()
{
    patientTableModel->insertRow(patientTableModel->rowCount(),
                                 QModelIndex());  //在末尾添加一个记录
    QModelIndex curIndex = patientTableModel->index(patientTableModel->rowCount() - 1,
                           1);  //创建最后一行的ModelIndex

    int curRecNo = curIndex.row();
    QSqlRecord curRec = patientTableModel->record(curRecNo);  //获取当前记录
    curRec.setValue("CREATEDTIMESTAMP", QDateTime::currentDateTime().toString("yyy-MM-dd"));
    curRec.setValue("ID", QUuid::createUuid().toString(QUuid::WithoutBraces));

    patientTableModel->setRecord(curRecNo, curRec);

    return curIndex.row();
}

bool IDatabase::searchPatient(QString filter)
{
    patientTableModel->setFilter(filter);
    return patientTableModel->select();
}

bool IDatabase::deleteCurrentPatient()
{
    QModelIndex curIndex = thePatientSelection->currentIndex();
    patientTableModel->removeRow(curIndex.row());
    patientTableModel->submitAll();
    patientTableModel->select();
}

bool IDatabase::subminPatientEdt()
{
    return patientTableModel->submitAll();
}

void IDatabase::revertPatientEdit()
{
    patientTableModel->revertAll();
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
