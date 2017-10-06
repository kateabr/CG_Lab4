#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "matrix3x2.h"
#include <QtWidgets>

class Drawable {
public:
  virtual void draw(QPixmap &pixmap) const = 0;
  virtual QString toString() = 0;
  virtual void update(Matrix3x2 &tr) = 0;

protected:
  QString pToStr(QPointF const &p) {
    return "(" + QString::number(p.x()) + ", " + QString::number(p.y()) + ")";
  }
};

class DrawablePoint : public Drawable {
private:
  QPointF center;

public:
  DrawablePoint(QPointF p) : center(p) {}
  void draw(QPixmap &pixmap) const override {
    QPainter p(&pixmap);
    p.setBrush(QBrush(Qt::black));
    p.setPen(Qt::black);
    p.drawEllipse(center, 3, 3);
  }

  void update(Matrix3x2 &tr) override { center = tr.mul(center); }

  QString toString() override { return "Point " + pToStr(center); }
};

class DrawableLine : public Drawable {
private:
  QPointF p1, p2;

public:
  DrawableLine(QPointF pFrom, QPointF pTo) : p1(pFrom), p2(pTo) {}
  void draw(QPixmap &pixmap) const override {
    QPainter p(&pixmap);
    p.setBrush(QBrush(Qt::black));
    p.setPen(QPen(Qt::black, 2));
    p.drawLine(p1, p2);
  }

  void update(Matrix3x2 &tr) override {
    p1 = tr.mul(p1);
    p2 = tr.mul(p2);
  }

  QString toString() override {
    return "Line " + pToStr(p1) + " -- " + pToStr(p2);
  }
};

class DrawablePolygon : public Drawable {
private:
  QVector<QPointF> points;

public:
  DrawablePolygon(QVector<QPointF> ps) : points(ps) {}
  void draw(QPixmap &pixmap) const override {
    QPainter p(&pixmap);
    p.setBrush(QBrush(Qt::black));
    p.setPen(QPen(Qt::black, 2));
    for (int i = 0; i < points.size(); ++i)
      p.drawLine(points[i], points[(i + 1) % points.size()]);
  }

  void update(Matrix3x2 &tr) override {
    for (int i = 0; i < points.size(); ++i)
      points[i] = tr.mul(points[i]);
  }

  QString toString() override {
    return "Polygon (" + QString::number(points.size()) + " angles)";
  }
};

#endif // DRAWABLE_H
