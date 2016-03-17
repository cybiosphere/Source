/*
https://github.com/cybiosphere
copyright (c) 2005-2014 Frederic RIBERT

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#pragma once 
  
#include "afxtempl.h" 
  
// Classe dérivée de CArray en reprenant toutes les capacités 
// Ajout de méthodes de tri et d'ajout par insertion avec recherche de position par dichotomie 
template <class TYPE, class ARG_TYPE = const TYPE&> 
class CSortedArray : public CArray<TYPE, ARG_TYPE> 
{ 
public: 
    INT_PTR AddSorted(TYPE newElement); // Ajout par insertion avec recherche de position par dichotomie 
    void Sort(void) { QuickSort(0, GetCount()-1); } // Effectue un tri complet de la table à l'aide la méthode QuickSort 
    void PartialSort(INT_PTR startIndex, INT_PTR endIndex); // Effectue un tri partiel de la table 
    void Swap(INT_PTR src, INT_PTR dst) { TYPE tmp=GetAt(src); SetAt(src, GetAt(dst)); SetAt(dst, tmp); } // Echange 2 éléments dans la table 
  
protected: 
    void QuickSort(INT_PTR startIndex, INT_PTR endIndex); 
}; 
  
// Ajoute un élément à la table par insertion en recherchant sa position par dichotomie 
// Entrée : le nouvel élément à ajouter 
// Sortie : l'index dans la table où le nouvel élément a été ajouté 
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
  
// Effectue un tri QuickSort des éléments compris entre les index "A" et "B" (inclus) 
// Entrée : l'index du premier élément et l'index du dernier élément à trier 
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
  
// Méthode de tri QuickSort d'une portion de la table 
// Entrée : l'index du premier élément à trier et l'index du dernier élément à trier 
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
