
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
#include "raw1grammodel.h"
#include <QColor>

Raw1GramModel::Raw1GramModel(QObject *parent)
    : QAbstractTableModel(parent)
    , raw_data(nullptr)
    , editrole_list(QList<int>{Qt::EditRole})
{
}
int Raw1GramModel::rowCount(const QModelIndex&) const{
    if(raw_data){
        return raw_data->size();
    }
    return 0;
}
void Raw1GramModel::clear(){
    beginResetModel();
    words = nullptr;
    //chosen = nullptr;
    counts = nullptr;
    gematria_sum = nullptr;
    runes = nullptr;
    raw_data = nullptr;
    endResetModel();
}
QVariant Raw1GramModel::data(const QModelIndex& index, int role)const{
    if (!index.isValid()) {
        return QVariant();
    }
    if(role == Qt::DisplayRole){
        if(index.column() == word_col){
            return QVariant((*words)[index.row()]).toString();
        }
        if(index.column() == counts_col){
            return QVariant::fromValue((*counts)[index.row()]).toLongLong();
        }
        if(index.column() == gematria_sum_col){
            return QVariant::fromValue((*gematria_sum)[index.row()]).toInt();
        }
        if(index.column() == runes_col){
            return QVariant((*runes)[index.row()]).toString();
        }
        if(index.column() == chosen_col){
            return QVariant::fromValue(
                       getChosen(index.row())
                       ).toBool();
        }

    }
    if(role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }
    if(role == Qt::ForegroundRole ) {
        if( (index.column() == chosen_col) && isChosen(index) ){
            return QVariant(QColor::fromRgb(255,0,0));
        }
        return QVariant(QColor::fromRgb(0,0,0));
    }
    return QVariant();
}
QVariant Raw1GramModel::headerData(int section, Qt::Orientation orientation, int role)const{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch(section ){
        case chosen_col: return QVariant(QString("chosen"));
        case word_col: return QVariant(QString("word"));
        case counts_col: return QVariant(QString("count"));
        case gematria_sum_col: return QVariant(QString("sum"));
        case runes_col: return QVariant(QString("runes"));
        }
    }
    return QVariant();
}

void Raw1GramModel::setNewData(RawWordData& raw_data_in){
    beginResetModel();
    words = &raw_data_in.words;
    counts = &raw_data_in.counts;
    runes = &raw_data_in.runes;
    gematria_sum = &raw_data_in.gematria_sum;
    raw_data = &raw_data_in;
    endResetModel();
}

bool Raw1GramModel::getChosen(int row)const{
    return raw_data->getChosen(row);
}
bool Raw1GramModel::isChosen(const QModelIndex& index)const{
    return raw_data->isChosen(index.row());
}

void Raw1GramModel::setChosen(const QModelIndexList&rows){
    for(const QModelIndex &in: rows){
        raw_data->setChosen(in.row(), true);
        emit dataChanged(index(in.row(), chosen_col),index(in.row(), chosen_col));
    }
}
void Raw1GramModel::setNotChosen(const QModelIndexList&rows){
    for(const QModelIndex &in: rows){
        raw_data->setChosen(in.row(), false);
        emit dataChanged(index(in.row(), chosen_col),index(in.row(), chosen_col));
    }
}
void Raw1GramModel::toggleChosen(const QModelIndexList&rows){
    for(const QModelIndex &in: rows){
        raw_data->toggleChosen(in.row());
        emit dataChanged(index(in.row(), chosen_col),index(in.row(), chosen_col));
    }
}
int Raw1GramModel::deleteSelected(const QModelIndexList&rows){
    beginResetModel();
    qDebug() << "Raw1GramModel::deleteSelected";
    //do all in one go
    QList<int> to_delete;
    for(const QModelIndex &in: rows){
        to_delete.append(in.row());
    }
    int new_selection = raw_data->deleteItems(to_delete);
    //emit dataChanged(index(in.row(), chosen_col),index(in.row(), chosen_col));
    endResetModel();
    return new_selection;
}

Qt::ItemFlags Raw1GramModel::flags(const QModelIndex&){
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled  | Qt::ItemIsEditable;
}
