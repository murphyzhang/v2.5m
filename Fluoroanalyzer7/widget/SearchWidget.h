#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QDialog>
#include <QString>
#include <QLabel>
#include "base/MyLineEdit.h"
#include "base/xbutton.h"
#include "base/xinputcontext.h"
#include "widget/syszuxim.h"
#include "widget/syszuxpinyin.h"
#include "common/configurate.h"

#define DATE_SEARCH 0
#define ID_SEARCH 1
#define NAME_SEARCH 2

class CSearchWidget : public QDialog
{
    Q_OBJECT

public:
    explicit CSearchWidget(QWidget *parent = 0);
    ~CSearchWidget();

    void InitUI();
    void ClearContent();

signals:
    void SendSearchText(int iMode,QString qStrText);
    void EndInput(QString qStrKey,int iIndex);
    void sigCancel();

public slots:
    void LineEditClicked(QString qStrText);
    void BtQueryClicked();
    void BtCancelClicked();
    void slotTextActer(QString text);
    void ConfirmString(QString qStrText);   //接收键盘发过来的数据

private:
    QComboBox *m_pCbbKey;
    CMyLineEdit *m_pLineEdit;
    XButton *m_pBtQuery;
    XButton *m_pBtCancel;
    XInPutWidget *m_pInputPanel;
    SyszuxPinyin *m_pSyszux;
};

#endif // SEARCHWIDGET_H
