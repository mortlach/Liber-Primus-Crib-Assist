
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
#ifndef BASEFORM_H
#define BASEFORM_H

#include <QObject>
#include <QWidget>
#include "filtermodel.h"

class BaseForm : public QWidget
{
    Q_OBJECT
public:
    BaseForm(int chosen_col, QWidget *parent = nullptr);
    FilterModel* my_sortfilterproxymodel;

public slots:
    void on_tableView_customContextMenuRequested();
    // TODO keep adding
    //void on_tableView_left_doubleclick_sig();
    //void on_tableView_space_bar_sig();

private:
    void applyFilter(int a);

    enum Constants
    {
        all_filter = 0,
        chosen_filter = 1,
        not_chosen_filter = 2
    };
};

#endif // BASEFORM_H
