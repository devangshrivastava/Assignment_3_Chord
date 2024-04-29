Computer Networks:

Assignment 3
Overview of the Problem:
Utilize OMNeT++ Discrete Event Simulator to simulate remote program execution,
where each task is divided into x subtasks, each with ID i, which is randomly generated. Each
subtask with ID i is sent to the client with ID i%N, where N is the total number of clients in the
network. For this assignment, focus on executing a simple task, like finding the maximum element
in an array. Given an array of k elements, the client divides it into approximately x equal parts,
where k/x ≥ 2.

Implementation:
First we had created a network architecture made up of clients im a ring topology and used CHORD method to communicate from any node to another node in log(n) steps. 
Code Explanation:
1) Client.cc : This file contains declaration of Client module. Firstly client will divide task
into x>number of clients subtasks and then it will send nthem to the other clients.
3) “ClientMsg” and “GossipMessage” are message classes used for message transmission.
Network.ned file will be generated dynamically using ‘topo.txt’ configuration file.
Steps to execute the program:
1) Create topo.txt configuration file and then generate Network.ned file using
python script file script.py.
2) Set Values of variables numClients in omnet.ini file.
4) Build and Execute the simulation in Omnetpp environment.
