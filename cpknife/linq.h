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
//#include "cpknife_exports.h"
#include <fstream>

namespace cpknife
{

#pragma region auxiliary

	/*
	** convenient detect where no more data to get
	*/
	class _INNER_Entor_End {};

	template<typename TI, typename TC>
	class _INNER_Iter_Cont_Pair
	{
		std::function<TI(const TC &)> get_ti;

	public:
		TC second;
		TI first;

		_INNER_Iter_Cont_Pair(const TC & tc, std::function<TI(const TC &)> get_ti_)
			: get_ti(get_ti_)
			, second(tc)
			, first(get_ti(second))
		{
		}

		_INNER_Iter_Cont_Pair(const _INNER_Iter_Cont_Pair<TI, TC> & pair)
			: get_ti(pair.get_ti)
			, second(pair.second)
			, first(get_ti(second))
		{
			for (auto it = pair.get_ti(pair.second); it != pair.first; ++it)
				first++;
		}
	};

	enum BytesDirection
	{
		FirstToLast,
		LastToFirst,
	};

	enum BitsDirection
	{
		HighToLow,
		LowToHigh,
	};

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
	class _INNER_Entor
	{
	private:
		std::function<TValue(TPos&)> _next; 									 
		TPos _pos;						 

	public:
		typedef typename TValue value_type;

		_INNER_Entor(std::function<TValue(TPos&)> next, TPos pos)
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
	template<typename TEntor>
	class Linqer
	{
	private:
		TEntor _entor;
		typedef typename TEntor::value_type value_type;

		template<typename TFunc, typename TArg>
		static auto get_return_type(TFunc * func = NULL, TArg * arg1 = NULL)
			-> decltype((*func)(*arg1));

		template<typename TFunc, typename T1Arg, typename T2Arg>
		static auto get_return_type(TFunc * func = NULL, T1Arg * arg1 = NULL, T2Arg * arg2 = NULL)
			-> decltype((*func)(*arg1, *arg2));

	public:
		Linqer(TEntor entor) : _entor(entor) {}

		value_type next() { return _entor.next(); }

		/*
		** action() for each element
		** a indexer can be used
		*/
		void _foreach(std::function<void(value_type, int)> action) const
		{
			auto entor = _entor; // why call directly error
			int index = 0;
			try
			{
				while (true) action(entor.next(), index++);
			}
			catch (_INNER_Entor_End &) {}
		}
		void foreach(std::function<void(value_type)> action) const
		{
			_foreach([=](value_type a, int) {return action(a); });
		}

		/*
		** where(predicate) return predicate->true elements
		** return LinqImpl<> to support pipe call
		*/
		Linqer<_INNER_Entor<value_type, std::pair<TEntor, int>>>
			_where(std::function<bool(value_type, int)> predicate) const
		{
			return _INNER_Entor<value_type, std::pair<TEntor, int> >([=](std::pair<TEntor, int> & pair)->value_type {
				value_type object;
				do
					object = pair.first.next();
				while (!predicate(object, pair.second++));
				return object;
			}, std::make_pair(_entor, 0));
		}

		Linqer<_INNER_Entor<value_type, std::pair<TEntor, int>>>
			where_(std::function<bool(value_type)> predicate) const
			{
				return _where([=](value_type a, int) {return predicate(a); });
			}

			/*
			** take n elements
			*/
			Linqer<_INNER_Entor<value_type, std::pair<TEntor, int>>>
				take(int count) const
			{
				return _where([=](value_type, int i) {
					if (i == count)
						throw _INNER_Entor_End();
					return true;
				});
			}

			/*
			** skip n elements
			*/
			Linqer<_INNER_Entor<value_type, std::pair<TEntor, int>>>
				skip(int count) const
			{
				return _where([=](value_type, int index) {
					return index >= count;
				});
			}

			/*
			** project
			*/
			template<typename TRet>
			Linqer<_INNER_Entor<TRet, std::pair<TEntor, int>>>
				_select(std::function<TRet(value_type, int)> transform) const
			{
				return _INNER_Entor<TRet, std::pair<TEntor, int>>
					([=](std::pair<TEntor, int>& pair)->TRet {
					return transform(pair.first.next(), pair.second++);
				}, make_pair(_entor, 0));
			}

