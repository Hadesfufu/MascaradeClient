#ifndef REVERSABLE_HASH_TABLE_H_INCLUDED
#define REVERSABLE_HASH_TABLE_H_INCLUDED

#define IS_BIT_TRUE(i, n)	(GET_BIT(i, n) == (1 << (n)))
#define GET_BIT(i,n)		((i) & 1 << (n))
#define SET_BIT_TRUE(i,n)	((i) |= (1 << (n)))
#define SET_BIT_FALSE(i,n)	((i) &= ~(1 << (n)))

#include <map>
#include <list>

template <class Colonne, class Ligne>
class ReversableHashTable
{
public:
	ReversableHashTable() :
		m_datas(nullptr),
		m_alocColonne(0),
		m_alocLigne(0),
		m_length(0)
	{

	}
	~ReversableHashTable()
	{
		if (m_datas)
			free(m_datas);
	}

	bool							link(Colonne col, Ligne lign){
		size_t colNumber = addColumn(col);
		size_t ligneNumber = addLigne(lign);
#if _DEBUG
		GfxDbgAssert(colNumber < m_alocColonne);
		GfxDbgAssert(ligneNumber < m_alocLigne);
#endif

		valideBit(ligneNumber, colNumber);
#if defined(_DEBUG) && defined(VERBOSE)
		printfDbg();
#endif
		return true;
	}
	bool							unlink(Colonne col, Ligne lign){
		auto indexCol = m_indexColonne.find(col);
		auto indexLigne = m_indexLigne.find(lign);

		//	both keys are already register
		if (indexCol != m_indexColonne.end() &&
			indexLigne != m_indexLigne.end()){
			size_t ligneNumber = indexLigne->second;
			size_t colNumber = indexCol->second;
#if _DEBUG
			GfxDbgAssert(colNumber < m_alocColonne);
			GfxDbgAssert(ligneNumber < m_alocLigne);
#endif

			invalidBit(ligneNumber, colNumber);

#if defined(_DEBUG) && defined(VERBOSE) 
			printfDbg();
#endif

			std::list<Ligne> bind1 = keyForFirstKey(col);
			if (bind1.empty())
				removeColumn(colNumber);

			std::list<Colonne> bind2 = keyForSecondKey(lign);
			if (bind2.empty())
				removeLigne(ligneNumber);

#if defined(_DEBUG) && defined(VERBOSE)
			printfDbg();
#endif

			return true;
		}

		return false;
	}

	const std::list<Ligne>			keyForFirstKey(Colonne col)const {
		std::list<Ligne> results = std::list<Ligne>();

		auto itCol = m_indexColonne.find(col);
		if (itCol == m_indexColonne.end())
			return results;
		size_t colNumber = itCol->second;

		for (auto it = m_indexLigne.begin(); it != m_indexLigne.end(); it++){
			size_t index = colNumber + (m_alocColonne * it->second);
			size_t unsafeIndex = index / 8;
			char unsafeData = m_datas[unsafeIndex];
			bool isLinked = IS_BIT_TRUE(unsafeData, 7 - (index % 8));
			if (isLinked)
				results.push_front(it->first);
		}

		return results;
	}
	const std::list<Colonne>		keyForSecondKey(Ligne ligne) const{
		std::list<Colonne> results = std::list<Colonne>();

		auto itLigne = m_indexLigne.find(ligne);
		if (itLigne == m_indexLigne.end())
			return results;
		size_t ligneNumber = itLigne->second;

		for (auto it = m_indexColonne.begin(); it != m_indexColonne.end(); it++){
			size_t index = it->second + (m_alocColonne * ligneNumber);
			size_t unsafeIndex = index / 8;
			char unsafeData = m_datas[unsafeIndex];
			bool isLinked = IS_BIT_TRUE(unsafeData, 7 - (index % 8));
			if (isLinked)
				results.push_front(it->first);
		}

		return results;
	}

