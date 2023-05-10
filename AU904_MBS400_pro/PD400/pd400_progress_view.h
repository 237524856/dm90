#ifndef PD400_PROGRESS_VIEW_H
#define PD400_PROGRESS_VIEW_H

#include <QWidget>
#include <QProgressBar>
#include <QLabel>

class pd400_progress_view : public QWidget
{
    Q_OBJECT
public:
    explicit pd400_progress_view(QWidget *parent = nullptr);
    void createUI();
    QProgressBar *m_progress_view;

    QLabel *m_current_value;

signals:

};

#endif // PD400_PROGRESS_VIEW_H
