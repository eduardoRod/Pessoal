#pragma once

/*
*	Template for Double Connected List
*	
*	Find Methods must be specialized for each type
*/
template <class T>
class Node
{
friend class ListaLigadaBase<T>;

private:
	T _data;
	Node<T>* _next;
	Node<T>* _previous;

public:
	Node(T data);
	T getData();
};

template <class T>
class ListaLigadaBase
{
protected:
	Node<T>* _first;
	Node<T>* _last;
	int _size;

	T getDataByNode(Node<T>* node);
	int removeNode(Node<T>* node);
	bool isEmpty();

public:
	ListaLigada();
	~NoListaLigada();
	ListaLigadaBase operator= (ListaLigadaBase);
	ListaLigadaBase operator+ (ListaLigadaBase);
	ListaLigadaBase operator+= (ListaLigadaBase);

	int addNode();
	int addNode(T data);
	int insertNode(int index);
	int insertNode(int index, T& data);
	int removeNode(int index);
	int setDataInIndex(int index, T& data);
	T& getDataFromIndex(int index);
	int getSize();
};

template <class T, class searchParam>
class ListaLigada: public ListaLigadaBase<T>
{
public:
	int findByData(searchParam data, ListaLigada<T>& result);
};