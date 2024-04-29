/*
 * classes.h
 *
 *  Created on: 06-Apr-2024
 *      Author: devan
 */
#include <omnetpp.h>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include<ctime>
#include<set>
#include <utility>
#include <sstream>

using namespace std;
using namespace omnetpp;

#ifndef CLASSES_H_
#define CLASSES_H_
class Server_Client_Message:public cMessage
{
     public:
         int ans;
         int subtask_num;
         int task_num;
};

class ClientMessage:public cMessage
{
     public:
         vector<int> arr;
         int arr_len;
         int subtask_num;
         int initial;
         int destination;
};

class GossipMessage:public cMessage
{
     public:
         time_t t;
         int ans;
         int client_id;

};


class msg{
public:
    int hash;
    bool flag;
};



class Client : public cSimpleModule
{
    public:
        int num_subtasks = 0;
        int task_num = 0;
        int num_server ;
        int num_client ;
        int c_lock = 0;
        int g_lock=0;
        int sum = {0};
        int self=0;

        set<pair<int,int>> ml;
        void send_message(std::vector<int> arr, int to, int subtask_num, int destination, int initial)
        {
            ClientMessage *newm= new ClientMessage();
            newm->arr=arr;
            newm->subtask_num=subtask_num;
            newm->destination = destination;
            newm->initial = initial;
            send(newm, "out", to);
        }
        void send_gossip(int ans,time_t time, int destination, int initial)
        {
            GossipMessage *newm= new GossipMessage();
            newm->client_id=initial;
            newm->ans=ans;
            newm->t=time;
            send(newm, "out", destination);
        }

  protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};



#endif /* CLASSES_H_ */
