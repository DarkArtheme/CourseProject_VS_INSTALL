#include "GridMaker.h"
#include "LevelMaker.h"
#include "BaseEnemy.h"
#include "BaseTower.h"
#include "ContextMenu.h"
#include "Loader.h"
#include <cmath>
#include <fstream>


template<class Vector>
bool checkEqual(const Vector &lhs, const Vector &rhs) {
    return (std::abs(lhs.x - rhs.x) < 1e-1 && std::abs(lhs.y - rhs.y) < 1e-1);
}

///@brief Загружает все ресурсы.
void loadSources(){
    Loader::loadAll();
    EnemyTextureLoader::LoadTextures("src/textures/EnemyUnits.png");
    TowerTextureLoader::LoadTextures("src/textures/Towers.png", "src/textures/ExtendedTowers.png");
    TowerSoundLoader::LoadSounds("src/sounds/");
    ButtonSoundLoader::LoadSounds("src/sounds/");
}

///@brief Производит обновление данных о врагах внутри всех башен.
void updateQuery(std::vector<std::shared_ptr<BaseTower>>& towers, std::vector<std::shared_ptr<BaseEnemy>>& enemies);
template<class Tower>
///@brief Строит башню.
void changeTower(std::vector<std::shared_ptr<BaseTower>>& towers, Tower new_tower, const size_t ind);

///@brief Обрабатывает главное меню.
bool manageMainMenu(sf::RenderWindow& window, sf::Texture& main_menu_image, ContextMenu& main_menu
    , std::vector<std::shared_ptr<Button>>& main_menu_buttons, ContextMenu& level_menu, const double scale_x, const double scale_y);

///@brief Обрабатывает меню выбора уровня.
bool manageLevelMenu(sf::RenderWindow& window, sf::Texture& main_menu_image, ContextMenu& level_menu
    , std::vector<std::shared_ptr<Button>>& level_menu_buttons, int& chosen_level, const int passed_levels, const double scale_x, const double scale_y);

///@brief Обрабатывает меню паузы.
bool managePauseMenu(sf::RenderWindow& window, ContextMenu& pause_menu
    , std::vector<std::shared_ptr<Button>>& pause_menu_buttons);

///@brief Обрабатывает меню победы.
bool manageVictoryMenu(sf::RenderWindow& window, ContextMenu& victory_menu, std::vector<std::shared_ptr<Button>>& victory_menu_buttons
    , int& chosen_level);

///@brief Обрабатывает меню поражения.
bool manageDefeatMenu(sf::RenderWindow& window, ContextMenu& victory_menu, std::vector<std::shared_ptr<Button>>& victory_menu_buttons, bool& try_again);

///@brief Порождает меню выбора башни.
std::pair<ContextMenu, std::vector<std::shared_ptr<Button>>> createTowerMenu(const sf::Font& font, const double scale_x, const double scale_y);

///@brief Порождает главное меню.
std::pair<ContextMenu, std::vector<std::shared_ptr<Button>>> createMainMenu(const sf::Font& font, const double scale_x, const double scale_y);

///@brief Порождает меню выбора уровня.
std::pair<ContextMenu, std::vector<std::shared_ptr<Button>>> createLevelMenu(const sf::Font& font, const int passed_levels, const double scale_x, const double scale_y);

///@brief Порождает меню паузы.
std::pair<ContextMenu, std::vector<std::shared_ptr<Button>>> createPauseMenu(const sf::Font& font, const double scale_x, const double scale_y);

///@brief Порождает меню победы.
std::pair<ContextMenu, std::vector<std::shared_ptr<Button>>> createVictoryMenu(const sf::Font& font, const double scale_x, const double scale_y);

///@brief Порождает меню поражения.
std::pair<ContextMenu, std::vector<std::shared_ptr<Button>>> createDefeatMenu(const sf::Font& font, const double scale_x, const double scale_y);

///@brief Обрабатывает основной игровой цикл
bool manageGameCycle(sf::RenderWindow& window, const double scale_x, const double scale_y, int& level_number, int& clicked_level, bool& should_show_main_menu);

int main(int argc, char* argv[]) {
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Prj-Towers", sf::Style::Fullscreen);
	window.setFramerateLimit(120);
	double scale_x = window.getSize().x / 1920.0;
	double scale_y = window.getSize().y / 1080.0;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;
    sf::View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    window.setView(view);
    float viewspeed = 2.3;
    loadSources();
    int lvl_num = -1;                           // выбранный уровень после клика
    int clicked_level = -1;                     // кликнутый уровень
    bool should_show_main_menu = true;          // нужно ли показывать главное меню
    while(manageGameCycle(window, scale_x, scale_y, lvl_num,clicked_level, should_show_main_menu)){
        if(lvl_num == -1 || clicked_level == -1){
            lvl_num = clicked_level;
        }
        if(!window.isOpen()) break;
    }
    return 0;

}


