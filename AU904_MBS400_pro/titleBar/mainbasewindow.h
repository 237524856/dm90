#ifndef MAINBASEWINDOW_H
#define MAINBASEWINDOW_H

#include <QMainWindow>
#include "maintitlebar.h"

class mainBasewindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit mainBasewindow(QWidget *parent = nullptr);

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
    mainTitleBar* m_titleBar;
signals:

};

#endif // MAINBASEWINDOW_H
