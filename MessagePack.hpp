/*
 * MIT License
 *
 * Copyright (c) 2020 Christian Tost
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef MESSAGEPACK_HPP
#define MESSAGEPACK_HPP

#include <vector>
#include <string.h>
#include <stdint.h>
#include <string>
#include <type_traits>
#include <map>
#include <unordered_map>
#include <memory>
#include <stdexcept>

enum MsgFormats : unsigned char
{
    POSITIVE_FIXINT         = 0x00, //positivi fixint 0xxxxxxx 0x00 - 0x7f
    FIXMAP                  = 0x80, //fixmap 1000xxxx 0x80 - 0x8f
    FIXARRAY                = 0x90, //fixarray 1001xxxx 0x90 - 0x9f
    FIXSTR                  = 0xa0, //fixstr 101xxxxx 0xa0 - 0xbf
    NIL                     = 0xc0, //NIL
    RESERVED                = 0xc1, //Never used
    FALSE                   = 0xc2, //False
    TRUE                    = 0xc3, //True
    BIN8                    = 0xc4, //Bin 8
    BIN16                   = 0xc5, //Bin 16
    BIN32                   = 0xc6, //Bin 32
    EXT8                    = 0xc7, //Ext 8
    EXT16                   = 0xc8, //Ext 16
    EXT32                   = 0xc9, //Ext 32
    FLOAT32                 = 0xca, //Float 32
    FLOAT64                 = 0xcb, //Float 64
    UINT8                   = 0xcc, //Uint 8
    UINT16                  = 0xcd, //Uint 16
    UINT32                  = 0xce, //Uint 32
    UINT64                  = 0xcf, //Uint 64
    INT8                    = 0xd0, //Int 8
    INT16                   = 0xd1, //Int 16
    INT32                   = 0xd2, //Int 32
    INT64                   = 0xd3, //Int 64
    FIXEXT1                 = 0xd4, //Fixext 1
    FIXEXT2                 = 0xd5, //Fixext 2
    FIXEXT4                 = 0xd6, //Fixext 4
    FIXEXT8                 = 0xd7, //Fixext 8
    FIXEXT16                = 0xd8, //Fixext 16
    STR8                    = 0xd9, //Str 8
    STR16                   = 0xda, //Str 16
    STR32                   = 0xdb, //Str 32
    ARRAY16                 = 0xdc, //Array 16
    ARRAY32                 = 0xdd, //Array 32
    MAP16                   = 0xde, //Map 16
    MAP32                   = 0xdf, //Map 32
    NEGATIVE_FIXINT         = 0xe0  //Negative fixint 111xxxxx 0xe0 - 0xff
};

/**
 * @brief MessagePack exception type for faster exception processing.
 */
enum class MsgPackErrorType
{
    INVALID_CAST,       //!< Occurred if a type couldn't cast to the given one.
    EMPTY_STREAM,       //!< Occurred if now data is loaded.
    INVALID_FLOATING_POINT, //!< Occured if a float number is not completed.
    UNKNOWN_TYPE            //!< Occured if the type is unknown.
};

class CMsgPackException : public std::exception
{
    public:
        CMsgPackException() {}
        CMsgPackException(MsgPackErrorType Type) : m_ErrType(Type) {}
        CMsgPackException(const std::string &Msg, MsgPackErrorType Type) : m_Msg(Msg), m_ErrType(Type) {}

        const char *what() const noexcept override
        {
            return m_Msg.c_str();
        }

        MsgPackErrorType GetErrType() const noexcept
        {
            return m_ErrType;
        }

    private:
        std::string m_Msg;
        MsgPackErrorType m_ErrType;
};

class CMessagePack
{
    /**-----------------------------------------Blackmagic for SFINAE-----------------------------------------**/

    //Concept from https://dev.krzaq.cc/post/checking-whether-a-class-has-a-member-function-with-a-given-signature/
    template<class T>
    struct has_push_back
    {
        private:
            using Type = typename std::remove_pointer<T>::type;

            template<class C> static auto Test(typename C::value_type *p) -> decltype(std::declval<C>().push_back(*p), std::true_type()) { return std::true_type(); }
            template<class> static std::false_type Test(...) { return std::false_type(); }
        public:
            static const bool value = std::is_same<std::true_type, decltype(Test<Type>(nullptr))>::value;
    };

