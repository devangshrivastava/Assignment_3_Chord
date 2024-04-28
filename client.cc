#include <classes.h>
#include <openssl/sha.h>
#include <sstream>
Define_Module(Client);

vector<int> random_server_list(int num_server) {
    vector<int> server_list;
    while (server_list.size() < (num_server / 2) + 1) {
        int server = std::rand() % num_server;
        if (find(server_list.begin(), server_list.end(), server) == server_list.end()) {
            server_list.push_back(server);
        }
    }
    return server_list;
}

bool check(vector<vector<int>> task, int task_id){
   int n = task.size();
   for (int i = 0; i < n-1; i++){
       if(task[i][5] == task_id) return false;
   }
   return true;
}

vector<vector<int>> gen_task(int x){
   vector<vector<int>> result;
   for (int i = 0; i < x; i++){
       result.push_back(std::vector<int>());
       for (int j = 0; j < 5; j++)
           result[i].push_back(std::rand()%100);
       int task_id = rand()%x;
       while(check(result, task_id)) task_id = rand()%x;
       result[i].push_back(task_id);  // last element as ID
   }
   return result;
}

int next_to_send(int current_node, int destination, int total_node){
   if(destination == (current_node-1) % total_node) return destination;
   int relative_dest = (destination - current_node + total_node) % total_node;
   int temp = relative_dest;
   int counter = -1;
   while(temp){
       temp /= 2;
       counter++;
   }
   return counter;
}


void Client::initialize(){

    num_server = par("totalServers");
    num_client = par("totalClients");

    task_result.resize(num_server, vector<int>(num_server, -1));
    scores.resize(num_server, 0);
    vector<vector<int>> arr_1 = gen_task(num_server);

    int moduleId = getId(); // Getting the module's own


    EV << "My module ID is: " << moduleId << endl;
    for (int i = 0; i < gateSize("out"); i++) {
            EV << "Gate out[" << i << "] is connected to " << gate("out", i)->getPathEndGate()->getOwnerModule()->getFullPath() << endl;
        }
    for (int i = 0; i < gateSize("in"); i++) {
        EV << "Gate in[" << i << "] is connected to " << gate("in", i)->getPathEndGate()->getOwnerModule()->getFullPath() << endl;
    }
        EV << "Client module initialized. totalServers=" << num_server << ", totalClients=" << num_client << endl;

}



void Client::handleMessage(cMessage *msg){
       if(t == 2) finish();
       ClientMessage *clientMsg = dynamic_cast<ClientMessage *>(msg);
       Server_Client_Message *ServerMsg =dynamic_cast<Server_Client_Message *>(msg);

       int node_id = msg->getArrivalGate()->getIndex();
       if (ServerMsg && node_id < num_server && t<2 ){
           int server_id = msg->getArrivalGate()->getIndex();
           int subtask_num = ServerMsg->subtask_num;
           EV << "Received Server Message from server"<<server_id<<": "<<ServerMsg ->ans<<"\n";

           task_result[subtask_num][server_id] = ServerMsg->ans;
           s_lock  += 1;
       }

       if (clientMsg && node_id >= num_server && t<2){
          int client_id = msg->getArrivalGate()->getIndex();
          rating.push_back(clientMsg->arr);
          c_lock++;
          EV << "Received server rating from client "<<client_id<<":\n";
      }


}