bool manageGameCycle(sf::RenderWindow& window, const double scale_x, const double scale_y, int& level_number, int& clicked_level, bool& should_show_main_menu){
    bool should_continue = true;
    sf::Text warning;
    warning.setFont(Loader::main_menu_font);
    const int frames_to_show_warning = 15;
    int past_frames_to_show_warning = 0;
    warning.setString("Not enough money!");
    warning.setCharacterSize(static_cast<int>(std::min(scale_x, scale_y) *  100.0f));
    warning.setFillColor(sf::Color::Red);
    warning.setOrigin(warning.getGlobalBounds().width / 2.0f, warning.getGlobalBounds().height / 2.0f);
    warning.setPosition(1000, 520);

    int passed_levels = 1;
    std::ifstream save_file_read;
    std::ofstream save_file_write;
    save_file_read.open("src/saves/save.txt");
    save_file_write.open("src/saves/save.txt", std::ios::app);
    if(!save_file_read.is_open()){
        save_file_write.close();
        save_file_write.open("src/saves/save.txt");
        save_file_write << 1;
    } else{
        int lvl = 1;
        save_file_read >> lvl;
        if(lvl > 0)
            passed_levels = lvl;
        else {
            save_file_write.close();
            save_file_write.open("src/saves/save.txt");
            save_file_write << 1;
        }
    }
    save_file_write.close();
    save_file_read.close();
    int score_number = 300;
    int local_level_number = level_number == -1 ? 1 : level_number;
    LevelMaker lvlmaker("src/levels/", "level", local_level_number, ".tmx", scale_x, scale_y);
    std::ifstream level_info;
    level_info.open("src/levels/level" + std::to_string(local_level_number) + ".txt");
    int dollars_to_win, number_of_enemies;
    level_info >> dollars_to_win >> number_of_enemies;
    std::vector<int> types_of_enemies(number_of_enemies);
    for(auto& x : types_of_enemies){
        level_info >> x;
    }
    int deleted_enemies = 0;
    int type_ind = 0;
    GridMaker grid_maker(window, 16);
    int chosen_level = -1;
    sf::Clock clock;
    float time_for_enemy = 0;
    float delta_time = 0;
    bool delta_should_stay = false;

    std::vector<std::shared_ptr<BaseTower>> towers;
    std::vector<ButtonSpr> tower_buttons;
    for(const auto& obj : lvlmaker.returnTowers()){
        towers.push_back(std::make_shared<LightTower>(sf::Vector2i(static_cast<int>(obj.rect.left), static_cast<int>(obj.rect.top)), 64, &clock, scale_x, scale_y));
        tower_buttons.emplace_back(ButtonSpr(TowerTextureLoader::place_for_tower, TowerTextureLoader::targeted_place_for_tower
                , TowerTextureLoader::targeted_place_for_tower, sf::Vector2f(obj.rect.left, obj.rect.top), 64, 64));
    }

    int sprite_side = 64;
    std::vector<std::shared_ptr<BaseEnemy>> enemies;


    auto [main_menu, main_menu_buttons] = createMainMenu(Loader::main_menu_font, scale_x, scale_y);
    auto [pause_menu, pause_menu_buttons] = createPauseMenu(Loader::main_menu_font, scale_x, scale_y);
    auto [menu_towers, menu_towers_buttons] = createTowerMenu(Loader::main_menu_font, scale_x, scale_y);
    auto [level_menu, level_menu_buttons] = createLevelMenu(Loader::main_menu_font, passed_levels, scale_x, scale_y);
    auto [victory_menu, victory_menu_buttons] = createVictoryMenu(Loader::main_menu_font, scale_x, scale_y);
    auto [defeat_menu, defeat_menu_buttons] = createDefeatMenu(Loader::main_menu_font, scale_x, scale_y);
    const float interval = 1600.0f;
    sf::CircleShape range_circle;
    Loader::main_theme.play();
    while (window.isOpen()) {
        if(level_number != -1){
            if(!should_show_main_menu)
                main_menu.setVisible(false);
        }
        if(main_menu.isVisible()){
            manageMainMenu(window, Loader::main_menu_image, main_menu, main_menu_buttons, level_menu, scale_x, scale_y);
        }
        else if(level_menu.isVisible()){
            manageLevelMenu(window, Loader::main_menu_image, level_menu, level_menu_buttons, chosen_level, passed_levels, scale_x, scale_y);
            clicked_level = chosen_level;
        } else if(victory_menu.isVisible()){
            int cur_level = level_number;
            should_continue = manageVictoryMenu(window, victory_menu, victory_menu_buttons, cur_level);
            if(cur_level != level_number){
                clicked_level = cur_level;
                level_number = -1;
                return true;
            }
            if(should_continue) {
                should_show_main_menu = true;
                clicked_level = -1;
                return should_continue;
            }

        } else if(defeat_menu.isVisible()){
            bool try_again = false;
            should_continue = manageDefeatMenu(window, defeat_menu, defeat_menu_buttons, try_again);
            if(try_again){
                level_number = -1;
                return true;
            }
            if(should_continue) {
                should_show_main_menu = true;
                clicked_level = -1;
                return should_continue;
            }
        }
        else if(pause_menu.isVisible()){
            should_continue = managePauseMenu(window, pause_menu, pause_menu_buttons);
            if(should_continue) {
                should_show_main_menu = true;
                clicked_level = -1;
                return should_continue;
            }
        }
        else {
            if(level_number == -1){
                should_show_main_menu = false;
                return true;
            }
            bool should_draw_warning = false;
            Loader::main_theme.stop();
            if(Loader::game_theme.getStatus() == sf::Music::Stopped)
                Loader::game_theme.play();
            float tm = clock.getElapsedTime().asMilliseconds();
            if(!delta_should_stay)
                delta_time = tm;
            else
                delta_should_stay = false;
            clock.restart();
            time_for_enemy += delta_time;
            tm /= 10.0f;
            if (time_for_enemy > interval && type_ind < number_of_enemies) {
                if(types_of_enemies[type_ind] == 1)
                    enemies.emplace_back(std::make_shared<LightEnemy>(lvlmaker, clock, grid_maker, sprite_side, scale_x, scale_y));
                else
                    enemies.emplace_back(std::make_shared<HeavyEnemy>(lvlmaker, clock, grid_maker, sprite_side, scale_x, scale_y));
                time_for_enemy  = 0;
                ++type_ind;
            }
            sf::Event event;
            while (window.pollEvent(event)) {

                bool should_break = false;
                switch (event.type) {
                    case sf::Event::Closed:
                        window.close();
                        break;
                    case sf::Event::KeyPressed:
                        if (event.key.code == sf::Keyboard::Escape) {
                            if(menu_towers.isVisible()){
                                menu_towers.setVisible(false);
                                tower_buttons[menu_towers.getTowerIndex()].setPermanentlyClicked(false);
                                sf::CircleShape a;
                                range_circle = a;
                                break;
                            }
                            pause_menu.setVisible(true);
                            delta_should_stay = true;
                            break;
                        }
                    case sf::Event::Resized:
                        grid_maker.updateGrid(window);
                        break;
                    case sf::Event::MouseButtonPressed: {
                        sf::Vector2i cursor = sf::Mouse::getPosition(window);
                        switch (event.mouseButton.button) {
                            case sf::Mouse::Button::Left:
                                size_t c = 0;
                                for (auto &button : tower_buttons) {
                                    if (button.isVisible() && button.isActivated(cursor) && !menu_towers.isVisible()) {
                                        menu_towers.setTowerIndex(c);
                                        menu_towers.setVisible(true);
                                        button.setPermanentlyClicked(true);
                                    }
                                    ++c;
                                }
                                if (menu_towers.isVisible()) {
                                    int cnt = 0;
                                    for (auto &button : menu_towers_buttons) {
                                        if (button->isVisible() && button->isActivated(cursor)) {
                                            if (cnt == 2) {
                                                menu_towers.setVisible(false);
                                                tower_buttons[menu_towers.getTowerIndex()].setPermanentlyClicked(false);
                                                sf::CircleShape a;
                                                range_circle = a;
                                                break;
                                            }
                                            if (cnt == 0) {
                                                int cost = towers[menu_towers.getTowerIndex()]->getCost();
                                                if(cost > score_number){
                                                    should_draw_warning = true;
                                                    continue;
                                                }
                                                score_number -= cost;
                                                towers[menu_towers.getTowerIndex()]->summon();
                                                tower_buttons[menu_towers.getTowerIndex()].setVisible(false);
                                                tower_buttons[menu_towers.getTowerIndex()].setPermanentlyClicked(false);
                                                menu_towers.setVisible(false);
                                                sf::CircleShape a;
                                                range_circle = a;
                                                break;
                                            }
                                            if(cnt == 1){
                                                size_t ind = menu_towers.getTowerIndex();
                                                changeTower(towers, HeavyTower(towers[ind]->getPos(), towers[ind]->getSide(), towers[ind]->getClockptr(), scale_x, scale_y), ind);
                                                int cost = towers[ind]->getCost();
                                                if(cost > score_number){
                                                    changeTower(towers, LightTower(towers[ind]->getPos(), towers[ind]->getSide(), towers[ind]->getClockptr(), scale_x, scale_y), ind);
                                                    should_draw_warning = true;
                                                    continue;
                                                }
                                                score_number -= cost;
                                                towers[ind]->summon();
                                                tower_buttons[ind].setVisible(false);
                                                tower_buttons[ind].setPermanentlyClicked(false);
                                                menu_towers.setVisible(false);
                                                sf::CircleShape a;
                                                range_circle = a;
                                                break;
                                            }
                                        }
                                        ++cnt;
                                    }
                                }
                                break;
                        }
                        should_break = true;
                        break;
                    }
                    case sf::Event::MouseMoved:
                        sf::Vector2i cursor = sf::Mouse::getPosition(window);
                        for (auto &button : tower_buttons) {
                            button.checkCursor(cursor);
                        }
                        if (menu_towers.isVisible()) {
                            size_t cnt = 0;
                            for (auto &button : menu_towers_buttons) {
                                if(cnt == 0){
                                    if(button->checkCursor(cursor)){
                                        range_circle = towers[menu_towers.getTowerIndex()]->getCircleRange();
                                        break;
                                    }

                                }
                                if(cnt == 1){
                                    if(button->checkCursor(cursor)){
                                        HeavyTower temp(towers[menu_towers.getTowerIndex()]->getPos()
                                                , towers[menu_towers.getTowerIndex()]->getSide(), towers[menu_towers.getTowerIndex()]->getClockptr(), scale_x, scale_y);
                                        range_circle = temp.getCircleRange();
                                        break;
                                    }

                                }
                                sf::CircleShape a;
                                range_circle = a;
                                button->checkCursor(cursor);
                                ++cnt;
                            }
                        }
                        break;
                }
                if (should_break) break;
            }
            lvlmaker.drawLevel(window);
            for (auto &button : tower_buttons) {
                button.draw(window);
            }


            updateQuery(towers, enemies);
            for (auto &tower : towers) {
                if (tower->getState() == READY) {
                    tower->isTimeToShoot();
                }
            }
            for (auto &tower : towers) {
                if (tower->getState() == READY) {
                    tower->rotate();
                }
            }

            std::vector<std::vector<std::shared_ptr<BaseEnemy>>::iterator> should_be_deleted;
            for (auto &tower : towers) {
                if (tower->getState() == RELOADING) {
                    auto to_delete = enemies.end();
                    if (tower->checkHit(to_delete)) {
                        tower->reload();
                        if((*to_delete)->getHealth() <= 0) {
                            ++deleted_enemies;
                            score_number += (*to_delete)->getPoints();
                            should_be_deleted.push_back(to_delete);
                        }
                        continue;
                    }
                    tower->flyRocket();
                }
            }
            std::vector<std::shared_ptr<BaseEnemy>> new_enemies;
            for (auto it = enemies.begin(); it != enemies.end(); ++it) {
                bool flag = true;
                for (auto to_delete : should_be_deleted) {
                    if (to_delete == it) {
                        flag = false;
                        break;
                    }
                }
                if (flag) {
                    new_enemies.push_back(*it);
                }
            }
            enemies = new_enemies;


            for (auto &tower : towers) {
                tower->draw(window);
            }
            window.draw(range_circle);
            std::vector<std::vector<std::shared_ptr<BaseEnemy>>::iterator> enemies_to_delete;
            for (auto it = enemies.begin(); it != enemies.end(); ++it) {
                (*it)->draw(window);
                if ((*it)->shouldBeDeleted()) {
                    ++deleted_enemies;
                    score_number -= 300;
                    enemies_to_delete.push_back(it);
                }
                (*it)->move(clock);
            }
            menu_towers.draw(window);
            new_enemies.clear();
            for (auto it = enemies.begin(); it != enemies.end(); ++it) {
                bool flag = true;
                for (auto enemy_to_delete : enemies_to_delete) {
                    if (enemy_to_delete == it) {
                        flag = false;
                        break;
                    }
                }
                if (flag) {
                    new_enemies.push_back(*it);
                }
            }
            enemies = new_enemies;
            updateQuery(towers, enemies);
            if(should_draw_warning || past_frames_to_show_warning > 0){
                window.draw(warning);
                ++past_frames_to_show_warning;
                if(past_frames_to_show_warning > frames_to_show_warning){
                    past_frames_to_show_warning = 0;
                }
            }
            sf::Text score;
            sf::Text required_sum;

            score.setFont(Loader::main_menu_font);
            required_sum.setFont(Loader::main_menu_font);
            score.setOutlineColor(sf::Color(255, 100, 0, 255));
            required_sum.setOutlineColor(sf::Color(255, 40, 0, 255));
            score.setOutlineThickness(3);
            required_sum.setOutlineThickness(3);
            score.setFillColor(sf::Color(255, 195, 0, 255));
            required_sum.setFillColor(sf::Color(255, 80, 0, 255));
            score.setCharacterSize(static_cast<int>(std::min(scale_x, scale_y) * 96));
            required_sum.setCharacterSize(static_cast<int>(std::min(scale_x, scale_y) * 96));
            if(score_number < 0){
                score.setFillColor(sf::Color::Red);
            }
            required_sum.setString("You need\n" + std::to_string(dollars_to_win) + "$");
            score.setString(std::to_string(score_number) + "$");
            required_sum.setPosition(10,880);
            score.setOrigin(score.getGlobalBounds().width, 0);
            score.setPosition(1900, 0);
            if(deleted_enemies >= number_of_enemies){
                if(score_number >= dollars_to_win){
                    victory_menu.setVisible(true);
                    std::ofstream save_file;
                    save_file.open("src/saves/save.txt");
                    save_file << std::max(passed_levels, level_number + 1);
                    save_file.close();
                } else{
                    defeat_menu.setVisible(true);
                }
            }
            window.draw(required_sum);
            window.draw(score);
        }
        window.display();
    }
    Loader::game_theme.stop();
    Loader::main_theme.stop();
    return should_continue;
}


