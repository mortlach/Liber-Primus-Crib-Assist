
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
#include "ngrammetadata.h"
#include "utilities.h"
#include <iostream>

NgramMetaData::NgramMetaData()
        :ngram_root_dir(QDir(""))
        ,raw_filepath(QString("RAW FILEPATH"))
        ,raw_filename(QString("RAW FILENAME"))
        ,phrase_dir(QDir(""))
        ,phrase_ngram_filename(QString("phrase_ngram_filename"))
        ,phrase_ngram_filepath(QString("phrase_ngram_filepath"))
        ,phrase_tokens( QList<int>())
        ,ngram_tokens( QList<int>())
        ,phrase_word_lengths(QList<int>())
        ,phrase_word_indices(QList<int>())
        ,ngram_word_indices(QList<int>())
        ,ngram_length(-1)
        ,first_token_index_in_phrase(-1)
        ,first_word_index_in_phrase_word_indices(-1)
        ,raw_file_exists(false)
        ,has_sent_start(false)
        ,has_sent_end(false)
        ,ngram_index(-1)
{}

void NgramMetaData::print(){
    std::cout << "raw_file_exists  " + QString::number((int)raw_file_exists).toStdString() <<std::endl;
    std::cout << "raw_filename = " << QString(raw_filename).toStdString() <<std::endl;
    std::cout << "raw_filepath = " << QString(raw_filepath).toStdString() <<std::endl;
    std::cout << "phrase_dir = " << phrase_dir.absolutePath().toStdString() <<std::endl;
    std::cout << "phrase_ngram_filepath = " << phrase_ngram_filepath.toStdString() <<std::endl;
    std::cout << "ngram_length = " << QString::number(ngram_length).toStdString() <<std::endl;
    std::cout << "phrase_dir = " << phrase_dir.path().toStdString() <<std::endl;
    std::cout << "ngram_root_dir = " << ngram_root_dir.path().toStdString() <<std::endl;
    std::cout << "phrase_tokens = " << utilities::toString(phrase_tokens).toStdString() <<std::endl;
    std::cout << "ngram_tokens = " << utilities::toString(ngram_tokens).toStdString() <<std::endl;
    std::cout << "ngram_index = " << QString::number(ngram_index).toStdString() <<std::endl;
    std::cout << "phrase_word_lengths = "<< utilities::toString(phrase_word_lengths).toStdString() <<std::endl;
    std::cout << "first_token_index_in_phrase " << QString::number(first_token_index_in_phrase).toStdString() <<std::endl;
    std::cout << "first_word_index_in<<phrase_word_indices = " << QString::number(first_word_index_in_phrase_word_indices).toStdString() <<std::endl;
    std::cout << "ngram_word_indices = " << utilities::toString(ngram_word_indices).toStdString() <<std::endl;
    std::cout << "hasAnyDefaultSetttings = " << hasAnyDefaultSetttings() <<std::endl;
}

bool NgramMetaData::hasAnyDefaultSetttings(){
    std::cout << "\n\nNgramMetaData::hasAnyDefaultSetttings" << std::endl;
    bool r = false;
    if( ngram_root_dir == QDir("") ){
        std::cout << "ngram_root_dir is at default" << std::endl;
        r = true;
    }
    if( raw_filepath == QString("RAW FILEPATH") ){
        std::cout << "ngram_root_dir is at default" << std::endl;
        r = true;
    }
    if( raw_filename == QString("RAW FILENAME") ){
        std::cout << "ngram_root_dir is at default" << std::endl;
        r = true;
    }
    if( phrase_dir == QDir("") ){
        std::cout << "phrase_dir is at default" << std::endl;
        r = true;
    }
    if( phrase_ngram_filename == QString("phrase_ngram_filename") ){
        std::cout << "phrase_ngram_filename is at default" << std::endl;
        r = true;
    }
    if( phrase_ngram_filepath == QString("phrase_ngram_filepath") ){
        std::cout << "phrase_ngram_filepath is at default" << std::endl;
        r = true;
    }
    if( phrase_tokens == QList<int>() ){
        std::cout << "phrase_tokens is at default" << std::endl;
        r = true;
    }
    if( ngram_tokens == QList<int>() ){
        std::cout << "ngram_tokens is at default" << std::endl;
        r = true;
    }
    if( phrase_word_lengths == QList<int>() ){
        std::cout << "phrase_word_lengths is at default" << std::endl;
        r = true;
    }
    if( phrase_word_indices == QList<int>() ){
        std::cout << "phrase_word_indices is at default" << std::endl;
        r = true;
    }
    if( ngram_word_indices == QList<int>() ){
        std::cout << "ngram_word_indices is at default" << std::endl;
        r = true;
    }
    if( ngram_length == -1){
        std::cout << "ngram_length is at default" << std::endl;
        r = true;
    }
    if( first_token_index_in_phrase == -1 ){
        std::cout << "first_token_index_in_phrase is at default" << std::endl;
        r = true;
    }
    if( first_word_index_in_phrase_word_indices == -1 ){
        std::cout << "first_word_index_in_phrase_word_indices is at default" << std::endl;
        r = true;
    }
    return r;
}



