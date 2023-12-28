
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    glowny  = QImage(ui->glowny->width(),ui->glowny->height(),QImage::Format_RGB32);
    docelowy  = QImage(ui->docelowy->width(),ui->docelowy->height(),QImage::Format_RGB32);
    draw();

    connect(ui->Prev,SIGNAL(clicked()),this,SLOT(zdjecie1()));
    connect(ui->Next,SIGNAL(clicked()),this,SLOT(zdjecie2()));

    ui->klatki->setRange(0,klatki);
    connect(ui->klatki, SIGNAL(valueChanged(int)), this, SLOT(klatkowanie(int)));
    connect(ui->klatkiS, SIGNAL(valueChanged(int)), this, SLOT(klatkowanie(int)));
    connect(ui->klatki,  &QSlider::valueChanged, ui->klatkiS, &QSpinBox::setValue);
    connect(ui->klatkiS, QOverload<int>::of(&QSpinBox::valueChanged), ui->klatki, &QSlider::setValue);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(ui->glowny->x(), ui->glowny->y(), glowny);
    p.drawImage(ui->docelowy->x(), ui->docelowy->y(), docelowy);
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    QPoint glownyMysz = ui->glowny->pos();
//    QPoint docelowyMysz = ui->docelowy->pos();
    int glownySzerokosc = ui->glowny->width();
    int glownyWysokosc = ui->docelowy->height();
//    int docelowySzerokosc = ui->docelowy->width(); !!!NIE POTRZEBNE BO NIC NA NIM NIE ROBIMY OPROCZ WYNIKU
//    int docelowyWysokosc = ui->docelowy->height(); !!!NIE POTRZEBNE BO NIC NA NIM NIE ROBIMY OPROCZ WYNIKU

    if (e->pos().x() >= glownyMysz.x() && e->pos().x() <= glownyMysz.x() + glownySzerokosc &&
        e->pos().y() >= glownyMysz.y() && e->pos().y() <= glownyMysz.y() + glownyWysokosc) {
        startPos = e->pos() - glownyMysz; // Zeby punkty nie rysowaly sie wyzej przez pasek nad widget
        if (e->button() == Qt::LeftButton)
        {
            if(zdjecie == 1)
            {
                if(glownePunkty.size()<3) glownePunkty.append(startPos);
            }
            else if (zdjecie == 2)
            {
                if(glownePunkty2.size()<3) glownePunkty2.append(startPos);
            }
        }
        else if (e->buttons() & Qt::RightButton)
        {
            if(zdjecie == 1)
            {
                glownyWyszukiwanie = findPoints(startPos, glownePunkty);
            }
            else if (zdjecie == 2)
            {
                glownyWyszukiwanie2 = findPoints(startPos, glownePunkty2);
            }
        }
    }
//    else if (e->pos().x() >= docelowyMysz.x() && e->pos().x() <= docelowyMysz.x() + docelowySzerokosc &&
//             e->pos().y() >= docelowyMysz.y() && e->pos().y() <= docelowyMysz.y() + docelowyWysokosc) {
//        startPos = e->pos() - docelowyMysz; // Zeby punkty nie rysowaly sie wyzej przez pasek nad widget

//        if (e->button() == Qt::LeftButton)
//        {
//            if(docelowePunkty.size()<3) docelowePunkty.append(startPos);
//        }
//        else if (e->buttons() & Qt::RightButton)
//        {
//            docelowyWyszukiwanie = findPoints(startPos, docelowePunkty);
//        }
//    }

    draw();
    QMainWindow::mousePressEvent(e);
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    QPoint glownyMysz = ui->glowny->pos();
//    QPoint docelowyMysz = ui->docelowy->pos(); !!!NIE POTRZEBNE BO NIC NA NIM NIE ROBIMY OPROCZ WYNIKU
    int glownySzerokosc = ui->glowny->width();
    int glownyWysokosc = ui->docelowy->height();
