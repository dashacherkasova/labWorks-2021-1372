#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QStyle>
#include <QTimer>
#include <QObject>
#include <QPushButton>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    w_end= new MainWindowEnd;
    for (int i=0;i<size*size;i++)
    {
        name_cell.push_back(i+1);
    }
    mix_cell();
    name_cell_str = new QString[size*size];
    set_name_cell(name_cell,name_cell_str);
    for (int i=0;i<size*size;i++)
    {
        cell=new QPushButton;
        cell->setObjectName(name_cell_str[i]);
        if (i==size*size-1)
        {
            cell->setText("");
        }
       else {
        cell->setText(name_cell_str[i]);
       }
        cells.push_back(cell);
        vector <int> pos_cells=what_is_position(i);
        cells[i]->move(pos_cells[0],pos_cells[1]);
        //qDebug() << cells[i]->pos();
    }
    //cells[size*size-1]->setText(cells[size*size-2]->text()); //эти две строчки я использовала для удобства проверки работы игры
    //cells[size*size-2]->setText("");
    for (int i=0;i<size*size;i++)
    {
       ui->gridLayout->addWidget(cells[i],i/size,i%size);
    }
    redrawing();
    QObject::connect(this,&MainWindow::gameover,this,[this](){game_over_with_win();});
    QObject::connect(w_end,&MainWindowEnd::show_game,this,[this](){other_game();});
    QObject::connect(ui->pushButton,&QPushButton::clicked, this,[this](){changed_complete();});

}

MainWindow::~MainWindow()
{
    delete ui;
    delete []name_cell_str;
}


void MainWindow::changed_complete() //если игрок устал и не хочет больше играть, он может сам завершить игру
{
   w_end->it_is_not_win();
   w_end->show();
   this->close();
}

void MainWindow::other_game()
{
    mix_cell();
    set_name_cell(name_cell,name_cell_str);
    for (int i=0;i<size*size;i++)
    {
        if (i==size*size-1)
        {
            cells[i]->setText("");
        }
       else {
        cells[i]->setText(name_cell_str[i]);
       }
    }
    this->show();
}

void MainWindow::game_over_with_win()
{
    //здесь таймер, чтоб игровое окно не закрывалось слишком быстро и игрок вообще успел понять, что он выиграл
    QTimer::singleShot(1000,[this](){
        w_end->it_is_win();
        w_end->show();
        this->close();});
}


void MainWindow::set_name_cell(vector <int> name_cell,QString * name_cell_str_)
{
    for (int i=0;i<size*size;i++)
    {
        if (name_cell[i]>=100)
        {
            char first=name_cell[i]/100+'0';
            char second=name_cell[i]/10%10+'0';
            char second_one=name_cell[i]%10+'0';
            name_cell_str_[i]=first;
            name_cell_str_[i].append(second);
            name_cell_str_[i].append(second_one);
        }
        if (name_cell[i]>=10) //в аски таблице нет двузначных чисел, поэтому мы создаем их искусственно
        {
            char first=name_cell[i]/10+'0';
            char second=name_cell[i]%10+'0';
            name_cell_str_[i]=first;
            name_cell_str_[i].append(second);
        }
        else {name_cell_str_[i]=name_cell[i]+'0';} //цифры 0-9 в аски таблице есть
    }
}

void MainWindow::mix_cell()
{

    //функция перемешивает элементы вектора
    random_shuffle(name_cell.begin(),name_cell.end()-1,[](int n){return rand() % n ;});
    //игра решаема, если количество перестановок четное
    //одна перестановка - это когда в паре элементов элемент с меньшим индексом больше, чем элемент с бОльшим индексом
    int perestanovki=0;
    for (size_t i=name_cell.size()-2;i>0;i--)
    {
        for (size_t j=0;j<i;j++)
        {
            if (name_cell[i]<name_cell[j] )
            {
                ++perestanovki;
            }
        }

    }
    if (perestanovki%2!=0)
    {
        //если количество перестановок нечетное, мы просто меняем 2 последних элемента друг с другом
        name_cell[name_cell.size()-1]=name_cell[name_cell.size()-2];
        name_cell[name_cell.size()-2]=name_cell[name_cell.size()-3];
        name_cell[name_cell.size()-3]=name_cell[name_cell.size()-1];
        name_cell[name_cell.size()-1]=size*size;
    }
}

void MainWindow::redrawing()
{
    for (int i=0;i<size*size;i++)
    {QObject::connect(cells[i],&QPushButton::clicked, this, &MainWindow::changed_cell);}

}

void MainWindow::changed_cell()
{

    QObject * s=QObject::sender();
     //qDebug() << s<<Qt::endl;
    if (s!=nullptr)
    {//qDebug()<<s->objectName()<<" ";
    int number_sender=0;
    int number_null=0;
    for (int i=0;i<size*size;i++)
    {
        if (s->objectName()==cells[i]->objectName())
        {
            number_sender=i;
        }
        if (cells[i]->text()=="")
        {
          number_null=i;
        }
    }
    if (is_good_position(what_is_position(number_sender),what_is_position(number_null)))
    {
        cells[number_null]->setText(cells[number_sender]->text());
        cells[number_sender]->setText("");
        if (is_win())
        {
            emit gameover();
        }
    }}

}

vector <int> MainWindow::what_is_position(int cur_cell)
{
    vector <int> position;
    position.push_back(cur_cell/size);
    position.push_back(cur_cell%size);
    return position;
}


bool MainWindow::is_good_position(vector <int> position_1,vector <int> position_2)
{
    if (position_1[0]==position_2[0])
    {
        if (abs(position_1[1]-position_2[1])==1)
        {
            return true;
        }
    }
    if (position_1[1]==position_2[1])
    {
        if (abs(position_1[0]-position_2[0])==1)
        {
            return true;
        }
    }
    return false;
}

bool MainWindow::is_win()
{
    vector <int> name_cell_copy;
    for (int i=0;i<size*size;i++)
    {
        name_cell_copy.push_back(i+1);
    }
    QString * name_cell_str_copy = new QString[size*size];
    set_name_cell(name_cell_copy,name_cell_str_copy);
    for (int i=0;i<size*size-1;i++)
    {

        if (cells[i]->text()!=name_cell_str[i])
        {
            delete []name_cell_str_copy;
            return false;
        }
    }
    delete []name_cell_str_copy;
    return true;
}