void updateQuery(std::vector<std::shared_ptr<BaseTower>>& towers, std::vector<std::shared_ptr<BaseEnemy>>& enemies){
    for(auto& tower : towers){
        std::vector<std::vector<std::shared_ptr<BaseEnemy>>::iterator> query;
        for(auto it = enemies.begin(); it != enemies.end(); ++it){
            query.push_back(it);
        }
        tower->setQuery(query);
    }
}


std::pair<ContextMenu, std::vector<std::shared_ptr<Button>>> createTowerMenu(const sf::Font& font, const double scale_x, const double scale_y){
    float menu_left = 0 * scale_x, menu_top = 300 * scale_y, menu_width = 400 * scale_x, menu_height = 720 * scale_y;
    float button_width  = 300 * scale_x, button_height = 80 * scale_y;
    float button_left = menu_left + (menu_width - button_width) / 2.0;
    float period = 100 * scale_y;
    float button_top = menu_top + 2.0 * period;

    sf::FloatRect parameters(menu_left, menu_top, menu_width, menu_height);
    std::vector<std::shared_ptr<Button>> buttons;
    buttons.push_back(std::make_shared<ButtonRec>(sf::FloatRect(button_left, button_top, button_width, button_height)
        , sf::Color(230, 135, 100, 255)
        , sf::Color::Green, sf::Color::Green, sf::Color::White, font, std::min(scale_x, scale_y) * 26, "Light Tower\n\n\t200$", 3));
    buttons.push_back(std::make_shared<ButtonRec>(sf::FloatRect(button_left, button_top + period, button_width, button_height)
            , sf::Color(230, 135, 100, 255)
            , sf::Color::Green, sf::Color::Green, sf::Color::White, font, std::min(scale_x, scale_y) * 26, "Heavy Tower\n\n\t500$", 3));
    buttons.push_back(std::make_shared<ButtonRec>(sf::FloatRect(button_left, button_top + period * 3, button_width, button_height)
            , sf::Color(230, 135, 100, 255)
            , sf::Color::Red, sf::Color::Red, sf::Color::White, font, std::min(scale_x, scale_y) * 26, "Close", 3));
    ContextMenu menu(parameters, font, static_cast<int>(std::min(scale_x, scale_y) * 64), "Pick Tower", buttons
        ,sf::Color(255, 250, 165, 255)
        , sf::Color(230, 165, 50, 255)
        , sf::Color::Red);
    return {menu, buttons};
}

