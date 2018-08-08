#include "clsParagraph.h"

namespace Targoman {
namespace PDF {

clsTextBox::clsTextBox(float _x, float _y, float _width, float _height)
{
    this->X = _x;
    this->Y = _y;
    this->Width = _width;
    this->Height = _height;
}

float clsTextBox::HorizontalOverlap(const clsTextBox &_other) const
{
    float x0 = std::max(this->getLeft(), _other.getLeft());
    float x1 = std::min(this->getRight(), _other.getRight());
    return (x1 - x0) / std::min(this->getWidth(), _other.getWidth());
}

float clsTextBox::verticallOverlap(const clsTextBox &_other) const
{
    float y0 = std::max(this->getTop(), _other.getTop());
    float y1 = std::min(this->getBottom(), _other.getBottom());
    return (y1 - y0) / std::min(this->getHeight(), _other.getHeight());
}

#define SAME_BOX_EPSILON    1e-4

float abs(float x)
{
    if(x < 0)
        return -x;
    return x;
}

bool clsTextBox::areSameBoxes(const clsTextBox &_other) const
{
    return
            abs(this->getX() - _other.getX()) < SAME_BOX_EPSILON &&
            abs(this->getY() - _other.getY()) < SAME_BOX_EPSILON &&
            abs(this->getWidth() - _other.getWidth()) < SAME_BOX_EPSILON &&
            abs(this->getHeight() - _other.getHeight()) < SAME_BOX_EPSILON;
}

clsTextBox &clsTextBox::unionBy(const clsTextBox &_other)
{
    if(this->Width == 0 || this->Height == 0)
    {
        this->setX(_other.getX());
        this->setY(_other.getY());
        this->setWidth(_other.getWidth());
        this->setHeight(_other.getHeight());
        return* this;
    }

    int x0 = std::min(this->getLeft(), _other.getLeft());
    int x1 = std::max(this->getRight(), _other.getRight());
    int y0 = std::min(this->getTop(), _other.getTop());
    int y1 = std::max(this->getBottom(), _other.getBottom());

    this->setX(x0);
    this->setY(y0);
    this->setWidth(x1 - x0);
    this->setHeight(y1 - y0);

    return *this;
}

clsTextBox &clsTextBox::intersectBy(const clsTextBox &_other)
{
    int x0 = std::max(this->getLeft(), _other.getLeft());
    int x1 = std::min(this->getRight(), _other.getRight());
    int y0 = std::max(this->getTop(), _other.getTop());
    int y1 = std::min(this->getBottom(), _other.getBottom());

    if(x1 < x0 || y1 < y0) {
        this->setWidth(0);
        this->setHeight(0);
    } else {

        this->setX(x0);
        this->setY(y0);
        this->setWidth(x1 - x0);
        this->setHeight(y1 - y0);

    }

    return *this;
}

}
}