//    int docelowySzerokosc = ui->glowny->width(); !!!NIE POTRZEBNE BO NIC NA NIM NIE ROBIMY OPROCZ WYNIKU
//    int docelowyWysokosc = ui->docelowy->height(); !!!NIE POTRZEBNE BO NIC NA NIM NIE ROBIMY OPROCZ WYNIKU

    if (e->pos().x() >= glownyMysz.x() && e->pos().x() <= glownyMysz.x() + glownySzerokosc &&
        e->pos().y() >= glownyMysz.y() && e->pos().y() <= glownyMysz.y() + glownyWysokosc)
    {
        endPos = e->pos() - glownyMysz; //jezeli tego nie ma to wraca do punktu (0,0)
        if (e->buttons() & Qt::RightButton)
        {
            if(zdjecie == 1 && glownyWyszukiwanie != -1)
            {
                glownePunkty.replace(glownyWyszukiwanie, endPos);
                    draw();
            }
            else if(zdjecie == 2 && glownyWyszukiwanie2 != -1)
            {
                glownePunkty2.replace(glownyWyszukiwanie2, endPos);
                    draw();
            }

        }

    }
//    else if (e->pos().x() >= docelowyMysz.x() && e->pos().x() <= docelowyMysz.x() + docelowySzerokosc &&
//             e->pos().y() >= docelowyMysz.y() && e->pos().y() <= docelowyMysz.y() + docelowyWysokosc)
//    {
//        endPos = e->pos() - docelowyMysz;//jezeli tego nie ma to wraca do punktu (0,0)
//        if (e->buttons() & Qt::RightButton)
//        {
//            docelowePunkty.replace(docelowyWyszukiwanie, endPos);
//            draw();
//        }
//    }

}

void MainWindow::drawPixel(QImage *im,int x,int y,int r,int g,int b)
{
    if(x<im->width() && x>=0 && y<im->height() && y>=0)
    {
        uchar *pix = im->scanLine(y) + 4 * x;
        pix[0] = r;
        pix[1] = g;
        pix[2] = b;
    }
}


