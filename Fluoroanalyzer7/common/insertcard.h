#ifndef INSERTCARD_H
#define INSERTCARD_H

#include <QThread>
#include "xioctl.h"

class CInsertCard : public QThread
{
    Q_OBJECT
public:
    explicit CInsertCard(QObject *parent = 0);
    void run();
    bool get_isinsert();
    QDateTime get_inserttime();
    int get_inserthowsec();

signals:
    void SigInsertCard(bool binsert);

public slots:

private:
    QDateTime m_inserttime;
    bool m_newinsert;
};

#endif // INSERTCARD_H
