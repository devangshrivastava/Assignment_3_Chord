def generate_ned_script(num_servers, num_clients, len, x):
    script = f"""\



simple Client
{{
    parameters:
        //        @display("i=block/routing");
        int totalServers;
        int totalClients;
        int x;
    gates:
        input in[];
        output out[];

    // You can access totalServers and totalClients in handleMessage method or other member functions
}}




network Net
{{
    parameters:
        int numServers = {num_servers};
        int numClients = {num_clients};
        int x = {x}
        @display("bgb=585,399");
    //        @display("bgb=512,374");
    submodules:

        c[numClients]: Client {{
            //            @display("p=200,100");
            totalServers = parent.numServers; // Assign total number of servers to each client module
            totalClients = parent.numClients; // Assign total number of clients to each client module
            x = parent.x;
        }}
    connections:

    // Connect each server to each client

"""
         

    a = 1
    for _ in range(len): 
        
        script += f"""\
            for i=0.. numClients-1 {{
                c[i].out++ --> {{  delay = 100ms; }} --> c[(i+{a})%numClients].in++;
            }}
            
"""
        a = a * 2
   
    script += f"""\
        
            for i=0.. numClients-1 {{
                c[i].in++ <-- {{  delay = 100ms; }} <-- c[(i+{1})%numClients].out++;
            }}
}}
"""
        
   


    return script

def write_ned_script(script):
    with open("net.ned", "w") as file:
        file.write(script)

# Example usage:

# num_servers = int(input("Enter the number of servers: "))
num_clients = int(input("Enter the number of clients: "))
x = int(input("Enter the value of x (greater than num_client as according to assignment): "))
temp = num_clients
len = 0

while(temp != 0):
    len = len + 1
    temp = temp // 2

# print("The length of the binary number is:", len)
# len = 5
num_servers = len-1
len = int(input("Enter the length of the binary number: "))
ned_script = generate_ned_script(num_servers, num_clients, len, x)
print(num_clients, num_servers, len)
write_ned_script(ned_script)
