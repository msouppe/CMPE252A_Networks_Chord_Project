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
	cout << endl << "FingerTable::getFingerTableData_successor " << endl;
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
		this->update_other_tables();
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
	 this->fingerTable_.set_successor(1, node->find_successor(this->fingerTable_.getFingerTableData_start(1)));
	 Node* successor = this->fingerTable_.getFingerTableData_successor(1);
	 Node* predecessor = successor->find_predecessor(successor->getId());
	 successor->find_predecessor(successor->getId()) = this;

	 for (int i = 1; i < (BITLENGTH - 1); i++) {
		 int n_start = this->fingerTable_.getFingerTableData_start(i+1);
		 int n_id = this->getId();
		 int n_successor = this->fingerTable_.getFingerTableData_successor(i)->getId();
		 if ( n_start >= n_id && n_start < n_successor) {
			 this->fingerTable_.set_successor(i+1, this->fingerTable_.getFingerTableData_successor(i));
		 }
		 else {
			 this->fingerTable_.set_successor(i+1, node->find_successor(this->fingerTable_.getFingerTableData_start(i+1)));
		 }
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
	Node* predecessor = this->find_predecessor(id_);
	return predecessor->fingerTable_.getFingerTableData_successor(1);
}

Node* Node::find_predecessor(uint64_t id_){
	cout << endl << "Node::find_predecessor()" << endl;
	Node* predecessor = this;
	cout << "condition 1 compares:\t" << (uint64_t)predecessor->getId() << " < " << id_ << endl;
	cout << "condition 2 compares:\t" << id_ << " <= " << (uint64_t)predecessor->fingerTable_.getFingerTableData_successor(1)->getId() << endl;
	bool n_id= ((uint64_t)predecessor->getId() < id_);
	bool n_suc_id = (id_ <= (uint64_t)predecessor->fingerTable_.getFingerTableData_successor(1)->getId());
	cout << "WHILE condition 1: " <<   n_id << endl;
	cout << "WHILE condition 2: " << n_suc_id << endl;
	while (!( n_id  &&  n_suc_id)) {
		counter++;
		if (counter == 5) {
			cout << "Break out of find_predcessor()" << endl;
			break;
		}
		predecessor = predecessor->closest_preceding_finger(id_);
		cout << "in while: " << predecessor->getId() << endl;
		n_id= ((uint64_t)predecessor->getId() < id_);
		n_suc_id = (id_ <= (uint64_t)predecessor->fingerTable_.getFingerTableData_successor(1)->getId());
//		break;
	}
	return predecessor;
}

Node* Node::closest_preceding_finger(uint64_t id_){
	cout << endl << "***********Node::closest_preceding_finger" << endl;
	cout  << "n: " << this->getId() << "\t id: " << id_ << endl;
	for (int i = BITLENGTH; i >= 1; i--) {
		cout << "i: " << i << endl;
		cout << "condition 1: n= " << this->getId() << "\t < \tfingerT entry: " << this->fingerTable_.getFingerTableData_successor(i)->getId() << endl;
		cout << "condition 2: id= " << id_ << "\t > \tfingerT entry: "  << (this->fingerTable_.getFingerTableData_successor(i)->getId()) << endl;
		if (( this->getId() < this->fingerTable_.getFingerTableData_successor(i)->getId()) && \
				(id_ > this->fingerTable_.getFingerTableData_start(i))) {
			cout << "INSIDE if condition Node::closest_preceding_finger" << endl;
			return this->fingerTable_.getFingerTableData_successor(i);
		}
	}
	return this;
}

/*
Resources:
	https://www.cs.cmu.edu/~dga/15-744/S07/lectures/16-dht.pdf
	https://www.linuxjournal.com/article/6797
*/
