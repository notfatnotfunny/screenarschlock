#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <bits/stdc++.h>


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

    SDL_Color textColor = {255, 255, 255, 255};

    std::ifstream rsc_file("../rsc_path.txt");
    if (!rsc_file.is_open()) {
	    std::cout << "error opening rsc file\n";
    }

    std::string rsc_path_str;
    std::getline(rsc_file, rsc_path_str);

    // std::cout << rsc_path_str << std::endl;

    rsc_path_str = rsc_path_str + "/rsc/fonts/OpenSans-Bold.ttf";
    const char* fontPath = rsc_path_str.c_str();

    bool running = true;
    SDL_Event event;

    // Store last font size to avoid redundant reloads
    int lastFontSize = 0;
    TTF_Font* font = nullptr;
    float xi;
    float yi;
    float xf;
    float yf;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
	    if (event.tfinger.type == SDL_EVENT_FINGER_DOWN) {
		    xi = event.tfinger.x;
		    yi = event.tfinger.y;
		    std::cout << "iniziale\n";
		    std::cout << "posizione x: " << xi << std::endl;
		    std::cout << "posizione y: " << yi << std::endl;

	    }
	    if(event.tfinger.type == SDL_EVENT_FINGER_UP) {
		    // Assuming a finger down event before!!
		    xf = event.tfinger.x;
		    yf = event.tfinger.y;
		    std::cout << "finale\n";
		    std::cout << "posizione x: " << xf << std::endl;
		    std::cout << "posizione y: " << yf << std::endl;

		    std::cout << "differenza\n";
		    float delta_x = xf - xi;
		    float delta_y = yf - yi;
		    std::cout << "delta x: " << delta_x << std::endl;
		    std::cout << "delta y: " << delta_y << std::endl;


		    if (delta_y < -0.4){  
			    running = false;
		    }
	    }
        }

        // Get current screen size
        int winW, winH;
        SDL_GetWindowSize(window, &winW, &winH);

        // Calculate font size relative to screen height
        int fontSize = winH / 10; // Try 10, 12, or even 6 depending on look

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