std::pair<ContextMenu, std::vector<std::shared_ptr<Button>>> createMainMenu(const sf::Font& font, const double scale_x, const double scale_y){
    float menu_left = 0 * scale_x, menu_top = 0 * scale_y, menu_width = 1920 * scale_x, menu_height = 1080 * scale_y;
    float button_width  = 400 * scale_x, button_height = 100 * scale_y;
    float button_left = menu_left + (menu_width - button_width) / 2.0;
    float period = 150 * scale_y;
    float button_top = menu_top + 2.0 * period;
    sf::FloatRect parameters(menu_left, menu_top, menu_width, menu_height);
    std::vector<std::shared_ptr<Button>> buttons;
    buttons.push_back(std::make_shared<ButtonRec>(sf::FloatRect(button_left, button_top + period, button_width, button_height)
            , sf::Color(255, 215, 0, 255)
            , sf::Color::Green, sf::Color::Green, sf::Color::White, font, std::min(scale_x, scale_y) * 48, "Play", 5));

    buttons.push_back(std::make_shared<ButtonRec>(sf::FloatRect(button_left, button_top + period * 3, button_width, button_height)
            , sf::Color(255, 120, 60, 255)
            , sf::Color::Red, sf::Color::Red, sf::Color::White, font, std::min(scale_x, scale_y) * 48, "Exit", 5));
    ContextMenu menu(parameters, font, static_cast<int>(std::min(scale_x, scale_y) * 128), "Tower\n          Royal", buttons
        ,sf::Color(255, 250, 165, 0)
        , sf::Color(230, 165, 50, 255)
        , sf::Color::Red);
    menu.setVisible(true);
    return {menu, buttons};
}

