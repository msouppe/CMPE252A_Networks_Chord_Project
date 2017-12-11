/*
 *	Filename: main.cpp
 *	Author: Mariette Souppe
 */


#include <iostream>
#include <math.h>
#include "node.h"

#define BITLENGTH 3

using namespace std;


int main() {

	Node n0(4);
	n0.join(NULL);
//	n0.getFingerTable().prettyPrint();
	Node n1(1);
	n1.join(&n0);
	Node n2(7);
	n2.join(&n1);
	cout << "======================" << endl << endl;
	n1.insert(3,3);
	n2.insert(6,45);
	n0.insert(0,3);
	n1.insert(4,3);
	n0.find(6);
	n2.remove(3);
	n0.prettyPrint();
	n1.prettyPrint();
	n2.prettyPrint();

//	cout << "\nAfter n1.join(&n0)\n\n";
//	n0.getFingerTable().prettyPrint();
//	n1.getFingerTable().prettyPrint();

	return 0;
}

