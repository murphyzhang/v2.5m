#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QDialog>
#include <QLabel>
#include "base/xbutton.h"
#include "base/xlabel.h"
#include "common/configurate.h"

#define BT_CONFIRM 0
#define BT_CONFIRM_CANCEL 1

class CMessageBox : public QDialog
{
    Q_OBJECT

public:
    explicit CMessageBox(QWidget *parent = 0);
    ~CMessageBox();
    void SetType(int iType,QString qStrMessage);

public slots:
    void BtConfirmClicked();
    void BtCancelClicked();

signals:
    void SigConfirm();
    void SigCancel();

private:
    QLabel *m_pLbPrompt;
    XButton *m_pBtConfirm;
    XButton *m_pBtCancel;
};

#endif // MESSAGEBOX_H
