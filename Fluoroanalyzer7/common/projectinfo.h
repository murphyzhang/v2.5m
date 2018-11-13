/*********************************************************************/
//
//
//
//
//
//
/*********************************************************************/

#ifndef PROJECTINFO_H
#define PROJECTINFO_H

#include <QObject>

class CProjectInfo : public QObject
{
    Q_OBJECT
public:
    explicit CProjectInfo(QObject *parent = 0);

signals:

public slots:

private:
    QString m_proname;      //项目名称
    QString m_prounit;      //项目单位
    int m_observedmin;      //最小测量值
    int m_observedmax;      //最大测量值
    int m_reactortime;      //反应时间
    QString m_prorefer;     //项目参考值
};

#endif // PROJECTINFO_H
