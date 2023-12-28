
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QQueue>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QImage glowny,docelowy,im1, im2;
    QQueue<QPoint> glownePunkty,glownePunkty2,docelowePunkty;
    QPoint startPos, endPos;
    int glownyWyszukiwanie=-1, glownyWyszukiwanie2=-1;

    int klatka=0, klatki=20;
    int zdjecie = 1;


    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void drawPixel(QImage *im,int x,int y,int r,int g,int b);
    void drawLine(QImage *im,int x1,int y1,int x2,int y2);
    void drawCircle2(QImage *im,int x0,int y0,int x1,int y1);
    void draw();
    void Trojkat(QImage *im,QQueue<QPoint> punkty, bool rysowanie);
    void Wypelnianie(int tryb);
    int findPoints(QPoint p, QQueue<QPoint> points);

private:
    Ui::MainWindow *ui;

public slots:
    void zdjecie1();
    void zdjecie2();
    void klatkowanie(int klatka);
};

#endif // MAINWINDOW_H
