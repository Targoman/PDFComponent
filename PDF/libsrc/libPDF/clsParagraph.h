#ifndef TARGOMAN_PDF_CLSPARAGRAPH_H
#define TARGOMAN_PDF_CLSPARAGRAPH_H

#include <vector>
#include <string>

namespace Targoman {
namespace PDF {

class clsTextBox {
private:
    float X, Y, Width, Height;

public:
    clsTextBox() : X(0), Y(0), Width(0), Height(0) { }
    clsTextBox(float _x, float _y, float _width, float _height);

    virtual float getX() const { return this->X; }
    virtual float getY() const { return this->Y; }
    virtual float getWidth() const { return this->Width; }
    virtual float getHeight() const { return this->Height; }

    virtual void setX(float _value) { this->X = _value; }
    virtual void setY(float _value) { this->Y = _value; }
    virtual void setWidth(float _value) { this->Width = _value; }
    virtual void setHeight(float _value) { this->Height = _value; }

    inline float getLeft() const { return this->getX(); }
    inline float getTop() const { return this->getY(); }
    inline float getRight() const { return this->getX() + this->getWidth(); }
    inline float getBottom() const { return this->getY() + this->getHeight(); }

    inline bool isEmpty() const { return this->Width >=0 && this->Height >= 0; }


    float HorizontalOverlap(const clsTextBox& _other) const;
    float verticallOverlap(const clsTextBox& _other) const;

    bool areSameBoxes(const clsTextBox& _other) const;

    clsTextBox& unionBy(const clsTextBox& _other);
    clsTextBox& intersectBy(const clsTextBox& _other);
};

namespace enuParagraphType {
    enum Type {
        Body,
        Header,
        Footer,
        Footnote
    };
}

class clsLine : public clsTextBox {
private:
    std::wstring Contents;

public:
    clsLine() : clsTextBox(), Contents(L"") { }
    clsLine(float _x, float _y, float _width, float _height, std::wstring _contents) :
        clsTextBox(_x, _y, _width, _height),
        Contents(_contents)
    { }

    void clear()
    {
        this->setX(0);
        this->setY(0);
        this->setWidth(0);
        this->setHeight(0);
        this->Contents.clear();
    }

    void setContents(std::wstring _contents) { this->Contents = _contents; }
    const std::wstring& getContents() const { return this->Contents; }
    std::wstring& getContents() { return this->Contents; }
};

typedef std::vector<clsLine> Lines_t;
typedef std::vector<clsLine> Sentence_t;
typedef std::vector<Sentence_t> Sentences_t;

class clsParagraph : public clsTextBox {
private:
    Sentences_t Contents;
    std::wstring ContentString;
    enuParagraphType::Type Type;
    int ReadingOrder;

public:
    clsParagraph() : Type(enuParagraphType::Body) { }
    clsParagraph(const Sentences_t& _sentences, const enuParagraphType::Type& _type, const int _readingOrder) :
        Contents(_sentences),
        Type(_type),
        ReadingOrder(_readingOrder)
    { }

    void addSentence(Sentence_t _sentence)
    {
        this->Contents.push_back(_sentence);
        for(const auto& l : _sentence) {
            this->unionBy(l);
            this->ContentString.append(l.getContents());
            this->ContentString.append(L" ");
        }
        this->ContentString = this->ContentString.substr(0, this->ContentString.size() - 1);
    }

    const Sentences_t& getContents() const { return this->Contents; }
    const std::wstring getContentString() const { return this->ContentString; }

    enuParagraphType::Type getType() const { return this->Type; }
    void setType(enuParagraphType::Type _value) { this->Type = _value; }
};

typedef std::vector<clsParagraph> Paragraphs_t;

}
}

#endif // TARGOMAN_PDF_CLSPARAGRAPH_H
