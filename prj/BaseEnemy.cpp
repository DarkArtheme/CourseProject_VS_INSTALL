#include "BaseEnemy.h"

std::vector<sf::Texture> EnemyTextureLoader::textures_common_enemy(4);
std::vector<sf::Texture> EnemyTextureLoader::textures_heavy_enemy(4);

void EnemyTextureLoader::LoadTextures(const std::string& path) {
    textures_common_enemy[Direction::DOWN].loadFromFile(path, sf::IntRect(64, 0, 64, 64));
    textures_common_enemy[Direction::UP].loadFromFile(path, sf::IntRect(192, 0, 64, 64));
    textures_common_enemy[Direction::RIGHT].loadFromFile(path, sf::IntRect(0, 0, 64, 64));
    textures_common_enemy[Direction::LEFT].loadFromFile(path, sf::IntRect(128, 0, 64, 64));
    textures_heavy_enemy[Direction::DOWN].loadFromFile(path, sf::IntRect(64, 64, 64, 64));
    textures_heavy_enemy[Direction::UP].loadFromFile(path, sf::IntRect(192, 64, 64, 64));
    textures_heavy_enemy[Direction::RIGHT].loadFromFile(path, sf::IntRect(0, 64, 64, 64));
    textures_heavy_enemy[Direction::LEFT].loadFromFile(path, sf::IntRect(128, 64, 64, 64));
}


void BaseEnemy::move(sf::Clock& clock) {
    float time = clock.getElapsedTime().asMilliseconds() / 6.0f;
    sf::FloatRect cur_rec = sprite.getGlobalBounds();
    sf::Vector2f cntr(cur_rec.left + sprite_side / 2.0, cur_rec.top + sprite_side / 2.0);

    switch(dir){
        case Direction::LEFT:
            sprite.move(-speed*time, 0);
            if(turn_num < path.size() && cntr.x < getCenter(path[turn_num].rect).x){
                sprite.setPosition(getCenter(path[turn_num].rect).x, getCenter(path[turn_num].rect).y);
                dir = setupDir(path[turn_num].type);
                turn_num++;
                this->changeTexture();
            }
            if(turn_num < path.size() && checkEqual(cntr, getCenter(path[turn_num].rect))){
                sprite.setPosition(getCenter(path[turn_num].rect).x, getCenter(path[turn_num].rect).y);
                dir = setupDir(path[turn_num].type);
                turn_num++;
                this->changeTexture();
            }
            break;
        case Direction::RIGHT:
            sprite.move(speed*time, 0);
            if(turn_num < path.size() && cntr.x > getCenter(path[turn_num].rect).x){
                sprite.setPosition(getCenter(path[turn_num].rect).x, getCenter(path[turn_num].rect).y);
                dir = setupDir(path[turn_num].type);
                turn_num++;
                this->changeTexture();
            }
            if(turn_num < path.size() && checkEqual(cntr, getCenter(path[turn_num].rect))){
                sprite.setPosition(getCenter(path[turn_num].rect).x, getCenter(path[turn_num].rect).y);
                dir = setupDir(path[turn_num].type);
                turn_num++;
                this->changeTexture();
            }
            break;
        case Direction::DOWN:
            //std::cout << getCenter(path[turn_num].rect).x << " " << getCenter(path[turn_num].rect).y << " | " << cntr.x <<" " <<cntr.y <<  std::endl;
            sprite.move(0, speed*time);
            if(turn_num < path.size() && cntr.y > getCenter(path[turn_num].rect).y){
                sprite.setPosition(getCenter(path[turn_num].rect).x, getCenter(path[turn_num].rect).y);
                dir = setupDir(path[turn_num].type);
                turn_num++;
                this->changeTexture();
            }
            if(turn_num < path.size() && checkEqual(cntr, getCenter(path[turn_num].rect))){
                sprite.setPosition(getCenter(path[turn_num].rect).x, getCenter(path[turn_num].rect).y);
                dir = setupDir(path[turn_num].type);
                turn_num++;
                this->changeTexture();
            }
            break;
        case Direction::UP:
            sprite.move(0, -speed*time);
            if(turn_num < path.size() && cntr.y < getCenter(path[turn_num].rect).y){
                sprite.setPosition(getCenter(path[turn_num].rect).x, getCenter(path[turn_num].rect).y);
                dir = setupDir(path[turn_num].type);
                turn_num++;
                this->changeTexture();
            }
            if(turn_num < path.size() && checkEqual(cntr, getCenter(path[turn_num].rect))){
                sprite.setPosition(getCenter(path[turn_num].rect).x, getCenter(path[turn_num].rect).y);
                dir = setupDir(path[turn_num].type);
                turn_num++;
                this->changeTexture();
            }
            break;
    }

}

