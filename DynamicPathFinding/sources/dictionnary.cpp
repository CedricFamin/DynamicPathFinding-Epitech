#include "../includes/dictionnary.hpp"

// AddWord --------------------------------------------------------------------
M_AI_DynamicPathFinding::DictionnaryNode_t* M_AI_DynamicPathFinding::AddWord(const char* word, const unsigned int& wordReference, M_AI_DynamicPathFinding::DictionnaryNode_t* rootNode)
{
    DictionnaryNode_t* ret = rootNode;
    DictionnaryNode_t* current;
    unsigned int wordLen = strlen(word);
    
    // Create root node if applicable
    if (ret == NULL)
    {
        ret = CreateNode('_');
    }
    
    // Add all node of the word
    current = ret;
    for (unsigned int i = 0; i < wordLen; ++i)
        current = CreateNode(word[i], current);
    // Set the word reference to the last node
    if (current->_wordReference != 0)
        if (current->_wordReference = wordReference)
            std::cerr << "/!\\ Warning : Word [" << word << "] was already setted" << std::endl;
        else
            std::cerr << "/!\\ Warning : Word [" << word << "] had reference [" << current->_wordReference << "] and now changed to [" << wordReference << "]" << std::endl;
    current->_wordReference = wordReference;
    
    return ret;
}

// CreateNode -----------------------------------------------------------------
M_AI_DynamicPathFinding::DictionnaryNode_t* M_AI_DynamicPathFinding::CreateNode(char c, M_AI_DynamicPathFinding::DictionnaryNode_t* parent)
{
    DictionnaryNode_t* ret = NULL;
    
    // Validate char
    if (!((c >= 'a' && c <= 'z')
          || (c >= 'A' && c <= 'Z')
          || (c >= '0' && c <= '9')
          || (c == '_')
          || (c == '-')
          || (c == '#')))
    {
        std::cerr << "Trying to add an invalid letter [" << c << "] to a dictionnary" << std::endl;
        return ret;
    }
    // if there is no parent, create an root node
    if (parent == NULL)
    {
        ret = new DictionnaryNode_t;
        ret->_letter = c;
        ret->_wordReference = 0;
        ret->_childNodes = NULL;
    }
    // else
    else
    {
        // create children pointer if applicable
        if (parent->_childNodes == NULL)
        {
            parent->_childNodes = new DictionnaryNode_t*[255];
            for (unsigned int i = 0; i < 255; ++i) parent->_childNodes[i] = NULL;
        }
        // set to the existing node if applicable
        ret = parent->_childNodes[c];
        // or create a new node and add it to the parent children list
        if (ret == NULL)
        {
            ret = new DictionnaryNode_t;
            ret->_letter = c;
            ret->_wordReference = 0;
            ret->_childNodes = NULL;
            parent->_childNodes[c] = ret;
        }
    }
    return ret;
}

// DeleteDictionnary ------------------------------------------------------------
void M_AI_DynamicPathFinding::DeleteDictionnary(M_AI_DynamicPathFinding::DictionnaryNode_t* root)
{
    if (root != NULL)
    {
        if (root->_childNodes != NULL)
        {
            for (unsigned int i = 0; i < 255; ++i)
            {
                if (root->_childNodes[i] != NULL) DeleteDictionnary(root->_childNodes[i]);
            }
        }
    }
    delete root;
    root = NULL;
}

// ParseWord --------------------------------------------------------------------
unsigned int M_AI_DynamicPathFinding::ParseWord(const M_AI_DynamicPathFinding::DictionnaryNode_t* root, const char* word)
{
    DictionnaryNode_t* current;
    unsigned int ret = 0;
    unsigned int wordLen = strlen(word);
    unsigned int i;
    
    current = const_cast<DictionnaryNode_t*>(root);
    for (i = 0; i < wordLen && current; ++i)
        current = current->_childNodes[word[i]];
    if (current && i == wordLen) ret = current->_wordReference;
    
    return ret;
}

// StripArgument --------------------------------------------------------------------
std::string M_AI_DynamicPathFinding::StripArgument(std::string& line, unsigned int argumentNb)
{
    size_t tmp = 0, begin = 0, end = 0;
    std::string word;
    
    do {
        // Get Begin of word
        do {
            begin = tmp;
            tmp = line.find_first_not_of(' ', tmp);
            if (tmp == begin) tmp = line.find_first_not_of('\t', tmp);
        } while (tmp != begin);
        // If there is a word
        if (begin != std::string::npos)
        {
            // Search for its end
            end = line.find_first_of(' ', begin);
            if (end == std::string::npos) end = line.find_first_of('\t', begin);
            if (end != std::string::npos) {
                end;
            }
        }
        tmp = end;
    } while (argumentNb--);
    
    // Substring the word and
    if (begin != std::string::npos)
        word = line.substr(begin, end - begin);
    
    return word;
}

// StripFirstWord -------------------------------------------------------------------
std::string M_AI_DynamicPathFinding::StripFirstWord(std::string& line)
{
    size_t tmp = 0, begin = 0, end = 0;
    std::string word;
    
    // Get Begin of word
    do {
        begin = tmp;
        tmp = line.find_first_not_of(' ', tmp);
        if (tmp == begin) tmp = line.find_first_not_of('\t', tmp);
    } while (tmp != begin);
    // If there is a word
    if (begin != std::string::npos)
    {
        // Search for its end
        end = line.find_first_of(' ', begin);
        if (end == std::string::npos) end = line.find_first_of('\t', begin);
        if (end != std::string::npos) {
            end = end - begin;
        }
        // Substring the word and 
        word = line.substr(begin, end);
    }
    return word;
}
