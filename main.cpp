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
	Node n0(63); // node_id == 5
	Node n1(50); // node_id == 63
	Node n2(10);
	n0.join(NULL); // the first node to join the Chord network.
//	n0.insert(15, 3); // insert key = 3
	n0.insert(15, 3);
	n0.insert(36, 3);
	n0.insert(100, 3);
	n0.insert(5, 3);
	n0.insert(54, 3);
	n1.join(&n0); // the second node join the Chord network.

	n1.insert(5, 100);
	n0.insert(3, 100);
	n1.insert(26, 100);
	n1.insert(6, 100);// insert key = 5
	n2.join(&n0);
	n0.find(54); // key query

	n0.prettyPrint();
	n1.prettyPrint();
	n2.prettyPrint();

	return 0;
}

