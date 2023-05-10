#include "dm90_screenshotdelegate.h"

dm90_screenShotDelegate::dm90_screenShotDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}
void dm90_screenShotDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // option.rect属性中保存了Item的位置
    editor->setGeometry(option.rect);
}
