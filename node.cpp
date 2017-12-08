/*
 *	Filename: node.cpp
 *	Author: Mariette Souppe
 */
#include <iostream>
#include <stdint.h>
#include <math.h>
#include <map>
#include <set>
#include <vector>
#include <cmath>
#include "node.h"

#define BITLENGTH 3

using namespace std;

int counter = 0;

void FingerTable::set_start(uint8_t nodeId_){
	cout << endl << "INIT FingerTable - set_start()" << endl;
	for (int i = 1; i <= BITLENGTH; i++) {
		fingertable_data* table = new fingertable_data;
		// (n +2^(k-1)) mod 2^m, n = nodeId_, k = iterate, m = BITLENGTH
		table->start = fmod (((int)nodeId_ + (int)pow(2, i-1)) , (int)pow(2, BITLENGTH));
		this->fingerTable_[i] = table;
	}
}

void FingerTable::set_interval(uint8_t nodeId_){
	cout << endl << "INIT FingerTable - set_interval()" << endl;
	for (int i = 1; i <= BITLENGTH; i++) {
		if (i == BITLENGTH) {
			this->fingerTable_[i]->interval[0] = this->fingerTable_[i]->start;
			this->fingerTable_[i]->interval[1] = (int) nodeId_;
			break;
		}
		this->fingerTable_[i]->interval[0] = this->fingerTable_[i]->start;
		this->fingerTable_[i]->interval[1] = this->fingerTable_[i+1]->start;
	}
}

void FingerTable::set_successor(size_t index, Node* successor){
	cout << endl << "FingerTable::set_successor() " << endl;
	this->fingerTable_[index]->success = successor;
}

uint64_t FingerTable::getFingerTableData_start(size_t index) {
	return this->fingerTable_[index]->start;
}

Node* FingerTable::getFingerTableData_successor(size_t index) {
	cout << endl << "FingerTable::getFingerTableData_successor() " << endl;
	cout << "Start: " << this->fingerTable_[index]->start << endl;
	return this->fingerTable_[index]->success;
}

void FingerTable::prettyPrint() {
	cout << "\nStart \t Interval \t Successor" << endl;
	for (int i = 1; i <= BITLENGTH; i++) {
		cout << this->fingerTable_[i]->start << " \t\t [" << \
				this->fingerTable_[i]->interval[0] 	<< ", " << this->fingerTable_[i]->interval[1] << ") \t   " << \
			    this->fingerTable_[i]->success->getId() << endl;
	}
}

void Node::join(Node* node) {
	cout << endl << "Node::join()" << endl;
	if (node != NULL) {
		cout << "Adding new node into network" << endl;
		this->initNodesFingerTable(node);
		this->update_others();
	}
	else {
		cout << "First node in the network" << endl;
		for (int i = 1; i <= BITLENGTH; i++) {
			Node* tempNode = this;
			this->fingerTable_.set_successor(i,tempNode);
		}
	}
}

void Node::initNodesFingerTable(Node* node) {
	 this->fingerTable_.set_successor(1, node->find_successor(this->fingerTable_.getFingerTableData_start(1))); // finger[1].node = n'.find_successor(finger[1].start)
	 Node* successor = this->fingerTable_.getFingerTableData_successor(1); //finger[1].node
	 Node* predecessor = successor->find_predecessor(successor->getId()); // predecessor  = finger[1].node.predecessor
//	 successor->find_predecessor(successor->getId()) = this; // finger[1].node.predecessor = n

	 for (int i = 1; i < (BITLENGTH - 1); i++) { // from i=1 to m-1
		 int n_start = this->fingerTable_.getFingerTableData_start(i+1); // finger[i+1].start
		 int n_id = this->getId(); // n
		 int n_successor = this->fingerTable_.getFingerTableData_successor(i)->getId(); // finger[i].node
		 if ( n_start >= n_id && n_start < n_successor) { // if (n <= finger[i].start < finger[i].node)
			 // finger[i+1].node = finger[i].node
			 this->fingerTable_.set_successor(i+1, this->fingerTable_.getFingerTableData_successor(i));
		 }
		 else {
			 // finger[i+1].node = n'.find_successor(finger[i+1].start)
			 this->fingerTable_.set_successor(i+1, node->find_successor(this->fingerTable_.getFingerTableData_start(i+1)));
		 }
	 }
}

