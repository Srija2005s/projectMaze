#include <stdio.h>
#include <stdlib.h>

#define MAX_ROW 100
#define MAX_COL 100

// Structure to represent a node in the adjacency list
struct Node {
    int x, y;
    struct Node* next;
};

// Structure to represent the maze
struct Maze {
    int rows, cols;
    struct Node* cells[MAX_ROW][MAX_COL];
};

// Function to create a new node for the adjacency list
struct Node* createNode(int x, int y) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->x = x;
    newNode->y = y;
    newNode->next = NULL;
    return newNode;
}

// Function to add an edge (neighbor) to the adjacency list
void addEdge(struct Maze* Maze, int x1, int y1, int x2, int y2) {
    struct Node* newNode = createNode(x2, y2);
    newNode->next = Maze->cells[x1][y1];
    Maze->cells[x1][y1] = newNode;
}

// Function to check if a point is inside the maze
int isInsideMaze(struct Maze* Maze, int x, int y) {
    return x >= 0 && x < Maze->rows && y >= 0 && y < Maze->cols;
}

// Function to check if a cell is a valid path
int isPath(struct Maze* Maze, int x, int y) {
    return isInsideMaze(Maze, x, y) && Maze->cells[x][y] != NULL;
}

// Function to read the maze from a file
void readMazeFromFile(struct Maze* Maze) {
    FILE *fptr;
    fptr = fopen("maze.txt","r");
    if (fptr == NULL) { perror("Error opening file"); exit(EXIT_FAILURE); }

    fscanf(fptr, "%d %d", &Maze->rows, &Maze->cols);

    for (int i = 0; i < Maze->rows; ++i)
        for (int j = 0; j < Maze->cols; ++j) {
            char cell;
            fscanf(fptr, " %c", &cell);
            Maze->cells[i][j] = (cell == '.') ? createNode(i, j) : NULL;
        }

    fclose(fptr);
}

// Function to print the maze
void printMaze(struct Maze* Maze) {
    for (int i = 0; i < Maze->rows; ++i) {
        for (int j = 0; j < Maze->cols; ++j) {
            if (Maze->cells[i][j] != NULL) {
                printf(". ");
            } else {
                printf("%c ", '#');
            }
        }
        printf("\n");
    }
}

// Function to perform breadth-first search
int breadthFirstSearch(struct Maze* Maze, int startRow, int startCol, int endRow, int endCol) {
    // Your BFS implementation using the adjacency list representation
    // Here, you can traverse the maze using BFS to find a path from (startX, startY) to (endX, endY)
    // Use the adjacency list structure to perform BFS
    int visited[MAX_ROW][MAX_COL] = {0};
        int dx[] = {0, 1, 0, -1}; // Right, Down, Left, Up
        int dy[] = {1, 0, -1, 0};

        struct Node* queue = createNode(startRow, startCol);
        visited[startRow][startCol] = 1;

        while (queue != NULL) {
            int currentX = queue->x;
            int currentY = queue->y;

            if (currentX == endRow && currentY == endCol) {
                // Destination reached
                return 1;
            }

            // Explore neighbors
            for (int i = 0; i < 4; ++i) {
                int nx = currentX + dx[i];
                int ny = currentY + dy[i];

                if (isPath(Maze, nx, ny) && !visited[nx][ny]) {
                    visited[nx][ny] = 1;
                    addEdge(Maze, currentX, currentY, nx, ny);

                    // Enqueue the neighbor
                    struct Node* newNode = createNode(nx, ny);
                    newNode->next = queue;
                    queue = newNode;
                }
            }

            // Dequeue the current node
            struct Node* temp = queue;
            queue = queue->next;
            free(temp);
        }

        return 0; // No path found
    }


   

// Main function
int main() {
   
    struct Maze Maze;
    for (int i = 0; i < MAX_ROW; ++i) {
           for (int j = 0; j < MAX_COL; ++j) {
               Maze.cells[i][j] = NULL;
           }
       }

    readMazeFromFile(&Maze);

    printf("Original Maze:\n");
    printMaze(&Maze);

    // Example: Call BFS from (0, 0) to (2, 3)
    int startRow = 0, startCol = 0;
    int endRow = 2, endCol = 3;

    if (breadthFirstSearch(&Maze, startRow, startCol, endRow, endCol)) {
        printf("Path found from (%d, %d) to (%d, %d)\n", startRow, startCol, endRow, endCol);
        // Print the path or perform additional actions
    } else {
        printf("No path found.\n");
    }

    return 0;
}
