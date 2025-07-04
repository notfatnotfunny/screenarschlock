#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() < 0) {
        SDL_Log("Failed to initialize TTF: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Time Display", 0, 0, SDL_WINDOW_FULLSCREEN);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    const char* fontPath = "/home/notfatnotfunny/Documents/codeIsFun/screenlock/OpenSans-Bold.ttf";
    SDL_Color textColor = {255, 255, 255, 255};

    bool running = true;
    SDL_Event event;

    // Store last font size to avoid redundant reloads
    int lastFontSize = 0;
    TTF_Font* font = nullptr;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        // Get current screen size
        int winW, winH;
        SDL_GetWindowSize(window, &winW, &winH);

        // Calculate font size relative to screen height
        int fontSize = winH / 8; // Try 10, 12, or even 6 depending on look

        // Recreate font only if size changes
        if (fontSize != lastFontSize) {
            if (font) {
                TTF_CloseFont(font);
            }
            font = TTF_OpenFont(fontPath, fontSize);
            if (!font) {
                SDL_Log("Failed to load font: %s", SDL_GetError());
                break;
            }
            lastFontSize = fontSize;
        }

        // Get time string
        std::time_t now = std::time(nullptr);
        std::tm* localTime = std::localtime(&now);
        std::ostringstream timeStream;
        timeStream << std::put_time(localTime, "%H:%M:%S");
        std::string timeString = timeStream.str();

        // Render text with anti-aliasing
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, timeString.c_str(), 0, textColor);
        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (textTexture) {
                int textWidth = textSurface->w;
                int textHeight = textSurface->h;

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                SDL_FRect destRect = {
                    (winW - textWidth) / 2.0f,
                    (winH - textHeight) / 2.0f,
                    (float)textWidth,
                    (float)textHeight
                };

                SDL_RenderTexture(renderer, textTexture, NULL, &destRect);
                SDL_RenderPresent(renderer);

                SDL_DestroyTexture(textTexture);
            }
            SDL_DestroySurface(textSurface);
        }

        SDL_Delay(100);
    }

    // Cleanup
    if (font) TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

