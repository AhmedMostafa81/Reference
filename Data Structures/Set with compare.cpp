#include <iostream>
#include <set>

int main() {
    // Define the lambda
    auto comp = [](int a, int b) { 
        return a > b; 
    };

    // Pass the type via decltype, and the instance to the constructor
    std::set<int, decltype(comp)> mySet(comp);
    
    mySet.insert(1);
    mySet.insert(5);
    mySet.insert(2);

    for (int num : mySet) {
        std::cout << num << " "; // Output: 5 2 1
    }

    return 0;
}
