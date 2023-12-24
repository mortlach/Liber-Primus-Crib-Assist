
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
#include "filtermodel.h"


FilterModel::FilterModel(int chosen_col, QObject* parent)
    : QSortFilterProxyModel(parent)
    , filter_mode(QString("all"))
    , chosen_col(chosen_col)
{
    dynamicSortFilter();
    setFilterKeyColumn(0);
}
bool FilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{
    if( filter_mode == "all")
        return true;
    if( filter_mode == QString("chosen")){
        bool c = sourceModel()->data(sourceModel()->index(source_row, chosen_col, source_parent)).toBool();
        if(c){
            return true;
        }
        return false;
    }
    if( filter_mode == QString("not_chosen")){
        bool c = sourceModel()->data(sourceModel()->index(source_row, chosen_col, source_parent)).toBool();
        if(!c)
            return true;
        return false;
    }
    return true;
}