std::pair<ContextMenu, std::vector<std::shared_ptr<Button>>> createLevelMenu(const sf::Font& font, const int passed_levels, const double scale_x, const double scale_y){
    float menu_left = 0 * scale_x, menu_top = 0 * scale_y, menu_width = 1920 * scale_x, menu_height = 1080 * scale_y;
    float button_width  = 75 * scale_x, button_height = 75 * scale_y;
    float period = 20 * scale_x;
    float button_left = period;
    float button_top = menu_height / 6;
    int lvl_in_row = 20;
    int rows = 4;
    sf::FloatRect parameters(menu_left, menu_top, menu_width, menu_height);
    std::vector<std::shared_ptr<Button>> buttons;
    sf::Color c = sf::Color::Green;
    sf::Color g = sf::Color::Cyan;
    if(passed_levels == 1)
        c = sf::Color::Yellow;
    int cnt = 1;
    for(int j = 1; j <= rows; ++j) {
        for (size_t i = 1; i <= lvl_in_row; ++i) {
            buttons.push_back(std::make_shared<ButtonRec>(
                    sf::FloatRect(button_left + (i - 1) * (button_width + period), button_top + (j - 1) * 10 * period, button_width,
                                  button_height),
                    c, g, g, sf::Color::White, font, std::min(scale_x, scale_y) * 24, std::to_string(i+(j-1) * lvl_in_row),
                    3));
            ++cnt;
            if(cnt == passed_levels){
                c = sf::Color::Yellow;
            }
            if(cnt > passed_levels){
                c = sf::Color(128,128,128);
                g = sf::Color::Red;
            }
        }
    }

    ContextMenu menu(parameters, font, static_cast<int>(std::min(scale_x, scale_y) * 128), "Pick the level", buttons
            ,sf::Color(255, 250, 165, 0)
            , sf::Color(230, 165, 50, 0)
            , sf::Color::Red);
    return {menu, buttons};
}

