#include "BaseTower.h"

void TowerTextureLoader::LoadTextures(const std::string &path1, const std::string &path2) {
    sf::Image im1;
    im1.loadFromFile(path1);
    place_for_tower.loadFromImage(im1, sf::IntRect(0, 0, 64, 64));
    targeted_place_for_tower.loadFromImage(im1, sf::IntRect(0, 256, 64, 64));
    sf::Image im2;
    im2.loadFromFile(path2);
    base_texture.loadFromImage(im2, sf::IntRect(64, 192, 64, 64));
    tower_texture_1.loadFromImage(im2, sf::IntRect(192, 256, 64, 64));
    empty_tower_texture_1.loadFromImage(im2, sf::IntRect(192, 320, 64, 64));
    tower_texture_2.loadFromImage(im2, sf::IntRect(256, 0, 64, 64));
    rocket.loadFromImage(im2, sf::IntRect(448, 0, 64, 64));
    smoke.loadFromImage(im2, sf::IntRect(256, 192, 64, 64));
    explosion.loadFromImage(im2, sf::IntRect(256, 256, 64, 64));
}



void BaseTower::draw(sf::RenderTarget &window) {
    if(explosion_is_visible){
        if(cur_frame > frames_for_explosion) {
            explosion_is_visible = false;
            cur_frame = 0;
        } else {
            ++cur_frame;
            window.draw(sprite_explosion);
        }
    }
    switch(state) {
        case NOT_BUILT:
        case TARGETED_NOT_BUILD:
            break;
        case READY:
        case RELOADING:
            //window.draw(circle);
            window.draw(sprite_base);
            window.draw(sprite_rocket);
            //sprite_tower.setTexture(TowerTextureLoader::tower_texture_1);
            window.draw(sprite_tower);
            break;
    }
}

void BaseTower::summon() {
    state = READY;
}

void BaseTower::rotate() {
    float time = clock_ptr->getElapsedTime().asMilliseconds() / 10.0f;
    sprite_tower.rotate(rotation_speed * time);
    sprite_rocket.rotate(rotation_speed * time);
}

void BaseTower::flyRocket() {
    if(!isInRange(sprite_rocket.getPosition())){
        sound_explosion.play();
        sprite_explosion.setPosition(sprite_rocket.getPosition());
        sprite_rocket.setPosition(static_cast<float>(pos.x + side / 2.0), static_cast<float>(pos.y + side / 2.0));
        explosion_is_visible = true;
        state = READY;
        cooldown_clock.restart();
        return;
    }
    float time = clock_ptr->getElapsedTime().asMilliseconds() / 1.0f;
    sprite_rocket.move(sf::Vector2f(std::cos(dtor(angle)) * rocket_speed * time * 1.0, std::sin(dtor(angle)) * rocket_speed * time * 1.0));
}

void BaseTower::shoot() {
    float time = cooldown_clock.getElapsedTime().asMilliseconds();
    if(time > cooldown) {
        sound_shot.play();
        state = RELOADING;
        angle = sprite_tower.getRotation();
        angle -= 90.0;
    }
}

bool BaseTower::isInRange(const sf::Vector2f& pt) const{
    sf::Vector2f center(pos.x * 1.0 + side / 2.0, pos.y * 1.0 + side / 2.0);
    return ((pt.x - center.x)*(pt.x - center.x) + (pt.y - center.y)*(pt.y - center.y) <= fire_range*fire_range);
}

void BaseTower::setQuery(const std::vector<std::vector<std::shared_ptr<BaseEnemy>>::iterator>& query_) {
    query.clear();
    for(auto iter : query_){
        if(isInRange((*iter)->getCenterPosition())){
            query.push_back(iter);
        }
    }
}

