#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "mainwindowend.h"
#include <QObject>
#include <QPushButton>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
     bool is_win();
signals:
     void gameover();

private:
    Ui::MainWindow *ui;
    vector <int> what_is_position(int cur_cell);
    QPushButton *cell;
    MainWindowEnd *w_end;
    void changed_complete();
    void other_game();

    void game_over_with_win();

    bool is_good_position(vector <int> position_1,vector <int> position_2);
    void mix_cell();
    void redrawing();
    vector <QPushButton *> cells;
    size_t size=7;
    vector <int> name_cell;
    QString *name_cell_str;
    void set_name_cell(vector <int> name_cell,QString * name_cell_str_); //присваиваем клеточкам их номера

private slots:
    void changed_cell();
};
#endif // MAINWINDOW_H
