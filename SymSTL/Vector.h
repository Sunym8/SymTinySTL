#pragma once

#include <iostream>
#include <cstring>
#include <typeinfo>

namespace sym {
	namespace stl {

		template <typename T>
		class Vector {
		public:
			Vector();
			Vector(std::initializer_list<T> init_list);
			~Vector();

			void push_back(const T& value);
			void pop_back();
			size_t size() const;
			size_t capacity() const;
			void show() const;
			void reserve(int n);
			void resize(int n);
			T& at(int n);
			T& operator[] (int n);
			bool empty() const;
			void clear();
			T& front();
			T& back();
			void swap(Vector& other);
			T* data();
			Vector& operator = (const Vector& other);

			class Iterator {
			public:

				using iterator_category = std::random_access_iterator_tag;  // 随机访问迭代器
				using value_type = T;
				using difference_type = std::ptrdiff_t;
				using pointer = T*;
				using reference = T&;

				Iterator() : m_pointer(nullptr) {}
				Iterator(T* pointer) : m_pointer(pointer) {}
				~Iterator() {}

				bool operator < (const Iterator& other) const {
					return *m_pointer < *(other.m_pointer);
				}

				bool operator == (const Iterator& other) const {
					return m_pointer == other.m_pointer;
				}
				bool operator != (const Iterator& other) const {
					return m_pointer != other.m_pointer;
				}

				Iterator& operator = (const Iterator& other) {
					if (*this != other) {
						m_pointer = other.m_pointer;
					}
					return *this;
				}

				T& operator [] (difference_type n) const {
					return *(m_pointer + n);
				}

				Iterator& operator ++ () {
					++m_pointer;
					return *this;
				}

				Iterator operator ++ (int) {
					Iterator tmp = *this;
					++m_pointer;
					return tmp;
				}

				Iterator& operator -- () {
					--m_pointer;
					return *this;
				}

				Iterator operator -- (int) {
					Iterator tmp = *this;
					--m_pointer;
					return tmp;
				}

				Iterator operator + (difference_type i) {
					return Iterator(m_pointer + i);
				}

				Iterator& operator += (difference_type i) {
					m_pointer += i;
					return *this;
				}

				Iterator operator - (difference_type i) {
					return Iterator(m_pointer - i);
				}

				Iterator& operator -= (difference_type i) {
					m_pointer -= i;
					return *this;
				}

				int operator - (const Iterator& other) const {
					return m_pointer - other.m_pointer;
				}

				T& operator * () const {
					return *m_pointer;
				}

				T* operator -> () {
					return m_pointer;
				}

			private:
				T* m_pointer;

			};

			Iterator begin();
			Iterator end();

			Iterator find(const T& value);

			Iterator insert(const Iterator it, const T& value);
			Iterator insert(const Iterator it, int n, const T& value);

			Iterator erase(const Iterator it);
			Iterator erase(const Iterator first, const Iterator last);

		private:
			bool is_basic_type();

		private:
			T* m_data;
			int m_size;
			int m_capacity;
		};

		template <typename T>
		Vector<T>::Vector() : m_data(nullptr), m_size(0), m_capacity(0) {}

		template<typename T>
		Vector<T>::Vector(std::initializer_list<T> init_list) : m_size(init_list.size()) {
			if (m_size == 0) {
				return;
			}
			else {
				m_capacity = 1;
				while (m_capacity < m_size) {
					m_capacity *= 2;
				}

				m_data = new T[m_capacity];
				int index = 0;
				for (const T& elem : init_list) {
					m_data[index++] = elem;
				}
			}
		}

		template <typename T>
		Vector<T>::~Vector() {
			if (m_data != nullptr) {
				delete[] m_data;
				m_data = nullptr;
			}
			m_size = 0;
			m_capacity = 0;
		}

