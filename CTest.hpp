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

#ifndef CTEST_HPP
#define CTEST_HPP

#include <functional>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace CT
{
    enum class CTestError
    {
        UNEXPECTED_RETURN
    };

    class CCTestException : public std::exception
    {
        public:
            CCTestException() {}
            CCTestException(CTestError Type) : m_ErrType(Type) {}
            CCTestException(CTestError Type, const std::string &Msg) : m_Msg(Msg), m_ErrType(Type) {}

            const char *what() const noexcept override
            {
                return m_Msg.c_str();
            }

            CTestError GetErrType() const noexcept
            {
                return m_ErrType;
            }

        private:
            std::string m_Msg;
            CTestError m_ErrType;
    };

    void TestFunction(const std::string &Name, std::function<void()> fn)
    {
        std::cout << "Begin Testing Function: '" << Name << "'" << std::endl;
        bool Error = true;
        
        try
        {
            fn();
            Error = false;
        }
        catch(const CCTestException& e)
        {
            std::cerr << e.what() << '\n';
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }

        std::cout << "Finished Testing Function: '" << Name << "'";
        if(Error)
            std::cout << " Function failed";
        
        std::cout << std::endl;
    }

    template<class T>
    void Check(const std::string &Name, T val, T expected, std::function<std::string(T)> Converter = nullptr)
    {
        if(val != expected)
        {
            if(Converter)
                throw CCTestException(CTestError::UNEXPECTED_RETURN, "Test for '"+ Name + "' failed! Expected: '" + Converter(expected) + "' Got: '" + Converter(val) + "'");

            throw CCTestException(CTestError::UNEXPECTED_RETURN, "Test for '"+ Name + "' failed! Unexpected value");
        }

        std::cout << "Test for '" + Name + "' succeded" << std::endl;
    }

} // namespace CT


#endif //CTEST_HPP