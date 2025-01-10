#include "coordinator.h"
using namespace std;

Define_Module(Coordinator);

void Coordinator::initialize()
{
    string startingNode, startingTime;
    ifstream newfile;

    newfile.open("coordinator.txt");
    Message *msg = new Message("Hi");
    newfile >> startingNode;
    newfile >> startingTime;
    newfile.close();
    msg->setPayload(startingNode.c_str());
    scheduleAt(simTime() + stod(startingTime), msg);
}

void Coordinator::handleMessage(cMessage *msg)
{
    Message *msg2 = check_and_cast<Message *>(msg);
    msg2->setMessageType(COORD_MSG);
    send(msg, "out", stoi(msg2->getPayload()));
}
