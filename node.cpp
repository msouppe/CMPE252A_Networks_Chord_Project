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
		for (int i = 1; i <= BITLENGTH; i++) {
			Node* tempNode = this;
			this->fingerTable_.set_successor(i,tempNode);
		}
		this->find_successor(getId());
	}
	else {
		cout << "First node in the network" << endl;
		for (int i = 1; i <= BITLENGTH; i++) {
			Node* tempNode = this;
			this->fingerTable_.set_successor(i,tempNode);
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
	cout << "condition 1 compares:\t" << (uint64_t)predecessor->getId() << "\t" << id_ << endl;
	cout << "condition 2 compares:\t" << id_ << "\t" << (uint64_t)predecessor->fingerTable_.getFingerTableData_successor(1)->getId() << endl;
	uint64_t n_id= ((uint64_t)predecessor->getId() < id_);
	uint64_t n_suc_id = (id_ <= (uint64_t)predecessor->fingerTable_.getFingerTableData_successor(1)->getId());
	cout << "condition 1: " <<   n_id << endl;
	cout << "condition 2: " << n_suc_id << endl;
	while ( !( n_id  &&  n_suc_id)) {
		predecessor = predecessor->closest_preceding_finger(id_);
		cout << "BREAK OUT OF WHILE LOOP" << endl;
		break;
	}
	return predecessor;
}

Node* Node::closest_preceding_finger(uint64_t id_){
	cout << endl << "Node::closest_preceding_finger" << endl;
	for (int i = BITLENGTH; i >= 1; i--) {
		cout << "this->getId() = " << this->getId() << endl;
		cout << "condition: " << ( this->getId() < this->fingerTable_.getFingerTableData_successor(i)->getId()) << endl;
		cout << "this->fingerTable_.getFingerTableData_successor(i)->getId() = " << this->fingerTable_.getFingerTableData_successor(i)->getId() << endl;
		if (( this->getId() < this->fingerTable_.getFingerTableData_successor(i)->getId()) && \
				(id_ > this->fingerTable_.getFingerTableData_successor(i)->getId())) {
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
