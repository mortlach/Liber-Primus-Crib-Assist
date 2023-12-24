
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
#include "phraseuniquewords.h"
#include "utilities.h"
#include "qdebug.h"

PhraseUniqueWords::PhraseUniqueWords()
    :word_index_in_phrase(-1)
{

}
PhraseUniqueWords::PhraseUniqueWords(int word_index,
                                     int word_length_in,
                                     const QString& data_file_path_in)
    : word_index_in_phrase(word_index)
    , words(QList<QString>())
    , word_length(word_length_in)
    , data_file_name("puw_"+QString::number(word_index) +".csv")
    , data_file_path(data_file_path_in)
    , data_path(QString("data_path NOT SET YET"))
{
    setFilePath(data_file_path,data_file_name);
    if(loadData()){
        qDebug() << "PhraseUniqueWords Constructor loaded" <<  words.size() << "words";
    }
    else{
        qDebug() << "PhraseUniqueWords Constructor failed to load data";
    }
}

PhraseUniqueWords::PhraseUniqueWords(int word_index,
                                     int word_length_in,
                                     const QString& data_file_path_in,
                                     const QHash<QString, QPair<int,int>>& hash_in)
    : word_index_in_phrase(word_index)
    , words(hash_in.keys())
    , word_length(word_length_in)
    , data_file_name("puw_" + QString::number(word_index) + ".csv")
    , data_file_path(data_file_path_in)
    , data_path(QString("data_path NOT SET YET"))
{
    setFilePath(data_file_path,data_file_name);
    chosen = QList<bool>(words.size(),true);
    for(const auto & item: hash_in.values()){
        total_counts_in_all_ngrams.push_back(item.first);
        ngrams_with_this_word.push_back(item.second);
    }
    updateWordToIndex();
}

void PhraseUniqueWords::getChosenWords(QList<QString>& new_words)const{
    new_words.clear();
    for(int i = 0; i < words.size(); ++i){
        if(chosen.at(i)){
            new_words.push_back(words.at(i));
        }
    }
}

void PhraseUniqueWords::updateWordToIndex(){
    word_to_index.clear();
    for(int i = 0 ; i < words.size(); ++i){
        word_to_index[words.at(i)] = i;
    }
}

void PhraseUniqueWords::keepDatForTheseWordssOnly(const QList<QString>& keep_words){
    QList<QString> new_words;
    QList<bool> new_chosen;
    QList<unsigned long long> new_total_counts_in_all_ngrams;
    QList<int> new_ngrams_with_this_word;
    new_words.reserve(new_words.size());
    new_chosen.reserve(new_words.size());
    new_total_counts_in_all_ngrams.reserve(new_words.size());
    new_ngrams_with_this_word.reserve(new_words.size());
    QHash<QString, int> new_word_to_index;
    int new_index = 0;
    for(const auto& word: keep_words){
        int i = word_to_index[word];
        new_words.push_back(word);
        new_chosen.push_back(chosen.at(i));
        new_total_counts_in_all_ngrams.push_back(total_counts_in_all_ngrams.at(i));
        new_ngrams_with_this_word.push_back(ngrams_with_this_word.at(i));
        new_word_to_index[word] = new_index;
        new_index +=1;
    }
}

void PhraseUniqueWords::setNewWords(const QList<QString>& new_words,
                                    const QList<unsigned long long>& new_total_counts_in_all_ngrams,
                                    const QList<int>& new_ngrams_with_this_word){
    qDebug() << "PhraseUniqueWords::setNewWords old size" << words.size() << "new size" << new_words.size();
    words = new_words;
    total_counts_in_all_ngrams = new_total_counts_in_all_ngrams;
    ngrams_with_this_word = new_ngrams_with_this_word;
    chosen = QList<bool>(words.size(), true);
    updateWordToIndex();
    updateChosenCounts();
}

bool PhraseUniqueWords::setTheseWordsChosenElseFalse(const QList<QString>& chosen_words){
    for(int i= 0; i < words.size(); ++i){
        if( chosen_words.contains(words.at(i)) ){
            chosen[i] = true;
        }
        else{
            chosen[i] = false;
        }
    }
    updateChosenCounts();
    return true;
}

bool PhraseUniqueWords::loadData(){
    qDebug() << "PhraseUniqueWords::loadData" << data_path;
    QFile file = QFile(data_path);
    if(file.exists()){
        has_data = false;
        any_data.clear();
        string_data.clear();
        if(utilities::readCSVFile(data_path.toStdString(), string_data)){
            any_data.reserve(string_data.size());
            for(const auto& item : string_data){
                std::vector<std::any> next_entry;
                next_entry.reserve(4);
                next_entry.push_back(std::any(item.at(0)));
                next_entry.push_back(std::any(std::stoull(item.at(1))));
                next_entry.push_back(std::any(std::stoi(item.at(2))));
                next_entry.push_back(std::any((bool)std::stoi(item.at(3))));
                any_data.push_back(next_entry);
            }
            if(setFromAnyData(any_data)){
                return true;
            }
        }
    }
    return false;
}

bool PhraseUniqueWords::getAsAnyData(std::vector<std::vector<std::any>>& data_to_fill)const{
    data_to_fill.clear();
    data_to_fill.reserve(words.size());
    for(unsigned int i = 0; i < words.size(); ++i){ // !
        std::vector<std::any> next_line;
        next_line.reserve(5);
        next_line.push_back( std::any(words.at(i).toStdString())  );
        next_line.push_back( std::any(total_counts_in_all_ngrams.at(i)) );
        next_line.push_back( std::any(ngrams_with_this_word.at(i)) );
        next_line.push_back( std::any(chosen.at(i)) );
        data_to_fill.push_back(next_line);
    }
    return true;
}

bool PhraseUniqueWords::setFromAnyData(const std::vector<std::vector<std::any>>& raw_data){
    words.clear();
    chosen.clear();
    total_counts_in_all_ngrams.clear();
    ngrams_with_this_word.clear();
    words.reserve(raw_data.size());
    chosen.reserve(raw_data.size());
    total_counts_in_all_ngrams.reserve(raw_data.size());
    ngrams_with_this_word.reserve(raw_data.size());
    // 0, string // 1, ull //2, ull // 3, bool
    for(const auto& item : raw_data){
        words.push_back(QString::fromStdString(std::any_cast<std::string>(item.at(0))));
        total_counts_in_all_ngrams.push_back(std::any_cast<unsigned long long >(item.at(1)));
        ngrams_with_this_word.push_back(std::any_cast<int>(item.at(2)));
        chosen.push_back(std::any_cast<bool>(item.at(3)));
    }
    updateWordToIndex();
    return true;
}

bool PhraseUniqueWords::saveData()const{
    std::vector<std::vector<std::any>> any_data;
    if( getAsAnyData(any_data)){
        qDebug() << "PhraseUniqueWords:saveData," << data_path;
        std::vector<std::vector<std::string>> string_data;
        for(const auto& item: any_data){
            if(item.size() != 4){// !
                qDebug() << "PhraseUniqueWords::saveRawWordData ERROR not enough columns in data";
                return false;
            }
            std::vector<std::string> next_line;// !
            next_line.reserve(4);
            next_line.push_back(std::any_cast<std::string>(item.at(0)));
            next_line.push_back(std::to_string (std::any_cast<unsigned long long>(item.at(1))));
            next_line.push_back(std::to_string (std::any_cast<int>(item.at(2))));
            next_line.push_back(std::to_string (std::any_cast<bool>(item.at(3))));
            string_data.push_back(next_line);
        }
        return utilities::writeCSVFile(data_path.toStdString(), string_data);
    }
    return false;
}



