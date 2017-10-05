#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <QtWidgets>

class Drawable {
public:
  virtual void draw(QPixmap &pixmap) const = 0;
};

class DrawablePoint : public Drawable {
private:
  QPoint center;

public:
  DrawablePoint(QPoint p) : center(p) {}
  void draw(QPixmap &pixmap) const override {
    QPainter p(&pixmap);
    p.setBrush(QBrush(Qt::black));
    p.setPen(Qt::black);
    p.drawEllipse(center, 3, 3);
  }
};

#endif // DRAWABLE_H