    template<class T>
    struct has_push_front
    {
        private:
            using Type = typename std::remove_pointer<T>::type;

            template<class C> static auto Test(typename C::value_type *p) -> decltype(std::declval<C>().push_front(*p), std::true_type()) { return std::true_type(); }
            template<class> static std::false_type Test(...) { return std::false_type(); }
        public:
            static const bool value = std::is_same<std::true_type, decltype(Test<Type>(nullptr))>::value;
    };

    template<class T>
    struct has_begin_end
    {
        private:
            using Type = typename std::remove_pointer<T>::type;

            template<class C> static auto TestBegin(typename C::const_iterator *) -> decltype(static_cast<typename C::const_iterator>(std::declval<C>().begin()), std::true_type())  { return std::true_type(); }
            template<class> static std::false_type TestBegin(...) { return std::false_type(); }

            template<class C> static auto TestEnd(typename C::const_iterator *) -> decltype(static_cast<typename C::const_iterator>(std::declval<C>().end()), std::true_type())  { return std::true_type(); }
            template<class> static std::false_type TestEnd(...) { return std::false_type(); }
        public:
            static const bool value = std::is_same<std::true_type, decltype(TestBegin<Type>(nullptr))>::value && std::is_same<std::true_type, decltype(TestEnd<Type>(nullptr))>::value;
    }; 

    template<class T>
    struct is_map : std::false_type {};

    template<class k, class v>
    struct is_map<std::map<k, v>> : std::true_type {};

    template<class k, class v>
    struct is_map<std::unordered_map<k, v>> : std::true_type {};

    template<class T>
    struct is_multimap : std::false_type {};

    template<class k, class v>
    struct is_multimap<std::multimap<k, v>> : std::true_type {};

    template<class k, class v>
    struct is_multimap<std::unordered_multimap<k, v>> : std::true_type {};

    template<class T>
    struct is_shared_ptr : std::false_type {};

    template<class T>
    struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

    template<class T>
    struct is_pointer_type
    {
        static const bool value = std::is_pointer<T>::value || is_shared_ptr<T>::value;
    };

    template<class T>
    struct pointer_type 
    {
        using type = typename std::remove_pointer<T>::type;
    };

    template<class T>
    struct pointer_type<std::shared_ptr<T>>
    {
        using type = T;
    };

    /**-----------------------------------------Blackmagic for SFINAE-----------------------------------------**/

    public:
        CMessagePack(/* args */) : m_Pairs(0), m_StreamPos(0) {}

        /**
         * @return Returns the serialized data stream.
         */
        inline std::vector<char> Serialize()
        {
            auto Ret = m_Data;
            Clear();
            return Ret;
        }

        inline std::vector<char> SerializeWithoutWipe()
        {
            auto Ret = m_Data;
            return Ret;
        }

        /**
         * @brief Loads a stream for deserialization.
         * 
         * @param Data: Stream to load.
         */
        inline void Deserialize(const std::vector<char> &Data)
        {
            m_Data = Data;
            m_StreamPos = 0;
        }

        /**
         * @brief Clears the messagepack.
         */
        void Clear()
        {
            m_Data.clear();
            m_StreamPos = 0;
            m_Pairs = 0;
        }

        /**
         * @brief Resets the position of the to the beginning of the stream.
         */
        void Reset()
        {
            m_StreamPos = 0;
        }

        /**
         * @brief Adds a value to the messagepack.
         * 
         * @param val: Value to add.
         */
        template<class T>
        inline void AddValue(const T &val)
        {
            ValueToMsgPack(val);
        }

        /**
         * @brief Adds an array to the output.
         * 
         * @param Size: Size of the array.
         */
        inline void AddArray(uint32_t Size)
        {
            if (Size <= FIXARRAY_MAX)
            {
                uint8_t Tmp = MsgFormats::FIXARRAY | (uint8_t)(FIXARRAY_MAX & Size); 
                m_Data.push_back((char)Tmp);
            }
            else if(Size <= UINT16_MAX)
            {
                m_Data.push_back(MsgFormats::ARRAY16);
                AddBytes((uint16_t)Size);
            }
            else if(Size <= UINT32_MAX)
            {
                m_Data.push_back(MsgFormats::ARRAY32);
                AddBytes(Size);
            }
        }

