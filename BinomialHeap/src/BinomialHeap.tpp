#include <assert.h>
#include <stdexcept>
#include <memory>
#include <iostream>

template <typename T>
void BinomialHeap<T>::SiftUp(std::shared_ptr<Node> init_Node){
    if (init_Node.get()->parrent_.get() != nullptr){
        if (init_Node.get()->parrent_.get()->key_ > init_Node.get()->key_){
            std::shared_ptr<Node> parrent_Node = init_Node.get()->parrent_;
            if (parrent_Node == head_){
                head_ = init_Node;
            }
            std::shared_ptr<Node> init_left_child = init_Node.get()->left_child_;
            std::shared_ptr<Node> init_left_sibling = init_Node.get()->left_sibling_;
            std::shared_ptr<Node> init_right_sibling = init_Node.get()->right_sibling_;
            init_Node.get()->left_sibling_ = parrent_Node.get()->left_sibling_;
            if (parrent_Node.get()->left_sibling_.get() != nullptr){
                parrent_Node.get()->left_sibling_.get()->right_sibling_ = init_Node;
            }
            init_Node.get()->right_sibling_ = parrent_Node.get()->right_sibling_;
            if (parrent_Node.get()->right_sibling_.get() != nullptr){
                parrent_Node.get()->right_sibling_.get()->left_sibling_ = init_Node;
            }
            init_Node.get()->parrent_ = parrent_Node.get()->parrent_;
            if (parrent_Node.get()->parrent_.get() != nullptr){
                if (parrent_Node.get()->parrent_.get()->left_child_ == parrent_Node){
                    parrent_Node.get()->parrent_.get()->left_child_ = init_Node;
                }
            }
            if (init_Node != parrent_Node.get()->left_child_) {
                init_Node.get()->left_child_ = parrent_Node.get()->left_child_;
            }
            else {
                init_Node.get()->left_child_ = parrent_Node;
            }

            parrent_Node.get()->left_child_ = init_left_child;
            if (init_left_child.get() != nullptr){
                init_left_child.get()->parrent_ = parrent_Node;
            }
            parrent_Node.get()->right_sibling_ = init_right_sibling;
            if (init_right_sibling.get() != nullptr){
                init_right_sibling.get()->left_sibling_ = parrent_Node;
            }
            parrent_Node.get()->left_sibling_ = init_left_sibling;
            if (init_left_sibling.get() != nullptr){
                init_left_sibling.get()->right_sibling_ = parrent_Node;
            }

            size_t parrent_degree = parrent_Node.get()->degree_;
            parrent_Node.get()->degree_ = init_Node.get()->degree_;
            init_Node.get()->degree_ = parrent_degree;

            std::shared_ptr<Node> current;
            current = init_Node.get()->left_child_;

            while (current.get() != nullptr){
                current.get()->parrent_ = init_Node;
                current = current.get()->right_sibling_;
            }

            current = parrent_Node.get()->left_child_;
            while (current.get() != nullptr){
                current.get()->parrent_ = parrent_Node;
                current = current.get()->right_sibling_;
            }

            SiftUp(init_Node);
        }
    }
}
template <typename T>
void BinomialHeap<T>::SiftDown(std::shared_ptr<Node> init_Node){
    if (init_Node.get()->left_child_.get() != nullptr){
        std::shared_ptr<Node> min_child = init_Node.get()->left_child_;
        std::shared_ptr<Node> current = init_Node.get()->left_child_;
        while (current.get() != nullptr){
            if (current.get()->key_ < min_child.get()->key_){
                min_child = current;
            }
            current = current.get()->right_sibling_;
        }
        if (min_child.get()->key_ < init_Node.get()->key_) {
            SiftUp(min_child);
            SiftDown(init_Node);
        }
    }
}

template <typename T>
BinomialHeap<T>::BinomialHeap(T &init_value) {
head_ = std::shared_ptr<Node>(new Node(init_value));
min_element_ = init_value;
}

template <typename T>
BinomialHeap<T>::BinomialHeap() {
    head_.reset();
    min_element_ = -1;
}

template <typename T>
BinomialHeap<T>::~BinomialHeap(){

}

template <typename T>
T BinomialHeap<T>::GetMin(){
    if (IsEmpty()) {
        throw std::out_of_range("Couldn't get min, the heap is empty!");
    }
    return min_element_;
}

template <typename T>
bool BinomialHeap<T>::IsEmpty(){
    if (head_.get() == nullptr) {
        return true;
    }
    else {
        return false;
    }
}

