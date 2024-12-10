#include <iostream>
#include <string>

class DFA {
public:
    // Function to check if the input string is accepted by the DFA
    bool isAccepted(const std::string& input) {
        int state = 0; // Start in state 0

        for (char ch : input) {
            switch (state) {
                case 0: // State for 'a's
                    if (ch == 'a') {
                        // Stay in state 0
                    } else if (ch == 'b') {
                        state = 1; // Move to state 1 on 'b'
                    } else if (ch == 'c') {
                        state = 2; // Move to state 2 on 'c'
                    } else {
                        return false; // Reject on any other character
                    }
                break;
                case 1: // State for 'b's
                    if (ch == 'b') {
                        // Stay in state 1
                    } else if (ch == 'c') {
                        state = 2; // Move to state 2 on 'c'
                    } else {
                        return false; // Reject on any other character
                    }
                break;
                case 2: // State for 'c's
                    if (ch == 'c') {
                        // Stay in state 2
                    } else {
                        return false; // Reject on any character after 'c'
                    }
                break;
                default:
                    return false; // Should not reach here
            }
        }

        // Accept if we are in state 0, 1, or 2 (valid end states)
        return true;
    }
};

int main() {
    DFA dfa;
    std::string input;

    std::cout << "Enter a string: ";
    std::cin >> input;

    if (dfa.isAccepted(input)) {
        std::cout << "The string is accepted." << std::endl;
    } else {
        std::cout << "The string is not accepted." << std::endl;
    }

    return 0;
}