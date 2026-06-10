#include <stdio.h>
#include <stdlib.h>

#define WIDTH 60
#define HEIGHT 30
#define MAX_SHAPES 100

// Define shape types
typedef enum { LINE, RECTANGLE, CIRCLE, TRIANGLE } ShapeType;

// Structure to hold shape data
typedef struct {
    int id;
    ShapeType type;
    int x1, y1, x2, y2, x3, y3; // Coordinates
    int r;                      // Radius for circle
    int active;                 // 1 if active, 0 if deleted
} Shape;

// Global Variables
char canvas[HEIGHT][WIDTH];
Shape shapes[MAX_SHAPES];
int shapeCount = 0;
int nextId = 1;

// --- Basic Canvas Operations ---

void initCanvas() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            canvas[i][j] = '_';
        }
    }
}

void displayCanvas() {
    printf("\n--- Current Canvas ---\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c ", canvas[i][j]);
        }
        printf("\n");
    }
    printf("----------------------\n");
}

void drawPoint(int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        canvas[y][x] = '*';
    }
}

// --- Drawing Algorithms ---

// Bresenham's Line Algorithm
void drawLineAlgo(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        drawPoint(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

// 8-Way Symmetry for Circle
void drawCirclePoints(int xc, int yc, int x, int y) {
    drawPoint(xc + x, yc + y); drawPoint(xc - x, yc + y);
    drawPoint(xc + x, yc - y); drawPoint(xc - x, yc - y);
    drawPoint(xc + y, yc + x); drawPoint(xc - y, yc + x);
    drawPoint(xc + y, yc - x); drawPoint(xc - y, yc - x);
}

// Midpoint Circle Algorithm
void drawCircleAlgo(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 3 - 2 * r;
    drawCirclePoints(xc, yc, x, y);
    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        drawCirclePoints(xc, yc, x, y);
    }
}

// --- Shape Renderers ---

void renderShapes() {
    initCanvas(); // Clear canvas
    for (int i = 0; i < shapeCount; i++) {
        if (!shapes[i].active) continue;
        
        Shape s = shapes[i];
        if (s.type == LINE) {
            drawLineAlgo(s.x1, s.y1, s.x2, s.y2);
        } else if (s.type == RECTANGLE) {
            drawLineAlgo(s.x1, s.y1, s.x2, s.y1); // Top
            drawLineAlgo(s.x2, s.y1, s.x2, s.y2); // Right
            drawLineAlgo(s.x2, s.y2, s.x1, s.y2); // Bottom
            drawLineAlgo(s.x1, s.y2, s.x1, s.y1); // Left
        } else if (s.type == CIRCLE) {
            drawCircleAlgo(s.x1, s.y1, s.r);
        } else if (s.type == TRIANGLE) {
            drawLineAlgo(s.x1, s.y1, s.x2, s.y2);
            drawLineAlgo(s.x2, s.y2, s.x3, s.y3);
            drawLineAlgo(s.x3, s.y3, s.x1, s.y1);
        }
    }
}

// --- Menu Operations ---

void addShape() {
    if (shapeCount >= MAX_SHAPES) {
        printf("Canvas is full!\n");
        return;
    }

    int choice;
    printf("\nSelect Shape to Add:\n1. Line\n2. Rectangle\n3. Circle\n4. Triangle\nChoice: ");
    scanf("%d", &choice);

    Shape s;
    s.id = nextId++;
    s.active = 1;

    switch (choice) {
        case 1:
            s.type = LINE;
            printf("Enter x1, y1, x2, y2: ");
            scanf("%d %d %d %d", &s.x1, &s.y1, &s.x2, &s.y2);
            break;
        case 2:
            s.type = RECTANGLE;
            printf("Enter Top-Left (x1, y1) and Bottom-Right (x2, y2): ");
            scanf("%d %d %d %d", &s.x1, &s.y1, &s.x2, &s.y2);
            break;
        case 3:
            s.type = CIRCLE;
            printf("Enter Center (x, y) and Radius (r): ");
            scanf("%d %d %d", &s.x1, &s.y1, &s.r);
            break;
        case 4:
            s.type = TRIANGLE;
            printf("Enter 3 points (x1 y1 x2 y2 x3 y3): ");
            scanf("%d %d %d %d %d %d", &s.x1, &s.y1, &s.x2, &s.y2, &s.x3, &s.y3);
            break;
        default:
            printf("Invalid choice!\n");
            return;
    }
    
    shapes[shapeCount++] = s;
    renderShapes();
    printf("Shape Added with ID: %d\n", s.id);
}

void deleteShape() {
    int id;
    printf("Enter Shape ID to delete: ");
    scanf("%d", &id);

    int found = 0;
    for (int i = 0; i < shapeCount; i++) {
        if (shapes[i].id == id && shapes[i].active) {
            shapes[i].active = 0;
            found = 1;
            break;
        }
    }
    
    if (found) {
        renderShapes();
        printf("Shape %d deleted successfully.\n", id);
    } else {
        printf("Shape ID not found or already deleted.\n");
    }
}

void modifyShape() {
    int id;
    printf("Enter Shape ID to modify: ");
    scanf("%d", &id);

    for (int i = 0; i < shapeCount; i++) {
        if (shapes[i].id == id && shapes[i].active) {
            printf("Modifying Shape %d...\n", id);
            if (shapes[i].type == LINE) {
                printf("Enter new x1, y1, x2, y2: ");
                scanf("%d %d %d %d", &shapes[i].x1, &shapes[i].y1, &shapes[i].x2, &shapes[i].y2);
            } else if (shapes[i].type == RECTANGLE) {
                printf("Enter new Top-Left (x1, y1) and Bottom-Right (x2, y2): ");
                scanf("%d %d %d %d", &shapes[i].x1, &shapes[i].y1, &shapes[i].x2, &shapes[i].y2);
            } else if (shapes[i].type == CIRCLE) {
                printf("Enter new Center (x, y) and Radius (r): ");
                scanf("%d %d %d", &shapes[i].x1, &shapes[i].y1, &shapes[i].r);
            } else if (shapes[i].type == TRIANGLE) {
                printf("Enter new 3 points (x1 y1 x2 y2 x3 y3): ");
                scanf("%d %d %d %d %d %d", &shapes[i].x1, &shapes[i].y1, &shapes[i].x2, &shapes[i].y2, &shapes[i].x3, &shapes[i].y3);
            }
            renderShapes();
            printf("Shape %d modified successfully.\n", id);
            return;
        }
    }
    printf("Shape ID not found or is inactive.\n");
}

void listShapes() {
    printf("\n--- Active Shapes ---\n");
    for (int i = 0; i < shapeCount; i++) {
        if (shapes[i].active) {
            printf("ID: %d | Type: ", shapes[i].id);
            if (shapes[i].type == LINE) printf("Line");
            else if (shapes[i].type == RECTANGLE) printf("Rectangle");
            else if (shapes[i].type == CIRCLE) printf("Circle");
            else if (shapes[i].type == TRIANGLE) printf("Triangle");
            printf("\n");
        }
    }
    printf("---------------------\n");
}

// --- Main Program Loop ---

int main() {
    initCanvas();
    int choice;

    while (1) {
        printf("\n=== 2D Graphics Editor ===\n");
        printf("1. Add Shape\n");
        printf("2. Delete Shape\n");
        printf("3. Modify Shape\n");
        printf("4. Display Canvas\n");
        printf("5. List Active Shapes (IDs)\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addShape(); break;
            case 2: deleteShape(); break;
            case 3: modifyShape(); break;
            case 4: displayCanvas(); break;
            case 5: listShapes(); break;
            case 6: 
                printf("Exiting editor...\n");
                return 0;
            default: printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}