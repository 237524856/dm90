#ifndef MAINTITLEBAR_H
#define MAINTITLEBAR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include "custom_button.h"
#include "custom_checkbox.h"
enum MainButtonType
{
    M_MIN_BUTTON = 0,         // 最小化和关闭按钮;
    M_MIN_MAX_BUTTON ,        // 最小化、最大化和关闭按钮;
    M_ONLY_CLOSE_BUTTON       // 只有关闭按钮;
};

class mainTitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit mainTitleBar(QWidget *parent = nullptr);

    // 设置标题栏背景色及是否设置标题栏背景色透明;
    void setBackgroundColor(int r, int g, int b , bool isTransparent = false);
    // 设置标题栏图标;
    void setTitleIcon(QString filePath , QSize IconSize = QSize(25 , 25));
    // 设置标题内容;
    void setTitleContent(QString titleContent , int titleFontSize = 9);
    // 设置标题栏长度;
    void setTitleWidth(int width);
    // 设置标题栏上按钮类型;
    void setButtonType(MainButtonType buttonType);
    // 设置标题栏中的标题是否会滚动;具体可以看效果;
    void setTitleRoll();
    // 设置窗口边框宽度;
    void setWindowBorderWidth(int borderWidth);

    // 保存/获取 最大化前窗口的位置及大小;
    void saveRestoreInfo(const QPoint point, const QSize size);
    void getRestoreInfo(QPoint& point, QSize& size);

private:
    void paintEvent(QPaintEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    // 初始化控件;
    void initControl();
    // 信号槽的绑定;
    void initConnections();
    // 加载样式文件;
    void loadStyleSheet(const QString &sheetName);

signals:
    // 按钮触发的信号;
    void signalButtonMinClicked();
    void signalButtonRestoreClicked();
    void signalButtonMaxClicked();
    void signalButtonCloseClicked();

private slots:
    // 按钮触发的槽;
    void onButtonMinClicked();
    void onButtonRestoreClicked();
    void onButtonMaxClicked();
    void onButtonCloseClicked();
    void onRollTitle();

public:
    QLabel *m_imageLogo;

private:
    QLabel* m_pIcon;                    // 标题栏图标;
    QLabel* m_pTitleContent;            // 标题栏内容;
    custom_button* m_pButtonMin;          // 最小化按钮;
    custom_button* m_pButtonRestore;      // 最大化还原按钮;
    custom_button* m_pButtonMax;          // 最大化按钮;
    custom_button* m_pButtonClose;        // 关闭按钮;

    // 标题栏背景色;
    int m_colorR;
    int m_colorG;
    int m_colorB;

    // 最大化，最小化变量;
    QPoint m_restorePos;
    QSize m_restoreSize;
    // 移动窗口的变量;
    bool m_isPressed;
    QPoint m_startMovePos;
    // 标题栏跑马灯效果时钟;
    QTimer m_titleRollTimer;
    // 标题栏内容;
    QString m_titleContent;
    // 按钮类型;
    MainButtonType m_buttonType;
    // 窗口边框宽度;
    int m_windowBorderWidth;
    // 标题栏是否透明;
    bool m_isTransparent;



};

#endif // MAINTITLEBAR_H
