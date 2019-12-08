#include "contextmenu.h"

void ContextMenu::contextMenuEvent(QContextMenuEvent *pe)
{
    m_pmnu->exec(pe->globalPos());
}


ContextMenu::ContextMenu(QWidget *pwgt) : QPushButton(pwgt)
{
    //setReadOnly(true);
    m_pmnu = new QMenu();
    m_pmnu->addAction("red");
    m_pmnu->addAction("green");
    m_pmnu->addAction("blue");
//    connect(m_pmnu, SIGNAL(triggered(QAction*)),
//                    SLOT(slotActivated(QAction*))
//            );

}

void ContextMenu::slotActivated(QPoint pos)
{
     m_pmnu->popup(this ->mapToGlobal(pos));
//    QString strColor = pAction->text().remove("&");
//    setHtml(QString("<BODY BGCOLOR = %1></BODY>").arg(strColor));
    qDebug() << "slotActivated";

}
