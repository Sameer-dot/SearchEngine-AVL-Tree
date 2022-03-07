#include<iostream>
using namespace std;

template<class type>
class SNode {
public:
	SNode() {
		SNode* next = NULL;
	}
	SNode(type val, SNode<type>* nptr = 0) {
		data = val;
		next = nptr;
	}
	template<typename type>friend class Stack;
private:
	type data;
	SNode<type>* next;
};

template<class type>
class Stack {
public:
	Stack() {
		head = 0;
	}
	int size();
	bool isEmpty();
	type const& top();
	void pop();
	void push(type const& e);
private:
	SNode<type>* head;
	int Size;
};

template<class type>
int Stack<type>::size() {
	return Size;
}

template<class type>
bool Stack<type>::isEmpty() {
	if (head == NULL) {
		return true;
	}
	else {
		return false;
	}
}

template<class type>
type const& Stack<type>::top() {
	if (isEmpty()) {
		cout << "Stack is Empty!\n";
	}
	else {
		return head->data;
	}
}

template<class type>
void Stack<type>::push(type const& e) {
	head = new SNode<type>(e, head);
	Size++;
}

template<class type>
void Stack<type>::pop() {
	if (Size == 1) {
		delete head;
		head = NULL;
	}
	else {
		SNode<type>* temp = new SNode<type>;
		temp = head->next;
		delete head;
		head = temp;
	}
}