#ifndef COMMON_FUNTION_H
#define COMMON_FUNTION_H

#include <QWidget>

class common_funtion : public QWidget
{
    Q_OBJECT
public:
    explicit common_funtion(QWidget *parent = nullptr);
    //是否是第一次打开
    bool is_frist_open();

    //一张图片可以分割为多种状态
    QString add_image_forstatus(QString images,double image_width);

    //多个图片的选择框
    QString add_checkBox_forstatus(QString images,int image_width);

    //保存当前语言的状态
    bool set_save_language_status(int);

    //获取当前语言状态
    int get_save_language_status();

    //获取DM90的自定义预设数据
    QVector<QStringList> getDM90CustomPreDatas(QString file_path);
    //保存DM90的自定义预设数据
    bool saveDM90CustomPreDatas(const QString &file_path,QVector<QStringList> saveDatas);
    //获取DM90 列表的选中下标
    bool getDM90ListRowSelectStatus();


signals:

};

#endif // COMMON_FUNTION_H
