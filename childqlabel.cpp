#include "childqlabel.h"
#include <QDebug>
#include <QMouseEvent>
#include <QMessageBox>

#include "globalvariables.h"
#include <iostream>

std::vector< std::pair< std::pair<int, int>, std::pair<int, int> > > v;

void childQlabel::mousePressEvent(QMouseEvent *event){
//    qDebug() << "Rows: " << IMAGE_ROWS_  << endl
//             << "Cols: " << IMAGE_COLS_;

    if (event->buttons() == Qt::LeftButton){
        qDebug() << "mouse pressed at: " << event->y() << " " << event->x()
                 << endl;
        int piX = event->y() < IMAGE_ROWS_ ? event->y() : -1;
        int piY = event->x() < IMAGE_COLS_ ? event->x() : -1;
        if (piX != -1 and piY != -1){
            pairInSideImage = true;
            qDebug() << "Pixel: " << piX << " " << piY << endl;
            MouseDownPair.first = piY;
            MouseDownPair.second = piX;
        }
    }
    else if(event->buttons() == Qt::RightButton){
        qDebug() << "there are" << v.size() << " masks" << endl;
    }

}

void childQlabel::mouseReleaseEvent(QMouseEvent *event){

    qDebug() << "mouse released at: " << event->y() << " " << event->x() << endl;
    int piX = event->y() < IMAGE_ROWS_ ? event->y() : -1;
    int piY = event->x() < IMAGE_COLS_ ? event->x() : -1;
    if (pairInSideImage and piX != -1 and piY != -1){
        pairInSideImage = false;
        qDebug() << "Pixel: " << piX << " " << piY << endl;
        MouseUpPair.first = piY;
        MouseUpPair.second = piX;

        int ans = QMessageBox::question(this, "You want to mask?",
                              "Do you want to avoid the seams"
                              " in the area selected?");
        if (ans == QMessageBox::Yes){
            std::pair< std::pair<int,int>, std::pair<int,int> > temp;
            temp.first = MouseDownPair;
            temp.second = MouseUpPair;
            v.push_back(temp);
        }

    }
}
