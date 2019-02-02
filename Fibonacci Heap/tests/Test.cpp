//
// Created by Азат on 15.11.2018.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/FibonacciHeap.h"
#include <queue>
#include <vector>
#include <algorithm>



using testing::Eq;

namespace {
    class ClassDeclaration : public testing::Test {
    public:
        FibonacciHeap<int> obj;
        ClassDeclaration(){
            obj;
        }
    };
}

TEST_F(ClassDeclaration, Insert_and_extract_test){
    FibonacciHeap<int> My_Heap;
    std::vector<int> a;
    std::vector<int> test = {66, 94, 12, 75, 29, 77, 44, 84, 8, 75, 37, 83, 51, 76, 30};
    for (int i = 0; i < 15; i++){
        My_Heap.Insert(test[i]);
        a.push_back(test[i]);
    }
    std::sort(a.begin(), a.begin() + a.size());
    for (int i = 0; i < 15; i++){
        ASSERT_EQ(a[i], My_Heap.ExtractMin());
    }
}

TEST_F(ClassDeclaration, Change_test){
    FibonacciHeap<int> My_Heap;
    std::vector<int> a;
    std::vector<int> b;
    std::vector<FibonacciHeap<int>::Pointer> c;
    std::vector<int> test = {66, 94, 12, 75, 29, 77, 44, 84, 8, 75, 37, 83, 51, 76, 30};
    for (int i = 0; i < 15; i++){
        c.push_back(My_Heap.Insert(test[i]));
        b.push_back(test[i]);
    }
    for (int i = 0; i < 15; i++){
        b[i] = My_Heap.GetMin() - 1;
        My_Heap.Change(c[i], b[i]);
        a = b;
        std::sort(a.begin(), a.begin() + a.size());
        ASSERT_EQ(a[0], My_Heap.GetMin());
    }
}

TEST_F(ClassDeclaration, Merge_test){
    FibonacciHeap<int> A;
    FibonacciHeap<int> B;
    std::vector<int> c;
    for (int i = 10; i < 20; i++){
        A.Insert(i);
        c.push_back(i);
    }
    for (int i = 20; i < 30; i++){
        B.Insert(i);
        c.push_back(i);
    }
    A.Merge(B);
    ASSERT_EQ(true, B.IsEmpty());
    for (int i = 0; i < 20; i++){
        ASSERT_EQ(c[i], A.ExtractMin());
    }
}

TEST_F(ClassDeclaration, Exceptions_test){
    FibonacciHeap<int> A;
    int t = 1;
    int f = 2;
    FibonacciHeap<int>::Pointer ptr;
    ptr = A.Insert(t);
    A.ExtractMin();
    try {
        A.Change(ptr, t);
    }
    catch(std::out_of_range const & err) {
        EXPECT_EQ(err.what(),std::string("Couldn't change element, the heap is empty!"));
    }
    ptr = A.Insert(t);
    A.Insert(f);
    A.ExtractMin();
    try {
        A.Change(ptr, t);
    }
    catch(std::out_of_range const & err) {
        EXPECT_EQ(err.what(),std::string("Couldn't change element, it is already deleted!"));
    }
    A.ExtractMin();
    try {
        A.ExtractMin();
    }
    catch(std::out_of_range const & err) {
        EXPECT_EQ(err.what(),std::string("Couldn't extract min, the heap is empty!"));
    }
    try {
        A.GetMin();
    }
    catch(std::out_of_range const & err) {
        EXPECT_EQ(err.what(),std::string("Couldn't get min, the heap is empty!"));
    }
}

