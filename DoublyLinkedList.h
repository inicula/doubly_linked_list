#pragma once
#include <cassert>

template<typename Ptr, bool is_const>
struct list_iterator
{

};


//wraps regular iterator to function as reversed
template<typename It>
struct reverse_list_iterator
{
	It it;
};

template<typename T>
class DoublyLinkedList
{
private:
	struct Node
	{
		Node() = default;
		Node(const T& data, Node* prev = nullptr, Node* next = nullptr)
			:
			data(data),
			prev(prev),
			next(next)
		{ }
		Node(T&& data, Node* prev = nullptr, Node* next = nullptr)
			:
			data(std::move(data)),
			prev(prev),
			next(next)
		{ }

		T data;
		Node* prev = nullptr;
		Node* next = nullptr;
	};
	using alloc_pointer = Node*;
public:
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;
	using size_type = size_t;
	using difference_type = std::ptrdiff_t;

	using iterator = list_iterator<alloc_pointer, false>;
	using const_iterator = list_iterator<alloc_pointer, true>;
	using reverse_iterator = reverse_list_iterator<iterator>;
	using const_reverse_iterator = reverse_list_iterator<const_iterator>;

public:
	DoublyLinkedList() = default;

	~DoublyLinkedList()
	{
		free_list();
	}

	//insert element with constructor arguments in-place at the end
	template<typename... U>
	void push_back(U&&... args)
	{
		if(first_ == nullptr)
		{
			first_ = new Node(T(std::forward<U>(args)...));
			last_ = first_;
		}
		else
		{
			last_->next = new Node(T(std::forward<U>(args)...), last_);
			last_ = last_->next;
		}
		size_++;
	}

	//insert element at the end
	template<typename U>
	void push_back(U&& value)
	{
		if(first_ == nullptr)
		{
			first_ = new Node(std::forward<U>(value));
			last_ = first_;
		}
		else
		{
			last_->next = new Node(std::forward<U>(value), last_);
			last_ = last_->next;
		}
		size_++;
	}

	//insert element with constructor arguments in-place at the beginning
	template<typename... U>
	void push_front(U&&... args)
	{
		if(first_ == nullptr)
		{
			first_ = new Node(T(std::forward<U>(args)...));
			last_ = first_;
		}
		else
		{
			first_->prev = new Node(T(std::forward<U>(args)...), nullptr, first_);
			first_ = first_->prev;
		}
		size_++;
	}

	//insert element at the beginning
	template<typename U>
	void push_front(U&& value)
	{
		if(first_ == nullptr)
		{
			first_ = new Node(std::forward<U>(value));
			last_ = first_;
		}
		else
		{
			first_->prev = new Node(std::forward<U>(value), nullptr, first_);
			first_ = first_->prev;
		}
		size_++;
	}


	//remove all elements equal to this value
	//return number of deleted elements
	size_t remove(const T& value)
	{
		size_t count = 0;
		auto head = first_;
		while(head != nullptr)
		{
			if(head->data == value)
			{
				auto next = head->next;
				detach(head);
				delete head;
				head = next;
				++count;
				--size_;
			}
			else
			{
				head = head->next;
			}
		}
		return count;
	}

	T pop_back()
	{
		assert(first_ != nullptr);
		auto value = last_->data;
		if(size_ == 1)
		{
			free_list();
		}
		else
		{
			last_ = last_->prev;
			delete last_->next;
			last_->next = nullptr;
			--size_;
		}
		return value;
	}

	T pop_front()
	{
		assert(first_ != nullptr);
		auto value = first_->data;
		if(size_ == 1)
		{
			free_list();
		}
		else
		{
			first_ = first_->next;
			delete first_->prev;
			first_->prev = nullptr;
			--size_;
		}
		return value;
	}

	reference front()
	{
		assert(first_ != nullptr);
		return first_->data;
	}

	const_reference front() const
	{
		assert(first_ != nullptr);
		return first_->data;
	}

	reference back()
	{
		assert(first_ != nullptr);
		return first_->last_;
	}

	const_reference back() const
	{
		assert(first_ != nullptr);
		return first_->last_;
	}

	auto size() const
	{
		return size_;
	}

	void print(std::ostream& os = std::cout, char sep = ' ', char endline_ch = '\n')
	{
		auto head = first_;
		while(head != nullptr)
		{
			os << head->data << sep;
			head = head->next;
		}
		os << endline_ch;
	}
	void print_reverse(std::ostream& os = std::cout, char sep = ' ', char endline_ch = '\n')
	{
		auto head = last_;
		while(head != nullptr)
		{
			os << head->data << sep;
			head = head->prev;
		}
		os << endline_ch;
	}


private:
	void free_list()
	{
		while(first_ != nullptr)
		{
			auto next = first_->next;
			delete first_;
			first_ = next;
		}
		first_ = nullptr;
		last_ = nullptr;
		size_ = 0;
	}

	void detach(Node* node)
	{
		assert(node != nullptr);
		if(node->next != nullptr)
		{
			node->next->prev = node->prev;
		}
		if(node->prev != nullptr)
		{
			node->prev->next = node->next;
		}
		if(node == first_)
		{
			first_ = first_->next;
		}
		if(node == last_)
		{
			last_ = last_->prev;
		}
	}


private:
	Node* first_ = nullptr;
	Node* last_ = nullptr;
	size_type size_ = 0;

};