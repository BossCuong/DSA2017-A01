/*
 * =========================================================================================
 * Name        : listLib.h
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 1 - Data structures and Algorithms - Fall 2017
 * =========================================================================================
 */

#ifndef A01_LISTLIB_H
#define A01_LISTLIB_H

#include <string>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

class DSAException {
    int     _error;
    string  _text;
public:

    DSAException() : _error(0), _text("Success") {}
    DSAException(int err) : _error(err), _text("Unknown Error") {}
    DSAException(int err, const char* text) : _error(err), _text(text) {}

    int getError() { return _error; }
    string& getErrorText() { return _text; }
};

template <class T>
struct L1Item {
    T data;
    L1Item<T> *pNext = NULL;
    L1Item() : pNext(NULL) {}
    L1Item(T &a) : data(a), pNext(NULL) {}
};

template <class T>
class L1List {
public:
    L1Item<T>   *_pHead = NULL;// The head pointer of linked list
    L1Item<T>   *_pTail = NULL;
    size_t      _size = 0;// number of elements in this list
public:
    L1List() : _pHead(NULL), _size(0) {}
    ~L1List();

    void    clean();
    bool    isEmpty() {
        return _pHead == NULL;
    }
    L1Item<T>* get_pHead()
    {
        return _pHead;
    }
    L1Item<T>*  get_pTail()
    {
        return _pTail;
    }
    size_t  getSize() {
        return _size;
    }

    T&      at(int i);
    T&      operator[](int i);

    bool    find(T& a, int& idx);
    bool    find(T& a, L1Item<T>* &_pIndex);
    int     insert(int i, T& a);
    int     remove(int i);

    int     push_back(T& a);
    int     insertHead(T& a);

    int     removeHead();
    int     removeLast();

    void    reverse();

    void    traverse(void (*op)(T&)) {
        L1Item<T>   *p = _pHead;
        while (p) {
            op(p->data);
            p = p->pNext;
        }
    }
    void    traverse(void(*op)(T&, void*), void* pParam) {
        L1Item<T>   *p = _pHead;
        while (p) {
            op(p->data, pParam);
            p = p->pNext;
        }
    }
    void    increasingSize();
};
template  <class T>
void L1List<T>::increasingSize()
{
    this->_size++;
}
/// List Destructor
template <class T>
L1List<T>::~L1List()
{
    if (_pHead)
    {
        while (_pHead)
        {
            L1Item<T>* _pTemp = _pHead;
            _pHead = _pHead->pNext;
            delete _pTemp;
        }
    }
}
/// List Indexer,Acess an item in list by index
/// Return data of item if success
template <class T>
T& L1List<T>::at(int i)
{
    if (_pHead == NULL || i < 0) throw new DSAException(-1, "Ds rong hoac sai index");
    L1Item<T>* _pRun = _pHead;
    while (_pRun && i > 0)
    {
        _pRun = _pRun->pNext;
        i--;
    }
    if(_pRun) return _pRun->data;
    else throw new DSAException(-1, "Vuot chi so index");
}
template <class T>
T& L1List<T>::operator[](int i)
{
    if (_pHead == NULL || i < 0) throw new DSAException(-1, "Ds rong hoac sai index");
    L1Item<T>* _pRun = _pHead;
    while (_pRun && i > 0)
    {
        _pRun = _pRun->pNext;
        i--;
    }
    if(_pRun) return _pRun->data;
    else throw new DSAException(-1, "Vuot chi so index");
}
///Find data in list
///Return true and return idx by reference if success
template<class T>
bool L1List<T>::find(T& a, int& idx)
{
    if(_pHead == NULL) throw new DSAException(-1, "Ds rong");

    idx = 0;
    L1Item<T>* _pRun = _pHead;
    while (_pRun)
    {
        if (_pRun->data == a) break;
        _pRun = _pRun->pNext;
        ++idx;
    }
    if (_pRun == NULL) return false;
    else return true;
}
///Find data in list
///Return true and return pointer to that element by reference if success
template  <class T>
bool L1List<T>::find(T& a, L1Item<T>* &_pIndex)
{
    if(_pHead == NULL) throw new DSAException(-1, "Ds rong");

    L1Item<T>* _pRun = _pHead;
    while (_pRun)
    {
        if (_pRun->data == a) break;
        _pRun = _pRun->pNext;
    }
    if (_pRun == NULL) return false;
    else
    {
        _pIndex = _pRun;
        return true;
    }
}
template <class T>
int L1List<T>::push_back(T &a) {
    if (_pHead == NULL) {
        _pHead = new L1Item<T>(a);
    } else {
        L1Item<T> *p = _pHead;
        while (p->pNext) p = p->pNext;
        p->pNext = new L1Item<T>(a);
    }
    _size++;
    return 0;
}
/// Insert item to the front of the list
/// Return 0 if success
template <class T>
int L1List<T>::insertHead(T &a) {
    L1Item<T>   *p = new L1Item<T>(a);
    p->pNext = _pHead;
    _pHead = p;
    _size++;
    return 0;
}

/// Remove the first item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeHead() {
    if (_pHead) {
        L1Item<T>* p = _pHead;
        _pHead = p->pNext;
        delete p;
        _size--;
        return 0;
    }
    return -1;
}

/// Remove the last item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeLast() {
    if (_pHead) {
        if (_pHead->pNext) {
            L1Item<T>* prev = _pHead;
            L1Item<T>* pcur = prev->pNext;
            while (pcur->pNext) {
                prev = pcur;
                pcur = pcur->pNext;
            }
            delete pcur;
            prev->pNext = NULL;
        }
        else {
            delete _pHead;
            _pHead = NULL;
        }
        _size--;
        return 0;
    }
    return -1;
}



#endif //A01_LISTLIB_H
