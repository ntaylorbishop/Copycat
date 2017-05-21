#pragma once

//What if object?
//What if pointer to object?
//What if const pointer to object?

const size_t 

template <typename T>
class BeirusHashTable {
public:
	//STRUCTORS
	BeirusHashTable();
	~BeirusHashTable();

	void Insert();
	void Find();
	void Remove();


private:
	T m_arr[];
};