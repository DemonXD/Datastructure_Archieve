#include <iostream>
#include <assert.h>

enum TYPE
{
	HEAD_TYPE,	//表头
	VALUE_TYPE,	//值
	SUB_TYPE,	//子表
};
 
struct GeneralizedNode
{
	GeneralizedNode(TYPE type,const char& value=0)
		:_type(type)
		,_next(NULL)
		,_sublink(NULL)
	{
		_value = value;
	}
 
	TYPE _type;		//类型
	GeneralizedNode* _next;	//指向下一个表项
	union
	{
		char _value;
		GeneralizedNode* _sublink;	//指向子表头
	};
	//除头结点外，剩下的节点中每个结点有可能是value也可能是sub，因此用联合
};


class GeneralizedList
{
	typedef GeneralizedNode Node;
public:
	GeneralizedList()
		:_head(NULL)
	{}
 
	GeneralizedList(const char* str)
		:_head(NULL)
	{
		_head = _CreatList(str);
	}
	
	GeneralizedList(const GeneralizedList& g)	//拷贝构造
	{
		_head = _copy(g._head);
	}
 
	~GeneralizedList()
	{
		Destory(_head);
	}
 
	GeneralizedList& operator=(const GeneralizedList& g)
	{
		//_head = _assignment(g);
		if (this != &g)
		{
			GeneralizedList tmp(g);
			std::swap(_head,tmp._head);
		}
		return *this;
	}
 
	size_t Size()
	{
		return _size(_head);
	}
	size_t Depth()
	{
		return _depth(_head);
	}
	void Print()
	{
		_print(_head);
	}
protected:
	bool Judge(const char& value)
	{
		if ((value >= '0' && value <= '9') ||
			(value >= 'a' && value <= 'z') ||
			(value >= 'A' && value <= 'Z') )	
			return true;
		else
			return false;
	}
	Node* _CreatList(const char*& str)
	{
		assert(*str == '(');
		Node* head = new Node(HEAD_TYPE,*str);
		Node* prev = head;
		head->_type = HEAD_TYPE;
		++str;
		while (*str)
		{
			if (Judge(*str))		//有效值项
			{
				Node* node = new Node(VALUE_TYPE,*str);
				prev->_next = node;
				prev = prev->_next;
 
				++str;
			}
			else if (*str == '(')		//有子表项
			{
				Node* node = new Node(SUB_TYPE,*str);
				prev->_next = node;
				prev = prev->_next;
				prev->_sublink = _CreatList(str);
				++str;
			}
			else if (*str == ')')
			{
				prev->_next = NULL;
 
				++str;
				return head;
			}
			else
				++str;
		}
		return head;
	}
	Node* _copy(Node* copyhead)
	{
		assert(copyhead);
		Node* newhead = new Node(HEAD_TYPE,copyhead->_value);	//新表的头
		Node* prev = newhead;
		Node* cur = copyhead->_next;
		while (cur)
		{
			if (cur->_type == VALUE_TYPE)	//拷贝值结点
			{
				Node* tmp = new Node(VALUE_TYPE,cur->_value);
				prev->_next = tmp;
				prev = prev->_next;
				cur = cur->_next;
			}
			else if (cur->_type == SUB_TYPE)		//拷贝子表
			{
				Node* tmp = new Node(SUB_TYPE);
				prev->_next = tmp;
				prev = prev->_next;
				tmp->_sublink = _copy(cur->_sublink);
				cur = cur->_next;
			}
			else
				cur = cur->_next;
		}
		return newhead;
	}
	void Destory(Node* head)
	{
		Node* cur = head;
		while (cur)
		{
			Node* del = cur;
			if (cur->_type == SUB_TYPE)
			{
				Destory(cur->_sublink);
			}
			cur = cur->_next;
			delete[] del;
		}
	}
	Node* _assignment(const GeneralizedList& g)
	{
		assert(g._head);
		if (this != &g)
		{
			Node* newhead = new Node(HEAD_TYPE,g._head->_value);
			Destory(_head);
			_head = newhead;
		}
		return _head;
	}
	size_t _size(Node* head)	//表内有效值的个数
	{
		Node* cur = head;
		size_t count = 0;
		while (cur)
		{
			if (cur->_type == VALUE_TYPE)
			{
				count++;
			}
			else if (cur->_type == SUB_TYPE)
			{
				count += _size(cur->_sublink);
			}
			cur = cur->_next;
		}
		return count;
	}
	size_t _depth(Node* head)	//表的最大深度
	{
		Node* cur = head;
		size_t maxdepth = 1;
		while(cur)
		{
			size_t depth = 1;
			if (cur->_type == SUB_TYPE)
			{
				depth += _depth(cur->_sublink);
				if (depth > maxdepth)
				{
					maxdepth = depth;
				}
			}
			cur = cur->_next;
		}
		return maxdepth;
	}
	void _print(Node* head)
	{
		assert(head);
		Node* cur = head;
		while (cur)
		{
			if (cur->_type == VALUE_TYPE)
			{
				std::cout<<cur->_value;
				if (cur->_next != NULL)
				{
					std::cout<<",";
				}
				cur = cur->_next;
			}
			else if (cur->_type == SUB_TYPE)
			{
				_print(cur->_sublink);
				if (cur->_next != NULL)
				{
					std::cout<<",";
				}
				cur = cur->_next;
			}
			else
			{
				std::cout<<"(";
				cur = cur->_next;
			}
		}
		std::cout<<")";
	}
protected:
	Node* _head;
};