void MainWindow::drawLine(QImage *im,int x1,int y1,int x2,int y2)
{
    //Uwzglednic ze oba punkty są zerami !! bo moze byc problem
    int x, y;
    //    if(x1>im.width()) x1 = im.width();
    //    if(x2>im.width()) x2 = im.width();
    //    if(x1<0) x1 = 0;
    //    if(x2<0) x2 = 0;
    //    if(y1>im.height()) y1 = im.height();
    //    if(y2>im.height()) y2 = im.height();
    //    if(y1<0) y1 = 0;
    //    if(y2<0) y2 = 0;
    if((x1 == x2) && (y1==y2))
    {
        drawPixel(im,x1,y1,255,255,255);
        return;
    }
    if(abs(y2-y1) <= abs(x2-x1))
    {
        /*
         Pojawienie sie problemu z rysowaniem linii w z prawej do lewej czyli zmniejszanie wartości
         aby to zrobić należało zmienić kolejność aby uzyskiwac poprawne wartości co gwarantuje nam podany warunek
        */
        if (x1 > x2)
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        for(x=x1; x<=x2; x++)
        {
            y=y1+(x-x1)*(y2-y1)/(x2-x1);
            drawPixel(im,x, y, 200, 200, 200);
        }
    }
    else
    {
        /*
         Pojawienie sie problemu z rysowaniem linii w z góry na dół czyli zmniejszanie wartości
         aby to zrobić należało zmienić kolejność aby uzyskiwac poprawne wartości co gwarantuje nam podany warunek
        */
        if (y1 > y2)
        {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        for(y=y1; y<=y2; y++)
        {
            x=x1+(y-y1)*(x2-x1)/(y2-y1);
            drawPixel(im,x, y, 200, 200, 200);
        }
    }
}

void MainWindow::drawCircle2(QImage *im,int x0,int y0,int x1,int y1)
{
    float R = sqrt(((x1-x0)*(x1-x0))+((y1-y0)*(y1-y0)));
    for(int i=-R; i<=R; i++)
    {
        double y = sqrt(R*R - i*i);
        drawPixel(im,i+x0,(int)floor(y+0.5)+y0,255,255,255);      // prawy dół
        drawPixel(im,i+x0,-(int)floor(y+0.5)+y0,255,255,255);    //prawa góra
        drawPixel(im,-i+x0,(int)floor(y+0.5)+y0,255,255,255);   //lewy dół
        drawPixel(im,-i+x0,-(int)floor(y+0.5)+y0,255,255,255);
        drawPixel(im,(int)floor(y+0.5)+x0,i+y0,255,255,255);       //prawy środek "dół"
        drawPixel(im,-(int)floor(y+0.5)+x0,i+y0,255,255,255);     //lewy środek "dół"
        drawPixel(im,(int)floor(y+0.5)+x0,-i+y0,255,255,255);    //prawy środek "góra"
        drawPixel(im,-(int)floor(y+0.5)+x0,-i+y0,255,255,255);  //lewy środek "góra"//lewa góra
    }
}

void MainWindow::draw()
{
    if(zdjecie==1){
        glowny.load("C:/Users/rosia/OneDrive/Pulpit/Studia/Grafika Komputerowa/Zdjecia/green.jpg");
        im1=glowny;
    }
    else if(zdjecie==2){
        glowny.load("/Users/rosia/OneDrive/Pulpit/Studia/Grafika Komputerowa/Zdjecia/blue.jpg");
        im2=glowny;
    }
    docelowy.fill(0);

    if(zdjecie==1)
        Trojkat(&glowny, glownePunkty, true);
    else if(zdjecie==2)
        Trojkat(&glowny, glownePunkty2, true);

    if(glownePunkty.size()==3 && glownePunkty2.size()==3)
    {
        docelowePunkty.clear();
        for(int i=0; i<3; i++){
            float x, y;
            //wzory z prezentacji na klatki pośrednie
            x = (1.0f-klatka/(double)klatki)*glownePunkty[i].x() + (klatka/(double)klatki)*glownePunkty2[i].x();
            y = (1.0f-klatka/(double)klatki)*glownePunkty[i].y() + (klatka/(double)klatki)*glownePunkty2[i].y();
            docelowePunkty.append({(int)round(x), (int)round(y)}); //pixele tylko  w int wiec aokrąglamy
        }
        Wypelnianie(1);
        Trojkat(&docelowy, docelowePunkty, false);
    }
    else if(glownePunkty.size()==3 && klatka==0){
        docelowePunkty=glownePunkty;
        Wypelnianie(2);
        Trojkat(&docelowy, docelowePunkty, false);
    }
    else if(glownePunkty2.size()==3 && klatka==klatki){
        docelowePunkty=glownePunkty2;
        Wypelnianie(3);
        Trojkat(&docelowy, docelowePunkty, false);
    }

    update();
}

void MainWindow::Trojkat(QImage *im,QQueue<QPoint> punkty, bool rysowanie)
{
    for(int i=0;i<punkty.size();i++)
    {
        if(rysowanie)
            drawCircle2(im,punkty.at(i).x(),punkty.at(i).y(),punkty.at(i).x()+4,punkty.at(i).y());

        if(punkty.size()==3)
        {
            drawLine(im,punkty.at(i).x(),punkty.at(i).y(),punkty.at((i+1)%3).x(),punkty.at((i+1)%3).y());
        }
    }

}

//Funkcja bazwoana na scanLine
void MainWindow::Wypelnianie(int tryb)
{
    int p1, p2;
    QVector<int> x;

    if (docelowePunkty.size() < 2)
        return; // Jeśli mniej niż 3 punkty na linii, zakończ funkcję

    int yMax = docelowePunkty[0].y(); // Największa wartość y
    int yMin = docelowePunkty[0].y(); // Najmniejsza wartość y

    // Znajdź największą i najmniejszą wartość y na linii
    for (int i = 0; i < docelowePunkty.size(); i++)
    {
        if (docelowePunkty[i].y() > yMax)
            yMax = docelowePunkty[i].y();
        if (docelowePunkty[i].y() < yMin)
            yMin = docelowePunkty[i].y();
    }

    for (int y = yMin; y <= yMax; y++)
    {
        for (int i = 0; i < docelowePunkty.size(); i++)
        {
            p1 = i;
            p2 = (i + 1) % docelowePunkty.size(); // Zapewnienie działania dla ostatniego punktu

            // Upewnij się, że p1 ma mniejszą wartość y niż p2
            if (docelowePunkty[p1].y() > docelowePunkty[p2].y())
                std::swap(p1, p2);

            // Jeśli y mieści się pomiędzy wartościami  p1 i p2 oraz wartości  p1 i p2 są różne
            if (y >= docelowePunkty[p1].y() && y < docelowePunkty[p2].y() && docelowePunkty[p1].y() != docelowePunkty[p2].y())
            {
                // Oblicz współrzędną x "twierdzenie z prezentacji"
                int xValue = docelowePunkty[p1].x() + (y - docelowePunkty[p1].y()) * (docelowePunkty[p2].x() - docelowePunkty[p1].x()) / (docelowePunkty[p2].y() - docelowePunkty[p1].y());
                x.push_back(xValue);
            }
        }

        std::sort(x.begin(), x.end()); // Posortuj współrzędne x

        if(x.size()>1){
            for(int X=x[0]; X<x[1]; X++){
                double W, Wv, Ww, u, v, w;
                double xt1, yt1,xt2, yt2;
                double alpha=klatka/(double)klatki;
                int red, green, blue;
                uchar *pix1, *pix2;
                QPoint a=docelowePunkty[0], b=docelowePunkty[1], c=docelowePunkty[2];

                W = (b.x() - a.x())*(c.y() - a.y()) - (b.y() - a.y())*(c.x() - a.x());
                Wv = ((double)X - a.x())*(c.y() - a.y()) - ((double)y - a.y())*(c.x() - a.x());
                Ww = (b.x() - a.x())*((double)y - a.y()) - (b.y() - a.y())*((double)X - a.x());
                v=Wv/W;
                w=Ww/W;
                u=1-v-w;

                //tryb 1 na pierwszym tryb 2 na drugim tryb 3 na obu

                if(tryb==1 || tryb==2){
                    QPoint at1=glownePunkty[0], bt1=glownePunkty[1], ct1=glownePunkty[2];
                    xt1=u*at1.x() + v*bt1.x() + w*ct1.x();
                    yt1=u*at1.y() + v*bt1.y() + w*ct1.y();
                    pix1 = im1.scanLine((int)yt1)+4*(int)xt1;
                }
                if(tryb==1 || tryb==3){
                    QPoint at2=glownePunkty2[0], bt2=glownePunkty2[1], ct2=glownePunkty2[2];
                    xt2=u*at2.x() + v*bt2.x() + w*ct2.x();
                    yt2=u*at2.y() + v*bt2.y() + w*ct2.y();
                    pix2 = im2.scanLine((int)yt2)+4*(int)xt2;
                }
                if(tryb==1){
                    red = (int)(alpha * pix2[2] + (1 - alpha) * pix1[2]);
                    green = (int)(alpha * pix2[1] + (1 - alpha) * pix1[1]);
                    blue = (int)(alpha * pix2[0] + (1 - alpha) * pix1[0]);
                }
                else if(tryb==2){
                    red = pix1[2];
                    green = pix1[1];
                    blue = pix1[0];
                }
                else if(tryb==3){
                    red = pix2[2];
                    green = pix2[1];
                    blue = pix2[0];
                }
                docelowy.setPixel(X, y, qRgb(red, green, blue));
            }
        }

        x.clear();

    }
}


int MainWindow::findPoints(QPoint p, QQueue<QPoint> points)
{
    int minDist = -1;
    int n = -1;
    for (int i = 0; i < points.size(); i++){
        int dist = (points.at(i) - p).manhattanLength();
        if (minDist == -1 || dist < minDist) {
            minDist = dist;
            n = i;
        }
    }
    return n;
}

void MainWindow::zdjecie1()
{
    zdjecie = 1;
    draw();
}

void MainWindow::zdjecie2()
{
    zdjecie = 2;
    draw();
}

void MainWindow::klatkowanie(int klatka)
{
    this->klatka=klatka;
    draw();
}


