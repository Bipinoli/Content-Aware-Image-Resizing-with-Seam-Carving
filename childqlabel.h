#ifndef CHILDQLABEL_H
#define CHILDQLABEL_H

#include <QLabel>

class childQlabel: public QLabel{
    Q_OBJECT

public:
      childQlabel(QWidget *parent = Q_NULLPTR): QLabel(parent){}

      void mousePressEvent(QMouseEvent *event);

      void mouseReleaseEvent(QMouseEvent *event);
private:
        // pairs hold the x and y value of pixel
        // x to the right from top left
        // y to the down from top left
        std::pair<int, int> MouseDownPair;
        std::pair<int, int> MouseUpPair;
        bool pairInSideImage = false;
};

#endif // CHILDQLABEL_H
