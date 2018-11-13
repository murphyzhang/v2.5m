#ifndef SELFCHECKWIDGET_H
#define SELFCHECKWIDGET_H
#include <QTimer>
#include <QMovie>
#include "basefunwidget.h"
#include "common/commoninterface.h"
#define CHECK_NUM 5


typedef enum
{
    NOT_CHECK,
    WAT_CHECK,
    ING_CHECK,
    END_CHECK
}CHECKTYPE;
typedef struct
{
    QString m_namestr;
    QString m_infostr;
    QLabel *m_checkname;
    QLabel *m_checkicon;
    QLabel *m_checkinfo;
    CHECKTYPE m_checked;
    bool m_checkok;
}CHECKPRO;

class CSelfCheckWidget : public CBaseFunWidget
{
    Q_OBJECT

public:
    explicit CSelfCheckWidget(QWidget *parent = 0);
    ~CSelfCheckWidget();
    void showEvent(QShowEvent *);
//    void hideEvent(QHideEvent *);

private slots:
    void slotcheckfun();

private:
    QPixmap m_bgPixmap;
    QLabel *m_title;
    CHECKPRO m_checkpro[CHECK_NUM];
    QStringList m_strlist;
    QStringList m_strinfo;
    QTimer *m_timer;
    QMovie *movie;
    bool m_candidi;

};

#endif // SELFCHECKWIDGET_H
