#ifndef PAINTCANVAS_H
#define PAINTCANVAS_H

#include "drawable.h"
#include "primitives.h"
#include <QtWidgets>

class PaintCanvas : public QFrame {
  Q_OBJECT

public:
  PaintCanvas(QWidget *parent = nullptr);
  void setColor(QColor c);
  QPixmap getPixmap() const;
  void setCurrentPrimitive(Primitives p);

public slots:
  void setThickness(int);
  void clearArea();

protected:
  void paintEvent(QPaintEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void resizeEvent(QResizeEvent *);

private:
  QColor color = Qt::black;
  QColor bgColor = Qt::white;
  bool mousePressed = false;
  QPixmap pixmap;
  QPoint curPos;
  int penThickness = 2;
  QVector<Drawable *> drawables;
  Primitives currentPrimitive;
};

#endif // PAINTCANVAS_H
