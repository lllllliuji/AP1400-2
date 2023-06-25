#include "mocha.h"
#include "espresso_based.h"
#include "ingredient.h"
#include "sub_ingredients.h"
Mocha::Mocha() {
    name = "Mocha";
    Ingredient *ingredient_a = new Espresso(2);
    Ingredient *ingredient_b = new Milk(2);
    Ingredient *ingredient_c = new MilkFoam(1);
    Ingredient *ingredient_d = new Chocolate(1);
    ingredients.push_back(ingredient_a);
    ingredients.push_back(ingredient_b);
    ingredients.push_back(ingredient_c);
    ingredients.push_back(ingredient_d);
}

Mocha::Mocha(const Mocha &cap) : EspressoBased(cap) {
    for (const auto &ptr : cap.side_items) {
        side_items.push_back(ptr->clone());
    }
}

Mocha::~Mocha() {
    for (const auto &i : side_items) delete i;
    side_items.clear();
}

void Mocha::operator=(const Mocha &cap) {
    if (this == &cap) {
        return;
    }
    // delete side_items' ingredient memory
    for (const auto &i : side_items) delete i;
    side_items.clear();
    EspressoBased::operator=(cap);
    for (const auto &ptr : cap.side_items) {
        side_items.push_back(ptr->clone());
    }
}

std::string Mocha::get_name() { return name; }
double Mocha::price() {
    double total = 0.0;
    for (auto ingredient_ptr : side_items) {
        total += ingredient_ptr->price();
    }
    for (auto ingredient_ptr : ingredients) {
        total += ingredient_ptr->price();
    }
    return total;
}

void Mocha::add_side_item(Ingredient *side) { side_items.push_back(side->clone()); }

std::vector<Ingredient *> &Mocha::get_side_items() { return side_items; }