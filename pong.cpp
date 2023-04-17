#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

void imageinit(SDL_Window *window, SDL_Renderer *renderer);

int main(int argc, char *args[])
{

    SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window;
    SDL_Renderer *renderer = NULL;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 0 ;
	}
    window = SDL_CreateWindow("first try", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    imageinit(window,renderer);

    return 0;
}

void imageinit(SDL_Window *window, SDL_Renderer *renderer){
    int a = 0;

    int width, height;
    SDL_GetWindowSize(window, &width, &height);

    SDL_Rect ballrect={width/2,height/2,20,20};
    SDL_Surface *ballsurface = IMG_Load("./images/ball.png");
	SDL_Texture *balltexture = SDL_CreateTextureFromSurface(renderer, ballsurface);

    SDL_Rect game={width/2-150,height/2,400,300};
    SDL_Surface *gamesurface = IMG_Load("./images/game over.png");
	SDL_Texture *gametexture = SDL_CreateTextureFromSurface(renderer, gamesurface);

    SDL_Rect winner={width/2-250,-50,500,500};
    SDL_Surface *winnersurface = IMG_Load("./images/winner2.png");
	SDL_Texture *winnertexture = SDL_CreateTextureFromSurface(renderer, winnersurface);

    SDL_Rect winner1={width/2-250,-50,500,500};
    SDL_Surface *winner1surface = IMG_Load("./images/winner1.png");
	SDL_Texture *winner1texture = SDL_CreateTextureFromSurface(renderer, winner1surface);


    SDL_Rect peddle1 = { 50,height/2-60, 7, 120 };
    SDL_Rect peddle2 = { width-50,height/2-60, 7, 120 };

    float ballspeedx = 250;
    float ballspeedy = 250;
    float paddle1speed = 600;
    float paddle2speed = 600;
    int change1 = 0;
    int change2 = 0;
    

    bool quit = false;

    while (!quit) {
        
        ballrect.x += float(ballspeedx*0.03);
        ballrect.y += float(ballspeedy *0.03);

        if(ballrect.y<0){
            
            ballrect.y = 0;
            ballspeedy *= -1;
        }

        if(ballrect.y>=height){
            ballrect.y = height;
            ballspeedy *= -1;
        }
      

        SDL_Event  event;
		while (SDL_PollEvent(&event)) {

            
            if(event.type==SDL_QUIT){
			   quit = true;
			   return;
		    }
            if(event.type==SDL_KEYDOWN){
                if(event.key.keysym.sym==SDLK_w){
                    change1 = -float(paddle1speed * 0.03);
                }
                if(event.key.keysym.sym==SDLK_s){
                    change1 = float(paddle1speed * 0.03);
                }
                if(event.key.keysym.sym==SDLK_DOWN){
                    change2 = float(paddle2speed * 0.03);
                }
                if(event.key.keysym.sym==SDLK_UP){
                    change2 = -float(paddle2speed * 0.03);
                }
            }
            if(event.type==SDL_KEYUP){
                if(event.key.keysym.sym==SDLK_w){
                    change1 = 0;
                }
                if(event.key.keysym.sym==SDLK_s){
                    change1 = 0;
                }
                if(event.key.keysym.sym==SDLK_DOWN){
                    change2 = 0;
                }
                if(event.key.keysym.sym==SDLK_UP){
                    change2 = 0;
                }
            }

            peddle1.y += change1;
            peddle2.y += change2;
            if(peddle1.y<=0){
                peddle1.y = 0;
            }
            if(peddle2.y<=0){
                peddle2.y = 0;
            }
            if(peddle2.y+120>=600){
                peddle2.y = 480;
            }
            if(peddle1.y+120>=600){
                peddle1.y = 480;
            }
        }
     SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(2);
	Mix_Chunk* popSound = Mix_LoadWAV("./music/pop.wav");

    if(SDL_HasIntersection(&ballrect, &peddle2)){
        Mix_PlayChannel(-1, popSound, 0);
        if(ballspeedx>0){
            if(ballspeedx>350){
                ballspeedx *= -0.9;
            }else
            ballspeedx *= -1.2f;   
        }
    }    
    if(SDL_HasIntersection(&ballrect, &peddle1)){
        Mix_PlayChannel(-1, popSound, 0);
        if(ballspeedx<0){
             if(ballspeedx>350){
                ballspeedx *=- 0.9;
            }else
            ballspeedx *= -1.2f; 
        }
    }    
    if(ballrect.x<=0){

        ballrect.y = -10000;
        ballrect.x = -10000;
        peddle1.x = -10000;
        peddle2.x = -10000;
        SDL_RenderCopy(renderer, gametexture, NULL,&game);
        SDL_RenderCopy(renderer, winnertexture, NULL,&winner);
    }

    if(ballrect.x>=width){
        
        ballrect.y = 10000;
        ballrect.x = 10000;
        peddle1.x = -10000;
        peddle2.x = -10000;
        SDL_RenderCopy(renderer, gametexture, NULL,&game);
        SDL_RenderCopy(renderer, winner1texture, NULL,&winner1);
        
       
    }



    //ball
    SDL_RenderCopy(renderer, balltexture, NULL,&ballrect);
    //peddle
    SDL_RenderFillRect(renderer, &peddle1);
    SDL_RenderFillRect(renderer, &peddle2);
    SDL_RenderPresent(renderer);
    

    SDL_Delay(30);
    }
}
