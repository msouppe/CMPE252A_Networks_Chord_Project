Mariette Souppe
CMPE 252A Computer Networks
December 11, 2017
Course Project: Chord - Implementing a distributed hash table

Partner: None

  ----------------------------------
   Files Includes With This Project
  ----------------------------------
main.cpp	node.cpp	node.h

  ---------------------------------
   How To Run - "DHT_ChordNetwork"
  ---------------------------------

> Assuming you have a C++ complier setup and Eclipse Oxygen
git clone https://github.com/msouppe/DHT_ChordNetwork.git
open Eclipse Oxygen
import DHT_ChordNetwork
run

* I didn't test or run my project using the terminal, but if you 
already have that set up, then I believe just copying and pasting
main.cpp, node.cpp, and node.h will work just the same

  ---------------------------------
            Extra Notes
  ---------------------------------
> I programmed my project with the IDE Eclipse Oxygen, and not from the terminal
> I've included a sample int main, with my submitted files. 
> There is one bug that I was not able to figure out:
  When I initialize n0(5) before n1(63), there is an issue with the successors and printing out the table
  However, when I initialized the nodes in this pattern n0(63), n1(5), n2(20), the program will print everything correctly

  --------------------------------------
   Sample Output With My Provided Main
  --------------------------------------
Code:
	int main() {
		Node n0(63); // node_id == 63
		Node n1(5); // node_id == 5
		Node n2(20); // node_id == 20
		n0.join(NULL); // the first node to join the Chord network.
		n0.insert(3, 3); // insert key = 3
		n1.join(&n0); // the second node join the Chord network.
		n2.join(&n1); // the third node join the Chord netowrk.
		n0.insert(5,78); // insert key = 5
		n1.find(5); // key query
		n1.remove(5);
		return 0;
	}

Output: 
-------------------------------
  First node in the network
-------------------------------

Node Id: 63
Start 	 Interval 	 Successor
64 		 [64, 65) 	   63
65 		 [65, 67) 	   63
67 		 [67, 71) 	   63
71 		 [71, 79) 	   63
79 		 [79, 95) 	   63
95 		 [95, 127) 	   63
127 		 [127, 191) 	   63
191 		 [191, 63) 	   63
Predecessor: 63

----------------------------------------------------------------
 Adding new node into network, updating other nodes in network
----------------------------------------------------------------

Node Id: 5
Start 	 Interval 	 Successor
6 		 [6, 7) 	   63
7 		 [7, 9) 	   63
9 		 [9, 13) 	   63
13 		 [13, 21) 	   63
21 		 [21, 37) 	   63
37 		 [37, 69) 	   63
69 		 [69, 133) 	   5
133 		 [133, 5) 	   5
Predecessor: 63
Moved keys: {3,3} 

Node Id: 63
Start 	 Interval 	 Successor
64 		 [64, 65) 	   5
65 		 [65, 67) 	   5
67 		 [67, 71) 	   5
71 		 [71, 79) 	   5
79 		 [79, 95) 	   5
95 		 [95, 127) 	   5
127 		 [127, 191) 	   5
191 		 [191, 63) 	   5
Predecessor: 5

----------------------------------------------------------------
 Adding new node into network, updating other nodes in network
----------------------------------------------------------------

Node Id: 20
Start 	 Interval 	 Successor
21 		 [21, 22) 	   63
22 		 [22, 24) 	   63
24 		 [24, 28) 	   63
28 		 [28, 36) 	   63
36 		 [36, 52) 	   63
52 		 [52, 84) 	   63
84 		 [84, 148) 	   5
148 		 [148, 20) 	   5
Predecessor: 5
Moved keys: 

Node Id: 5
Start 	 Interval 	 Successor
6 		 [6, 7) 	   20
7 		 [7, 9) 	   20
9 		 [9, 13) 	   20
13 		 [13, 21) 	   20
21 		 [21, 37) 	   63
37 		 [37, 69) 	   63
69 		 [69, 133) 	   5
133 		 [133, 5) 	   5
Predecessor: 63

Node Sequence to find key=5: 5 5 5
