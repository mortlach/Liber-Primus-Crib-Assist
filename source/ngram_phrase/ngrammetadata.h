
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
#ifndef NGRAMMETADATA_H
#define NGRAMMETADATA_H

#include <QObject>
#include <QDir>
#include <QString>
#include <QList>

class NgramMetaData
{
public:
    NgramMetaData();
    ~NgramMetaData(){}

    void print();
    bool hasAnyDefaultSetttings();
    // file paths to get to raw data
    QDir ngram_root_dir; // raw files opnly ever read
    QString raw_filepath;
    QString raw_filename;
    // file paths to get to  "local copy" phrase data
    QDir phrase_dir;
    QString phrase_ngram_filename; // local files can save and read
    QString phrase_ngram_filepath; // local files can save and read
    // define word lengths, or tags, -1 == S, -2 ==E and must be at start, end
    QList<int> phrase_tokens; // tokens for whole phrase
    QList<int> ngram_tokens; // tokens for this ngram
    QList<int> phrase_word_lengths;
    QList<int> ngram_word_lengths;
    QList<int> phrase_word_indices;
    QList<int> ngram_word_indices;
    // number of tokens in ngram (including S E tags)
    int ngram_length;
    // index of first token this ngram in phrase
    int first_token_index_in_phrase;
    // index of first word in phrase
    int first_word_index_in_phrase_word_indices; // index of first phrase-word in this ngram
    bool raw_file_exists;
    bool has_sent_start;
    bool has_sent_end;
    // index of this ngram in the phrase
    int ngram_index;
};


struct NgramPhraseMetaData
{
public:
    NgramPhraseMetaData()
        :config_path(QString())
        ,raw_ngram_root_dir(QDir())
        ,phrase_dir(QDir())
        ,phrase_tokens( QList<int>())
        ,phrase_word_lengths(QList<int>())
        ,ngram_token_lengths(QList<int>())
        ,ngram_lengths(QList<int>())
        ,phrase_ngram_filenames(QList<QString>())
        ,raw_ngram_filenames(QList<QString>())
        ,has_sent_start(false)
        ,has_sent_end(false)
        ,ngram_meta_data(QList<NgramMetaData>())
    {}
    ~NgramPhraseMetaData(){}
    //
    QString config_path;
    //
    QDir raw_ngram_root_dir; // raw files opnly ever read
    QDir phrase_dir; // local files can save an dread
    // define word lengths, or tags, -1 == S, -2 ==E and must be at start, end
    QList<int> phrase_tokens;
    QList<int> phrase_word_lengths;
    QList<int> ngram_token_lengths;
    QList<int> ngram_lengths;
    QList<QString> phrase_ngram_filenames;
    QList<QString> raw_ngram_filenames;
    bool has_sent_start;
    bool has_sent_end;
    QList<NgramMetaData> ngram_meta_data;
};
#endif // NGRAMMETADATA_H
