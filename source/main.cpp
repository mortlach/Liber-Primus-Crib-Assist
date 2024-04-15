
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
#include "mainwindow.h"
#include "ngramdatahub.h"
#include "utilities.h"
#include "definitions.h"
#include <QApplication>
#include <QSettings>
#include <QHash>
#include <QString>
#include <QDir>
#include <QSettings>
#include <iostream>


int main(int argc, char *argv[])
{
    std::cout << "LPCribAssist Passed" << argc << " arguments:\n";
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << "\n";
    }
    QString cfpath;
    if(argc > 1){
        cfpath = QString(argv[1]);
    }
    else{
        cfpath = definitions::CONFIG_PATH;
    }
    std::cout << "CONFIG PATH:" << cfpath.toStdString() << std::endl;

    auto td = QDir(cfpath);
    qDebug() << td.absolutePath();

    QSettings settings(cfpath, QSettings::IniFormat);
    utilities::printSettings(settings);
    NgramDataHub ngramData = NgramDataHub(settings);

    QApplication a(argc, argv);
    qDebug() << ".exe path" << QCoreApplication::applicationDirPath();
    QApplication::setFont(utilities::getFont());
    MainWindow w(settings);
    w.show();
    //w.setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    w.raise();  // for MacOS
    w.activateWindow();
    return a.exec();
}























// READ RAW 1 -grams
//    QString path_to_raw_1grams = QString(settings.value(definitions::path_to_raw_1grams).toString());
//    QMap<int, RawWordData> raw_word_data;
//    QDir dir_1grams = QDir(path_to_raw_1grams);
//    int l = 1;
//    foreach (const QString &key, definitions::raw_1gram_keys){
//        QString fp = dir_1grams.absoluteFilePath(settings.value(key).toString());
//        qDebug() << fp;
//        QFile f = QFile(fp);
//        if(f.exists()){
//            raw_word_data.insert(l, RawWordData(l, fp));
//        }
//        else{
//            qDebug() << fp << "does not exist";
//        }
//        l++;
//    }
