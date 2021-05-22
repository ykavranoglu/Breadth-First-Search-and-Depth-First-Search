#include <iostream>
#include<fstream>
#include<string>
#include<vector>
#include <chrono>

struct Node {
	std::vector<Node*> children;
	bool** matrix = NULL;
};

class Graph {
public:
	Graph(const unsigned&, unsigned&);
	void create_children(Node* &, const unsigned, unsigned&);
	void BFS(const unsigned*, const unsigned*, const unsigned*, const std::string, const unsigned, const unsigned, const unsigned, unsigned&, std::string);
	void DFS(const unsigned*, const unsigned*, const unsigned*, const std::string, const unsigned, const unsigned, const unsigned, unsigned&, std::string);
	void clear();
	void delete_node(Node* &);
private:
	Node* head;
	unsigned unique_char_size;
};

struct Q_Node{
	Node* value = NULL;	//points to the store graph node
	Q_Node* next = NULL;	//points to the next element in Queue
};

class Queue {
public:
	Queue();
	void push_back(Node*&);
	void insert(Node*&, unsigned);	//will be used for DFS
	Node*& return_val();	//returns the first priority element
	void dequeue();			//removes the first priority element from the queue. Must be used after return_val;
	bool is_empty();
	void clear();
private:
	Q_Node* head;
	Q_Node* tail;
};

unsigned find_col(bool** , unsigned);

bool same_checker(bool**, unsigned);




int main(int argc, char* argv[])
{
	std::string choice = argv[1];

	std::string first = argv[2];
	std::string second = argv[3];
	std::string result = argv[4];

	std::string out_name = argv[5];

	//these will be used as: "unique_chars[first_mapping[i]] + unique_chars[second_mapping[i]]" (check if they exist beforehand, first and second might have different lengths)
	unsigned first_size = first.length();
	unsigned second_size = second.length();
	unsigned result_size = result.length();

	unsigned* first_mapping = new unsigned[first_size];
	unsigned* second_mapping = new unsigned[second_size];
	unsigned* result_mapping = new unsigned[result_size];

	std::string file_read_name = first + " " + second + " " + result + ".txt";

	std::ifstream file_read;
	file_read.open(file_read_name);

	if (!file_read) {
		std::cerr << "File cannot be opened!";
		exit(1);
	}

	std::string character;
	std::string unique_chars;

	// "+ 32" is to compare upper-case letters with lower-case letters
	while (getline(file_read, character)) {
		if (getline(file_read, character, '\t')) {
			for (unsigned i = 0; i < first_size; i++) {
				if (character[0] == first[i] + 32) {
					first_mapping[first_size - 1 - i] = unique_chars.length();
				}
			}
			for (unsigned i = 0; i < second_size; i++) {
				if (character[0] == second[i] + 32) {
					second_mapping[second_size - 1 - i] = unique_chars.length();
				}
			}
			for (unsigned i = 0; i < result_size; i++) {
				if (character[0] == result[i] + 32) {
					result_mapping[result_size - 1 - i] = unique_chars.length();
				}
			}
			unique_chars += character;
		}
	}
	unsigned no_of_total_nodes = 0;
	Graph graph(unique_chars.length(), no_of_total_nodes);
	
	if (choice == "BFS") {
		graph.BFS(first_mapping, second_mapping, result_mapping, unique_chars, first.length(), second.length(), result.length(), no_of_total_nodes, out_name);
	}
	else if (choice == "DFS") {
		graph.DFS(first_mapping, second_mapping, result_mapping, unique_chars, first.length(), second.length(), result.length(), no_of_total_nodes, out_name);
	}


	graph.clear();
	delete[] first_mapping;
	delete[] second_mapping;
	delete[] result_mapping;
    return 0;
}

Graph::Graph(const unsigned& unique_char_size_init, unsigned& no_of_total_nodes)
	:unique_char_size(unique_char_size_init)
{

	head = new Node();
	no_of_total_nodes++;
	head->matrix = new bool* [unique_char_size];

	for (unsigned i = 0; i < unique_char_size; i++) {
		head->matrix[i] = new bool[10];
		for (unsigned j = 0; j < 10; j++) {
			head->matrix[i][j] = false;
		}
	}

	create_children(head, 0, no_of_total_nodes);
}