        /**
         * @brief Adds a map to the output.
         * 
         * @param Pairs: Number of the pairs.
         */
        inline void AddMap(uint32_t Pairs)
        {
            if (Pairs <= FIXMAP_MAX)
            {
                uint8_t Tmp = MsgFormats::FIXMAP | (uint8_t)(FIXMAP_MAX & Pairs); 
                m_Data.push_back((char)Tmp);
            }
            else if(Pairs <= UINT16_MAX)
            {
                m_Data.push_back(MsgFormats::MAP16);
                AddBytes((uint16_t)Pairs);
            }
            else if(Pairs <= UINT32_MAX)
            {
                m_Data.push_back(MsgFormats::MAP32);
                AddBytes(Pairs);
            }
        }

        /**
         * @brief Adds raw binary data to the output.
         * 
         * @param Data: Data to add.
         */
        inline void AddBin(const std::vector<char> &Data)
        {
            AddBin(Data.data(), Data.size());
        }

        /**
         * @brief Adds raw binary data to the output.
         * 
         * @param Data: Data to add.
         * @param Size: Size of the data in bytes.
         */
        inline void AddBin(const char *Data, uint32_t Size)
        {
            if(Size <= UINT8_MAX)
            {
                m_Data.push_back(MsgFormats::BIN8);
                AddBytes((uint8_t)Size);
            }
            else if(Size <= UINT16_MAX)
            {
                m_Data.push_back(MsgFormats::BIN16);
                AddBytes((uint16_t)Size);
            }
            else if(Size <= UINT32_MAX)
            {
                m_Data.push_back(MsgFormats::BIN32);
                AddBytes((uint32_t)Size);
            }

            for (uint32_t i = 0; i < Size; i++)
                m_Data.push_back(Data[i]);
        }

        /**
         * @brief Adds a key value pair to a map.
         * 
         * @param key: Key of the pair.
         * @param value: Value of the pair.
         */
        template<class k, class v>
        inline void AddPair(k key, v value)
        {
            m_Pairs++;
            ValueToMsgPack(key);
            ValueToMsgPack(value);
        }

        /**
         * @brief Get the next value of the stream.
         * 
         * @throw CMsgPackException If any error occurres.
         */
        template<class T>
        inline T GetValue()
        {
            return MsgPackToValue<T>();
        }

        /**
         * @return Returns the next type inside the stream.
         */
        inline MsgFormats GetNextType()
        {
            if(m_StreamPos < m_Data.size())
            {
                uint8_t c = m_Data[m_StreamPos];
                if((c & 0x80) == MsgFormats::POSITIVE_FIXINT)
                    return MsgFormats::POSITIVE_FIXINT;
                else if((c & 0xF0) == MsgFormats::FIXARRAY)
                    return MsgFormats::FIXARRAY;
                else if((c & 0xF0) == MsgFormats::FIXMAP)
                    return MsgFormats::FIXMAP;
                else if((c & 0xE0) == MsgFormats::FIXSTR)
                    return MsgFormats::FIXSTR;
                else if((c & 0xE0) == MsgFormats::NEGATIVE_FIXINT)
                    return MsgFormats::NEGATIVE_FIXINT;
                else
                    return (MsgFormats)c;
            }

            return MsgFormats::RESERVED;
        }

        /**
         * @return Unpacks an array and returns the size of the array. 
         */
        uint32_t UnpackArray()
        {
            CheckStreamPos();
            
            auto fmt = GetNextType();

            switch (fmt)
            {
                case MsgFormats::FIXARRAY:
                case MsgFormats::ARRAY16:
                case MsgFormats::ARRAY32:
                {
                    auto Size = GetSize();
                    SkipHeader();

                    return Size;
                }break;
            
                default:
                {
                    throw CMsgPackException(MsgPackErrorType::INVALID_CAST);
                }break;
            }
        }

        /**
         * @return Unpacks a map and returns the pair count of the map. 
         */
        uint32_t UnpackMap()
        {
            CheckStreamPos();
            
            auto fmt = GetNextType();

            switch (fmt)
            {
                case MsgFormats::FIXMAP:
                case MsgFormats::MAP16:
                case MsgFormats::MAP32:
                {
                    auto Size = GetSize();
                    SkipHeader();

                    return Size;
                }break;
            
                default:
                {
                    throw CMsgPackException(MsgPackErrorType::INVALID_CAST);
                }break;
            }
        }

