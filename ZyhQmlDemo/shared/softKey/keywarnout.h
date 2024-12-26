#ifndef KEYWARNOUT_H
#define KEYWARNOUT_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QDateTime>
#include <QFile>
//#include <QCoreApplication>
#include <iostream>

class KeyWarnOut{
public:
    static void out(const QString &msg){

//        QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
//        QString message = QString("[%1] %2").arg(current_date_time).arg(msg);


//        std::cout<<message.toStdString()<<std::endl;

//        QFile file(qApp->applicationDirPath()+"/log.txt");
//        file.open(QIODevice::WriteOnly | QIODevice::Append);

//        QTextStream text_stream(&file);
//        text_stream << message << "\r\n";
//        file.flush();
//        file.close();
        qWarning()<<msg;
    }
};

#endif // KEYWARNOUT_H

