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

vector<vector<int>> gen_task(int num_server){
    vector<vector<int>> result;
    for (int i = 0; i < num_server; i++){
        result.push_back(std::vector<int>());
        for (int j = 0; j < 5; j++)
            result[i].push_back(std::rand()%100);
    }
    return result;
}


void Client::initialize(){

    num_server = par("totalServers");
    num_client = par("totalClients");

    task_result.resize(num_server, vector<int>(num_server, -1));
    scores.resize(num_server, 0);
    vector<vector<int>> arr_1 = gen_task(num_server);

    int moduleId = getId(); // Getting the module's own
//    for (int i = 0; i < num_server; i++){
//        for (int j = 0; j < 5; j++) EV<<arr_1[i][j]<<" ";
//        EV<<"\n";
//
//    }
//  EV << "Client " << moduleId - 2 << " send a message \n";
//    send_message({1},1,2,1);
    EV << "My module ID is: " << moduleId << endl;
    for (int i = 0; i < gateSize("out"); i++) {
            EV << "Gate out[" << i << "] is connected to " << gate("out", i)->getPathEndGate()->getOwnerModule()->getFullPath() << endl;
        }
    for (int i = 0; i < gateSize("in"); i++) {
        EV << "Gate in[" << i << "] is connected to " << gate("in", i)->getPathEndGate()->getOwnerModule()->getFullPath() << endl;
    }
        EV << "Client module initialized. totalServers=" << num_server << ", totalClients=" << num_client << endl;

}

// return total and score of server as well
int calculate_score(vector<vector<int>> &task_result, vector<int> &scores,int num_server){
    int s = 0;
    for(int i = 0; i < task_result.size(); i++){
        map<int,int> freq;
        int max_freq = -1;
        int max_freq_elem = -1;

        for(int j = 0; j < task_result[i].size(); j++){
            if(task_result[i][j] != -1){
                freq[task_result[i][j]]++;
            }
        }

        for(auto it = freq.begin(); it != freq.end(); it++){
            if(it->second >= max_freq){
                max_freq_elem = it->first;
                max_freq = it->second;
            }
        }
        for(int j = 0; j < task_result[i].size(); j++){
            if(task_result[i][j] == max_freq_elem && task_result[i][j] != -1){
                scores[j]++;
            }
        }
        s += max_freq_elem;
    }
    return s;
}

string vectorToString(const std::vector<int>& vec) {
    stringstream ss;
    for (int i : vec) {
        ss << i;
    }
    return ss.str();
}

vector<int> firstKHighestIndices(const vector<int>& vec, int k) {
    vector<int> indices(vec.size());
    for (int i = 0; i < indices.size(); ++i) {
        indices[i] = i;
    }


    partial_sort(indices.begin(), indices.begin() + k, indices.end(),[&vec](int a, int b) { return vec[a] > vec[b]; });

    return std::vector<int>(indices.begin(), indices.begin() + k);
}
//

//}

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
