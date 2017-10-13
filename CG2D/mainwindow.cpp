#include "mainwindow.h"
#include "matrix3x2.h"
#include "primitives.h"
#include "tablemodel.h"
#include "drawable.h"
#include "ui_mainwindow.h"
#include <functional>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  auto fun = [&](Primitives p) { ui->canvas->setCurrentPrimitive(p); };
  auto funPoint = std::bind(fun, Primitives::Point);
  auto funLine = std::bind(fun, Primitives::Line);
  auto funPolygon = std::bind(fun, Primitives::Polygon);
  connect(ui->selectedPoint, &QRadioButton::clicked, funPoint);
  connect(ui->selectedLine, &QRadioButton::clicked, funLine);
  connect(ui->selectedPolygon, &QRadioButton::clicked, funPolygon);
  connect(ui->findIntersection, &QPushButton::clicked, ui->primitivesList,
          &MyListView::intersect);

  TableModel *tModel = new TableModel();

  ui->primitivesList->setModel(tModel);
  ui->canvas->setTableModel(tModel);
  ui->primitivesList->setSelectionMode(QAbstractItemView::ExtendedSelection);

  connect(ui->primitivesList, &MyListView::itemsChanged, [&]() {
    ui->canvas->clearArea();
    ui->canvas->repaint();
  });
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_applyButton_clicked() {
  float scaleX = ui->scaleX->value() != 0 ? ui->scaleX->value() : 1;
  float scaleY = ui->scaleY->value() != 0 ? ui->scaleY->value() : 1;
  float rad = 0.01745329252 * ui->rotationAngle->value();
  bool zeroAngle = rad == 0;
  Matrix3x2 m(scaleX * (zeroAngle ? 1 : qCos(rad)), scaleY * (zeroAngle ? 0 : qSin(rad)),
              scaleX * (zeroAngle ? 0 : -qSin(rad)), scaleY * (zeroAngle ? 1 : qCos(rad)),
              ui->trX->value(), ui->trY->value());
  if (!ui->centerAsPivot->isChecked()) {
    Matrix3x2 p(1, 0, 0, 1, -ui->pivotX->value(), -ui->pivotY->value());
    ui->primitivesList->update(p, false, false);
    ui->primitivesList->update(m, false, false);
    Matrix3x2 pp(1, 0, 0, 1, ui->pivotX->value(), ui->pivotY->value());
    ui->primitivesList->update(pp, false);
  } else
    ui->primitivesList->update(m, true);
}
