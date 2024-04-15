
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
#ifndef UTILITIES_H
#define UTILITIES_H
#include <map>
#include <string>
#include <stdexcept>
#include <vector>
#include <any>
#include <QMap>
#include <QString>
#include <QList>
#include <QSettings>
#include <filesystem>
#include <QFont>
#include <QDir>
#include <QFileDialog>
#include "ngrammetadata.h"

namespace utilities{
    extern void printSettings(const QSettings& settings);
    extern void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    extern std::string getNgramPhraseDataAsString(const NgramPhraseMetaData& data_in, bool print = true);
    extern bool allRuneChar(const QString& word);
    extern bool addWordListToPhraseList(QList<QString>& phrase_list,const QList<QString>& word_list);
    /* app config file */
    extern void getConfigKeyValues(const QString& path, QStringList& keys, QStringList& values);
    /* */
    extern std::string getFullPath(const std::string &directory, const std::string &filename = "");
    extern QString getFullPath(const QString &directory, const QString &filename = "");
    /* generic write csv file */
    extern bool writeCSVFile(const std::string &directory, const std::string &filename,
                             const std::vector<std::vector<std::string>>& file_data);
    extern bool writeCSVFile(const std::string& path, const std::vector<std::vector<std::string>>& file_data);
    /* generic read csv file */
    extern bool readTrueCSVFile(const std::string& path, std::vector<std::vector<std::string>>& file_data);
    extern bool readCSVFile(const std::string& path, std::vector<std::vector<std::string>>& file_data);
    extern bool readCSVFile(const std::string &directory, const std::string &filename,
                            std::vector<std::vector<std::string>>& file_data);

    extern bool parseLPSentencesDataFile(const QString &full_path, QList<QStringList>& file_data);
    extern QFont getFont(int fs = 11);
    extern QString getNgramDir(int ngram_length);
    extern QPair<QString,QString> ngramSchemaDataToFilePath(const QDir& root, const QList<int>& ngram_lengths);

    template<typename T>
    std::vector<T> slice(const std::vector<T>& v, int start=0, int end=-1)
    {
        int oldlen = v.size();
        int newlen;
        if (end == -1 or end >= oldlen){
            newlen = oldlen-start;
        } else {
            newlen = end-start;
        }
        std::vector<T> nv(newlen);
        for (int i=0; i<newlen; i++) {
            nv[i] = v[start+i];
        }
        return nv;
    }
    template<typename T>
    QList<T> slice(const QList<T>& v, int start=0, int end=-1)
    {
        int oldlen = v.size();
        int newlen;
        if (end == -1 or end >= oldlen){
            newlen = oldlen-start;
        } else {
            newlen = end-start;
        }
        QList<T> nv(newlen);
        for (int i=0; i<newlen; i++) {
            nv[i] = v[start+i];
        }
        return nv;
    }

    inline std::string boolToString(bool b)
    {
      return b ? std::string("1") : std::string("0");
    }
    inline bool stringToBool(const std::string& b, bool& answer)
    {
      if(b == std::string("1")){
          answer = true;
          return true;
      }
      else if(b == std::string("0")){
          answer = false;
          return true;
      }
      return false;
    }

    extern QList<unsigned int> findIndexesOf(const QList<QString>& list, const QString& item);
    template<typename T>
    QList<unsigned int> findIndexesOf(const QList<T>& list, const T& item)
    {
        QList<unsigned int> r = QList<unsigned int>();
        unsigned int counter = 0;
        for(const auto& list_item: list){
            if(list_item == item){
              r.push_back(counter);
            }
            ++counter;
        }
        return r;
    }

    extern QString toString(const QList<int>& list);
    extern QList<int>toIntList(const QString & list);