std::pair<ContextMenu, std::vector<std::shared_ptr<Button>>> createPauseMenu(const sf::Font& font, const double scale_x, const double scale_y){
    float menu_left = 0 * scale_x, menu_top = 0 * scale_y, menu_width = 1920 * scale_x, menu_height = 1080 * scale_y;
    float button_width  = 400 * scale_x, button_height = 100 * scale_y;
    float button_left = menu_left + (menu_width - button_width) / 2.0;
    float period = 150 * scale_y;
    float button_top = menu_top + 2.0 * period;
    sf::FloatRect parameters(menu_left, menu_top, menu_width, menu_height);
    std::vector<std::shared_ptr<Button>> buttons;
    buttons.push_back(std::make_shared<ButtonRec>(sf::FloatRect(button_left, button_top + period, button_width, button_height)
            , sf::Color(255, 215, 0, 255)
            , sf::Color::Green, sf::Color::Green, sf::Color::White, font, std::min(scale_x, scale_y) * 48, "Continue", 5));

    buttons.push_back(std::make_shared<ButtonRec>(sf::FloatRect(button_left, button_top + period * 2, button_width, button_height)
            , sf::Color(255, 215, 0, 255)
            , sf::Color::Green, sf::Color::Green, sf::Color::White, font, std::min(scale_x, scale_y) * 48, "Return to menu", 5));

    buttons.push_back(std::make_shared<ButtonRec>(sf::FloatRect(button_left, button_top + period * 3, button_width, button_height)
            , sf::Color(255, 120, 60, 255)
            , sf::Color::Red, sf::Color::Red, sf::Color::White, font, std::min(scale_x, scale_y) * 48, "Exit", 5));
    ContextMenu menu(parameters, font, static_cast<int>(std::min(scale_x, scale_y) * 128), "Paused", buttons
            ,sf::Color(255, 250, 165, 1)
            , sf::Color(230, 165, 50, 255)
            , sf::Color::Red);
    menu.setVisible(false);
    return {menu, buttons};
}

std::pair<ContextMenu, std::vector<std::shared_ptr<Button>>> createVictoryMenu(const sf::Font& font, const double scale_x, const double scale_y){
    float menu_left = 0 * scale_x, menu_top = 0 * scale_y, menu_width = 1920 * scale_x, menu_height = 1080 * scale_y;
    float button_width  = 400 * scale_x, button_height = 100 * scale_y;
    float button_left = menu_left + (menu_width - button_width) / 2.0;
    float period = 150 * scale_y;
    float button_top = menu_top + 2.0 * period;
    sf::FloatRect parameters(menu_left, menu_top, menu_width, menu_height);
    std::vector<std::shared_ptr<Button>> buttons;
    buttons.push_back(std::make_shared<ButtonRec>(sf::FloatRect(button_left, button_top + period, button_width, button_height)
            , sf::Color(255, 215, 0, 255)
            , sf::Color::Green, sf::Color::Green, sf::Color::White, font, std::min(scale_x, scale_y) * 48, "Next Level", 5));

    buttons.push_back(std::make_shared<ButtonRec>(sf::FloatRect(button_left, button_top + period * 2, button_width, button_height)
            , sf::Color(255, 215, 0, 255)
            , sf::Color::Yellow, sf::Color::Yellow, sf::Color::White, font, std::min(scale_x, scale_y) * 48, "Return to menu", 5));

    buttons.push_back(std::make_shared<ButtonRec>(sf::FloatRect(button_left, button_top + period * 3, button_width, button_height)
            , sf::Color(255, 120, 60, 255)
            , sf::Color::Red, sf::Color::Red, sf::Color::White, font, std::min(scale_x, scale_y) * 48, "Exit", 5));
    ContextMenu menu(parameters, font, std::min(scale_x, scale_y) * 128, "VICTORY!!!", buttons
            ,sf::Color(25, 128, 0, 255)
            , sf::Color(230, 165, 50, 255)
            , sf::Color::Red);
    menu.setVisible(false);
    return {menu, buttons};
}

