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

#include <iostream>
#include "MessagePack.hpp"
#include <fstream>
#include <iomanip>
#include <bitset>
#include "CTest.hpp"

using namespace std;

class CTest1
{
    public:
        CTest1(/* args */) {}

        void Serialize(CMessagePack &Pack) const
        {
            Pack.AddPair(12, 345);
            Pack.AddPair("Test", 123);

            std::vector<int> Vec = {5, 6, 7, 8};
            Pack.AddPair(123, Vec);
            Pack.AddPair(Vec, "789");

            char *c = nullptr;
            Pack.AddPair("Nil", c);
        }

        ~CTest1() {}
    private:
        /* data */
};

class CTest
{
    public:
        CTest(/* args */) {}

        void Serialize(CMessagePack &Pack) const
        {
            Pack.AddPair(12, 345);
            Pack.AddPair("Test", 123);

            std::vector<int> Vec = {5, 6, 7, 8};
            Pack.AddPair(123, Vec);
            Pack.AddPair(Vec, "789");

            char *c = nullptr;
            Pack.AddPair("Nil", c);

            CTest1 *tt = new CTest1();
            Pack.AddPair("CTest1", tt);
            Pack.AddPair(tt, "CTest1");

            delete tt;
        }

        ~CTest() {}
    private:
        /* data */
};

void PrintMsgFormats(MsgFormats val)
{
	switch(val)
	{
		case MsgFormats::POSITIVE_FIXINT:
		{
			cout << "MsgFormats::POSITIVE_FIXINT" << endl;
		}break;

		case MsgFormats::FIXMAP:
		{
			cout << "MsgFormats::FIXMAP" << endl;
		}break;

		case MsgFormats::FIXARRAY:
		{
			cout << "MsgFormats::FIXARRAY" << endl;
		}break;

		case MsgFormats::FIXSTR:
		{
			cout << "MsgFormats::FIXSTR" << endl;
		}break;

		case MsgFormats::NIL:
		{
			cout << "MsgFormats::NIL" << endl;
		}break;

		case MsgFormats::RESERVED:
		{
			cout << "MsgFormats::RESERVED" << endl;
		}break;

		case MsgFormats::FALSE:
		{
			cout << "MsgFormats::FALSE" << endl;
		}break;

		case MsgFormats::TRUE:
		{
			cout << "MsgFormats::TRUE" << endl;
		}break;

		case MsgFormats::BIN8:
		{
			cout << "MsgFormats::BIN8" << endl;
		}break;

		case MsgFormats::BIN16:
		{
			cout << "MsgFormats::BIN16" << endl;
		}break;

		case MsgFormats::BIN32:
		{
			cout << "MsgFormats::BIN32" << endl;
		}break;

		case MsgFormats::EXT8:
		{
			cout << "MsgFormats::EXT8" << endl;
		}break;

		case MsgFormats::EXT16:
		{
			cout << "MsgFormats::EXT16" << endl;
		}break;

		case MsgFormats::EXT32:
		{
			cout << "MsgFormats::EXT32" << endl;
		}break;

		case MsgFormats::FLOAT32:
		{
			cout << "MsgFormats::FLOAT32" << endl;
		}break;

		case MsgFormats::FLOAT64:
		{
			cout << "MsgFormats::FLOAT64" << endl;
		}break;

		case MsgFormats::UINT8:
		{
			cout << "MsgFormats::UINT8" << endl;
		}break;

		case MsgFormats::UINT16:
		{
			cout << "MsgFormats::UINT16" << endl;
		}break;

		case MsgFormats::UINT32:
		{
			cout << "MsgFormats::UINT32" << endl;
		}break;

		case MsgFormats::UINT64:
		{
			cout << "MsgFormats::UINT64" << endl;
		}break;

		case MsgFormats::INT8:
		{
			cout << "MsgFormats::INT8" << endl;
		}break;

		case MsgFormats::INT16:
		{
			cout << "MsgFormats::INT16" << endl;
		}break;

		case MsgFormats::INT32:
		{
			cout << "MsgFormats::INT32" << endl;
		}break;

		case MsgFormats::INT64:
		{
			cout << "MsgFormats::INT64" << endl;
		}break;

		case MsgFormats::FIXEXT1:
		{
			cout << "MsgFormats::FIXEXT1" << endl;
		}break;

		case MsgFormats::FIXEXT2:
		{
			cout << "MsgFormats::FIXEXT2" << endl;
		}break;

		case MsgFormats::FIXEXT4:
		{
			cout << "MsgFormats::FIXEXT4" << endl;
		}break;

		case MsgFormats::FIXEXT8:
		{
			cout << "MsgFormats::FIXEXT8" << endl;
		}break;

		case MsgFormats::FIXEXT16:
		{
			cout << "MsgFormats::FIXEXT16" << endl;
		}break;

		case MsgFormats::STR8:
		{
			cout << "MsgFormats::STR8" << endl;
		}break;

		case MsgFormats::STR16:
		{
			cout << "MsgFormats::STR16" << endl;
		}break;

		case MsgFormats::STR32:
		{
			cout << "MsgFormats::STR32" << endl;
		}break;

		case MsgFormats::ARRAY16:
		{
			cout << "MsgFormats::ARRAY16" << endl;
		}break;

		case MsgFormats::ARRAY32:
		{
			cout << "MsgFormats::ARRAY32" << endl;
		}break;

		case MsgFormats::MAP16:
		{
			cout << "MsgFormats::MAP16" << endl;
		}break;

		case MsgFormats::MAP32:
		{
			cout << "MsgFormats::MAP32" << endl;
		}break;

		case MsgFormats::NEGATIVE_FIXINT:
		{
			cout << "MsgFormats::NEGATIVE_FIXINT" << endl;
		}break;

	}
}

