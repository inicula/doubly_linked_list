#pragma once
#include <cassert>

template<typename>
class reverse_list_iterator;

template<typename>
class DoublyLinkedList;

template<typename List, bool is_const>
class list_iterator
{
public:
	friend class DoublyLinkedList<typename List::value_type>;
	friend class reverse_list_iterator<list_iterator>;

	using iterator_category = typename std::bidirectional_iterator_tag;
	using value_type = typename List::value_type;
	using difference_type = typename List::difference_type;
	using pointer = typename std::conditional_t<is_const, typename List::const_pointer, typename List::pointer>;
	using reference = typename std::conditional_t<is_const, typename List::const_reference, typename List::reference>;
	using alloc_type = typename List::alloc_type;

public:
	explicit list_iterator(alloc_type ptr)
		:
		ptr_(ptr)
	{ }

	template<typename OtherIt>
	bool operator==(const OtherIt& rhs) const
	{
		return ptr_ == rhs.ptr_;
	}

	template<typename OtherIt>
	bool operator!=(const OtherIt& rhs) const
	{
		return ptr_ != rhs.ptr_;
	}

	reference operator*() const
	{
		return ptr_->data;
	}

	pointer operator->() const
	{
		return &(ptr_->data);
	}

	list_iterator& operator++()
	{
		ptr_ = ptr_->next;
		return *this;
	}

	list_iterator operator++(int)
	{
		auto current = *this;
		ptr_ = ptr_->next;
		return current;
	}

	list_iterator& operator--()
	{
		ptr_ = ptr_->prev;
		return *this;
	}

	list_iterator operator--(int)
	{
		auto current = *this;
		ptr_ = ptr_->prev;
		return current;
	}

private:
	alloc_type ptr_;
};


//wraps regular iterator to function as reversed
template<typename It>
class reverse_list_iterator
{
public:
	
	using iterator_category = typename It::iterator_category;
	using value_type = typename It::value_type;
	using difference_type = typename It::difference_type;
	using pointer = typename It::pointer;
	using reference = typename It::reference;
	using alloc_type = typename It::alloc_type;

public:
	explicit reverse_list_iterator(alloc_type ptr)
		:
		it(ptr)
	{ }

	bool operator==(const reverse_list_iterator& rhs) const
	{
		return it == rhs.it;
	}
	
	bool operator!=(const reverse_list_iterator& rhs) const
	{
		return it != rhs.it;
	}

	reference operator*() const
	{
		return it.operator*();
	}

	pointer operator->() const
	{
		return it.operator->();
	}

	reverse_list_iterator& operator++()
	{
		--it;
		return *this;
	}

	reverse_list_iterator operator++(int)
	{
		auto current = *this;
		--current;
		return current;
	}

	reverse_list_iterator& operator--()
	{
		++it;
		return *this;
	}

	reverse_list_iterator operator--(int)
	{
		auto current = *this;
		++current;
		return current;
	}

private:
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
public:
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;
	using size_type = size_t;
	using difference_type = std::ptrdiff_t;
	using alloc_type = Node*;

	using iterator = list_iterator<DoublyLinkedList, false>;
	using const_iterator = list_iterator<DoublyLinkedList, true>;
	using reverse_iterator = reverse_list_iterator<iterator>;
	using const_reverse_iterator = reverse_list_iterator<const_iterator>;

public:
	DoublyLinkedList() = default;

	DoublyLinkedList(const std::initializer_list<T>& i_l)
	{
		for(const auto& el : i_l)
		{
			push_back(el);
		}
	}

	DoublyLinkedList(const DoublyLinkedList& rhs)
	{
		for(const auto& el : rhs)
		{
			push_back(el);
		}
	}

	DoublyLinkedList(DoublyLinkedList&& rhs)
		:
		first_(rhs.first_),
		last_(rhs.last_),
		size_(rhs.size_)
	{
		rhs.first_ = nullptr;
		rhs.last_ = nullptr;
		rhs.size_ = 0;
	}

	DoublyLinkedList& operator=(const DoublyLinkedList& rhs)
	{
		free_list();
		for(const auto& el : rhs)
		{
			push_back(rhs);
		}
	}

	DoublyLinkedList& operator=(DoublyLinkedList&& rhs)
	{
		free_list();

		first_ = rhs.first_;
		last_ = rhs.last_;
		size_ = rhs.size_;

		rhs.first_ = nullptr;
		rhs.last_ = nullptr;
		rhs.size_ = 0;
	}

	~DoublyLinkedList()
	{
		free_list();
	}

	friend void swap(DoublyLinkedList& lhs, DoublyLinkedList& rhs)
	{
		std::swap(lhs.first_, rhs.first_);
		std::swap(lhs.last_, rhs.last_);
		std::swap(lhs.size_, rhs.size_);
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

	iterator find(const T& value)
	{
		auto head = first_;
		while(head != nullptr)
		{
			if(head->data == value)
			{
				return iterator(head);
			}
			head = head->next;
		}
		return iterator(nullptr);
	}

	const_iterator find(const T& value) const
	{
		auto head = first_;
		while(head != nullptr)
		{
			if(head->data == value)
			{
				return iterator(head);
			}
			head = head->next;
		}
		return const_iterator(nullptr);
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

	iterator erase(iterator i1, iterator i2)
	{
		while(i1 != i2)
		{
			auto to_delete = i1.ptr_;
			++i1;
			detach(to_delete);
			delete to_delete;
			--size_;
		}
		return iterator(nullptr);
	}

	iterator erase(const_iterator i1, const_iterator i2)
	{
		while(i1 != i2)
		{
			auto to_delete = i1.ptr_;
			++i1;
			detach(to_delete);
			delete to_delete;
		}
		--size_;
		return iterator(nullptr);
	}

	iterator erase(iterator i1)
	{
		auto to_delete = i1.ptr_;
		++i1;
		detach(to_delete);
		delete to_delete;
		--size_;
		return i1;
	}

	iterator erase(const_iterator i1)
	{
		auto to_delete = i1.ptr_;
		++i1;
		detach(to_delete);
		delete to_delete;
		--size_;
		return iterator(i1.ptr_);
	}

	iterator begin()
	{
		return iterator(first_);
	}

	const_iterator begin() const
	{
		return const_iterator(first_);
	}

	const_iterator cbegin() const
	{
		return const_iterator(first_);
	}

	iterator end()
	{
		return iterator(nullptr);
	}

	const_iterator end() const
	{
		return const_iterator(nullptr);
	}

	const_iterator cend() const
	{
		return const_iterator(nullptr);
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator(last_);
	}

	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator(last_);
	}

	const_reverse_iterator crbegin() const
	{
		return const_reverse_iterator(last_);
	}

	reverse_iterator rend()
	{
		return reverse_iterator(nullptr);
	}

	const_reverse_iterator rend() const
	{
		return const_reverse_iterator(nullptr);
	}

	const_reverse_iterator crend() const
	{
		return const_reverse_iterator(nullptr);
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

	void print(std::ostream& os = std::cout, char sep = ' ', char endline_ch = '\n') const
	{
		auto head = first_;
		while(head != nullptr)
		{
			os << head->data << sep;
			head = head->next;
		}
		os << endline_ch;
	}

	void print_reverse(std::ostream& os = std::cout, char sep = ' ', char endline_ch = '\n') const
	{
		auto head = last_;
		while(head != nullptr)
		{
			os << head->data << sep;
			head = head->prev;
		}
		os << endline_ch;
	}

	void pc()
	{
		Node* first = last_;
		first = new Node(5);
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