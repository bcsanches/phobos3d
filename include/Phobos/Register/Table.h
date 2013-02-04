/*
Phobos 3d
September 2010
Copyright (c) 2005-2013 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_REGISTER_TABLE_H
#define PH_REGISTER_TABLE_H

#include <unordered_map>

#include <PH_Node.h>

#include "Phobos/Register/RegisterAPI.h"
#include "Phobos/Register/TableFwd.h"

namespace Phobos
{
	class Parser_c;

	namespace Register
	{
		/**
			The table is basically a key/value conteiner with some special capabilities:
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
		class PH_REGISTER_API Table_c: public Node_c
		{	
			//Not really needed, because node already blocks copies, but makes it easier to find errors
			PH_DISABLE_COPY(Table_c);

			private:
				enum ValueType_e
				{
					STRING,
					CHAR_MATRIX
				};			

				struct Value_s
				{
					ValueType_e eType;

					String_t strValue;

					UInt16_t u16NumRows;
					UInt16_t u16NumColumns;

					inline Value_s():
						eType(STRING),
						u16NumColumns(0),
						u16NumRows(0)
					{
						//empty
					}
					
					explicit inline Value_s(const String_t &value):
						strValue(value),
						eType(STRING),
						u16NumColumns(0),
						u16NumRows(0)
					{
						//empty
					}

					explicit inline Value_s(const String_t &value, UInt16_t numRows, UInt16_t numColumns):
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
				static TablePtr_t Create(const String_t &name);
				Table_c(const String_t &name);

				~Table_c();

				void Load(Parser_c &parser);

				const String_t &GetString(const String_t &key) const;
				bool TryGetString(const String_t &key, String_t &out) const;
				const String_t *TryGetString(const String_t &key) const;

				const MatrixDataHandle_c GetMatrix(const String_t &key) const;

				bool GetBool(const String_t &key) const;
				bool TryGetBool(bool &outValue, const String_t &key) const;

				void Get4Float(float values[4], const String_t &key) const;
				bool TryGet4Float(float values[4], const String_t &key) const;
				void Get3Float(float values[3], const String_t &key) const;
				int GetInt(const String_t &key) const;
				float GetFloat(const String_t &key) const;
				bool TryGetFloat(float &outValue, const String_t &key) const;

				void SetString(const String_t &key, const String_t &value);
				void SetCharMatrix(const String_t &key, const String_t &data, UInt16_t numRows, UInt16_t numColumns);
			
				const Table_c *GetInherited() const;

				void SetInherited(const String_t &base);
				void SetBaseHive(const String_t &baseHive);

			private:			
				static const String_t *TryGetString(const Table_c *current, const String_t &key);

				static const Value_s *TryGetValue(const Table_c *current, const String_t &key);
				static const Value_s &GetValue(const Table_c *current, const String_t &key);

				void ParseSpecialValue(const String_t &idName, Parser_c &parser);

				void CheckInvalidKey(const String_t &key, const char *keys[], const char *message) const;
				void CheckForKeyword(const String_t &key) const;		

				const Value_s *TryGetValue(const String_t &key) const;

			private:
				typedef std::unordered_map<String_t, Value_s> ValueMap_t;
				typedef ValueMap_t::const_iterator ValueMapConstIterator_t;

				ValueMap_t mapValues;

				String_t strInherit;
				String_t strBaseHive;
				mutable const Table_c *pclInherit;
		};	
	}
}

#endif
