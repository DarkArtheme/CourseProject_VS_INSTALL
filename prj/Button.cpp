#include "Button.h"

bool Button::isVisible() const { return is_visible; }

void Button::setVisible(const bool value) { is_visible = value; }

//bool Button::checkCursor(const sf::Vector2i &curs_pos) { return true; }
//bool Button::isActivated(const sf::Vector2i &curs_pos) { return true; }
//void Button::draw(sf::RenderTarget &window) const { ; }

ButtonSpr::ButtonSpr(const sf::Texture &deactivated_texture, const sf::Texture &targeted_texture
    , const sf::Texture& clicked_texture, sf::Vector2f position, const float width, const float height)
    : texture_deactivated(deactivated_texture), texture_targeted(targeted_texture), texture_clicked(clicked_texture){
    pos = position;
    sound_targeted.setBuffer(ButtonSoundLoader::targeted);
    sound_clicked.setBuffer(ButtonSoundLoader::clicked);
    sound_targeted.setVolume(20);
    sound_clicked.setVolume(20);
    width_ = width;
    height_ = height;
    is_visible = true;
    status = DEACTIVATED;
    sprite.setTexture(deactivated_texture);
    sprite.setPosition(pos);
}

bool ButtonSpr::checkCursor(const sf::Vector2i &curs_pos) {
    if(is_visible) {
        sf::FloatRect area(pos, {width_, height_});
        if (area.contains(sf::Vector2f(curs_pos))) {
            if(status != TARGETED)
                sound_targeted.play();
            status = TARGETED;
            return true;
        }
        status = DEACTIVATED;
        if (permanently_clicked) {
            status = CLICKED;
        }
    }
    return false;
}

bool ButtonSpr::isActivated(const sf::Vector2i &curs_pos) {
    sf::FloatRect area(pos, {width_, height_});
    if (area.contains(sf::Vector2f(curs_pos))) {
        sound_clicked.play();
        status = CLICKED;
        return true;
    }
    status = DEACTIVATED;
    if(permanently_clicked){
        status = CLICKED;
    }
    return false;
}

void ButtonSpr::draw(sf::RenderTarget &window) {
    if(is_visible){
        switch (status){
            case DEACTIVATED:
                sprite.setTexture(texture_deactivated);
                break;
            case CLICKED:
                sprite.setTexture(texture_clicked);
                break;
            case TARGETED:
                sprite.setTexture(texture_targeted);
                break;
        }
        window.draw(sprite);
    }
}


ButtonRec::ButtonRec(const sf::FloatRect &place, const sf::Color &color_deactivated
    , const sf::Color &color_targeted, const sf::Color &color_clicked
    , const sf::Color& color_text, const sf::Font &font, const unsigned int size
    , const std::string& str, const float thickness_shadow)
    : rect(place), deactivated_color(color_deactivated)
    , targeted_color(color_targeted), clicked_color(color_clicked)
    , text_color(color_text)
    , shadow_thickness(thickness_shadow) {
    text.setFont(font);
    width_ = rect.width;
    height_ = rect.height;
    pos = {rect.left, rect.top};
    text.setCharacterSize(size);
    text.setString(str);
    text.setFillColor(text_color);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(0);
    auto text_rect = text.getGlobalBounds();
    text.setOrigin(static_cast<float>(text_rect.left + text_rect.width / 2.0), static_cast<float>(text_rect.top + text_rect.height / 2.0));
    text.setPosition(static_cast<float>(pos.x + width_ / 2.0), static_cast<float>(pos.y + height_ / 2.0));
    rectshape.setSize({width_, height_});
    rectshape.setPosition(pos);
    rectshape.setFillColor(deactivated_color);
    rectshape.setOutlineColor(sf::Color::Black);
    rectshape.setOutlineThickness(0);
    sound_targeted.setBuffer(ButtonSoundLoader::targeted);
    sound_clicked.setBuffer(ButtonSoundLoader::clicked);
    sound_targeted.setVolume(20);
    sound_clicked.setVolume(20);
    status = DEACTIVATED;
    is_visible = true;
}

bool ButtonRec::checkCursor(const sf::Vector2i &curs_pos) {
    if(is_visible) {
        if (rect.contains(sf::Vector2f(curs_pos))) {
            if(status != TARGETED)
                sound_targeted.play();
            status = TARGETED;
            return true;
        }
        status = DEACTIVATED;
        if (permanently_clicked) {
            status = CLICKED;
        }
    }
    return false;
}

bool ButtonRec::isActivated(const sf::Vector2i &curs_pos) {
    if (rect.contains(sf::Vector2f(curs_pos))) {
        sound_clicked.play();
        status = CLICKED;
        return true;
    }
    status = DEACTIVATED;
    if(permanently_clicked){
        status = CLICKED;
    }
    return false;
}

void ButtonRec::draw(sf::RenderTarget &window) {
    if(is_visible){
        switch(status){
            case DEACTIVATED:
                rectshape.setFillColor(deactivated_color);
                rectshape.setOutlineThickness(0);
                text.setOutlineThickness(0);
                break;
            case TARGETED:
                rectshape.setFillColor(targeted_color);
                rectshape.setOutlineThickness(shadow_thickness);
                text.setOutlineThickness(shadow_thickness);
                break;
            case CLICKED:
                rectshape.setFillColor(clicked_color);
                rectshape.setOutlineThickness(shadow_thickness);
                text.setOutlineThickness(shadow_thickness);
                break;
        }
        window.draw(rectshape);
        window.draw(text);
    }
}

void ButtonSoundLoader::LoadSounds(const std::string &folder_path) {
    targeted.loadFromFile(folder_path + "click2.ogg");
    clicked.loadFromFile(folder_path + "switch2.ogg");
}