	void							reset(){
		m_indexLigne.clear();
		m_indexColonne.clear();

		m_alocLigne = 0;
		m_alocColonne = 0;
		m_length = 0;

		if (m_datas)
			free(m_datas);
		m_datas = nullptr;
	}

#if VERBOSE
	void                            printfDbg(){
		GfxDbgPrintf("-------\n");

		size_t parcours = 0;
		size_t lignes = 0;
		for (size_t size = 0; size < m_length; size++){
			for (int i = 0; i < 8; i++){
				if (lignes == m_alocLigne)
					continue;

				if (GET_BIT(m_datas[size], 7 - i) > 0)
					GfxDbgPrintf("%u ", 1);
				else
					GfxDbgPrintf("%u ", 0);

				parcours++;
				if (parcours == m_alocColonne){
					lignes++;
					parcours = 0;
					GfxDbgPrintf("\n");
				}
			}
		}

		GfxDbgPrintf("-------\n");
	}
#endif

private:
	size_t							addColumn(Colonne col){
		auto index = m_indexColonne.find(col);
		if (index != m_indexColonne.end())
			return index->second;

		size_t unusedAllocatedBits = 8 * m_length - m_alocColonne * m_alocLigne;
		if (unusedAllocatedBits < m_alocLigne){
#if _DEBUG
			//GfxDbgPrintf("new alloc in addColumn\n");
			size_t oldLength = m_length;
#endif
			size_t prod = (m_alocColonne + 1) * m_alocLigne;
			if (prod % 8 == 0)
				m_length = prod / 8;
			else
				m_length = prod / 8 + 1;
			char *temp = (char *)malloc(m_length);
#if _DEBUG
			for (size_t m = 0; m < m_length; m++)
				temp[m] = 0;
#endif

			for (size_t l = 0; l < m_alocLigne; l++){
				for (size_t c = 0; c <= m_alocColonne; c++){
					if (c == m_alocColonne){
						size_t prod = ((m_alocColonne + 1) * l) + c;
						size_t unsafePos = prod / 8;
#if _DEBUG
						GfxDbgAssert(unsafePos < m_length);
#endif
						char unsafeValue = temp[unsafePos];

#if _DEBUG
						GfxDbgAssert(GET_BIT(unsafeValue, 7 - (prod % 8)) == 0);
#endif

						temp[unsafePos] = SET_BIT_FALSE(unsafeValue, 7 - (prod % 8));
					}
					else{
						size_t oldProd = (m_alocColonne * l) + c;
						size_t unsafeOldPos = oldProd / 8;
#if _DEBUG
						GfxDbgAssert(unsafeOldPos < oldLength);
#endif
						char oldUnsafeValue = m_datas[unsafeOldPos];
						bool isLinked = IS_BIT_TRUE(oldUnsafeValue, 7 - (oldProd % 8));

						size_t newPos = ((m_alocColonne + 1) * l) + c;
						size_t unsafeNewPos = newPos / 8;
#if _DEBUG
						GfxDbgAssert(unsafeNewPos < m_length);
#endif
						char newUnsafeValue = temp[unsafeNewPos];

#if _DEBUG
						GfxDbgAssert(GET_BIT(newUnsafeValue, 7 - (newPos % 8)) == 0);
#endif

						if (isLinked)
							temp[unsafeNewPos] = SET_BIT_TRUE(newUnsafeValue, 7 - (newPos % 8));
						else
							temp[unsafeNewPos] = SET_BIT_FALSE(newUnsafeValue, 7 - (newPos % 8));
					}
				}
			}

			if (m_datas)
				free(m_datas);
			m_datas = temp;
		}
		else{
			if (m_alocLigne > 0){
				for (size_t l = 0; l < m_alocLigne; l++){
					size_t lInverse = m_alocLigne - 1 - l;
					for (size_t c = 0; c <= m_alocColonne; c++){
						size_t cInverse = m_alocColonne - c;
						if (cInverse == m_alocColonne){
							size_t prod = ((m_alocColonne + 1) * lInverse) + cInverse;
							size_t unsafePos = prod / 8;
#if _DEBUG
							GfxDbgAssert(unsafePos < m_length);
#endif
							char unsafeValue = m_datas[unsafePos];
							m_datas[unsafePos] = SET_BIT_FALSE(unsafeValue, 7 - (prod % 8));
						}
						else{
							size_t oldProd = (m_alocColonne * lInverse) + cInverse;
							size_t unsafeOldPos = oldProd / 8;
#if _DEBUG
							GfxDbgAssert(unsafeOldPos < m_length);
#endif
							char oldUnsafeValue = m_datas[unsafeOldPos];
							bool isLinked = IS_BIT_TRUE(oldUnsafeValue, 7 - (oldProd % 8));

							size_t newPos = ((m_alocColonne + 1) * lInverse) + cInverse;
							size_t unsafeNewPos = newPos / 8;
#if _DEBUG
							GfxDbgAssert(unsafeNewPos < m_length);
#endif
							char newUnsafeValue = m_datas[unsafeNewPos];
							if (isLinked)
								m_datas[unsafeNewPos] = SET_BIT_TRUE(newUnsafeValue, 7 - (newPos % 8));
							else
								m_datas[unsafeNewPos] = SET_BIT_FALSE(newUnsafeValue, 7 - (newPos % 8));
						}
					}
				}
			}
		}

		m_alocColonne++;
		auto it = m_indexColonne.insert(std::pair<Colonne, size_t>(col, m_alocColonne - 1));
		return it.first->second;
	}
	size_t							addLigne(Ligne ligne){
		auto index = m_indexLigne.find(ligne);
		if (index != m_indexLigne.end())
			return index->second;

		size_t unusedAllocatedBits = 8 * m_length - m_alocColonne * m_alocLigne;
		if (unusedAllocatedBits < m_alocColonne){
#if _DEBUG
			size_t oldLength = m_length;
#endif
			size_t size = m_alocColonne * (m_alocLigne + 1);
			if (size % 8 == 0)
				m_length = size / 8;
			else
				m_length = size / 8 + 1;
			char *temp = (char *)malloc(m_length);
#if _DEBUG
			for (size_t m = 0; m < m_length; m++)
				temp[m] = 0;
#endif

			for (size_t l = 0; l <= m_alocLigne; l++){
				for (size_t c = 0; c < m_alocColonne; c++){
					if (l == m_alocLigne){
						size_t prod = (m_alocColonne * l) + c;
						size_t unsafePos = prod / 8;
#if _DEBUG
						GfxDbgAssert(unsafePos < m_length);
#endif
						char unsafeValue = temp[unsafePos];
						temp[unsafePos] = SET_BIT_FALSE(unsafeValue, 7 - (prod % 8));
					}
					else{
						size_t oldProd = (m_alocColonne * l) + c;
						size_t unsafeOldPos = oldProd / 8;
#if _DEBUG
						GfxDbgAssert(unsafeOldPos < oldLength);
#endif
						char oldUnsafeValue = m_datas[unsafeOldPos];
						bool isLinked = IS_BIT_TRUE(oldUnsafeValue, 7 - (oldProd % 8));

						size_t newPos = (m_alocColonne * l) + c;
						size_t unsafeNewPos = newPos / 8;
#if _DEBUG
						GfxDbgAssert(unsafeNewPos < m_length);
#endif
						char newUnsafeValue = temp[unsafeNewPos];
						if (isLinked)
							temp[unsafeNewPos] = SET_BIT_TRUE(newUnsafeValue, 7 - (newPos % 8));
						else
							temp[unsafeNewPos] = SET_BIT_FALSE(newUnsafeValue, 7 - (newPos % 8));
					}
				}
			}

			if (m_datas)
				free(m_datas);
			m_datas = temp;
		}
		else{
			for (size_t col = 0; col < m_alocColonne; col++){
				size_t pos = col + (m_alocColonne * m_alocLigne);
				size_t unsafePos = pos / 8;
#if _DEBUG
				GfxDbgAssert(unsafePos < m_length);
#endif
				char unsafeElem = m_datas[unsafePos];
				m_datas[unsafePos] = SET_BIT_FALSE(unsafeElem, 7 - (pos % 8));
			}
		}

		m_alocLigne++;
		auto it = m_indexLigne.insert(std::pair<Ligne, int>(ligne, m_alocLigne - 1));
		return it.first->second;
	}