std::string MsgFormatsToString(MsgFormats val)
{
	switch(val)
	{
		case MsgFormats::POSITIVE_FIXINT:
		{
			return "MsgFormats::POSITIVE_FIXINT";
		}break;

		case MsgFormats::FIXMAP:
		{
			return "MsgFormats::FIXMAP";
		}break;

		case MsgFormats::FIXARRAY:
		{
			return "MsgFormats::FIXARRAY";
		}break;

		case MsgFormats::FIXSTR:
		{
			return "MsgFormats::FIXSTR";
		}break;

		case MsgFormats::NIL:
		{
			return "MsgFormats::NIL";
		}break;

		case MsgFormats::RESERVED:
		{
			return "MsgFormats::RESERVED";
		}break;

		case MsgFormats::FALSE:
		{
			return "MsgFormats::FALSE";
		}break;

		case MsgFormats::TRUE:
		{
			return "MsgFormats::TRUE";
		}break;

		case MsgFormats::BIN8:
		{
			return "MsgFormats::BIN8";
		}break;

		case MsgFormats::BIN16:
		{
			return "MsgFormats::BIN16";
		}break;

		case MsgFormats::BIN32:
		{
			return "MsgFormats::BIN32";
		}break;

		case MsgFormats::EXT8:
		{
			return "MsgFormats::EXT8";
		}break;

		case MsgFormats::EXT16:
		{
			return "MsgFormats::EXT16";
		}break;

		case MsgFormats::EXT32:
		{
			return "MsgFormats::EXT32";
		}break;

		case MsgFormats::FLOAT32:
		{
			return "MsgFormats::FLOAT32";
		}break;

		case MsgFormats::FLOAT64:
		{
			return "MsgFormats::FLOAT64";
		}break;

		case MsgFormats::UINT8:
		{
			return "MsgFormats::UINT8";
		}break;

		case MsgFormats::UINT16:
		{
			return "MsgFormats::UINT16";
		}break;

		case MsgFormats::UINT32:
		{
			return "MsgFormats::UINT32";
		}break;

		case MsgFormats::UINT64:
		{
			return "MsgFormats::UINT64";
		}break;

		case MsgFormats::INT8:
		{
			return "MsgFormats::INT8";
		}break;

		case MsgFormats::INT16:
		{
			return "MsgFormats::INT16";
		}break;

		case MsgFormats::INT32:
		{
			return "MsgFormats::INT32";
		}break;

		case MsgFormats::INT64:
		{
			return "MsgFormats::INT64";
		}break;

		case MsgFormats::FIXEXT1:
		{
			return "MsgFormats::FIXEXT1";
		}break;

		case MsgFormats::FIXEXT2:
		{
			return "MsgFormats::FIXEXT2";
		}break;

		case MsgFormats::FIXEXT4:
		{
			return "MsgFormats::FIXEXT4";
		}break;

		case MsgFormats::FIXEXT8:
		{
			return "MsgFormats::FIXEXT8";
		}break;

		case MsgFormats::FIXEXT16:
		{
			return "MsgFormats::FIXEXT16";
		}break;

		case MsgFormats::STR8:
		{
			return "MsgFormats::STR8";
		}break;

		case MsgFormats::STR16:
		{
			return "MsgFormats::STR16";
		}break;

		case MsgFormats::STR32:
		{
			return "MsgFormats::STR32";
		}break;

		case MsgFormats::ARRAY16:
		{
			return "MsgFormats::ARRAY16";
		}break;

		case MsgFormats::ARRAY32:
		{
			return "MsgFormats::ARRAY32";
		}break;

		case MsgFormats::MAP16:
		{
			return "MsgFormats::MAP16";
		}break;

		case MsgFormats::MAP32:
		{
			return "MsgFormats::MAP32";
		}break;

		case MsgFormats::NEGATIVE_FIXINT:
		{
			return "MsgFormats::NEGATIVE_FIXINT";
		}break;

	}
	return "Unknown";
}

