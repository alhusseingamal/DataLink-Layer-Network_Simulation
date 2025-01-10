#include "logger.h"
#include <bitset>

Logger::Logger()
{
    logFile = new ofstream("output.txt");
}

Logger::~Logger()
{
    logFile->close();
}

void Logger::log_read_line(string startTime, string id, string errorCode)
{
    *logFile << "At time " << startTime << ", Node[" << id << "] : Introducing channel error with code = [" << errorCode << "]" << endl;
}

void Logger::log_pretransmission(string startTime, string id, string seqNum, string payload, char trailer,
                                           char modified, int lost, char duplicate, char delay)
{
    bitset<8> temp(trailer);
    string myLost;
    string isModified = "1";
    if (lost == 0)
        myLost = "No";
    else
        myLost = "Yes";
    if (modified == '0')
        isModified = "-1";

    *logFile << "At time " << startTime << ", Node[" << id << "] : sent frame with seq_Num[" << seqNum << "], Payload[" << payload << "]"
             << ", Trailer[" << temp.to_string() << "], Modified [" << isModified << "], Lost [" << myLost 
             << "] Duplicate [" << duplicate << "] Delay [" << delay << "]" << endl;
}

void Logger::log_timeout(string startTime, string id, string seqNum)
{
    *logFile << "Time out event at time " << startTime << ", at Node[" << id << "] for frame with seq_num = " << seqNum << endl;
}

void Logger::log_control_frame(string startTime, string id, int type, string seqNum, int loss)
{
    string myLoss;
    string ackType;
    if (loss == 0)
        myLoss = "No";
    else
        myLoss = "Yes";

    if (type == 1)
        ackType = "ACK";
    else
        ackType = "NACK";

    *logFile << "At time " << startTime << ", Node[" << id << "] sending " << ackType << " with number[" << seqNum << "], loss [" << myLoss << "]" << endl;
}