void Graph::create_children(Node*& node, const unsigned row, unsigned& no_of_total_nodes)
{
	Node* temp;
	for (unsigned i = 0; i < 10; i++) {
		bool constraint_flag = false;

		{	//this is to scope j variable name
		unsigned j = 0;
		while (!constraint_flag && (j < row)) {
			if (find_col(node->matrix, j) == i) {
				constraint_flag = true;
			}
			j++;
		}
		}	//this is to scope j variable name

		if (!constraint_flag) {
			temp = new Node();
			no_of_total_nodes++;
			temp->matrix = new bool* [unique_char_size];
			for (unsigned j = 0; j < unique_char_size; j++) {
				temp->matrix[j] = new bool[10];
				for (unsigned k = 0; k < 10; k++) {
					temp->matrix[j][k] = node->matrix[j][k];	//check: node->matrix[i][j] changed to node->matrix[j][k]
				}
			}
			temp->matrix[row][i] = true;
			if (row < unique_char_size - 1) {	//This if() ensures the termination of recursion	not sure about "< 9"
				create_children(temp, row + 1, no_of_total_nodes);
			}
			node->children.push_back(temp);
		}
	}
	

}

void Graph::BFS(const unsigned* first_mapping, const unsigned* second_mapping, const unsigned* result_mapping, const std::string unique_chars, const unsigned first_length, const unsigned second_length, const unsigned result_length, unsigned& no_of_total_nodes, std::string out_name)
{
	auto t1 = std::chrono::high_resolution_clock::now();

	unsigned no_of_visited = 0;

	Queue queue;
	queue.push_back(head);

	Node* temp = head;
	while (!queue.is_empty()) {
		temp = queue.return_val();
		queue.dequeue();
		no_of_visited++;
		
		if (!(temp->children.empty())) {
			unsigned i = 0;
			while (i < temp->children.size()) {
				bool dont_expand = false;
				
				if (first_mapping[first_length - 1] >= i) {	// might be ">" instead of ">="unique_char_size
					if (find_col(temp->matrix, first_mapping[first_length - 1]) == 0) {
						dont_expand = true;
					}
				}
				if (second_mapping[second_length - 1] >= i) {
					if (find_col(temp->matrix, second_mapping[second_length - 1]) == 0) {
						dont_expand = true;
					}
				}
				if (result_mapping[result_length - 1] >= i) {
					if (find_col(temp->matrix, result_mapping[result_length - 1]) == 0) {
						dont_expand = true;
					}
				}
				
				if (!dont_expand) {
					queue.push_back(temp->children[i]);
				}

				i++;
			}
		}
		else if(same_checker(temp->matrix, unique_char_size)){	// at the bottom of the graph, now we should check for solutions
			
			bool flag = true;
			unsigned i = 0;
			unsigned carry = 0;
			while ((i < first_length) && (i < second_length) && flag) {
				unsigned temp_value = find_col(temp->matrix, first_mapping[i]) + find_col(temp->matrix, second_mapping[i]) + carry;
				if (temp_value >= 10) {
					carry = 1;
				}
				else {
					carry = 0;
				}
				
				if ((temp_value % 10) == find_col(temp->matrix, result_mapping[i])) {
					i++;
				}
				else {
					flag = false;
				}
			}
			if (flag) {
				if (!(i < first_length) && !(i < second_length)) {	//first and second has the same length
					if (result_length > first_length) {	//result is longer than first and second
						if ((find_col(temp->matrix, result_mapping[i]) == carry) && (carry == 1)) {	//Solution Found
							queue.clear();
						}
					}
					else {	//result has the same length with first and second
						if (carry == 0) {	//Solution Found
							queue.clear();
						}
					}
				}
				else if (!(i < first_length)) {	//first is longer
					unsigned temp_value = find_col(temp->matrix, first_mapping[i]) + carry;
					if (temp_value >= 10) {
						carry = 1;
					}
					else {
						carry = 0;
					}

					if (result_length > first_length) {	//result is longer than first
						if ((temp_value % 10) == find_col(temp->matrix, result_mapping[i])) {
							i++;
							if ((find_col(temp->matrix, result_mapping[i]) == carry) && (carry == 1)) {	//Solution Found
								queue.clear();
							}
						}
					}
					else {	//result has the same length with first
						if ((temp_value % 10) == find_col(temp->matrix, result_mapping[i])) {	//Solution Found
							queue.clear();
						}
					}

				}
				else {	//second is longer
					unsigned temp_value = find_col(temp->matrix, second_mapping[i]) + carry;
					if (temp_value >= 10) {
						carry = 1;
					}
					else {
						carry = 0;
					}

					if (result_length > second_length) {	//result is longer than second
						if ((temp_value % 10) == find_col(temp->matrix, result_mapping[i])) {
							i++;
							if ((find_col(temp->matrix, result_mapping[i]) == carry) && (carry == 1)) {	//Solution Found
								queue.clear();
							}
						}
					}
					else {	//result has the same length with first
						if ((temp_value % 10) == find_col(temp->matrix, result_mapping[i])) {	//Solution Found
							queue.clear();
						}
					}
				}
			}
		}


	}
	auto t2 = std::chrono::high_resolution_clock::now();

	auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

	std::ofstream file_out;
	file_out.open(out_name);

	if (!file_out) {
		std::cerr << "File cannot be opened!";
		exit(1);
	}

	file_out << "\t0\t1\t2\t3\t4\t5\t6\t7\t8\t9\n";
	for (unsigned i = 0; i < unique_char_size; i++) {
		file_out << unique_chars[i];
		for (unsigned j = 0; j < 10; j++) {
			if (find_col(temp->matrix, i) == j) {
				file_out << "\t" << "1";
			}
			else {
				file_out << "\t" << ".";
			}
		}
		file_out << "\n";
	}

	std::cout << "Algorithm: DFS";
	std::cout << "\nNumber of the visited nodes: " << no_of_visited;
	std::cout << "\nMaximum number of nodes kept in the memory: " << no_of_total_nodes;
	std::cout << "\nRunning time(ms): " << ms_int.count();
	std::cout << "\nSolution: ";

	std::cout << unique_chars[0] << ": " << find_col(temp->matrix, 0);
	for (unsigned i = 1; i < unique_char_size; i++) {
		std::cout << ", " << unique_chars[i] << ": " << find_col(temp->matrix, i);
	}
}

