// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * @file SimpleString.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifndef _FAST_DDS_GENERATED_SIMPLESTRING_H_
#define _FAST_DDS_GENERATED_SIMPLESTRING_H_


#include <stdint.h>
#include <array>
#include <string>
#include <vector>
#include <map>
#include <bitset>

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#define eProsima_user_DllExport __declspec( dllexport )
#else
#define eProsima_user_DllExport
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define eProsima_user_DllExport
#endif  // _WIN32

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#if defined(SimpleString_SOURCE)
#define SimpleString_DllAPI __declspec( dllexport )
#else
#define SimpleString_DllAPI __declspec( dllimport )
#endif // SimpleString_SOURCE
#else
#define SimpleString_DllAPI
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define SimpleString_DllAPI
#endif // _WIN32

namespace eprosima {
namespace fastcdr {
class Cdr;
} // namespace fastcdr
} // namespace eprosima


/*!
 * @brief This class represents the structure SimpleString defined by the user in the IDL file.
 * @ingroup SIMPLESTRING
 */
class SimpleString
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport SimpleString();

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~SimpleString();

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object SimpleString that will be copied.
     */
    eProsima_user_DllExport SimpleString(
            const SimpleString& x);

    /*!
     * @brief Move constructor.
     * @param x Reference to the object SimpleString that will be copied.
     */
    eProsima_user_DllExport SimpleString(
            SimpleString&& x);

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object SimpleString that will be copied.
     */
    eProsima_user_DllExport SimpleString& operator =(
            const SimpleString& x);

    /*!
     * @brief Move assignment.
     * @param x Reference to the object SimpleString that will be copied.
     */
    eProsima_user_DllExport SimpleString& operator =(
            SimpleString&& x);

    /*!
     * @brief Comparison operator.
     * @param x SimpleString object to compare.
     */
    eProsima_user_DllExport bool operator ==(
            const SimpleString& x) const;

    /*!
     * @brief Comparison operator.
     * @param x SimpleString object to compare.
     */
    eProsima_user_DllExport bool operator !=(
            const SimpleString& x) const;

    /*!
     * @brief This function sets a value in member index
     * @param _index New value for member index
     */
    eProsima_user_DllExport void index(
            uint32_t _index);

    /*!
     * @brief This function returns the value of member index
     * @return Value of member index
     */
    eProsima_user_DllExport uint32_t index() const;

    /*!
     * @brief This function returns a reference to member index
     * @return Reference to member index
     */
    eProsima_user_DllExport uint32_t& index();

    /*!
     * @brief This function copies the value in member data
     * @param _data New value to be copied in member data
     */
    eProsima_user_DllExport void data(
            const std::string& _data);

    /*!
     * @brief This function moves the value in member data
     * @param _data New value to be moved in member data
     */
    eProsima_user_DllExport void data(
            std::string&& _data);

    /*!
     * @brief This function returns a constant reference to member data
     * @return Constant reference to member data
     */
    eProsima_user_DllExport const std::string& data() const;

    /*!
     * @brief This function returns a reference to member data
     * @return Reference to member data
     */
    eProsima_user_DllExport std::string& data();

    /*!
     * @brief This function returns the maximum serialized size of an object
     * depending on the buffer alignment.
     * @param current_alignment Buffer alignment.
     * @return Maximum serialized size.
     */
    eProsima_user_DllExport static size_t getMaxCdrSerializedSize(
            size_t current_alignment = 0);

    /*!
     * @brief This function returns the serialized size of a data depending on the buffer alignment.
     * @param data Data which is calculated its serialized size.
     * @param current_alignment Buffer alignment.
     * @return Serialized size.
     */
    eProsima_user_DllExport static size_t getCdrSerializedSize(
            const SimpleString& data,
            size_t current_alignment = 0);


    /*!
     * @brief This function serializes an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    eProsima_user_DllExport void serialize(
            eprosima::fastcdr::Cdr& cdr) const;

    /*!
     * @brief This function deserializes an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    eProsima_user_DllExport void deserialize(
            eprosima::fastcdr::Cdr& cdr);



    /*!
     * @brief This function returns the maximum serialized size of the Key of an object
     * depending on the buffer alignment.
     * @param current_alignment Buffer alignment.
     * @return Maximum serialized size.
     */
    eProsima_user_DllExport static size_t getKeyMaxCdrSerializedSize(
            size_t current_alignment = 0);

    /*!
     * @brief This function tells you if the Key has been defined for this type
     */
    eProsima_user_DllExport static bool isKeyDefined();

    /*!
     * @brief This function serializes the key members of an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    eProsima_user_DllExport void serializeKey(
            eprosima::fastcdr::Cdr& cdr) const;

private:

    uint32_t m_index;
    std::string m_data;
};

#endif // _FAST_DDS_GENERATED_SIMPLESTRING_H_