		template<typename T>
		void Vector<T>::push_back(const T& value){
			if (m_size < m_capacity) {
				m_data[m_size] = value;
				m_size++;
				return;
			}

			if (m_capacity == 0) {
				m_capacity = 1;
			}
			else {
				m_capacity *= 2;
			}

			T* data = new T[m_capacity];

			for (int i = 0; i < m_size; i++) {
				data[i] = m_data[i];
			}

			if (m_data != nullptr) {
				delete[] m_data;
				m_data = nullptr;
			}

			m_data = data;
			m_data[m_size] = value;
			m_size++;
			return;
		}

		template <typename T>
		void Vector<T>::pop_back() {
			if (m_size > 0) {
				m_size--;
			}
		}

		template <typename T>
		size_t Vector<T>::capacity() const {
			return m_capacity;
		}

		template <typename T>
		size_t Vector<T>::size() const {
			return m_size;
		}

		template <typename T>
		void Vector<T>::show() const {
			std::cout << "Size=" << size() << ", capacity=" << capacity() << std::endl;
			for (int i = 0; i < m_size; i++) {
				std::cout << m_data[i] << ", ";
			}
			std::cout << std::endl;
		}

		template <typename T>
		void Vector<T>::reserve(int n) {
			if (m_capacity >= n) {
				return;
			}

			while (m_capacity < n) {
				if (m_capacity != 0) {
					m_capacity *= 2;
				}
				else {
					m_capacity = 1;
				}
			}
			T* data = new T[m_capacity];
			for (int i = 0; i < m_size; i++) {
				data[i] = m_data[i];
			}
			if (m_data != nullptr) {
				delete[] m_data;
				m_data = nullptr;
			}
			m_data = data;
		}

		template <typename T>
		void Vector<T>::resize(int n) {
			if (n <= m_size) {
				for (int i = n; i < m_size; i++) {
					m_data[i].~T();
				}
				m_size = n;
				return;
			}
			if (n <= m_capacity) {
				for (int i = m_size; i < n; i++) {
					m_data[i] = T();
				}
				m_size = n;
				return;
			}

			reserve(n);
			for (int i = m_size; i < n; i++) {
				m_data[i] = std::move(T());
			}
			m_size = n;
		}

		template <typename T>
		T& Vector<T>::at(int n) {
			if (n < 0 || n >= m_size) {
				throw std::out_of_range("");
			}

			return m_data[n];
		}

		template <typename T>
		T& Vector<T>::operator[](int n) {
			return at(n);
		}

		template <typename T>
		bool Vector<T>::empty() const {
			return m_size == 0;
		}

		template <typename T>
		void Vector<T>::clear() {
			resize(0);
		}

		template <typename T>
		T& Vector<T>::front() {
			return at(0);
		}

		template <typename T>
		T& Vector<T>::back() {
			return at(m_size - 1);
		}

		template<typename T>
		void Vector<T>::swap(Vector<T>& other) {
			T* data = other.m_data;
			int size = other.m_size;
			int capacity = other.m_capacity;

			other.m_data = m_data;
			other.m_size = m_size;
			other.m_capacity = m_capacity;

			m_data = data;
			m_size = size;
			m_capacity = capacity;
		}

		template <typename T>
		T* Vector<T>::data(){
			return m_data;
		}

		template <typename T>
		Vector<T>& Vector<T>::operator = (const Vector<T>& other){
			if (this == &other) {
				return *this;
			}

			if (m_data != nullptr) {
				delete[] m_data;
				m_data = nullptr;
			}
			if (other.m_capacity != 0) {
				m_data = new T[other.m_capacity];
			}
			m_size = other.m_size;
			m_capacity = other.m_capacity;

			for (int i = 0; i < m_size; i ++ ) {
				m_data[i] = other.m_data[i];
			}

			return *this;
		}

		template <typename T>
		typename Vector<T>::Iterator Vector<T>::begin() {
			Vector<T>::Iterator it(m_data);
			return it;
		}

		template <typename T>
		typename Vector<T>::Iterator Vector<T>::end() {
			Vector<T>::Iterator it(m_data + m_size);
			return it;
		}