void Graph::DFS(const unsigned* first_mapping, const unsigned* second_mapping, const unsigned* result_mapping, const std::string unique_chars, const unsigned first_length, const unsigned second_length, const unsigned result_length, unsigned& no_of_total_nodes, std::string out_name)
{
	auto t1 = std::chrono::high_resolution_clock::now();

	unsigned no_of_visited = 0;

	Queue queue;
	queue.push_back(head);

	Node* temp = head;
	while (!queue.is_empty()) {
		temp = queue.return_val();
		queue.dequeue();
		no_of_visited++;

		if (!(temp->children.empty())) {
			
			unsigned i = 0;
			while (i < temp->children.size()) {
				bool dont_expand = false;

				if (first_mapping[first_length - 1] >= i) {
					if (find_col(temp->matrix, first_mapping[first_length - 1]) == 0) {
						dont_expand = true;
					}
				}
				if (second_mapping[second_length - 1] >= i) {
					if (find_col(temp->matrix, second_mapping[second_length - 1]) == 0) {
						dont_expand = true;
					}
				}
				if (result_mapping[result_length - 1] >= i) {
					if (find_col(temp->matrix, result_mapping[result_length - 1]) == 0) {
						dont_expand = true;
					}
				}

				if (!dont_expand) {
					queue.insert(temp->children[i], i);	// This part makes it a DFS "1 + i" changed to "i"
				}
				
				i++;
			}
		}
		else if (same_checker(temp->matrix, unique_char_size)) {	// at the bottom of the graph, now we should check for solutions

			bool flag = true;
			unsigned i = 0;
			unsigned carry = 0;
			while ((i < first_length) && (i < second_length) && flag) {

				unsigned temp_value = find_col(temp->matrix, first_mapping[i]) + find_col(temp->matrix, second_mapping[i]) + carry;
				if (temp_value >= 10) {
					carry = 1;
				}
				else {
					carry = 0;
				}

				if ((temp_value % 10) == find_col(temp->matrix, result_mapping[i])) {
					i++;
				}
				else {
					flag = false;
				}
			}
			if (flag) {
				if (!(i < first_length) && !(i < second_length)) {	//first and second has the same length
					if (result_length > first_length) {	//result is longer than first and second
						if ((find_col(temp->matrix, result_mapping[i]) == carry) && (carry == 1)) {	//Solution Found
							queue.clear();
						}
					}
					else {	//result has the same length with first and second
						if (carry == 0) {	//Solution Found
							queue.clear();
						}
					}
				}
				else if (!(i < first_length)) {	//first is longer
					unsigned temp_value = find_col(temp->matrix, first_mapping[i]) + carry;
					if (temp_value >= 10) {
						carry = 1;
					}
					else {
						carry = 0;
					}

					if (result_length > first_length) {	//result is longer than first
						if ((temp_value % 10) == find_col(temp->matrix, result_mapping[i])) {
							i++;
							if ((find_col(temp->matrix, result_mapping[i]) == carry) && (carry == 1)) {	//Solution Found
								queue.clear();
							}
						}
					}
					else {	//result has the same length with first
						if ((temp_value % 10) == find_col(temp->matrix, result_mapping[i])) {	//Solution Found
							queue.clear();
						}
					}

				}
				else {	//second is longer
					unsigned temp_value = find_col(temp->matrix, second_mapping[i]) + carry;
					if (temp_value >= 10) {
						carry = 1;
					}
					else {
						carry = 0;
					}

					if (result_length > second_length) {	//result is longer than second
						if ((temp_value % 10) == find_col(temp->matrix, result_mapping[i])) {
							i++;
							if ((find_col(temp->matrix, result_mapping[i]) == carry) && (carry == 1)) {	//Solution Found
								queue.clear();
							}
						}
					}
					else {	//result has the same length with first
						if ((temp_value % 10) == find_col(temp->matrix, result_mapping[i])) {	//Solution Found
							queue.clear();
						}
					}
				}
			}
		}


	}
	auto t2 = std::chrono::high_resolution_clock::now();

	auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

	std::ofstream file_out;
	file_out.open(out_name);

	if (!file_out) {
		std::cerr << "File cannot be opened!";
		exit(1);
	}

	file_out << "\t0\t1\t2\t3\t4\t5\t6\t7\t8\t9\n";
	for (unsigned i = 0; i < unique_char_size; i++) {
		file_out << unique_chars[i];
		for (unsigned j = 0; j < 10; j++) {
			if (find_col(temp->matrix, i) == j) {
				file_out << "\t" << "1";
			}
			else {
				file_out << "\t" << ".";
			}
		}
		file_out << "\n";
	}

	std::cout << "Algorithm: DFS";
	std::cout << "\nNumber of the visited nodes: " << no_of_visited;
	std::cout << "\nMaximum number of nodes kept in the memory: " << no_of_total_nodes;
	std::cout << "\nRunning time(ms): " << ms_int.count();
	std::cout << "\nSolution: ";

	std::cout << unique_chars[0] << ": " << find_col(temp->matrix, 0);
	for (unsigned i = 1; i < unique_char_size; i++) {
		std::cout << ", " << unique_chars[i] << ": " << find_col(temp->matrix, i);
	}
}