    static const QMap<QString, QString> letter_to_rune =  {
               {QString("F"), QString("ᚠ")}         , {QString("U"), QString("ᚢ")}         , {QString("TH"), QString("ᚦ")}
             , {QString('O'), QString("ᚩ")}         , {QString('R'), QString("ᚱ")}         , {QString('C'), QString("ᚳ")}
             , {QString('G'), QString("ᚷ")}         , {QString('W'), QString("ᚹ")}         , {QString('H'), QString("ᚻ")}
             , {QString('N'), QString("ᚾ")}         , {QString('I'), QString("ᛁ")}         , {QString('J'), QString("ᛂ")}
             , {QString("EO"), QString("ᛇ")}        , {QString('P'), QString("ᛈ")}         , {QString('X'), QString("ᛉ")}
             , {QString('S'), QString("ᛋ")}         , {QString('T'), QString("ᛏ")}         , {QString('B'), QString("ᛒ")}
             , {QString('E'), QString("ᛖ")}         , {QString('M'), QString("ᛗ")}         , {QString('L'), QString("ᛚ")}
             , {QString("ING"), QString("ᛝ")}       , {QString("OE"), QString("ᛟ")}         , {QString("D"), QString("ᛞ")}
             , {QString("A"), QString("ᚪ")}         , {QString("AE"), QString("ᚫ")}         , {QString("Y"), QString("ᚣ")}
             , {QString("IO"), QString("ᛡ")}        , {QString("EA"), QString("ᛠ")}
         };
    static const QMap<QString, QString> rune_to_letter = {
               {QString("ᚠ"), QString("F")}         , {QString("ᚢ"), QString("U")}         , {QString("ᚦ"), QString("TH")}
             , {QString("ᚩ"), QString('O')}         , {QString("ᚱ"), QString('R')}         , {QString("ᚳ"), QString('C')}
             , {QString("ᚷ"), QString('G')}         , {QString("ᚹ"), QString('W')}         , {QString("ᚻ"), QString('H')}
             , {QString("ᚾ"), QString('N')}         , {QString("ᛁ"), QString('I')}         , {QString("ᛂ"), QString('J')}
             , {QString("ᛇ"), QString("EO")}         , {QString("ᛈ"), QString('P')}         , {QString("ᛉ"), QString('X')}
             , {QString("ᛋ"), QString('S')}         , {QString("ᛏ"), QString('T')}         , {QString("ᛒ"), QString('B')}
             , {QString("ᛖ"), QString('E')}         , {QString("ᛗ"), QString('M')}         , {QString("ᛚ"), QString('L')}
             , {QString("ᛝ"), QString("ING")}         , {QString("ᛟ"), QString("OE")}         , {QString("ᛞ"), QString("D")}
             , {QString("ᚪ"), QString("A")}         , {QString("ᚫ"), QString("AE")}         , {QString("ᚣ"), QString("Y")}
             , {QString("ᛡ"), QString("IO")}         , {QString("ᛠ"), QString("EA")}     };

    static const  QMap<unsigned int, QString> number_to_rune =  {
               {0,  QString("ᚠ")}         , {1, QString("ᚢ")}    , {2, QString("ᚦ")}
             , {3,  QString("ᚩ")}         , {4, QString("ᚱ")}    , {5, QString("ᚳ")}
             , {6,  QString("ᚷ")}         , {7, QString("ᚹ")}    , {8, QString("ᚻ")}
             , {9,  QString("ᚾ")}         , {10, QString("ᛁ")}   , {11, QString("ᛂ")}
             , {12, QString("ᛇ")}        , {13, QString("ᛈ")}   , {14, QString("ᛉ")}
             , {15, QString("ᛋ")}         , {16, QString("ᛏ")}  , {17, QString("ᛒ")}
             , {18, QString("ᛖ")}         , {19, QString("ᛗ")}  , {20, QString("ᛚ")}
             , {21, QString("ᛝ")}       , {22, QString("ᛟ")}    , {23, QString("ᛞ")}
             , {24, QString("ᚪ")}         , {25, QString("ᚫ")}  , {26, QString("ᚣ")}
             , {27, QString("ᛡ")}        , {28, QString("ᛠ")}
         };
    static const QMap<unsigned int, QString> number_to_letter= {
               {0,  QString("F")}     , { 1, QString("U")}    , { 2  , QString("TH")}
             , {3,  QString('O')}     , { 4 , QString('R')}   , { 5 , QString('C')}
             , {6,  QString('G')}     , { 7 , QString('W')}   , { 8 , QString('H')}
             , {9,  QString('N')}     , { 10 , QString('I')}  , { 11 , QString('J')}
             , {12, QString("EO")}    , { 13 , QString('P')}  , { 14 , QString('X')}
             , {15, QString('S')}     , { 16 , QString('T')}  , { 17 , QString('B')}
             , {18, QString('E')}     , { 19 , QString('M')}  , { 20 , QString('L')}
             , {21, QString("ING")}   , { 22 , QString("OE")} , { 23 , QString("D")}
             , {24, QString("A")}     , { 25 , QString("AE")} , { 26 , QString("Y")}
             , {27, QString("IO")}    , { 28 , QString("EA")}     };

