#ifndef SUB_INGREDIENTS_H
#define SUB_INGREDIENTS_H
#include "ingredient.h"

#define SubIngredient(className, pricePerUnit)                                                 \
    class className : public Ingredient {                                                      \
       public:                                                                                 \
        className(size_t units) : Ingredient(pricePerUnit, units) { this->name = #className; } \
        virtual std::string get_name() override { return this->name; }                         \
        virtual ~className() override {}                                                       \
        virtual className *clone() override { return new className(units); }                   \
                                                                                               \
       private:                                                                                \
        std::string name;                                                                      \
    }
SubIngredient(Cinnamon, 5);
SubIngredient(Chocolate, 5);
SubIngredient(Sugar, 1);
SubIngredient(Cookie, 10);
SubIngredient(Espresso, 15);
SubIngredient(Milk, 10);
SubIngredient(MilkFoam, 5);
SubIngredient(Water, 1);
#endif  // SUB_INGREDIENTS_H