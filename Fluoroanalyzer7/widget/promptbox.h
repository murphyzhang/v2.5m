#ifndef PROMPTBOX_H
#define PROMPTBOX_H

#include <QWidget>
#include <QTimer>
#include <base/xlabel.h>
#include <base/xbutton.h>

typedef enum
{
    XBT_NUL = 0x00,
    XBT_YES = 0x01,
    XBT_CMS = 0x02
}XBUTTON_EM;

class XPromptBox : public QWidget
{
    Q_OBJECT
public:
    explicit XPromptBox(QWidget *parent = 0);
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);
    void setnsec(int nsec);
    void settitle(QString title);
    void setwhichbutton(int button);
    void setpromptser(int ser);
    void setnobutton();
    void showprompt(QString title,int time ,int button,XBUTTON_EM def);
	void setOkText(QString okText);
signals:
    void sigclickcancel(int ser);
    void sigclickok(int ser);
public slots:
    void slot1sec();
    void slotclickcancel();
    void slotclickok();
private:
    int m_promptser;
    XLabel *m_showtext;
    XButton *m_cancel;
    XButton *m_ok;
    int m_whichone; //设置默认哪个按键倒计时
    QTimer *m_1sec;
    int m_runnsec;
    QString m_title;

};

#endif // PROMPTBOX_H
