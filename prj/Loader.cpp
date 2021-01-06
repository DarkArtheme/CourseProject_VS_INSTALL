#include "Loader.h"

void Loader::loadAll() {
    main_menu_font.loadFromFile("src/fonts/font_menu.ttf");
    main_theme.openFromFile("src/sounds/main_theme.ogg");
    main_theme.setVolume(4);
    main_theme.setLoop(true);
    game_theme.openFromFile("src/sounds/game_theme.ogg");
    game_theme.setVolume(8);
    game_theme.setLoop(true);
    main_menu_image.loadFromFile("src/textures/main_menu_image.jpg");
}
