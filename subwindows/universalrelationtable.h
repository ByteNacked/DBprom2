#ifndef UNIVERSALRELATIONTABLE_H
#define UNIVERSALRELATIONTABLE_H

#include <QMainWindow>
#include <core/storage.h>
#include <QTableWidget>

namespace Ui {
class UniversalRelationTable;
}

class UniversalRelationTable : public QMainWindow
{
    Q_OBJECT

public:
    explicit UniversalRelationTable(QWidget *parent = 0, Storage *rStorage = 0);
    ~UniversalRelationTable();

private:
    Ui::UniversalRelationTable *ui;
    Storage *storage;
    QTableWidget *wt;
    QVector<QVector<QString>> *uniTable;
private slots:
    void updateTable(QTableWidgetItem *rItem);
};

#endif // UNIVERSALRELATIONTABLE_H