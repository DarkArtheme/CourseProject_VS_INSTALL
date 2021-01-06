#ifndef BASEENEMY_H
#define BASEENEMY_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "LevelMaker.h"
#include "GridMaker.h"
#include <cmath>

///@brief Направление движения врага.
enum Direction {
    ///@brief Вверх.
	UP,
    ///@brief Влево.
	LEFT,
    ///@brief Вниз.
	DOWN,
    ///@brief Вправо.
	RIGHT,
};

///@brief Класс, подгружающий все текстуры для врагов.
class EnemyTextureLoader{
public:
    ///@brief Подгружает все поля класса.
    static void LoadTextures(const std::string& path);
    static std::vector<sf::Texture> textures_common_enemy;
    static std::vector<sf::Texture> textures_heavy_enemy;
};

///@brief Абстрактный класс, задающий общий интерфейс врагов.
class BaseEnemy{
public:
	BaseEnemy() = default;
	BaseEnemy(const BaseEnemy&) = default;
	~BaseEnemy() = default;
	BaseEnemy& operator=(const BaseEnemy&) = default;
	/**
	 * @return Скорость врага.
	 */
	float getSpeed() const { return speed; }
	/**
	 * @brief Устанавливает скорость врага.
	 * @param value Значение скорости.
	 */
	void setSpeed(float value) { speed = value; }
    /**
     * @return Здоровье врага.
     */
	int getHealth() const { return health; }
    /**
     * @brief Устанавливает здоровье врага.
     * @param value Значение здоровья.
     */
	void setHealth(int value) { health = value; }
    /**
     * @return Количество очков за убийство врага.
     */
	int getPoints() const { return points_for_kill; }
	/**
	 * @brief Осуществляет движение врага.
	 * @param clock Ссылка на таймер в основном цикле игры.
	 */
	void move(sf::Clock& clock);
    /**
     * @return Логическое значение, означающее, должен ли враг быть удален.
     */
	bool shouldBeDeleted() const { return shoud_be_deleted; }
    /**
     * @return Координаты центра спрайта врага.
     */
	sf::Vector2f getCenterPosition() const { return sprite.getPosition(); }
    /**
     * @return Прямоугольник, в который вписан спрайт врага.
     */
    sf::FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); }
    /**
     * @return Текущее направление движения врага.
     */
    Direction getDirection() const { return dir; }
    /**
     * @brief Отрисовывает врага.
     * @param window Окно игры.
     */
	void draw(sf::RenderTarget& window);

protected:
    //sf::Music sound_step;
    std::vector<sf::Texture>* textures;
	sf::Sprite sprite;
	int healthbar_width{0};
	int healthbar_height{0};
	sf::RectangleShape healthbar_border;
	sf::RectangleShape healthbar_green;
	sf::RectangleShape healthbar_red;
	Direction dir{Direction::DOWN};      // текущее направление
    double scale_x, scale_y;
	float speed{0.1};
	int health{0};
	int sprite_side;
	int points_for_kill{0};
    int max_health{0};
    int outline_thickness{0};
	std::size_t turn_num{0};             // номер текущего поворота
    std::vector<TmxObject> path;  // масссив со всеми поворотами
    bool shoud_be_deleted{false};        // должен ли экземпляр быть утилизирован в текущий момент времени
    template<class T>
    sf::Vector2f getCenter(const sf::Rect<T>& r); // получить координаты центра прямоугольника
    Direction setupDir(const std::string& str);  //вспомогательная функция для конструктора
    void changeTexture();                // меняет текстуру в соответствии с направлением
    template<class Vector>
    bool checkEqual(const Vector& lhs, const Vector& rhs); // грамотное сравнение векторов (дробных)
};

///@brief Класс первого вида врагов (легкий враг).
class LightEnemy : public BaseEnemy{
public:
    LightEnemy() = default;
    LightEnemy(const LightEnemy&) = default;
    LightEnemy& operator=(const LightEnemy&) = default;
    ~LightEnemy() = default;
    /**
     *
     * @param lvl Ссылка на отрисовщик уровня.
     * @param clk Ссылка на таймер в основном цикле игры.
     * @param grd Ссылка на отрисовщик сетки.
     * @param side Размер стороны квадрата спрайта врага.
     * @param scale_x_ Коэффициент масштабирования по X.
     * @param scale_y_ Коэффициент масштабирования по Y.
     */
    explicit LightEnemy(const LevelMaker& lvl, sf::Clock& clk, const GridMaker& grd, const int side, const double scale_x_, const double scale_y_);
private:
};

///@brief Класс второго вида врагов (тяжелый враг).
class HeavyEnemy : public BaseEnemy{
public:
    HeavyEnemy() = default;
    HeavyEnemy(const HeavyEnemy&) = default;
    HeavyEnemy& operator=(const HeavyEnemy&) = default;
    ~HeavyEnemy() = default;
    /**
     *
     * @param lvl Ссылка на отрисовщик уровня.
     * @param clk Ссылка на таймер в основном цикле игры.
     * @param grd Ссылка на отрисовщик сетки.
     * @param side Размер стороны квадрата спрайта врага.
     * @param scale_x_ Коэффициент масштабирования по X.
     * @param scale_y_ Коэффициент масштабирования по Y.
     */
    explicit HeavyEnemy(const LevelMaker& lvl, sf::Clock& clk, const GridMaker& grd, const int side, const double scale_x_, const double scale_y_);
private:

};



#endif

