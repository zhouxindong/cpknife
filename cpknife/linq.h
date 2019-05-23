#pragma once

/*
** linq in cpp
** defactor from boolinq@git.hub
** zhouxindong (13611050407@139.com)
*/

#include <set>
#include <list>
#include <deque>
#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <climits>
#include <unordered_set>
#include <unordered_map>

#include "exportapi.h"

namespace cpknife
{
#pragma region auxiliary

	/*
	** convenient detect iterator ending
	*/
	class _EnumeratorEnding {};

#pragma endregion

#pragma  region core

	/*
	** actually iterater
	*/
	template<typename TValue, typename TIter>
	class _Enumerator
	{
	private:
		std::function<TValue(TIter&)> _next;
		TIter _iter;

	public:
		typedef TValue value_type;

		_Enumerator(std::function<TValue(TIter&)> next, TIter iter)
			: _next(next)
			, _iter(iter)
		{
		}

		TValue next()
		{
			return _next(_iter);
		}
	};

	/*
	** linq class
	*/
	template<typename TEnumerator>
	class LinqImpl
	{
	private:
		TEnumerator* _enumerator;
		typedef typename TEnumerator::value_type value_type;
		void foreach_impl(std::function<void(value_type, int)> action) const;
	
	public:
		LinqImpl(TEnumerator* enumerator) : _enumerator(enumerator) {}
		virtual ~LinqImpl() { delete _enumerator; }

		/*
		** Linq method
		*/
		void foreach(std::function<void(value_type)> action) const
		{
			foreach_impl([&](value_type a, int) {return action(a); });
		}
	};

#pragma endregion

#pragma region linq api

	template<typename TValue, typename TIter>
	LinqImpl<_Enumerator<TValue, TIter>> from(
		TIter begin, 
		TIter end)
	{
		return new _Enumerator<TValue, TIter>([=](TIter& iter) {
			return (iter == end) ? throw _EnumeratorEnding() : *(iter++);
		}, begin);
	}

	/*
	** in-built array[]
	*/
	template<typename TValue, int n>
	auto from(TValue(&array)[n])
		-> decltype(from<TValue>(array, array + n))
	{
		return from<TValue>(array, array + n);
	}

	/*
	** array<typename T, size_t N>
	*/
	template<typename TValue, size_t N,
		template<typename, size_t> class TArray>
	auto from(const TArray<TValue,N> & container)
	    -> decltype(from<TValue>(std::begin(container), std::end(container)))
	{
	    return from<TValue>(std::begin(container), std::end(container));
	}

	/*
	** template<typename T,
	**			typename Allocator = allocator<T>>
	** sequence container
	*/
	template<typename T, typename Allocator = allocator<T>,
		template<typename, typename> class TSeqCtn>
	auto from(const TSeqCtn<T,Allocator> & container)
	    -> decltype(from<T>(std::begin(container), std::end(container)))
	{
	    return from<T>(std::begin(container), std::end(container));
	}

	/*
	** template<typename T,
	**			typename Compare = less<T>,
	**			typename Allocator = allocator<T>>
	** set
	*/
	template<
		typename T, 
		typename Compare = less<T>, 
		typename Allocator = allocator<T>,
		template<typename, typename, typename> class TSet>
	auto from(const TSet<T,Compare,Allocator> & container)
	    -> decltype(from<T>(std::begin(container), std::end(container)))
	{
	    return from<T>(std::begin(container), std::end(container));
	}

	/*
	** template<typename Key, typename T,
	**			typename Compare = less<Key>,
	**			typename Allocator = allocator<pair<const Key, T>>>
	** map
	*/
	template<
		typename Key, typename T, 
		typename Compare = less<T>, 
		typename Allocator = allocator<pair<const Key, T>>,
		template<typename, typename, typename, typename> class TMap>
	auto from_map(const TMap<Key,T,Compare,Allocator> & container)
	    -> decltype(from<std::pair<Key, T> >(std::begin(container), std::end(container)))
	{
	    return from<std::pair<Key,T> >(std::begin(container), std::end(container));
	}

	/*
	** template<typename T,
	**			typename Hash = hash<T>,
	**			typename EqPred = equal_to<T>,
	**			typename Allocator = allocator<T>>
	** unordered_set
	*/
	template<
		typename T,
		typename Hash = hash<T>,
		typename EqPred = equal_to<T>,
		typename Allocator = allocator<T>,
		template<typename, typename, typename, typename> class TUdSet>
	auto from_unordered_set(const TUdSet<T, Hash, EqPred, Allocator>& container)
		-> decltype(from<T>(std::begin(container), std::end(container)))
	{
		return from<T>(std::begin(container), std::end(container));
	}

	/*
	** template<typename Key, typename T,
	**			typename Hash = hash<T>,
	**			typename EqPred = equal_to<T>,
	**			typename Allocator = allocator<pair<const Key, T>>>
	** unordered_map
	*/
	template<
		typename Key, typename T,
		typename Hash = hash<T>,
		typename EqPred = equal_to<T>,
		typename Allocator = allocator<pair<const Key, T>>,
		template<typename, typename, typename, typename, typename> class TUdMap>
	auto from(const TUdMap<Key, T, Hash, EqPred, Allocator>& containter)
		-> decltype(from<std::pair<Key, T>>(std::begin(containter), std::end(containter)))
	{
		return from<std::pair<Key, T>>(std::begin(containter), std::end(containter));
	}

#pragma endregion

#pragma region linq method implement

	template<typename TEnumerator>
	inline void LinqImpl<TEnumerator>::foreach_impl(
		std::function<void(value_type, int)> action) const
	{
		int index = 0;
		try
		{
			while (true) action(_enumerator->next(), index++);
		}
		catch (_EnumeratorEnding &) {}
	}
#pragma endregion

}