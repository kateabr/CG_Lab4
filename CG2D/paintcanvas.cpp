#include "paintcanvas.h"
#include <queue>

PaintCanvas::PaintCanvas(QWidget *parent) : QFrame(parent) {
  pixmap = QPixmap(this->width(), this->height());
  pixmap.fill();
}

void PaintCanvas::setColor(QColor c) { color = c; }

QPixmap PaintCanvas::getPixmap() const { return pixmap; }

void PaintCanvas::setCurrentPrimitive(Primitives p) { currentPrimitive = p; }

void PaintCanvas::setThickness(int val) { penThickness = val; }

void PaintCanvas::clearArea() {
  pixmap.fill();
  repaint();
}

void PaintCanvas::paintEvent(QPaintEvent *) {
  pixmap.fill();
  for (Drawable *d : drawables)
    d->draw(pixmap);
  QPainter p(this);
  p.drawPixmap(0, 0, pixmap);
}

void PaintCanvas::mousePressEvent(QMouseEvent *e) {
  curPos = e->pos();
  if (e->buttons() & Qt::LeftButton) {
    switch (currentPrimitive) {
    case Primitives::Point:
      drawables.push_back(new DrawablePoint(curPos));
      break;
    case Primitives::Line:
      break;
    case Primitives::Polygon:
      break;
    }
  } else
    mousePressed = true;
  repaint();
}

void PaintCanvas::mouseReleaseEvent(QMouseEvent *) { mousePressed = false; }

void PaintCanvas::mouseMoveEvent(QMouseEvent *e) {
  if (!mousePressed)
    return;
  QPainter p(&pixmap);
  // p.setRenderHint(QPainter::Antialiasing);
  if (e->buttons() & Qt::LeftButton)
    p.setPen(
        QPen(color, penThickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  p.drawLine(curPos, e->pos());
  curPos = e->pos();
  repaint();
}

void PaintCanvas::resizeEvent(QResizeEvent *) {
  pixmap = QPixmap(width(), height());
  pixmap.fill();
}
