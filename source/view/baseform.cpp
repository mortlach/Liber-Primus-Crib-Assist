
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
#include <QMenu>
#include "baseform.h"

BaseForm::BaseForm(int chosen_col, QWidget *parent)
    : QWidget(parent)
    , my_sortfilterproxymodel(new FilterModel(chosen_col))
{
}

// void BaseForm::on_changeFontSize(int fs){
//     //tableView->changeFontSize(fs);
// }

void BaseForm::on_tableView_customContextMenuRequested(){
    //qDebug() << pt.x() << pt.y();
    //qDebug() << "showContextMenu passed" << pt.x() << pt.y();
    //QPoint globalPos = ui->tableView->mapToGlobal(pt);
    //qDebug() << globalPos.x() << globalPos.y();
    qDebug() << QCursor().pos().x() << QCursor().pos().y();
    QPoint pt2 = QPoint(QCursor().pos().x() - 100 , QCursor().pos().y()  );
    QMenu contextMenu(tr("NGRAM Filter Menu"), this);
    QAction* all = contextMenu.addAction("Filer: All");
    QAction* chosen = contextMenu.addAction("Filer: Chosen");
    QAction* not_chosen = contextMenu.addAction("Filer: Not Chosen");
    //QAction* reload = contextMenu.addAction("Reload Data");
    QAction* selectedItem = contextMenu.exec(pt2);
    if (selectedItem == all){ applyFilter(all_filter); }
    if (selectedItem == chosen){ applyFilter(chosen_filter); }
    if (selectedItem == not_chosen){ applyFilter(not_chosen_filter); }
    //if (selectedItem == reload){ applyFilter(3); }
}
void BaseForm::applyFilter(int a){
    //    check filters for phrase unqiue words
    qDebug() << "ngramApplyFilter" << a;
    switch(a){
    case Constants::not_chosen_filter: my_sortfilterproxymodel->setFilter("not_chosen"); break;
    case Constants::chosen_filter:     my_sortfilterproxymodel->setFilter("chosen");     break;
    case Constants::all_filter:        my_sortfilterproxymodel->setFilter("");           break;
    default:;
    }
}

//void BaseForm::on_tableView_space_bar_sig(){
    // QModelIndexList to_toggle;
    // QModelIndex new_selection;
    // bool add_row = true;
    // int new_selected_row = 0;
    // for(const QModelIndex& index: ui->tableView->selectionModel()->selectedRows()){
    //     //qDebug() << "row/col" << index.row() << "/" << index.column();
    //     to_toggle.push_back(my_sortfilterproxymodel->mapToSource(index));
    //     if(add_row){
    //         new_selection = index;
    //         qDebug() << new_selection.row();
    //         if(new_selection.row() > 0){
    //             new_selected_row = new_selection.row()-1;
    //         }
    //         add_row = false;
    //     }
    // }
    // if(my_sortfilterproxymodel->filter_mode == QString("all") ){
    // }
    // else{
    //     ui->tableView->selectRow(new_selected_row);
    // }
    // model.toggleChosen(to_toggle);
    // update();
//}


//void BaseForm::on_tableView_left_doubleclick_sig(){
    // QModelIndexList to_toggle;
    // for(const QModelIndex& index: ui->tableView->selectionModel()->selectedRows()){
    //     qDebug() << "row/col" << index.row() << "/" << index.column();
    //     to_toggle.push_back(my_sortfilterproxymodel->mapToSource(index));
    // }
    // model.toggleChosen(to_toggle);
    // update();
//}
