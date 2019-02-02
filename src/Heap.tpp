#include <iostream>
#include <assert.h>
#include <stdexcept>
#include <memory>

//public methods:

template <typename T>
Heap<T>::Heap() {
    last_element_ = 0;
}

template <typename T>
template<class Iterator>
Heap<T>::Heap(Iterator Begin, Iterator End) {
    last_element_ = 0;

    while (Begin < End) {
        std::shared_ptr<Node> insert_shared = std::shared_ptr<Node>(new Node(*Begin, ++last_element_));
        buffer_.Insert(insert_shared);
        ++Begin;
    }

    size_t half_array_index = (last_element_ + 1) / 2 + 1;
    for (size_t i = half_array_index; i > 0; --i) {
        SiftDown(i);
    }
}

template <typename T>
Heap<T>::~Heap() {}

template <typename T>
typename Heap<T>::Pointer Heap<T>:: Insert(T& init_value_) {
    std::shared_ptr<Node> insert_node = std::shared_ptr<Node>(new Node(init_value_, ++last_element_));
    buffer_.Insert(insert_node);
    SiftUp(last_element_);
    return Pointer(insert_node);
}

template <typename T>
bool Heap<T>::IsEmpty() {
    return last_element_ == 0;
}

template <typename T>
T Heap<T>::GetMin() {
    if (IsEmpty()) {
        throw std::out_of_range("Couldn't get min, the heap is empty!");
    }
    return buffer_[1].get()->value_;
}

template <typename T>
T Heap<T>::ExtractMin() {
    if (IsEmpty()) {
        throw std::out_of_range("Couldn't extract min, the heap is empty!");
    }
    T min_element = buffer_[1].get()->value_;
    buffer_[1].reset();
    buffer_[1] = buffer_[last_element_--];
    buffer_[last_element_ + 1].reset();
    SiftDown(1);
    return min_element;
}

template <typename T>
void Heap<T>::Delete(Pointer init_ptr) {
    if (init_ptr.point_.expired()) {
        throw std::out_of_range("Couldn't delete element, it is already deleted!");
    }
    init_ptr.point_.lock()->value_ = buffer_[1]->value_;
    SiftUp(init_ptr.point_.lock()->index_);
    ExtractMin();
}

template <typename T>
void Heap<T>::Change(Pointer init_ptr, T& new_value) {
    if (init_ptr.point_.expired()) {
        throw std::out_of_range("Couldn't change element, it is already deleted!");
    }
    if (init_ptr.point_.expired()) {
        throw init_ptr.point_.expired();
    }
    size_t current_index = init_ptr.point_.lock().get()->index_;
    T prev_value = buffer_[current_index].get()->value_;
    buffer_[current_index].get()->value_ = new_value;
    if (buffer_[current_index].get()->value_ > prev_value) {
        SiftDown(current_index);
    } else {
        SiftUp(current_index);
    }
}

//private methods:

template <typename T>
size_t Heap<T>::ParrentIndex(size_t element_index){
    return element_index / 2;
}

template <typename T>
size_t Heap<T>::LeftChildIndex(size_t element_index){
    return element_index * 2;
}

template <typename T>
size_t Heap<T>::RightChildIndex(size_t element_index){
    return element_index * 2 + 1;
}

template <typename T>
void Heap<T>::SwapNodes(size_t first_node_index, size_t second_node_index){
    buffer_[first_node_index].swap(buffer_[second_node_index]);
    buffer_[first_node_index].get()->index_ = first_node_index;
    buffer_[second_node_index].get()->index_ = second_node_index;
}

template <typename T>
void Heap<T>::SiftUp (size_t element_index) {

    if (ParrentIndex(element_index) >= 1) {
        if (buffer_[element_index].get()->value_ < buffer_[ParrentIndex(element_index)].get()->value_) {
            SwapNodes(ParrentIndex(element_index), element_index);
            SiftUp(ParrentIndex(element_index));
        }
    }
}

template <typename T>
void Heap<T>::SiftDown (size_t element_index) {
    if (RightChildIndex(element_index) <= last_element_) {

        if (buffer_[element_index].get()->value_ > buffer_[LeftChildIndex(element_index)].get()->value_ ||
            buffer_[element_index].get()->value_ > buffer_[RightChildIndex(element_index)].get()->value_) {

            if (buffer_[LeftChildIndex(element_index)].get()->value_ < buffer_[RightChildIndex(element_index)].get()->value_) {
                SwapNodes(LeftChildIndex(element_index), element_index);
                SiftDown(LeftChildIndex(element_index));
            } else {
                SwapNodes(RightChildIndex(element_index), element_index);
                SiftDown(RightChildIndex(element_index));
            }

        }

    } else{

        if (LeftChildIndex(element_index) <= last_element_ &&
            buffer_[LeftChildIndex(element_index)].get()->value_ < buffer_[element_index].get()->value_) {
            SwapNodes(LeftChildIndex(element_index), element_index);
            SiftDown(LeftChildIndex(element_index));
        }
    }
}
