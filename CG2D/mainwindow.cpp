#include "mainwindow.h"
#include "primitives.h"
#include "tablemodel.h"
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

  TableModel *tModel = new TableModel();

  ui->primitivesList->setModel(tModel);
  ui->canvas->setTableModel(tModel);

  connect(ui->primitivesList, &MyListView::itemRemoved, [&]() {
    ui->canvas->clearArea();
    ui->canvas->repaint();
  });
}

MainWindow::~MainWindow() { delete ui; }
