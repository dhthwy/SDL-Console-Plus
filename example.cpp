#include <stdio.h>
#include <thread>
#include <atomic>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "SDL_console.h"

void thread_fun(std::atomic<Console_tty *> &ttyref);

static std::atomic<bool> do_draw{true};

int
main (int argc, char **argv)
{
    std::atomic<Console_tty *> tty{nullptr};
    std::thread th(thread_fun, std::ref(tty));

    // Wait for initialization
    int ms = 1;
    while(tty == nullptr) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        printf("waiting...%dms\r", ms);
        ms++;
    }
    printf("\nConsole is ready.\n");
    printf("Window supports %d columns and %d rows of text.\n", Console_GetColumns(tty), Console_GetRows(tty));

    while(1) {
        std::string buf;
        if (Console_ReadLine(tty, buf)) {
            if (buf == "die") {
                Console_Die(tty);
                break;
            } else if (buf == "clear") {
                Console_Clear(tty);
            } else if (buf == "test") {
                Console_AddLine(tty, "❤ ♥ Really long output! \r \bLorem ipsum dolor sit amet, consectetur adipiscing elit. Sed tincidunt, odio quis pulvinar suscipit, dolor nibh lobortis massa, quis sollicitudin ipsum sapien nec leo. Donec id sem sapien. Quisque dignissim eget sem ac bibendum. Suspendisse aliquam est finibus tellus molestie faucibus. Vestibulum volutpat feugiat nulla ut pharetra. Etiam facilisis, nunc in ullamcorper tempus, velit ante molestie turpis, at aliquet orci odio in arcu. Aenean dignissim dolor libero, et rhoncus felis elementum hendrerit. Donec aliquam accumsan nunc, vitae tempor sem tristique non. Duis at velit libero. Fusce ac justo vel leo lacinia vehicula sed vel felis. Nullam lacus orci, faucibus eu dapibus nec, gravida quis dui. Fusce faucibus, eros eu dignissim pharetra, velit velit imperdiet urna, gravida commodo est arcu eget lectus. Nunc leo ipsum, maximus vel dictum sit amet, maximus vitae arcu. Donec suscipit elit nec dolor lobortis rhoncus ♥ ❤");
            } else {
                // echo
                Console_AddLine(tty, buf.c_str());
            }
        }
    }
    th.join();
    printf("Console shutdown successfully\n");
    return 0;
}

void thread_fun(std::atomic<Console_tty *> &ttyref) {
    SDL_Event     e;
    SDL_Window   *window = NULL;
    SDL_GLContext glContext = NULL;
    Console_tty  *tty = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL Failed to init: %s\n", SDL_GetError());
        exit(1);
    }

    tty = Console_Create(
            "Console",
            "> ",         /* SDL_Window */
            14              /* font size */
            );

    ttyref = tty;
    SDL_StartTextInput();

    while(1) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                goto quit;
                break;
            }

        }

        /* Draw your program here */

        if (Console_Draw(tty)) { /* handle drawing the console if toggled */
            fprintf(stderr, "%s\n", Console_GetError());
            /* handle fatal console error */
            goto quit;
        }

        SDL_Delay(100);
    }

    quit:
    SDL_Quit();
}
