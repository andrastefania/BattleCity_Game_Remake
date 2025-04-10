export module Tank;
import <iostream>;

export class Tank {
public:
    enum class ColorTank : uint8_t {
        Red,
        Blue,
        Green,
        Yellow
    };

    static ColorTank GetNextColor() {
        static int colorIndex = 0;
        ColorTank color = static_cast<ColorTank>(colorIndex);
        colorIndex = (colorIndex + 1) % 4;
        return color;
    }

    Tank();
    ColorTank GetColor() const;

    Tank(Tank&&) noexcept = default;
    Tank& operator=(Tank&&) noexcept = default;
    Tank(const Tank&) = delete;
    Tank& operator=(const Tank&) = delete;

private:
    ColorTank m_color : 2; 
};

Tank::Tank() : m_color(GetNextColor()) {}

Tank::ColorTank Tank::GetColor() const {
    return m_color;
}
