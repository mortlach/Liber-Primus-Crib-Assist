
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
#include "definitions.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <QDebug>
#include <QDir>
#include <QVariant>
#include <QSettings>
#include <QHash>

namespace utilities{

using VVString = std::vector<std::vector<std::string>>;

void printSettings(const QSettings& settings){
    Q_UNUSED(settings);
    QStringList keys = settings.allKeys();
    for(const auto& item: keys){

        std::cout << item.toStdString() << " ";
        std::cout << settings.value(item).toString().toStdString() << std::endl;
    }
    //TODO Write
}
//
QFont getFont(){
    QFont font("Courier New",9);
    font.setStyleHint(QFont::Monospace);
    return font;
}
//
QString toString(const QList<int>& list){
    if( list.size() == 1){
        return QString::number(list.at(0));
    }
    QString r;
    for(int i=0; i< list.size(); ++i){
        r.append( QString::number(list.at(i)));
        if(i<list.size()-1){
            r.append(",");
        }
    }
    return r;
}
//
bool extendPhrases(QList<QStringList>& phrase, const QList<QString>& words){
    QList<QStringList> new_phrases;
    for(const auto& p : phrase){
        //QStringList phrase_base = p;
        for(const auto& w: words){
            QStringList new_phrase = p;
            new_phrase << w;
            new_phrases << new_phrase;
        }
    }
    phrase = new_phrases;
    return true;
}
//
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "%s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    case QtInfoMsg:
        fprintf(stderr, "%s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    }
}

std::string getNgramPhraseDataAsString(const NgramPhraseMetaData& data_in, bool print){
    QStringList passed_data_as_stringlist = QStringList();
    passed_data_as_stringlist.append("");
    passed_data_as_stringlist.append("*** show new passed data ***");
    passed_data_as_stringlist.append("TOTAL PUW count = " + QString::number(data_in.phrase_word_lengths.size()));
//    for(const auto& item: data_in.puw_filenames){
//        passed_data_as_stringlist.append(item);
//    }
    int ngram_count = data_in.ngram_meta_data.size();
    passed_data_as_stringlist.append("TOTAL Ngram count = " + QString::number(ngram_count));
    passed_data_as_stringlist.append("Ngram individual data:");
    int counter = 0;
    for(const auto& item: data_in.ngram_meta_data){
        passed_data_as_stringlist.append("");
        passed_data_as_stringlist.append("NGRAM " + QString::number(counter));
        passed_data_as_stringlist.append("raw_file_exists  " + QString::number((int)item.raw_file_exists));
        passed_data_as_stringlist.append("raw_filename = " + QString(item.raw_filename));
        passed_data_as_stringlist.append("raw_filepath = " + QString(item.raw_filepath));
        passed_data_as_stringlist.append("phrase_dir = " + item.phrase_dir.absolutePath());
        passed_data_as_stringlist.append("phrase_ngram_filepath = " + item.phrase_ngram_filepath);
        passed_data_as_stringlist.append("ngram_length = " + QString::number(item.ngram_length));
        passed_data_as_stringlist.append("phrase_dir = " + item.phrase_dir.path());
        passed_data_as_stringlist.append("ngram_root_dir = " + item.ngram_root_dir.path());
        passed_data_as_stringlist.append("phrase_tokens = " + utilities::toString(item.phrase_tokens));
        passed_data_as_stringlist.append("ngram_tokens = " + utilities::toString(item.ngram_tokens));
        passed_data_as_stringlist.append("ngram_index = " + QString::number(item.ngram_index));
        passed_data_as_stringlist.append("phrase_word_lengths = " + utilities::toString(item.phrase_word_lengths));
        passed_data_as_stringlist.append("first_token_index_in_phrase = " + QString::number(item.first_token_index_in_phrase));
        passed_data_as_stringlist.append("first_word_index_in_phrase_word_indices = " + QString::number(item.first_word_index_in_phrase_word_indices));
        //passed_data_as_stringlist.append("phrase_word_indices = " + utilities::toString(item.phrase_word_indices));
        passed_data_as_stringlist.append("ngram_word_indices = " + utilities::toString(item.ngram_word_indices));
        ++counter;
    }
    passed_data_as_stringlist.append("***************************");
    std::string f = passed_data_as_stringlist.join('\n').toStdString();

    if(print){
        std::cout << f << std::endl;
    }
    return f;
}

QList<int>toIntList(const QString & list){
    QList<int> r;
    if(list.contains(",")){
        QStringList l = list.split(u',', Qt::SkipEmptyParts);
        for(int i = 0; i < l.size(); i++)
        {
            bool isNum = false;
            int n = QString(l.at(i)).toInt(&isNum);
            if(isNum){
                r << n;
            }
        }
    }
    else{
        std::cout << "\ntoIntList No, in list " << list.toStdString() << std::endl;
        bool ok = false;
        int single_value = list.toInt(&ok);
        if(ok){
            r << single_value;
        }
    }
    return r;
}

bool addWordListToPhraseList(QList<QString>& phrase_list,const QList<QString>& word_list){
    QList<QString> phrase_list_r;
    for(auto& phrase: phrase_list){
        QString& next_root_phrase = phrase;
        for(auto& word: word_list){
            next_root_phrase.append(" ");
            next_root_phrase.append(word);
            phrase_list_r.push_back(next_root_phrase);
        }
    }
    phrase_list = phrase_list_r;
    return true;
}

std::string getFullPath(const std::string& directory, const std::string& filename){
    std::filesystem::path full_path = directory;
    if(filename != std::string("")){
        full_path /= filename;
    }
    return std::filesystem::absolute(full_path).u8string();
}

QString getFullPath(const QString &directory, const QString &filename){
    std::string s = getFullPath(directory.toStdString(),filename.toStdString());
    return QString( s.c_str()  );
}

// how to do settgings and pass tehm aroiund!
void getConfigKeyValues(const QString& path, QStringList& keys, QStringList& values){
    keys.clear();
    values.clear();
    QSettings data(path, QSettings::IniFormat);
    qDebug() << data.status();
    keys = data.allKeys();
    foreach (const QString &key, keys){
        values << data.value(key).toString();
        qDebug() << "next k/v: " << key << " = " << values.back();
    }
}

QString getNgramDir(int ngram_length){
    switch(ngram_length) {
        case 1: return "/1-grams";
        case 2: return "/2-grams";
        case 3: return "/3-grams";
        case 4: return "/4-grams";
        case 5: return "/5-grams";
    default: return "error";
    }
    return "error";
}

// convert code used in gui to file names
QPair<QString,QString> ngramSchemaDataToFilePath(const QDir& root, const QList<int>& ngram_lengths){
    QString next_fp("");
    for(const auto& c : ngram_lengths){
        QString str;
        switch(c){
            case -2: str = "e"; break;
            case -1: str = "s"; break;
            default: str.setNum(c);
        }
        next_fp += str;
        next_fp += " ";
    }
    next_fp.chop(1);
    QString filename = next_fp + ".txt";
    QString fp = root.path() + getNgramDir(ngram_lengths.size()) + "/" + filename;
    return QPair<QString,QString>(filename,fp);
}

// TODO move to class
bool parseLPSentencesDataFile(const QString &full_path, QList<QStringList>& file_data)
{
    QString p = full_path;
    QFile file(full_path);
    if(!file.exists()){
        qDebug() << "parseLPSentencesDataFile ERROR, bad path, " << full_path;
        return false;
    }
    file_data.clear();
    if (file.open(QFile::ReadOnly)) {
        QTextStream in(&file);
        bool reading_data = false;
        while (!in.atEnd()) {
            QString line = in.readLine();
            QString original_line = line;
            // trim each line
            line = line.trimmed();
            if(line.isEmpty()){
                continue;
            }
            // check for start end tags
            if(line.at(0) == definitions::CURLY_OPEN){
                reading_data = true;
                continue;
            }
            if(line.at(0) == definitions::CURLY_CLOSE){
                break;
            }
            if(reading_data){
                // remove to the right of comment char #
                int pos = line.lastIndexOf(definitions::COMMENT);
                if( pos != -1){
                    line = line.left(pos);
                    line = line.trimmed();
                }
                // split on space ' '
                QStringList data = line.split(definitions::SPACE);
                if(data.length() < 1 ){
                    qDebug() << "parseLPSentencesDataFile skipping over this line: " << original_line;
                    continue;
                }else{

                    file_data.push_back(data);
                }
            }
        }
    }
    return true;
}
/* generic write csv file */
bool writeCSVFile(const std::string &directory, const std::string &filename,
                  const VVString& file_data){
    return utilities::writeCSVFile(utilities::getFullPath(directory,filename), file_data);
}
bool writeCSVFile(const std::string& path,const VVString& file_data)
{
    auto p = std::filesystem::absolute(path);
    std::ofstream  file;
    file.open(p);
    if (!file) {
        qDebug() << "failed to open file: " << p.string().c_str();
        return false;
    }{
        for(const auto& item : file_data){
            for(unsigned int i = 0; i < item.size() -1; ++i ){
                file << item.at(i);
                file << definitions::COMMA;
            }
            file << item.back();
            file << std::endl;
        }
    }
    file.close();
    qDebug() << path.c_str() << " wrote ok by write1GramDataFile";
    return true;
}


/* generic read csv file */
bool readCSVFile(const std::string &directory, std::string &filename,
                 VVString& file_data){
    return utilities::readCSVFile(utilities::getFullPath(directory,filename), file_data);
}
//
bool readCSVFile(const std::string& path, VVString& file_data){
    auto p = std::filesystem::absolute(path);
    std::ifstream  file;
    file.open(p);
    //file.open(path);
    if (!file) {
        qDebug() << "readCSVFile failed to open file: " << path.c_str();
        return false;
    }
    else {
        //VVString::const_iterator file_data_it = file_data.cbegin();
        auto line_count = std::count(std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>(), '\n');
        file_data.clear();
        file_data.reserve(line_count);
        qDebug() << "Read:" << path.c_str();
        //int lineindex = 0;                     // file line counter
        std::string line;
        file.seekg(0, std::ios::beg);
        while (std::getline(file, line))
        {
            // TODO
            // replace " " with "," !!! breaking csv formate etc
            std::replace(line.begin(), line.end(), ' ', ',');
            //line.erase(remove(line.begin(),line.end(),' '),line.end()); // no spaces allowed!
            // parse the read line into separate pieces(tokens) with "," as the delimiter
            std::string tmp;
            std::stringstream ss(line);
            std::vector<std::string> new_data;
            // TODO no error checking
            while(std::getline(ss, tmp, definitions::COMMA)){
                new_data.push_back(tmp);
            }
            file_data.push_back(new_data);
            //lineindex++;
        }
    }
    return true;
}
//
bool readTrueCSVFile(const std::string& path, VVString& file_data){
    auto p = std::filesystem::absolute(path);
    std::ifstream  file;
    file.open(p);
    //file.open(path);
    if (!file) {
        qDebug() << "readCSVFile failed to open file: " << path.c_str();
        return false;
    }
    else {
        //VVString::const_iterator file_data_it = file_data.cbegin();
        auto line_count = std::count(std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>(), '\n');
        file_data.clear();
        file_data.reserve(line_count);
        qDebug() << "Read:" << path.c_str();
        //int lineindex = 0;                     // file line counter
        std::string line;
        file.seekg(0, std::ios::beg);
        while (std::getline(file, line))
        {
            // TODO
            // replace " " with "," !!! breaking csv formate etc
            //std::replace(line.begin(), line.end(), ' ', ',');
            //line.erase(remove(line.begin(),line.end(),' '),line.end()); // no spaces allowed!
            // parse the read line into separate pieces(tokens) with "," as the delimiter
            std::string tmp;
            std::stringstream ss(line);
            std::vector<std::string> new_data;
            // TODO no error checking
            while(std::getline(ss, tmp, definitions::COMMA)){
                new_data.push_back(tmp);
            }
            file_data.push_back(new_data);
            //lineindex++;
        }
    }
    return true;
}
//
bool getFileNameAndDir(const QString& start_dir,
                              QString& chosen_dir, QString& chosen_filename)
{
    qDebug() << "getFileNameAndDir()";
    QFileDialog* fileDlg = new QFileDialog(nullptr,"Choose File",start_dir);
    fileDlg->setFileMode(QFileDialog::AnyFile);
    fileDlg->setOption(QFileDialog::DontUseNativeDialog);
    bool worked = false;
    if (fileDlg->exec()==QDialog::Accepted) {
        QStringList fl = fileDlg->selectedFiles();
        if (!fl.isEmpty()){
            QString fn = fl[0];
            QFileInfo fi(fn);
            chosen_dir = fi.absolutePath();
            chosen_filename = fi.fileName();
            worked = true;
            QDir binPath(QDir::currentPath());
            chosen_dir = binPath.relativeFilePath(chosen_dir);
        }
        else{
            worked = false;
        }
    }
    return worked;
}
//
bool getDirectory(const QString& start_dir, QString& path, const QString& message)
{
    QString m = "Choose Directory";
    if( message != ""){
        m = message;
    }
    qDebug() << "getDirectory()";
    QFileDialog* fileDlg = new QFileDialog(nullptr, m, start_dir);
    fileDlg->setFileMode(QFileDialog::Directory);
    fileDlg->setViewMode(QFileDialog::List);
    fileDlg->setOption(QFileDialog::ShowDirsOnly);
    fileDlg->setOption(QFileDialog::HideNameFilterDetails);
    fileDlg->setOption(QFileDialog::DontUseNativeDialog);
    bool worked = false;
    if (fileDlg->exec()) {
        QStringList fl = fileDlg->selectedFiles();
        if (!fl.isEmpty()){
            QString chosen_path = fl[0];
            path = chosen_path;
            qDebug() << "getDirectory() return path:" << path;
            worked = true;
        }
        else{
            worked = false;
        }
    }
    QDir binPath(QDir::currentPath());
    path = binPath.relativeFilePath(path);
    return worked;
}
//
bool getSaveFileName(const QString& start_dir, QString& path)
{
    qDebug() << "getSaveFileName()";
    QFileDialog* fileDlg = new QFileDialog(nullptr,"Choose Directory");
    fileDlg->setDirectory(start_dir);
    fileDlg->setFileMode(QFileDialog::AnyFile);
    bool worked = false;
    if (fileDlg->exec()==QDialog::Accepted) {
        QStringList fl = fileDlg->selectedFiles();
        if (!fl.isEmpty()){
            QString fn = fl[0];
            path = QFileInfo(fn).absoluteFilePath();
            worked = false;
        }
        else{
            worked = true;
        }
    }
    QDir binPath(QDir::currentPath());
    path = binPath.relativeFilePath(path);
    return worked;
}
//
bool allRuneChar(const QString& word){
    Q_UNUSED(word);
    return false;
}

};