void BaseEnemy::draw(sf::RenderTarget& window){
    float hp_left_coef = static_cast<float>(health) * 1.0f / (static_cast<float>(max_health) * 1.0f);
    healthbar_green.setSize({(healthbar_border.getGlobalBounds().width - static_cast<float>(outline_thickness)) * hp_left_coef,
                             healthbar_border.getGlobalBounds().height - static_cast<float>(outline_thickness)});
    healthbar_red.setSize({(healthbar_border.getGlobalBounds().width - static_cast<float>(outline_thickness)) * (1.0f - hp_left_coef),
                            healthbar_border.getGlobalBounds().height - static_cast<float>(outline_thickness)});
    healthbar_green.setPosition(this->getCenterPosition().x - static_cast<float>(sprite_side) / 2.0f, this->getCenterPosition().y - static_cast<float>(sprite_side) / 2.0f - static_cast<float>(healthbar_height));
    healthbar_border.setPosition(this->getCenterPosition().x - static_cast<float>(sprite_side) / 2.0f, this->getCenterPosition().y - static_cast<float>(sprite_side) / 2.0f - static_cast<float>(healthbar_height));
    healthbar_red.setPosition(healthbar_green.getGlobalBounds().left + healthbar_green.getGlobalBounds().width
            , healthbar_green.getGlobalBounds().top);
    window.draw(sprite);
    window.draw(healthbar_green);
    window.draw(healthbar_red);
    window.draw(healthbar_border);
    if(sprite.getPosition().x > window.getSize().x || sprite.getPosition().y > window.getSize().y
        || sprite.getPosition().x < 0.0f || sprite.getPosition().y < 0.0f) {
        shoud_be_deleted = true;
    }
}

template<class T>
sf::Vector2f BaseEnemy::getCenter(const sf::Rect<T>& r) {
    return sf::Vector2f (r.left + r.width / 2.0, r.top + r.height / 2.0);
}

Direction BaseEnemy::setupDir(const std::string &str) {
    if(str == "left") return LEFT;
    if(str == "down") return DOWN;
    if(str == "up") return UP;
    if(str == "right") return RIGHT;
    return DOWN;
}

void BaseEnemy::changeTexture() {
    sprite.setTexture((*textures)[dir]);
}

template<class Vector>
bool BaseEnemy::checkEqual(const Vector &lhs, const Vector &rhs) {
    return (std::abs(lhs.x - rhs.x) < 1e-2 && std::abs(lhs.y - rhs.y) < 1e-2);
}