        /**
         * @brief Skips the next value/-s
         * 
         * @param Count: Count of values to skip.
         * 
         * @throw Throws CMsgPackException if an unknown type occured.
         */
        inline void SkipValue(size_t Count = 1)
        {
            if(m_StreamPos < m_Data.size())
            {
                for (size_t i = 0; i < Count; i++)
                {
                    MsgFormats fmt = GetNextType();

                    switch (fmt)
                    {
                        case MsgFormats::POSITIVE_FIXINT:
                        case MsgFormats::NEGATIVE_FIXINT:
                        case MsgFormats::NIL:
                        case MsgFormats::FALSE:
                        case MsgFormats::TRUE:
                        {
                            SkipHeader();
                        }break;

                        case MsgFormats::FIXARRAY:
                        case MsgFormats::ARRAY16:
                        case MsgFormats::ARRAY32:
                        {
                            uint32_t Size = GetSize();
                            SkipHeader();

                            SkipValue(Size);
                        } break;

                        case MsgFormats::FIXMAP:
                        case MsgFormats::MAP16:
                        case MsgFormats::MAP32:
                        {
                            uint32_t Size = GetSize();
                            SkipHeader();

                            for (uint32_t i = 0; i < Size; i++)
                                SkipValue(2);   //Skips the next pairs.
                        } break;

                        case MsgFormats::FIXSTR:  
                        case MsgFormats::STR8:
                        case MsgFormats::STR16:
                        case MsgFormats::STR32:
                        case MsgFormats::UINT8:
                        case MsgFormats::UINT16:
                        case MsgFormats::UINT32:
                        case MsgFormats::UINT64:
                        case MsgFormats::INT8:
                        case MsgFormats::INT16:
                        case MsgFormats::INT32:
                        case MsgFormats::INT64:
                        case MsgFormats::FLOAT32:
                        case MsgFormats::FLOAT64:
                        case MsgFormats::BIN8:
                        case MsgFormats::BIN16:
                        case MsgFormats::BIN32:
                        {
                            m_StreamPos += GetSize();
                            SkipHeader();
                        }break;

                        default:
                        {
                            throw CMsgPackException(MsgPackErrorType::UNKNOWN_TYPE);
                        } break;
                    }

                    if(m_StreamPos >= m_Data.size())
                        break;
                }  
            }
        }

        ~CMessagePack() {}
    private:
        const static char POS_FIXINT_MAX = INT8_MAX;
        const static char NEG_FIXINT_MAX = -32;
        const static char FIXARRAY_MAX = 0xF;
        const static char FIXSTR_MAX = 0x1F;
        const static char FIXMAP_MAX = 0xF;

        std::vector<char> m_Data;
        uint32_t m_Pairs;
        size_t m_StreamPos;

        inline uint32_t GetSize()
        {
            MsgFormats fmt = GetNextType();
            uint32_t Ret = 0;
            size_t Pos = m_StreamPos;

            switch (fmt)
            {
                case MsgFormats::POSITIVE_FIXINT:
                case MsgFormats::NEGATIVE_FIXINT:
                case MsgFormats::NIL:
                case MsgFormats::FALSE:
                case MsgFormats::TRUE:
                {
                    Ret = 1;
                }break;

                case MsgFormats::FIXARRAY:
                case MsgFormats::FIXMAP:
                case MsgFormats::FIXSTR:  
                {
                    uint8_t c = m_Data[Pos];
                    Ret = c & (uint8_t)~fmt;
                }break;     

                case MsgFormats::UINT8:
                case MsgFormats::INT8:
                {
                    return 1;
                }break;

                case MsgFormats::UINT16:
                case MsgFormats::INT16:
                {
                    return 2;
                }break;

                case MsgFormats::UINT32:
                case MsgFormats::INT32:
                case MsgFormats::FLOAT32:
                {
                    return 4;
                }break;

                case MsgFormats::UINT64:
                case MsgFormats::INT64:
                case MsgFormats::FLOAT64:
                {
                    return 8;
                }break;

                case MsgFormats::STR8:
                case MsgFormats::BIN8:
                {
                    Ret = m_Data[++Pos];
                }break;

                case MsgFormats::STR16:
                case MsgFormats::BIN16:
                case MsgFormats::ARRAY16:
                case MsgFormats::MAP16:
                {
                    Ret = ReadInt<short>(++Pos, 2);
                }break;

                case MsgFormats::STR32:
                case MsgFormats::BIN32:
                case MsgFormats::ARRAY32:
                case MsgFormats::MAP32:
                {
                    Ret = ReadInt<int>(++Pos, 4);
                }break;
            }

            return Ret;
        }

