//
// Created by Азат on 15.11.2018.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/Heap.h"
#include <queue>
#include <vector>
#include <algorithm>



using testing::Eq;

namespace {
    class ClassDeclaration : public testing::Test {
    public:
        Heap<int> obj;
        ClassDeclaration(){
            obj;
        }
    };
}

TEST_F(ClassDeclaration, Test1){
    Heap<int> My_Heap;
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

TEST_F(ClassDeclaration, Test2){
    std::vector<int> a;
    std::vector<int> b;
    std::vector<int> test = {66, 94, 12, 75, 29, 77, 44, 84, 8, 75, 37, 83, 51, 76, 30};
    for (int i = 0; i < 15; i++){
        a.push_back(test[i]);
        b.push_back(test[i]);
    }
    std::sort(a.begin(), a.begin() + a.size());
    Heap<int> My_Heap(b.begin(), b.begin() + b.size());
    for (int i = 0; i < 15; i++){
        ASSERT_EQ(a[i], My_Heap.ExtractMin());
    }
}

TEST_F(ClassDeclaration, Test3){
    Heap<int> My_Heap;
    std::vector<int> a;
    std::vector<int> b;
    std::vector<Heap<int>::Pointer> c;
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

TEST_F(ClassDeclaration, Test4){
    Heap<int> My_Heap;
    Heap<int>::Pointer f;
    for (int i = 0; i < 15; i++){
        if (i == 12) {
            f = My_Heap.Insert(i);
        }
        else{
            My_Heap.Insert(i);
        }
    }
    My_Heap.Delete(f);
    for (int i = 0; i < 15; i++){
        if (i != 12) {
            ASSERT_EQ(i, My_Heap.ExtractMin());
        }
    }
}

