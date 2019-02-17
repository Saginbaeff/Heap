#include <iostream>
#include <assert.h>
#include <stdexcept>
#include <memory>
#include "vector.h"

template <typename T>
class Heap {
public:
    struct Pointer{
    private:
        friend class Heap<T>;

        Pointer (std::shared_ptr<typename Heap<T>::Node> init_pointer) {
            point_ = init_pointer;
        }
        std::weak_ptr<typename Heap<T>::Node> point_;
    public:
        Pointer() {}

        ~Pointer() {}

    };

    Heap();

    template<class Iterator>
    Heap(Iterator Begin, Iterator End);

    ~Heap();

    Pointer Insert (T& init_value_);

    bool IsEmpty();

    T GetMin();

    T ExtractMin();

    void Delete(Pointer init_ptr);

    void Change(Pointer init_ptr, T& new_value);

private:
    struct Node{
    private:
        friend class Heap<T>;
        Node() {
            value_ = 0;
            index_ = 0;
        }
        Node(T &init_value, size_t &init_index) {
            value_ = init_value;
            index_ = init_index;
        }

        T value_;
        size_t index_; //текущий индекс элемента в массиве

    public:
        ~Node() {};
    };

    vector<std::shared_ptr<Node> > buffer_;
    size_t last_element_;

    void SiftUp (size_t el_num);

    void SiftDown (size_t el_num);

    size_t ParrentIndex(size_t element_index);
    size_t LeftChildIndex(size_t element_index);
    size_t RightChildIndex(size_t element_index);
    void SwapNodes(size_t first_node_index, size_t second_node_index);
};

#include "Heap.tpp"