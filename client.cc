#include <classes.h>
#include <openssl/sha.h>
#include <sstream>
Define_Module(Client);



bool check(vector<int> task, int task_id){
   int n = task.size();
   for (int i = 0; i < n-1; i++){
       if(task[i] == task_id) return true;
   }
   return false;
}

vector<int> rand_task(int x){
    vector<int> v;
    for(int i=0;i<x;i++){
        int a = rand()%200;
        while(check(v, a)) a = rand()%200;
        v.push_back(a);
    }
    return v;
}

vector<vector<int>> gen_task(int x, int current_node, int total_node){
   vector<vector<int>> result;
   for (int i = 0; i < x; i++){
       result.push_back(std::vector<int>());
       for (int j = 0; j < 5; j++)
           result[i].push_back(std::rand()%100);

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

    int current_node = getId()-2; // Getting the module's own
    vector<vector<int>> arr_1 = gen_task(x, current_node, num_client);

    EV << "My module ID is: " << current_node << endl;

//    if(current_node == 0){
        vector<int> task_id = rand_task(x);
        for(int i=0;i<arr_1.size();i++){
            int destination = task_id[i]%num_client;
            int next = next_to_send(current_node, destination, num_client, len);
            for(int j=0;j<arr_1[0].size();j++){
                EV<<arr_1[i][j]<<" ";
            }

            if(destination != current_node){
                EV<<"; destination:" <<destination<<"--> first sending to: "<<next<<" \n";
                send_message(arr_1[i],next,task_id[i],destination, current_node);
            }
            else{
                EV<<"; destination:" <<destination<<"--> first sending to itself \n";
                self++;
                sum = 0;
                for(int j=0;j<arr_1[0].size();j++) sum += arr_1[i][j];
            }


        }
//    }

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
       GossipMessage *gossipMsg = dynamic_cast<GossipMessage *>(msg);
       if(clientMsg){
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
               for(int i=0;i<v.size();i++) EV<<v[i]<<" ";
               int s = 0;
               for(int i=0;i<v.size();i++) s += v[i];
               EV<<"\n sum: "<<s<<" sending to "<<initial<<"\n";
               int next = next_to_send(current_node, initial, num_client, len);
               send_message({s},next,subtask_num, initial, destination);
           }
           if(current_node == destination && v.size() == 1){
               sum += v[0];
               c_lock++;
           }

           if(c_lock == x-self){
               EV<<current_node<<" has received all messages with sum "<<sum<<"\n";

               for(int i=0;i<gateSize("out");i++){
                   EV<<"sending gossip to: "<<gate("out", i)->getPathEndGate()->getOwnerModule()->getFullPath() << endl;
                   time_t t=time(nullptr);
                   send_gossip(sum,t,i,current_node);
               }
//               EV<<"\n finished!"<<endl;
//               finish();
           }
       }
       if(gossipMsg && ml.size() != num_client){

           int current_node = getId()-2;
           int client_id = gossipMsg->client_id;
           int ans= gossipMsg->ans;
           time_t t=gossipMsg->t;
           auto it = ml.find({ans,client_id});
           if(it== ml.end()){
               ml.insert(make_pair(ans,client_id));
               for(int i=0;i<gateSize("out");i++){
                   int destinationModuleID=gate("out", i)->getPathEndGate()->getOwnerModule()->getId();
                   if(destinationModuleID!=client_id){
                       EV<<"sending gossip to: "<<destinationModuleID << endl;
                       EV<<"gossip: time->"<<t<<" ans-->"<<ans<<" client_id-->"<<client_id<<endl;
                       send_gossip(ans,t,i,current_node);
                   }
               }
           }

           if(ml.size() ==num_client){
               EV<<current_node<<" has received all gossips\n finished!"<<endl;
//               for (const auto& element : ml) {
//                       EV<<element.first<<" client ID "<<element.second<<endl;
//                   }
               finish();
           }
       }


}



