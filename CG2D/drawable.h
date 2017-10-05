#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <QtWidgets>

class Drawable {
public:
  virtual void draw(QPixmap &pixmap) const = 0;
  virtual QString toString() = 0;

protected:
  QString pToStr(QPoint const &p) {
    return "(" + QString::number(p.x()) + ", " + QString::number(p.y()) + ")";
  }
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

  QString toString() override { return "Point " + pToStr(center); }
};

class DrawableLine : public Drawable {
private:
  QPoint p1, p2;

public:
  DrawableLine(QPoint pFrom, QPoint pTo) : p1(pFrom), p2(pTo) {}
  void draw(QPixmap &pixmap) const override {
    QPainter p(&pixmap);
    p.setBrush(QBrush(Qt::black));
    p.setPen(QPen(Qt::black, 2));
    p.drawLine(p1, p2);
  }

  QString toString() override {
    return "Line " + pToStr(p1) + " -- " + pToStr(p2);
  }
};

#endif // DRAWABLE_H
