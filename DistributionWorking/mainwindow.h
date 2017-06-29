#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <ActiveQt/QAxObject>
#include "member.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void clearTable();
    int isDigitStr(QString src);
    bool isSubStr(QString str, QString son);
    QString wipeStr(QString str, QString son);
    int getStrMemberNum(QString str);

    Ui::MainWindow *ui;

    QString filePath = NULL;
    QAxObject *excel = NULL;
    QAxObject *workbook = NULL;
    QAxObject *workbooks = NULL;

    Member *member = NULL;
    int memberNumber = -1;

    QString classAnswer[5][7];
    int classAnswerFlag[5][7];
    QAction *openFileAction;
private slots:
    void OpenFile();
    void MemberChanged(int index);
    void on_pushButton_clicked();
    void on_textEdit_1_textChanged();
    void on_textEdit_2_textChanged();
};

#endif // MAINWINDOW_H
