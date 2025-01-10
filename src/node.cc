#include "node.h"
using namespace std;

Define_Module(Node);

void Node::read_data()
{
    // creating file name (the two files are input0.txt and input1.txt)
    string fileName = "input" + to_string(getIndex()) + ".txt";

    ifstream inputFile;
    inputFile.open(fileName);

    string line, errorCode, text;

    // getting the the whole line
    while (getline(inputFile, line))
    {
        errorCode = line.substr(0, ERROR_CODE_LENGTH);
        text = line.substr(TEXT_START_INDEX, line.length() - TEXT_START_INDEX);
        data.push_back(make_pair(errorCode, text));
    }
    inputFile.close();
}


// For Error Detection
bool Node::check_parity(Message *msg)
{
    std::string payload = msg->getPayload();
    char trailer = msg->getTrailer();

    bitset<BYTE_LENGTH> parity(0);
    for (const auto& character : payload)
        parity = parity ^ bitset<BYTE_LENGTH>(character);

    // XORing parity with trailer
    parity = parity ^ bitset<BYTE_LENGTH>(trailer);

    if (parity.to_ulong() == 0)
        return true;
    else
        return false;
}

char Node::create_parity(const std::string &payload)
{
    bitset<BYTE_LENGTH> parity(0);
    for (const auto& character : payload)
        parity = parity ^ bitset<BYTE_LENGTH>(character);
    
    return (char)parity.to_ulong();
}


// Put a flag before each escape or flag encountered in the pay-load
string Node::stuff_byte(string text)
{
    string payload = "";
    for (int i = 0; i < text.length(); ++i)
    {
        if (text[i] == BYTE_STUFFING_FLAG || text[i] == BYTE_STUFFING_ESCAPE_CHARACTER)
            payload += BYTE_STUFFING_ESCAPE_CHARACTER;
        payload += text[i];
    }
    return BYTE_STUFFING_FLAG + payload + BYTE_STUFFING_FLAG;
}

// byte-stuff the data, calcualte parity, and then create the frame by combining the message type, payload, seqNum, and trailer
Message *Node::create_frame(string text, seq_nr seqNum)
{
    string payload = stuff_byte(text);
    char parityByte = create_parity(payload);

    Message *msg = new Message();
    msg->setMessageType(NORMAL_MSG);
    msg->setPayload(payload.c_str());
    msg->setSeqNum(seqNum);
    msg->setTrailer(parityByte);

    return msg;
}


string Node::destuff_byte(const std::string &payload)
{
    string message = "";
    for (int i = FLAG_BYTES_COUNT; i < payload.length() - FLAG_BYTES_COUNT; ++i) // loop, ignoring flag bytes at start and end
    {
        if (payload[i] == BYTE_STUFFING_ESCAPE_CHARACTER)
            i++;
        message += payload[i];
    }
    return message;
}


// Error Simulation
// Modify the message by flipping a random bit
void Node::add_error(Message *msg)
{
    string payload(msg->getPayload());
    int size = payload.length();
    bitset<BYTE_LENGTH> forerror(1);
    vector<bitset<BYTE_LENGTH>> vec;
    for (int i = 0; i < size; i++)
    {
        bitset<BYTE_LENGTH> xbits(payload[i]);
        vec.push_back(xbits);
    }

    int error = int(uniform(0, size));
    vec[error] = vec[error] ^ forerror;

    string collect = "";

    for (int it = 0; it < vec.size(); it++)
    {
        collect = collect + (char)(vec[it]).to_ulong();
    }

    msg->setPayload(collect.c_str());
}


