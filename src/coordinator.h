#ifndef __GOBACKN_COORDINATOR_H_
#define __GOBACKN_COORDINATOR_H_

#include <omnetpp.h>
#include "Message_m.h"
#include <fstream>
#include <string>
#include "my_utils.h"


using namespace omnetpp;

class Coordinator : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
