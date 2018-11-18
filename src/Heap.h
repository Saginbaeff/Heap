#include <iostream>
#include <assert.h>
#include <stdexcept>
#include <memory>

template <typename T>
class Heap {
public:
    struct Node{
    private:
        friend class Heap<T>;
        Node(){
            value_ = 0;
            index_ = 0;
        }
        Node(T &init_value, size_t &init_index){
            value_ = init_value;
            index_ = init_index;
        }

        T value_;
        size_t index_;

    public:

        ~Node(){};
    };

    struct Pointer{
    private:
        friend class Heap<T>;

        Pointer (std::shared_ptr<Node> init_pointer){
            point_ = init_pointer;
        }
        std::weak_ptr<Node> point_;
    public:
        Pointer(){}

        ~Pointer(){}

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
    std::shared_ptr<Node>* buffer_;
    size_t size_;
    size_t last_element_;


    void Expand();

    void Constrict();

    void SiftUp (size_t el_num);

    void SiftDown (size_t el_num);


};

#include "Heap.tpp"