		template <typename T>
		typename Vector<T>::Iterator Vector<T>::find(const T& value) {
			for (Vector<T>::Iterator it = begin(); it != end(); it++) {
				if (*it == value) {
					return it;
				}
			}
			return end();
		}

		template<typename T>
		typename Vector<T>::Iterator Vector<T>::insert(const Iterator it, const T& value)
		{
			return insert(it, 1, value);
		}

		template<typename T>
		typename Vector<T>::Iterator Vector<T>::insert(const Iterator it, int n, const T& value)
		{
			if (it - begin() < 0 || it - end() > 0) {
				throw std::out_of_range("");
			}

			int position = it - begin();

			if (m_size + n <= m_capacity) {

				if (is_basic_type()) {
					// memmove适用于内存重叠的情况，能显著提高性能
					std::memmove(m_data + position + n, m_data + position, (m_size - position) * sizeof(T));
				}
				else {
					for (int i = m_size - 1; i >= position; i--) {
						m_data[i + n] = m_data[i];
					}
				}

				std::fill(m_data + position, m_data + position + n, value);

				m_size += n;
				return it;
			}
			else {
				while (m_size + n > m_capacity) {
					if (m_capacity != 0) {
						m_capacity *= 2;
					}
					else {
						m_capacity = 1;
					}
				}
				T* data = new T[m_capacity];
				if (is_basic_type()) {
					// memcpy适用于内存不重叠的情况，能显著提高性能
					std::memcpy(data, m_data, sizeof(T) * position);
					std::fill(data + position, data + position + n, value);
					std::memcpy(data + position + n, m_data + position, sizeof(T) * (m_size - position));
				}
				else {
					for (int i = 0; i < position; i++) {
						data[i] = m_data[i];
					}

					for (int i = 0; i < n; i++) {
						data[position + i] = value;
					}

					for (int i = position; i < m_size; i++) {
						data[n + i] = m_data[i];
					}
				}
				
				if (m_data != nullptr) {
					delete[] m_data;
					m_data = nullptr;
				}

				m_data = data;
				m_size += n;
				return Vector<T>::Iterator(m_data + position);
			}
		}

		template<typename T>
		typename Vector<T>::Iterator Vector<T>::erase(const Iterator it) {
			if (it - begin() < 0 || it - end() >= 0) {
				throw std::out_of_range("");
			}

			if (end() - it == 1) {
				m_size--;
				return it;
			}

			int position = it - begin();
			if (is_basic_type()) {
				std::memmove(m_data + position, m_data + position + 1, sizeof(T) * (m_size - position - 1));
			}
			else {
				for (int i = position; i < m_size - 1; i++) {
					m_data[i] = m_data[i + 1];
				}
			}

			m_size--;
			return it;
		}

		template<typename T>
		typename Vector<T>::Iterator Vector<T>::erase(const Iterator first, const Iterator last){
			int f = first - begin();
			int l = last - begin();
			int n = l - f;

			if (is_basic_type()) {
				std::memmove(m_data + f, m_data + l, sizeof(T) * (m_size - l));
			}
			else {
				for (int i = l; i < m_size; i++) {
					m_data[f - l + i] = m_data[i];
				}
			}
			m_size -= n;
			return first;
		}

		template<typename T>
		bool Vector<T>::is_basic_type(){
			if (std::is_pointer<T>::value) {
				return true;
			}

			return (typeid(T) == typeid(char)) || (typeid(T) == typeid(unsigned char))
				|| (typeid(T) == typeid(short)) || (typeid(T) == typeid(unsigned short))
				|| (typeid(T) == typeid(int)) || (typeid(T) == typeid(unsigned int))
				|| (typeid(T) == typeid(long)) || (typeid(T) == typeid(unsigned long))
				|| (typeid(T) == typeid(long long)) || (typeid(T) == typeid(unsigned long long))
				|| (typeid(T) == typeid(bool)) || (typeid(T) == typeid(float))
				|| (typeid(T) == typeid(double));
		}
 	}
}