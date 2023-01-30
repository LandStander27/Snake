#include <iostream>
#include <SDL2\SDL.h>
#include "SDL2\SDL_Main.h"
#include <SDL2\SDL_ttf.h>
#include <SDL2\SDL_mixer.h>
#include <deque>
#include <windows.h>
#include <vector>
#include <string>
#include <cmath>

typedef int64_t msec_t;

const int width = 1000;
const int height = 1000;

const int block_size = 20;

const long start_time = SDL_GetTicks();

long time_ms() {
	return GetTickCount();
}

int main(int argc, char* argv[]) {

	std::cout << "SDL_mixer init" << std::endl;
	if (Mix_Init(MIX_INIT_MP3) == -1) {
		std::cout << "Unable to init SDL2_mixer: " << SDL_GetError() << std::endl;
		return 1;
	}

	if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		std::cout << "Unable to open audio: " << Mix_GetError() << std::endl;
		return 1;
	}

	Mix_Chunk *music = Mix_LoadWAV("assets\\music.wav");
	if (!music) {
		std::cout << "Unable to open music: " << Mix_GetError() << std::endl;
		return 1;
	}

	std::cout << "Starting music" << std::endl;
	Mix_PlayChannel(-1, music, -1);

	std::cout << "SDL init" << std::endl;
	SDL_Init(SDL_INIT_EVERYTHING);

	std::cout << "SDL_ttf init" << std::endl;
	if (TTF_Init() == -1) {
		std::cout << "Unable to init SDL2_ttf: " << SDL_GetError() << std::endl;
		return 1;
	}



	std::cout << "Window init" << std::endl;
	SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_ALLOW_HIGHDPI);

	if (NULL == window) {
		std::cout << "Unable to create window: " << SDL_GetError() << std::endl;
		return 1;
	}

	std::cout << "Renderer init" << std::endl;
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	if (NULL == renderer) {
		std::cout << "Unable to create renderer: " << SDL_GetError() << std::endl;
		return 1;
	}

	TTF_Font* font = TTF_OpenFont("assets\\font.ttf", 12);
	if (!font) {
		std::cout << "Unable to open font: " << TTF_GetError() << std::endl;
		return 1;
	}

	std::cout << "Setting up game" << std::endl;

	SDL_Event event;

	std::deque<SDL_Rect> snake;

	std::deque<SDL_Rect> menu_snake;

	// int size = 1;

	std::vector<SDL_Rect> apples;

	for (int i = 0; i < 2; i++) {
		apples.push_back( SDL_Rect {
			rand()%40*25,
			rand()%40*25,
			block_size,
			block_size
		});
	}

	std::vector<SDL_Rect> menu_apples;

	for (int i = 0; i < 2; i++) {
		menu_apples.push_back( SDL_Rect {
			rand()%40*25,
			rand()%40*25,
			block_size,
			block_size
		});
	}
	
	bool running = true;

	

	int dir = 0;
	int last_dir = 0;

	int menu_dir = 0;

	long last_update = time_ms();

	SDL_Rect head {
		500,
		500,
		block_size,
		block_size
	};

	snake.push_back(head);
	snake.push_front(SDL_Rect {
		500-25,
		500,
		block_size,
		block_size
	});
	snake.push_front(SDL_Rect {
		500-50,
		500,
		block_size,
		block_size
	});

	SDL_Rect menu_head {
		500,
		500,
		block_size,
		block_size
	};

	menu_snake.push_back(menu_head);
	menu_snake.push_front(SDL_Rect {
		500-25,
		500,
		block_size,
		block_size
	});
	menu_snake.push_front(SDL_Rect {
		500-50,
		500,
		block_size,
		block_size
	});

	int apples_collected = 0;
	bool in_menu = true;

	while (running) {

		while(SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}

			if (event.type == SDL_KEYDOWN) {
				if (last_dir == dir && !in_menu) {
					if (event.key.keysym.sym == SDLK_s) {
						// std::cout << "Key down pressed" << std::endl;
						if (dir != 1) {
							dir = 3;
						}
					} else if (event.key.keysym.sym == SDLK_w) {
						if (dir != 3) {
							dir = 1;
						}
					} else if (event.key.keysym.sym == SDLK_d) {
						if (dir != 2) {
							dir = 0;
						}
					} else if (event.key.keysym.sym == SDLK_a) {
						if (dir != 0) {
							dir = 2;
						}
					}
				}

				if (event.key.keysym.sym == SDLK_ESCAPE) {
					if (in_menu) {
						running = false;
					} else {
						snake.clear();
						snake.push_back(SDL_Rect {
							500,
							500,
							block_size,
							block_size
						});
						head = snake[0];
						snake.push_front(SDL_Rect {
							500-25,
							500,
							block_size,
							block_size
						});
						snake.push_front(SDL_Rect {
							500-50,
							500,
							block_size,
							block_size
						});

						menu_snake.clear();
						menu_snake.push_back(SDL_Rect {
							500,
							500,
							block_size,
							block_size
						});
						menu_head = menu_snake[0];
						menu_snake.push_front(SDL_Rect {
							500-25,
							500,
							block_size,
							block_size
						});
						menu_snake.push_front(SDL_Rect {
							500-50,
							500,
							block_size,
							block_size
						});

						menu_dir = 0;

						last_dir = 0;
						dir = 0;
						last_update = time_ms();
						apples_collected = 0;

						in_menu = true;
					}
				}
				if (event.key.keysym.sym == SDLK_SPACE) {
					in_menu = false;
				}
				// std::cout << dir << std::endl;
			}
		}

		// if (!Mix_PlayingMusic()) {

		// 	// Mix_FreeMusic(music);

		// 	// music = Mix_LoadMUS("assets\\music.mp3");

		// 	Mix_PlayMusic(music, 1);
		// }

		bool is_in = false;
		if ((head.x <= -25 || head.x >= width) || (head.y <= -25 || head.y >= height)) {
			std::cout << "Wall collision detected" << std::endl;
			SDL_Delay(1000);
			snake.clear();
			snake.push_back(SDL_Rect {
				500,
				500,
				block_size,
				block_size
			});
			head = snake[0];
			snake.push_front(SDL_Rect {
				500-25,
				500,
				block_size,
				block_size
			});
			snake.push_front(SDL_Rect {
				500-50,
				500,
				block_size,
				block_size
			});

			menu_snake.clear();
			menu_snake.push_back(SDL_Rect {
				500,
				500,
				block_size,
				block_size
			});
			menu_head = menu_snake[0];
			menu_snake.push_front(SDL_Rect {
				500-25,
				500,
				block_size,
				block_size
			});
			menu_snake.push_front(SDL_Rect {
				500-50,
				500,
				block_size,
				block_size
			});

			menu_dir = 0;

			last_dir = 0;
			dir = 0;
			last_update = time_ms();
			is_in = true;
			apples_collected = 0;

			in_menu = true;
		}

		for (int i = 0; i < snake.size()-1; i++) {
			// std::cout << snake[i].x << ", ";
			if (snake[i].x == snake.back().x && snake[i].y == snake.back().y) {
				std::cout << "Snake collision detected" << std::endl;
				SDL_Delay(1000);
				snake.clear();
				snake.push_back(SDL_Rect {
					500,
					500,
					block_size,
					block_size
				});
				head = snake[0];
				snake.push_front(SDL_Rect {
					500-25,
					500,
					block_size,
					block_size
				});
				snake.push_front(SDL_Rect {
					500-50,
					500,
					block_size,
					block_size
				});

				menu_snake.clear();
				menu_snake.push_back(SDL_Rect {
					500,
					500,
					block_size,
					block_size
				});
				menu_head = menu_snake[0];
				menu_snake.push_front(SDL_Rect {
					500-25,
					500,
					block_size,
					block_size
				});
				menu_snake.push_front(SDL_Rect {
					500-50,
					500,
					block_size,
					block_size
				});

				menu_dir = 0;

				last_dir = 0;
				dir = 0;
				last_update = time_ms();
				apples_collected = 0;
				is_in = true;

				in_menu = true;
				break;

			}
		}
		// std::cout << std::endl;
		if (is_in) {
			continue;
		}


		for (int i = 0; i < apples.size(); i++) {
			if (snake.back().x == apples[i].x && snake.back().y == apples[i].y) {
				std::cout << "Apple collision detected: " << apples_collected+1 << std::endl;
				apples.erase(apples.begin() + i);
				snake.push_front(SDL_Rect {
					(snake[0].x - snake[1].x) + snake[0].x,
					(snake[0].y - snake[1].y) + snake[0].y,
					block_size,
					block_size
				});
				int x;
				int y;
				while (true) {
					x = rand()%40*25;
					y = rand()%40*25;
					bool is_in = false;
					for (int x = 0; i < snake.size(); i++) {
						if (snake[i].x == x && snake[i].y == y) {
							is_in = true;
							break;
						}
					}
					for (int x = 0; i < apples.size(); i++) {
						if (apples[i].x == x && apples[i].y == y) {
							is_in = true;
							break;
						}
					}
					if (!is_in) {
						break;
					}

				}
				apples.push_back(SDL_Rect {
					x,
					y,
					block_size,
					block_size
				});
				apples_collected++;
				break;
			}
		}


		if (time_ms() - last_update > (200 - (apples_collected*3)) && !in_menu) {

			switch(dir) {

				case 0:
					head.x += 25;
					break;
				case 1:
					head.y -= 25;
					break;
				case 2:
					head.x -= 25;
					break;
				case 3:
					head.y += 25;
					break;

			}

			snake.push_back(head);
			snake.pop_front();

			last_dir = dir;

			last_update = GetTickCount();
		}

		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		if (!in_menu) {

			std::string points = std::to_string(apples_collected);

			SDL_Surface* surf = TTF_RenderText_Solid(font, points.c_str(), {255,255,255});
			SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surf);
			SDL_Rect rect {
				(int)(width/2-points.length()*50/2),
				0,
				(int)(points.length()*50),
				100
			};

			SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);

			// std::cout << snake.size() << std::endl;
			for (int i = 0; i < snake.size()-3; i++) {
				SDL_RenderFillRect(renderer, &snake[i]);
			}

			SDL_SetRenderDrawColor(renderer, 0, 125, 0, 255);
			SDL_RenderFillRect(renderer, &snake.back());
			SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
			SDL_RenderFillRect(renderer, &snake[snake.size()-2]);
			SDL_SetRenderDrawColor(renderer, 0, 175, 0, 255);
			SDL_RenderFillRect(renderer, &snake[snake.size()-3]);
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

			for (int i = 0; i < apples.size(); i++) {
				SDL_RenderFillRect(renderer, &apples[i]);
			}
			// SDL_RenderFillRect(renderer, &head);
			SDL_RenderCopy(renderer, message, NULL, &rect);
			SDL_FreeSurface(surf);
			SDL_DestroyTexture(message);
		} else {

			for (int i = 0; i < 2; i++) {
				if (menu_head.x == menu_apples[i].x && menu_head.y == menu_apples[i].y) {
					
					menu_apples.erase(menu_apples.begin() + i);
					menu_apples.push_back(SDL_Rect {
						rand()%40*25,
						rand()%40*25,
						block_size,
						block_size
					});
					// std::cout << menu_head.x << ", " << menu_apples.back().x << std::endl;
					break;
				}
			}

			if (time_ms() - last_update > 100) {

				switch(menu_dir) {

					case 0:

						menu_head.x += 25;

						
						break;
					case 1:
						menu_head.y -= 25;
						break;
					case 2:
						menu_head.x -= 25;
						break;
					case 3:
						menu_head.y += 25;
						break;

				}

				menu_snake.push_back(menu_head);
				menu_snake.pop_front();

				last_update = GetTickCount();
			}

			if ((pow(menu_head.x - menu_apples[0].x, 2) + pow(menu_head.y - menu_apples[0].y, 2)) > (pow(menu_head.x - menu_apples[1].x, 2) + pow(menu_head.y - menu_apples[1].y, 2))) {
				if (menu_head.x != menu_apples[1].x) {
					if (menu_head.x < menu_apples[1].x) {
						menu_dir = 0;
					}
					if (menu_head.x > menu_apples[1].x) {
						menu_dir = 2;
					}
				} else {
					if (menu_head.y < menu_apples[1].y) {
						menu_dir = 3;
					}
					if (menu_head.y > menu_apples[1].y) {
						menu_dir = 1;
					}
				}
			} else {
				if (menu_head.x != menu_apples[0].x) {
					if (menu_head.x < menu_apples[0].x) {
						menu_dir = 0;
					}
					if (menu_head.x > menu_apples[0].x) {
						menu_dir = 2;
					}
				} else {
					if (menu_head.y < menu_apples[0].y) {
						menu_dir = 3;
					}
					if (menu_head.y > menu_apples[0].y) {
						menu_dir = 1;
					}
				}
			}


			// if (menu_head.x != menu_apples[1].x) {
			// 	if (menu_head.x < menu_apples[1].x) {
			// 		menu_dir = 0;
			// 	}
			// 	if (menu_head.x > menu_apples[1].x) {
			// 		menu_dir = 2;
			// 	}
			// } else {
			// 	if (menu_head.y < menu_apples[1].y) {
			// 		menu_dir = 3;
			// 	}
			// 	if (menu_head.y > menu_apples[1].y) {
			// 		menu_dir = 1;
			// 	}
			// }

			SDL_SetRenderDrawColor(renderer, 0, 125, 0, 255);
			SDL_RenderFillRect(renderer, &menu_snake[2]);
			SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
			SDL_RenderFillRect(renderer, &menu_snake[1]);
			SDL_SetRenderDrawColor(renderer, 0, 175, 0, 255);
			SDL_RenderFillRect(renderer, &menu_snake[0]);

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			for (int i = 0; i < menu_apples.size(); i++) {
				SDL_RenderFillRect(renderer, &menu_apples[i]);
			}

			SDL_Surface* title_surf = TTF_RenderText_Solid(font, "SNAKE", {255,255,255});
			SDL_Texture* title_message = SDL_CreateTextureFromSurface(renderer, title_surf);
			SDL_Rect title_rect {
				(int)(width/2-225),
				(int)(height/5),
				450,
				180
			};
			
			SDL_RenderCopy(renderer, title_message, NULL, &title_rect);

			SDL_Surface* start_surf = TTF_RenderText_Solid(font, "Press space to start", {255,255,255});
			SDL_Texture* start_message = SDL_CreateTextureFromSurface(renderer, start_surf);
			SDL_Rect start_rect {
				(int)(width/2-175),
				(int)(height/5*3),
				350,
				75
			};
			
			SDL_RenderCopy(renderer, start_message, NULL, &start_rect);


			SDL_Surface* exit_surf = TTF_RenderText_Solid(font, "Press escape to exit", {255,255,255});
			SDL_Texture* exit_message = SDL_CreateTextureFromSurface(renderer, exit_surf);
			SDL_Rect exit_rect {
				(int)(width/2-175),
				(int)(height/5*3) + 75,
				350,
				75
			};
			
			SDL_RenderCopy(renderer, exit_message, NULL, &exit_rect);

			SDL_FreeSurface(title_surf);
			SDL_DestroyTexture(title_message);

			SDL_FreeSurface(start_surf);
			SDL_DestroyTexture(start_message);

			SDL_FreeSurface(exit_surf);
			SDL_DestroyTexture(exit_message);

		}


		SDL_RenderPresent(renderer);
		
		// TTF_CloseFont(font);
		// std::cout << "\r" << head.x;


		// SDL_Delay(25);
	}

	std::cout << "Freeing audio" << std::endl;
	Mix_FreeChunk(music);

	music = nullptr;

	std::cout << "Closing font" << std::endl;
	TTF_CloseFont(font);
	
	std::cout << "Destroying window" << std::endl;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();

	return 0;


}
