
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
#ifndef DEFINITIONS_H
#define DEFINITIONS_H


#include<QHash>
#include<QString>
#include<QVector>

namespace definitions{

const char  DOUBLE_QUOTE = '\"';
const char  COMMENT = '#';
const char  SPACE = ' ';
const char  TAB = '\t';
const char  COLON = ':';
const char  COMMA = ',';
const char  CURLY_OPEN = '{';
const char  CURLY_CLOSE = '}';
const char  UNDERSCORE = '_';
const QString raw_1gram_filekey_01 = "raw_1grams_01";
const QString raw_1gram_filekey_02 = "raw_1grams_02";
const QString raw_1gram_filekey_03 = "raw_1grams_03";
const QString raw_1gram_filekey_04 = "raw_1grams_04";
const QString raw_1gram_filekey_05 = "raw_1grams_05";
const QString raw_1gram_filekey_06 = "raw_1grams_06";
const QString raw_1gram_filekey_07 = "raw_1grams_07";
const QString raw_1gram_filekey_08 = "raw_1grams_08";
const QString raw_1gram_filekey_09 = "raw_1grams_09";
const QString raw_1gram_filekey_10 = "raw_1grams_10";
const QString raw_1gram_filekey_11 = "raw_1grams_11";
const QString raw_1gram_filekey_12 = "raw_1grams_12";
const QString raw_1gram_filekey_13 = "raw_1grams_13";
const QString raw_1gram_filekey_14 = "raw_1grams_14";
const QString CONFIG_PATH= "../../config.ini";

static QVector<QString> raw_1gram_keys ={
raw_1gram_filekey_01,raw_1gram_filekey_02,raw_1gram_filekey_03,raw_1gram_filekey_04,
raw_1gram_filekey_05,raw_1gram_filekey_06,raw_1gram_filekey_07,raw_1gram_filekey_08,
raw_1gram_filekey_09,raw_1gram_filekey_10,raw_1gram_filekey_11,raw_1gram_filekey_12,
raw_1gram_filekey_13,raw_1gram_filekey_14};

//const char path_to_raw_1grams[] = "path_to_raw_1grams";
const QString PATH_TO_RAW_1GRAMS = "path_to_raw_1grams";
const QString PATH_TO_RAW_NGRAMS = "path_to_raw_ngrams";
const QString PATH_TO_PHRASE_SAVE_ROOT = "path_to_phrase_save_root";
const QString PATH_TO_LAST_PHRASE = "path_to_last_phrase";
const QString LOAD_LAST_PHRASE_ON_STARTUP = "load_last_phrase_on_startup";
const QString PATH_TO_LP_SENTENCES = "path_to_lp_sentences";
const QString PATH_TO_PHRASES = QString("PATH_TO_PHRASES");
const QString empty_string = "_";
const char empty_char = '_';

};

#endif // DEFINITIONS_H
