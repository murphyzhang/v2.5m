#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H

#include <QtGui>
#include <QtCore>
#include <QWidget>
#include <QInputContext>

namespace Ui {
    class XInPutWidget;
}

class XInPutWidget : public QWidget
{
    Q_OBJECT

public:
    explicit XInPutWidget(QWidget *parent = 0);
    ~XInPutWidget();
    void xsetFocus(QWidget *newFocus);

signals:
    void sigPBClicked(QString text);

protected:
    bool event(QEvent *e);

private slots:
    void saveFocusWidget(QWidget *oldFocus, QWidget *newFocus);
    void buttonClicked(QString text);

private:
    Ui::XInPutWidget *form;
    QWidget *lastFocusedWidget;
    QSignalMapper signalMapper;
    QList<QPushButton*> m_pblist;

};

#endif // INPUTWIDGET_H
