#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>

template <typename T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vectors must be of the same size.");
    }

    std::vector<T> result(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] + b[i];
    }
    return result;
}

template <typename T>
std::vector<T> operator-(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vectors must be of the same size.");
    }

    std::vector<T> result(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] - b[i];
    }
    return result;
}

void fillCircle(std::vector<float> norm_center, int winW, int winH, int radius, SDL_Renderer* renderer) {
	for (int i = 0; i < radius; i++){
		for(int j = 0; j <= 360; j ++) {
			float angle = j * M_PI / 180.0f;
			float x = norm_center[0]*winW + i * cos(angle);
			float y = norm_center[1]*winH + i * sin(angle);
			SDL_RenderPoint(renderer, x, y); 
		}
	}
}
void drawCircle(std::vector<float> norm_center, int winW, int winH, int radius, SDL_Renderer* renderer) {
	for(int j = 0; j <= 360; j ++) {
		float angle = j * M_PI / 180.0f;
		float x = norm_center[0]*winW + radius * cos(angle);
		float y = norm_center[1]*winH + radius * sin(angle);
		SDL_RenderPoint(renderer, x, y); 
	}

}

#ifndef RSC_PATH
#define RSC_PATH "./rsc" // default if not found
#endif


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

    std::string passwd = "1432";
    std::vector<float> m1 = {1/4.0f, 1/2.0f};
    std::vector<float> m2 = {3/4.0f, 1/2.0f};
    std::vector<float> m3 = {1/4.0f, 3/4.0f};
    std::vector<float> m4 = {3/4.0f, 3/4.0f};
    std::vector<float>* p1 = &m1;
    std::vector<float>* p2 = &m2;
    std::vector<float>* p3 = &m3;
    std::vector<float>* p4 = &m4;
    std::vector<std::vector<float>*> M = {
	    p1, p2, p3, p4
    }; 
    std::string passwd_check = "";

    int winW, winH;

    SDL_Color textColor = {255, 255, 255, 255};

    const std::string fontPath = std::string(RSC_PATH) + "/fonts/OpenSans-Bold.ttf";
    // std::cout << fontPath << std::endl;

    bool running = true;
    SDL_Event event;

    // Store last font size to avoid redundant reloads
    int lastFontSize = 0;
    TTF_Font* font = nullptr;
    // std::vector <float> pos_i = {0,0};
    // std::vector <float> pos_f = {0,0};
    std::vector <float> pos = {0,0};

    std::vector<std::vector<float>> checkedPoints;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
	    // if (event.tfinger.type == SDL_EVENT_FINGER_DOWN) {
	    //
	    //  pos_i[0] = event.tfinger.x;
	    //  pos_i[1] = event.tfinger.y;
	     pos[0] = event.tfinger.x;
	     pos[1] = event.tfinger.y;
	    //  std::cout << "iniziale\n";
	    //  std::cout << "posizione x: " << pos_i[0] << std::endl;
	    //  std::cout << "posizione y: " << pos_i[1] << std::endl;
	    //
	    // }
	    if (event.tfinger.type == SDL_EVENT_FINGER_MOTION) {
		    std::vector <float> diff = {event.tfinger.dx, event.tfinger.dy};
		    // std::cout << "spostamento dx: " << diff[0] << std::endl;
		    // std::cout << "spostamento dy: " << diff[1] << std::endl;

		    pos = pos + diff;

		    if (passwd_check.size() != passwd.size()) {
			    for (int i = 0; i<M.size(); i++) {
				    if (M[i]) {
					    std::vector<float> distance = *(M[i]) - pos;
					    float abs_diff = sqrt(pow(distance[0],2) + pow(distance[1],2));
					    // std::cout << "absolute difference: " << abs_diff << std::endl;
					    if (abs_diff < 0.05){
						    int num = i+1;
						    passwd_check = passwd_check + std::to_string(num);
						    // std::cout << "found " << i << std::endl;
						    checkedPoints.push_back(*(M[i]));
						    M[i] = NULL;
					    }
				    }
			    }
		    }
		    else {
			    if (passwd_check == passwd){
				    running = false;
			    }
		    }
	    }
	    if (event.tfinger.type == SDL_EVENT_FINGER_UP) {
		    // // Assuming a finger down event before!!
		    // pos_f[0] = event.tfinger.x;
		    // pos_f[1] = event.tfinger.y;
		    // std::cout << "finale\n";
		    // std::cout << "posizione x: " << pos_f[0] << std::endl;
		    // std::cout << "posizione y: " << pos_f[1] << std::endl;
		    //
		    // std::cout << "differenza\n";
		    // std::vector <float> delta = {0,0};
		    // delta = pos_f - pos_i;
		    // std::cout << "delta x: " << delta[0] << std::endl;
		    // std::cout << "delta y: " << delta[1] << std::endl;


		    p1 = &m1;
		    p2 = &m2;
		    p3 = &m3;
		    p4 = &m4;
		    M = {p1,p2,p3,p4}; 
		    passwd_check = "";
		    checkedPoints = {};

	    }
        }


	// Get current screen size
	SDL_GetWindowSize(window, &winW, &winH);

	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // background color + alpha
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	std::vector<std::vector<float>> grid = {m1,m2,m3,m4};
	for (int i = 0; i<grid.size(); i++) {
		drawCircle(grid[i], winW, winH, 10, renderer);
	}
	if (checkedPoints.size() != 0) {
		for (int i = 0; i<checkedPoints.size(); i++) {
			fillCircle(checkedPoints[i], winW, winH, 10, renderer);
		}
	}

	// Calculate font size relative to screen height
	int fontSize = winH / 12; 

	// Recreate font only if size changes
	if (fontSize != lastFontSize) {
            if (font) {
                TTF_CloseFont(font);
            }
            font = TTF_OpenFont(fontPath.c_str(), fontSize);
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
	std::ostringstream dateStream;
	dateStream << std::put_time(localTime, "%A %d %Y");
	std::string dateString = dateStream.str();

	// std::cout << "time: " << timeString << std::endl;
	// std::cout << "date: " << dateString << std::endl;

	// Render text with anti-aliasing
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, timeString.c_str(), 0, textColor);
	SDL_Surface* dateSurface = TTF_RenderText_Blended(font, dateString.c_str(), 0, textColor);
	if (textSurface && dateSurface) {
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		SDL_Texture* dateTexture = SDL_CreateTextureFromSurface(renderer, dateSurface);

		if (textTexture && dateTexture) {
			int textWidth = textSurface->w;
			int textHeight = textSurface->h;

			int dateWidth = dateSurface->w;
			int dateHeight = dateSurface->h;

			SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // background color + alpha

			SDL_FRect textRect = {
				(winW - textWidth) / 2.0f,
				(winH - 2.0f * textHeight) / 4.0f,
				(float)textWidth,
				(float)textHeight
			};
			SDL_FRect dateRect = {
				(winW - dateWidth) / 2.0f,
				(winH + 2.0f * dateHeight) / 4.0f,
				(float)dateWidth,
				(float)dateHeight
			};

			SDL_RenderTexture(renderer, textTexture, NULL, &textRect);
			SDL_RenderTexture(renderer, dateTexture, NULL, &dateRect);

			SDL_DestroyTexture(textTexture);
			SDL_DestroyTexture(dateTexture);
		}
		SDL_DestroySurface(textSurface);
		SDL_DestroySurface(dateSurface);
        }

	SDL_RenderPresent(renderer);


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

