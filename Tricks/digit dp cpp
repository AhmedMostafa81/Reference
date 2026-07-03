#include <bits/stdc++.h>
using namespace std;
#define ll long long

const int MAX_LEN = 20;    // Maximum number of digits (10^18 is 19 digits)
const int MAX_STATE1 = 165; // Example: Max sum of digits
const int MAX_STATE2 = 165; // Example: Max modulo
// Add more states as needed for the specific problem

// dp[length][state1][state2]
// Meaning: Number of combinations of length 'length' that result in 'state1' and 'state2'
ll dp[MAX_LEN][MAX_STATE1][MAX_STATE2]; 

void precompute() {
    // 1. Base Case: An empty suffix (length 0) has a base state.
    // Usually, sum = 0, mod = 0 gives 1 valid empty string.
    dp[0][0][0] = 1; 

    // 2. Build the DP bottom-up by increasing length
    for (int len = 0; len < MAX_LEN - 1; len++) {
        for (int state1 = 0; state1 < MAX_STATE1; state1++) {
            for (int state2 = 0; state2 < MAX_STATE2; state2++) {
                
                // If this state is impossible, skip it to save time
                if (dp[len][state1][state2] == 0) continue; 
                
                // 3. Try appending every possible digit (0-9) to the current suffix
                for (int d = 0; d <= 9; d++) {
                    
                    // TODO: Calculate the next states based on the problem's rules
                    // Example:
                    int nxt_state1 = state1 + d;
                    int nxt_state2 = (state2 * 10 + d) % /* some mod */;
                    
                    // Make sure we don't go out of bounds
                    if (nxt_state1 < MAX_STATE1) {
                        dp[len + 1][nxt_state1][nxt_state2] += dp[len][state1][state2];
                    }
                }
            }
        }
    }
}

ll solve(string S) {
    ll ans = 0;
    
    // Track the running state of the exact prefix we are matching
    int running_state1 = 0; 
    int running_state2 = 0;
    
    int n = S.length();
    
    // Walk through the upper limit string from left to right
    for (int i = 0; i < n; i++) {
        int current_limit_digit = S[i] - '0';
        int remaining_len = n - i - 1;
        
        // 1. Branch off: Try all digits strictly LESS than the limit digit
        for (int d = 0; d < current_limit_digit; d++) {
            
            // TODO: Figure out what state we need from the remaining suffix
            // to satisfy the global condition, given our 'running' prefix + 'd'.
            
            // int needed_state1 = TARGET - (running_state1 + d);
            // int needed_state2 = ... ;
            
            // if (needed_state_is_valid) {
            //     ans += dp[remaining_len][needed_state1][needed_state2];
            // }
        }
        
        // 2. Commit to the exact prefix limit for the next iteration
        // TODO: Update your running states as if you placed 'current_limit_digit'
        running_state1 += current_limit_digit;
        // running_state2 = (running_state2 * 10 + current_limit_digit) % ... ;
        
        // 3. Optional Break: If the exact prefix becomes permanently invalid, stop.
        // Example: if (running_state1 > MAX_TARGET) break;
    }
    
    // 4. Finally, check if the exact string S itself is valid
    // We only reach here if we didn't break out of the loop early
    // if (running_state1 == TARGET && running_state2 == 0) {
    //     ans++;
    // }
    
    return ans;
}

int main() {
    // Run this exactly once globally
    precompute(); 
    
    int t;
    cin >> t;
    while (t--) {
        ll L, R;
        cin >> L >> R;
        // Output solve(R) - solve(L - 1)
        cout << solve(to_string(R)) - solve(to_string(L - 1)) << '\n';
    }
    return 0;
}
