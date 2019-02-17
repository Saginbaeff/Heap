#include <assert.h>

template <typename T>
void vector<T>::Insert(T& init_value){
    if (last_element_ == size_ - 1){
        Expand();
    }

    buffer_[last_element_++] = init_value;
}

template <typename T>
vector<T>::vector(){
    size_ = 10;
    last_element_ = 1;
    buffer_ = new T[size_];
    for (int i = 0; i < size_; i++){
        buffer_[i] = NULL;
    }
}

template <typename T>
vector<T>::~vector(){
    delete[] buffer_;
}

template <typename T>
T& vector<T>::operator[](size_t id){
    return buffer_[id];
}

template <typename T>
size_t vector<T>::GetSize(){
    return size_;
}

template <typename T>
void vector<T>::Expand(){
    T* new_buffer_ = new T[size_ << 1];

    for(int i = 0; i < last_element_ + 1; ++i){
        new_buffer_[i] = buffer_[i];
    }

    size_ = size_ << 1;
    delete[] buffer_;
    buffer_ = new_buffer_;
}

template <typename T>
void vector<T>::Constrict(){
    T* new_buffer_ = new T[size_ >> 1];

    for(int i = 0; i < last_element_ + 1; ++i){
        new_buffer_[i] = buffer_[i];
    }
    size_ = size_ >> 1;
    for(int i = last_element_ + 1; i < size_; i ++){
        new_buffer_[i] = NULL;
    }
    delete[] buffer_;
    buffer_ = new_buffer_;

}
