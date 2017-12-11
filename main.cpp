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
	n0.getFingerTable().prettyPrint();
	n1.getFingerTable().prettyPrint();
	n2.getFingerTable().prettyPrint();

//	cout << "\nAfter n1.join(&n0)\n\n";
//	n0.getFingerTable().prettyPrint();
//	n1.getFingerTable().prettyPrint();

	return 0;
}