template <typename T>
void BinomialHeap<T>::Merge(BinomialHeap *otherHeap) {
    if (this->head_.get() != nullptr && otherHeap->head_.get() != nullptr) {
        BinomialHeap *Heap = new BinomialHeap;
        std::shared_ptr<Node> current_Heap1 = this->head_;
        std::shared_ptr<Node> current_Heap2 = otherHeap->head_;
        if (this->head_.get()->degree_ < otherHeap->head_.get()->degree_) {
            Heap->head_ = this->head_;
            current_Heap1 = current_Heap1.get()->right_sibling_;
        } else {
            Heap->head_ = otherHeap->head_;
            current_Heap2 = current_Heap2.get()->right_sibling_;
        }
        std::shared_ptr<Node> current_Heap = Heap->head_;
        while (current_Heap1.get() != nullptr && current_Heap2.get() != nullptr) {
            if (current_Heap1.get()->degree_ < current_Heap2.get()->degree_) {
                current_Heap.get()->right_sibling_ = current_Heap1;
                current_Heap1.get()->left_sibling_ = current_Heap;
                current_Heap1 = current_Heap1.get()->right_sibling_;
            } else {
                current_Heap.get()->right_sibling_ = current_Heap2;
                current_Heap2.get()->left_sibling_ = current_Heap;
                current_Heap2 = current_Heap2.get()->right_sibling_;
            }
            current_Heap = current_Heap.get()->right_sibling_;
        }
        if (current_Heap1.get() == nullptr) {
            while (current_Heap2.get() != nullptr) {
                current_Heap.get()->right_sibling_ = current_Heap2;
                current_Heap2.get()->left_sibling_ = current_Heap;
                current_Heap2 = current_Heap2.get()->right_sibling_;
                current_Heap = current_Heap.get()->right_sibling_;
            }
        }
        if (current_Heap2.get() == nullptr) {
            while (current_Heap1.get() != nullptr) {
                current_Heap.get()->right_sibling_ = current_Heap1;
                current_Heap1.get()->left_sibling_ = current_Heap;
                current_Heap1 = current_Heap1.get()->right_sibling_;
                current_Heap = current_Heap.get()->right_sibling_;
            }
        }
        current_Heap = Heap->head_;
        std::shared_ptr<Node> pre_current_Heap;
        pre_current_Heap.reset();
        while (current_Heap.get()->right_sibling_.get() != nullptr) {
            if (current_Heap.get()->degree_ == current_Heap.get()->right_sibling_.get()->degree_) {
                if (current_Heap.get()->key_ < current_Heap.get()->right_sibling_.get()->key_) {
                    std::shared_ptr<Node>temporary = current_Heap.get()->right_sibling_.get()->right_sibling_;
                    current_Heap.get()->right_sibling_.get()->right_sibling_ = current_Heap.get()->left_child_;
                    if (current_Heap.get()->left_child_.get() != nullptr) {
                        current_Heap.get()->left_child_.get()->left_sibling_ = current_Heap.get()->right_sibling_;
                    }
                    current_Heap.get()->right_sibling_.get()->left_sibling_.reset();
                    current_Heap.get()->left_child_ = current_Heap.get()->right_sibling_;
                    current_Heap.get()->left_child_.get()->parrent_ = current_Heap;
                    current_Heap.get()->right_sibling_ = temporary;
                    if (temporary.get() != nullptr) {
                        temporary.get()->left_sibling_ = current_Heap;
                    }
                } else {
                    if(pre_current_Heap.get() != nullptr) {
                        pre_current_Heap.get()->right_sibling_ = current_Heap.get()->right_sibling_;
                        current_Heap.get()->right_sibling_.get()->left_sibling_ = pre_current_Heap;
                    }
                    else {
                        Heap->head_ = current_Heap.get()->right_sibling_;
                    }
                    std::shared_ptr<Node>temporary = current_Heap;
                    current_Heap = current_Heap.get()->right_sibling_;
                    current_Heap.get()->left_sibling_ = temporary.get()->left_sibling_;
                    temporary.get()->right_sibling_ = current_Heap.get()->left_child_;
                    if (current_Heap.get()->left_child_.get() != nullptr) {
                        current_Heap.get()->left_child_.get()->left_sibling_ = temporary;
                    }
                    current_Heap.get()->left_child_ = temporary;
                    if (current_Heap.get()->left_child_.get() != nullptr) {
                        current_Heap.get()->left_child_.get()->left_sibling_.reset();
                    }
                    temporary.get()->parrent_ = current_Heap;
                }
                ++current_Heap.get()->degree_;
            }
            else {
                pre_current_Heap = current_Heap;
                current_Heap = current_Heap.get()->right_sibling_;
            }
        }
        if (this->min_element_> otherHeap->min_element_){
            this->min_element_ = otherHeap->min_element_;
        }
        this->head_ = Heap->head_;
    }
    if (this->head_.get() == nullptr) {
        this->head_ = otherHeap->head_;
        this->min_element_ = otherHeap->min_element_;
    }
    otherHeap->head_.reset();
}

