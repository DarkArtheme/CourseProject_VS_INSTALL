#ifndef TMXLEVEL_H 
#define TMXLEVEL_H 

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <TinyXML2/tinyxml2.h>

/**
 * @brief Структура, хранящая TMX-объект.
 * @details В картах TMX объект - это область на карте, имеющая имя, тип,
 * границы, набор пользовательских свойств (в формате ключ-значение)
 * и текстурные координаты.
 * Текстурные координаты позволяют связать с объектом спрайт,
 * использующий основную текстуру карты как источник данных.
 */
struct TmxObject
{
    int GetPropertyInt(const std::string &propertyName);
    float GetPropertyFloat(const std::string &propertyName);
    std::string GetPropertyString(const std::string &propertyName);

    void MoveBy(const sf::Vector2f &movement);
    void MoveTo(const sf::Vector2f &position);

    std::string name;
    std::string type;
    sf::FloatRect rect;                                   //was changed from sf::FloatRect
    std::map<std::string, std::string> properties;

    sf::Sprite sprite;
};

/**
 * @brief Структура, хранящая TMX-слой.
 * @details В картах TMX слой - это набор тайлов (спрайтов),
 * из которых складывается ландшафт карты.
 * Слоёв может быть несколько, что позволяет нарисовать,
 * например, слой травы поверх слоя земли.
 */

struct TmxLayer
{
    sf::Uint8 opacity = 0;
    std::vector<sf::Sprite> tiles;
};

///@brief Класс, который парсит файл уровня
class TmxLevel
{
  public:
    /**
     * @brief Загружает данные из TMX-файла в память объекта.
     * @param filepath Путь до файла.
     * @return Логическое значение, означающее, получилось ли загрузить файл.
     */
    bool LoadFromFile(const std::string &filepath);
    /**
     * @param name Имя объекта.
     * @return Первый объект с именем name.
     */
    TmxObject GetFirstObject(const std::string &name) const;
    /**
     * @param type Тип объекта.
     * @return Все объекты заданного типа.
     */
    std::vector<TmxObject> GetAllObjects(const std::string &type) const;
    /**
     * @return Размер тайла.
     */
    sf::Vector2i GetTileSize() const;
    /**
     * @return Ширина уровня.
     */
    float GetTilemapWidth() const;
    /**
     * @return Высота уровня.
     */
    float GetTilemapHeight() const;
    /**
     * @return Ширина и высота уровня.
     */
    sf::Vector2f GetTilemapSize() const;
    /**
     * @brief Рисует все слои тайлов один за другим, но не рисует объекты (рисованием которых должна заниматься игра).
     * @param target Окно для отрисовки.
     * @param scale_x Коэффициент масштабирования по X.
     * @param scale_y Коэффициент масштабирования по Y.
     */
    void Draw(sf::RenderTarget &target, const double scale_x, const double scale_y);

  private:
    int m_width = 0;
    int m_height = 0;
    int m_tileWidth = 0;
    int m_tileHeight = 0;
    int m_firstTileID = 0;
    int m_space = 0;
    sf::Texture m_tilesetImage;
    std::vector<TmxObject> m_objects;
    std::vector<TmxLayer> m_layers;
};

#endif 

