
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
#ifndef SPOILER_H
#define SPOILER_H

#include <QFrame>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>

//namespace ui
//{
class Spoiler : public QWidget
{
    Q_OBJECT

private:
    QGridLayout* mainLayout;
    QToolButton* toggleButton;
    QFrame* headerLine;
    QParallelAnimationGroup* toggleAnimation;
    QScrollArea* contentArea;
    int animationDuration;
    int collapsedHeight;
    int collapsedWidth;
    bool isExpanded = false;

public slots:
    void toggle(bool collapsed);

public:
    static const int DEFAULT_DURATION = 0;

    explicit Spoiler(QWidget* parent = 0);
    explicit Spoiler(const QString& title = "", const int animationDuration = DEFAULT_DURATION, QWidget* parent = 0);

    // set layout of content
    void setContentLayout(QLayout& contentLayout);

    // set title
    void setTitle(QString title);

    // update animations and their heights
    void updateHeights();
    void updateWidths();
};
//}

#endif // SPOILER_H