bool BaseTower::checkHit(std::vector<std::shared_ptr<BaseEnemy>>::iterator& it) {
    if(!query.empty()) {
        //if (sprite_rocket.getGlobalBounds().intersects(query[0]->getGlobalBounds())){
        for(size_t i = 0; i < query.size(); ++i) {
            if (sprite_rocket.getGlobalBounds().contains((*query[i])->getCenterPosition())) {
                int left_health = (*query[i])->getHealth() - damage;
                (*query[i])->setHealth(left_health);
                sound_explosion.play();
                sprite_rocket.setPosition(static_cast<float>(pos.x + side / 2.0), static_cast<float>(pos.y + side / 2.0));
                explosion_is_visible = true;
                sprite_explosion.setPosition((*query[i])->getCenterPosition());
                state = READY;
                cooldown_clock.restart();
                it = query[i];
                return true;
            }
        }
    }
    return false;
}

void BaseTower::isTimeToShoot() {
    if(query.empty()) return;
    for(size_t i = 0; i < query.size(); ++i) {
        std::shared_ptr<BaseEnemy>& enemy = *query[i];
        double temp_angle = sprite_rocket.getRotation();
        //while(angle >= 90) { angle -= 90; }
        double dx = (enemy->getCenterPosition().x - (pos.x + side / 2.0) * 1.0);
        double dy = (enemy->getCenterPosition().y - (pos.y + side / 2.0) * 1.0);
        double dangle{0.0};
        if (enemy->getDirection() == LEFT || enemy->getDirection() == RIGHT) {
            dangle = isEqualExact(dy, 0.0) ? 90.0 : rtod(std::atan(std::abs(dx * 1.0 / dy)));
            if (dx <= 0.0 && dy <= 0.0) {
                dangle = 360.0 - dangle;
            } else if (dx <= 0.0 && dy > 0.0) {
                dangle = 180.0 + dangle;
            } else if (dx > 0.0 && dy <= 0.0) {
                dangle = 0.0 + dangle;
            } else {
                dangle = 180.0 - dangle;
            }
        } else {
            dangle = isEqualExact(dx, 0.0) ? 90.0 : rtod(std::atan(std::abs(dy * 1.0 / dx)));
            if (dx <= 0.0 && dy <= 0.0) {
                dangle = 270.0 + dangle;
            } else if (dx <= 0.0 && dy > 0.0) {
                dangle = 270.0 - dangle;
            } else if (dx > 0.0 && dy <= 0.0) {
                dangle = 90.0 - dangle;
            } else {
                dangle = 90.0 + dangle;
            }

        }
        if (isEqualApprox(temp_angle, dangle)) {
            //std::cout << '{' << angle <<", " << dangle << "}\n";
            this->shoot();
        }
    }
}

void BaseTower::reload() {
   float time = cooldown_clock.getElapsedTime().asMilliseconds();
   if(time > cooldown) {
       state = READY;
   }
}