template <typename T>
typename BinomialHeap<T>::Pointer BinomialHeap<T>::Insert(T &init_value){
    BinomialHeap *Heap = new BinomialHeap(init_value);
    Pointer pointer;
    pointer.point_ = Heap->head_;
    Merge(Heap);
    delete Heap;
    return pointer;
}

template <typename T>
T BinomialHeap<T>::ExtractMin() {
    if (IsEmpty()) {
        throw std::out_of_range("Couldn't extract min, the heap is empty!");
    }
    std::shared_ptr<Node>current = head_;
    std::shared_ptr<Node>extract_node = head_;
    std::shared_ptr<Node>pre_extract_node;
    pre_extract_node.reset();
    T min_key = min_element_;

    while (current.get()->right_sibling_.get() != nullptr) {
        if (min_key == current.get()->right_sibling_.get()->key_) {
            pre_extract_node = current;
            extract_node = current.get()->right_sibling_;
        }
        current = current.get()->right_sibling_;
    }

    if (pre_extract_node.get() != nullptr) {
        pre_extract_node.get()->right_sibling_ = extract_node.get()->right_sibling_;
        if (extract_node.get()->right_sibling_.get() != nullptr) {
            extract_node.get()->right_sibling_.get()->left_sibling_ = pre_extract_node;
        }
    } else {
        head_ = extract_node.get()->right_sibling_;
    }

    current = head_;
    if (current.get() != nullptr) {
        min_element_ = current.get()->key_;
        while (current.get()->right_sibling_.get() != nullptr) {
            if (current.get()->right_sibling_.get()->key_ < min_element_) {
                min_element_ = current.get()->right_sibling_.get()->key_;
            }
            current = current.get()->right_sibling_;
        }
    }

    if (extract_node.get()->left_child_.get() != nullptr) {
        std::shared_ptr<Node>current = extract_node.get()->left_child_;
        std::shared_ptr<Node>pre_current;
        pre_current.reset();
        std::shared_ptr<Node>current_next = current.get()->right_sibling_;
        BinomialHeap *Heap = new BinomialHeap;
        Heap->min_element_ = current.get()->key_;
        while (current.get() != nullptr){
            if (Heap->min_element_ > current.get()->key_){
                Heap->min_element_ = current.get()->key_;
            }
            current.get()->right_sibling_ = pre_current;
            if (pre_current.get() != nullptr) {
                pre_current.get()->left_sibling_ = current;
            }
            current.get()->parrent_.reset();
            pre_current = current;
            current = current_next;
            if (current_next.get() != nullptr){
                current_next = current_next.get()->right_sibling_;
            }
        }
        Heap->head_ = pre_current;
        Merge(Heap);
        delete Heap;
    }
    extract_node.reset();
    return min_key;
}

template <typename T>
void BinomialHeap<T>::Change(Pointer init_pointer, T& new_value){
    if (IsEmpty()){
        throw std::out_of_range("Couldn't change element, the heap is empty!");
    }
    if (init_pointer.point_.expired()) {
        throw std::out_of_range("Couldn't change element, it is already deleted!");
    }
    std::shared_ptr<Node> current = init_pointer.point_.lock();
    if (current.get()->key_ > new_value){
        if (new_value < min_element_){
            min_element_ = new_value;
        }
        current.get()->key_ = new_value;
        SiftUp(current);
    }
    else {
        current.get()->key_ = new_value;
        SiftDown(current);
    }
}

template <typename T>
void BinomialHeap<T>::Delete(Pointer init_pointer){
    if (init_pointer.point_.expired()) {
        throw std::out_of_range("Couldn't delete element, it is already deleted!");
    }
    T minimum = min_element_ - 1;
    Change(init_pointer, minimum);
    ExtractMin();
}


