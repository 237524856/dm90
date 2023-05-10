#ifndef MAINSUBVIEW_H
#define MAINSUBVIEW_H

#include <QWidget>
#include <QMouseEvent>

class mainSubView : public QWidget
{
    Q_OBJECT
public:
    explicit mainSubView(QWidget *parent = nullptr);

signals:


private:

    // 鼠标点击事件;
    void mousePressEvent(QMouseEvent* event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

private:


    int is_next = 0;
    int m_press_x=0;
};

#endif // MAINSUBVIEW_H
