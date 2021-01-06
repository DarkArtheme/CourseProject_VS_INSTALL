#include "ContextMenu.h"



void ContextMenu::draw(sf::RenderTarget &window) const {
    if(is_visible) {
        window.draw(menu);
        window.draw(name);
        for (const auto &button : buttons) {
            button->draw(window);
        }
    }
}

ContextMenu::ContextMenu(const sf::FloatRect &parameters, const sf::Font &font
    , const unsigned int size, const std::string &header
    , const std::vector<std::shared_ptr<Button>> &buttons_
    , sf::Color menu_clr = sf::Color(255, 250, 165, 255)
    , sf::Color border_clr = sf::Color(230, 165, 50, 255)
    , sf::Color text_clr = sf::Color::Red)
    : buttons(buttons_), pos({parameters.left, parameters.top})
    , width_(parameters.width), height_(parameters.height)
    , menu_color(menu_clr), border_color(border_clr) {
    is_visible = false;
    menu.setPosition(parameters.left, parameters.top);
    menu.setSize({parameters.width, parameters.height});
    menu.setFillColor(ContextMenu::menu_color);
    menu.setOutlineColor(ContextMenu::border_color);
    menu.setOutlineThickness(ContextMenu::thickness);
    name.setFont(font);
    name.setCharacterSize(size);
    name.setFillColor(text_clr);
    name.setString(header);
    auto name_pos = name.getGlobalBounds();
    name.setOrigin(name_pos.left + name_pos.width / 2.0, name_pos.top + name_pos.height / 2.0);
    name.setPosition(pos.x + width_ / 2, pos.y + name_pos.height);
}
