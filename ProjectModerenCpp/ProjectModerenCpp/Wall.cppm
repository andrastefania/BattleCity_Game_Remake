export module Wall;

export class Wall {
public:
    enum class TypeWall {
        destructible,
        indestructible,
        none
    };
    Wall(TypeWall typeWall);
    TypeWall IsDestructible() const;

private:
    TypeWall m_typeWall;
};

Wall::Wall(TypeWall typeWall)
    : m_typeWall{ typeWall } {
}
Wall::TypeWall Wall::IsDestructible() const {
    return m_typeWall;
}