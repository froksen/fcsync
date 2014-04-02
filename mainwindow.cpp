#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QRegExp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    dir1 = QDir::homePath() + "/New Folder 2";
    dir2 = QDir::homePath() + "/New Folder";
    updateDirs();

    setConflictcopys(ui->c_argumentCheckbox->isChecked());
    setVerbose(ui->v_argumentCheckbox->isChecked());
    setDryrun(ui->dryrun_argumentCheckbox->isChecked());

    mProcess = new process(this);
    connect(mProcess,SIGNAL(stdoutText(QString)),this,SLOT(setConsoleText(QString)));
    connect(mProcess,SIGNAL(stderrText(QString)),this,SLOT(setConsoleText(QString)));
    connect(mProcess,SIGNAL(FinishedExitCode(int)),this,SLOT(onSyncEnded(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_dir1BrowseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     ui->dir1lineEdit->text(),
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);

    if(dir != ""){
        dir1 = dir;
        updateDirs();
    }
}

void MainWindow::on_dir2BrowseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     ui->dir2lineEdit->text(),
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);

    if(dir != ""){
        dir2 = dir;
        updateDirs();
    }
}

void MainWindow::setConflictcopys(bool status)
{
    arg_c = status;
    qDebug() << "Conflict-copys set to" << status;
}

void MainWindow::setVerbose(bool status)
{
    arg_v = status;
    qDebug() << "Verbose set to" << status;
}

void MainWindow::setDryrun(bool status)
{
    arg_dryrun = status;
    qDebug() << "dry-run set to" << status;
}


void MainWindow::updateDirs()
{
    ui->dir2lineEdit->setText(dir2);
    ui->dir1lineEdit->setText(dir1);

}

QStringList MainWindow::updateArguments()
{
    QStringList arguments;
    if(ui->c_argumentCheckbox->isChecked()){
        arguments << "-c";
    }

    if(ui->v_argumentCheckbox->isChecked()){
        arguments << "-v";
    }
    if(ui->dryrun_argumentCheckbox->isChecked()){
        arguments << "--dry-run";
    }

    return arguments;
}

void MainWindow::startSync(QString dir1, QString dir2, QStringList arguments)
{
    mProcess->setCommand("csync");

    arguments << dir1;
    arguments << dir2;


    mProcess->setArguments(arguments);

    mProcess->startCommand();
}

void MainWindow::on_c_argumentCheckbox_clicked(bool checked)
{
    setConflictcopys(checked);
}

void MainWindow::on_v_argumentCheckbox_clicked(bool checked)
{
    setVerbose(checked);
}

void MainWindow::on_syncButton_released()
{

    QString dir1 = ui->dir1lineEdit->text();
    QString dir2 = ui->dir2lineEdit->text();

    if(dir1 == dir2){
        QMessageBox msgBox;
        msgBox.setText("The directories are the same.");
        msgBox.setInformativeText("The two directories must not be the same! The Sync will not run");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();

        return;
    }

    ui->plainTextEdit->clear();
    ui->syncButton->setDisabled(true);
    ui->forcestopButton->setEnabled(true);

    startSync(dir1,dir2,updateArguments());

}

void MainWindow::setConsoleText(QString text)
{
    updateProgressbar(text);
    qDebug () << "køres";
     ui->plainTextEdit->appendPlainText(text);
     ui->statusBar->showMessage("Sync is running");
}

void MainWindow::onSyncEnded(int exitcode)
{
    ui->statusBar->showMessage("Sync ended with " + QString::number(exitcode));
    ui->syncButton->setEnabled(true);
    ui->forcestopButton->setDisabled(true);

}

void MainWindow::on_dryrun_argumentCheckbox_clicked(bool checked)
{

}

void MainWindow::on_forcestopButton_clicked()
{
    mProcess->stopCommand();
}

void MainWindow::updateProgressbar(QString text)
{
    QRegExp rx("^File\\s#\\s\\d\/\\s\\d");

    //Checks if the line matches pattern else return


    //Finds pattern in text
    rx.indexIn(text);
    int matchedLength = rx.matchedLength();

    //Removes everything else than the matched.
    text = text.remove(matchedLength,1000);

    //Copies the matched text into two new variables
    QString totalProgressTxt = text;
    QString currentProgressTxt = text;

    //TotalProgress
    totalProgressTxt.remove(0,9);
    int totalProgress = totalProgressTxt.toInt();
    qDebug () << "Total" << totalProgress;
   // ui->progressBar->setMaximum(totalProgress);


    currentProgressTxt.remove(0,6).remove(2,1000);
    int currentProgress = currentProgressTxt.toInt();
    qDebug() << "CurrentProgress" << currentProgress;
    //ui->progressBar->setValue(currentProgress);

        //Updates maximum

        //Updates current progress

}
