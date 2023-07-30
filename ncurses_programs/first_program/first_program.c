#include <ncurses.h>
#include <ncurses/curses.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    initscr();
    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal doesn't support colors!!\n");
        exit(1);
    }
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLUE);

    attron(COLOR_PAIR(1));
    mvwprintw(stdscr, 10, 10, "%s", "Hello World, in color!!");
    refresh();
    attroff(COLOR_PAIR(1));
    getch();
    endwin();

    return 0;
}
