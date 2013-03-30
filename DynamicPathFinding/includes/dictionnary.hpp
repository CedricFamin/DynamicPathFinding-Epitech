#ifndef __M_AI_DYNAMICPATHFINDING_DICTIONNARY_HPP__
#define __M_AI_DYNAMICPATHFINDING_DICTIONNARY_HPP__

// Standard C Includes
#include <stddef.h>
#include <string.h>

// Standard C++ Includes
#include <iostream>

// TBB Includes

// SFML Includes

// DynamicPathFinding Includes

namespace M_AI_DynamicPathFinding
{
    typedef struct DictionnaryNode_s
    {
        char _letter;
        unsigned int _wordReference;
        struct DictionnaryNode_s** _childNodes;
    } DictionnaryNode_t;
    
    /// Add a new word to the given dictionnary and set the given word reference to the final node
    /// if root node is not given, then create a new one.
    DictionnaryNode_t* AddWord(const char* word, const unsigned int& wordReference, DictionnaryNode_t* root = NULL);
    
    /// Create a new Node
    /// Append to the parent if parent is not null
    /// return a pointer to the new node
    DictionnaryNode_t* CreateNode(char c, DictionnaryNode_t* parent = NULL);
    
    /// delete all dictionnary nodes from the given node (included)
    void DeleteDictionnary(DictionnaryNode_t* root);
    
    /// Parse Word using the dictionnary given by its root node
    /// Return the word reference
    /// Return 0 if word is not registered
    unsigned int ParseWord(const DictionnaryNode_t* root, const char* word);
    
    /// Get the Nth argument
    std::string StripArgument(std::string& line, unsigned int argumentNb);
    
    /// Get first word from a line (string version)
    /// Return the word or a null string
    std::string StripFirstWord(std::string& line);
}

#endif /* __M_AI_DYNAMICPATHFINDING_DICTIONNARY_HPP__ */
