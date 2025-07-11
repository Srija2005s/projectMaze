#include <stdio.h>
#include <stdlib.h>

#define MAX_ROW 100
#define MAX_COL 100

struct Node {
    int x, y;
    struct Node* next;
};

struct Maze {
    int rows, cols;
    struct Node* cells[MAX_ROW][MAX_COL];
};

struct Node* createNode(int x, int y) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->x = x;
    newNode->y = y;
    newNode->next = NULL;
    return newNode;
}

int isInsideMaze(struct Maze* Maze, int x, int y) {
    return x >= 0 && x < Maze->rows && y >= 0 && y < Maze->cols;
}

int isPath(struct Maze* Maze, int x, int y) {
    return isInsideMaze(Maze, x, y) && Maze->cells[x][y] != NULL;
}

void readMazeFromFile(struct Maze* Maze) {
    FILE *fptr = fopen("maze.txt", "r");
    if (fptr == NULL) { perror("Error opening file"); exit(EXIT_FAILURE); }

    fscanf(fptr, "%d %d", &Maze->rows, &Maze->cols);
    for (int i = 0; i < Maze->rows; ++i) {
        for (int j = 0; j < Maze->cols; ++j) {
            char cell;
            fscanf(fptr, " %c", &cell);
            Maze->cells[i][j] = (cell == '.') ? createNode(i, j) : NULL;
        }
    }
    fclose(fptr);
}

int breadthFirstSearch(struct Maze* Maze, int startRow, int startCol, int endRow, int endCol, int path[][2], int* pathLen) {
    int visited[MAX_ROW][MAX_COL] = {0};
    int parent[MAX_ROW][MAX_COL][2];

    int dx[] = {0, 1, 0, -1};
    int dy[] = {1, 0, -1, 0};

    struct Node* queue[MAX_ROW * MAX_COL];
    int front = 0, rear = 0;

    queue[rear++] = createNode(startRow, startCol);
    visited[startRow][startCol] = 1;
    parent[startRow][startCol][0] = -1;
    parent[startRow][startCol][1] = -1;

    while (front < rear) {
        struct Node* curr = queue[front++];
        int x = curr->x;
        int y = curr->y;
        free(curr);

        if (x == endRow && y == endCol) {
            int px = x, py = y;
            *pathLen = 0;
            while (px != -1 && py != -1) {
                path[(*pathLen)][0] = px;
                path[(*pathLen)++][1] = py;
                int tx = parent[px][py][0];
                int ty = parent[px][py][1];
                px = tx; py = ty;
            }
            return 1;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i], ny = y + dy[i];
            if (isPath(Maze, nx, ny) && !visited[nx][ny]) {
                visited[nx][ny] = 1;
                parent[nx][ny][0] = x;
                parent[nx][ny][1] = y;
                queue[rear++] = createNode(nx, ny);
            }
        }
    }
    return 0;
}

void writeMazeJSON(struct Maze* Maze, int startRow, int startCol, int endRow, int endCol, int path[][2], int pathLen) {
    FILE *out = fopen("maze.json", "w");
    if (!out) { perror("Cannot write maze.json"); exit(1); }

    fprintf(out, "{\n  \"maze\": [\n");
    for (int i = 0; i < Maze->rows; ++i) {
        fprintf(out, "    [");
        for (int j = 0; j < Maze->cols; ++j) {
            fprintf(out, "\"%c\"", Maze->cells[i][j] ? '.' : '#');
            if (j < Maze->cols - 1) fprintf(out, ",");
        }
        fprintf(out, "]");
        if (i < Maze->rows - 1) fprintf(out, ",");
        fprintf(out, "\n");
    }
    fprintf(out, "  ],\n");

    fprintf(out, "  \"start\": [%d, %d],\n", startRow, startCol);
    fprintf(out, "  \"end\": [%d, %d],\n", endRow, endCol);
    fprintf(out, "  \"path\": [\n");
    for (int i = pathLen - 1; i >= 0; --i) {
        fprintf(out, "    [%d, %d]%s\n", path[i][0], path[i][1], i ? "," : "");
    }
    fprintf(out, "  ]\n}\n");

    fclose(out);
}

int main() {
    struct Maze Maze;
    for (int i = 0; i < MAX_ROW; ++i)
        for (int j = 0; j < MAX_COL; ++j)
            Maze.cells[i][j] = NULL;

    readMazeFromFile(&Maze);

    int startRow = 0, startCol = 0;
    int endRow = Maze.rows - 1, endCol = Maze.cols - 1;

    int path[MAX_ROW * MAX_COL][2], pathLen = 0;
    int found = breadthFirstSearch(&Maze, startRow, startCol, endRow, endCol, path, &pathLen);

    if (found)
        printf("Path found.\n");
    else
        printf("No path found.\n");

    writeMazeJSON(&Maze, startRow, startCol, endRow, endCol, path, pathLen);

    return 0;
}
