
#pragma once

#include <set>
#include <utility>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <tuple>
#include <aptk2/tools/jenkins_12bit.hxx>

namespace aptk
{

	typedef		unsigned 	VariableIndex;
	typedef		unsigned	ValueIndex;

	class ValuesTuple {
	public:

		typedef 	std::tuple< VariableIndex, ValueIndex > Entry;
		typedef		std::vector< Entry >										Container;

		ValuesTuple();
		ValuesTuple( size_t sz, bool preallocate = true );
		ValuesTuple( const ValuesTuple& other );
		ValuesTuple( ValuesTuple&& other );

		ValuesTuple&	operator=( ValuesTuple&& other ) {
			if ( this == &other ) return *this;
			elements = other.elements;
			return *this;
		}

		const ValuesTuple&	operator=( const ValuesTuple& other ) {
			if ( this == &other ) return *this;
			elements = other.elements;
			return *this;
		}

		~ValuesTuple();

		void	add( VariableIndex x, ValueIndex v ) {
			elements.push_back( std::make_tuple(x,v) );
		}

		void	set( unsigned i, VariableIndex x, ValueIndex v ) {
			elements[i] = std::make_tuple(x,v);
		}

		void	finish() {
			std::sort( elements.begin(), elements.end() );
		}

		//! Comparison operators
		inline bool operator==( const ValuesTuple& t ) const { return elements == t.elements; }
		inline bool operator!=( const ValuesTuple& t ) const { return !(operator==(t)); }
		inline bool operator< ( const ValuesTuple& t ) const { return elements < t.elements; }
		inline bool operator> ( const ValuesTuple& t ) const { return t.operator<(*this); }
		inline bool operator<=( const ValuesTuple& t ) const { return !(operator>(t)); }
		inline bool operator>=( const ValuesTuple& t ) const { return !(operator<(t)); }

		Container::iterator
		begin()	{ return elements.begin(); }

		Container::iterator
		end() { return elements.end(); }

		Container::const_iterator
		begin() const { return elements.begin(); }

		Container::const_iterator
		end() const { return elements.end(); }

		friend std::ostream& 	operator<<( std::ostream& stream, const ValuesTuple& t );

		class Hasher {
		public:
			std::size_t operator()( const ValuesTuple& t ) const {
				std::size_t hashcode = 0;
				for ( const Entry& e : t.elements ) {
					VariableIndex x;
					ValueIndex v;
					std::tie( x, v ) = e;
					hashcode = jenkins_hash( (ub1*)(&x), sizeof(VariableIndex), hashcode );
					hashcode = jenkins_hash( (ub1*)(&v), sizeof(ValueIndex), hashcode );
				}

				return hashcode;
			}
		};

	protected:

		Container	elements;
	};

	std::ostream& 	operator<<( std::ostream& stream, const ValuesTuple& t );


	typedef	std::set< ValuesTuple >	Tuple;

	class ValuesTupleIterator {
	public:

		ValuesTupleIterator( const std::vector<VariableIndex>& X, const std::vector<ValueIndex>& v, size_t sz );

		~ValuesTupleIterator();

		void	reset() {
			for ( unsigned k = 0; k < tuple_sz; k++ ) {
				offsets[k] = k;
				index[k] = offsets[k];
			}
			count = 0;
		}

		bool	next( ValuesTuple& t ) {
			/*
			for ( unsigned k = 0; k < tuple_sz; k++ )
				std::cout << index[k] << " ";
			std::cout << std::endl;
			*/
			for ( unsigned k = 0; k < tuple_sz; k++ ) {
				t.set( k, index[k], values[index[k]] );
			}
			index[tuple_sz-1]++;
			for ( unsigned k = tuple_sz-1; k > 0; k-- ) {
				if ( index[k] < vars.size() )
					break;
				index[k-1]++;
				for ( unsigned j = k; j < tuple_sz; j++ )
					index[j] = index[j-1]+1;//offsets[j];
			}
			count++;
			return index[0] < vars.size();
		}

		unsigned current_tuple_index() const { return count; }

	protected:
		const 	std::vector<VariableIndex>&			vars;
		const   std::vector<ValueIndex>&			values;
		size_t							tuple_sz;
		std::vector<unsigned>					offsets;
		std::vector<unsigned>					index;
		unsigned						count;
	};



}