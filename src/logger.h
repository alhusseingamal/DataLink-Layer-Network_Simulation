#include <iostream>
#include <fstream>
#include <string>
#ifndef LOGSGENERATOR_H_
#define LOGSGENERATOR_H_

using namespace std;

static ofstream *logFile;
class Logger
{
public:
    Logger();
    virtual ~Logger();
    void log_read_line(string, string, string);
    void log_pretransmission(string startTime, string id, string seqNum, string payload, char trailer, char modified, int lost, char duplicate, char delay);
    void log_timeout(string, string, string);
    void log_control_frame(string, string, int, string, int);
};

#endif /* LOGSGENERATOR_H_ */
