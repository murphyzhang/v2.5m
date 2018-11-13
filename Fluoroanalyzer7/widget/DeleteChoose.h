#ifndef DELETECHOOSE_H
#define DELETECHOOSE_H

#include <QWidget>
#include "base/xbutton.h"
#include "common/configurate.h"

#define BTDELCURRENT 0
#define BTDELALL     1
#define BTRETURN     2

class CDeleteChoose : public QWidget
{
    Q_OBJECT

public:
    explicit CDeleteChoose(QWidget *parent = 0);
    ~CDeleteChoose();

signals:
    void DelSelect(int iSelect);

public slots:
    void BtDelCurrentClicked();
    void BtDelAllClicked();
    void BtReturnClicked();

private:
    XButton *m_pBtDelCurrent;
    XButton *m_pBtDelAll;
    XButton *m_pBtReturn;
};

#endif // DELETECHOOSE_H
