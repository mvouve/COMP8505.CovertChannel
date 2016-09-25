#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QErrorMessage>
#include <QtDebug>
#include <climits>
#include "clientdriver.h"
#include "serverdriver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void errorString(QString);
    void listen();
    void send();
    void finishSend();
    void endListen();
    void writeCharacter(char c, int pos);

private:
    Ui::MainWindow *ui;

    void startSend();
};

#endif // MAINWINDOW_H
