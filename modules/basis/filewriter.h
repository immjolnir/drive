#pragma once

#include <fstream>
#include <string>

namespace hacking
{
    namespace _internal
    {
        class FileWriter {
          public:
            FileWriter(const std::string& filename) {
                _ofile.setstate(std::ios_base::failbit | std::ios_base::badbit);
                _ofile.open(filename, std::ios_base::out);
            }

            ~FileWriter() { _ofile.close(); }

            /**
             * Because of unable to pass the std::endl, we have to flush it explicitly.
             */
            void flush() { _ofile.flush(); }

            // Error: operator<< must have exactly one argument
            /*
            template <typename... Ts>
            inline FileWriter& operator<<(const FileWriter& writer, Ts... args) {
                ((_ofile << args), ...);
                _ofile << std::endl;
                return *this;
            }
            */
            // error: 'friend' used outside of class
            template <typename T>
            friend inline FileWriter& operator<<(FileWriter& writer, const T& arg) {
                writer._ofile << arg;
                return writer;
            }

            friend inline FileWriter& operator<<(FileWriter& writer, const char* arg) {
                return (writer << std::string(arg));
            }

            friend inline FileWriter& operator<<(FileWriter& writer, char* arg) { return (writer << std::string(arg)); }

          private:
            std::ofstream _ofile;
        };

    }  // namespace _internal

    // 类似于 std::cout, std::ci, cerr 三个全局的静态变量
    static _internal::FileWriter file_writer("pose_trajectory.txt");
}  // namespace hacking