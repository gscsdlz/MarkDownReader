#ifndef RECENTFILE_H
#define RECENTFILE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStandardItem>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class RecentFile;
}

class RecentFile : public QWidget
{
    Q_OBJECT

public:
    explicit RecentFile(QWidget *parent = 0);
    void deleteFile(QString);
    void insertFile(QString);
    void load();
    QString getLastDir();
    void setLastDir(QString);
    ~RecentFile();
signals:
    void fileOpen(QString);
private slots:
    void on_listView_doubleClicked(const QModelIndex &index);

    void on_listView_clicked(const QModelIndex &index);

private:
    Ui::RecentFile *ui;
    QSqlDatabase conn;
};

#endif // RECENTFILE_H