void PrintMsgPackErrorType(MsgPackErrorType val)
{
	switch(val)
	{
		case MsgPackErrorType::INVALID_CAST:
		{
			cout << "MsgPackErrorType::INVALID_CAST" << endl;
		}break;

		case MsgPackErrorType::EMPTY_STREAM:
		{
			cout << "MsgPackErrorType::EMPTY_STREAM" << endl;
		}break;

		case MsgPackErrorType::INVALID_FLOATING_POINT:
		{
			cout << "MsgPackErrorType::INVALID_FLOATING_POINT" << endl;
		}break;

	}
}

std::string MsgPackErrorTypeToString(MsgPackErrorType val)
{
	switch(val)
	{
		case MsgPackErrorType::INVALID_CAST:
		{
			return "MsgPackErrorType::INVALID_CAST";
		}break;

		case MsgPackErrorType::EMPTY_STREAM:
		{
			return "MsgPackErrorType::EMPTY_STREAM";
		}break;

		case MsgPackErrorType::INVALID_FLOATING_POINT:
		{
			return "MsgPackErrorType::INVALID_FLOATING_POINT";
		}break;

	}
	return "Unknown";
}

CMessagePack Pack;

void SaveTestMsgPack(const std::string &Name)
{
	ofstream out(Name, ios::binary);
    auto val = Pack.SerializeWithoutWipe();

    out.write((char*)val.data(), val.size());
    out.close();
}

void TestSerialPrimitives()
{
	Pack.AddValue(nullptr);

	Pack.AddValue(true);
	Pack.AddValue(false);

	Pack.AddValue(1);
	Pack.AddValue(-31);

	Pack.AddValue((uint8_t)0xFF);
	Pack.AddValue((uint16_t)0xFFFF);
	Pack.AddValue((uint32_t)0xFFFFFFFF);
	Pack.AddValue((uint64_t)0xFFFFFFFFFFFFFFFF);

	Pack.AddValue((int8_t)-127);
	Pack.AddValue((int16_t)(0xFFFF / 2 + 1));
	Pack.AddValue((int32_t)(0xFFFFFFFF / 2 + 1));
	Pack.AddValue((int64_t)(0xFFFFFFFFFFFFFFFF / 2 + 1));

	Pack.AddValue((float)1.6);
	Pack.AddValue((double)1.8);

	Pack.AddValue("Hallo Welt!");
	Pack.AddValue("123456789012345678901234567890345678");

	const char Data[] = {5, 2, 1, 5, 7};
	Pack.AddBin(Data, sizeof(Data));

	std::vector<int> vec = {10, 11, -21, 243};
	Pack.AddValue(vec);

	std::map<int, std::string> map = {{1, "Test"}, {2, "Hallo"}, {3, "Hallo Test"}};
	Pack.AddValue(map);

	std::multimap<int, std::string> cmmap = {{1, "Test1"}, {1, "Hallo1"}, {3, "Hallo Test1"}};
	Pack.AddValue(cmmap);	

	// SaveTestMsgPack("TestSerialPrimitives.mpack");
}

