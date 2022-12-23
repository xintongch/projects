Xintong Chen
5746831810

Please run ./script
for a second run, please enter "killall Terminal"
and then reopen a new terminal and rerun ./script


1. what I do in the assignment
I did both the mandatory and optional part of the assignment. The optional part is interwoven with the mandatory part. So I will talk about them together.
The idea is for central to send both inputs from clientA and clientB to serverT. serverT has a graph object that store information about the edgelist. The graph object assign each name an ID, and use breadth first search to search all components. So you can find out if two names are in the same component. serverT can then get ID for each input name through the graph object and send back the ID, the adjacent lists of all ID in the target component, all ID-name pairs in the target component
Central then sends the ID-name pairs to serverS and get ID-score pairs. Then central sends all these information to serverP. ServerP has a "calculator" object that can take all these information and run depth first search to find the best path (with smallest matching gap). serverP send back the result in the final output form to Central and Central build up the final result for clients, then sends them to clientA and clientB who can directly print the received message on the standard output.

Notes about the extra credits assignment:
No matter how many inputs clientB types in, the only component that will be sent back to central is always the component that the clientA is within. 
For severP to calculate best path and smallest matching gap for more than one clientB inputs, it just needs to call multiple times of the "calculator" object's function for the multiple amount of clientB inputs.

Boundary conditions:
If the input name is not inside the graph database, then it is marked as a "NULL". If any clientB input is not in the same component as the clientA input, it is marked as a "NULL" too. But if the clientA's input is not in the graph, the message contains only a "NULL" and terminate.


2. What are my code files and what do they do
in addition to the following:
Makefile
readme.txt


There are 6 basic code files:
central.cpp
serverT.cpp
serverS
serverP
clientA.cpp
clientB.cpp

and (3+3=6) additional code files for three self defined classes:
graph.cpp
graph.h
scores.cpp
scores.h
calculator.cpp
calculator.h



What do they do:
central.cpp: Set up one TCP listening socket for clientA, one TCP listening socket for clientB, one UDP socket for server data exchange. Receive names from clientA and clientB and send the names to serverT. Retrieve names in the graph from serveT and send them to serverS. Get scores for all related names, and send the client inputs, graph, scores, dictionary (for mapping number and name) to serverP. Get the result from serverP and send them to clients.
serverT: Set up a UCP socket for server data exchange. Construct a graph object initialized by such as a "edgelist.txt" file. Send client inputs to get the component structure (in adjacent lists). Send the numbers, adjacent list, name number mapping (dictionary) too central.
serverS: Set up UDP socket for server data exchange. Construct a scores object initialized by "scores.txt" file. Send names from central to scores object and get all their respective scores and send them too central.
serverP: Set up one UDP socket. Construct a calculator object initialized by the data received from central (client inputs, adjacent lists, scores, name number mapping). Use the calculator object to calculator the smallest matching gap and best path for clientA input and every clientB inputs. Send the paths and gaps to central.
graph.h: declaration of class graph.
gragh.cpp: implementation of class graph. Object graph can be initialized by an edge list file and build a name number mapping, adjacent lists and components array (mark node in its own component). 
scores.h: declaration of class scores;
scores.cpp: implementation of class scores. Object graph can be initialized by such as a "scores.txt" file, and build up a mapping for name and score.
calculator.h: declaration of the class calculator
calculator.cpp: implementation of the class calculator. Object calculator can be initialized by some adjacent lists, scores array, name number mapping. Then, user can use the object to get smallest matching gap and the corresponding path for two nodes.





3. The format of all the messages exchanged
In the following, "header" is a fixed length field containing the number of times of sending the information exchange would take.

(1) when clientB types two inputs and both are connected to clientA input in the graph
clientA->central:
<inputA>

clientB->central: 
<inputB1> <inputB2> 

central->serverT: 
<inputA> <inputB1> <inputB2>

serverT->central: 
header <inputA's ID> <inputB1's ID> <inputB2's ID> ; <input/user's ID1> <adjacent ID1> | <input/user's ID2> <adjacent ID1> <adjacent ID2> <adjacent ID3> | <input/user's ID3> <adjacent ID1> <adjacent ID2>; <input/user's ID1> <corresponding name> <input/user's ID2> <corresponding name> <input/user's ID3> <corresponding name>

central->serverS: 
header <input/user's ID1> <corresponding name> <input/user's ID2> <corresponding name> <input/user's ID3> <corresponding name>

serverS->central:
header <input/user's ID1> <corresponding score> <input/user's ID2> <corresponding score> <input/user's ID3> <corresponding score>

central-serverP: 
header <inputA's ID> <inputB1's ID> <inputB2's ID>; <input/user's ID1> <adjacent ID1> | <input/user's ID2> <adjacent ID1> <adjacent ID2> <adjacent ID3> | <input/user's ID3> <adjacent ID1> <adjacent ID2>; <input/user's ID1> <corresponding score> <input/user's ID2> <corresponding score> <input/user's ID3> <corresponding score>;<input/user's ID1> <corresponding name> <input/user's ID2> <corresponding name> <input/user's ID3> <corresponding name>