void Graph::clear()	//this exists for more readable code, but basically uses delete_node()
{
	this->delete_node(head);
}

void Graph::delete_node(Node*& node)
{
	if (!(node->children.empty())) {
		unsigned i = 0;
		while (i < node->children.size()) {
			delete_node(node->children[i]);
			i++;
		}
	}
	for (unsigned i = 0; i < unique_char_size; i++) {
		delete[] node->matrix[i];
	}
	delete[] node->matrix;
	node->children.clear();
	delete node;
}

Queue::Queue()
	:head(NULL), tail(NULL)
{
}

void Queue::push_back(Node*& graph_node)
{
	Q_Node* new_node = new Q_Node();
	new_node->value = graph_node;

	if (!head) {
		head = new_node;
		tail = new_node;
		return;
	}

	tail->next = new_node;
	tail = new_node;
}

void Queue::insert(Node*& graph_node, unsigned index)
{
	Q_Node* new_node = new Q_Node();
	new_node->value = graph_node;

	if (index == 0) {
		new_node->next = head;
		if (head) {
			tail = head;
		}
		else {
			tail = new_node;
		}
		head = new_node;
		return;
	}

	Q_Node* temp = head;
	unsigned i = 0;
	while ((temp) && (i < index - 1)) {
		temp = temp->next;
		i++;
	}

	if (temp) {
		new_node->next = temp->next;
		temp->next = new_node;

		if (!(temp->next)) {
			tail = new_node;
		}
		return;
	}
	std::cerr << "Erronous insert operation!\n";
}

Node*& Queue::return_val()
{
	if (!head) {
		std::cerr << "Queue is empty. Can't return value.";
		exit(1);
	}
	
	return head->value;
}

void Queue::dequeue()
{
	if (!head) {
		std::cerr << "Queue is empty. Can't dequeue.";
		return;
	}

	if (head == tail) { //if queue only has 1 element
		tail = NULL;
	}

	Q_Node* temp;
	temp = head;
	head = head->next;
	delete temp;
}

bool Queue::is_empty()
{
	if (head) {
		return false;
	}
	return true;
}

void Queue::clear()
{
	if (!head) {
		return;
	}

	Q_Node* temp1 = head;
	Q_Node* temp2 = head;
	while (temp1 != tail) {
		temp1 = temp1->next;
		delete temp2;
		temp2 = temp1;
	}
	delete temp1;
	head = NULL;
	tail = NULL;
}

unsigned find_col(bool** matrix, unsigned row)
{
	for (unsigned i = 0; i < 10; i++) {
		if (matrix[row][i]) {
			return i;
		}
	}
	
	return 10;	// can be interpreted as error code
}

bool same_checker(bool** matrix, unsigned row_size)
{
	unsigned* positions = new unsigned[row_size];

	for (unsigned i = 0; i < row_size; i++) {
		positions[i] = 10;
	}

	for (unsigned i = 0; i < row_size; i++) {
		for (unsigned j = 0; j < 10; j++) {
			if (matrix[i][j] == true) {
				positions[i] = j;
			}
		}
	}

	for (unsigned i = 0; i < row_size; i++) {
		for (unsigned j = 0; j < row_size; j++) {
			if ((positions[i] == positions[j]) && (i != j)) {
				return false;
			}
		}
	}
	return true;
}
