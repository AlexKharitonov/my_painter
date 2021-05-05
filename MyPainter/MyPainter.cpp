#include "MyPainter.h"
#include <QAction>
#include <QColor>
#include <QColorDialog>
#include <QFileDialog>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QSize>
#include <QSpinBox>
#include <QString>
#include <QVBoxLayout>
#include "./ui_MyPainter.h"

MyPainter::MyPainter(QWidget* parent)
  : QMainWindow(parent), ui(new Ui::MyPainter) {
  ui->setupUi(this);
  pen_width_ = 2;
  pen_color_ = new QColor(Qt::black);
  brush_ = new QBrush(Qt::white);
  is_dlg_chose_pen_color_open_ = false;
  is_dlg_chose_brush_color_open_ = false;
  curent_file_exist_ = false;
  start_point_ = QPoint(0, 0);
  end_point_ = QPoint(0, 0);
  mouse_not_released_ = false;
  drawing_mode = circle;
  image_ = new QImage(":/new/prefix1/clear.png");

  QWidget* line_width = new QWidget(ui->tool_bar_figures);
  QVBoxLayout* line_width_layout = new QVBoxLayout(line_width);
  QSpinBox* line_width_spin = new QSpinBox(line_width);
  QLabel* line_width_name = new QLabel("Line width", line_width);
  line_width_layout->addWidget(line_width_name);
  line_width_layout->addWidget(line_width_spin);
  line_width->setLayout(line_width_layout);

  line_width_spin->setValue(pen_width_);

  connect(line_width_spin,
          static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this,
          [this](int i) { this->pen_width_ = i; });

  ui->tool_bar_propereties->addWidget(line_width);

  line_color_button_ = new QPushButton("Line color", this);
  constexpr uint k_temp_pix_size = 10;
  QPixmap temp_pixmap(k_temp_pix_size, k_temp_pix_size);
  temp_pixmap.fill(*pen_color_);
  line_color_button_->setIcon(temp_pixmap);
  line_color_button_->setIconSize(temp_pixmap.size());

  connect(line_color_button_, &QPushButton::clicked, this,
          &MyPainter::change_pen_color);

  ui->tool_bar_propereties->addWidget(line_color_button_);

  brush_color_button_ = new QPushButton("Brush color", this);
  temp_pixmap.fill(brush_->color());
  brush_color_button_->setIcon(temp_pixmap);
  brush_color_button_->setIconSize(temp_pixmap.size());

  connect(brush_color_button_, &QPushButton::clicked, this,
          &MyPainter::change_brush_color);

  ui->tool_bar_propereties->addWidget(brush_color_button_);

  connect(ui->action_open, &QAction::triggered, this,
          &MyPainter::open_new_image);

  connect(ui->action_save, &QAction::triggered, this, &MyPainter::save_image);

  connect(ui->action_circle, &QAction::triggered, this, [this]() {
      drawing_mode = circle;
      ui->action_line->setChecked(false);
      ui->action_rectangle->setChecked(false);
      ui->action_triangle->setChecked(false);
    });

  connect(ui->action_line, &QAction::triggered, this, [this]() {
      drawing_mode = line;
      ui->action_circle->setChecked(false);
      ui->action_rectangle->setChecked(false);
      ui->action_triangle->setChecked(false);
    });

  connect(ui->action_rectangle, &QAction::triggered, this, [this]() {
      drawing_mode = rect;
      ui->action_circle->setChecked(false);
      ui->action_line->setChecked(false);
      ui->action_triangle->setChecked(false);
    });

  connect(ui->action_triangle, &QAction::triggered, this, [this]() {
      drawing_mode = triang;
      ui->action_circle->setChecked(false);
      ui->action_line->setChecked(false);
      ui->action_rectangle->setChecked(false);
    });
}

MyPainter::~MyPainter() {
  delete ui;
}

void MyPainter::change_pen_color() {
  if (!is_dlg_chose_pen_color_open_) {
      QColorDialog* dlg_chose_pen_color = new QColorDialog();
      if (dlg_chose_pen_color)
        is_dlg_chose_pen_color_open_ = true;
      dlg_chose_pen_color->show();

      connect(dlg_chose_pen_color, &QColorDialog::colorSelected, this,
              [this, dlg_chose_pen_color](const QColor& new_color) {
          *pen_color_ = new_color;
          dlg_chose_pen_color->close();
          QPixmap temp_pixmap(10, 10);
          temp_pixmap.fill(*pen_color_);
          line_color_button_->setIcon(temp_pixmap);
          is_dlg_chose_pen_color_open_ = false;
        });

      connect(dlg_chose_pen_color, &QColorDialog::finished, this,
              [this]() { is_dlg_chose_pen_color_open_ = false; });
    }
}

