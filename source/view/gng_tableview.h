
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
#ifndef GNG_TABLEVIEW_H
#define GNG_TABLEVIEW_H
#include <QWidget>
#include <QTableView>
#include <QKeyEvent>
#include <QMouseEvent>

class GNGTableView : public QTableView
{
    Q_OBJECT
public:
    GNGTableView(QWidget *parent = nullptr);

    void allWidgetsEnabled(bool value);

public: signals:
    void space_bar_sig();
    void alt_f_sig();
    void f_sig();
    void t_sig();
    void alt_a_sig();
    void alt_c_sig();
    void alt_n_sig();
    void ctrl_c_sig();
    void delete_sig();
    void left_doubleclick_sig();


public slots:
    void changeFontSize(int fs);


protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent( QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

    int count;
};

#endif // GNG_TABLEVIEW_H
