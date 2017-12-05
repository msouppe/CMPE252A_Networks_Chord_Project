/*
 *	Filename: node.h
 *  Author: Mariette Souppe
 */

#ifndef NODE_H_
#define NODE_H_
#include <iostream>
#include <stdint.h>
#include <map>
#include <set>
#include <vector>
#include <cmath>

#define BITLENGTH 3

using namespace std;

//forward declaration
class Node;

//Since the index uniquely determines the interval, only the successor needs to be maintained.
class FingerTable{
	struct fingertable_data {
		int start;
		int interval[2];
		Node* success;

		void structPrint() {
			cout << "start: " << start <<  endl;
//			cout << "start: " << start << "\tinterval: [" << interval[0] << "," << interval[1] << ")" << endl;
		}
	};

	public:
		FingerTable(uint8_t nodeId) {
			nodeId_ = nodeId;

			// According to Chord paper, the finger table starts from index=1
			fingerTable_.resize(BITLENGTH + 1);

			this->set_start(nodeId_);
			this->set_interval(nodeId_);

			cout << "FingerTable Constructor" << endl;
		}

		void set_start(uint8_t nodeId_);

		void set_interval(uint8_t nodeId_);

		void set_successor(size_t index, Node* successor){
			cout << "FingerTable::set_successor() " << endl;
			this->fingerTable_[index]->success = successor;
		}

		Node* getFingerTableData_successor(size_t index) {
			cout << "FingerTable::getFingerTableData_successor " << endl;
			cout << this->fingerTable_[index]->success << endl;
			return this->fingerTable_[index]->success;
		}

		uint64_t getFingerTableData_start(size_t index) {
			return this->fingerTable_[index]->start;
		}

		// note Node* was changed from uint8_t
		fingertable_data* get(size_t index) {
			return fingerTable_[index];
		}

		// TODO: complete print function
		void prettyPrint();

	private:
		uint8_t nodeId_;
		vector<fingertable_data*> fingerTable_;
};

class Node {
	public:
		Node(uint8_t id): id_(id), fingerTable_(id) {}

		//TODO: implement node join function
		/**
		 * @param node: the first node to contact with to initialize join process. If this is the first node to join the Chord network, the parameter is NULL.
		 */
		void join(Node* node);

		//TODO: implement DHT lookup
		uint8_t find(uint8_t key);

		//TODO: implement DHT key insertion
		// Insert key VALUE
		void insert(uint8_t key);

		//TODO: implement DHT key deletion
		void remove(uint8_t key);

		int getId() {
			return (int)this->id_;
		}

		FingerTable getFingerTable() {
			return this->fingerTable_;
		}

		// Extra helper functions
		void updateTable();

		// Ask node n to find id's successor
		Node* find_successor(uint64_t id_);

		// Ask node n to find id's predecessor
		Node* find_predecessor(uint64_t id_);

		// Return closest finger preceding id
		Node* closest_preceding_finger(uint64_t id_);

		// Check fingertable empty
//		bool check_table_empty();

	private:
		uint64_t id_;
		FingerTable fingerTable_;
		map<uint8_t, uint8_t> localKeys_; // ex) node 4, value "blah"
};

#endif /* NODE_H_ */