        inline void SkipHeader()
        {
            MsgFormats fmt = GetNextType();
            switch (fmt)
            {
                case MsgFormats::POSITIVE_FIXINT:
                case MsgFormats::NEGATIVE_FIXINT:
                case MsgFormats::NIL:
                case MsgFormats::FALSE:
                case MsgFormats::TRUE:
                case MsgFormats::FIXARRAY:
                case MsgFormats::FIXMAP:
                case MsgFormats::FIXSTR:  
                case MsgFormats::UINT8:
                case MsgFormats::INT8:
                case MsgFormats::UINT16:
                case MsgFormats::INT16:
                case MsgFormats::UINT32:
                case MsgFormats::INT32:
                case MsgFormats::FLOAT32:
                case MsgFormats::UINT64:
                case MsgFormats::INT64:
                case MsgFormats::FLOAT64:
                {
                    m_StreamPos++;
                }break;     

                case MsgFormats::STR8:
                case MsgFormats::BIN8:
                {
                    m_StreamPos += 2;
                }break;

                case MsgFormats::STR16:
                case MsgFormats::BIN16:
                case MsgFormats::ARRAY16:
                case MsgFormats::MAP16:
                {
                    m_StreamPos += 3;
                }break;

                case MsgFormats::STR32:
                case MsgFormats::BIN32:
                case MsgFormats::ARRAY32:
                case MsgFormats::MAP32:
                {
                    m_StreamPos += 5;
                }break;
            }
        }

        template<class T>
        inline T ReadInt(size_t Pos, char Count)
        {
            T Ret = 0;

            for (size_t i = Pos; i < Pos + Count; i++)
            {
                if(i < m_Data.size())
                    Ret |= ((uint8_t)m_Data[i] << (8 * (i - Pos)));
            }

            return ChangeEndianess(Ret, Count);
        }

        template <class T>
        inline T ReadFloat(size_t Pos, size_t n)
        {
            T ret = 0;
            char *fc = (char*)&ret;
            std::vector<char> Float = ReadBytes(Pos, n);

            if(Float.size() < sizeof(T))
                throw CMsgPackException(MsgPackErrorType::INVALID_FLOATING_POINT);

            for (size_t i = 0; i < sizeof(T); i++)
                fc[i] = Float[i];
            
            return ChangeEndianess(ret, n);
        }

        inline std::vector<char> ReadBytes(size_t Pos, size_t n)
        {
            std::vector<char> Ret;

            for (size_t i = Pos; i < Pos + n; i++)
            {
                if(i < m_Data.size())
                    Ret.push_back(m_Data[i]);
            }

            return Ret;
        }

        inline bool IsLittleEndian()
        {
            int i = 1;
            return ((char*)&i)[0];
        }

        template<class T, typename std::enable_if<!std::is_floating_point<T>::value>::type * = nullptr>
        inline T ChangeEndianess(T val, int n)
        {
            T Ret = val;

            if(IsLittleEndian())
            {
                char *CVal = (char*)&val;
                Ret = 0;

                for (char i = 0; i < n; i++)
                    Ret = (Ret << 8) | (uint8_t)CVal[i];
            }

            return Ret;
        }

        template<class T, typename std::enable_if<std::is_floating_point<T>::value>::type * = nullptr>
        inline T ChangeEndianess(T val, int n)
        {
            T ret = val;

            if(IsLittleEndian())
            {
                char *CRet = (char*)&ret;
                char *CVal = (char*)&val;

                for (char i = 0; i < n; i++)
                {
                    CRet[n - i - 1] = CVal[i];
                }
            }

            return ret;
        }

        template<class T>
        inline void AddBytes(T val)
        {
            val = ChangeEndianess(val, sizeof(T));

            char *CVal = (char*)&val;
            for (char i = 0; i < sizeof(T); i++)
                m_Data.push_back(CVal[i]);
        }

        //----------------------------------------Serialization----------------------------------------

