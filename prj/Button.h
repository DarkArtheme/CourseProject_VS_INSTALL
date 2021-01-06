#pragma once
#ifndef SFML_SANDBOX_BUTTON_H
#define SFML_SANDBOX_BUTTON_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

///@brief Состояние кнопки.
enum Status{
    ///@brief Обычное состояние.
    DEACTIVATED,
    ///@brief Наведен курсор.
    TARGETED,
    ///@brief Произведено нажатие.
    CLICKED
};

///@brief Класс, подгружающий звуки кнопок.
class ButtonSoundLoader{
public:
    ///@brief Загрузка полей класса.
    static void LoadSounds(const std::string &folder_path);
    inline static sf::SoundBuffer targeted;
    inline static sf::SoundBuffer clicked;
};

///@brief Абстрактный класс, задающий общий интерфейс кнопок.
class Button{
public:
    Button() = default;
    Button(const Button&) = default;
    Button& operator=(const Button&) = default;
    ~Button() = default;
    /**
     * @return Логическое значение, означающее, видна ли кнопка
     */
    virtual bool isVisible() const;
    /**
     * @param value Значение, изменяющее видимость кнопки (true - видима).
     */
    virtual void setVisible(const bool value);
    /**
     * @return Логическое значение, означающее, установлено ли состояние перманентного нажатия.
     */
    virtual bool isPermanentlyClicked() const { return permanently_clicked; }
    /**
     * @param value Значение, изменяющее состояние перманентного нажатия (true - перманентно нажата).
     */
    virtual void setPermanentlyClicked(const bool value) { permanently_clicked = value; }
    /**
     * @brief Проверка, попадает ли курсор в границы кнопки.
     * @param curs_pos Позиция курсора.
     * @return Логическое значение, означающее, попадает ли курсор в границы кнопки.
     */
    virtual bool checkCursor(const sf::Vector2i& curs_pos) = 0;
    /**
     * @brief Проверка, произошло ли нажатие кнопки.
     * @param curs_pos Позиция курсора.
     * @return Логическое значение, означающее, произошло ли нажатие кнопки.
     */
    virtual bool isActivated(const sf::Vector2i& curs_pos) = 0;
    /**
     * @brief Отрисовывает кнопку.
     * @param window Окно игры.
     */
    virtual void draw(sf::RenderTarget& window) = 0;
protected:
    sf::Vector2f pos;
    sf::Sound sound_targeted;
    sf::Sound sound_clicked;
    float width_{0}, height_{0};
    bool is_visible{true};
    bool permanently_clicked{false};
    Status status;
};

///@brief Класс кнопки, основанной на наборе спрайтов.
class ButtonSpr : public Button {
public:
    ButtonSpr() = delete;
    ButtonSpr(const ButtonSpr&) = default;
    ButtonSpr& operator=(const ButtonSpr&) = default;
    ~ButtonSpr() = default;
    /**
     * @brief Конструктор.
     * @param deactivated_texture Текстура для обычного состояния кнопки.
     * @param targeted_texture Текстура для кнопки, на которую наведен курсор.
     * @param clicked_texture Текстура для нажатой кнопки.
     * @param position Координаты верхнего левого угла кнопки на экране.
     * @param width Ширина кнопки.
     * @param height Высота кнопки.
     */
    explicit ButtonSpr(const sf::Texture& deactivated_texture
    , const sf::Texture& targeted_texture, const sf::Texture& clicked_texture
    , sf::Vector2f position, const float width, const float height);

    bool checkCursor(const sf::Vector2i& curs_pos) override;
    bool isActivated(const sf::Vector2i& curs_pos) override;
    void draw(sf::RenderTarget& window) override;
private:
    const sf::Texture& texture_deactivated;
    const sf::Texture& texture_targeted;
    const sf::Texture& texture_clicked;
    sf::Sprite sprite;
};

///@brief Класс кнопки, основанной на цветном прямоугольнике.
class ButtonRec : public Button {
public:
    ButtonRec() = delete;
    ButtonRec(const ButtonRec&) = default;
    ButtonRec& operator=(const ButtonRec&) = default;
    ~ButtonRec() = default;
    /**
     * @brief Конструктор.
     * @param place Скелет кнопки (Прямоугольник, размеры которого совпадают с размерами кнопки).
     * @param color_deactivated Цвет для обычного состояния кнопки.
     * @param color_targeted Цвет для кнопки, на которую наведен курсор.
     * @param color_clicked Цвет для нажатой кнопки.
     * @param color_text Цвет текста на кнопке.
     * @param font Шрифт текста на кнопке.
     * @param size Размер текста на кнопке.
     * @param str Строка в тексте на кнопке.
     * @param thickness_shadow Толщина контура кнопки.
     */
    explicit ButtonRec(const sf::FloatRect& place, const sf::Color& color_deactivated
    , const sf::Color& color_targeted, const sf::Color& color_clicked
    , const sf::Color& color_text, const sf::Font& font
    , const unsigned int size, const std::string& str, float thickness_shadow);
    bool checkCursor(const sf::Vector2i& curs_pos) override;
    bool isActivated(const sf::Vector2i& curs_pos) override;
    void draw(sf::RenderTarget& window) override;
private:
    sf::FloatRect rect;
    sf::RectangleShape rectshape;
    sf::Text text;
    sf::Color deactivated_color;
    sf::Color targeted_color;
    sf::Color clicked_color;
    sf::Color text_color;
    float shadow_thickness{0};
};


#endif //SFML_SANDBOX_BUTTON_H
