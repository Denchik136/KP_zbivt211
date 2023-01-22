#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->depComboBox->addItem("Стоматология");
    ui->depComboBox->addItem("Отоларинголог");

    qDebug() << QFile::remove(QDir::currentPath() + "/testDB.db");
    qDebug()<< QFile::copy(":/database/media/testDB.db", QDir::currentPath() + "/testDB.db");

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./testDB.db");
    if(db.open())
    {
        qDebug("open");
    }else{
        qDebug("noopen");
    }

    query = new QSqlQuery(db);


    changeIndex(ui->depComboBox->currentIndex());
    showCurrentStep(indexPage);
  }


MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::getRandomNumber()
{
  // Установить генератор случайных чисел
  srand(time(NULL));
int min=1;
int max=100;
  // Получить случайное число - формула
  int num = min + rand() % (max - min + 1);

  return num;
}


void MainWindow::on_depComboBox_activated(int index)
{
    changeIndex(index);
}

// Функция смены индекса в комбобоксе первой страницы
void MainWindow::changeIndex(int index)
{
    int depatmentIndex = index;
    if(depatmentIndex != 0)
    {
        ui->labelWelcom->close();
        ui->nextButton->show();
        ui->docName->clear();
    }
    else
    {
        ui->labelWelcom->show();
        ui->nextButton->hide();
    }
    ui->labelImg->setStyleSheet("image: url(:/img/media/" + QString::number(depatmentIndex) + ".png);");
    makeDocBox(depatmentIndex);
    makeDataBox();
}

// Функция создания списка врачей
void MainWindow::makeDocBox(int index)
{
    query -> exec("SELECT * FROM doctors WHERE dep='" + QString::number(index) +"';");
    docList .clear();
    while(query->next()){
        docList << query->value(1).toString();
    }
    for (int i = 0; i < docList.length(); i++)
    {
        ui->docName->addItem(docList[i]);
    }

}

// Функция создания выбора времени визита
void MainWindow::makeDataBox()
{
    query -> exec("SELECT date FROM doctors");
    dateList .clear();
    while(query->next()){
        dateList << query->value(0).toString();
    }
    for (int i = 0; i < dateList.length(); i++)
    {
        ui->dateEdit->addItem(dateList[i]);
    }

}


void MainWindow::firstPageListCreate()
{
    firstPageList.clear();
    int num = getRandomNumber();
    firstPageList.append("Ваш талон № " + QString::number(num) + "\n");
    firstPageList.append("Вы записаны в отделение: \n");
    firstPageList.append(ui->depComboBox->currentText() + '\n');
}

void MainWindow::secontPageListCreate()
{
    secondPageList.clear();
    secondPageList.append("Ваш врач: \n");
    secondPageList.append(ui->docName->currentText() + '\n');
    secondPageList.append("Ваш визит назначен на: \n");
    secondPageList.append(ui->dateEdit->currentText() + '\n');
    secondPageList.append("Пациент: \n");
    secondPageList.append(ui->patientName->text());
    secondPageList.append(ui->birthdayEdit->text());
    secondPageList.append(ui->phoneEdit->text());
    resultList.clear();
    resultList.append(firstPageList);
    resultList.append(secondPageList);
}

void MainWindow::showCurrentStep(int indexPage)
{
    showSecondStep(indexPage);
    showFirstStep(indexPage);
    showLastStep(indexPage);
}


void MainWindow::showFirstStep(int indexPage)
{
    if (indexPage == 1)
    {
        ui->labelImg->show();
        ui->depComboBox->show();
        ui->backButton->setText("Выход");
    }else{
        ui->labelWelcom->hide();
        ui->labelImg->hide();
        ui->depComboBox->hide();
        ui->backButton->setText("Назад");
    }
}

void MainWindow::showSecondStep(int indexPage)
{
    if(indexPage == 2){
        ui->dateEdit->show();
        ui->dateLabel->show();
        ui->docLabel->show();
        ui->docName->show();
        ui->patientName->show();
        ui->patientLabel->show();
        ui->birthdaLabel->show();
        ui->birthdayEdit->show();
        ui->phoneLabel->show();
        ui->phoneEdit->show();
    }else{
        ui->dateEdit->hide();
        ui->dateLabel->hide();
        ui->docLabel->hide();
        ui->docName->hide();
        ui->patientName->hide();
        ui->patientLabel->hide();
        ui->birthdaLabel->hide();
        ui->birthdayEdit->hide();
        ui->phoneLabel->hide();
        ui->phoneEdit->hide();
    }
}

void MainWindow::showLastStep(int indexPage)
{
    if (indexPage == 3)
    {
        ui->result->clear();
        for (int i = 0; i < resultList.length(); i++)
        {
            ui->result->append(resultList[i]);
        }
        ui->result->show();
        ui->nextButton->setText("Готово");
        ui->saveButton->show();
    }
    else
    {
        ui->result->hide();
        ui->nextButton->setText("Вперед");
        ui->saveButton->hide();
    }
}



void MainWindow::on_backButton_clicked()
{

    if(indexPage == 1)
    {
        close();
    } else
    {
        indexPage--;
    }

    showCurrentStep(indexPage);
}


void MainWindow::on_nextButton_clicked()
{
    indexPage++;
    if (indexPage == 2) {
        firstPageListCreate();
    } else if (indexPage == 3){
        secontPageListCreate();

    }else{
        close();
    }

    showCurrentStep(indexPage);
}


void MainWindow::on_saveButton_clicked()
{

    QString filename = QFileDialog::getSaveFileName(this, "" , "Талон.txt");
    QTextDocumentWriter writer(filename);
    writer.write(ui->result->document());
    ui->result->setText("Талон сохранен по адресу: \n" + filename);
    ui->saveButton->hide();
}