// Receiver side
void Node::receive(Message *msg, bool isSelfMessage)
{

    if (!isSelfMessage)
    {
        if (msg->getSeqNum() == frameExpected) // if received frame is the frame expected
        {
            if (check_parity(msg)) // check for error
            {
                msg->setAckNum(frameExpected);
                increment_seq_nr(frameExpected);
                msg->setFrameType(ACK);
                recData.push_back(msg->getPayload());
            }
            else // if error is found send nack
            {
                msg->setAckNum(frameExpected);
                msg->setFrameType(NACK);
            }
            int Loss = int(uniform(0, MAX_LOSS_PROB));
            double x = (double)simTime().dbl();
            if (Loss >= LossProb) // check for loss probability
            {
                string s = to_string(msg->getAckNum());
                msg->setName(s.c_str());
                msg->setErrorType(CORRECT);
            }
            else // msg was lost
            {
                msg->setErrorType(LOSS);
            }
            scheduleAt(simTime() + processingTime, msg);
        }
        else // if the received frame wasn't the one expected, send ack for the last received frame
        {
            msg->setAckNum(decrement_seq_nr(frameExpected));
            double x = (double)simTime().dbl();
            msg->setFrameType(ACK);
            int Loss = int(uniform(0, MAX_LOSS_PROB));
            if (Loss >= LossProb)
            {
                string s = to_string(msg->getAckNum());
                msg->setName(s.c_str());
                msg->setErrorType(CORRECT);
            }
            else
            {
                msg->setErrorType(LOSS);
            }
            scheduleAt(simTime() + processingTime, msg);
        }
    }
    else
    {
        double x = (double)simTime().dbl();
        logger->log_control_frame(to_string(x), to_string(getIndex()), msg->getFrameType(), to_string(msg->getAckNum()), msg->getErrorType());
        if (msg->getErrorType() != LOSS)
        {
            sendDelayed(msg, transmissionDelay, "out");
        }
        else
        {
            delete msg;
        }
    }
}


// GO-BACK-N ARQ FUNCTIONS
void Node::increment_seq_nr(seq_nr &curr_seq_nr)
{
    curr_seq_nr = (curr_seq_nr + 1) % (max_seq + 1);
}

seq_nr Node::decrement_seq_nr(seq_nr last_ACK)
{
    return (last_ACK ? last_ACK - 1 : max_seq);
}

void Node::start_timer(const seq_nr &seqNum)
{
    if (timerMessages.find(seqNum) == timerMessages.end())
    {
        Message *timerMessage = new Message("TIMEOUT");

        timerMessage->setPayload(to_string(seqNum).c_str());
        timerMessage->setMessageType(TIMEOUT_MSG);

        timerMessages[seqNum] = timerMessage;

        scheduleAt(simTime() + timeout, timerMessage);
    }
}

void Node::reset_timer()
{
    for (auto it = timerMessages.begin(); it != timerMessages.end(); it++)
    {
        cancelAndDelete(it->second);
    }
    timerMessages.clear();
}

void Node::stop_timer(const seq_nr &seqNum)
{
    if (timerMessages[seqNum])
    {
        cancelAndDelete(timerMessages[seqNum]);
        timerMessages.erase(seqNum);
    }
}

ErrorType Node::check_error_type(string errorString, Message *msg)
{
    if (errorString[ERROR_CODE_LOSS_BIT] == '1')
        return LOSS;
    if (errorString[ERROR_CODE_MODIFICATION_BIT] == '1')
        add_error(msg);
    if (errorString[ERROR_CODE_DUPLICATION_BIT] == '1' && errorString[ERROR_CODE_DELAY_BIT] == '1')
        return DELAYED_AND_DUPLICATED;
    if (errorString[ERROR_CODE_DUPLICATION_BIT] == '1')
        return DUPLICATED;
    if (errorString[ERROR_CODE_DELAY_BIT] == '1')
        return DELAYED;
    return CORRECT;
}

