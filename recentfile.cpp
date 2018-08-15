#include "recentfile.h"
#include "ui_recentfile.h"

RecentFile::RecentFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecentFile)
{
    ui->setupUi(this);

    this->setFixedSize(QSize(400, 400));
    ui->listView->setFixedSize(QSize(400, 400));

    QString exePath = QCoreApplication::applicationDirPath();
    conn = QSqlDatabase::addDatabase("QSQLITE");
    conn.setDatabaseName(exePath + "\\db\\data.db");

    if (conn.open()) {
        load();
    }
}

RecentFile::~RecentFile()
{
    delete ui;
}

void RecentFile::on_listView_doubleClicked(const QModelIndex &index)
{
    int res = QMessageBox::warning(this, "", QStringLiteral("是否删除该项？（不删除文件）"), QMessageBox::Ok, QMessageBox::Cancel);
    if (res == QMessageBox::Ok) {
        this->deleteFile(index.data().toString());
    }
}

void RecentFile::deleteFile(QString path)
{
    conn.exec("DELETE FROM files WHERE path = '" + path + "'");
    load();
}

void RecentFile::insertFile(QString path)
{
    QSqlQuery res = conn.exec("SELECT * FROM files WHERE path ='" + path + "'");
    if (res.size() == 0) {
            conn.exec("INSERT INTO files (path) VALUES ('"+ path +"')"); 
    } else {
        qDebug() << "this";
        conn.exec("UPDATE files SET view_count = view_count + 1 WHERE path = '"+ path + "'");
    }
    load();
}

void RecentFile::load()
{
    QSqlQuery res = conn.exec("SELECT path FROM files WHERE 1 ORDER BY view_count DESC");
    QStandardItemModel *model = new QStandardItemModel;
    while (res.next()) {
        QStandardItem *item = new QStandardItem(res.value(0).toString());
        item->setEditable(false);
        model->appendRow(item);
    }

    ui->listView->setModel(model);
}

void RecentFile::on_listView_clicked(const QModelIndex &index)
{
    emit fileOpen(index.data().toString());
    this->hide();
}

QString RecentFile::getLastDir()
{
    QSqlQuery res = conn.exec("SELECT path FROM last_dir WHERE 1");
    QString path;
    while (res.next()) {
        path = res.value(0).toString();
    }
    return path;
}

void RecentFile::setLastDir(QString path)
{
    conn.exec("UPDATE last_dir SET path = '" + path + "' WHERE 1");
}
