// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp = 0) : MaxPower(mp), BitField(mp)
{
}

// конструктор копирования
TSet::TSet(const TSet &s) : MaxPower(s.MaxPower), BitField(s.BitField)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : MaxPower(bf.GetLength()), BitField(bf)
{
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    MaxPower = s.MaxPower;
    BitField = s.BitField;

    return *this;
}

bool TSet::operator==(const TSet &s) const // сравнение
{
    return MaxPower == s.MaxPower && BitField == s.BitField;
}

bool TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TSet res = *this;
    res.MaxPower = std::max(this->MaxPower, s.MaxPower);
    res.BitField = res.BitField | s.BitField;

    return res;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet res = *this;
    res.BitField.SetBit(Elem);

    return res;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet res = *this;
    res.BitField.ClrBit(Elem);

    return res;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TSet res = *this;
    res.MaxPower = std::max(this->MaxPower, s.MaxPower);
    res.BitField = res.BitField & s.BitField;

    return res;
}

TSet TSet::operator~(void) // дополнение
{
    TSet res = *this;
    res.BitField = ~res.BitField;

    return res;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    uint64_t elem;
    while (istr >> elem) {
        if (elem >= s.MaxPower) throw runtime_error("TSet operator>>: can't element bigger or equal to MaxPower");
        s.InsElem(elem);
    }

    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    for (size_t i = 0; i < s.MaxPower; ++i)
        if (s.IsMember(i)) {
            ostr << i;
            if (i+1 < s.MaxPower) ostr << " ";
        }

    return ostr;
}
