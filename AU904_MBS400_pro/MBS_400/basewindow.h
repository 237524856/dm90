#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QMainWindow>
#include "mytitlebar.h"

class basewindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit basewindow(QWidget *parent = nullptr);
    ~basewindow();

    //virtual bool event(QEvent *event);

private:
    void initTitleBar();
    void paintEvent(QPaintEvent *event);
    void loadStyleSheet(const QString &sheetName);




private slots:
    void onButtonMinClicked();
    void onButtonRestoreClicked();
    void onButtonMaxClicked();
    void onButtonCloseClicked();

protected:
    MyTitleBar* m_titleBar;

signals:

};

#endif // BASEWINDOW_H