LightEnemy::LightEnemy(const LevelMaker &lvl, sf::Clock &clk, const GridMaker &grd, const int side, const double scale_x_, const double scale_y_) {
    scale_x = scale_x_;
    scale_y = scale_y_;
    path = lvl.returnPath();
    sprite_side = side;
    textures = &EnemyTextureLoader::textures_common_enemy;
    dir = setupDir(lvl.getStart().name);
    auto start_rect = lvl.getStart().rect;
    sprite.setOrigin(sf::Vector2f(side / 2.0, side / 2.0));
	sprite.setPosition(static_cast<float>(start_rect.left + grd.getScale() * 3.0 / 2.0), static_cast<float>(start_rect.top + grd.getScale() * 3.0 / 2.0));
	sprite.setTexture((*textures)[dir]);

	healthbar_width = sprite_side;
	healthbar_height = sprite_side / 32;
	outline_thickness = 2;
	max_health = 40;
	health = max_health;
	points_for_kill = 10;
	speed = 0.8f;
	turn_num = 0;
	shoud_be_deleted = false;
	healthbar_green.setSize(sf::Vector2f(healthbar_width, healthbar_height));
	healthbar_border.setSize(sf::Vector2f(healthbar_width + outline_thickness, healthbar_height + outline_thickness));
	healthbar_border.setOutlineThickness(static_cast<float>(outline_thickness));
	healthbar_border.setOutlineColor(sf::Color::Black);
	healthbar_border.setFillColor(sf::Color(0,0,0,0));
	healthbar_red.setFillColor(sf::Color::Red);
	healthbar_green.setFillColor(sf::Color::Green);
    healthbar_green.setPosition(this->getCenterPosition().x - static_cast<float>(side) / 2,
                                this->getCenterPosition().y - static_cast<float>(side) / 2.0f - static_cast<float>(healthbar_height));
    healthbar_border.setPosition(this->getCenterPosition().x - static_cast<float>(side) / 2,
                                 this->getCenterPosition().y - static_cast<float>(side) / 2.0f - static_cast<float>(healthbar_height));
    healthbar_red.setPosition(healthbar_green.getGlobalBounds().left + healthbar_green.getGlobalBounds().width
            , healthbar_green.getGlobalBounds().top);

}

HeavyEnemy::HeavyEnemy(const LevelMaker &lvl, sf::Clock &clk, const GridMaker &grd, const int side, const double scale_x_, const double scale_y_) {
    scale_x = scale_x_;
    scale_y = scale_y_;
    path = lvl.returnPath();
    sprite_side = side;
    textures = &EnemyTextureLoader::textures_heavy_enemy;
    dir = setupDir(lvl.getStart().name);
    auto start_rect = lvl.getStart().rect;



    sprite.setOrigin(sf::Vector2f(side / 2.0, side / 2.0));
    sprite.setPosition(static_cast<float>(start_rect.left + grd.getScale() * 3.0 / 2.0), static_cast<float>(start_rect.top + grd.getScale() * 3.0 / 2.0));
    sprite.setTexture((*textures)[dir]);

    healthbar_width = sprite_side;
    healthbar_height = sprite_side / 32;
    outline_thickness = 2;
    max_health = 80;
    health = max_health;
    points_for_kill = 50;
    speed = 0.4f;
    turn_num = 0;
    shoud_be_deleted = false;

    healthbar_green.setSize(sf::Vector2f(healthbar_width, healthbar_height));
    healthbar_border.setSize(sf::Vector2f(healthbar_width + outline_thickness, healthbar_height + outline_thickness));
    healthbar_border.setOutlineThickness(static_cast<float>(outline_thickness));
    healthbar_border.setOutlineColor(sf::Color::Black);
    healthbar_border.setFillColor(sf::Color(0,0,0,0));
    healthbar_red.setFillColor(sf::Color::Red);
    healthbar_green.setFillColor(sf::Color::Green);
    healthbar_green.setPosition(this->getCenterPosition().x - static_cast<float>(side) / 2,
                                this->getCenterPosition().y - static_cast<float>(side) / 2.0f - static_cast<float>(healthbar_height));
    healthbar_border.setPosition(this->getCenterPosition().x - static_cast<float>(side) / 2,
                                 this->getCenterPosition().y - static_cast<float>(side) / 2.0f - static_cast<float>(healthbar_height));
    healthbar_red.setPosition(healthbar_green.getGlobalBounds().left + healthbar_green.getGlobalBounds().width
        , healthbar_green.getGlobalBounds().top);
}
