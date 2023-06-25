#include "espresso_based.h"
#include "ingredient.h"
EspressoBased::EspressoBased() {}
EspressoBased::EspressoBased(const EspressoBased &esp) {
    name = esp.name;
    for (const auto &ptr : esp.ingredients) {
        ingredients.push_back(ptr->clone());
    }
}
void EspressoBased::operator=(const EspressoBased &esp) {
    if (this == &esp) {
        return;
    }
    name = esp.name;
    // delete current ingredient's memory
    for (const auto &i : ingredients) delete i;
    ingredients.clear();
    for (const auto &ptr : esp.ingredients) {
        ingredients.push_back(ptr->clone());
    }
}
EspressoBased::~EspressoBased() {
    for (const auto &i : ingredients) delete i;
    ingredients.clear();
}
std::vector<Ingredient *> &EspressoBased::get_ingredients() { return ingredients; }