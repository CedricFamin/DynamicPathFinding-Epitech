//
//  Node.h
//  DynamicPathFinding
//
//  Created by Cédric Famin on 01/04/13.
//
//

#ifndef DynamicPathFinding_Node_h
#define DynamicPathFinding_Node_h

// Node for A* algorithm
// You can embed yout data (travel cost ...) in the data property
// and use this in the eval_node method of APathFinder
template<typename Data>
class Node
{
public:
    typedef Data data_type;
    
    Node() : _closed(false), _opened(false), _parent(0) {}
    ~Node() {}
    
    void AddNode(Node<data_type> * node) { _edges.push_back(node); }
    std::list<Node<data_type>*> const & Edges() const { return _edges; }
    
    data_type & GetData() { return _data; }
    
    
    unsigned int X() const { return _x; }
    unsigned int Y() const { return _y; }
    unsigned int & X() { return _x; }
    unsigned int & Y() { return _y; }
    
    bool Closed() const { return _closed; }
    void Close(bool value) { _closed = value; }
    
    bool Opened() const { return _opened; }
    void Open(bool value) { _opened = value; }
    
    void SetParent(Node<data_type>* parent) { _parent = parent; }
    Node<data_type>* GetParent() const { return _parent; }
    
    unsigned int GetDepth() const { return _depth; }
    void SetDepth(unsigned int parValue) { _depth = parValue; }
    
    void Reinit()
    {
        _closed = false;
        _opened = false;
        _depth = 0;
        _parent = 0;
    };
    
private:
    std::list<Node<data_type>*> _edges;
    Data _data;
    unsigned int _x;
    unsigned int _y;
    unsigned int _depth;
    bool _closed;
    bool _opened;
    
    Node<data_type>* _parent;
};

#endif
