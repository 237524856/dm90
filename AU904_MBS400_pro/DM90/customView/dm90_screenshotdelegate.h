

#pragma once
#include <QStyledItemDelegate>

#ifndef DM90_SCREENSHOTDELEGATE_H
#define DM90_SCREENSHOTDELEGATE_H

#include <QObject>

class dm90_screenShotDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit dm90_screenShotDelegate(QObject *parent = nullptr);

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;


signals:

};

#endif // DM90_SCREENSHOTDELEGATE_H
