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

vector<vector<int>> gen_task(int x, int current_node, int total_node){
   vector<vector<int>> result;

   for (int i = 0; i < x; i++){

       result.push_back(std::vector<int>());
       for (int j = 0; j < 5; j++)
           result[i].push_back(std::rand()%100);

       int task_id = rand()%x;
       while(check(result, task_id) && current_node == task_id) task_id = rand()%total_node;
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
    int current_node = getId()-2; // Getting the module's own
    vector<vector<int>> arr_1 = gen_task(3, current_node, num_client);

    EV << "My module ID is: " << current_node << endl;

    if(current_node == 0){

        EV<<"row "<<arr_1.size()<<"\n";
        EV<<"col "<<arr_1[0].size()<<"\n";

        for(int i=0;i<arr_1.size();i++){
            int destination = arr_1[i][arr_1[0].size()-1];
            int next = next_to_send(current_node, destination, num_client);
            send_message(arr_1[i],next,1);

            for(int j=0;j<arr_1[0].size();j++){
                EV<<arr_1[i][j]<<" ";
            }
            EV<<" --> first sending to: "<<next<<" \n";
//            EV<<"\n";
        }
        EV<<"\n\n";

         // arr, send_id, task_id



    }
    for (int i = 0; i < gateSize("out"); i++)
            EV << "Gate out[" << i << "] is connected to " << gate("out", i)->getPathEndGate()->getOwnerModule()->getFullPath() << endl;

    for (int i = 0; i < gateSize("in"); i++)
        EV << "Gate in[" << i << "] is connected to " << gate("in", i)->getPathEndGate()->getOwnerModule()->getFullPath() << endl;


    EV << "Client module initialized. totalServers=" << num_server << ", totalClients=" << num_client << endl;

}



void Client::handleMessage(cMessage *msg){

       ClientMessage *clientMsg = dynamic_cast<ClientMessage *>(msg);
       int current_node = getId()-2;
       vector<int> v =  clientMsg->arr;
       int destination = v[v.size()-1];
       int subtask_num = clientMsg->subtask_num;
       EV<<"Recieved by: "<<current_node<<" sending to: "<<destination<<"\n";
       if(current_node != destination){
           int next = next_to_send(current_node, destination, num_client);
           send_message(v,next,subtask_num);
       }
       else EV<<"Reached!! at "<<current_node<<"\n";


//       if (clientMsg && node_id >= num_server && t<2){
//          int client_id = msg->getArrivalGate()->getIndex();
//          rating.push_back(clientMsg->arr);
//          c_lock++;
//          EV << "Received server rating from client "<<client_id<<":\n";
//      }


}
