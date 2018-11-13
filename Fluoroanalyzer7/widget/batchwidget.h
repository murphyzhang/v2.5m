#ifndef BATCHWIDGET_H
#define BATCHWIDGET_H

#include "basefunwidget.h"
#include "common/commoninterface.h"
#include "promptbox.h"


class CBatchWidget : public CBaseFunWidget
{
    Q_OBJECT
public:
    explicit CBatchWidget(QWidget *parent = 0);
    void showEvent(QShowEvent *);
    void updatarecord(QString table,int first,bool up_down);
    void removebatch(int row);
signals:

public slots:
    void slotclickpageup();
    void slotclickpagedown();
    void slotclicksearch();
    void slotclickdel1();
    void slotclickdeln();
    void slotclickback();

    void slotrecvnewbatch(QString batch);

    void slotclickOK();

    void slotpromptok(int ser);
    void slotpromptcs(int ser);
private:

    XLabel *m_spacename;
    XLabel *m_spacevaule;

    QTableWidget *m_batchrecord;

    XButton *m_pageup;
    XButton *m_pagedown;
    XButton *m_search;
    XButton *m_del1;
    XButton *m_deln;
    XButton *m_back;

    XLabel *m_searchprompt;
    XLabel *m_searchlabel;
    XLineEdit *m_searchedit;
    XButton *m_ok;

    XPromptBox *m_promptbox;
};

#endif // BATCHWIDGET_H
