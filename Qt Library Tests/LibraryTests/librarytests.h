#ifndef LIBRARYTESTS_H
#define LIBRARYTESTS_H

#include "librarytests_global.h"
#include <iostream>
#include <vector>

using namespace std;

/* ============================================
 * Double connected tree class
 * ============================================*/
template<class T>
class DCTree
{
private:
    DCTree<T>* parent;
    vector< DCTree<T>* > child;
    T& data_;

public:
    DCTree();
    ~DCTree();

    T& getData(const DCTree<T>* child);
    int setData(const T& data);
    DCTree<T>* findChildByData(const T& data);
    int addChild(const T& data);
    int removeChild(const DCTree<T>* child);

};

class LIBRARYTESTSSHARED_EXPORT LibraryTests
{
public:
    LibraryTests();
    ~LibraryTests();
};

#endif // LIBRARYTESTS_H
