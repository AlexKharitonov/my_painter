#ifndef MYPAINTER_H
#define MYPAINTER_H

#include <QMainWindow>
class QPicture;
class QColor;
class QBrush;
class QPushButton;
class QImage;

QT_BEGIN_NAMESPACE namespace Ui {
  class MyPainter;
}
QT_END_NAMESPACE

class MyPainter final : public QMainWindow {
  Q_OBJECT

 public:
  MyPainter(QWidget *parent = nullptr);
  ~MyPainter();

 public slots:
  void change_pen_color();
  void change_brush_color();
  void open_new_image();
  void save_image();

 private:
  enum { triang, rect, circle, line } drawing_mode;

  Ui::MyPainter* ui;
  QImage* image_;
  QColor* pen_color_;
  QBrush* brush_;
  quint8 pen_width_;
  QPoint start_point_;
  QPoint end_point_;

  QPushButton* line_color_button_;
  QPushButton* brush_color_button_;

  bool is_dlg_chose_pen_color_open_;
  bool is_dlg_chose_brush_color_open_;
  bool curent_file_exist_;
  bool mouse_not_released_;

  const int image_shift_left = 50;
  const int image_shift_high = 70;

 protected:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
};

#endif  // MYPAINTER_H