			template<typename TFunc>
			Linqer<_INNER_Entor<decltype(get_return_type<TFunc, value_type, int>()),
				std::pair<TEntor, int> > > _select(TFunc transform) const
			{
				return _select<decltype(get_return_type<TFunc, value_type, int>())>(transform);
			}

			template<typename TRet>
			Linqer<_INNER_Entor<TRet, std::pair<TEntor, int>>>
				select(std::function<TRet(value_type)> transform) const
			{
				return _select<TRet>([=](value_type a, int) {return transform(a); });
			}

			template<typename TFunc>
			Linqer<_INNER_Entor<decltype(get_return_type<TFunc, value_type>()),
				std::pair<TEntor, int>>> select(TFunc transform) const
			{
				return select<decltype(get_return_type<TFunc, value_type>())>(transform);
			}

			template<typename TRet>
			Linqer<_INNER_Entor<TRet, std::pair<TEntor, int>>> cast() const
			{
				return _select<TRet>([=](value_type a, int)
				{ return a; });
			}

			template<typename TRet>
			Linqer<_INNER_Entor<value_type, std::pair<TEntor, std::set<TRet>>>>
				distinct(std::function<TRet(value_type)> transform) const
			{
				typedef std::pair<TEntor, std::set<TRet>> DataType;

				return _INNER_Entor<value_type, DataType>([=](DataType & pair)->value_type {
					for (;;)
					{
						value_type object = pair.first.next();
						TRet key = transform(object);
						if (pair.second.find(key) == pair.second.end())
						{
							pair.second.insert(key);
							return object;
						}
					}
				}, std::make_pair(_entor, std::set<TRet>()));
			}

			template<typename TFunc>
			Linqer<_INNER_Entor<value_type, std::pair<TEntor, std::set<decltype(get_return_type<TFunc, value_type>())>>>>
				distinct(TFunc transform) const
			{
				return distinct<decltype(get_return_type<TFunc, value_type>())>(transform);
			}

			Linqer<_INNER_Entor<value_type, std::pair<TEntor, std::set<value_type>>>>
				distinct() const
			{
				return distinct<value_type>([](value_type a) {return a; });
			}

	private:

		template<typename T, typename TRet>
		class _INNER_Trans_Comp
		{
		public:
			std::function<TRet(T)> func;
			_INNER_Trans_Comp(std::function<TRet(T)> func_) : func(func_) {}

			bool operator()(const T & a, const T & b) const
			{
				return func(a) < func(b);
			}
		};

	public:
	
		template<typename TRet>
		Linqer<_INNER_Entor<value_type, _INNER_Iter_Cont_Pair<typename std::multiset<value_type, _INNER_Trans_Comp<value_type, TRet> >::iterator,
			std::multiset<value_type, _INNER_Trans_Comp<value_type, TRet>>>>>
			order_by(std::function<TRet(value_type)> transform) const
		{
			typedef _INNER_Iter_Cont_Pair<typename std::multiset<value_type, _INNER_Trans_Comp<value_type, TRet> >::iterator,
				std::multiset<value_type, _INNER_Trans_Comp<value_type, TRet>>> DataType;

			std::multiset<value_type, _INNER_Trans_Comp<value_type, TRet> > objects(transform);
			try
			{
				auto en = _entor;
				for (;;)
					objects.insert(en.next());
			}
			catch (_INNER_Entor_End &) {}

			return _INNER_Entor<value_type, DataType>([](DataType & pair)
			{
				return (pair.first == pair.second.end())
					? throw _INNER_Entor_End() : *(pair.first++);
			}, DataType(objects, [](const std::multiset<value_type, _INNER_Trans_Comp<value_type, TRet>> & mset)
			{return mset.begin(); }));
		}

		template<typename TFunc>
		Linqer<_INNER_Entor<value_type, _INNER_Iter_Cont_Pair<typename std::multiset<value_type, _INNER_Trans_Comp<value_type, decltype(get_return_type<TFunc, value_type>())>>::iterator,
			std::multiset<value_type, _INNER_Trans_Comp<value_type, decltype(get_return_type<TFunc, value_type>())>>>>>
			order_by(TFunc transform) const
		{
			return order_by<decltype(get_return_type<TFunc, value_type>())>(transform);
		}