    static const  QMap<QString, unsigned int> rune_to_number =  {
               {QString("ᚠ")  ,0   }      , {QString("ᚢ") ,1,}    , {QString("ᚦ") ,2}
             , {QString("ᚩ")  ,3   }      , {QString("ᚱ") ,4 }    , {QString("ᚳ") ,5 }
             , {QString("ᚷ")  ,6   }      , {QString("ᚹ") ,7 }    , {QString("ᚻ") ,8 }
             , {QString("ᚾ")  ,9   }      , {QString("ᛁ") ,10}    , { QString("ᛂ") ,11 }
             , {QString("ᛇ")  ,12  }      , {QString("ᛈ") ,13}    , { QString("ᛉ"),14 }
             , {QString("ᛋ")  ,15  }      , {QString("ᛏ") ,16}    , { QString("ᛒ"),17 }
             , {QString("ᛖ")  ,18  }       , {QString("ᛗ") ,19}    , { QString("ᛚ"),20 }
             , {QString("ᛝ")  ,21  }       , {QString("ᛟ") ,22}    , { QString("ᛞ"),23 }
             , {QString("ᚪ")  ,24  }       , {QString("ᚫ") ,25}    , { QString("ᚣ"),26 }
             , {QString("ᛡ")  ,27  }       , {QString("ᛠ") ,28}
         };
    static const QMap<QString , unsigned int> letter_to_number = {
               {QString("F")  ,0    }   , {  QString("U") , 1   }   , { QString("TH") , 2   }
             , {QString('O')  ,3    }   , {  QString('R') , 4   }   , { QString('C')  , 5   }
             , {QString('G')  ,6    }   , {  QString('W') , 7   }   , { QString('H')  , 8   }
             , {QString('N')  ,9    }   , {  QString('I') , 10  }   , {  QString('J') , 11  }
             , {QString("EO") ,12   }   , {  QString('P') , 13  }   , {  QString('X') , 14  }
             , {QString('S')  ,15   }   , {  QString('T') , 16  }   , {  QString('B') , 17  }
             , {QString('E')  ,18   }   , {  QString('M') , 19  }   , {  QString('L') , 20  }
             , {QString("ING"),21   }   , {  QString("OE"), 22  }   , {  QString("D") , 23  }
             , {QString("A")  ,24   }   , {  QString("AE"), 25  }   , {  QString("Y") , 26  }
             , {QString("IO") ,27   }   , {  QString("EA"), 28  }     };

    extern bool getFileNameAndDir(const QString& start_dir,
                                  QString& chosen_dir, QString& chosen_filename);
    extern bool getDirectory(const QString& start_dir, QString& path, const QString& message);
    extern bool getSaveFileName(const QString& start_dir, QString& path);
    extern bool extendPhrases(QList<QStringList>& phrase, const QList<QString>& words);

}
#endif // UTILITIES_H
