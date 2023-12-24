
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
#include "utilities.h"
#include "rawworddata.h"
#include <QFile>

RawWordData::RawWordData(unsigned int word_length, const QString& full_path_in)
    : counts(QVector<unsigned long long>())
    //, chosen(QVector<bool>())
    , chosen_count(0)
    , not_chosen_count(0)
    , words(QVector<QString>())
    , chosen_words(QVector<QString>())
    , not_chosen_words(QVector<QString>())
    , runes(QVector<QString>())
    , gematria_sum(QVector<unsigned int>())
    //, word_to_index(QHash<QString, int>())
    , any_data(std::vector<std::vector<std::any>>())
    , string_data(std::vector<std::vector<std::string>>())
    , word_length(word_length)
    , has_data(false)
    , data_file_path(full_path_in)
{
    if(data_file_path != ""){
        if(!loadData()){
            qDebug() << "RawWordData::RawWordData ERROR loading data from" << data_file_path;
        }
    }
}
bool RawWordData::loadData(){
    QFile file = QFile(data_file_path);
    if(file.exists()){
        any_data.clear();
        string_data.clear();
        if(utilities::readCSVFile(data_file_path.toStdString(), string_data)){
            any_data.clear();
            any_data.reserve(string_data.size());
            for(const auto& item : string_data){
                std::vector<std::any> next_entry;
                next_entry.reserve(5);
                next_entry.push_back( std::any( std::any_cast<std::string>(item.at(0))));
                next_entry.push_back( std::any(std::stoull(std::any_cast<std::string>(item.at(1))))) ;
                next_entry.push_back( std::any((bool)std::stoi(item.at(2))));
                next_entry.push_back( std::any(item.at(3)));
                next_entry.push_back( std::any( (unsigned int)std::stoul(item.at(4))));
                any_data.push_back(next_entry);
            }
            has_data = setFromAnyData(any_data);
        }
    }
    return has_data;
}

bool RawWordData::setFromAnyData(const std::vector<std::vector<std::any>>& raw_data){
    words.clear();
    counts.clear();
    chosen.clear();
    runes.clear();
    runes.clear();
    gematria_sum.clear();
    words.reserve((qsizetype)raw_data.size());
    chosen.reserve((qsizetype)raw_data.size());
    chosen.reserve((qsizetype)raw_data.size());
    runes.reserve((qsizetype)raw_data.size());
    gematria_sum.reserve((qsizetype)raw_data.size());
    for(const auto& item : raw_data){
        words.append(QString::fromStdString(std::any_cast<std::string>(item.at(0))));
        counts.append(std::any_cast<unsigned long long>(item.at(1)));
        chosen.append(std::any_cast<bool>(item.at(2)));
        runes.append(QString::fromStdString(std::any_cast<std::string>(item.at(3))));
        gematria_sum.append(std::any_cast<unsigned int>(item.at(4)));
    }
    // ALWAYS call this after items added / deleted
    //finishAFterDataAddedOrDeleted();
    updateWordToIndex();
    // simple test if any data was loaded
    if(words.size() >0){
        return true;
    }
    return false;
}

bool RawWordData::getAsAnyData(std::vector<std::vector<std::any>>& data_to_fill)const{
    data_to_fill.clear();
    data_to_fill.reserve(words.size());
    for(unsigned int i = 0; i < words.size(); ++i){ // !
        std::vector<std::any> next_line;
        next_line.reserve(5);
        next_line.push_back( std::any(words.at(i).toStdString())  );
        next_line.push_back( std::any(counts.at(i)) );
        next_line.push_back( std::any(chosen.at(i)) );
        next_line.push_back( std::any(runes.at(i).toStdString()) );
        next_line.push_back( std::any(gematria_sum.at(i)) );
        data_to_fill.push_back(next_line);
    }
    return true;
}

bool RawWordData::saveData()const{
    std::vector<std::vector<std::any>> data_to_write;
    if( getAsAnyData(data_to_write)){
        qDebug() << "RawWordData:saveRawWordData," << data_file_path;
        std::vector<std::vector<std::string>> string_data;
        for(const auto& item: data_to_write){
            if(item.size() != 5){// !
                qDebug() << "RawWordData::saveRawWordData ERROR not enough columns in data";
                return false;
            }
            std::vector<std::string> next_line;// !
            next_line.reserve(5);
            next_line.push_back(std::any_cast<std::string>(item.at(0)));
            next_line.push_back(std::to_string(std::any_cast<unsigned long long>(item.at(1))));
            next_line.push_back(std::to_string(std::any_cast<bool>(item.at(2))));
            next_line.push_back(std::any_cast<std::string>(item.at(3)));
            next_line.push_back(std::to_string(std::any_cast<unsigned int>(item.at(4))));
            string_data.push_back(next_line);
        }
        return utilities::writeCSVFile(data_file_path.toStdString(), string_data);
    }
    return false;
};



bool RawWordData::deleteNotChosen2(){
    // faster than delete from orignal lists
    QVector<unsigned int> chosen_indexes = utilities::findIndexesOf<bool>(chosen,true);
    unsigned int start_size = chosen.size();
    unsigned int end_size = chosen_indexes.size();
    unsigned int delete_size = start_size - end_size;
    QVector<QString> words2;
    words2.reserve(chosen_indexes.size()) ;
    QVector<bool> chosen2;
    chosen2.reserve(chosen_indexes.size());
    QVector<QString> runes2;
    runes2.reserve(chosen_indexes.size());
    QVector<unsigned long long> counts2;
    counts2.reserve(chosen_indexes.size());
    QVector<unsigned int> gematria_sum2;
    gematria_sum2.reserve(chosen_indexes.size());
    for(const auto& i : chosen_indexes){
        words2.push_back( words.at(i));
        chosen2.push_back( chosen.at(i));
        runes2.push_back( runes.at(i));
        counts2.push_back( counts.at(i));
        gematria_sum2.push_back( gematria_sum.at(i));
    }
    words = words2;
    chosen = chosen2;
    runes = runes2;
    counts = counts2;
    gematria_sum = gematria_sum2;

    // TODO here used to do
    // setWordToIndex(); // kept word to index lookup - probably used in ngram deleting !!
#ifdef QT_SIG
    updateChosenCounts();
#endif
    updateWordToIndex();
    Q_ASSERT( end_size == words.size());
    Q_ASSERT( end_size == chosen.size());
    Q_ASSERT( end_size == counts.size());
    Q_ASSERT( end_size == gematria_sum.size());
    Q_ASSERT( end_size == runes.size());
    qDebug() << "RawWordData::deleteNotChosen,"  << word_length << "start/end/dif =" << start_size
             << end_size << delete_size;
    return true;
}

void RawWordData::updateWordToIndex(){
    word_to_index.clear();
    for(int i = 0 ; i < words.size(); ++i){
        word_to_index[words.at(i)] = i;
    }
//    auto itA = words.begin();
//    auto itB = chosen.begin();
//    for (; (itA != words.end()) && (itB != chosen.end()); (++itA, ++itB)){
//        word_to_index[*itA] = *itB;
//    }
}