	void							removeColumn(size_t indexColonne){
		if (m_alocColonne == 1){
#if _DEBUG
			GfxDbgAssert(indexColonne == 0);
#endif
			m_indexColonne.clear();
			if (m_datas)
				free(m_datas);
			m_datas = nullptr;
			return;
		}

#if _DEBUG
		size_t oldLength = m_length;
#endif

		size_t allocatedBits = m_length * 8;
		size_t requierdBit = m_alocLigne * (m_alocColonne - 1);
		if (requierdBit < allocatedBits - 7){
			if (requierdBit % 8 == 0)
				m_length = requierdBit / 8;
			else
				m_length = requierdBit / 8 + 1;
			char *temp = (char *)malloc(m_length);
#if _DEBUG
			for (size_t m = 0; m < m_length; m++)
				temp[m] = 0;
#endif

			for (size_t l = 0; l < m_alocLigne; l++){
				for (size_t c = 0; c < m_alocColonne; c++){
					if (c == indexColonne)
						break;

					size_t pos = l * m_alocColonne + c;
					size_t unsafeIndex = pos / 8;
#if _DEBUG
					GfxDbgAssert(unsafeIndex < oldLength);
#endif
					char unsafeData = m_datas[unsafeIndex];
					bool isSet = IS_BIT_TRUE(unsafeData, 7 - (pos % 8));

					size_t newPos;
					if (c < indexColonne)
						newPos = l * (m_alocColonne - 1) + c;
					else
						newPos = l * (m_alocColonne - 1) + c - 1;
					size_t newUnsafeIndex = newPos / 8;
#if _DEBUG
					GfxDbgAssert(newUnsafeIndex < m_length);
					GfxDbgAssert(!IS_BIT_TRUE(temp[newUnsafeIndex], 7 - (newPos % 8)));
#endif
					if (isSet)
						temp[newUnsafeIndex] = SET_BIT_TRUE(temp[newUnsafeIndex], 7 - (newPos % 8));
					else
						temp[newUnsafeIndex] = SET_BIT_FALSE(temp[newUnsafeIndex], 7 - (newPos % 8));
				}
			}

			if (m_datas)
				free(m_datas);
			m_datas = temp;
		}
		else{
			for (size_t l = 0; l < m_alocLigne; l++){
				size_t reversedLigne = m_alocLigne - l - 1;
				for (size_t c = 0; c < m_alocColonne; c++){
					size_t reversedCol = m_alocColonne - c - 1;
					if (reversedCol == indexColonne)
						break;

					size_t oldPos = reversedLigne * m_alocColonne + reversedCol;
					size_t oldUnsafeIndex = oldPos / 8;
#if _DEBUG
					GfxDbgAssert(oldUnsafeIndex < oldLength);
#endif
					char unsafeData = m_datas[oldUnsafeIndex];
					bool isSet = IS_BIT_TRUE(unsafeData, 7 - (oldPos % 8));

					size_t newPos;
					if (c < indexColonne)
						newPos = reversedLigne * (m_alocColonne - 1) + reversedCol;
					else
						newPos = reversedLigne * (m_alocColonne - 1) + reversedCol - 1;
					size_t newUnsafeIndex = newPos / 8;
#if _DEBUG
					GfxDbgAssert(newUnsafeIndex < m_length);
#endif
					if (isSet)
						m_datas[newUnsafeIndex] = SET_BIT_TRUE(m_datas[newUnsafeIndex], 7 - (newPos % 8));
					else
						m_datas[newUnsafeIndex] = SET_BIT_FALSE(m_datas[newUnsafeIndex], 7 - (newPos % 8));
				}
			}
		}

		m_alocColonne--;
		for (auto it = m_indexColonne.begin(); it != m_indexColonne.end();){
			size_t ligneMaped = it->second;
			if (ligneMaped == indexColonne)
				it = m_indexColonne.erase(it);
			else if (ligneMaped > indexColonne){
				it->second--;
				it++;
			}
			else{
				it++;
			}
		}
	}
	void							removeLigne(size_t indexLigne){
		if (m_alocLigne == 1){
#if _DEBUG
			GfxDbgAssert(indexLigne == 0);
#endif
			m_indexLigne.clear();
			if (m_datas)
				free(m_datas);
			m_datas = nullptr;
			return;
		}

#if _DEBUG
		size_t oldLength = m_length;
#endif

		size_t allocatedBits = m_length * 8;
		size_t requierdBit = (m_alocLigne - 1) * m_alocColonne;
		if (requierdBit < allocatedBits - 7){
			if (requierdBit % 8 == 0)
				m_length = requierdBit / 8;
			else
				m_length = requierdBit / 8 + 1;
			char *temp = (char *)malloc(m_length);
#if _DEBUG
			for (size_t m = 0; m < m_length; m++)
				temp[m] = 0;
#endif

			for (size_t l = 0; l < indexLigne; l++){
				for (size_t c = 0; c < m_alocColonne; c++){
					size_t pos = l * m_alocColonne + c;
					size_t unsafeIndex = pos / 8;
#if _DEBUG
					GfxDbgAssert(unsafeIndex < oldLength);
#endif
					char unsafeData = m_datas[unsafeIndex];
					bool isSet = IS_BIT_TRUE(unsafeData, 7 - (pos % 8));

#if _DEBUG
					GfxDbgAssert(unsafeIndex < m_length);
					GfxDbgAssert(!IS_BIT_TRUE(temp[unsafeIndex], 7 - (pos % 8)));
#endif
					if (isSet)
						temp[unsafeIndex] = SET_BIT_TRUE(temp[unsafeIndex], 7 - (pos % 8));
					else
						temp[unsafeIndex] = SET_BIT_FALSE(temp[unsafeIndex], 7 - (pos % 8));
				}
			}

			for (size_t l = indexLigne + 1; l < m_alocLigne; l++){
				for (size_t c = 0; c < m_alocColonne; c++){
					size_t pos = l * m_alocColonne + c;
					size_t unsafeIndex = pos / 8;
#if _DEBUG
					GfxDbgAssert(unsafeIndex < oldLength);
#endif
					char unsafeData = m_datas[unsafeIndex];
					bool isSet = IS_BIT_TRUE(unsafeData, 7 - (pos % 8));

					size_t newPos = (l - 1) * m_alocColonne + c;
					size_t newUnsafeIndex = newPos / 8;
#if _DEBUG
					GfxDbgAssert(newUnsafeIndex < m_length);
					GfxDbgAssert(!IS_BIT_TRUE(temp[newUnsafeIndex], 7 - (newPos % 8)));
#endif
					if (isSet)
						temp[newUnsafeIndex] = SET_BIT_TRUE(temp[newUnsafeIndex], 7 - (newPos % 8));
					else
						temp[newUnsafeIndex] = SET_BIT_FALSE(temp[newUnsafeIndex], 7 - (newPos % 8));
				}
			}

			if (m_datas)
				free(m_datas);
			m_datas = temp;
		}
		else{
			for (size_t l = indexLigne; l < m_alocLigne - 1; l++){
				for (size_t c = 0; c < m_alocColonne; c++){
					size_t pos = l * m_alocColonne + c;
					size_t unsafeIndex = pos / 8;
#if _DEBUG
					GfxDbgAssert(unsafeIndex < oldLength);
#endif
					char unsafeData = m_datas[unsafeIndex];
					bool isSet = IS_BIT_TRUE(unsafeData, 7 - (pos % 8));

					size_t newPos = (l + 1) * m_alocColonne + c;
					size_t newUnsafeIndex = newPos / 8;
#if _DEBUG
					GfxDbgAssert(newUnsafeIndex < m_length);
#endif
					if (isSet)
						m_datas[newUnsafeIndex] = SET_BIT_TRUE(m_datas[newUnsafeIndex], 7 - (pos % 8));
					else
						m_datas[newUnsafeIndex] = SET_BIT_FALSE(m_datas[newUnsafeIndex], 7 - (pos % 8));
				}
			}
		}

		m_alocLigne--;
		for (auto it = m_indexLigne.begin(); it != m_indexLigne.end();){
			size_t ligneMaped = it->second;
			if (ligneMaped == indexLigne)
				it = m_indexLigne.erase(it);
			else if (ligneMaped > indexLigne){
				it->second--;
				it++;
			}
			else{
				it++;
			}
		}
	}

