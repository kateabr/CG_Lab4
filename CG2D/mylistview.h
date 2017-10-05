#ifndef MYLISTVIEW_H
#define MYLISTVIEW_H

#include <QtWidgets>

class MyListView : public QListView {
  Q_OBJECT
public:
  MyListView(QWidget *parent = nullptr) : QListView(parent) {}

signals:
  void itemRemoved();

protected:
  void keyPressEvent(QKeyEvent *e) {
    if ((e->key() == Qt::Key_Delete) && (model()->rowCount() > 0) &&
        (!selectedIndexes().empty())) {
      model()->removeRow(selectedIndexes()[0].row());
      emit itemRemoved();
    }
  }
};

#endif // MYLISTVIEW_H
