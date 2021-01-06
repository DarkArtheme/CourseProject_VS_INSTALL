#pragma once
#ifndef SFML_SANDBOX_LOADER_H
#define SFML_SANDBOX_LOADER_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

///@brief Класс подгрузки всех необходимых для игры файлов.
class Loader {
public:
    ///@brief Загружает все поля данного класса.
    static void loadAll();
    inline static sf::Font main_menu_font;
    inline static sf::Music main_theme;
    inline static sf::Music game_theme;
    inline static sf::Texture main_menu_image;
};


#endif //SFML_SANDBOX_LOADER_H