std::pair<ContextMenu, std::vector<std::shared_ptr<Button>>> createDefeatMenu(const sf::Font& font, const double scale_x, const double scale_y){
    float menu_left = 0 * scale_x, menu_top = 0 * scale_y, menu_width = 1920 * scale_x, menu_height = 1080 * scale_y;
    float button_width  = 400 * scale_x, button_height = 100 * scale_y;
    float button_left = menu_left + (menu_width - button_width) / 2.0;
    float period = 150 * scale_y;
    float button_top = menu_top + 2.0 * period;
    sf::FloatRect parameters(menu_left, menu_top, menu_width, menu_height);
    std::vector<std::shared_ptr<Button>> buttons;
    buttons.push_back(std::make_shared<ButtonRec>(sf::FloatRect(button_left, button_top + period, button_width, button_height)
            , sf::Color(255, 215, 0, 255)
            , sf::Color::Green, sf::Color::Green, sf::Color::White, font, std::min(scale_x, scale_y) * 48, "Try Again", 5));

    buttons.push_back(std::make_shared<ButtonRec>(sf::FloatRect(button_left, button_top + period * 2, button_width, button_height)
            , sf::Color(255, 215, 0, 255)
            , sf::Color::Yellow, sf::Color::Yellow, sf::Color::White, font, std::min(scale_x, scale_y) * 48, "Return to menu", 5));

    buttons.push_back(std::make_shared<ButtonRec>(sf::FloatRect(button_left, button_top + period * 3, button_width, button_height)
            , sf::Color(255, 120, 60, 255)
            , sf::Color::Red, sf::Color::Red, sf::Color::White, font, std::min(scale_x, scale_y) * 48, "Exit", 5));
    ContextMenu menu(parameters, font, std::min(scale_x, scale_y) * 128, "DEFEAT!", buttons
            ,sf::Color(128, 30, 0, 255)
            , sf::Color(230, 165, 50, 255)
            , sf::Color::Red);
    menu.setVisible(false);
    return {menu, buttons};
}

bool manageMainMenu(sf::RenderWindow& window, sf::Texture& main_menu_image, ContextMenu& main_menu
    , std::vector<std::shared_ptr<Button>>& main_menu_buttons, ContextMenu& level_menu, const double scale_x, const double scale_y){
    sf::Event event;
    while (window.pollEvent(event)) {
        bool should_break = false;
        switch (event.type) {
            case sf::Event::Closed:
                window.close();

                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();

                    break;
                }
            case sf::Event::MouseButtonPressed: {
                sf::Vector2i cursor = sf::Mouse::getPosition(window);
                switch (event.mouseButton.button) {
                    case sf::Mouse::Button::Left:
                        size_t cnt = 0;
                        for(auto& button : main_menu_buttons){
                            if(cnt == 0 && button->isActivated(cursor)){
                                main_menu.setVisible(false);
                                level_menu.setVisible(true);
                                should_break = true;
                                break;
                            }
                            if(cnt == main_menu_buttons.size() - 1 && button->isActivated(cursor)){
                                window.close();
                                break;
                            }
                            ++cnt;
                        }
                }
                break;
            }
            case sf::Event::MouseMoved:
                sf::Vector2i cursor = sf::Mouse::getPosition(window);
                for (auto &button : main_menu_buttons) {
                    button->checkCursor(cursor);
                }
                break;
        }
    }
    sf::Sprite main_menu_sprite;
    main_menu_sprite.setTexture(main_menu_image);
    main_menu_sprite.setScale(scale_x, scale_y);
    window.draw(main_menu_sprite);
    main_menu.draw(window);
    return false;
}


bool manageLevelMenu(sf::RenderWindow& window, sf::Texture& main_menu_image, ContextMenu& level_menu
    , std::vector<std::shared_ptr<Button>>& level_menu_buttons, int& chosen_level, const int passed_levels, const double scale_x, const double scale_y){
    sf::Event event;
    while (window.pollEvent(event)) {
        bool should_break = false;
        switch (event.type) {
            case sf::Event::MouseButtonPressed: {
                sf::Vector2i cursor = sf::Mouse::getPosition(window);
                switch (event.mouseButton.button) {
                    case sf::Mouse::Button::Left:
                        int cnt = 0;
                        for(auto& button : level_menu_buttons){
                            if(cnt < passed_levels && button->isActivated(cursor)){
                                level_menu.setVisible(false);
                                chosen_level = cnt + 1;
                                should_break = true;
                                break;
                            }
                            ++cnt;
                        }
                }
                break;
            }
            case sf::Event::MouseMoved:
                sf::Vector2i cursor = sf::Mouse::getPosition(window);
                for (auto &button : level_menu_buttons) {

                    button->checkCursor(cursor);
                }
                break;
        }
        if(should_break) break;
    }
    sf::Sprite level_menu_sprite;
    level_menu_sprite.setTexture(main_menu_image);
    level_menu_sprite.setScale(scale_x, scale_y);
    window.draw(level_menu_sprite);
    level_menu.draw(window);
    return false;
}

