
namespace Math {

    struct Vec3 {
        double x{0.0};
        double y{0.0};
        double z{0.0};

        constexpr void operator+(const Vec3& other);
    };
}