void Node::update_others(){
	for (int i = 1; i <= BITLENGTH; i++) { //for i=1 to m
		Node* p = find_predecessor(this->getId() - pow(2, i-1)); // p = find_predecessor(n - 2^(i-1))
		p->update_finger_table(this, i); // p.update_finger_table(n, i)
	}
}

void Node::update_finger_table(Node* node, int index) {
	int node_id = node->getId(); // s
	int this_id = this->getId(); // n
	int n_successor = this->fingerTable_.getFingerTableData_successor(index)->getId(); // finger[i].node

	if ( this_id <= node_id && node_id < n_successor ) { // if (n <= s < finger[i].node)
		this->fingerTable_.set_successor(index, node); // finger[i].node = s
		Node* p; // p = predecessor ??
		p->update_finger_table(node, index); // p.update_finger_table(s,index)
	}
}

uint8_t Node::find(uint8_t key) {
	// use fingertable?
	uint8_t yo = 0;
	return yo;
}

void Node::insert(uint8_t key) {
	// Insert in the fingertable?
}

void Node::remove(uint8_t key) {
	// Update fingertable
}

int Node::getId() {
	return (int)this->id_;
}

FingerTable Node::getFingerTable() {
	return this->fingerTable_;
}

Node* Node::find_successor(uint64_t id_){
	cout << endl << "Node::find_successor()" << endl;
	Node* predecessor = this->find_predecessor(id_); // n' = find_predecessor(id)
	return predecessor->fingerTable_.getFingerTableData_successor(1); //n'.successor
}

Node* Node::find_predecessor(uint64_t id_){
	cout << endl << "Node::find_predecessor()" << endl;
	Node* predecessor = this; // n' = n
	cout << "condition 1 compares:\t" << (uint64_t)predecessor->getId() << " < " << id_ << endl;
	cout << "condition 2 compares:\t" << id_ << " <= " << (uint64_t)predecessor->fingerTable_.getFingerTableData_successor(1)->getId() << endl;
	bool n_id= ((uint64_t)predecessor->getId() < id_);
	bool n_suc_id = (id_ <= (uint64_t)predecessor->fingerTable_.getFingerTableData_successor(1)->getId());
	cout << "WHILE condition 1: " <<   n_id << endl;
	cout << "WHILE condition 2: " << n_suc_id << endl;
	while (!( n_id  &&  n_suc_id)) { // while !(n' < id <= n'.successor)
		counter++;
		if (counter == 5) {
			cout << "Break out of find_predcessor()" << endl;
			break;
		}
		predecessor = predecessor->closest_preceding_finger(id_); // n' = n'.closet_preceding_finger(id)
		cout << "in while: " << predecessor->getId() << endl;
		n_id= ((uint64_t)predecessor->getId() < id_);
		n_suc_id = (id_ <= (uint64_t)predecessor->fingerTable_.getFingerTableData_successor(1)->getId());
//		break;
	}
	return predecessor; // return n
}

Node* Node::closest_preceding_finger(uint64_t id_){
	cout << endl << "***********Node::closest_preceding_finger" << endl;
	cout  << "n: " << this->getId() << "\t id: " << id_ << endl;
	for (int i = BITLENGTH; i >= 1; i--) { // for i =m to 1
		cout << "i: " << i << endl;
		cout << "condition 1: n= " << this->getId() << "\t < \tfingerT entry: " << \
				this->fingerTable_.getFingerTableData_successor(i)->getId() << endl;
		cout << "condition 2: id= " << id_ << "\t > \tfingerT entry: "  << \
				(this->fingerTable_.getFingerTableData_successor(i)->getId()) << endl;
		if (( this->getId() < this->fingerTable_.getFingerTableData_successor(i)->getId()) && \
				(id_ > this->fingerTable_.getFingerTableData_start(i))) { // if (n < finger[i].node < id)
			return this->fingerTable_.getFingerTableData_successor(i); // return finger[i].node
		}
	}
	return this; // return n
}

/*
Resources:
	https://www.cs.cmu.edu/~dga/15-744/S07/lectures/16-dht.pdf
	https://www.linuxjournal.com/article/6797
*/