bool managePauseMenu(sf::RenderWindow& window, ContextMenu& pause_menu, std::vector<std::shared_ptr<Button>>& pause_menu_buttons){
    sf::Event event;
    while (window.pollEvent(event)) {
        bool should_break = false;
        switch (event.type) {
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) {
                    pause_menu.setVisible(false);

                    break;
                }
            case sf::Event::MouseButtonPressed: {
                sf::Vector2i cursor = sf::Mouse::getPosition(window);
                switch (event.mouseButton.button) {
                    case sf::Mouse::Button::Left:
                        size_t cnt = 0;
                        for(auto& button : pause_menu_buttons){
                            if(cnt == 0 && button->isActivated(cursor)){
                                pause_menu.setVisible(false);
                                should_break = true;
                                break;
                            }
                            if(cnt == 1 && button->isActivated(cursor)){
                                pause_menu.setVisible(false);
                                should_break = true;
                                return true;
                                break;
                            }
                            if(cnt == pause_menu_buttons.size() - 1 && button->isActivated(cursor)){
                                pause_menu.setVisible(false);
                                window.close();
                                return false;
                                break;
                            }
                            ++cnt;
                        }
                }
                break;
            }
            case sf::Event::MouseMoved:
                sf::Vector2i cursor = sf::Mouse::getPosition(window);
                for (auto &button : pause_menu_buttons) {
                    button->checkCursor(cursor);
                }
                break;
        }
    }
    pause_menu.draw(window);
    return false;
}

bool manageVictoryMenu(sf::RenderWindow& window, ContextMenu& victory_menu, std::vector<std::shared_ptr<Button>>& victory_menu_buttons
, int& chosen_level){
    sf::Event event;

    while (window.pollEvent(event)) {
        bool should_break = false;
        switch (event.type) {
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) {
                    victory_menu.setVisible(false);

                    break;
                }
            case sf::Event::MouseButtonPressed: {
                sf::Vector2i cursor = sf::Mouse::getPosition(window);
                switch (event.mouseButton.button) {
                    case sf::Mouse::Button::Left:
                        size_t cnt = 0;
                        for(auto& button : victory_menu_buttons){
                            if(cnt == 0 && button->isActivated(cursor)){
                                victory_menu.setVisible(false);
                                should_break = true;
                                ++chosen_level;
                                break;
                            }
                            if(cnt == 1 && button->isActivated(cursor)){
                                victory_menu.setVisible(false);
                                should_break = true;
                                return true;
                                break;
                            }
                            if(cnt == victory_menu_buttons.size() - 1 && button->isActivated(cursor)){
                                victory_menu.setVisible(false);
                                window.close();
                                return false;
                                break;
                            }
                            ++cnt;
                        }
                }
                break;
            }
            case sf::Event::MouseMoved:
                sf::Vector2i cursor = sf::Mouse::getPosition(window);
                for (auto &button : victory_menu_buttons) {
                    button->checkCursor(cursor);
                }
                break;
        }
    }
    victory_menu.draw(window);
    return false;
}

bool manageDefeatMenu(sf::RenderWindow& window, ContextMenu& defeat_menu, std::vector<std::shared_ptr<Button>>& defeat_menu_buttons, bool& try_again){
    sf::Event event;
    while (window.pollEvent(event)) {
        bool should_break = false;
        switch (event.type) {
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) {
                    defeat_menu.setVisible(false);

                    break;
                }
            case sf::Event::MouseButtonPressed: {
                sf::Vector2i cursor = sf::Mouse::getPosition(window);
                switch (event.mouseButton.button) {
                    case sf::Mouse::Button::Left:
                        size_t cnt = 0;
                        for(auto& button : defeat_menu_buttons){
                            if(cnt == 0 && button->isActivated(cursor)){
                                defeat_menu.setVisible(false);
                                should_break = true;
                                try_again = true;
                                break;
                            }
                            if(cnt == 1 && button->isActivated(cursor)){
                                defeat_menu.setVisible(false);
                                should_break = true;
                                return true;
                                break;
                            }
                            if(cnt == defeat_menu_buttons.size() - 1 && button->isActivated(cursor)){
                                defeat_menu.setVisible(false);
                                window.close();
                                return false;
                                break;
                            }
                            ++cnt;
                        }
                }
                break;
            }
            case sf::Event::MouseMoved:
                sf::Vector2i cursor = sf::Mouse::getPosition(window);
                for (auto &button : defeat_menu_buttons) {
                    button->checkCursor(cursor);
                }
                break;
        }
    }
    defeat_menu.draw(window);
    return false;
}

template<class Tower>
void changeTower(std::vector<std::shared_ptr<BaseTower>>& towers, Tower new_tower, const size_t ind){
    towers[ind] = std::make_shared<Tower>(std::move(new_tower));
}