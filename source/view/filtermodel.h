
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
#ifndef FILTERMODEL_H
#define FILTERMODEL_H
#include <QObject>
#include <QString>
#include <QSortFilterProxyModel>


class FilterModel : public QSortFilterProxyModel
{
public:
    FilterModel(int chosen_col, QObject* parent = nullptr);
    ~FilterModel(){};

public:
    bool filterAcceptsRow(int source_row,const QModelIndex &source_parent) const;
    inline void setFilter(const QString &s){
        filter_mode.clear();
        filter_mode = s;
        invalidateFilter();
    }
    QString filter_mode; // must match NgramsModel
    int chosen_col;

private:
};

#endif // FILTERMODEL_H