void MyPainter::change_brush_color() {
  if (!is_dlg_chose_brush_color_open_) {
      QColorDialog* dlg_chose_brush_color = new QColorDialog();
      if (dlg_chose_brush_color)
        is_dlg_chose_brush_color_open_ = true;
      dlg_chose_brush_color->show();

      connect(dlg_chose_brush_color, &QColorDialog::colorSelected, this,
              [this, dlg_chose_brush_color](const QColor& new_color) {
          brush_->setColor(new_color);
          dlg_chose_brush_color->close();
          constexpr uint k_temp_pix_size = 10;
          QPixmap temp_pixmap(k_temp_pix_size, k_temp_pix_size);
          temp_pixmap.fill(brush_->color());
          brush_color_button_->setIcon(temp_pixmap);
          is_dlg_chose_brush_color_open_ = false;
        });

      connect(dlg_chose_brush_color, &QColorDialog::finished, this,
              [this]() { is_dlg_chose_brush_color_open_ = false; });
    }
}

void MyPainter::open_new_image() {
  QString file_name;
  if (curent_file_exist_) {
      QMessageBox msg;
      msg.setText("You have opened project. Would you like to save it?");
      msg.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel |
                             QMessageBox::Discard);
      msg.setDefaultButton(QMessageBox::Save);

      int answer = msg.exec();

      switch (answer) {
        case QMessageBox::Cancel:
          return;
        case QMessageBox::Discard:
          break;
        case QMessageBox::Save:
          file_name =
              QFileDialog::getSaveFileName(this, tr("Save image"), "./",
                                           tr("Image files (*.png *.jpg *.bmp)"));
          image_->save(file_name);
        }      
    }

  file_name = QFileDialog::getOpenFileName(
        this, tr("Open image"), "./", tr("Image files (*.png *.jpg *.bmp)"));

  image_->load(file_name);
  curent_file_exist_ = true;
  update();
}

void MyPainter::save_image() {
  QString file_name = QFileDialog::getSaveFileName(
        this, tr("Save image"), "./", tr("Image files (*.png *.jpg *.bmp)"));
  image_->save(file_name);
}

void MyPainter::paintEvent(QPaintEvent* event) {
  QMainWindow::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.drawImage(QPoint(image_shift_left, image_shift_high), *image_);
  painter.setBrush(*brush_);
  painter.setPen(QPen(*pen_color_, pen_width_));

  if (mouse_not_released_) {
      constexpr uint triang_points_count = 3;
      QPoint point_arr[triang_points_count] = {start_point_, end_point_,
                                               QPoint(start_point_.x(), end_point_.y())};
      switch (drawing_mode) {
        case circle:
          painter.drawEllipse(QRect(start_point_, end_point_));
          break;

        case rect:
          painter.drawRect(QRect(start_point_, end_point_));
          break;

        case triang:
          painter.drawPolygon(point_arr, triang_points_count);
          break;

        case line:
          painter.drawLine(start_point_, end_point_);
          break;
        }
    }
}

void MyPainter::mousePressEvent(QMouseEvent* event) {
  start_point_ = event->pos();
  mouse_not_released_ = true;
}

void MyPainter::mouseMoveEvent(QMouseEvent* event) {
  end_point_ = event->pos();
  update();
}

void MyPainter::mouseReleaseEvent(QMouseEvent* event) {
  mouse_not_released_ = false;

  QPainter painter(image_);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.translate(-image_shift_left, -image_shift_high);
  painter.setBrush(*brush_);
  painter.setPen(QPen(*pen_color_, pen_width_));

  constexpr uint triang_points_count = 3;
  QPoint point_arr[triang_points_count] = {start_point_, end_point_,
                                           QPoint(start_point_.x(), end_point_.y())};
  switch (drawing_mode) {
    case circle:
      painter.drawEllipse(QRect(start_point_, end_point_));
      break;

    case rect:
      painter.drawRect(QRect(start_point_, end_point_));
      break;

    case triang:
      painter.drawPolygon(point_arr, triang_points_count);
      break;

    case line:
      painter.drawLine(start_point_, end_point_);
      break;
    }
}