		Linqer<_INNER_Entor<value_type, _INNER_Iter_Cont_Pair<typename std::multiset<value_type, _INNER_Trans_Comp<value_type, value_type>>::iterator,
			std::multiset<value_type, _INNER_Trans_Comp<value_type, value_type>>>>> order_by() const
		{
			return order_by<value_type>([](value_type a) {return a; });
		}

		Linqer<_INNER_Entor<value_type, _INNER_Iter_Cont_Pair<typename std::vector<value_type>::const_reverse_iterator, std::vector<value_type>>>> 
			reverse() const
		{
			typedef _INNER_Iter_Cont_Pair<typename std::vector<value_type>::const_reverse_iterator, std::vector<value_type>> DataType;

			return _INNER_Entor<value_type, DataType>([](DataType & pair)
			{
				return (pair.first == pair.second.crend())
					? throw _INNER_Entor_End() : *(pair.first++);
			}, DataType(to_vector(), [](const std::vector<value_type> & vec) {return vec.crbegin(); }));
		}

		template<typename TRet>
		TRet aggregate(TRet start, std::function<TRet(TRet, value_type)> accumulate) const
		{
			try
			{
				auto en = _entor;
				for (;;)
					start = accumulate(start, en.next());
			}
			catch (_INNER_Entor_End &) {}
			return start;
		}

		template<typename TRet>
		TRet sum(std::function<TRet(value_type)> transform) const
		{
			return aggregate<TRet>(TRet(), [=](TRet accumulator, value_type object) {
				return accumulator + transform(object);
			});
		}

		template<typename TFunc>
		decltype(get_return_type<TFunc, value_type>()) sum(TFunc transform) const
		{
			return sum<decltype(get_return_type<TFunc, value_type>())>(transform);
		}

		template<typename TRet>
		TRet sum() const
		{
			return sum<TRet>([](value_type a) {return a; });
		}

		value_type sum() const
		{
			return sum<value_type>();
		}

		template<typename TRet>
		TRet avg(std::function<TRet(value_type)> transform) const
		{
			int count = 0;
			return aggregate<TRet>(TRet(), [&](TRet accumulator, value_type object)->TRet {
				count++;
				return (accumulator*(count - 1) + transform(object)) / count;
			});
		}

		template<typename TFunc>
		decltype(get_return_type<TFunc, value_type>()) avg(TFunc transform) const
		{
			return avg<decltype(get_return_type<TFunc, value_type>())>(transform);
		}

		template<typename TRet>
		TRet avg() const
		{
			return avg<TRet>([](value_type a) {return a; });
		}

		value_type avg() const
		{
			return avg<value_type>();
		}

		int count(std::function<bool(value_type)> predicate) const
		{
			return aggregate<int>(0, [=](int count, value_type a) 
			{return count + (predicate(a) ? 1 : 0); });
		}

		int count(const value_type & value) const
		{
			return count([=](value_type a) {return a == value; });
		}

		int count() const
		{
			return count([](value_type) {return true; });
		}

		bool any(std::function<bool(value_type)> predicate) const
		{
			try
			{
				auto en = _entor;
				for (;;)
					if (predicate(en.next()))
						return true;
			}
			catch (_INNER_Entor_End &) {}
			return false;
		}

		bool any() const
		{
			return any([](value_type a) {return static_cast<bool>(a); });
		}

		bool all(std::function<bool(value_type)> predicate) const
		{
			return !any([=](value_type a) {return !predicate(a); });
		}

		bool all() const
		{
			return all([](value_type a) {return static_cast<bool>(a); });
		}

		bool contains(const value_type & value) const
		{
			return any([&](value_type a) {return value == a; });
		}

		value_type elect(std::function<value_type(value_type, value_type)> accumulate) const
		{
			auto en = _entor;
			value_type result = en.next();
			try
			{
				for (;;)
					result = accumulate(result, en.next());
			}
			catch (_INNER_Entor_End &) {}
			return result;
		}

