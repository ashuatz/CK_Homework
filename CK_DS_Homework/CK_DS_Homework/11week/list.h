#pragma once
#include "pch.h"
#include "object.h"

class Object;
class Node
{
public:
	Node* prev;
	Object* data;
	Node* next;

	Node() : prev(nullptr), next(nullptr), data(nullptr) {}
};

class LinkedList
{
private:
	Node* head;
	Node* tail;
	int m_count;

public:
	LinkedList() : head(nullptr), tail(nullptr), m_count(0) {}

	void PushFront(Object* obj)
	{
		Node* temp = new Node();
		temp->data = obj;

		if (head == nullptr)
		{
			head = temp;
			tail = temp;
		}
		else
		{
			temp->next = head;
			head->prev = temp;

			head = temp;
		}

		m_count++;
	}

	void PushBack(Object* obj)
	{
		Node* temp = new Node();
		temp->data = obj;

		if (tail == nullptr)
		{
			head = temp;
			tail = temp;
		}
		else if (head == tail)
		{
			head->next = temp;
			temp->prev = head;

			tail = temp;
		}
		else
		{
			temp->prev = tail;
			tail->next = temp;

			tail = temp;
		}

		m_count++;
	}

	void insert(int idx, Object* obj)
	{
		//index out of range exception.
		if (idx < 0)
		{
			PushFront(obj);
			return;
		}
		if (idx >= m_count)
		{
			PushBack(obj);
			return;
		}

		Node* it = head;
		for (int i = 0; i < idx; ++i)
		{
			it = it->next;
		}

		Node* temp = new Node();
		temp->data = obj;

		//insert to it's position
		temp->next = it;
		temp->prev = it->prev;

		if (it != head)
			it->prev->next = temp;

		it->prev = temp;
		m_count++;
	}

	Object* remove(int idx)
	{
		if (idx < 0 || idx >= m_count)
			return nullptr;

		Node* it = head;
		for (int i = 0; i < idx; ++i)
		{
			it = it->next;
		}

		Object* temp = it->data;

		//releaseConnection
		if (it == head)
		{
			head = it->next;

			if (head != nullptr)
				head->prev = nullptr;
			else
				tail = nullptr;

			delete it;
		}
		else if (it == tail)
		{
			tail = it->prev;

			if (tail != nullptr)
				tail->next = nullptr;
			else
				head = nullptr;

			delete it;
		}
		else
		{
			it->next->prev = it->prev;
			it->prev->next = it->next;

			delete it;
		}

		m_count--;

		return temp;
	}

	Object* remove(Object* target)
	{
		int idx = indexOf(target);
		return remove(idx);
	}

	const int indexOf(Object* target)
	{
		if (head == nullptr)
			return -1;

		Node* it = head;
		int idx = 0;
		while (it != nullptr)
		{
			if (it->data == target)
				return idx;

			idx++;
			it = it->next;
		}

		return -1;
	}

	Object* retrieve(int idx) const
	{
		if (idx >= m_count || idx < 0)
			return nullptr;

		Node* it = head;
		for (int i = 0; i < idx; ++i)
		{
			if (it == nullptr)
				return nullptr;

			it = it->next;
		}

		return it->data;
	}

	Object** removeAll(int& count)
	{
		Object** arr = new Object*[count];

		int idx = 0;
		Node* it = head;
		while (true)
		{
			arr[idx] = it->data;
			head = it->next;
			delete it;

			if (head == nullptr)
				break;

			it = head->next;
			idx++;
		}

		head = nullptr;
		tail = nullptr;
		m_count = 0;

		return arr;
	}

	const int Count() { return m_count; }
}; 