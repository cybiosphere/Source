
#pragma once 
  
//#include "afxtempl.h" 
  
// Classe d�riv�e de CArray en reprenant toutes les capacit�s 
// Ajout de m�thodes de tri et d'ajout par insertion avec recherche de position par dichotomie 
template <class TYPE, class ARG_TYPE = const TYPE&> 
class CSortedArray : public CArray<TYPE, ARG_TYPE> 
{ 
public: 
    INT_PTR AddSorted(TYPE newElement); // Ajout par insertion avec recherche de position par dichotomie 
    void Sort(void) { QuickSort(0, GetCount()-1); } // Effectue un tri complet de la table � l'aide la m�thode QuickSort 
    void PartialSort(INT_PTR startIndex, INT_PTR endIndex); // Effectue un tri partiel de la table 
    void Swap(INT_PTR src, INT_PTR dst) { TYPE tmp=GetAt(src); SetAt(src, GetAt(dst)); SetAt(dst, tmp); } // Echange 2 �l�ments dans la table 
  
protected: 
    void QuickSort(INT_PTR startIndex, INT_PTR endIndex); 
}; 
  
// Ajoute un �l�ment � la table par insertion en recherchant sa position par dichotomie 
// Entr�e : le nouvel �l�ment � ajouter 
// Sortie : l'index dans la table o� le nouvel �l�ment a �t� ajout� 
template <class TYPE, class ARG_TYPE> 
AFX_INLINE INT_PTR CSortedArray<TYPE, ARG_TYPE>::AddSorted(TYPE newElement) 
{ 
    INT_PTR start = 0, end = GetCount(); 
    while(start < end) 
    { 
        INT_PTR pos = (start + end) / 2; 
        if(newElement > GetAt(pos)) 
            start = pos + 1; 
        else 
            end = pos; 
    } 
    InsertAt(start, newElement); 
    return start; 
} 
  
// Effectue un tri QuickSort des �l�ments compris entre les index "A" et "B" (inclus) 
// Entr�e : l'index du premier �l�ment et l'index du dernier �l�ment � trier 
template <class TYPE, class ARG_TYPE> 
AFX_INLINE void CSortedArray<TYPE, ARG_TYPE>::PartialSort(INT_PTR startIndex, INT_PTR endIndex) 
{ 
    if(startIndex < 0) 
        startIndex = 0; 
  
    if(endIndex >= GetCount()) 
        endIndex = GetCount() - 1; 
  
    if(startIndex >= endIndex) 
        return; 
  
    QuickSort(startIndex, endIndex); 
} 
  
// M�thode de tri QuickSort d'une portion de la table 
// Entr�e : l'index du premier �l�ment � trier et l'index du dernier �l�ment � trier 
template <class TYPE, class ARG_TYPE> 
AFX_INLINE void CSortedArray<TYPE, ARG_TYPE>::QuickSort(INT_PTR startIndex, INT_PTR endIndex) 
{ 
    if(startIndex >= endIndex) 
        return; 
  
    if(startIndex == endIndex - 1) 
    { 
        if(GetAt(startIndex) > GetAt(endIndex)) 
            Swap(startIndex, endIndex); 
        return; 
    } 
  
    INT_PTR startToEnd = startIndex, endToStart = endIndex; 
  
    Swap((startToEnd + endToStart) / 2, endToStart); 
  
    TYPE middle = GetAt(endToStart); 
  
    while(startToEnd < endToStart) 
    { 
        while(GetAt(startToEnd) <= middle && startToEnd < endToStart) startToEnd++; 
        while(middle <= GetAt(endToStart) && startToEnd < endToStart) endToStart--; 
        if(startToEnd < endToStart) 
            Swap(startToEnd, endToStart); 
    } 
  
    SetAt(endIndex, GetAt(endToStart)); 
    SetAt(endToStart, middle); 
  
    QuickSort(startIndex, startToEnd - 1); 
  
    QuickSort(endToStart + 1, endIndex); 
} 
