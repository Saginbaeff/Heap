#include <iostream>
#include <assert.h>
#include <stdexcept>
#include <memory>

//public methods:

template <typename T>
Heap<T>::Heap(){
    buffer_ = new std::shared_ptr<Node>[10];
    size_ = 10;
    last_element_ = 0;
}

template <typename T>
template<class Iterator>
Heap<T>::Heap(Iterator Begin, Iterator End){
    buffer_ = new std::shared_ptr<Node>[10];
    size_ = 10;
    last_element_ = 0;

    while (Begin < End){
        std::shared_ptr<Node> insert_shared = std::shared_ptr<Node>(new Node(*Begin, ++last_element_));
        buffer_[last_element_] = insert_shared;
        ++Begin;
        if (last_element_ == size_ - 1) {
            Expand();
        }
    }

    size_t id = (last_element_ + 1) / 2 + 1;
    for (size_t i = id; i > 0; --i){
        SiftDown(i);
    }
}

template <typename T>
Heap<T>::~Heap(){
    for (int i = 0; i < last_element_; i++){
        buffer_[i + 1].reset();
    }

    delete[] buffer_;
}

template <typename T>
typename Heap<T>::Pointer Heap<T>:: Insert(T& init_value_){
    if (last_element_ == size_ - 1) {
        Expand();
    }

    std::shared_ptr<Node> shared = std::shared_ptr<Node>(new Node(init_value_, ++last_element_));
    buffer_[last_element_] = shared;
    SiftUp(last_element_);
    Pointer pointer(shared);
    return pointer;
}

template <typename T>
bool Heap<T>::IsEmpty(){
    if (last_element_ == 0) {
        return true;
    }
    else {
        return false;
    }
}

template <typename T>
T Heap<T>::GetMin(){
    try {
        if (IsEmpty()) throw IsEmpty();
        return buffer_[1].get()->value_;
    }
    catch(...){
        std::cout << "Error: couldn't get min, Heap is empty!\n";
        return -1;
    }
}

template <typename T>
T Heap<T>::ExtractMin(){
    try {
        if (IsEmpty()){
            throw IsEmpty();
        }
        T min_el = buffer_[1].get()->value_;
        buffer_[1].reset();
        buffer_[1] = buffer_[last_element_--];
        buffer_[last_element_ + 1].reset();
        SiftDown(1);
        if (last_element_ < size_ / 3) {
            Constrict();
        }
        return min_el;
    }
    catch (...){
        std::cout << "Error: couldn't extract min, Heap is empty!\n ";
        return -1;
    }
}

template <typename T>
void Heap<T>::Delete(Pointer init_ptr){
    try {
        if (init_ptr.point_.expired()){
            throw init_ptr.point_.expired();
        }
        size_t id = init_ptr.point_.lock().get()->index_;
        T prev_value = buffer_[id].get()->value_;
        buffer_[id].reset();
        buffer_[id] = buffer_[last_element_--];
        buffer_[last_element_ + 1].reset();
        if (last_element_ < size_ / 3) {
            Constrict();
        }
        if (!IsEmpty()) {
            if (buffer_[id].get()->value_ > prev_value) {
                SiftDown(id);
            } else {
                SiftUp(id);
            }
        }
    }
    catch (...){
        std::cout << "Error: couldn't delete element, it's already deleted!\n ";
    }
}

template <typename T>
void Heap<T>::Change(Pointer init_ptr, T& new_value){
    try {
        if (init_ptr.point_.expired()) {
            throw init_ptr.point_.expired();
        }
        size_t id = init_ptr.point_.lock().get()->index_;
        T prev_value = buffer_[id].get()->value_;
        buffer_[id].get()->value_ = new_value;
        if (buffer_[id].get()->value_ > prev_value) {
            SiftDown(id);
        } else {
            SiftUp(id);
        }
    }
    catch (...){
        std::cout << "Error: couldn't change element, it's already deleted!\n ";
    }
}

//private methods:

template <typename T>
void Heap<T>::Expand(){
    std::shared_ptr<Node>* new_buffer_ = new std::shared_ptr<Node>[size_ << 1];

    for(int i = 0; i < last_element_; ++i){
        new_buffer_[i + 1] = buffer_[i + 1];
        buffer_[i + 1].reset();
    }

    size_ = size_ << 1;
    delete[] buffer_;
    buffer_ = new_buffer_;
}

template <typename T>
void Heap<T>::Constrict(){
    std::shared_ptr<Node>* new_buffer_ = new std::shared_ptr<Node>[size_ >> 1];

    for(int i = 0; i < last_element_; ++i){
        new_buffer_[i + 1] = buffer_[i + 1];
        buffer_[i + 1].reset();
    }

    size_ = size_ >> 1;
    delete[] buffer_;
    buffer_ = new_buffer_;
}

template <typename T>
void Heap<T>::SiftUp (size_t el_num){

    if (el_num >> 1 >= 1){
        if (buffer_[el_num].get()->value_ < buffer_[el_num >> 1].get()->value_) {
            buffer_[el_num].swap(buffer_[el_num >> 1]);
            buffer_[el_num].get()->index_ = el_num;
            buffer_[el_num >> 1].get()->index_ = el_num >> 1;
            SiftUp(el_num >> 1);
        }
    }
}

template <typename T>
void Heap<T>::SiftDown (size_t el_num){
    if ((el_num << 1) + 1 <= last_element_) {

        if (buffer_[el_num].get()->value_ > buffer_[el_num << 1].get()->value_ ||
            buffer_[el_num].get()->value_ > buffer_[(el_num << 1) + 1].get()->value_) {

            if (buffer_[el_num << 1].get()->value_ < buffer_[(el_num << 1) + 1].get()->value_) {
                buffer_[el_num << 1].swap(buffer_[el_num]);
                buffer_[el_num << 1].get()->index_ = el_num << 1;
                buffer_[el_num].get()->index_ = el_num;
                SiftDown(el_num << 1);
            }

            else {
                buffer_[(el_num << 1) + 1].swap(buffer_[el_num]);
                buffer_[(el_num << 1) + 1].get()->index_ = (el_num << 1) + 1;
                buffer_[el_num].get()->index_ = el_num;
                SiftDown((el_num << 1) + 1);
            }

        }

    }

    else{

        if (el_num << 1 <= last_element_ &&
            buffer_[el_num << 1].get()->value_ < buffer_[el_num].get()->value_){
            buffer_[el_num << 1].swap(buffer_[el_num]);
            buffer_[el_num << 1].get()->index_ = el_num << 1;
            buffer_[el_num].get()->index_ = el_num;
            SiftDown(el_num << 1);
        }
    }
}
