#ifndef __GOBACKN_NODE_H_
#define __GOBACKN_NODE_H_

#include "my_utils.h"
#include "logger.h"
#include "Message_m.h"
#include <omnetpp.h>
#include <vector>
#include <fstream>
#include <utility>
#include <bitset>
#include <map>
#include <string>

typedef int seq_nr;

using namespace omnetpp;
using namespace std;

class Node : public cSimpleModule
{
    Logger* logger;
    role myRole;

    cMessage* coordMsg;

    int current_msg_index; // index of the current message in the message queue
    seq_nr max_seq;
    seq_nr nextFrameToSend;
    seq_nr ackExpected;
    seq_nr frameExpected = 0;

    int receiverWindowSize = GO_BACK_N_RECEIVER_WS;
    int senderWindowSize;

    double processingTime;
    double LossProb;
    double timeout;
    double errorDelay;
    double duplicationDelay;
    double transmissionDelay;

    bool isNetworkLayerReady;

    map<seq_nr, Message*> timerMessages;

    vector<string> recData;
    vector<Message*> sender_msg_buffer;
    vector<pair<string, string>> data; //data.first -> error code e.g. 1011
                                       //data.second -> actual text
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    ~Node();

    void read_data();
    bool check_parity(Message *msg);
    char create_parity(const std::string &payload);
    Message* create_frame(string text, seq_nr seqNum);
    string stuff_byte(string text);
    string destuff_byte(const std::string &payload);
    void add_error(Message*msg);


    void sender(Message*msg, bool isSelfMessage);
    void start_timer(const seq_nr &seqNum);
    void stop_timer(const seq_nr &seqNum);
    void handle_error_messages(Message*msg, ErrorType typesOfError, double currentMsg);
    void resend_buffer();
    ErrorType check_error_type(string errorString, Message* msg);
    void update_buffer_state(const seq_nr &seqNum);
    void reset_timer();
    bool is_valid_ACK(const seq_nr &receivedAckNum);
    void from_network_layer(Message *msg, double& currentMsg);
    void handle_ACKs(Message *msg);
    void to_physical_layer(Message *msg);

    void receive(Message*msg,bool isSelfMessage);

    seq_nr decrement_seq_nr(seq_nr lastAck);
    void increment_seq_nr(seq_nr& currentSeqNum);
};

#endif
