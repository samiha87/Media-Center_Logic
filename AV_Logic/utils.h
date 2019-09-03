#ifndef UTILS_H
#define UTILS_H

#include <QObject>

class Utils : public QObject
{
    Q_OBJECT
public:
    explicit Utils(QObject *parent = nullptr);
    // Clean received message from hardware layer. Ensures that message is clean and has no extra"
signals:

public slots:
};

#endif // UTILS_H
