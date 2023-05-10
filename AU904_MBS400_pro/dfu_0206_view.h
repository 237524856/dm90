#ifndef DFU_0206_VIEW_H
#define DFU_0206_VIEW_H

#include <QWidget>

#include <QPushButton>
#include <QProcess>
#include <QMessageBox>
#include <QDir>

class dfu_0206_view : public QWidget
{
    Q_OBJECT
public:
    explicit dfu_0206_view(QWidget *parent = nullptr);

    QProcess *myProcess;

    int m_total_lengh = 0;

    int m_pid;

    void startNewExe(QString file_name);
public slots:

    void readResult(int);
    void onReadData();


signals:

    void dsp_progress_value(int);
    void dsp_finish_progress(int);

};

#endif // DFU_0206_VIEW_H
