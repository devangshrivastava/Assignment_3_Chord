


simple Client
{
    parameters:
        //        @display("i=block/routing");
        int totalServers;
        int totalClients;
        int x;
    gates:
        input in[];
        output out[];

    // You can access totalServers and totalClients in handleMessage method or other member functions
}




network Net
{
    parameters:
        int numServers = 3;
        int numClients = 8;
        int x = 10
        @display("bgb=585,399");
    //        @display("bgb=512,374");
    submodules:
//        s[numServers]: Server {
//            //            @display("p=100,100");
//            totalServers = parent.numServers; // Assign total number of servers to each server module
//            totalClients = parent.numClients; // Assign total number of clients to each server module
//        }
        c[numClients]: Client {
            //            @display("p=200,100");
            totalServers = parent.numServers; // Assign total number of servers to each client module
            totalClients = parent.numClients; // Assign total number of clients to each client module
            x = parent.x;
        }
    connections:

    // Connect each server to each client

            for i=0.. numClients-1 {
                c[i].out++ --> {  delay = 100ms; } --> c[(i+1)%numClients].in++;
            }
            
            for i=0.. numClients-1 {
                c[i].out++ --> {  delay = 100ms; } --> c[(i+2)%numClients].in++;
            }
            
            for i=0.. numClients-1 {
                c[i].out++ --> {  delay = 100ms; } --> c[(i+4)%numClients].in++;
            }
            
        
            for i=0.. numClients-1 {
                c[i].in++ <-- {  delay = 100ms; } <-- c[(i+1)%numClients].out++;
            }
}
