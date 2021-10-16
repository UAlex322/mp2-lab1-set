// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <string>
#include <algorithm>

#define MEM_SIZE(BitLen) ((BitLen)+8*sizeof(TELEM)-1)/(8*sizeof(TELEM))

TBitField::TBitField(int len)
{
	if (len < 0) 
		throw runtime_error("TBitField constructor: can't create bitset with negative length");

	BitLen = len;
	MemLen = MEM_SIZE(BitLen);
	pMem = new TELEM[MemLen]{};
} 

TBitField::TBitField(const TBitField &bf): BitLen(bf.BitLen), MemLen(bf.MemLen) // конструктор копирования
{
	pMem = new TELEM[MemLen];
	for (size_t i = 0; i < MemLen; ++i)
		pMem[i] = bf.pMem[i];
}

TBitField::TBitField(const std::string &s): BitLen(s.size()), MemLen(MEM_SIZE(BitLen)) { // Конструктор от бинарной строки (из нулей и единиц)
	pMem = new TELEM[MemLen];

	TELEM buffer;
	for (size_t i = 0; i < MemLen; ++i) {
		buffer = 0;
		for (size_t j = i*8*sizeof(TELEM); j < (i+1)*8*sizeof(TELEM) && j < BitLen; ++j) {
			if (s[j] != '0' && s[j] != '1') 
				throw runtime_error("Bitfield constructor from string: string has an invalid character");
			else
				buffer ^= (1 << j % (8*sizeof(TELEM)));
		}
		pMem[i] = buffer;
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n/(8*sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return static_cast<TELEM>(1) << (n % (8*sizeof(TELEM)));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0) 
		throw runtime_error("SetBit: negative index is forbidden");
	if (n >= BitLen) 
		throw runtime_error("SetBit: too large index is forbidden");

	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0) 
		throw runtime_error("ClrBit: negative index is forbidden");
	if (n >= BitLen) 
		throw runtime_error("ClrBit: too large index is forbidden");

	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0) 
		throw runtime_error("GetBit: negative index is forbidden");
	if (n >= BitLen) 
		throw runtime_error("GetBit: too large index is forbidden");

	return (pMem[GetMemIndex(n)] & GetMemMask(n)) >> (n % (8*sizeof(TELEM)));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf) return *this;

	if (MemLen != bf.MemLen) {
		delete[] pMem;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
	}
	BitLen = bf.BitLen;
	for (size_t i = 0; i < MemLen; ++i)
		pMem[i] = bf.pMem[i];

	return *this;
}

bool TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) return false;
	for (size_t i = 0; i < MemLen; ++i)
		if (pMem[i] != bf.pMem[i]) return false;
	return true;
}

bool TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField res(std::max(this->BitLen, bf.BitLen));
	for (size_t i = 0; i < this->MemLen; ++i)
		res.pMem[i] = this->pMem[i];

	for (size_t i = 0; i < res.MemLen; ++i)
		res.pMem[i] |= bf.pMem[i];

	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField res(std::max(this->BitLen, bf.BitLen));
	for (size_t i = 0; i < this->MemLen; ++i)
		res.pMem[i] = this->pMem[i];

	for (size_t i = 0; i < res.MemLen; ++i)
		res.pMem[i] &= bf.pMem[i];

	return res;
}

TBitField TBitField::operator~() // отрицание
{
	TBitField res = *this;
	for (size_t i = 0; i < res.MemLen-1; ++i)
		res.pMem[i] = ~pMem[i];
	res.pMem[MemLen-1] = ~pMem[MemLen-1] & ((1 << BitLen % (8*sizeof(TELEM))) - 1);

	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	std::string input;
	istr >> input;

	bf.BitLen = input.size();
	bf.MemLen = MEM_SIZE(bf.BitLen);
	bf.pMem = new TELEM[bf.MemLen];
	
	for (size_t i = 0; i < bf.MemLen; ++i) {
		string sbuf = input.substr(i*8*sizeof(TELEM), 8*sizeof(TELEM));
		TELEM buffer = 0;
		for (int j = sbuf.size()-1; j >= 0; --j) {
			buffer <<= 1;
			buffer ^= sbuf[j] - '0';
		}
		bf.pMem[i] = buffer;
	}

	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	std::string result(bf.BitLen, ' ');
	TELEM buffer;
	size_t pos = 0;

	for (size_t i = 0; i < bf.MemLen; ++i) {
		buffer = bf.pMem[i];
		for (size_t j = 0; j < 8*sizeof(TELEM) && pos < bf.BitLen; ++j) {
			result[pos++] = (buffer & 1) + '0';
			buffer >>= 1;
		}
	}

	ostr << result;
	return ostr;
}

