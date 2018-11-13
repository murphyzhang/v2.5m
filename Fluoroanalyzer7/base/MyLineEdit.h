#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QObject>
#include <QWidget>
#include <QStringListModel>
#include <QPushButton>
#include <QLineEdit>
#include <QApplication>
#include <QDesktopWidget>
#include <QCompleter>
#include <QDebug>
#include <QMouseEvent>

class CMyLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit CMyLineEdit(QWidget *parent = 0);
    void AddValue(const QString &value);
    void SetMyGeometry(int iPosX, int iPosY, int iWidth, int iHeight);
    void SetMyTextMargins(int iLeft,int iTop,int iRight,int iBottom);

protected:
    virtual void mousePressEvent(QMouseEvent *event);

signals:
    void SendText(QString qStrText);

private slots:
    void EditComplete();
    void BtClicked();

private:
    QStringList valueList;
    QStringListModel *m_pListModel;
    QPushButton *m_pPushButton;
};

#endif // MYLINEEDIT_H


