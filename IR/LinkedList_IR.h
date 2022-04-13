//
// Created by SNAPDRAGON_11800H on 18/11/2021.
//

/* Generate new template class for block_List define in new */

#ifndef E5_MIPS_REWORK_LINKEDLIST_IR_H
#define E5_MIPS_REWORK_LINKEDLIST_IR_H

template<class T>
class LinkedList_IR {
public:
    T *h, *t;

    LinkedList_IR();

    void push_back(T *t);
};

template<class T>
void LinkedList_IR<T>::push_back(T *tail) {
    if (t) {
        t->next = tail;
        tail->prev = t;
        t = tail;
    } else {
        h = t = tail;
    }
}

template<class T>
LinkedList_IR<T>::LinkedList_IR() {
    h = t = nullptr;
}

#endif //E5_MIPS_REWORK_LINKEDLIST_IR_H
