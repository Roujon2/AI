#include <iostream>
#include <vector>

// Function used to calculate the number of conflicts in a certain board
int numConflicts(const std::vector<int>& current_board){
    // Initialize the number of conflicts and the size of the board
    int conflicts = 0;
    int N = current_board.size();

    // Looping through the board and checking how many conflicts summed up for each queen
    for(int row=0; row<N; row++){
        // Skipping a column because the data structure ensure that there is only 1 queen per column
        for(int next_row=row+1; next_row<N; next_row++){
            // If there is a conflict in the same row and diagonal, add 1 to the number of conflicts
            if(current_board[row] == current_board[next_row] || abs(row - next_row) == abs(current_board[row] - current_board[next_row])){
                conflicts++;
            }
        }
    }

    return conflicts;
}

void localSearch(int N, std::vector<int>& result){
    // Board vector to try solutions
    std::vector<int> board(N);

    // Temperature variable for simulated annealing
    double T = 1.0;
    // Constant used to decrease the temperature over time
    double alpha = 0.8;

    // Initialize the board vector with a random state
    for(int i=0; i<N; i++){
        board[i] = rand() % N;
    }

    // Initialize the number of conflicts with the random board
    int current_conflicts = numConflicts(board);

    // Iterate until a solution is found
    while(current_conflicts > 0){
        // Generate neighbor states by moving each queen to the row with a lesser number of conflicts

        // Iterate over the columns (the index of the board vector)
        for (int col = 0; col < N; col++) {
            // Keep track of the original row so we can undo the move
            int current_row = board[col];
            // Iterate over the rows of that column, moving the queen to the row with the least number of conflicts
            for (int row = 0; row < N; row++) {
                // Skip the current row, since we've already checked that and the conflicts would be the same
                if (row != current_row) {
                    // Move the queen to the new row
                    board[col] = row;
                    // Calculate the number of conflicts in the new state
                    int new_conflicts = numConflicts(board);
                    // If the new state has less conflicts, move to that state
                    if (new_conflicts < current_conflicts) {
                        current_conflicts = new_conflicts;
                        break;  // Move to the next column
                    }else{
                        // If the new state has more conflicts, move to that state with a probability of e^(-(current_conflicts-new_conflicts)/T)
                        double probability = exp(-(new_conflicts-current_conflicts)/T);
                        double random = (double)rand() / RAND_MAX;
                        // If the random number is less than the probability, move to that state
                        if(random < probability){
                            current_conflicts = new_conflicts;
                            T = T * alpha;
                            break;
                        }
                    }
                    board[col] = current_row;  // Undo the move
                }
            }

            // If the current state has no conflicts, we've found a solution
            if (current_conflicts == 0) {
                break;
            }
        }

        // Copy the final state to the result vector
        result = board;
    }
}


int main(int argc, char const *argv[])
{
    // Seed for random number generator
    srand(time(NULL));

    // Number of queens on the board
    int N;

    // User input for N
    std::cout << "Specify the number of queens on the board: ";
    std::cin >> N;

    // Vector to store the result of queen positioning
    std::vector<int> result;

    // Call the local search function that (hopefully) provides a solution
    localSearch(N, result);

    // Print the result in a board-like print
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (result[col] == row) {
                std::cout << "Q ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }

    // Printing the vector of values
    for(int queen : result){
        std::cout << queen << " ";
    }
    std::cout << std::endl;

    return 0;
}
