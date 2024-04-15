
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
#include "definitions.h"
#include "raw1grams.h"
#include <QString>
#include <QMap>
#include <QDir>
#include <QSettings>

QSettings* Raw1Grams::settings = nullptr;
RawWordData* Raw1Grams::dummy_model_data = new RawWordData(0, QString("")); // to handle bad requets
QList<RawWordData*> Raw1Grams::raw_word_data = QList<RawWordData*>();
QList<RawWordData*> Raw1Grams::phrase_word_data = QList<RawWordData*>();
bool Raw1Grams::loaded_phrase_local1gram_files  = false;
bool Raw1Grams::loaded_raw1gram_files  = false;
bool Raw1Grams::loaded_local1gram_files = false;
unsigned int Raw1Grams::onegram_file_count = 14;

Raw1Grams::Raw1Grams()
{}
Raw1Grams::~Raw1Grams(){
     qDebug() << "destroy Raw1Grams";
//    qDeleteAll(phrase_word_data.begin(), phrase_word_data.end());
//    phrase_word_data.clear();
//    qDeleteAll(raw_word_data.begin(), raw_word_data.end());
//    raw_word_data.clear();
}

Raw1Grams::Raw1Grams(QSettings& settings_in){
    applySettingsAndLoad(settings_in);
}

bool Raw1Grams::applySettingsAndLoad(QSettings& settings_in){
    settings = &settings_in;
    return loadRaw1grams();
}

bool Raw1Grams::loadRaw1grams(){
    QString path_to_raw_1grams = QString(settings->value(definitions::PATH_TO_RAW_1GRAMS).toString());
    QDir dir_1grams = QDir(path_to_raw_1grams);
    qDebug() << "loadRaw1grams:  1grams data directory =" << dir_1grams.absolutePath();
    int l = 1;
    for(const auto& key : definitions::raw_1gram_keys){
        QString fp = dir_1grams.absoluteFilePath(settings->value(key).toString());
        QFile f = QFile(fp);
        if(f.exists()){
            raw_word_data.push_back(new RawWordData(l, fp));
        }
        else{
            qDebug() << fp << "does not exist";
        }
        l++;
    }
    return true;
}

bool Raw1Grams::saveRaw1grams()const{
    int fail_count = 0;
    for(auto& item: raw_word_data){
        if(!item->saveData()){
            fail_count+=1;
        }
    }
    if(fail_count >0){
        return false;
    }
    return true;

}
