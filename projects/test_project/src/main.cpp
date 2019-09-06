#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  qDebug() << "Say hello to my little friend";

  return a.exec();
}
