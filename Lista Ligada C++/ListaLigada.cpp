#pragma once

#include "ListaLigada.h"

/*
*	Node Declarations
*/
template <class T>
Node<T>::Node(T data=NULL)
{
	_data = data;
	_next = nullptr;
	_previous = nullptr;
}

template <class T>
T Node<T>::getData()
{
	return _data;
}

/*
*	ListaLigadaBase Declarations
*/
template <class T>
ListaLigadaBase<T>::ListaLigadaBase()
{
	_first = nullptr;
	_last = nullptr;
	_size = 0;
}

template <class T>
ListaLigadaBase<T>::~ListaLigadaBase()
{
	if(!isEmpty)
	{
		Node<T>* currentPtr = _first;
		Node<T>* tempPtr;
		
		while(currentPtr != nullptr)
		{
			tempPtr = currentPtr;
			currentPtr = currentPtr->_next;
			delete tempPtr;
			_size--;
		}
	}
	return _size;
}

template <class T>
T ListaLigadaBase<T>::getDataByNode(Node<T>* node)
{
	return node->getData();
}

template <class T>
int ListaLigadaBase<T>::removeNode(Node<T>* node)
{
	if(!isEmpty())
	{
		node->_previous->_next = node->_next;
		node->_next->_previous = node->_previous;
		delete node;
		_size--;
		return 0;
	}

	return 1;
}

template <class T>
bool ListaLigadaBase<T>::isEmpty()
{
	return (_size == 0 ? true : false);
}

template <class T>
int ListaLigadaBase<T>::addNode()
{
	Node<T>* newPtr = new Node<T>;
	Node<T>* currentPtr = _first;

	while(currentPtr->_next != nullptr)
	{
		currentPtr = currentPtr->_next;
	}

	currentPtr->_next = newPtr;
	newPtr->_previous = currentPtr;
	return ++_size;
}

template <class T>
int ListaLigadaBase<T>::addNode(T data)
{
	Node<T>* newPtr = new Node<T>(data);
	Node<T>* currentPtr = _first;

	while(currentPtr->_next != nullptr)
	{
		currentPtr = currentPtr->_next;
	}

	currentPtr->_next = newPtr;
	newPtr->_previous = currentPtr;
	return ++_size;
}

template <class T>
int ListaLigadaBase<T>::insertNode(int index)
{
	if(index <= _size)
	{
		if(index == 0)
		{
			Node<T>* newPtr = new Node<T>;

			newPtr->_next = _first;
			_first->_previous = newPtr;
			_first = newPtr;
			return 0;
		}else{
			Node<T>* newPtr = new Node<T>;
			Node<T>* currentPtr = _first;

			for(int i = 0; i <= index; i++)
			{
				currentPtr = currentPtr->_next;
			}

			currentPtr->_previous->_next = newPtr;
			newPtr->_previous = currentPtr->_previous;
			currentPtr->_revious = newPtr;
			newPtr->_next = currentPtr;

			_size++;
			return 0;
		}
	}
	return 1;
}