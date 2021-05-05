#include <MyPainter.h>
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MyPainter painter;
  painter.show();

  return a.exec();
}