LightTower::LightTower(const sf::Vector2i &position, const int side_, sf::Clock *clock_, const double scale_x_, const double scale_y_){
    scale_x = scale_x_;
    scale_y = scale_y_;
    pos = position;
    clock_ptr = clock_;
    state = State::NOT_BUILT;
    side = side_;

    sprite_base.setTexture(TowerTextureLoader::base_texture);
    sprite_rocket.setTexture(TowerTextureLoader::rocket);
    sprite_tower.setTexture(TowerTextureLoader::tower_texture_1);
    sprite_base.setOrigin(static_cast<float>(side_ / 2.0), static_cast<float>(side_ / 2.0));
    sprite_tower.setOrigin(static_cast<float>(side_ / 2.0), static_cast<float>(side_ / 2.0));
    sprite_rocket.setOrigin(static_cast<float>(side_ / 2.0), static_cast<float>(side_ / 2.0));
    sprite_base.setPosition(sf::Vector2f(pos.x + side_ / 2.0, pos.y + side_ / 2.0));
    sprite_tower.setPosition(sf::Vector2f(pos.x + side_ / 2.0, pos.y + side_ / 2.0));
    sprite_rocket.setPosition(sf::Vector2f(pos.x + side_ / 2.0, pos.y + side_ / 2.0));
    sprite_explosion.setTexture(TowerTextureLoader::explosion);
    sprite_explosion.setOrigin(static_cast<float>(side) / 2.0f, static_cast<float>(side) / 2.0f);
    sound_explosion.setBuffer(TowerSoundLoader::explosion);
    sound_shot.setBuffer(TowerSoundLoader::shot1);
    sound_explosion.setVolume(10);
    sound_shot.setVolume(10);

    damage = 40;
    fire_range = static_cast<float>(300 * std::min(scale_x, scale_y));
    explosion_is_visible = false;
    cost = 200;
    rotation_speed = 2.0f;
    rocket_speed = 2.2f;
    cooldown = 100.0f;

    circle.setRadius(fire_range);
    circle.setOutlineThickness(2);
    circle.setOutlineColor(sf::Color::Cyan);
    circle.setOrigin(sf::Vector2f(fire_range, fire_range));
    circle.setFillColor(sf::Color(0, 0, 0, 0));
    circle.setPosition(sf::Vector2f(pos.x + side / 2.0, pos.y + side / 2.0));
}

HeavyTower::HeavyTower(const sf::Vector2i &position, const int side_, sf::Clock *clock_, const double scale_x_, const double scale_y_) {
    scale_x = scale_x_;
    scale_y = scale_y_;
    pos = position;
    clock_ptr = clock_;
    state = State::NOT_BUILT;
    side = side_;

    sprite_base.setTexture(TowerTextureLoader::base_texture);
    sprite_rocket.setTexture(TowerTextureLoader::rocket);
    sprite_tower.setTexture(TowerTextureLoader::tower_texture_2);
    sprite_base.setOrigin(static_cast<float>(side_ / 2.0), static_cast<float>(side_ / 2.0));
    sprite_tower.setOrigin(static_cast<float>(side_ / 2.0), static_cast<float>(side_ / 2.0));
    sprite_rocket.setOrigin(static_cast<float>(side_ / 2.0), static_cast<float>(side_ / 2.0));
    sprite_base.setPosition(sf::Vector2f(pos.x + side_ / 2.0, pos.y + side_ / 2.0));
    sprite_tower.setPosition(sf::Vector2f(pos.x + side_ / 2.0, pos.y + side_ / 2.0));
    sprite_rocket.setPosition(sf::Vector2f(pos.x + side_ / 2.0, pos.y + side_ / 2.0));
    sprite_explosion.setTexture(TowerTextureLoader::explosion);
    sprite_explosion.setOrigin(static_cast<float>(side) / 2.0f, static_cast<float>(side) / 2.0f);
    sound_explosion.setBuffer(TowerSoundLoader::explosion);
    sound_shot.setBuffer(TowerSoundLoader::shot2);
    sound_explosion.setVolume(10);
    sound_shot.setVolume(10);

    damage = 80;
    fire_range = static_cast<float>(500 * std::min(scale_x, scale_y));
    explosion_is_visible = false;
    cost = 500;
    rotation_speed = 1.0f;
    rocket_speed = 1.4f;
    cooldown = 150.0f;

    circle.setRadius(fire_range);
    circle.setOutlineThickness(2);
    circle.setOutlineColor(sf::Color::Cyan);
    circle.setOrigin(sf::Vector2f(fire_range, fire_range));
    circle.setFillColor(sf::Color(0, 0, 0, 0));
    circle.setPosition(sf::Vector2f(pos.x + side / 2.0, pos.y + side / 2.0));
}

void TowerSoundLoader::LoadSounds(const std::string &folder_path) {
    shot1.loadFromFile(folder_path + "shot_sound.ogg");
    shot2.loadFromFile(folder_path + "shot_sound2.ogg");
    explosion.loadFromFile(folder_path + "explosion_sound.ogg");
}
