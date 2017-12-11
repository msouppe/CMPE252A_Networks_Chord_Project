/*
 *	Filename: main.cpp
 *	Author: Mariette Souppe
 */


#include <iostream>
#include <math.h>
#include "node.h"

#define BITLENGTH 8

using namespace std;


int main() {
	Node n0(63); // node_id == 63
	Node n1(5); // node_id == 5
	Node n2(20);
	n0.join(NULL); // the first node to join the Chord network.
	n0.insert(3, 3); // insert key = 3
	n1.join(&n0); // the second node join the Chord network.
	n2.join(&n1);
	n0.insert(5,78); // insert key = 5
	n1.find(5); // key query
	n1.remove(5);


	return 0;
}

