// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0) throw "incorrect operation";
	BitLen = len;
	MemLen = (BitLen + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) : BitLen(bf.BitLen), MemLen(bf.MemLen) // конструктор копирования
{
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0 || n >= BitLen) throw "incorrect operation";
	return MemLen - n / (sizeof(TELEM) * 8) - 1;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0 || n >= BitLen) throw "incorrect operation";
	return n / (sizeof(TELEM) * 8);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n >= BitLen) throw "incorrect operation";
	pMem[GetMemIndex(n)] |= (1 << n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n >= BitLen) throw "incorrect operation";
	pMem[GetMemIndex(n)] &= ~(1 << n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n >= BitLen) throw "incorrect operation";
	return (1 << n) & pMem[GetMemIndex(n)];
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	TELEM* mas1 = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		mas1[i] = bf.pMem[i];
	}
	delete[] pMem;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = mas1[i];
	}
	delete[] mas1;
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (BitLen == bf.BitLen) {
		for (int i = 0; i < MemLen; i++) {
			if (pMem[i] != bf.pMem[i]) return 0;
		}
		return 1;
	}
	return 0;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	if (*this == bf) return 0;
	return 1;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	int min = bf.BitLen;
	TBitField res(*this);
	if (BitLen < bf.BitLen) {
		min = BitLen;
		res = bf;
	}
	for (int i = 0; i < min; i++) {
		if (this->GetBit(i) || bf.GetBit(i)) res.SetBit(i);
		else res.ClrBit(i);
	}
	return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	int min = bf.BitLen;
	TBitField res(*this);
	if (BitLen < bf.BitLen) {
		min = BitLen;
		res = bf;
	}
	for (int i = 0; i < min; i++) {
		if (this->GetBit(i) && bf.GetBit(i)) res.SetBit(i);
		else res.ClrBit(i);
	}
	for (int i = min; i < res.BitLen; i++) {
		res.ClrBit(i);
	}
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);
	for (int i = 0; i < BitLen; i++) {
		GetBit(i) == 0 ? res.SetBit(i) : res.ClrBit(i);
	}
	return res;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	char bit;
	for (int i = 0; i < bf.BitLen; i++) {
		istr >> bit;
		if (bit == '1') {
			bf.SetBit(i);
		}
		else if (bit == '0') {
			bf.ClrBit(i);
		}
		else {
			throw "Incorrect operation";
		}
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++) {
		ostr << (bool)bf.GetBit(i);
	}
	return ostr;
}
