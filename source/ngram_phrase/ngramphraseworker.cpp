
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
#include "ngramphraseworker.h"
#include "mainwindow.h"
#include "ngramphrase.h"
#include "ngramdatahub.h"



void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if(MainWindow::s_textEdit == 0)
    {
        QByteArray localMsg = msg.toLocal8Bit();
        switch (type) {
        case QtDebugMsg:
            fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
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
            break;
        case QtInfoMsg:
            break;
        }
    }
    else
    {
        switch (type) {
        case QtDebugMsg:
        case QtWarningMsg:
        case QtCriticalMsg:
            // redundant check, could be removed, or the
            // upper if statement could be removed
            if(MainWindow::s_textEdit != 0)
                MainWindow::s_textEdit->append(msg);
            break;
        case QtFatalMsg:
            abort();
            break;
        case QtInfoMsg:
            break;
        }
        //    }
    }
};

NgramPhraseWorker::NgramPhraseWorker()
    :last_process_was_success(false)
    ,is_working(false)
    ,dir_path(QString("dir_path NOT SET"))
    ,filename(QString("filename NOT SET"))
    ,do_delete(false)
    ,puw_has_priority(false)
{

};


// NgramPhraseWorker::NgramPhraseWorker(NgramPhrase& phrase,
//                                      NgramDataHub& ngd)
//     :phrase_ptr(&phrase)
//     ,ngram_data_hub_ptr(&ngd)
//     ,last_process_was_success(false)
//     ,is_working(false)
//     ,dir_path(QString("dir_path NOT SET"))
//     ,filename(QString("filename NOT SET"))
//     ,do_delete(false)
// {}
// NgramPhraseWorker::NgramPhraseWorker(NgramDataHub& ngd)
//     :phrase_ptr(&ngd.getNgramPhraseRef())
//     ,ngram_data_hub_ptr(&ngd)
//     ,last_process_was_success(false)
//     ,is_working(false)
//     ,dir_path(QString("dir_path NOT SET"))
//     ,filename(QString("filename NOT SET"))
//     ,do_delete(false)
// {}

void NgramPhraseWorker::processLoadPhrase(){
    //qInstallMessageHandler(myMessageOutput);
    NgramDataHub ngram_data_hub  = NgramDataHub();
    NgramPhrase& phrase_ptr = ngram_data_hub.getNgramPhraseRef();
    is_working = true;
    last_process_was_success = false;
    last_process_was_success = phrase_ptr.loadPhraseFromConfig(dir_path, filename);
    is_working = false;
    emit finished(last_process_was_success);
}
void NgramPhraseWorker::processNewPhrase(){
    //qInstallMessageHandler(myMessageOutput);
    NgramDataHub ngram_data_hub  = NgramDataHub();
    is_working = true;
    last_process_was_success = false;
    last_process_was_success = ngram_data_hub.newPhrase(ngram_phrase_data);
    is_working = false;
    emit finished(last_process_was_success);
}
void NgramPhraseWorker::processRationalizePhrase(){
    //qInstallMessageHandler(myMessageOutput);
    NgramDataHub ngram_data_hub  = NgramDataHub();
    is_working = true;
    last_process_was_success = false;
    last_process_was_success = ngram_data_hub.rationalizeNgramData(do_delete, puw_has_priority);
    is_working = false;
    emit finished(last_process_was_success);
}