void TestDeserialPrimitives()
{
	std::function<std::string(MsgFormats)> fn = std::bind(MsgFormatsToString, std::placeholders::_1);

	CT::Check("Typecheck null", Pack.GetNextType(), MsgFormats::NIL, fn);
	CT::Check("Check value", Pack.GetValue<nullptr_t>(), nullptr);

	CT::Check("Typecheck true", Pack.GetNextType(), MsgFormats::TRUE, fn);
	CT::Check("Check value", Pack.GetValue<bool>(), true);

	CT::Check("Typecheck false", Pack.GetNextType(), MsgFormats::FALSE, fn);
	CT::Check("Check value", Pack.GetValue<bool>(), false);

	CT::Check("Typecheck posfixint", Pack.GetNextType(), MsgFormats::POSITIVE_FIXINT, fn);
	CT::Check("Check value", Pack.GetValue<int>(), 1);

	CT::Check("Typecheck negfixint", Pack.GetNextType(), MsgFormats::NEGATIVE_FIXINT, fn);
	CT::Check("Check value", Pack.GetValue<int>(), -31);

	CT::Check("Typecheck uint8", Pack.GetNextType(), MsgFormats::UINT8, fn);
	CT::Check("Check value", Pack.GetValue<uint8_t>(), (uint8_t)0xFF);

	CT::Check("Typecheck uint16", Pack.GetNextType(), MsgFormats::UINT16, fn);
	CT::Check("Check value", Pack.GetValue<uint16_t>(), (uint16_t)0xFFFF);

	CT::Check("Typecheck uint32", Pack.GetNextType(), MsgFormats::UINT32, fn);
	CT::Check("Check value", Pack.GetValue<uint32_t>(), 0xFFFFFFFF);

	CT::Check("Typecheck uint64", Pack.GetNextType(), MsgFormats::UINT64, fn);
	CT::Check("Check value", Pack.GetValue<uint64_t>(), 0xFFFFFFFFFFFFFFFF);

	CT::Check("Typecheck int8", Pack.GetNextType(), MsgFormats::INT8, fn);
	CT::Check("Check value", Pack.GetValue<int8_t>(), (int8_t)-127);

	CT::Check("Typecheck int16", Pack.GetNextType(), MsgFormats::INT16, fn);
	CT::Check("Check value", Pack.GetValue<int16_t>(), (int16_t)(0xFFFF / 2 + 1));

	CT::Check("Typecheck int32", Pack.GetNextType(), MsgFormats::INT32, fn);
	CT::Check("Check value", Pack.GetValue<int32_t>(), (int32_t)(0xFFFFFFFF / 2 + 1));

	CT::Check("Typecheck int64", Pack.GetNextType(), MsgFormats::INT64, fn);
	CT::Check("Check value", Pack.GetValue<int64_t>(), (int64_t)(0xFFFFFFFFFFFFFFFF / 2 + 1));

	CT::Check("Typecheck float", Pack.GetNextType(), MsgFormats::FLOAT32, fn);
	CT::Check("Check value", Pack.GetValue<float>(), (float)1.6);

	CT::Check("Typecheck double", Pack.GetNextType(), MsgFormats::FLOAT64, fn);
	CT::Check("Check value", Pack.GetValue<double>(), 1.8);

	CT::Check("Typecheck fixstring", Pack.GetNextType(), MsgFormats::FIXSTR, fn);
	CT::Check("Check value", Pack.GetValue<std::string>(), std::string("Hallo Welt!"));

	CT::Check("Typecheck string8", Pack.GetNextType(), MsgFormats::STR8, fn);
	CT::Check("Check value", Pack.GetValue<std::string>(), std::string("123456789012345678901234567890345678"));

	CT::Check("Typecheck bin8", Pack.GetNextType(), MsgFormats::BIN8, fn);
	const char Data[] = {5, 2, 1, 5, 7};
	bool Ok = true;

	std::vector<char> Val = Pack.GetValue<std::vector<char>>();

	if(Val.size() != sizeof(Data))
		Ok = false;
	else
	{
		for (size_t i = 0; i < sizeof(Data); i++)
		{
			if(Val[i] != Data[i])
			{
				Ok = false;
				break;
			}
		}
	}

	CT::Check("Check value", Ok, true);

	CT::Check("Typecheck fixarray", Pack.GetNextType(), MsgFormats::FIXARRAY, fn);

	std::vector<int> Vali = Pack.GetValue<std::vector<int>>();
	std::vector<int> vec = {10, 11, -21, 243};
	if(Vali.size() != vec.size())
		Ok = false;
	else
	{
		for (size_t i = 0; i < vec.size(); i++)
		{
			if(Vali[i] != vec[i])
			{
				Ok = false;
				break;
			}
		}
	}

	CT::Check("Check value", Ok, true);

	CT::Check("Typecheck fixmap(std::map)", Pack.GetNextType(), MsgFormats::FIXMAP, fn);

	std::map<int, std::string> Valm = Pack.GetValue<std::map<int, std::string>>();
	std::map<int, std::string> map = {{1, "Test"}, {2, "Hallo"}, {3, "Hallo Test"}};
	if(Valm.size() != map.size())
		Ok = false;
	else
	{
		for (size_t i = 0; i < map.size(); i++)
		{
			auto valP = *std::next(Valm.begin(), i);
			auto mapP = *std::next(map.begin(), i);

			if(valP.first != mapP.first || valP.second != mapP.second)
			{
				Ok = false;
				break;
			}
		}
	}

	CT::Check("Check value", Ok, true);


	CT::Check("Typecheck fixmap(std::multimap)", Pack.GetNextType(), MsgFormats::FIXMAP, fn);

	std::multimap<int, std::string> Valmm = Pack.GetValue<std::multimap<int, std::string>>();
	std::multimap<int, std::string> mmap = {{1, "Test1"}, {1, "Hallo1"}, {3, "Hallo Test1"}};
	if(Valmm.size() != mmap.size())
		Ok = false;
	else
	{
		for (size_t i = 0; i < mmap.size(); i++)
		{
			auto valP = *std::next(Valmm.begin(), i);
			auto mapP = *std::next(mmap.begin(), i);

			if(valP.first != mapP.first || valP.second != mapP.second)
			{
				Ok = false;
				break;
			}
		}
	}

	CT::Check("Check value", Ok, true);
}

