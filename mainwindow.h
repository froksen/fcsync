#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "process.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_dir1BrowseButton_clicked();
    void on_dir2BrowseButton_clicked();
    void on_c_argumentCheckbox_clicked(bool checked);
    void on_v_argumentCheckbox_clicked(bool checked);
    void on_syncButton_released();
    void setConsoleText(QString text);
    void onSyncEnded(int exitcode);
    void on_dryrun_argumentCheckbox_clicked(bool checked);
    void on_forcestopButton_clicked();


private:
    Ui::MainWindow *ui;

    QString dir1;
    QString dir2;

    bool arg_c;
    bool arg_v;
    bool arg_dryrun;

    void setConflictcopys(bool status=false);
    void setVerbose(bool status=false);
    void setDryrun(bool status=false);

    void updateProgressbar(QString text);

    void updateDirs();
    QStringList updateArguments();

    void startSync(QString dir1, QString dir2, QStringList arguments);

    process *mProcess;

};

#endif // MAINWINDOW_H
