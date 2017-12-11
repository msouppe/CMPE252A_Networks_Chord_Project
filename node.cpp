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
#define success_node  fingerTable_.getFingerTableData_successor(1)

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
	this->fingerTable_[index]->success = successor;
}

uint64_t FingerTable::getFingerTableData_start(size_t index) {
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
	this->fingerTable_.prettyPrint();
	cout << "Successor: " << this->fingerTable_.getFingerTableData_successor(1)->getId() << endl;
	cout << "Predecessor: " << this->predecessor->getId() << endl;
}

void Node::join(Node* node) {
	cout << endl << "Node::join()" << endl;
	if (node != NULL) {
		cout << "Adding new node into network: NodeId - " << this->getId() << endl;
		cout << "new node = " << this->getId() << " \t node in network = " << node->getId() << endl;
		this->initNodesFingerTable(node);
		node->prettyPrint();
		this->prettyPrint();
		cout << "@@@@@ BACK IN JOIN" << endl;
		this->update_others();
		node->prettyPrint();
		this->prettyPrint();
	}
	else {
		cout << "First node in the network: NodeId - " << this->getId() << endl;
		for (int i = 1; i <= BITLENGTH; i++) {
			Node* tempNode = this;
			this->fingerTable_.set_successor(i,tempNode);
		}
		this->predecessor = this; // predecessor = n
		cout << "predecessor\t" << this->predecessor->getId() << endl;
		this->fingerTable_.prettyPrint(); // print finger table when new node joins network
	}
}

void Node::initNodesFingerTable(Node* node) {
	 this->fingerTable_.set_successor(1, node->find_successor(this->fingerTable_.getFingerTableData_start(1))); // finger[1].node = n'.find_successor(finger[1].start)
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
	cout << endl << "@@@@@ Node::update_finger_table()" << endl;
	int node_id = node->getId(); // s
	cout << "node_id: " << node_id << endl;
	int this_id = this->getId(); // n
	cout << "this_id: " << this_id << endl;
	int n_successor = node->fingerTable_.getFingerTableData_successor(index)->getId(); // finger[i].node

	cout << ">>>>this_id <= node_id && node_id < n_successor <<<< " << endl;
	cout << ">>>>" << this_id << " <= " << node_id << " && " << node_id << " < " << n_successor << endl;
	if (node_id > n_successor) {
		if ((this_id < node_id && node_id <= pow(2,BITLENGTH)-1) || (0 <= node_id && node_id < n_successor)) { // wrap around
			this->fingerTable_.set_successor(index, node); // finger[i].node = s
			Node* p = this->predecessor;
			cout << "Update_finger_table p: " << p->getId() << "\tnode: "<< node->getId() << endl;
			p->update_finger_table(node, index); // p.update_finger_table(s,index)
		}
	} else {
		if ( this_id <= node_id && node_id < n_successor ) { // if (n <= s < finger[i].node)
			this->fingerTable_.set_successor(index, node); // finger[i].node = s
			Node* p = this->predecessor;
			cout << "Update_finger_table p: " << p->getId() << "\tnode: "<< node->getId() << endl;
			p->update_finger_table(node, index); // p.update_finger_table(s,index)
		}
	}
}

//uint8_t Node::find(uint8_t key)  {
//	Node* temp = this->find_successor(key);
//    auto search = temp->localKeys_.find(key);
//    if(search != temp->localKeys_.end()) {
//        return search->second;
//    }
//    return (uint8_t)-1;
//}

//void Node::insert(uint8_t key,  uint8_t value) {
//	Node* temp = this->find_successor(key);
//	temp->localKeys_.insert(key, value);
//}

//void Node::remove(uint8_t key) {
//	Node* temp = this->find_successor(key);
//	temp->localKeys_.erase(key);
//}

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

Node* Node::find_successor(uint64_t id_){
	cout << endl << "Node::find_successor()" << endl;
	Node* n_ = this->find_predecessor(id_); // n' = find_predecessor(id)
	cout << "Node::find_successor() RETURN VALUE \t" << (uint64_t)n_->success_node->getId() << endl;
	return n_->success_node; //n'.successor
}

Node* Node::find_predecessor(uint64_t id_){
	cout << endl << "Node::find_predecessor("<< id_ <<")" << "\t this_id: "<< this->getId()<< endl;
	Node* p = this; // n' = n
	Node* previous = p;
	int counter = 0;
	cout << (uint64_t)p->getId() << " < " <<  id_ <<  " <= " <<(uint64_t)p->success_node->getId() << endl;
	while (!(((uint64_t)p->getId() < id_)  &&  (id_ <= (uint64_t)p->success_node->getId()))) { // while !(n' < id <= n'.successor)
		p = p->closest_preceding_finger(id_); // n' = n'.closet_preceding_finger(id)
		cout << "p after closest_preceding_finger : " << p->getId() << endl;
//		counter++;
//		if (counter >= 5) {
//			break;
//		}
		if (previous->getId() == p->getId()) {
			cout << "find_predecessor: IF STATEMENT" << endl;
			break;
		}
		previous = p;
	}
	cout << "Node::find_predecessor() RETURN = " << (uint64_t)p->getId() << endl;
	return p; // return n
}

Node* Node::closest_preceding_finger(uint64_t id_){
	int CPFCounter = 0;
	cout << endl << "***********Node::closest_preceding_finger \t\t closest_precedig_finger_counter: " << CPFCounter <<endl;
	cout  << "n: " << this->getId() << "\t id: " << id_ << endl;
	for (int i = BITLENGTH; i >= 1; i--) { // for i = m to 1
		int n = this->getId( );
		this->prettyPrint();
		int finger_node = (int)this->fingerTable_.getFingerTableData_successor(i)->getId();
		cout << n << " < " << finger_node << " < " << id_ << endl;
		cout << "Interval: \t(" << n << "," << id_ << ")" << endl;
		cout << "n < finger_node :" << n << " < " << finger_node << "\t finger_node <= pow(2,BITLENGTH)-1 : " << finger_node << "<= " <<pow(2,BITLENGTH)-1 << endl;
		cout << "0 <= finger_node :" << "0" << " <= " << finger_node << "\t finger_node < id_ : " << finger_node << " < " << id_ << endl;
		if (n > (int)id_) { // then wrap around
			cout << "interval wraps around" << endl;
			if ((n < finger_node && finger_node <= pow(2,BITLENGTH)-1) || (0 <= finger_node && (uint64_t)finger_node < id_)) {
					return this->fingerTable_.getFingerTableData_successor(i); // return finger[i].node
			}
		}
		else {  // non- wrap around
			if ((n < finger_node) && ((uint64_t)finger_node < id_)) { // if (n < finger[i].node < id)
				return this->fingerTable_.getFingerTableData_successor(i); // return finger[i].node
			}
		}
	}
	return this; // return n
}