        template <class T, typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value && !std::is_unsigned<T>::value>::type* =nullptr>
        inline void ValueToMsgPack(T val)
        {
            if (val >= 0 && val <= POS_FIXINT_MAX)
            {
                uint8_t Tmp = MsgFormats::POSITIVE_FIXINT | (uint8_t)(0xFF & val);
                m_Data.push_back((char)Tmp);
            }
            else if(val < 0 && val >= NEG_FIXINT_MAX)
            {
                uint8_t Tmp = MsgFormats::NEGATIVE_FIXINT | (uint8_t)(0x1F & val);
                m_Data.push_back((char)Tmp);
            }
            else if (val >= INT8_MIN && val <= INT8_MAX)
            {
                m_Data.push_back(MsgFormats::INT8);
                m_Data.push_back((char)val);
            }
            else if (val >= INT16_MIN && val <= INT16_MAX)
            {
                m_Data.push_back(MsgFormats::INT16);
                AddBytes((short)val);
            }
            else if (val >= INT32_MIN && val <= INT32_MAX)
            {
                m_Data.push_back(MsgFormats::INT32);
                AddBytes((int)val);
            }
            else if (val >= INT64_MIN && val <= INT64_MAX)
            {
                m_Data.push_back(MsgFormats::INT64);
                AddBytes((int64_t)val);
            }
        }

        template <class T, typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value && std::is_unsigned<T>::value>::type* =nullptr>
        inline void ValueToMsgPack(T val)
        {
            if (val <= POS_FIXINT_MAX)
            {
                uint8_t Tmp = MsgFormats::POSITIVE_FIXINT | (uint8_t)(0xFF & val);
                m_Data.push_back((char)Tmp);
            }
            else if (val <= UINT8_MAX)
            {
                m_Data.push_back(MsgFormats::UINT8);
                m_Data.push_back((char)val);
            }
            else if (val <= UINT16_MAX)
            {
                m_Data.push_back(MsgFormats::UINT16);
                AddBytes((uint16_t)val);
            }
            else if (val <= UINT32_MAX)
            {
                m_Data.push_back(MsgFormats::UINT32);
                AddBytes((uint32_t)val);
            }
            else if (val <= UINT64_MAX)
            {
                m_Data.push_back(MsgFormats::UINT64);
                AddBytes((uint64_t)val);
            }
        }

        template<class T, typename std::enable_if<std::is_pointer<T>::value && std::is_same<typename std::remove_pointer<T>::type, char>::value>::type* = nullptr>
        inline void ValueToMsgPack(T val)
        {
            if(val)
                ValueToMsgPack(std::string(val));
            else
                ValueToMsgPack(nullptr);
        }
        
        inline void ValueToMsgPack(const std::string &Val)
        {
            if(Val.empty())
            {
                ValueToMsgPack(nullptr);
                return;
            }

            if(Val.size() <= FIXSTR_MAX)
            {
                uint8_t Tmp = MsgFormats::FIXSTR | (uint8_t)(FIXSTR_MAX & Val.size());
                m_Data.push_back((char)Tmp);
            }
            else if(Val.size() <= UINT8_MAX)
            {
                m_Data.push_back(MsgFormats::STR8);
                AddBytes((uint8_t)Val.size());
            }
            else if(Val.size() <= UINT16_MAX)
            {
                m_Data.push_back(MsgFormats::STR16);
                AddBytes((uint16_t)Val.size());
            }
            else if(Val.size() <= UINT32_MAX)
            {
                m_Data.push_back(MsgFormats::STR32);
                AddBytes((uint32_t)Val.size());
            }

            for (uint32_t i = 0; i < (uint32_t)Val.size(); i++)
                m_Data.push_back(Val[i]);
        }

        template<class T, typename std::enable_if<std::is_null_pointer<T>::value>::type* = nullptr>
        inline void ValueToMsgPack(T val)
        {
            m_Data.push_back(MsgFormats::NIL);
        }

        template<class T, typename std::enable_if<std::is_floating_point<T>::value>::type * = nullptr>
        inline void ValueToMsgPack(T val)
        {
            if(sizeof(T) == sizeof(float))
            {
                m_Data.push_back(MsgFormats::FLOAT32);
                AddBytes(val);
            }
            else if(sizeof(T) == sizeof(double) || (sizeof(double) == sizeof(float) && sizeof(T) == sizeof(long double)))
            {
                m_Data.push_back(MsgFormats::FLOAT64);
                AddBytes(val);
            }
        }

