
/////////////////////////////////////////////////////////////////////////////
//    LPCribAssist v0.1: to help with cribbing the Liber Primus
//    mortlach
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////
#include "gng_tableview.h"
#include "utilities.h"
#include <QObject>
#include <QHeaderView>
#include <QApplication>
#include <QClipboard>

GNGTableView::GNGTableView(QWidget* parent) :
    QTableView(parent) ,
    count(0)
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setFocusPolicy(Qt::WheelFocus);
    setSortingEnabled(true);
    setAlternatingRowColors(true);
    horizontalHeader()->setVisible(true);
    verticalHeader()->setVisible(true);
    horizontalHeader()->setSectionsClickable(true);
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    verticalHeader()->setSectionsClickable(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
}
void GNGTableView::allWidgetsEnabled(bool value){
    if(value){
        setEditTriggers(QAbstractItemView::AllEditTriggers);
    }
    else{
        setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}
bool GNGTableView::eventFilter(QObject *obj, QEvent *event)
{   // see  OneGramTableView::eventFilter
    return QTableView::eventFilter(obj, event);
}

void GNGTableView::changeFontSize(int fs){
    setFont(utilities::getFont(fs));
}


void GNGTableView::keyPressEvent(QKeyEvent *event)
{
    if( (event->key() == Qt::Key_Space)
            && !event->isAutoRepeat() ){
        emit space_bar_sig();
    }
    else if( (event->key() == Qt::Key_F)
             && (int(event->modifiers()) == Qt::AltModifier) ){
        emit alt_f_sig();
    }
    else if(event->matches(QKeySequence::Copy)){
            QString text;
            QItemSelectionRange range = selectionModel()->selection().first();
            for (auto i = range.top(); i <= range.bottom(); ++i)
            {
                QStringList rowContents;
                for (auto j = range.left(); j <= range.right(); ++j)
                    rowContents << model()->index(i,j).data().toString();
                text += rowContents.join("\t");
                text += "\n";
            }
            QApplication::clipboard()->setText(text);
    }
    else if( (event->key() == Qt::Key_F)  ){emit f_sig();}
    else if( (event->key() == Qt::Key_T)  ){emit t_sig();}
    else if( (event->key() == Qt::Key_A)  && (int(event->modifiers()) == Qt::AltModifier) ){emit alt_a_sig();}
    else if( (event->key() == Qt::Key_C)  && (int(event->modifiers()) == Qt::AltModifier) ){emit alt_c_sig();}
    else if( (event->key() == Qt::Key_N)  && (int(event->modifiers()) == Qt::AltModifier) ){emit alt_n_sig();}
    else if( (event->key() == Qt::Key_C)
             && (int(event->modifiers()) == Qt::ControlModifier) ){emit ctrl_c_sig();}
    else{
        qDebug() << "other caught";
        QTableView::keyPressEvent(event);
    }
}
void GNGTableView::mouseDoubleClickEvent( QMouseEvent * e )
{
    if ( e->button() == Qt::LeftButton ){
        qDebug() << "GNGTableView left double click caught";
        emit left_doubleclick_sig();
    }
}
