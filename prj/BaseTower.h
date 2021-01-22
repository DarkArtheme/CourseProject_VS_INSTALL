#pragma once
#ifndef SFML_SANDBOX_BASETOWER_H
#define SFML_SANDBOX_BASETOWER_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "BaseEnemy.h"
//#include <deque>
#include <cmath>
#include <memory>

///@brief Состояние башни.
enum State{
    ///@brief Башня еще не построена.
    NOT_BUILT,
    ///@brief Выбрана кнопка постройки башни.
    TARGETED_NOT_BUILD,
    ///@brief Башня готова к стрельбе.
    READY,
    ///@brief Башня перезаряжается.
    RELOADING,
};

///@brief Класс, подгружающий все текстуры для башен.
class TowerTextureLoader{
public:
    ///@brief Подгружает все поля класса.
    static void LoadTextures(const std::string &path1, const std::string &path2);
    inline static sf::Texture place_for_tower;
    inline static sf::Texture targeted_place_for_tower;
    inline static sf::Texture base_texture;
    inline static sf::Texture tower_texture_1;
    inline static sf::Texture empty_tower_texture_1;
    inline static sf::Texture tower_texture_2;
    inline static sf::Texture rocket;
    inline static sf::Texture smoke;
    inline static sf::Texture explosion;
};

///@brief Класс, подгружающий все звуки для башен.
class TowerSoundLoader{
public:
    ///@brief Подгружает все поля класса.
    static void LoadSounds(const std::string &folder_path);
    inline static sf::SoundBuffer shot1;
    inline static sf::SoundBuffer shot2;
    inline static sf::SoundBuffer explosion;
};

///@brief Абстрактный класс, задающий общий интерфейс башен.
class BaseTower {
public:
    BaseTower() = default;
    BaseTower(const BaseTower&) = default;
    BaseTower& operator=(const BaseTower&) = default;
    ~BaseTower() = default;
    /**
     * @return Координаты верхнего левого угла башни.
     */
    sf::Vector2i getPos() const { return pos; }
    /**
     * @return Размер стороны квадрата спрайта башни.
     */
    int getSide() const { return side; }
    /**
     * @return Стоимость башни.
     */
    int getCost() const { return cost; }
    /**
     * @return Указатель на таймер внутри основного игрового цикла.
     */
    sf::Clock* getClockptr() const { return clock_ptr; }
    /**
     * @return Круг, представляющий собой область поражения башни.
     */
    sf::CircleShape getCircleRange() const { return circle; }
    /**
     *
     * @return Текущее состояние башни.
     */
    State getState() const { return state; }
    /**
     * @brief Отрисовывает башню.
     * @param window Окно игры.
     */
    void draw(sf::RenderTarget& window);
    /**
     * @brief Размещает башню.
     */
    void summon();
    /**
     * @brief Производит выстрел башни.
     */
    void shoot();
    /**
     * @brief Производит перезарядку башни.
     */
    void reload();
    /**
     * @brief Производит полет ракеты.
     */
    void flyRocket();
    /**
     * @brief Производит поворот башни.
     */
    void rotate();
    /**
     * @brief Устанавливает врагов в радиусе поражения.
     * @param query_ Вектор итераторов основного вектора врагов.
     */
    void setQuery(const std::vector<std::vector<std::shared_ptr<BaseEnemy>>::iterator>& query_);
    /**
     * @brief Проверяет, попала ли ракета в какого-либо врага в радиусе поражения.
     * @param it Ссылка на итератор, в который записывается, в какого врага попала ракета (если ракета не попала, то записи не происходит).
     * @return Логическое значение, означающее, попала ли ракета в какого-либо врага в радиусе поражения.
     */
    bool checkHit(std::vector<std::shared_ptr<BaseEnemy>>::iterator& it);
    /**
     * @brief Проверяет, настало ли время выстрела.
     */
    void isTimeToShoot();
protected:
    static constexpr double PI = 3.1415926535;
    static constexpr unsigned int frames_for_explosion = 6;
    sf::Vector2i pos;
    int side{64};
    sf::Sprite sprite_base;
    sf::Sprite sprite_tower;
    sf::Sprite sprite_rocket;
    sf::Sprite smoke;
    sf::Sprite sprite_explosion;
    State state{State::NOT_BUILT};
    sf::Clock* clock_ptr{nullptr};
    sf::Clock cooldown_clock;
    sf::Sound sound_shot;
    sf::Sound sound_explosion;
    int cost{0};
    float cooldown{0};
    float fire_range{0};
    sf::CircleShape circle;
    float rotation_speed{0};
    float rocket_speed{0};
    float angle{0};
    int damage{0};
    double scale_x{1.0}, scale_y{1.0};
    bool explosion_is_visible{false};
    unsigned int cur_frame{0};
    std::vector<std::vector<std::shared_ptr<BaseEnemy>>::iterator> query;
    double dtor(const double deg) { return deg * PI / 180.0; }
    double rtod(const double rad) { return rad * 180.0 / PI; }
    bool isInRange(const sf::Vector2f& pt) const;

    bool isEqualExact(const double lhs, const double rhs) { return std::abs(rhs - lhs) < 1e-1; }
    bool isEqualApprox(const double lhs, const double rhs) { return std::abs(rhs - lhs) < 1.3; }
};

///@brief Класс первого вида башен (легкая башня).
class LightTower : public BaseTower{
public:
    LightTower() = delete;
    ~LightTower() = default;
    LightTower(const LightTower&) = default;
    LightTower& operator=(const LightTower&) = default;
    /**
     * @brief Конструктор.
     * @param position Координаты верхнего левого угла спрайта башни.
     * @param side_ Размер стороны квадрата спрайта башни.
     * @param clock_ Указатель на таймер внутри основного игрового цикла.
     * @param scale_x_ Коэффициент масштабирования по X.
     * @param scale_y_ Коэффициент масштабирования по Y.
     */
    explicit LightTower(const sf::Vector2i & position, int side_
    , sf::Clock* clock_, double scale_x_, double scale_y_);
private:
};

///@brief Класс второго вида башен (тяжелая башня).
class HeavyTower : public BaseTower{
public:
    HeavyTower() = delete;
    ~HeavyTower() = default;
    HeavyTower(const HeavyTower&) = default;
    HeavyTower& operator=(const HeavyTower&) = default;
    /**
     *
     * @param position Координаты верхнего левого угла спрайта башни.
     * @param side_ Размер стороны квадрата спрайта башни.
     * @param clock_ Указатель на таймер внутри основного игрового цикла.
     * @param scale_x_ Коэффициент масштабирования по X.
     * @param scale_y_ Коэффициент масштабирования по Y.
     */
    explicit HeavyTower(const sf::Vector2i & position, int side_
    , sf::Clock* clock_, double scale_x_, double scale_y_);
};



#endif //SFML_SANDBOX_BASETOWER_H
