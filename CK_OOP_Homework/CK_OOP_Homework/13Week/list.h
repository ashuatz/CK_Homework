#pragma once
#include <iostream>

template <typename T>
class Node
{
public:
	Node* prev;
	T* data;
	Node* next;

	Node() : prev(nullptr), next(nullptr), data(nullptr) {}
};

template <typename T>
class LinkedList
{
private:
	Node<T>* head;
	Node<T>* tail;
	int m_count;

public:
	LinkedList() : head(nullptr), tail(nullptr), m_count(0) {}

	void PushFront(T* obj)
	{
		Node<T>* temp = new Node<T>();
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

	void PushBack(T* obj)
	{
		Node<T>* temp = new Node<T>();
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

	void insert(int idx, T* obj)
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

		Node<T>* it = head;
		for (int i = 0; i < idx; ++i)
		{
			it = it->next;
		}

		Node<T>* temp = new Node<T>();
		temp->data = obj;

		//insert to it's position
		temp->next = it;
		temp->prev = it->prev;

		if (it != head)
			it->prev->next = temp;

		it->prev = temp;
		m_count++;
	}

	T* remove(int idx)
	{
		if (idx < 0 || idx >= m_count)
			return nullptr;

		Node<T>* it = head;
		for (int i = 0; i < idx; ++i)
		{
			it = it->next;
		}

		T* temp = it->data;

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


	template <class _Predicate>
	T* remove(_Predicate _pred)
	{
		int idx = indexOf(_pred);
		return remove(idx);
	}

	template <>
	T* remove(T* target)
	{
		int idx = indexOf(target);
		return remove(idx);
	}


	template <class _Predicate>
	const int indexOf(_Predicate pred) const
	{
		if (head == nullptr)
			return -1;

		Node<T>* it = head;
		int idx = 0;
		while (it != nullptr)
		{
			if (pred(it->data))
				return idx;

			idx++;
			it = it->next;
		}

		return -1;
	}

	template <>
	const int indexOf(T* target) const
	{
		if (head == nullptr)
			return -1;

		Node<T>* it = head;
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

	T* retrieve(int idx) const
	{
		if (idx >= m_count || idx < 0)
			return nullptr;

		Node<T>* it = head;
		for (int i = 0; i < idx; ++i)
		{
			if (it == nullptr)
				return nullptr;

			it = it->next;
		}

		return it->data;
	}

	T** removeAll(const int& count)
	{
		T** arr = new T*[count];

		int idx = 0;
		Node<T>* it = head;
		while (true)
		{
			arr[idx] = it->data;
			head = it->next;
			delete it;

			if (head == nullptr)
				break;

			it = head;
			idx++;
		}

		head = nullptr;
		tail = nullptr;
		m_count = 0;

		return arr;
	}

	~LinkedList()
	{
		if (m_count == 0)
			return;

		int size = m_count;
		auto data = removeAll(size);

		for (int i = 0; i < size; ++i)
		{
			delete data[i];
		}

		delete[] data;
	}


	const int Count() const { return m_count; }

	friend std::ostream& operator<<(std::ostream& os, const LinkedList<T>& list)
	{
		Node<T>* it = list.head;
		while (it != nullptr)
		{
			os << *(it->data) << " ";
			it = it->next;
		}

		return os;
	}
}; 