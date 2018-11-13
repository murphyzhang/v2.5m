#ifndef SHOWTITLEWIDGET_H
#define SHOWTITLEWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include <QTextEdit>
#include <QScrollBar>
#include <QDebug>
#include "base/xlabel.h"
namespace Ui {
    class CShowTitleWidget;
}

class CShowTitleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CShowTitleWidget(QWidget *parent = 0);
    ~CShowTitleWidget();
    void set_text(QString title);
    void set_timer(int ms);
    void set_geometry(int x, int y, int w, int h);
    void set_alignment(Qt::Alignment ag);
    void setprompt();
    void showtext(QString text);
private slots:
    void updata_title();
    void scroll_prompt();
private:
    Ui::CShowTitleWidget *ui;
    QString m_title;
    QTimer *m_timer;
    XLabel *m_label;
    QTextEdit *m_promptshow;
    QTimer *m_srcoll;
    int m_currpos;
    QString m_textshow;
    int m_texttime;
    bool m_startsrcoll;
};

#endif // SHOWTITLEWIDGET_H
