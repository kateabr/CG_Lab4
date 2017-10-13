#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include "drawable.h"
#include "matrix3x2.h"
#include "primitives.h"
#include <QtWidgets>

class TableModel : public QAbstractListModel {
  Q_OBJECT
public:
  explicit TableModel(QObject *parent = 0) : QAbstractListModel(parent) {}
  int rowCount(const QModelIndex &parent = QModelIndex()) const override {
    //    qDebug() << "row count " << drawables.size();
    return drawables.size();
  }

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override {
    if (role == Qt::DisplayRole || index.row() < 0 ||
        index.row() >= drawables.size()) {
      return drawables[index.row()]->toString();
    }
    return QVariant();
  }

  QVector<Drawable *> getData() const { return drawables; }

  void add(Drawable *item) {
    beginInsertRows(QModelIndex(), drawables.size(), drawables.size());
    drawables.push_back(item);
    endInsertRows();

    QModelIndex top = createIndex(drawables.size() - 1, 0, nullptr);
    QModelIndex bottom = createIndex(drawables.size() - 1, 0, nullptr);

    emit dataChanged(top, bottom);
  }

  bool removeRows(int row, int count,
                  const QModelIndex &parent = QModelIndex()) override {
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    while (count--)
      delete drawables.takeAt(row);
    endRemoveRows();
    return true;
  }

  void update(Matrix3x2 &tr, QModelIndexList &list, bool aroundCenter) {
    if (!aroundCenter) {
      for (int i = 0; i < list.size(); ++i)
        drawables[list[i].row()]->update(tr);
    } else
      for (int i = 0; i < list.size(); ++i)
        drawables[list[i].row()]->updateCenter(tr);
    QModelIndex top = createIndex(drawables.size() - 1, 0, nullptr);
    QModelIndex bottom = createIndex(drawables.size() - 1, 0, nullptr);
    emit dataChanged(top, bottom);
  }

  void intersect(int ind1, int ind2) {
    if ((drawables[ind1]->whoAmI() == Primitives::Line) &&
        (drawables[ind2]->whoAmI() == Primitives::Line)) {
      QPair<bool, QPointF> res =
          drawables[ind1]->intersectsWith(drawables[ind2]);
      qDebug() << res.first;
      if (res.first) {
        drawables.push_back(new DrawablePoint(res.second));
        QModelIndex top = createIndex(drawables.size() - 2, 0, nullptr);
        QModelIndex bottom = createIndex(drawables.size() - 1, 0, nullptr);
        emit dataChanged(top, bottom);
      }
    }
  }

  QString relativePos(int ind1, int ind2) {
  DrawablePoint *p = nullptr;
  DrawableLine *l = nullptr;
  DrawablePolygon * poly = nullptr;

  if ((drawables[ind1]->whoAmI() == Primitives::Point) &&
      (drawables[ind2]->whoAmI() == Primitives::Line)) {
      p = static_cast<DrawablePoint *>(drawables[ind1]);
      l = static_cast<DrawableLine *>(drawables[ind2]);
      return posToStr(p->relativePosition(*l));
    }
    else if ((drawables[ind1]->whoAmI() == Primitives::Line) &&
             (drawables[ind2]->whoAmI() == Primitives::Point)) {
      p = static_cast<DrawablePoint *>(drawables[ind2]);
      l = static_cast<DrawableLine *>(drawables[ind1]);
      return posToStr(p->relativePosition(*l));
    }
    else if ((drawables[ind1]->whoAmI() == Primitives::Point) &&
             (drawables[ind2]->whoAmI() == Primitives::Polygon)) {
      p = static_cast<DrawablePoint *>(drawables[ind1]);
      poly = static_cast<DrawablePolygon *>(drawables[ind2]);
      return posToStr(p->relPosPolygon(*poly));
    }
     else if ((drawables[ind1]->whoAmI() == Primitives::Polygon) &&
              (drawables[ind2]->whoAmI() == Primitives::Point)) {
      p = static_cast<DrawablePoint *>(drawables[ind2]);
      poly = static_cast<DrawablePolygon *>(drawables[ind1]);
      return posToStr(p->relPosPolygon(*poly));
    }
  }

private:
  QVector<Drawable *> drawables;

  QString posToStr(Position pos) {
      switch (pos) {
          case Position::Left: return "Left";
          case Position::Right: return "Right";
          case Position::Belongs: return "Belongs";
          case Position::Inside: return "Inside";
          case Position::Outside: return "Outside";
      }
  }
};

#endif // TABLEMODEL_H