	void							valideBit(size_t ligneNumber, size_t colNumber){
		GfxDbgAssert(m_datas != nullptr);
		GfxDbgAssert(colNumber < m_alocColonne, "invalid index for column");
		GfxDbgAssert(ligneNumber < m_alocLigne, "invalid index for ligne");

		size_t prod = colNumber + ligneNumber * m_alocColonne;
		int unsafeIndex = prod / 8;
		char unsafeData = m_datas[unsafeIndex];
		m_datas[unsafeIndex] = SET_BIT_TRUE(unsafeData, 7 - (prod % 8));
	}
	void							invalidBit(size_t ligneNumber, size_t colNumber){
		GfxDbgAssert(m_datas != nullptr);
		GfxDbgAssert(colNumber <= m_alocColonne, "invalid index for column");
		GfxDbgAssert(ligneNumber <= m_alocLigne, "invalid index for ligne");

		size_t prod = colNumber + ligneNumber * m_alocColonne;
		int unsafeIndex = prod / 8;
		char unsafeData = m_datas[unsafeIndex];
		m_datas[unsafeIndex] = SET_BIT_FALSE(unsafeData, 7 - (prod % 8));
	}

private:
	std::map<Ligne, size_t>		m_indexLigne;
	std::map<Colonne, size_t>	m_indexColonne;
	size_t						m_alocLigne;
	size_t						m_alocColonne;
	size_t						m_length;
	char*						m_datas;
};

#endif