        template<class T, typename std::enable_if<!is_map<T>::value && !is_multimap<T>::value && has_begin_end<T>::value && !std::is_same<T, std::string>::value>::type* = nullptr>
        inline void ValueToMsgPack(const T &val)
        {
            AddArray(val.size());
            for (auto &&e : val)
                ValueToMsgPack(e);            
        }

        template <class T, typename std::enable_if<std::is_integral<T>::value && std::is_same<T, bool>::value>::type* =nullptr>
        inline void ValueToMsgPack(T val)
        {
            m_Data.push_back(val ? MsgFormats::TRUE : MsgFormats::FALSE);
        }

        template<class T, typename std::enable_if<!is_pointer_type<T>::value && (is_map<T>::value || is_multimap<T>::value)>::type* = nullptr>
        inline void ValueToMsgPack(const T &val)
        {
            AddMap(val.size());
            for (auto &&e : val)
            {
                ValueToMsgPack(e.first);
                ValueToMsgPack(e.second);
            }
        }

        template<class T, typename std::enable_if<!is_pointer_type<T>::value && !has_begin_end<T>::value && std::is_class<T>::value>::type* = nullptr>
        inline void ValueToMsgPack(const T &Obj)
        {
            static_assert(std::is_class<T>::value, "Please use structs or objects!");
            CMessagePack Tmp;
            Obj.Serialize(Tmp);

            AddMap(Tmp.m_Pairs);
            auto Data = Tmp.Serialize();
            m_Data.insert(m_Data.end(), Data.begin(), Data.end());
        }

        template<class T, typename std::enable_if<is_pointer_type<T>::value && !has_begin_end<T>::value && std::is_class<T>::value>::type* = nullptr>
        inline void ValueToMsgPack(const T Obj)
        {
            static_assert(std::is_class<T>::value, "Please use structs or objects!");
            CMessagePack Tmp;
            Obj->Serialize(Tmp);

            AddMap(Tmp.m_Pairs);
            auto Data = Tmp.Serialize();
            m_Data.insert(m_Data.end(), Data.begin(), Data.end());
        }

        template<class T, typename std::enable_if<is_pointer_type<T>::value && !has_begin_end<T>::value && std::is_class<typename pointer_type<T>::type>::value>::type* = nullptr>
        inline void ValueToMsgPack(const T obj)
        {
            static_assert(std::is_class<typename std::remove_pointer<T>::type>::value, "Please use structs or objects!");
            if(!obj)
                ValueToMsgPack(nullptr);
            else
                ValueToMsgPack(*obj);
        }

        //----------------------------------------Deserialization----------------------------------------

        inline void CheckStreamPos()
        {
            if(m_StreamPos >= m_Data.size())
                throw CMsgPackException(MsgPackErrorType::EMPTY_STREAM);
        }

        template<class T, typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value>::type * = nullptr>
        inline T MsgPackToValue()
        {
            CheckStreamPos();

            MsgFormats fmt = GetNextType();
            T Ret = 0;

            switch (fmt)
            {
                case MsgFormats::POSITIVE_FIXINT:
                {
                    Ret = (uint8_t)m_Data[m_StreamPos++] & (uint8_t)~fmt;
                }break;

                case MsgFormats::NEGATIVE_FIXINT:
                {
                    Ret = m_Data[m_StreamPos++];
                }break;

                case MsgFormats::INT8:
                case MsgFormats::INT16:
                case MsgFormats::INT32:
                case MsgFormats::INT64:
                case MsgFormats::UINT8:
                case MsgFormats::UINT16:
                case MsgFormats::UINT32:
                case MsgFormats::UINT64:
                {
                    uint32_t Size = GetSize();
                    SkipHeader();
                    Ret = ReadInt<T>(m_StreamPos, Size);
                    m_StreamPos += Size;
                }break;

                default:
                {
                    throw CMsgPackException(MsgPackErrorType::INVALID_CAST);
                }break;
            }

            return Ret;
        }

        template<class T, typename std::enable_if<std::is_same<T, bool>::value>::type * = nullptr>
        inline T MsgPackToValue()
        {
            CheckStreamPos();

            MsgFormats fmt = GetNextType();
            T Ret;

            switch (fmt)
            {
                case MsgFormats::TRUE:
                {
                    Ret = true;
                }break;

                case MsgFormats::FALSE:
                {
                    Ret = false;
                }break;
            
                default:
                {
                    throw CMsgPackException(MsgPackErrorType::INVALID_CAST);
                } break;
            }                

            m_StreamPos++;
            return Ret;
        }

