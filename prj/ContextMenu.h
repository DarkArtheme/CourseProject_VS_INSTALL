#pragma once
#ifndef SFML_SANDBOX_CONTEXTMENU_H
#define SFML_SANDBOX_CONTEXTMENU_H
#include "Button.h"
#include <memory>

///@brief Класс, реализующий все меню в игре.
class ContextMenu {
public:
    ContextMenu() = delete;
    ContextMenu(const ContextMenu&) = default;
    ContextMenu& operator=(const ContextMenu&) = default;
    ~ContextMenu() = default;
    /**
     * @brief Конструктор.
     * @param parameters Скелет меню (Прямоугольник, размеры которого совпадают с размерами меню).
     * @param font Шрифт названия меню.
     * @param size Размер названия меню.
     * @param header Название меню.
     * @param buttons_ Кнопки меню.
     * @param menu_clr Цвет фона меню.
     * @param border_clr Цвет границ меню.
     * @param text_clr Цвет названия меню.
     */
    ContextMenu(const sf::FloatRect& parameters, const sf::Font& font, unsigned int size
    , const std::string& header, const std::vector<std::shared_ptr<Button>>& buttons_
    , sf::Color menu_clr, sf::Color border_clr, sf::Color text_clr);
    /**
     * @param value Значение, изменяющее видимость меню (true - видимо).
     */
    bool isVisible() const { return is_visible; }
    /**
     * @param value Значение, изменяющее видимость меню (true - видимо).
     */
    void setVisible(const bool value) { is_visible = value; }
    /**
     * @return Индекс выбранной игроком башни.
     */
    size_t getTowerIndex() const { return index_of_tower; }
    /**
     * @brief Устанавливает индекс выбранной игроком башни.
     * @param value Индекс выбранной игроком башни.
     */
    void setTowerIndex(const size_t value) { index_of_tower = value; }
    /**
     * @brief Отрисовывает меню.
     * @param window Окно игры.
     */
    void draw(sf::RenderTarget& window) const;
private:
    sf::Color menu_color;
    sf::Color border_color;
    static const unsigned int thickness = 10;
    bool is_visible;
    size_t index_of_tower{0};
    sf::Vector2f pos;
    float width_, height_;
    sf::RectangleShape menu;
    sf::Text name;
    std::vector<std::shared_ptr<Button>> buttons;
};



#endif //SFML_SANDBOX_CONTEXTMENU_H
