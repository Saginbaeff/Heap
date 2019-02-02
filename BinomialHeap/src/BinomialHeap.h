<<<<<<< HEAD
#include <assert.h>
#include <stdexcept>
#include <iostream>
#include <memory>


template <typename T>
class BinomialHeap {
public:
    class Pointer{
    private:
        friend class BinomialHeap<T>;

        Pointer (std::shared_ptr<typename BinomialHeap<T>::Node> init_pointer){
            point_ = init_pointer;
        }
        std::weak_ptr<typename BinomialHeap<T>::Node> point_;
    public:
        Pointer(){}

        ~Pointer(){}

    };

    BinomialHeap(T &init_value);
    BinomialHeap();
    ~BinomialHeap();
    bool IsEmpty();
    void Merge(BinomialHeap &otherHeap);
    Pointer Insert(T &init_value);
    T GetMin();
    T ExtractMin() ;
    void Change(Pointer init_pointer, T& new_value);
    void Delete(Pointer init_pointer);

private:
    struct Node {
        std::shared_ptr<Node> parrent_;
        std::shared_ptr<Node> left_child_;
        std::shared_ptr<Node> right_sibling_;
        std::shared_ptr<Node> left_sibling_;
        T key_;
        size_t degree_;

        Node(T &init_value) {
            parrent_.reset();
            left_child_.reset();
            right_sibling_.reset();
            left_sibling_.reset();
            key_ = init_value;
            degree_ = 0;
        }

        ~Node(){
            parrent_.reset();
            left_child_.reset();
            right_sibling_.reset();
            left_sibling_.reset();
        }
    };

    std::shared_ptr<Node> head_;
    T min_element_;

    void SiftUp(std::shared_ptr<Node> init_Node);
    void SiftDown(std::shared_ptr<Node> init_Node);

};

#include "BinomialHeap.tpp"
=======
#include <assert.h>
#include <stdexcept>
#include <memory>
#include <iostream>

template <typename T>
class BinomialHeap {
public:
    struct Node {
    private:
        friend class BinomialHeap<T>;
        std::shared_ptr<Node> parrent_;
        std::shared_ptr<Node> left_child_;
        std::shared_ptr<Node> right_sibling_;
        std::shared_ptr<Node> left_sibling_;
        T key_;
        size_t degree_;

        Node(T &init_value) {
            parrent_.reset();
            left_child_.reset();
            right_sibling_.reset();
            left_sibling_.reset();
            key_ = init_value;
            degree_ = 0;
        }

    public:
        ~Node(){
            parrent_.reset();
            left_child_.reset();
            right_sibling_.reset();
            left_sibling_.reset();
        }
    };

    struct Pointer{
    private:
        friend class BinomialHeap<T>;

        Pointer (std::shared_ptr<Node> init_pointer){
            point_ = init_pointer;
        }
        std::weak_ptr<Node> point_;
    public:
        Pointer(){}

        ~Pointer(){}

    };

    BinomialHeap(T &init_value);
    BinomialHeap();
    ~BinomialHeap();
    bool IsEmpty();
    void Merge(BinomialHeap *otherHeap);
    Pointer Insert(T &init_value);
    T GetMin();
    T ExtractMin() ;
    void Change(Pointer init_pointer, T& new_value);
    void Delete(Pointer init_pointer);

private:
    std::shared_ptr<Node> head_;
    T min_element_;

    void SiftUp(std::shared_ptr<Node> init_Node);
    void SiftDown(std::shared_ptr<Node> init_Node);

};

#include "BinomialHeap.tpp"
>>>>>>> 1e05e146985a255939860ef3c03aa9abfbc67b6d
