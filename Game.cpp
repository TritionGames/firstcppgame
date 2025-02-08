#include "Game.h"
#include <iostream>
#include "util.h"
#include "Perlin.h"
#include "SFML/Graphics.hpp"

void Game::initVariables()
{
    this->window = nullptr;
    srand(1);
};

void Game::initWindow()
{
    this->videoMode.height = 600;
    this->videoMode.width = 800;
    this->window = new sf::RenderWindow(this->videoMode, "Game", sf::Style::Titlebar | sf::Style::Close);
    window->setFramerateLimit(60);

    font.loadFromFile("Minecraft.ttf");

    this->player.setRenderWindow(this->window);
    player.setFont(font);

    sf::Vector2f pos[10] = {
        sf::Vector2f(0, 400),
        sf::Vector2f(50, 400),
        sf::Vector2f(50, 400),
        sf::Vector2f(50, 350),
        sf::Vector2f(100, 350),
        sf::Vector2f(150, 350),
        sf::Vector2f(150, 400),
        sf::Vector2f(200, 400),
        sf::Vector2f(300, 400),
        sf::Vector2f(400, 400),
    };
    std::cout << tile_count;

    /*for (int i = 0; i < tile_count; i++) {
        this->walls[i].setRenderWindow(this->window);
        std::cout << i << std::endl;
        this->walls[i].setPosition(pos[i]);(
    }*/
  
    /*for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            std::cout << x*10 + y << std::endl;
            this->walls[x*10+y].setRenderWindow(this->window);
            this->walls[x*10+y].setPosition(sf::Vector2f(x*50, y*50 + 300));
            tile_count++;
        }
    }
    */


    wall.setRenderWindow(window);

    this->background_texture.loadFromFile("background.png");
    this->background_sprite.setTexture(this->background_texture);

    //this->wall.setRenderWindow(this->window);
};

//constructor / Destructor

void Game::generate_chunk(std::string position, sf::Vector2f chunk) {
    std::map<std::string, int> walls;

    std::cout << "building chunk" << std::endl;

    if (chunk.x < 0) {
        return;
    }

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            //walls;
            int type = 0;

            double offset = noise((x + chunk.x * 4)/2, 15) * 10;

            double cave = noise((chunk.x * 4 + x), (chunk.y * 4 + y));
            //double = noise((chunk.x * 4 + x), (chunk.y * 4 + y));

            int pos = chunk.y * 4 + y + offset;

            if (pos == 1) {
                type = 2;
            }

            else if (cave + (0.3 * (64-pos) / 64) < 0.001) {
                type = 0;
            }

            else if (pos > 10) {
                type = 3;
            }

            else if (pos > 0) {
                type = 1;
            }

            if (pos > 128) {
                type = 0;
            }

            walls.emplace(vec2_to_string(sf::Vector2f(x, y)), type);
        }
    };

    chunks.emplace(position, walls);
}

Game::Game() {
	this->initVariables();
    this->initWindow();
};

Game::~Game() {
	delete this->window;
};


void Game::pollEvents()
{
    while (this->window->pollEvent(this->ev))
    {
        switch (this->ev.type) {
        case sf::Event::Closed:
            this->window->close();
            break;

        case sf::Event::KeyPressed:
            switch (this->ev.key.code) {
            case sf::Keyboard::Escape:
                this->window->close();
            }

            break;
        };
    };
};



void Game::update() {
    pollEvents();

    //std::cout << chunk_coord.x << ';' << chunk_coord.y;

    player.update(chunks, dt, mouse, *window);

    chunk_coord = sf::Vector2f(player.getPosition());
    chunk_coord.x = int(chunk_coord.x / 200);
    chunk_coord.y = int(chunk_coord.y / 200);

    //std::cout << chunk_coord.x << ";" << chunk_coord.y << std::endl;
    //text.setPosition(player.getPosition() - sf::Vector2f(50, 50));

    float current_time = this->clock.restart().asSeconds();
    //if (current_time > 0) {
    float fps = 1.f / (current_time);

    dt = current_time;
    if (dt > 0.1) dt = 0.1;

    //std::cout << "FPS: " << fps << std::endl;
    //};
}

                                                                                                                       
void Game::render() {
    //window->clear(sf::Color::Black);

    window->draw(this->background_sprite);

    //window->draw(this->text);

    //render here
    camera_pos = lerp2f(player.getPosition() - sf::Vector2f(videoMode.width / 2, videoMode.height / 2), camera_pos, std::pow(0.02, dt));

    //text.move();

    /*for (int i = 0; i < tile_count; i++) {
        walls[i].render(camera_pos);
    }*/

    render_chunks.clear();

    for (int x = 0; x < 7; x++) {
        for (int y = 0; y < 7; y++) {

            if (chunks.count(vec2_to_string(sf::Vector2f(x - 3, y - 3) + chunk_coord)) == 0) {
                generate_chunk(vec2_to_string(sf::Vector2f(x - 3, y - 3) + chunk_coord), sf::Vector2f(x - 3, y - 3) + chunk_coord);
            }
            render_chunks.push_back(sf::Vector2f(x - 3, y - 3) + chunk_coord);
        }
    }

    for (sf::Vector2f render_chunk : render_chunks) {
        auto chunk = chunks[vec2_to_string(render_chunk)];

        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                if (!chunk[vec2_to_string(sf::Vector2f(x, y))]) {
                    continue;
                }
                auto chunk_coord = render_chunk;
                wall.set_block_type(chunk[vec2_to_string(sf::Vector2f(x, y))]);
                wall.setPosition(sf::Vector2f(x * 50 + chunk_coord.x * 50 * 4, y * 50 + chunk_coord.y * 50 * 4));
                wall.render(camera_pos);
            }
        }
    };

    player.render(camera_pos, wall.get_images());
    //};

    window->display();
};

//accessors
const bool Game::running() const
{
	return this->window->isOpen();
};