		template<typename TRet>
		value_type max_(std::function<TRet(value_type)> transform) const
		{
			return elect([=](value_type a, value_type b) {return transform(a) < transform(b) ? b : a; });
		}

		template<typename TFunc>
		value_type max_(TFunc transform) const
		{
			return max_<decltype(get_return_type<TFunc, value_type>())>(transform);
		}

		value_type max_() const
		{
			return max_<value_type>([](value_type a) {return a; });
		}

		template<typename TRet>
		value_type min_(std::function<TRet(value_type)> transform) const
		{
			return elect([=](value_type a, value_type b) {return transform(a) < transform(b) ? a : b; });
		}

		template<typename TFunc>
		value_type min_(TFunc transform) const
		{
			return min_<decltype(get_return_type<TFunc, value_type>())>(transform);
		}

		value_type min_() const
		{
			return min_<value_type>([](value_type a) {return a; });
		}

		value_type element_at(int index) const
		{
			auto en = _entor;
			for (int i = 0; i < index; i++)
				en.next();
			return en.next();
		}

		value_type first_(std::function<bool(value_type)> predicate) const
		{
			return where_(predicate).next();
		}

		value_type first_() const
		{
			return first_([](value_type) {return true; });
		}

		value_type firstOrDefault(std::function<bool(value_type)> predicate)
		{
			try { return first_(predicate); }
			catch (_INNER_Entor_End &) { return value_type(); }
		}

		value_type firstOrDefault() const
		{
			try { return first_(); }
			catch (_INNER_Entor_End &) { return value_type(); }
		}

		value_type last_(std::function<bool(value_type)> predicate) const
		{
			auto linq = where(predicate);
			value_type object = linq._entor.next();
			try { for (;;) object = linq._entor.next(); }
			catch (_INNER_Entor_End &) { return object; }
		}

		value_type last_() const
		{
			return last_([](value_type) {return true; });
		}

		value_type lastOrDefault(std::function<bool(value_type)> predicate) const
		{
			try { return last_(predicate); }
			catch (_INNER_Entor_End &) { return value_type(); }
		}

		value_type lastOrDefault() const
		{
			return lastOrDefault([](value_type) {return true; });
		}

	private:

		template<typename TColl, typename TFunc>
		TColl _To_Coll(TFunc func) const 
		{
		    TColl container;
		    try
		    {
				auto entor = _entor;
				while (true) func(container, entor.next());
		    }
		    catch(_INNER_Entor_End &) {}
		    return container;
		}

	public:

		std::vector<value_type> to_vector() const
		{
			return _To_Coll<vector<value_type>>([](
				vector<value_type>& cnt, const value_type& val) {
				cnt.push_back(val);
			});
		}

		std::list<value_type> to_list() const
		{
			return _To_Coll<list<value_type>>([](
				list<value_type>& cnt, const value_type& val) {
				cnt.push_back(val);
			});
		}

		std::deque<value_type> to_deque() const
		{
			return _To_Coll<deque<value_type>>([](
				deque<value_type>& cnt, const value_type& val) {
				cnt.push_back(val);
			});
		}

		std::set<value_type> to_set() const
		{
			return _To_Coll<set<value_type>>([](
				set<value_type>& cnt, const value_type& val) {
				cnt.insert(val);
			});
		}
	};

#pragma endregion

#pragma region linq api

	/*
	** linq start point
	*/
	template<typename TValue, typename TPos>
	Linqer<_INNER_Entor<TValue, TPos>> from(
		TPos begin,
		TPos end)
	{
		return _INNER_Entor<TValue, TPos>([=](TPos& iter) {
			return (iter == end) ? throw _INNER_Entor_End() : *(iter++);
		}, begin);
	}

	template<typename TValue, typename TPos>
	Linqer<_INNER_Entor<TValue, std::pair<TPos, int>>> from(TPos begin, int length)
	{
		return _INNER_Entor<TValue, std::pair<TPos, int> >([=](std::pair<TPos, int> & pair) {
			return (pair.second++ == length) ? throw _INNER_Entor_End() : *(pair.first++);
		}, std::make_pair(begin, 0));
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