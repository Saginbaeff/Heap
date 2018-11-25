#include <assert.h>
#include <stdexcept>
#include <memory>
#include <iostream>

template <typename T>
class FibonacciHeap {
public:
    struct Node {
    private:
        friend class FibonacciHeap<T>;
        std::shared_ptr<Node> parrent_;
        std::shared_ptr<Node>left_sibling_;
        std::shared_ptr<Node>right_sibling_;
        std::shared_ptr<Node>child_;
        T key_;
        size_t degree_;
        bool mark_;

    public:
        Node(T &init_value) {
            parrent_.reset();
            left_sibling_.reset();
            right_sibling_.reset();
            child_.reset();
            key_ = init_value;
            degree_ = 0;
            mark_ = false;
        }
        ~Node(){
            parrent_.reset();
            left_sibling_.reset();
            right_sibling_.reset();
            child_.reset();
        }
    };

    struct Pointer{
    private:
        friend class FibonacciHeap<T>;

        Pointer (std::shared_ptr<Node> init_pointer){
            point_ = init_pointer;
        }
        std::weak_ptr<Node> point_;
    public:
        Pointer(){}

        ~Pointer(){}

    };

    FibonacciHeap();

    bool IsEmpty();

    T GetMin();

    Pointer Insert(T &init_value);

    void Merge(FibonacciHeap *otherHeap);

    T ExtractMin();

    void Change(Pointer init_pointer, T& new_value);

/*
    void test(){
        std::shared_ptr<Node> current = head_;
        std::cout << current.get()->degree_ << " ";
        current = current.get()->right_sibling_;
        while (current != head_){
            std::cout << current.get()->degree_ << " ";
            current = current.get()->right_sibling_;
        }
        std::cout <<"\n";
    }
*/
private:
    std::shared_ptr<Node> head_;

    void Consolidate();

    void CascadingCut(std::shared_ptr<Node> init_pointer);

};

#include "FibonacciHeap.tpp"