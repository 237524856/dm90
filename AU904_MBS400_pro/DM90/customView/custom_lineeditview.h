#ifndef CUSTOM_LINEEDITVIEW_H
#define CUSTOM_LINEEDITVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QMouseEvent>

class custom_lineEditView : public QLineEdit
{
    Q_OBJECT
public:
    explicit custom_lineEditView(QWidget *parent = nullptr);

    void focusInEvent(QFocusEvent *) override;
    void focusOutEvent(QFocusEvent *) override;

    bool eventFilter(QObject *watched, QEvent *event) override;


protected:

    int m_press_x;

signals:

    void lineViewBlockText(QString);

};

#endif // CUSTOM_LINEEDITVIEW_H
