// target specific IO header
#include <avr/io.h>

// NOTE that you need to add library platform independent and platform specific
// folders to compiler include files search Pbths:
// '../../mcucpp' and '../../mcucpp/AVR' respectively for this example

// defination of TPin class
#include <iopins.h>

// defination of PinList class
#include <pinlist.h>

// virtual port
#include <vport.h>

// all IO stuff are in namespce
using namespace Mcucpp::IO;

/// Работа с портами.
void Ports()
{
    // set all pins in Portb to output
    // DDRB = 0xFF;
    Portb::SetConfiguration( 0xFF, Portb::Out );

    // Записать значение в порт: Portb = 0x55.
    Portb::Write( 0x55 );

    // set pins with mask: Portb |= 0xAA.
    Portb::Set( 0xAA );

    // clear pins with mask: Portb &= ~0xf0.
    Portb::Clear( 0xF0 );

    // togle pins with mask: Portb ^= ~0xf0.
    Portb::Toggle( 0xFF );

    uint8_t clearMask = 0x0F;
    uint8_t outputValue = 0x03;

    // Portb = (Portb & ~clearMask) | outputValue.
    Portb::ClearAndSet( clearMask, outputValue );
    
    // Чтение регистра ввода-вывода.
    // set all pins in Portb to input
    Portb::SetConfiguration( 0xFF, Portb::In );

    // value = Portb.
    uint8_t value = Portb::Read();

    Portb::Set( value );

    // read input register: value = PIN.
    value = Portb::PinRead();

    // template versions of port manipulations for writing constant values
    // these functions are guaranteed to be inlined for maximum speed

    Portb::SetConfiguration<0xFF, Portb::Out>();
    Portb::Set<0xAA>();
    Portb::Clear<0xF0>();
    Portb::Toggle<0xFF>();

    const uint8_t clearMask2 = 0x0F;
    const uint8_t outputValue2 = 0x03;

    Portb::ClearAndSet<clearMask2, outputValue2>();
}


/// Работа с индивидуальными выводами.
void IndividualPins()
{
    // definition of one IO pin: pin 1 at Portb.
    typedef TPin<Portb, 1> Pin1;

    // либо можно использовать предопределённое короткое имя.
    // typedef Pb1 Pin1;

    // Настройка вывода на выход: DDRB |= (1 << PinNumber)
    Pin1::SetConfiguration( Pin1::Port::Out );

    // set pin to logical 1
    Pin1::Set();

    // set pin to logical 0
    Pin1::Clear();

    // toggle pin state
    Pin1::Toggle();

    // Configure pin as input
    Pin1::SetConfiguration(Pin1::Port::In);

    // Проверка состояния вывода.
    // if (PINB & (1 << PinNumber))
    if ( Pin1::IsSet() )
    {
        // do something
    }

    // definition of one IO pin: pin 1 at Portb with inverted logic
    // ie. 'Set' will write logical 0 to pin and 'Clear' - logical 1.
    // reading (IsSet) is not inverted
    typedef InvertedPin<Portb, 1> Pin2;

    // or you can use predefined short name
    // typedef Pb1Inv Pin2;

    // Configure pin as output
    Pin2::SetConfiguration(Pin2::Port::Out);

    // set pin to logical 1
    Pin2::Set();

    // set pin to logical 0
    Pin2::Clear();

    // toggle pin state
    Pin2::Toggle();

    // Configure pin as input
    Pin2::SetConfiguration( Pin2::Port::In );

    // Проверка состояния вывода.
    if ( Pin2::IsSet() ) // ->> if(PINA & (1 << PinNumber))
    {
        // do something
    }
}


/// Работа с группой выводов - их списком.
void PinLists()
{
    // Definition of group of IO pins which acts like a virtual IO port.
    // One group can contain up to 32 pins from different ports.
    // Pins in the group can have an arbitrary order.
    typedef PinList<Pb1, Pb0, Pb2, Pb1, Pc3, Pc4, Pc5> Group1;

    // В группу можно включать выводы с инвертированной логикой.
    typedef PinList<Pb1, Pb0Inv, Pb2, Pb1, Pc3Inv, Pc4, Pc5> Group2;

    uint8_t value = Group2::PinRead();

    Group2::Set( value );

    // PinList has interface similar to IO port interface,
    // but is is a little simplified
    Group1::SetConfiguration( Group1::Out );

    // write a value to group
    Group1::Write( 0x55 );

    // set pins with mask
    Group1::Set( 0xAA ); 

    // clear pins with mask
    Group1::Clear( 0xF0 );

    // set all pins in group to input
    Group1::SetConfiguration( Group1::In );

    // read input register
    value = Group1::PinRead(); 

    Group1::Set( value );

    // If you have a constant value to write to group,
    // use template varsions of writing functions.
    // They are much faster and smaller, since most of things are evaluated
    // at compile time and only actual Read/Modify/Write operation will take place in runtime.

    Group1::SetConfiguration<Group1::Out>();
    Group1::Write<0x55>();
    Group1::Set<0xAA>();
    Group1::Clear<0xF0>();

    // Individual pins in group can be accessed in this way:
    // Set pin 1 in group (indexing starts whith 0)
    Group1::Pin<1>::Set();

    // you can 'typdef' is for best readability
    typedef Group1::Pin<1> Pin1;

    Pin1::Clear();
    
    // Переключаем последний вывод в группе.
    const int lastPinIndex = Group1::Length - 1;

    Group1::Pin<lastPinIndex>::Toggle();

    // You can take a slice of group, it will be another PinList
    const int startingIndex = 4;
    const int sliceLenth = 3;

    typedef Group1::Slice<startingIndex, sliceLenth> LastTreePins;

    // Note that sliced pins will have they origin bit position in the input value.
    // ie. other pins in group will be just masked out
    LastTreePins::Write( 0x70 );
}


/// Работа с динамическими виртуальными портами и выводами.
void TestVPin( VPin pin )
{
    pin.SetConfiguration( pin.Port.Out );
    pin.Set();
    pin.Clear();
}


void TestVPort( VPortBase & port )
{
    port.Write( 0xFF );
    port.Clear( 0xFF );

    VPin pin1( port, 1 );
    TestVPin( pin1 );
}


void VPorts()
{
    // Виртуальный порт на основе имеющегося.
    VPort<Portb> portb;    

    // Виртуальный порт на основе группы выводов.
    typedef PinList<Pb1, Pb0, Pb2, Pb1, Pc3, Pc4, Pc5> Group1;

    VPort<Group1> group1;

    TestVPort( portb );
    TestVPort( group1 );
}


int main()
{
    // Работа с портами.
    Ports();

    // Работа с индивидуальными выводами.    
    IndividualPins();

    // Работа с группой выводов (списками).
    PinLists();
}


extern "C" void __cxa_pure_virtual()
{
    while (1);
}
