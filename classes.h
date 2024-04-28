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
         int task_num;
};

class msg{
public:
    int from;
    int score;
    int hash;
    vector<int> to;
};

class Client : public cSimpleModule
{
    public:
        int num_subtasks = 0;
        int task_num = 0;
        int num_server ;
        int num_client ;
        int s_lock = 0;
        int c_lock = 0;
        int t = 0;

        vector<vector<int>> task_result;
        vector<vector<int>> rating;
        vector<int> scores;
        vector<msg> ml;
        void send_message(std::vector<int> arr, int arr_len, int server_id, int subtask_num)
        {
            ClientMessage *newm= new ClientMessage();
            newm->arr=arr;
            newm->arr_len=arr_len;
            newm->subtask_num=subtask_num;
            send(newm, "out", server_id);
        }

  protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};


class Server : public cSimpleModule
{
    public:
        void send_message(int ans, int subtask_num, int client_id)
        {
            Server_Client_Message *newm= new Server_Client_Message();
            newm->ans=ans;
//            newm->subtask_id=subtask_id;
            newm->subtask_num=subtask_num;
            send(newm, "out", client_id);
        }
  protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

#endif /* CLASSES_H_ */
