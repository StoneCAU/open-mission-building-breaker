#pragma once
#include <string>

class UIRenderer {
public:
    void renderMenu(int highScore) const;

private:
    void printBorder() const;
};