std::string IntToString(int val)
{
	return std::to_string(val);
}

void TestSkipValues()
{
	std::function<std::string(MsgFormats)> fn = std::bind(MsgFormatsToString, std::placeholders::_1);
	std::function<std::string(int)> fnInt = std::bind(IntToString, std::placeholders::_1);
	Pack.Clear();

	Pack.AddArray(5);
	Pack.AddValue("test");
	Pack.AddValue(5);
	Pack.AddValue(256);
	Pack.AddValue(5.0);
	Pack.AddValue("Lol");

	Pack.AddValue(42);

	//Skips the array
	Pack.SkipValue();
	CT::Check("Typecheck after skip", Pack.GetNextType(), MsgFormats::POSITIVE_FIXINT, fn);
	CT::Check("Value check after skip", Pack.GetValue<int>(), 42, fnInt);

	Pack.Reset();
	Pack.SkipValue(5);	//Checks if the app doesn't crash and if the cursor is at the end of the stream.
	CT::Check("Typecheck after second skip", Pack.GetNextType(), MsgFormats::RESERVED, fn);

	Pack.AddMap(4);

	//1
	Pack.AddValue("testMap");
	Pack.AddValue(7.5);

	//2
	Pack.AddArray(2);
	Pack.AddValue(2);
	Pack.AddValue(5);
	Pack.AddValue("Hello World!");

	//3
	Pack.AddMap(2);
	Pack.AddValue(5);
	Pack.AddValue(43);
	Pack.AddValue("TT");
	Pack.AddValue("Test Test");
	Pack.AddValue(32);

	//4
	Pack.AddValue(66);
	Pack.AddValue(70);

	Pack.AddValue(89);

	//Skip the map.
	Pack.SkipValue();
	CT::Check("Typecheck after third skip", Pack.GetNextType(), MsgFormats::POSITIVE_FIXINT, fn);
	CT::Check("Value check after third skip", Pack.GetValue<int>(), 89, fnInt);
}