serverP->central: 
header Found compatibility for <inputA> and <inputB1>: \n <INPUTA>---<USERY>---<USERX>---<INPUTB1> \n Matching Gap: <value1> \n | Found compatibility for <inputB1> and <inputA>: \n <INPUTB1>---<USERX>---<USERY>---<INPUTA> \n Matching Gap: <value1> \n| Found compatibility for <inputA> and <inputB2>: \n <INPUTA>---<USERW>---<USERZ>---<INPUTB2> \n Matching Gap: <value2> \n | Found compatibility for <inputB2> and <inputA>: \n <INPUTB2>---<USERZ>---<USERW>---<INPUTA> \n Matching Gap: <value2> \n|

central->clientA: 
Found compatibility for <inputA> and <inputB1>: \n <INPUTA>---<USERY>---<USERX>---<INPUTB1> \n Matching Gap: <value1> \nFound compatibility for <inputA> and <inputB2>: \n <INPUTA>---<USERW>---<USERZ>---<INPUTB2> \n Matching Gap: <value2> \n

central->clientB: 
Found compatibility for <inputB1> and <inputA>: \n <INPUTB1>---<USERX>---<USERY>---<INPUTA> \n Matching Gap: <value1> \nFound compatibility for <inputB2> and <inputA>: \n <INPUTB2>---<USERZ>---<USERW>---<INPUTA> \n Matching Gap: <value2> \n



(2) when clientB types two inputs and one of them is not connected to clientA in the graph.
clientA->central:
<inputA>

clientB->central: 
<inputB1> <inputB2> 

central->serverT: 
<inputA> <inputB1> <inputB2>

serverT->central: 
header <inputA's ID> NULL <inputB2's ID> ; <input/user's ID1> <adjacent ID1> | <input/user's ID2> <adjacent ID1> <adjacent ID2> <adjacent ID3> | <input/user's ID3> <adjacent ID1> <adjacent ID2>; <input/user's ID1> <corresponding name> <input/user's ID2> <corresponding name> <input/user's ID3> <corresponding name>

central->serverS: 
header <input/user's ID1> <corresponding name> <input/user's ID2> <corresponding name> <input/user's ID3> <corresponding name>

serverS->central:
header <input/user's ID1> <corresponding score> <input/user's ID2> <corresponding score> <input/user's ID3> <corresponding score>

central-serverP: 
header <inputA's ID> <inputB2's ID>; <input/user's ID1> <adjacent ID1> | <input/user's ID2> <adjacent ID1> <adjacent ID2> <adjacent ID3> | <input/user's ID3> <adjacent ID1> <adjacent ID2>; <input/user's ID1> <corresponding score> <input/user's ID2> <corresponding score> <input/user's ID3> <corresponding score>;<input/user's ID1> <corresponding name> <input/user's ID2> <corresponding name> <input/user's ID3> <corresponding name>

serverP->central: 
header Found compatibility for <inputA> and <inputB2>: \n <INPUTA>---<USERW>---<USERZ>---<INPUTB2> \n Matching Gap: <value2> \n | Found compatibility for <inputB2> and <inputA>: \n <INPUTB2>---<USERZ>---<USERW>---<INPUTA> \n Matching Gap: <value2> \n|

central->clientA: 
Found no compatibility for <INPUTA> and <INPUTB1>\nFound compatibility for <inputA> and <inputB2>: \n <INPUTA>---<USERW>---<USERZ>---<INPUTB2> \n Matching Gap: <value2> \n

central->clientB: 
Found no compatibility for <INPUTB1> and <INPUTA> \nFound compatibility for <inputB2> and <inputA>: \n <INPUTB2>---<USERZ>---<USERW>---<INPUTA> \n Matching Gap: <value2> \n



(3)if client A input is not in the graph
clientA->central:
<inputA>

clientB->central: 
<inputB1> <inputB2> 

central->serverT: 
<inputA> <inputB1> <inputB2>

serverT->central: 
NULL

central->clientA: 
Found no compatibility for <INPUTA> and <INPUTB1>\nFound no compatibility for <INPUTA> and <INPUTB2>\n

central->clientB: 
Found no compatibility for <INPUTB1> and <INPUTA> \nFound no compatibility for <INPUTB2> and <INPUTA> \n



(4)when none of clientB inputs are connected to clientA input in the graph
clientA->central:
<inputA>

clientB->central: 
<inputB1> <inputB2> 

central->serverT: 
<inputA> <inputB1> <inputB2>

serverT->central: 
header <inputA's ID> NULL NULL

central->clientA: 
Found no compatibility for <INPUTA> and <INPUTB1>\nFound no compatibility for <INPUTA> and <INPUTB2>\n

central->clientB: 
Found no compatibility for <INPUTB1> and <INPUTA> \nFound no compatibility for <INPUTB2> and <INPUTA> \n





5.idiosyncrasy
If any problem happens, could you please try waiting for some time or recompling using "make" command. Thank you so much.


4. Reused codes
I have reused codes from Beej's book. They are all commented in the source files.



6. Other information
If clientA types two inputs or more, only the first one will be sent to serverC.
If clientB types more than two inputs, it also works.


