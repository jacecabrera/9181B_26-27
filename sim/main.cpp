#include <SDL2/SDL.h>
#include "lvgl/lvgl.h"
#include <cstdio>

// Forward declaration of your auton selector initializer
void auton_selector_init(void);

// SDL texture for rendering LVGL buffer
static SDL_Texture *sdl_texture = nullptr;

// LVGL display flush callback
static void sdl_display_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p) {
    SDL_Rect rect = {
        .x = area->x1,
        .y = area->y1,
        .w = (area->x2 - area->x1 + 1),
        .h = (area->y2 - area->y1 + 1)
    };

    SDL_UpdateTexture(sdl_texture, &rect, color_p, rect.w * sizeof(lv_color_t));
    lv_disp_flush_ready(disp_drv);
}

// SDL mouse input driver callback for LVGL touch simulation
static void sdl_mouse_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data) {
    int mouse_x, mouse_y;
    Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

    data->point.x = mouse_x;
    data->point.y = mouse_y;
    data->state = (buttons & SDL_BUTTON_LMASK) ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
}

int main(int argc, char **argv) {
    // 1. Initialize SDL and LVGL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }
    lv_init();

    // 2. Create the 480x240 V5 Brain window
    SDL_Window * window = SDL_CreateWindow("VEX V5 Brain Simulator",
                                         SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                         480, 240, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    sdl_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565, 
                                    SDL_TEXTUREACCESS_STREAMING, 480, 240);

    // 3. Set up LVGL display buffer and driver
    static lv_disp_draw_buf_t disp_buf;
    static lv_color_t buf_1[480 * 240];
    lv_disp_draw_buf_init(&disp_buf, buf_1, NULL, 480 * 240);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 480;
    disp_drv.ver_res = 240;
    disp_drv.flush_cb = sdl_display_flush;
    disp_drv.draw_buf = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    // 4. Set up SDL Mouse Input Driver
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = sdl_mouse_read;
    lv_indev_drv_register(&indev_drv);

    // 5. Load your Auton Selector UI
    auton_selector_init();

    // 6. Desktop simulator main loop
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Drive LVGL timers, animations, and updates
        lv_timer_handler();

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, sdl_texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Delay(5); // ~5ms frame pacing
    }

    // Cleanup
    SDL_DestroyTexture(sdl_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}