#include <ncurses.h>
#include <math.h>

int main()
{
    // Initialize ncurses
    initscr();
    start_color();

    // Check if colors are supported
    if (!has_colors()) {
        endwin();
        printf("Colors are not supported on this terminal.\n");
        return 1;
    }

    // Enable color pairs
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);

    // Get window dimensions
    int width, height;
    getmaxyx(stdscr, height, width);

    // Calculate circle center coordinates
    int center_x = width / 2;
    int center_y = height / 2;

    // Calculate circle radius
    int radius = (width < height ? width : height) / 4;

    // Draw the circle
    for (int angle = 0; angle < 360; angle++) {
        // Calculate coordinates of a point on the circle
        int x = center_x + radius * cos(angle * M_PI / 180);
        int y = center_y + radius * sin(angle * M_PI / 180);

        // Set color based on the angle
        attron(COLOR_PAIR((angle % 6) + 1));

        // Display the circle point
        mvprintw(y, x, "o");

        // Refresh the screen
        refresh();
    }

    // Wait for a key press before exiting
    getch();

    // Clean up and exit
    endwin();
    return 0;
}
