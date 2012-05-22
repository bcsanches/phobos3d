/*
Phobos 3d
September 2010
Copyright (c) 2005-2011 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_DICTIONARY_H
#define PH_DICTIONARY_H

#include <boost/unordered_map.hpp>

#include <PH_Node.h>

#include "PH_DictionaryAPI.h"
#include "PH_DictionaryFwd.h"

namespace Phobos
{
	class Parser_c;

	/**
		The dictionary is basically a key/value conteiner with some special capabilities:
			* Supports a primitive inheritance: if a key of "inherit" is added, the value is considered a parent to this dictionary
				* On a value lookup, if the value is not found on the current dictionary data, the "inherit" dictionary is checked and its "inherit" too, 
				until the value is found or the chain terminates
			* Because dictionaries are stored on hives, the "base_hive" key is used to indicate the hive of the "inherit" dictionary.

		The fundamental supported value is string, but also a character matrix is supported.

		Also the dictionary can parses its data on the Load method, its expects something like:
		{
			key1 = value1;
			key2 = value2;

			//... etc
		}

		For defining a CharMatrix, use:
		{
			matrix1 = new CharMatrix(
				"####",
				"#..#",
				"####"
			);

			//... etc
		}

		The above exampel defines a matrix with 3 rows and 4 columns.
	*/
	class PH_DICTIONARY_API Dictionary_c: public Node_c
	{			
		private:
			enum ValueType_e
			{
				STRING,
				CHAR_MATRIX
			};			

			struct Value_s
			{
				ValueType_e eType;

				String_c strValue;

				UInt16_t u16NumRows;
				UInt16_t u16NumColumns;

				inline Value_s():
					eType(STRING),
					u16NumColumns(0),
					u16NumRows(0)
				{
					//empty
				}
					
				explicit inline Value_s(const String_c &value):
					strValue(value),
					eType(STRING),
					u16NumColumns(0),
					u16NumRows(0)
				{
					//empty
				}

				explicit inline Value_s(const String_c &value, UInt16_t numRows, UInt16_t numColumns):
					strValue(value),
					eType(CHAR_MATRIX),
					u16NumColumns(numColumns),
					u16NumRows(numRows)
				{
					//empty
				}

			};

		public:
			/**
				The internal structure that holds the data is not exposed, a MatrixHandle is provided for a "safe" and more simple data access.

			*/
			class MatrixDataHandle_c
			{
				private:
					const Value_s &rstValue;

				public:
					inline MatrixDataHandle_c(const Value_s &value):
						 rstValue(value)
					 {
						 //empty
					 }
				
					inline MatrixDataHandle_c(const MatrixDataHandle_c &rhs):
						rstValue(rhs.rstValue)
					{
						//empty
					}

					inline Char_t operator()(UInt16_t row, UInt16_t col) const
					{
						return rstValue.strValue[row * rstValue.u16NumColumns + col];
					}

					inline UInt16_t GetNumColumns() const
					{
						return rstValue.u16NumColumns;
					}

					inline UInt16_t GetNumRows() const
					{
						return rstValue.u16NumRows;
					}
			};	

		public:
			static DictionaryPtr_t Create(const String_c &name);

			~Dictionary_c();

			void Load(Parser_c &parser);

			const String_c &GetString(const String_c &key) const;
			bool TryGetString(const String_c &key, String_c &out) const;
			const String_c *TryGetString(const String_c &key) const;

			const MatrixDataHandle_c GetMatrix(const String_c &key) const;

			bool GetBool(const String_c &key) const;

			void Get4Float(float values[4], const String_c &key) const;
			void Get3Float(float values[3], const String_c &key) const;
			int GetInt(const String_c &key) const;
			float GetFloat(const String_c &key) const;

			void SetString(const String_c &key, const String_c &value);
			void SetCharMatrix(const String_c &key, const String_c &data, UInt16_t numRows, UInt16_t numColumns);
			
			const Dictionary_c *GetInherited() const;

			void SetInherited(const String_c &base);
			void SetBaseHive(const String_c &baseHive);

		private:
			Dictionary_c(const String_c &name);			

			static const String_c *TryGetString(const Dictionary_c *current, const String_c &key);

			static const Value_s *TryGetValue(const Dictionary_c *current, const String_c &key);
			static const Value_s &GetValue(const Dictionary_c *current, const String_c &key);

			void ParseSpecialValue(const String_c &idName, Parser_c &parser);

			void CheckInvalidKey(const String_c &key, const char *keys[], const char *message) const;
			void CheckForKeyword(const String_c &key) const;		

			const Value_s *TryGetValue(const String_c &key) const;

		private:
			typedef boost::unordered_map<String_c, Value_s> ValueMap_t;
			typedef ValueMap_t::const_iterator ValueMapConstIterator_t;

			ValueMap_t mapValues;

			String_c strInherit;
			String_c strBaseHive;
			mutable const Dictionary_c *pclInherit;
	};	
}

#endif
