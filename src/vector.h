#include <assert.h>

template <typename T>
class vector{
public:
    void Insert(T& init_value);

    vector();
    ~vector();

    T& operator[](size_t id);

    size_t GetSize();
private:
    T* buffer_;
    size_t size_;
    size_t last_element_;

    void Expand();
    void Constrict();
};

#include "vector.tpp"
