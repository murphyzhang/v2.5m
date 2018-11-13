#ifndef SHUTDOWN_H
#define SHUTDOWN_H


#include <QPushButton>

#include "widget/promptbox.h"


class CShutdown: public QPushButton
{
    Q_OBJECT

public:
    CShutdown(QWidget *parent=0);
    ~CShutdown();

private:
    void paintEvent(QPaintEvent *e);

private slots:
    void onClick(bool checked = false);

    void slotpromptok(int ser);
    void slotpromptcs(int ser);

private:
    XPromptBox *m_promptbox;
    QWidget * m_parent;

    QPixmap m_pixmap;
};

#endif // SHUTDOWN_H
