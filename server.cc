#include <classes.h>

Define_Module(Server);

void Server::initialize()
{

}

void Server::handleMessage(cMessage *msg){

    ClientMessage *m=check_and_cast<ClientMessage *>(msg);
    if (m) {

        int client_id = msg->getArrivalGate()->getIndex();
        EV << "Received from Client "<<client_id<<" subtask number: "<<m->subtask_num<<":\n";
        EV << "Array: ";
        for (int i = 0; i < m->arr.size(); ++i) {
            EV << m->arr[i] << ", ";
        }
        EV<<"\n";


        int s = 0;
        for(int i: m->arr) s += i;
//        *std::max_element(m->arr.begin(), m->arr.end());
        long treeId = msg->getTreeId();
        int serverId = getId();
        if(treeId%4 == serverId%4){ s = s - 1; EV<<"Reporting malicious ";}
        EV<<"Sum of element: "<<s<<"\n";

        send_message(s, m->subtask_num, client_id);
    }
    else EV << "Received a message of unexpected type\n";

}


