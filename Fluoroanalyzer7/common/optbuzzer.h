#ifndef OPTBUZZER_H
#define OPTBUZZER_H

#include <QThread>
#include "xioctl.h"

class XOptBuzzer : public QThread
{
    Q_OBJECT
public:
    explicit XOptBuzzer(QObject *parent = 0);
    void run();
    void start_ring(int times,int ringtime,int quiettime);
    void stop_ring();
    void ringdiii();
    void ringdi();
    void ringdidi();
    void ringdidididi();

signals:

public slots:

private:
    int m_times;
    int m_ring;
    int m_quiet;

};

#endif // OPTBUZZER_H