void Node::handle_error_messages(Message *msg, ErrorType error_type, double currentMsg)
{
    if (error_type == LOSS)
    {
        msg->setErrorType(LOSS);
        scheduleAt(simTime() + processingTime * currentMsg, msg);
    }
    else if (error_type == DELAYED)
    {
        msg->setErrorType(DELAYED);
        scheduleAt(simTime() + processingTime * currentMsg, msg);
    }
    else if (error_type == DELAYED_AND_DUPLICATED)
    {
        Message *msgDup = new Message(*msg);
        msg->setErrorType(DELAYED);
        msgDup->setErrorType(DELAYED_AND_DUPLICATED);
        scheduleAt(simTime() + processingTime * currentMsg, msg);
        scheduleAt(simTime() + processingTime * currentMsg + duplicationDelay, msgDup);
    }
    else if (error_type == DUPLICATED)
    {
        Message *msgDup = new Message(*msg);
        msgDup->setErrorType(DUPLICATED);
        msg->setErrorType(CORRECT);
        scheduleAt(simTime() + processingTime * currentMsg, msg);
        scheduleAt(simTime() + processingTime * currentMsg + duplicationDelay, msgDup);
    }
    else if (error_type == CORRECT)
    {
        msg->setErrorType(CORRECT);
        scheduleAt(simTime() + processingTime * currentMsg, msg);
    }
}

// Resend all the messages in the sender buffer (after a frame or its ACK is lost)
void Node::resend_buffer()
{
    double currentMsg = 0.0;
    sender_msg_buffer[0]->setErrorString("0000"); // Reset error string for the first message
    for (int i = 0; i < sender_msg_buffer.size(); ++i)
    {
        if (sender_msg_buffer[i]->getMessageState() == WAITING)
            continue; // if this is a return statement, it works as well, but why?
        sender_msg_buffer[i]->setMessageState(WAITING);
        Message *msg_copy = new Message(*sender_msg_buffer[i]);
        currentMsg++;
        ErrorType error = check_error_type(sender_msg_buffer[i]->getErrorString(), msg_copy);
        handle_error_messages(msg_copy, error, currentMsg);
    }
}

// update the message state in the sender buffer (sliding window) to SENT
void Node::update_buffer_state(const seq_nr &seqNum)
{
    for (int i = 0; i < sender_msg_buffer.size(); ++i)
    {
        if (sender_msg_buffer[i]->getSeqNum() == seqNum)
        {
            sender_msg_buffer[i]->setMessageState(SENT);
            return;
        }
    }
}

void Node::to_physical_layer(Message *msg) 
{
    string s = msg->getPayload();
    update_buffer_state(msg->getSeqNum());

    start_timer(msg->getSeqNum());
    char duplicationIndex = msg->getErrorString()[2];
    if (msg->getErrorType() == DELAYED_AND_DUPLICATED || msg->getErrorType() == DUPLICATED)
    {
        duplicationIndex = '2';
    }
    logger->log_pretransmission(to_string((double)(simTime().dbl())), to_string(myRole), to_string(msg->getSeqNum()), msg->getPayload(), msg->getTrailer(),
                                msg->getErrorString()[0], msg->getErrorString()[1] - '0', duplicationIndex, msg->getErrorString()[3]);

    if (msg->getErrorType() == DELAYED || msg->getErrorType() == DELAYED_AND_DUPLICATED)
    {
        msg->setErrorType(CORRECT);
        sendDelayed(msg, errorDelay + transmissionDelay, "out");
    }
    else if (msg->getErrorType() == CORRECT || msg->getErrorType() == DUPLICATED)
    {
        msg->setErrorType(CORRECT);
        sendDelayed(msg, transmissionDelay, "out");
    }
    else if (msg->getErrorType() == LOSS)
        delete msg;
}


// check valid ACK by ensuring that the received ACK is in set of sequence numbers present in the sender buffer
bool Node::is_valid_ACK(const seq_nr &receivedAckNum)
{
    for (int i = 0; i < sender_msg_buffer.size(); ++i)
    {
        if (receivedAckNum == sender_msg_buffer[i]->getSeqNum())
            return true;
    }
    return false;
}

