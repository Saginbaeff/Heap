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
bool FibonacciHeap<T>::IsEmpty(){
        if (head_ == nullptr) {
            return true;
        }
        else
        {
            return false;
        }
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
            new_node.get()->right_sibling_ = new_node;
            new_node.get()->left_sibling_ = new_node;
            head_ = new_node;
        }
        else{
            new_node.get()->right_sibling_ = head_.get()->right_sibling_;
            new_node.get()->left_sibling_ = head_;
            new_node.get()->right_sibling_.get()->left_sibling_ = new_node;
            head_.get()->right_sibling_ = new_node;
            if (head_.get()->key_ > new_node.get()->key_){
                head_ = new_node;
            }
        }
        return pointer;
}

template <typename T>
void FibonacciHeap<T>::Merge(FibonacciHeap *otherHeap){
        if(otherHeap->head_.get() != nullptr) {
            if (head_.get() == nullptr) {
                head_ = otherHeap->head_;
            }
            else{
                //std::cout<<head_->key_<<"\n";
                head_.get()->right_sibling_.get()->left_sibling_ = otherHeap->head_.get()->left_sibling_;
                otherHeap->head_.get()->left_sibling_.get()->right_sibling_ = head_.get()->right_sibling_;
                head_.get()->right_sibling_ = otherHeap->head_;
                otherHeap->head_.get()->left_sibling_ = head_;
                if(otherHeap->head_.get()->key_ < head_.get()->key_){
                    head_ = otherHeap->head_;
                }
                otherHeap->head_.reset();
            }
        }
}

template <typename T>
T FibonacciHeap<T>::ExtractMin(){
    if (IsEmpty()) {
        throw std::out_of_range("Couldn't extract min, the heap is empty!");
    }
    std::shared_ptr<Node> extract_node = head_;
        head_ = head_.get()->right_sibling_;
        if (head_ == extract_node){
            head_.reset();
        }
        extract_node.get()->right_sibling_.get()->left_sibling_ = extract_node.get()->left_sibling_;
        extract_node.get()->left_sibling_.get()->right_sibling_ = extract_node.get()->right_sibling_;
        if (extract_node.get()->child_.get() != nullptr) {
            FibonacciHeap *Heap = new FibonacciHeap;
            Heap->head_ = extract_node.get()->child_;
            Heap->head_.get()->parrent_.reset();
            extract_node.get()->child_.reset();
            Merge(Heap);
            delete Heap;
        }
        Consolidate();
        T extract = extract_node.get()->key_;
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
        current.get()->key_ = new_value;
        if(current.get()->parrent_ != nullptr){
            std::shared_ptr<Node> current_parrent = current.get()->parrent_;
            if (current_parrent.get()->child_ == current){
                if (current.get()->right_sibling_ != current){
                    current_parrent.get()->child_ = current.get()->right_sibling_;
                }
                else{
                    current_parrent.get()->child_.reset();
                }
            }
            current_parrent.get()->degree_--;
            current.get()->parrent_.reset();
            current.get()->right_sibling_.get()->left_sibling_ = current.get()->left_sibling_;
            current.get()->left_sibling_.get()->right_sibling_ = current.get()->right_sibling_;
            current.get()->left_sibling_ = current;
            current.get()->right_sibling_ = current;
            FibonacciHeap *Heap = new FibonacciHeap;
            Heap->head_ = current;
            Merge(Heap);
            delete Heap;
            if (current_parrent.get()->mark_){
                CascadingCut(current_parrent);
            }
            else {
                current_parrent.get()->mark_ = true;
            }
        }
        else{
            current.get()->key_ = new_value;
            if (head_.get()->key_ > new_value){
                head_ = current;
            }
        }

}

template <typename T>
void FibonacciHeap<T>::Consolidate(){
        vector<std::shared_ptr<Node> > buffer(8);
        std::shared_ptr<Node> current = head_;

        while (current.get() != nullptr){
            if(current.get()->right_sibling_ != current) {
                current.get()->right_sibling_.get()->left_sibling_ = current.get()->left_sibling_;
                current.get()->left_sibling_.get()->right_sibling_ = current.get()->right_sibling_;
                head_ = current.get()->right_sibling_;
                current.get()->right_sibling_ = current;
                current.get()->left_sibling_ = current;
            }
            else{
                head_.reset();
            }
            while (buffer[current->degree_].get() != nullptr){
                if (current.get()->key_ > buffer[current.get()->degree_].get()->key_) {
                    FibonacciHeap *Heap1 = new FibonacciHeap;
                    FibonacciHeap *Heap2 = new FibonacciHeap;
                    Heap1->head_ = buffer[current.get()->degree_].get()->child_;
                    Heap2->head_ = current;
                    Heap1->Merge(Heap2);
                    current.get()->parrent_ = buffer[current.get()->degree_];
                    current = Heap1->head_.get()->parrent_;
                    current.get()->child_ = Heap1->head_;
                    buffer[current.get()->degree_] = nullptr;
                    ++current.get()->degree_;
                    Heap1->head_ = nullptr;
                    delete Heap1;
                    delete Heap2;
                }
                else{
                    FibonacciHeap *Heap1 = new FibonacciHeap;
                    FibonacciHeap *Heap2 = new FibonacciHeap;
                    Heap1->head_ = current.get()->child_;
                    Heap2->head_ = buffer[current.get()->degree_];
                    Heap1->Merge(Heap2);
                    current.get()->child_ = Heap1->head_;
                    buffer[current.get()->degree_].get()->parrent_ = current;
                    current = Heap1->head_.get()->parrent_;
                    current.get()->child_ = Heap1->head_;
                    buffer[current.get()->degree_] = nullptr;
                    ++current.get()->degree_;
                    Heap1->head_ = nullptr;
                    delete Heap1;
                    delete Heap2;
                }
            }
            buffer[current.get()->degree_] = current;
            current = head_;
        }
        for(int i = 0; i < buffer.GetSize(); i++){
            if (buffer[i] != nullptr){
                FibonacciHeap *Heap = new FibonacciHeap;
                Heap->head_ = buffer[i];
                Heap->head_.get()->right_sibling_ = Heap->head_;
                Heap->head_.get()->left_sibling_ = Heap->head_;
                Merge(Heap);
                delete Heap;
            }
        }
}

template <typename T>
void FibonacciHeap<T>::CascadingCut(std::shared_ptr<Node> init_pointer){
        std::shared_ptr<Node> current = init_pointer;
        current.get()->mark_ = false;
        if(current.get()->parrent_ != nullptr){
            std::shared_ptr<Node> current_parrent = current.get()->parrent_;
            if (current_parrent.get()->child_ == current){
                if (current.get()->right_sibling_ != current){
                    current_parrent.get()->child_ = current.get()->right_sibling_;
                }
                else{
                    current_parrent.get()->child_.reset();
                }
            }
            current_parrent.get()->degree_--;
            current.get()->parrent_.reset();
            current.get()->right_sibling_.get()->left_sibling_ = current.get()->left_sibling_;
            current.get()->left_sibling_.get()->right_sibling_ = current.get()->right_sibling_;
            current.get()->left_sibling_ = current;
            current.get()->right_sibling_ = current;
            FibonacciHeap *Heap = new FibonacciHeap;
            Heap->head_ = current;
            Merge(Heap);
            delete Heap;
            if (current_parrent.get()->mark_){
                CascadingCut(current_parrent);
            }
            else {
                current_parrent.get()->mark_ = true;
            }
        }
}