        template<class T, typename std::enable_if<std::is_floating_point<T>::value>::type * = nullptr>
        inline T MsgPackToValue()
        {
            CheckStreamPos();

            MsgFormats fmt = GetNextType();
            T Ret;

            switch (fmt)
            {
                case MsgFormats::FLOAT32:
                case MsgFormats::FLOAT64:
                {
                    uint32_t Size = GetSize();
                    SkipHeader();
                    Ret = ReadFloat<T>(m_StreamPos, Size);
                    m_StreamPos += Size;
                }break;
            
                default:
                {
                    throw CMsgPackException(MsgPackErrorType::INVALID_CAST);
                } break;
            }     

            return Ret;
        }

        template<class T, typename std::enable_if<std::is_same<T, std::string>::value || (has_begin_end<T>::value && !is_multimap<T>::value && !is_map<T>::value && std::is_same<typename T::value_type, char>::value)>::type * = nullptr>
        inline T MsgPackToValue()
        {
            CheckStreamPos();

            MsgFormats fmt = GetNextType();
            T Ret;

            switch (fmt)
            {
                case MsgFormats::FIXSTR:
                case MsgFormats::STR8:
                case MsgFormats::STR16:
                case MsgFormats::STR32:
                case MsgFormats::BIN8:
                case MsgFormats::BIN16:
                case MsgFormats::BIN32:
                {
                    uint32_t Size = GetSize();
                    SkipHeader();
                    auto bytes = ReadBytes(m_StreamPos, Size);
                    Ret = T(bytes.begin(), bytes.end());
                    m_StreamPos += Size;
                }break;
            
                default:
                {
                    throw CMsgPackException(MsgPackErrorType::INVALID_CAST);
                } break;
            }     

            return Ret;
        }

        template<class T, typename std::enable_if<!std::is_same<T, std::string>::value && (has_begin_end<T>::value && !is_multimap<T>::value && !is_map<T>::value && !std::is_same<typename T::value_type, char>::value)>::type * = nullptr>
        inline T MsgPackToValue()
        {
            CheckStreamPos();

            MsgFormats fmt = GetNextType();
            T Ret;

            switch (fmt)
            {
                case MsgFormats::FIXARRAY:
                case MsgFormats::ARRAY16:
                case MsgFormats::ARRAY32:
                {
                    uint32_t Size = GetSize();
                    SkipHeader();

                    for (size_t i = 0; i < Size; i++)
                        Ret.push_back(MsgPackToValue<typename T::value_type>());
                }break;
            
                default:
                {
                    throw CMsgPackException(MsgPackErrorType::INVALID_CAST);
                } break;
            }     

            return Ret;
        }

        template<class T, typename std::enable_if<is_multimap<T>::value || is_map<T>::value>::type * = nullptr>
        inline T MsgPackToValue()
        {
            CheckStreamPos();

            MsgFormats fmt = GetNextType();
            T Ret;

            switch (fmt)
            {
                case MsgFormats::FIXMAP:
                case MsgFormats::MAP16:
                case MsgFormats::MAP32:
                {
                    uint32_t Size = GetSize();
                    SkipHeader();

                    for (size_t i = 0; i < Size; i++)
                    {
                        auto key = MsgPackToValue<typename T::key_type>();
                        auto val = MsgPackToValue<typename T::mapped_type>();

                        Ret.insert(std::pair<typename T::key_type, typename T::mapped_type>(key, val));
                    }
                }break;
            
                default:
                {
                    throw CMsgPackException(MsgPackErrorType::INVALID_CAST);
                } break;
            }     

            return Ret;
        }

        template<class T, typename std::enable_if<std::is_null_pointer<T>::value>::type * = nullptr>
        inline T MsgPackToValue()
        {
            CheckStreamPos();

            MsgFormats fmt = GetNextType();

            switch (fmt)
            {
                case MsgFormats::NIL:
                {
                    m_StreamPos++;
                    return nullptr;
                }break;
            
                default:
                {
                    throw CMsgPackException(MsgPackErrorType::INVALID_CAST);
                } break;
            }     
        }
};

#endif //MESSAGEPACK_HPP