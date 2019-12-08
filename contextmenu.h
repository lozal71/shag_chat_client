#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H

#include <QtWidgets>
//#include <QtCore/QObject>
//#include <QtCore/qglobal.h>

class ContextMenu : public QPushButton
{
    Q_OBJECT
private:
    QMenu* m_pmnu;
protected:
    //virtual void contextMenuEvent(QContextMenuEvent *pe);
public:
    ContextMenu(QWidget* pwgt = nullptr);
    virtual void contextMenuEvent(QContextMenuEvent *pe);
public slots:
    void slotActivated(QPoint pos);
};

#endif // CONTEXTMENU_H
