// 1. Put ONLY optimize above #include
#pragma GCC optimize("O3,unroll-loops")
 
#include <bits/stdc++.h>
 
// 2. Put target BELOW #include so STL headers don't break in GCC 14!
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
