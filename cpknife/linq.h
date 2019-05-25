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
#include <fstream>

using namespace std;

namespace cpknife
{
#pragma region auxiliary

	/*
	** convenient detect where no more data to get
	*/
	class _no_more_data {};

#pragma endregion

#pragma  region core

	/*
	** wrapped:
	** 1. a callable object that can get data one by one
	** 2. a pos to point current data position
	** <TValue>	 container element's type
	** <TPos>	 any type which can get a element value(a iterator or a _innter_enumerator...)
	*/
	template<typename TValue, typename TPos>
	class _inner_enumerator
	{
	private:
		std::function<TValue(TPos&)> _next; 									 
		TPos _pos;						 

	public:
		typedef typename TValue value_type;

		_inner_enumerator(std::function<TValue(TPos&)> next, TPos pos)
			: _next(next)
			, _pos(pos)
		{
		}

		TValue next()
		{
			return _next(_pos);
		}
	};

	/*
	** linq implement class
	** use _inner_enumerator object to get elements
	** the start point is from(), that return a LinqImpl object
	** which has a _inner_enumerator object can iterate all elements
	*/
	template<typename TEnumerator>
	class LinqImpl
	{
	private:
		TEnumerator* _enumerator;
		typedef typename TEnumerator::value_type value_type;

		/*
		** action() for each element
		** a indexer can be used
		*/
		void foreach_impl(std::function<void(value_type, int)> action) const
		{
			int index = 0;
			try
			{
				while (true) action(_enumerator->next(), index++);
			}
			catch (_no_more_data &) {}
		}

		/*
		** where(predicate) return predicate->true elements
		** return LinqImpl<> to support pipe call
		*/
		LinqImpl<_inner_enumerator<value_type, TEnumerator>>
			where_impl(std::function<bool(value_type, int)> predicate) const
		{
			static int index = 0;
			return new _inner_enumerator<value_type, TEnumerator>
				([=](TEnumerator & en) {
				value_type object = en.next(); 
				while (!predicate(object, index++)) {
					object = en.next();
				}
				return object;
			}, *_enumerator);
		}

	public:
		LinqImpl(TEnumerator* enumerator) : _enumerator(enumerator) {}
		virtual ~LinqImpl() { delete _enumerator; }

		/*
		** Linq API
		*/
		void foreach(std::function<void(value_type)> action) const
		{
			foreach_impl([&](value_type a, int) {return action(a); });
		}		

		LinqImpl<_inner_enumerator<value_type, TEnumerator>> 
			where(std::function<bool(value_type)> predicate) const
		{
			return where_impl([=](value_type a, int) {return predicate(a); });
		}

		/*
		** get n elements
		*/
		LinqImpl<_inner_enumerator<value_type, TEnumerator>> 
			take(int count) const
		{
			return new _inner_enumerator<value_type, TEnumerator>
				([&](TEnumerator & en) {
				while (count-- > 0) {
					return en.next();
				};
				throw _no_more_data();  // abort iterate
			}, *_enumerator);
		}

	private:

		template<typename TColl, typename TFunc>
		TColl _To_Coll(TFunc func) const 
		{
		    TColl container;
		    try
		    {
				while (true) func(container, _enumerator->next());
		    }
		    catch(_no_more_data &) {}
		    return container;
		}

	public:

		vector<value_type> ToVector() const
		{
			return _To_Coll<vector<value_type>>([](
				vector<value_type>& cnt, const value_type& val) {
				cnt.push_back(val);
			});
		}

		list<value_type> ToList() const
		{
			return _To_Coll<list<value_type>>([](
				list<value_type>& cnt, const value_type& val) {
				cnt.push_back(val);
			});
		}

		deque<value_type> ToDeque() const
		{
			return _To_Coll<deque<value_type>>([](
				deque<value_type>& cnt, const value_type& val) {
				cnt.push_back(val);
			});
		}

		set<value_type> ToSet() const
		{
			return _To_Coll<set<value_type>>([](
				set<value_type>& cnt, const value_type& val) {
				cnt.insert(val);
			});
		}
	};

#pragma endregion

#pragma region linq api

	template<typename TValue, typename TPos>
	LinqImpl<_inner_enumerator<TValue, TPos>> from(
		TPos begin,
		TPos end)
	{
		return new _inner_enumerator<TValue, TPos>([=](TPos& iter) {
			return (iter == end) ? throw _no_more_data() : *(iter++);
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
	auto from(const TArray<TValue, N> & container)
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
	auto from(const TSeqCtn<T, Allocator> & container)
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
	auto from(const TSet<T, Compare, Allocator> & container)
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
	auto from_map(const TMap<Key, T, Compare, Allocator> & container)
		-> decltype(from<std::pair<Key, T> >(std::begin(container), std::end(container)))
	{
		return from<std::pair<Key, T> >(std::begin(container), std::end(container));
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

}