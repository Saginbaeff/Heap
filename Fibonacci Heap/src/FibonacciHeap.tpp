#include <assert.h>
#include <stdexcept>
#include <memory>
#include <iostream>
#include "vector.h"

template <typename T>
FibonacciHeap<T>::FibonacciHeap(){
        head_ = nullptr;
}

template <typename T>
FibonacciHeap<T>::~FibonacciHeap(){
    while (!IsEmpty()) {
        ExtractMin();
    }
}

template <typename T>
bool FibonacciHeap<T>::IsEmpty(){
    return head_ == nullptr;
}

template <typename T>
T FibonacciHeap<T>::GetMin(){
    if (IsEmpty()) {
        throw std::out_of_range("Couldn't get min, the heap is empty!");
    }
    return head_->key_;
}

template <typename T>
typename FibonacciHeap<T>::Pointer FibonacciHeap<T>::Insert(T &init_value){
        std::shared_ptr<Node> new_node = std::shared_ptr<Node>(new Node(init_value));
        Pointer pointer (new_node);
        if (IsEmpty()) {
            new_node->right_sibling_ = new_node;
            new_node->left_sibling_ = new_node;
            head_ = new_node;
        }
        else{
            new_node->right_sibling_ = head_->right_sibling_;
            new_node->left_sibling_ = head_;
            new_node->right_sibling_->left_sibling_ = new_node;
            head_->right_sibling_ = new_node;
            if (head_->key_ > new_node->key_){
                head_ = new_node;
            }
        }
        return pointer;
}

template <typename T>
void FibonacciHeap<T>::Merge(FibonacciHeap &otherHeap){
        if(otherHeap.head_ != nullptr) {
            if (head_ == nullptr) {
                head_ = otherHeap.head_;
            }
            else{
                head_->right_sibling_->left_sibling_ = otherHeap.head_->left_sibling_;
                otherHeap.head_->left_sibling_->right_sibling_ = head_->right_sibling_;
                head_->right_sibling_ = otherHeap.head_;
                otherHeap.head_->left_sibling_ = head_;
                if(otherHeap.head_->key_ < head_->key_){
                    head_ = otherHeap.head_;
                }
            }
            otherHeap.head_.reset();
        }
}

template <typename T>
T FibonacciHeap<T>::ExtractMin(){
    if (IsEmpty()) {
        throw std::out_of_range("Couldn't extract min, the heap is empty!");
    }
    std::shared_ptr<Node> extract_node = head_;
        head_ = head_->right_sibling_;
        if (head_ == extract_node){
            head_.reset();
        }
        extract_node->right_sibling_->left_sibling_ = extract_node->left_sibling_;
        extract_node->left_sibling_->right_sibling_ = extract_node->right_sibling_;
        if (extract_node->child_ != nullptr) {
            FibonacciHeap *Heap = new FibonacciHeap;
            Heap->head_ = extract_node->child_;
            Heap->head_->parrent_.reset();
            extract_node->child_.reset();
            Merge(*Heap);
            delete Heap;
        }
        Consolidate();
        T extract = extract_node->key_;
        extract_node.reset();
        return extract;
}

template <typename T>
void FibonacciHeap<T>::Change(Pointer init_pointer, T& new_value){
    if (IsEmpty()){
        throw std::out_of_range("Couldn't change element, the heap is empty!");
    }
    if (init_pointer.point_.expired()) {
        throw std::out_of_range("Couldn't change element, it is already deleted!");
    }
    std::shared_ptr<Node> current = init_pointer.point_.lock();
    current->key_ = new_value;
    if(current->parrent_ != nullptr){
        std::shared_ptr<Node> current_parrent = current->parrent_;
        if (current_parrent->child_ == current){
            if (current->right_sibling_ != current){
                current_parrent->child_ = current->right_sibling_;
            }
            else{
                current_parrent->child_.reset();
            }
        }
        current_parrent->degree_--;
        current->parrent_.reset();
        current->right_sibling_->left_sibling_ = current->left_sibling_;
        current->left_sibling_->right_sibling_ = current->right_sibling_;
        current->left_sibling_ = current;
        current->right_sibling_ = current;
        FibonacciHeap *Heap = new FibonacciHeap;
        Heap->head_ = current;
        Merge(*Heap);
        delete Heap;
        if (current_parrent->is_marked_){
            CascadingCut(current_parrent);
        }
        else {
            current_parrent->is_marked_ = true;
        }
    }
    else{
        current->key_ = new_value;
        if (head_->key_ > new_value){
            head_ = current;
        }
    }

}

template <typename T>
void FibonacciHeap<T>::Consolidate(){
        vector<std::shared_ptr<Node> > buffer(8);
        std::shared_ptr<Node> current = head_;
        while (current != nullptr){
            if(current->right_sibling_ != current) {
                current->right_sibling_->left_sibling_ = current->left_sibling_;
                current->left_sibling_->right_sibling_ = current->right_sibling_;
                head_ = current->right_sibling_;
                current->right_sibling_ = current;
                current->left_sibling_ = current;
            }
            else{
                head_.reset();
            }
            while (buffer[current->degree_] != nullptr){
                FibonacciHeap *Heap1 = new FibonacciHeap;
                FibonacciHeap *Heap2 = new FibonacciHeap;
                if (current->key_ > buffer[current->degree_]->key_) {
                    Heap1->head_ = buffer[current->degree_]->child_;
                    Heap2->head_ = current;
                    current->parrent_ = buffer[current->degree_];
                }
                else{
                    Heap1->head_ = current->child_;
                    Heap2->head_ = buffer[current->degree_];
                    buffer[current->degree_]->parrent_ = current;
                }
                Heap1->Merge(*Heap2);
                current = Heap1->head_->parrent_;
                current->child_ = Heap1->head_;
                buffer[current->degree_].reset();
                ++current->degree_;
                Heap1->head_.reset();
                delete Heap1;
                delete Heap2;
            }
            buffer[current->degree_] = current;
            current = head_;
        }
        for(int i = 0; i < buffer.GetSize(); i++){
            if (buffer[i] != nullptr){
                FibonacciHeap *Heap = new FibonacciHeap;
                Heap->head_ = buffer[i];
                Heap->head_->right_sibling_ = Heap->head_;
                Heap->head_->left_sibling_ = Heap->head_;
                Merge(*Heap);
                delete Heap;
            }
        }
}

template <typename T>
void FibonacciHeap<T>::CascadingCut(std::shared_ptr<Node> init_pointer){
        std::shared_ptr<Node> current = init_pointer;
        current->is_marked_ = false;
        if(current->parrent_ != nullptr){
            std::shared_ptr<Node> current_parrent = current->parrent_;
            if (current_parrent->child_ == current){
                if (current->right_sibling_ != current){
                    current_parrent->child_ = current->right_sibling_;
                }
                else{
                    current_parrent->child_.reset();
                }
            }
            current_parrent->degree_--;
            current->parrent_.reset();
            current->right_sibling_->left_sibling_ = current->left_sibling_;
            current->left_sibling_->right_sibling_ = current->right_sibling_;
            current->left_sibling_ = current;
            current->right_sibling_ = current;
            FibonacciHeap *Heap = new FibonacciHeap;
            Heap->head_ = current;
            Merge(*Heap);
            delete Heap;
            if (current_parrent->is_marked_){
                CascadingCut(current_parrent);
            }
            else {
                current_parrent->is_marked_ = true;
            }
        }
}


