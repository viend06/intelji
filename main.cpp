#include "include/game.h"
#include "include/utils.h"
#include <SDL2/SDL_ttf.h>  
int main(int argc, char* argv[]) {
    srand(time(0)); 
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();  
    window = SDL_CreateWindow("Memory Card Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenwidth, screenheight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
     menubackground = loadTexture("assets/image.png"); 
    backgroundTexture = loadTexture("assets/background.jpg");
    anhnutstart = loadTexture("assets/Button.png");
    helpButtonTexture = loadTexture("assets/Button.png");
    backTexture = loadTexture("assets/catback.png");
    smallbackground = loadTexture("assets/small_background.png");
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("Không thể khởi tạo SDL_mixer: %s", Mix_GetError());
        return -1;
    }
    nhacnen = Mix_LoadMUS("assets/soundbackground.mp3");
    cardFlipSound = Mix_LoadWAV("assets/cardflip.ogg");
    gameOverSound = Mix_LoadWAV("assets/gameover.wav");
    Mix_VolumeMusic(30);  
    Mix_Volume(-1, 64);   
    vector<string> imagePaths = {"assets/cat.jpg", "assets/cat1.jpg", "assets/cat2.jpg", "assets/cat3.jpg", "assets/cat4.jpg", "assets/cat5.jpg", "assets/cat6.jpg", "assets/cat7.jpg","assets/cat8.jpg","assets/cat9.jpg"};//
    gameOverTexture = loadTexture("assets/Gameover.png"); 
    shuffleCards(imagePaths);
    startTime = SDL_GetTicks();
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (showMenu&&!showHelpScreen) {
                    if (x >= startButtonRect.x && x <= startButtonRect.x + startButtonRect.w &&
                        y >= startButtonRect.y && y <= startButtonRect.y + startButtonRect.h) {
                        showMenu = false;
                        startTime = SDL_GetTicks();
                        timeUp = false;
                        shuffleCards(imagePaths); 
                        if (nhacnen) {
                            Mix_PlayMusic(nhacnen, -1); 
                            Mix_VolumeMusic(MIX_MAX_VOLUME / 2); 
                        }
                    }else if(x >= helpButtonRect.x && x <= helpButtonRect.x+helpButtonRect.w&& y >= helpButtonRect.y && y <= helpButtonRect.y+helpButtonRect.h){// kiểm tra click nút help 
                        showHelpScreen = true;
                    }
                }else if (showHelpScreen){
                    showHelpScreen = false;
                }
                else if (!timeUp) {
                    handleMouseClick(event.button.x, event.button.y);
                }
            }
        }
        if(showHelpScreen){
            renderHelpScreen();
        }
        else if (showMenu) {
            renderMenu(); 
        }
        else {
            updateGame(); 
            renderGame();
            if (timeUp && !showMenu) {
                renderGameOverScreen();
                handleGameOverEvents(running, showMenu);
            }
        }
        SDL_RenderPresent(renderer); 
        SDL_Delay(16);
    }
    TTF_Quit();  
    SDL_Quit();
    return 0;
}