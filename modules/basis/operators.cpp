/*

- operator <<
    - Examples:
    /usr/include/opencv4/opencv2/core/persistence.hpp

    @brief Writes data to a file storage.
    template<typename _Tp>
    static inline FileStorage& operator << (FileStorage& fs, const _Tp& value)
    {
        if( !fs.isOpened() )
            return fs;
        if( fs.state == FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP )
            CV_Error( Error::StsError, "No element name has been given" );
        write( fs, fs.elname, value );
        if( fs.state & FileStorage::INSIDE_MAP )
            fs.state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
        return fs;
    }

    static inline FileStorage& operator << (FileStorage& fs, const char* str)
    {
        return (fs << String(str));
    }

- operator >>
    - Examples:
    /usr/include/opencv4/opencv2/core/persistence.hpp

    template<typename _Tp>
    static inline FileNodeIterator& operator >> (FileNodeIterator& it, _Tp& value)
    {
        read( *it, value, _Tp());
        return ++it;
    }

    @brief Reads data from a file storage.
    template<typename _Tp>
    static inline void operator >> (const FileNode& n, _Tp& value)
    {
        read( n, value, _Tp());
    }

*/