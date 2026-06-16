#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 60
#define HEIGHT 20
#define MAX_OBJS 100

// ANSI Color Codes for Aesthetics
#define COLOR_RESET   "\x1b[0m"
#define COLOR_CANVAS  "\x1b[90m" // Dark Gray
#define COLOR_DRAW    "\x1b[36m" // Cyan
#define COLOR_MENU    "\x1b[33m" // Yellow
#define COLOR_ERROR   "\x1b[31m" // Red
#define COLOR_SUCCESS "\x1b[32m" // Green

typedef enum { LINE, RECT, CIRCLE, TRIANGLE } ObjType;

typedef struct {
    int id;
    int active;
    ObjType type;
    int x1, y1, x2, y2, x3, y3, r; // Coordinates and radius
} Object;

Object display_list[MAX_OBJS];
int next_id = 1;
char canvas[HEIGHT][WIDTH];

// --- RASTERIZATION ENGINE ---

void put_pixel(int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        canvas[y][x] = '*';
    }
}

// Bresenham's Line Algorithm
void draw_line(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
    int err = dx + dy, e2;
    for (;;) {
        put_pixel(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void draw_rect(int x1, int y1, int x2, int y2) {
    draw_line(x1, y1, x2, y1); // Top
    draw_line(x1, y2, x2, y2); // Bottom
    draw_line(x1, y1, x1, y2); // Left
    draw_line(x2, y1, x2, y2); // Right
}

// Midpoint Circle Algorithm
void draw_circle(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 3 - 2 * r;
    while (y >= x) {
        put_pixel(xc+x, yc+y); put_pixel(xc-x, yc+y);
        put_pixel(xc+x, yc-y); put_pixel(xc-x, yc-y);
        put_pixel(xc+y, yc+x); put_pixel(xc-y, yc+x);
        put_pixel(xc+y, yc-x); put_pixel(xc-y, yc-x);
        x++;
        if (d > 0) { y--; d = d + 4 * (x - y) + 10; }
        else { d = d + 4 * x + 6; }
    }
}

void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    draw_line(x1, y1, x2, y2);
    draw_line(x2, y2, x3, y3);
    draw_line(x3, y3, x1, y1);
}

// --- STATE MANAGEMENT ---

void render_canvas() {
    // 1. Clear canvas
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            canvas[y][x] = '_';

    // 2. Draw all active objects
    for (int i = 0; i < MAX_OBJS; i++) {
        if (!display_list[i].active) continue;
        Object obj = display_list[i];
        switch(obj.type) {
            case LINE: draw_line(obj.x1, obj.y1, obj.x2, obj.y2); break;
            case RECT: draw_rect(obj.x1, obj.y1, obj.x2, obj.y2); break;
            case CIRCLE: draw_circle(obj.x1, obj.y1, obj.r); break;
            case TRIANGLE: draw_triangle(obj.x1, obj.y1, obj.x2, obj.y2, obj.x3, obj.y3); break;
        }
    }

    // 3. Print canvas with colors
    printf("\n");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (canvas[y][x] == '*') printf(COLOR_DRAW "%c " COLOR_RESET, canvas[y][x]);
            else printf(COLOR_CANVAS "%c " COLOR_RESET, canvas[y][x]);
        }
        printf("\n");
    }
}

int find_free_slot() {
    for (int i = 0; i < MAX_OBJS; i++) {
        if (!display_list[i].active) return i;
    }
    return -1;
}

void add_object() {
    int slot = find_free_slot();
    if (slot == -1) {
        printf(COLOR_ERROR "Error: Canvas full!\n" COLOR_RESET);
        return;
    }

    int choice;
    printf(COLOR_MENU "\nSelect Shape (0:Line, 1:Rect, 2:Circle, 3:Triangle): " COLOR_RESET);
    scanf("%d", &choice);

    Object new_obj = { .id = next_id++, .active = 1, .type = choice };

    switch(choice) {
        case LINE:
        case RECT:
            printf("Enter x1 y1 x2 y2: ");
            scanf("%d %d %d %d", &new_obj.x1, &new_obj.y1, &new_obj.x2, &new_obj.y2);
            break;
        case CIRCLE:
            printf("Enter center x y and radius r: ");
            scanf("%d %d %d", &new_obj.x1, &new_obj.y1, &new_obj.r);
            break;
        case TRIANGLE:
            printf("Enter x1 y1 x2 y2 x3 y3: ");
            scanf("%d %d %d %d %d %d", &new_obj.x1, &new_obj.y1, &new_obj.x2, &new_obj.y2, &new_obj.x3, &new_obj.y3);
            break;
        default:
            printf(COLOR_ERROR "Invalid shape!\n" COLOR_RESET);
            return;
    }
    display_list[slot] = new_obj;
    printf(COLOR_SUCCESS "Object ID %d added successfully.\n" COLOR_RESET, new_obj.id);
}

void delete_object(int id) {
    for (int i = 0; i < MAX_OBJS; i++) {
        if (display_list[i].active && display_list[i].id == id) {
            display_list[i].active = 0;
            printf(COLOR_SUCCESS "Object ID %d deleted.\n" COLOR_RESET, id);
            return;
        }
    }
    printf(COLOR_ERROR "Object ID %d not found.\n" COLOR_RESET, id);
}

// --- MAIN MENU LOOP ---

int main() {
    for(int i=0; i<MAX_OBJS; i++) display_list[i].active = 0; // Initialize list
    
    int choice;
    do {
        render_canvas();
        printf(COLOR_MENU "\n--- 2D ASCII GRAPHICS EDITOR ---\n" COLOR_RESET);
        printf("1. Add Object\n2. Delete Object\n3. Modify Object\n4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        int id;
        switch(choice) {
            case 1: add_object(); break;
            case 2: 
                printf("Enter Object ID to delete: ");
                scanf("%d", &id);
                delete_object(id);
                break;
            case 3:
                printf("Enter Object ID to modify: ");
                scanf("%d", &id);
                delete_object(id); // Simple modification: delete old, prompt for new
                printf(COLOR_MENU "Please enter the new parameters for the object:\n" COLOR_RESET);
                add_object();
                break;
            case 4: printf("Exiting engine...\n"); break;
            default: printf(COLOR_ERROR "Invalid choice!\n" COLOR_RESET);
        }
    } while (choice != 4);

    return 0;
}