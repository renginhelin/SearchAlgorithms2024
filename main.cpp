/**
 * BLG 336E - Analysis of Algorithms II
 * Assignment 1
 * <Rengin Helin Yalçın>
 * <150220767>
*/

/* * * * * * * * * * * * * * * * * *
 * DO NOT MODIFY UNLESS REQUESTED  *
* * * * * * * * * * * * * * * * * */

#include <iostream>         
#include <vector>
#include <fstream>
#include <sstream>       
#include <stack>
#include <queue>
#include <chrono>       // For time measurement
#include <algorithm>

using namespace std;
using namespace std::chrono;

/* USE HERE IF YOU NEED TO DEFINE EXTRA FUNCTIONS */
/* START YOUR CODE HERE */


/* END YOUR CODE HERE */

/**
 * Perform Depth-First Search (DFS) traversal on a map.
 * 
 * @param map The map represented as a 2D vector of integers.
 * @param row The starting row index for the DFS traversal.
 * @param col The starting column index for the DFS traversal.
 * @param resource The resource value to search for during the traversal.
 * @return The size of the colony found during the DFS traversal.
 */
int dfs(vector<vector<int>>& map, int row, int col, int resource) {

    int count = 0;
    int rows = map.size();
    int cols = map[0].size();
    
    // Wrap around the edges if the position is out of bounds
    row = (row + rows) % rows;
    col = (col + cols) % cols;
    

    // Check if the current planet has the same resource type
    if (map[row][col] == resource) {
        map[row][col] = -1;
        count++; 

        // Explore adjacent planets
        count += dfs(map, row - 1, col, resource); // Up
        count += dfs(map, row + 1, col, resource); // Down
        count += dfs(map, row, col - 1, resource); // Left
        count += dfs(map, row, col + 1, resource); // Right
    }

    return count;

}


/**
 * Perform Breadth-First Search (BFS) traversal on a map.
 * 
 * @param map The map represented as a 2D vector of integers.
 * @param row The starting row index for the BFS traversal.
 * @param col The starting column index for the BFS traversal.
 * @param resource The resource value to search for during the traversal.
 * @return The size of the colony found during the BFS traversal.
 */
int bfs(vector<vector<int>>& map, int row, int col, int resource) {
    
    int count = 0;
    int rows = map.size();
    int cols = map[0].size();

    // Define the direction vectors for BFS traversal
    vector<int> drow = {-1, 1, 0, 0}; 
    vector<int> dcol = {0, 0, -1, 1}; 

    queue<pair<int, int>> q;
    q.push({row, col});
    map[row][col] = -1; 

    // Perform BFS traversal
    while (!q.empty()) {
        pair<int, int> current = q.front();
        q.pop();
        count++;

        // Explore adjacent planets
        for (int i = 0; i < 4; ++i) {
            int nrow = (current.first + drow[i] + rows) % rows; // Wrap around rows
            int ncol = (current.second + dcol[i] + cols) % cols; // Wrap around columns

            // Check if the new position has the same resource type
            if (map[nrow][ncol] == resource) {
                map[nrow][ncol] = -1;
                q.push({nrow, ncol});
            }
        }
    }

    return count;

}

/**
 * Finds the top-k largest colonies in a given map.
 *
 * @param map The map represented as a 2D vector of integers.
 * @param useDFS A boolean flag indicating whether to use Depth-First Search (DFS) or Breadth-First Search (BFS) algorithm.
 * @param k The number of top colonies to find.
 * @return A vector of pairs representing the size and resource type of the top-k largest colonies.
 */
vector<pair<int, int>> top_k_largest_colonies(vector<vector<int>>& map, bool useDFS, unsigned int k) {

    auto start = high_resolution_clock::now();      // Start measuring time
    
    vector<pair<int, int>> colonies;

     // Check if the map is empty
    if (map.empty() || map[0].empty()) {
        cout << "Map is empty." << endl;
        auto stop = high_resolution_clock::now(); // Stop measuring time
        auto duration = duration_cast<nanoseconds>(stop - start); // Calculate the duration
        cout << "Time taken: " << duration.count() << " nanoseconds" << endl;
        return colonies; 
    }

    // Iterate through the map to find colonies
    int rows = map.size();
    int cols = map[0].size();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (map[i][j] != -1) { 
                int colonyResource = map[i][j];
                int colony_size = useDFS ? dfs(map, i, j, map[i][j]) : bfs(map, i, j, map[i][j]);
                colonies.push_back({colony_size, colonyResource});
            }
        }
    }

    // Sort colonies based on size (in descending order), and if size is the same, sort resource type in ascending order
    sort(colonies.begin(), colonies.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        if (a.first != b.first) { // If sizes are different, sort based on size in descending order
            return a.first > b.first;
        } else { // If sizes are the same, sort based on resource type in ascending order
            return a.second < b.second;
        }
    });

    // Resize colonies vector to contain only the top-k colonies
    colonies.resize(min(k, (unsigned int)colonies.size()));

    auto stop = high_resolution_clock::now();                       // Stop measuring time
    auto duration = duration_cast<nanoseconds>(stop - start);       // Calculate the duration
    cout << "Time taken: " << duration.count() << " nanoseconds" << endl;

    return colonies;

}

// Main function
int main(int argc, char* argv[]) {

    // Check the command line arguments
    if (argc != 4) {

        // It should be: <1 or 0> <k> <filename.txt>
        cerr << "Usage: " << argv[0] << " <1 or 0> <k> <filename.txt>" << endl;
        return 1;
    }

    // Get the arguments
    bool useDFS = (atoi(argv[1]) == 1);
    int k = atoi(argv[2]);

    // Read the map from the text file
    string filename = argv[3];
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return 1;
    }

    vector<vector<int>> sector_map;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        vector<int> row;
        int num;
        while (iss >> num) {
            row.push_back(num);
        }
        sector_map.push_back(row);
    }
    file.close();   

    // Find the largest colony
    vector<pair<int, int>> result = top_k_largest_colonies(sector_map, useDFS, k);

    // Print the result
    cout << "Algorithm: " << (useDFS ? "DFS" : "BFS") << endl;
    cout << "Map: " << filename << endl;

    // If no colonies are found
    if (result.empty()) {
        cout << "No colonies found." << endl;
        return 0;
    }

    // Print the top-k colonies
    for (size_t i = 0; i < result.size(); ++i) {
        cout << "Colony " << i + 1 << ": Size = " << result[i].first << ", Resource Type = " << result[i].second << endl;
    }

    return 0;
}