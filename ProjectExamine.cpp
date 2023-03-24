#include <iostream>
#include <tuple>
#include <SDL.h>

using namespace std;


struct Point
{
    int x;
    int y;
};

class Entity
{
public:
    Entity(Point pos, uint16_t hp, uint8_t speed) : position_(pos), hp_(hp), speed_(speed) {};
    virtual void lvlup() {};
    virtual void Movement(const Uint8** state, SDL_Rect& entity)
    {
        if (state[SDL_SCANCODE_UP] and !state[SDL_SCANCODE_DOWN]) position_.y -= speed_;
        if (!state[SDL_SCANCODE_UP] and state[SDL_SCANCODE_DOWN]) position_.y += speed_;
        if (state[SDL_SCANCODE_RIGHT] and !state[SDL_SCANCODE_LEFT]) position_.x += speed_;
        if (!state[SDL_SCANCODE_RIGHT] and state[SDL_SCANCODE_LEFT]) position_.x -= speed_;
    };
    virtual std::pair<int, int> GetPos() { return { position_.x, position_.y }; }
protected:
    Point position_;
    uint16_t hp_;
    uint8_t speed_;
};

class Player : protected Entity
{
public:
    Player(Point coords, uint16_t hp, uint8_t speed) : Entity(coords, hp, speed) {};
    void Movement(const Uint8** state, SDL_Rect &entity) override
    {
        if (state[SDL_SCANCODE_UP] and !state[SDL_SCANCODE_DOWN]) position_.y -= speed_;
        if (!state[SDL_SCANCODE_UP] and state[SDL_SCANCODE_DOWN]) position_.y += speed_;
        if (state[SDL_SCANCODE_RIGHT] and !state[SDL_SCANCODE_LEFT]) position_.x += speed_;
        if (!state[SDL_SCANCODE_RIGHT] and state[SDL_SCANCODE_LEFT]) position_.x -= speed_;

        entity.x = position_.x; 
        entity.y = position_.y;
    };
    std::pair<int, int> GetPos() override { return {position_.x, position_.y }; }
private:
};


int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    int WIDTH = 1200, HEIGHT = 800;


    SDL_Window* window = SDL_CreateWindow("SDL GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event ev;
    const Uint8* state = SDL_GetKeyboardState(NULL);

    const uint8_t FPS = 60;
    const uint8_t PLAYER_SIZE = 15;
    Player player({ WIDTH / 2, HEIGHT - 50 }, 100, 1);
    SDL_Rect player_vis = { player.GetPos().first, player.GetPos().second, PLAYER_SIZE, PLAYER_SIZE};

    bool isRunning = true;
    while (isRunning)
    {
        while (SDL_PollEvent(& ev))
        {
            switch (ev.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (ev.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    isRunning = false;
                    break;
                }

            }
        }
        
        player.Movement(&state, player_vis);

        
        SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
        SDL_RenderClear(renderer);


        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &player_vis);




        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / FPS);
    }
    
    
    
    SDL_Quit();
    
    return 0;
}