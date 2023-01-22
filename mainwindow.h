#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTextEdit>
#include <QFileDialog>
#include <QTextDocumentWriter>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_depComboBox_activated(int index);

    void on_backButton_clicked();

    void on_nextButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlQuery *query;
    QList<QString> firstPageList;
    QList<QString> secondPageList;
    QList<QString> resultList;
    QList<QString> docList;
    QList<QString> dateList;
    int indexPage = 1;
    int number;

    void changeIndex(int indexDep);
    void showCurrentStep(int indexPage);
    void showFirstStep(int indexPage);
    void showSecondStep(int indexPage);
    void showLastStep(int indePage);
    void makeDocBox (int index);
    void makeDataBox();
    void firstPageListCreate();
    void secontPageListCreate();
    int getRandomNumber();
};
#endif // MAINWINDOW_H
