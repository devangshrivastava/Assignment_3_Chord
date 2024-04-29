#include <classes.h>
#include <openssl/sha.h>
#include <sstream>
Define_Module(Client);



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

       int task_id = rand()%total_node;
       while(check(result, task_id) && current_node == task_id) task_id = rand()%total_node;
       result[i].push_back(task_id);  // last element as ID
   }

   return result;
}

int next_to_send(int current_node, int destination, int total_node, int len){
   if(destination == (current_node-1) % total_node) return len ;
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
    int x = par("x");
    int len = num_server;

//
    int current_node = getId()-2; // Getting the module's own
    vector<vector<int>> arr_1 = gen_task(x, current_node, num_client);

    EV << "My module ID is: " << current_node << endl;

    if(current_node == 0){
        for(int i=0;i<arr_1.size();i++){
            int destination = arr_1[i][arr_1[0].size()-1];
            int next = next_to_send(current_node, destination, num_client, len);
            send_message(arr_1[i],next,1,destination, current_node);
            for(int j=0;j<arr_1[0].size();j++){
                EV<<arr_1[i][j]<<" ";
            }
            EV<<" --> first sending to: "<<next<<" \n";

        }
    }

    EV<<"\n\n";

    for (int i = 0; i < gateSize("out"); i++)
            EV << "Gate out[" << i << "] is connected to " << gate("out", i)->getPathEndGate()->getOwnerModule()->getFullPath() << endl;

    for (int i = 0; i < gateSize("in"); i++)
        EV << "Gate in[" << i << "] is connected to " << gate("in", i)->getPathEndGate()->getOwnerModule()->getFullPath() << endl;


    EV << ", totalClients=" << num_client << endl;
}



void Client::handleMessage(cMessage *msg){
       int len = num_server;
       int x = par("x");
       ClientMessage *clientMsg = dynamic_cast<ClientMessage *>(msg);
       int current_node = getId()-2;

       vector<int> v =  clientMsg->arr;
       int destination = clientMsg->destination;
       int initial = clientMsg->initial;
       int subtask_num = clientMsg->subtask_num;



       if(current_node != destination ){
           EV<<"Received by: "<<current_node<<" sending to: "<<destination<<"\n";
           int next = next_to_send(current_node, destination, num_client, len);
           send_message(v,next,subtask_num, destination, initial);
       }
       else if(current_node == destination && v.size() > 1){
           EV<<"Reached!! at "<<current_node<<" data--> ";
           for(int i=0;i<v.size()-1;i++) EV<<v[i]<<" ";
           EV<<"\n";
           int s = 0;
           for(int i=0;i<v.size()-1;i++) s += v[i];
           EV<<"sum: "<<s<<" sending to "<<initial<<"\n";
           int next = next_to_send(current_node, initial, num_client, len);
           send_message({s},next,subtask_num, initial, destination);
       }
       if(current_node == destination && v.size() == 1){
           sum += v[0];
           c_lock++;
       }

       if(c_lock == x){
           EV<<current_node<<" has received all messages with sum "<<sum<<"\n";
           finish();
       }

}