// cumulative ACK handling : for the received ACK, remove all the messages from the sender buffer that have a sequence number less than or equal to the received ACK
void Node::handle_ACKs(Message *msg){
    if (is_valid_ACK(msg->getAckNum()))
    {
        int index = 0;
        for (int i = 0; i < sender_msg_buffer.size(); ++i)
        {
            index++;
            stop_timer(sender_msg_buffer[i]->getSeqNum());
            increment_seq_nr(frameExpected);
            if (sender_msg_buffer[i]->getSeqNum() == msg->getAckNum())
            {
                delete sender_msg_buffer[i];
                break;
            }
            delete sender_msg_buffer[i];
        }
        sender_msg_buffer.erase(sender_msg_buffer.begin(), sender_msg_buffer.begin() + index);
    }
    delete msg;
}

// create a frame from the data and put it into the sender buffer
void Node::from_network_layer(Message *msg, double& currentMsg){
    msg = create_frame(data[current_msg_index].second, nextFrameToSend);
    currentMsg++;
    msg->setMessageState(WAITING);
    msg->setErrorString(data[current_msg_index].first.c_str());
    Message *newMsg = new Message(*msg);
    sender_msg_buffer.push_back(newMsg); // Put it into buffer before modification
    ErrorType error_type = check_error_type(data[current_msg_index].first, msg);
    if (currentMsg == 1)
    {
        logger->log_read_line(to_string((double)(simTime().dbl())), to_string(myRole), msg->getErrorString());
    }
    else
    {
        Message *printMessage = new Message(*msg);
        printMessage->setMessageType(PRINT_MSG);
        scheduleAt(simTime() + processingTime * (currentMsg - 1), printMessage);
    }
    handle_error_messages(msg, error_type, currentMsg);
}

// 
void Node::sender(Message *msg, bool isSelfMessage)
{
    if (isSelfMessage)
    {
        if (msg->getMessageType() == TIMEOUT_MSG)
        {
            logger->log_timeout(to_string((double)(simTime().dbl())), to_string(myRole), msg->getPayload());
            reset_timer();
            resend_buffer();
        }
        else if (msg->getMessageType() == PRINT_MSG)
        {
            logger->log_read_line(to_string((double)(simTime().dbl())), to_string(myRole), msg->getErrorString());
            delete msg;
        }
        else
        {
            to_physical_layer(msg);
        }
    }
    else
    {
        if (msg->getFrameType() == ACK)
            handle_ACKs(msg);
        else if (msg->getFrameType() == NACK)
            delete msg;
    }

    double currentMsg = 0.0;
    if (current_msg_index < data.size())
    {
        while (sender_msg_buffer.size() < senderWindowSize)
        {
            if (current_msg_index == data.size())
                break;
            from_network_layer(msg, currentMsg);
            current_msg_index++;
            increment_seq_nr(nextFrameToSend);
        }
    }
}

void Node::initialize()
{
    logger = new Logger();
    myRole = RECEIVER;
    processingTime = par("PT").doubleValue();
    LossProb = par("LP").doubleValue();
    timeout = par("TO").doubleValue();
    errorDelay = par("ED").doubleValue();
    duplicationDelay = par("DD").doubleValue();
    transmissionDelay = par("TD").doubleValue();
    senderWindowSize = par("windowSize").intValue();
    max_seq = senderWindowSize;
    current_msg_index = 0;
    ackExpected = 0;
    nextFrameToSend = 0;
    isNetworkLayerReady = true;
    frameExpected = 0;
}

void Node::handleMessage(cMessage *msg)
{
    Message *msg2 = check_and_cast<Message *>(msg);
    if (!msg->isSelfMessage())
    {
        if (msg2->getMessageType() == COORD_MSG && stoi(msg2->getPayload()) == getIndex())
        {
            coordMsg = msg;
            myRole = SENDER;
            read_data();
        }
    }
    if (myRole == SENDER)
        sender(msg2, msg->isSelfMessage());
    else
        receive(msg2, msg->isSelfMessage());
}


Node::~Node()
{
    delete logger;
}

/*
Notes:
isSelfMessage: a boolean that indicates whether the message being processed
is a self-message or not. In OMNeT++, a self-message is a message that a module schedules
to be sent to itself at a later time. This is often used for implementing timers or delayed
actions within the module.

*/