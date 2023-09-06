#pragma once

// "AVR, С++ и умные указатели"
// Ссылка: http://we.easyelectronics.ru/Soft/avr-s-i-umnye-ukazateli.html
// Автор: http://we.easyelectronics.ru/profile/neiver/

// Литература:
// Скотт Мейерс-Наиболее эффективное использование C++
// Abrahams D., Gurtovoy A. — C++ Template Metaprogramming
// Александреску А. Современное проектирование на C++
// Стефан К. Дьюхэрст «Скользкие места С++»

//
// Этот шаблон принимает два параметра:
// 1 - тип переменной на которую указывает указатель
// 2 - класс содержащий функцию чтения из целевого адресного пространства
template < class T, class Accessor >
class BasePtr {

        // псевдоним для собственного имени класса для краткости
        typedef BasePtr Self;

public:

    BasePtr( T *address = 0 ): _address( address ) {}

    // далее определяем все операции поддерживаемые этим указателем
    // преинкремент
    inline Self& operator ++() {

            _address++;

            return *this;

    }

    // постинкремент
    inline Self operator ++( int ) {

        Self tmp = *this;
        _address++;

        return tmp;

    }

    // предекремент
    inline Self& operator --() {

        _address--;

        return *this;

    }

    // постдекремент
    inline Self operator --( int ) {

        Self tmp = *this;
        _address--;

        return tmp;

    }

    inline Self& operator +=( int value ) {

        _address += value;

        return *this;

    }

    inline Self& operator -=( int value ) {

        _address -= value;

        return *this;

    }

    inline Self operator +( int value ) {

        return Self( _address + value );

    }

    inline Self operator -( int value ) {

        return Self( _address - value );

    }

    inline bool operator !=( const Self &other ) const {

        return _address != other._address;

    }

    inline bool operator ==( const Self &other ) const {

        return _address == other._address;

    }

    // Операция разъименовывания указателя. Здесь осуществляется чтение по указателю.
    inline const T operator *() const {

        union {
                T value = 0;
                uint8_t bytes[ sizeof(T) ];
        } data;

        for ( unsigned i = 0; i < sizeof(T); ++i ) {

            data.bytes[i] = Accessor::Read( ( const uint8_t * const )( _address ) + i );

        }

        return data.value;

    }

    explicit operator const T * () const {

        return _address;
    }

    inline const T operator []( int value ) {

        return * Self( _address + value );

    }

private:

    T * _address;

};

template<class T>
class FlashPtr: public BasePtr<T, FlashPtr<T> > {

    public:

        FlashPtr( T *address ): BasePtr< T, FlashPtr<T> >( address ) {}

        static uint8_t Read( const uint8_t * const addr ) {

                return pgm_read_byte( addr );

        }

};

template<class T>
class EepromPtr: public BasePtr< T, EepromPtr<T> > {

    public:

        EepromPtr( T *address ): BasePtr< T, EepromPtr<T> >( address ) {}

        static uint8_t Read( const uint8_t * const addr ) {

                return eeprom_read_byte( addr );

        }

};
