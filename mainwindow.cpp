#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString title(tr("Main window [DB prom] v"));
    title += version;
    this->setWindowTitle(title);

    storage = new Storage();
    ui->textEdit->setReadOnly(1);

    scene = new MyScene(storage);
    ui->graphicsView->setScene(scene);

    ui->masterKeyLabel->setText("Введите связи между атрибутами для отображения ключа универсального отношения.");

    //QSettings *settings = new QSettings("settings.conf",QSettings::IniFormat, this);
    this->show();

    NewDialog *wd = new NewDialog(this, this, storage);
    wd->show();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete storage;
}

void MainWindow::showOutput()
{
    QString outText;
    ui->textEdit->setHtml(storage->getOutputText(outText));
    scene->fillScene();

}

void MainWindow::saveFile()
{
    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << version << *(storage->getAttrTable())
        << *(storage->getVMatrix()) << *(storage->getUniTable()) << storage->getdbName();
}

void MainWindow::reset()
{
    scene->clear();
    storage->clear();
    ui->textEdit->clear();
    ui->masterKeyLabel->setText("Введите связи между атрибутами для отображения ключа универсального отношения.");
}

void MainWindow::on_addAttrButton_clicked()
{
    Attribute *wa = new Attribute(this, storage);
    wa->show();
}

void MainWindow::on_addConButton_clicked()
{
    Connection *wc = new Connection(this, storage);
    wc->show();
    storage->updateSuperKey();
    if (!storage->getSuperKeytoString().isEmpty())
        ui->masterKeyLabel->setText(QString(tr("Ключ универсального отношения: ")) += storage->getSuperKeytoString());
}

void MainWindow::on_normButton_clicked()
{
    storage->startNormalization();
    this->showOutput();
}

void MainWindow::on_pushButton_clicked()
{
    close();
}

void MainWindow::on_actionSave_File_triggered()
{
    saveFile();
    qDebug() << filePath << "saved.";

}

void MainWindow::on_actionOpen_File_triggered()
{
    filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.dbp)"));

    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    QString outVersion;
    QVector<QVector<QString> > outAttrTable;
    QVector<QVector<int> > outVMatrix;
    QVector<QVector<QString> > outUniTable;
    QString outdbName;
    in >> outVersion >> outAttrTable >> outVMatrix >> outUniTable >> outdbName;
    if (outVersion != version) {
        qDebug() << "Разные версии";
        return;
    }

    storage->setAttrTable(outAttrTable);
    storage->setVMatrix(outVMatrix);
    storage->setUniTable(outUniTable);
    storage->setdbName(outdbName);
    qDebug() << filePath << "opened.";

    if (storage->getAttrTable()->isEmpty() || storage->getVMatrix()->isEmpty()) return;
    on_normButton_clicked();

    storage->updateSuperKey();
    if (!storage->getSuperKeytoString().isEmpty())
        ui->masterKeyLabel->setText(QString(tr("Ключ универсального отношения: ")) += storage->getSuperKeytoString());
}

void MainWindow::on_actionSave_ass_triggered()
{
    filePath = QFileDialog::getSaveFileName(this, tr("Save file"), "untitled.dbp", tr("DB prom files (*.dbp)"));
    saveFile();
    qDebug() << filePath << "saved ass...";
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("О программе..."), tr("Программа нормализации реляционных баз данных. 2014. Масленников Роман. E-mail: m3angreen@gmail.com"));
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_uniRelButton_clicked()
{
    UniversalRelationTable *wu = new UniversalRelationTable(this, storage);
    wu->show();
}

void MainWindow::on_action_triggered()
{
    DBnameDialog *wdb = new DBnameDialog(this, storage);
    wdb->show();
}

void MainWindow::on_actionNew_data_base_scheme_triggered()
{
    NewDialog *wd = new NewDialog(this, this, storage);
    wd->show();
}