int main(int argc, char const *argv[])
{
	CT::TestFunction("TestSerialPrimitives", TestSerialPrimitives);
	CT::TestFunction("TestDeserialPrimitives", TestDeserialPrimitives);
	CT::TestFunction("TestSkipValues", TestSkipValues);

    // CMessagePack Pack;
    // CTest tt;
    // Pack.AddValue(tt);

    // CMessagePack Pack1;

    // // Pack1.AddArray(4);
    // Pack1.AddValue(1);
    // Pack1.AddValue("Hallo Welt");
    // Pack1.AddValue(1.5);
    // Pack1.AddValue(-1);

    // cout << std::bitset<8>((char)-32) << endl;
    // Pack1.AddValue(true);
    // Pack1.AddValue(false);

	// Pack1.AddArray(5);
	// Pack1.AddValue(1);
	// Pack1.AddValue(2);
	// Pack1.AddValue(3);
	// Pack1.AddValue(4);
	// Pack1.AddValue(5);

	// const char Data[] = {5, 2, 1, 5, 7};
	// Pack1.AddBin(Data, sizeof(Data));

    // // Pack1.AddValue()

    // // Pack.AddArray(13);

    // // Pack.AddValue((long)65000);
    // // Pack.AddValue(127);
    // // Pack.AddValue(-32);

    // // Pack.AddValue((long)125);
    // // Pack.AddValue((long)256);
    // // Pack.AddValue((long)1234556);
    // // Pack.AddValue((long)0x1FFFFFFFF);

    // // Pack.AddValue((unsigned)128);
    // // Pack.AddValue(1.34223);
    // // Pack.AddValue(true);
    // // Pack.AddValue("Test232");
    
    // // char Data[] = {'H', 123, 6};
    // // Pack.AddBin(Data, 3);

    // // std::map<int, int> dd;

    // // dd[1] = 34;
    // // dd[2] = 378;
    // // dd[3] = 3456;
    // // dd[4] = 356;

    // // Pack.AddValue(dd);

    // // ofstream out("out.msg", ios::binary);
    // auto val = Pack1.Serialize();

    // // out.write(val.data(), val.size());
    // // out.close();

    // CMessagePack Pack2;
    // Pack2.Deserialize(val);

    // PrintMsgFormats(Pack2.GetNextType());
    // cout << Pack2.GetValue<int>() << endl;

    // PrintMsgFormats(Pack2.GetNextType());
    // cout << Pack2.GetValue<std::string>() << endl;

    // PrintMsgFormats(Pack2.GetNextType());
    // cout << Pack2.GetValue<float>() << endl;

    // PrintMsgFormats(Pack2.GetNextType());
    // cout << Pack2.GetValue<int>() << endl;

    // PrintMsgFormats(Pack2.GetNextType());
    // cout << Pack2.GetValue<bool>() << endl;

    // PrintMsgFormats(Pack2.GetNextType());
    // cout << Pack2.GetValue<bool>() << endl;

	// PrintMsgFormats(Pack2.GetNextType());
	// std::vector<int> Ret = Pack2.GetValue<std::vector<int>>();

	// for (size_t i = 0; i < Ret.size(); i++)
	// {
	// 	cout << Ret[i] << endl;
	// }
	
	// PrintMsgFormats(Pack2.GetNextType());
	// std::vector<char> Ret1 = Pack2.GetValue<std::vector<char>>();

	// for (size_t i = 0; i < Ret1.size(); i++)
	// {
	// 	cout << (int)Ret1[i] << endl;
	// }

    //     Pack1.AddValue(1);
    // Pack1.AddValue("Hallo Welt");
    // Pack1.AddValue(1.5);
    // Pack1.AddValue(-1);

    return 0;
}
