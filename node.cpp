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

#define BITLENGTH 8
#define success_node  fingerTable_.getFingerTableData_successor(1)

using namespace std;

bool PRINT_N_SEQ = false;

void FingerTable::set_start(uint8_t nodeId_){
	for (int i = 1; i <= BITLENGTH; i++) {
		fingertable_data* table = new fingertable_data;
		// (n +2^(k-1)) mod 2^m, n = nodeId_, k = iterate, m = BITLENGTH
		table->start = fmod (((int)nodeId_ + (int)pow(2, i-1)) , (int)pow(2, BITLENGTH));
		this->fingerTable_[i] = table;
	}
}

void FingerTable::set_interval(uint8_t nodeId_){
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
	this->fingerTable_[index]->success = successor;
}

uint8_t FingerTable::getFingerTableData_start(size_t index) {
	return this->fingerTable_[index]->start;
}

Node* FingerTable::getFingerTableData_successor(size_t index) {
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

void Node::prettyPrint() {
	cout <<"Node Id: " << this->getId();
	this->fingerTable_.prettyPrint();
	cout << "Predecessor: " << this->predecessor->getId()<<endl;
//	map<uint8_t, uint8_t>::const_iterator Printit;
//
//	for( Printit = localKeys_.begin(); Printit != localKeys_.end(); Printit++){
//		cout << "{" << (int)Printit->first << "," << (int)Printit->second << "} ";
//	  }
}

void Node::join(Node* node) {
	if (node != NULL) {
		cout <<"--------------------------------------------------------------------------\n";
		cout << " Adding new node into network, updating other nodes in network\n";
		cout <<"--------------------------------------------------------------------------\n\n";
		this->initNodesFingerTable(node);
		this->update_others();
		this->prettyPrint();
		// Move keys around
		cout << "Moved keys: ";
		Node* sucessor = this->fingerTable_.getFingerTableData_successor(1);
		map<uint8_t, uint8_t>::iterator it = sucessor->localKeys_.begin();
		while (it != sucessor->localKeys_.end()) {
			if(((int)it->first <= this->getId()) || ((int)it->first > sucessor->getId())) {
				cout << "{" << (int)it->first << "," << (int)it->second <<"} ";
				uint8_t tempFirst = it->first;
				uint8_t tempSecond = it->second;
				this->insert(tempFirst, tempSecond);
				sucessor->localKeys_.erase(it++);
			} else {
				++it;
			}
		}
		cout << "\n\n";
		node->prettyPrint();
	}
	else {
		for (int i = 1; i <= BITLENGTH; i++) {
			Node* tempNode = this;
			this->fingerTable_.set_successor(i,tempNode);
		}
		this->predecessor = this; // predecessor = n
		cout <<"-------------------------------\n";
		cout << "  First node in the network\n";
		cout <<"-------------------------------\n\n";
		this->prettyPrint(); // Print fingerTable of first node in the network
		cout<< "\n";
	}
}

void Node::initNodesFingerTable(Node* node) {
	 this->fingerTable_.set_successor(1, node->find_successor(this->fingerTable_.getFingerTableData_start(1))); // finger[1].node = n'.find_successor(finger[1].start)
	 Nodessuccessor = this->fingerTable_.getFingerTableData_successor(1);
	 predecessor = this->success_node->predecessor; // predecessor  = finger[1].node.predecessor
	 this->success_node->predecessor = this; // finger[1].node.predecessor = n
	 Node* tempSuccessor;
	 for (int i = 1; i <= BITLENGTH; i++) { // from i=1 to m-1
		 tempSuccessor = node->find_successor(this->fingerTable_. getFingerTableData_start(i));
		 if (tempSuccessor->getId() < (int)this->fingerTable_. getFingerTableData_start(i)) {
			 if ((int)this->fingerTable_. getFingerTableData_start(i) > this->getId())  {
				 this->fingerTable_.set_successor(i,this);
			 }
		 }
		 else{
			 this->fingerTable_.set_successor(i, tempSuccessor);
		 }
	 }
}

void Node::update_others(){
	Node* currentNode = this;
	do{
		currentNode->predecessor->fingerTable_.set_successor(1,currentNode);
		for (int i = 2; i <= BITLENGTH; i++) { //for i=1 to m
			int currentStart = currentNode->fingerTable_.getFingerTableData_start(i);
			Node* current_predecessor = currentNode->find_predecessor(currentStart);
			currentNode->fingerTable_.set_successor(i, current_predecessor->fingerTable_.getFingerTableData_successor(1));
		}
		currentNode = currentNode->predecessor;
	} while (currentNode != this);
}

void Node::update_finger_table(Node* node, int index) {
	int node_id = node->getId(); // s
	int this_id = this->getId(); // n
	int n_successor = node->fingerTable_.getFingerTableData_successor(index)->getId(); // finger[i].node
	if (node_id > n_successor) {
		if ((this_id < node_id && node_id <= pow(2,BITLENGTH)-1) || (0 <= node_id && node_id < n_successor)) { // wrap around
			this->fingerTable_.set_successor(index, node); // finger[i].node = s
			Node* p = this->predecessor;
			p->update_finger_table(node, index); // p.update_finger_table(s,index)
		}
	} else {
		if ( this_id <= node_id && node_id < n_successor ) { // if (n <= s < finger[i].node)
			this->fingerTable_.set_successor(index, node); // finger[i].node = s
			Node* p = this->predecessor;
			p->update_finger_table(node, index); // p.update_finger_table(s,index)
		}
	}
}

uint8_t Node::find(uint8_t key)  {
	PRINT_N_SEQ = true;
	cout << "\nNode Sequence to find key="<<(int)key<<": " << this->getId() << " ";
	Node* temp = this->find_successor(key);
	cout << temp->getId() << endl;
    auto search = temp->localKeys_.find(key);
    PRINT_N_SEQ = false;
    return search->second;
}

void Node::insert(uint8_t key,  uint8_t value) {
	Node* temp = this->find_successor(key);
	temp->localKeys_.insert({key, value});
}

void Node::remove(uint8_t key) {
	Node* temp = this->find_successor(key);
	temp->localKeys_.erase(key);
}

int Node::getId() {
	return (int)this->id_;
}

void Node::set_predecessor(Node* node) {
	this->predecessor = node;
}

Node* Node::get_predecessor() {
	return this->predecessor;
}

FingerTable Node::getFingerTable() {
	return this->fingerTable_;
}

Node* Node::find_successor(uint8_t id_){
	Node* n_ = this->find_predecessor(id_); // n' = find_predecessor(id)
	if (PRINT_N_SEQ) {
		cout << n_->getId() << " ";
	}
	if (n_->getId() == id_){
		return n_;
	}
	return n_->success_node; //n'.successor
}

Node* Node::find_predecessor(uint8_t id_){
	Node* p = this; // n' = n
	Node* previous = p;
	while (!(((uint8_t)p->getId() < id_)  &&  (id_ <= (uint8_t)p->success_node->getId()))) { // while !(n' < id <= n'.successor)
		p = p->closest_preceding_finger(id_); // n' = n'.closet_preceding_finger(id)
		if (previous->getId() == p->getId()) {
			break;
		}
		previous = p;
	}
	return p; // return n
}

Node* Node::closest_preceding_finger(uint8_t id_){
	for (int i = BITLENGTH; i >= 1; i--) { // for i = m to 1
		int n = this->getId( );
		int finger_node = (int)this->fingerTable_.getFingerTableData_successor(i)->getId();
		if (n > (int)id_) { // then wrap around
			if ((n < finger_node && finger_node <= pow(2,BITLENGTH)-1) || (0 <= finger_node && (uint8_t)finger_node < id_)) {
					return this->fingerTable_.getFingerTableData_successor(i); // return finger[i].node
			}
		}
		else {  // non- wrap around
			if ((n < finger_node) && ((uint8_t)finger_node < id_)) { // if (n < finger[i].node < id)
				return this->fingerTable_.getFingerTableData_successor(i); // return finger[i].node
			}
		}
	}
	return this; // return n
}
