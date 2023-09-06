/*****************************************************************************
 * dbf.cpp
 *****************************************************************************
 * Library to read information from dBASE files
 * Author: Bjoern Berg, clergyman@gmx.de
 * (C) Copyright 2004, Bj�rn Berg
 *
 *****************************************************************************
 * Permission to use, copy, modify and distribute this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation. The
 * author makes no representations about the suitability of this software for
 * any purpose. It is provided "as is" without express or implied warranty.
 *
 * $Id: dbf.c,v 1.8 2004/09/07 16:08:23 steinm Exp $
 ****************************************************************************/

#include "../FatFs/FatFs.h"
#include "dbf.h"

P_DBF * CDBF::_pdbf = nullptr;
CFileFs CDBF::_file = {};


// Convert version field of header into human readable string.
const char * CDBF::GetDBVersion( int version )
{
    static char name[ 31 ];

    switch ( version )
    {
        // without memo fields
        case 0x02: { return "FoxBase"; }
        
        // without memo fields
        case 0x03: { return "FoxBase+/dBASE III+"; }

        // without memo fields
        case 0x04: { return "dBASE IV"; }

        // without memo fields
        case 0x05: { return "dBASE 5.0"; }

        case 0x83: { return "FoxBase+/dBASE III+"; }

        case 0x8B: { return "dBASE IV"; }

        // without memo fields
        case 0x30: { return "Visual FoxPro"; }

        // with memo fields
        case 0xF5: { return "FoxPro 2.0"; }
        
        default:
        {
            sprintf_P( name, F( "Unknown (code 0x%.2X)" ), version );

            return name;
        }
    }
}


// Reads header from file into struct.
int CDBF::ReadHeaderInfo()
{
    auto header = ( DB_HEADER * ) malloc( sizeof( DB_HEADER ) );

    if ( header == nullptr )
    {
        return -1;
    }

    auto nrd = _file.read( header, sizeof( DB_HEADER ) );

    if ( nrd == 0 )
    {
        free( header );
        return -1;
    }

    // Endian Swapping.
    header->header_length = rotate2b( header->header_length );
    header->record_length = rotate2b( header->record_length );
    header->records = rotate4b( header->records );

    _pdbf->header = header;

    return 0;
}


// Write header into file.
int CDBF::WriteHeaderInfo( DB_HEADER * header )
{
    time_t ps_calendar_time;
    struct tm * ps_local_tm;

    ( void ) ps_calendar_time;
    ( void ) ps_local_tm;

    auto newheader = ( DB_HEADER * ) malloc( sizeof( DB_HEADER ) );

    if ( newheader == nullptr )
    {
        //loginfo( "Can't alloc newheader." );
        return -1;
    }

    memcpy( newheader, header, sizeof( DB_HEADER ) );

    //ps_calendar_time = time( nullptr );
    //
    //if ( ps_calendar_time != ( time_t ) ( -1 ) )
    //{
    //    ps_local_tm = localtime( & ps_calendar_time );
    //
    //    newheader->last_update[0] = ps_local_tm->tm_year;
    //    newheader->last_update[1] = ps_local_tm->tm_mon + 1;
    //    newheader->last_update[2] = ps_local_tm->tm_mday;
    //}

    newheader->header_length = rotate2b( newheader->header_length );
    newheader->record_length = rotate2b( newheader->record_length );
    newheader->records = rotate4b( newheader->records );

    // Make sure the header is written at the beginning of the file
    // because this function is also called after each record has
    // been written.
    _file.seekSet(0);

    auto nwrt = _file.write( newheader, sizeof( DB_HEADER ) );

    free( newheader );

    if ( nwrt == 0 )
    {
        //loginfo( "Can't write newheader." );
        return -1;
    }

    return 0;
}


// Sets _pdbf->fields to an array of DB_FIELD containing the specification
// for all columns.
int CDBF::ReadFieldInfo()
{
    int columns = NumCols();

    auto fields = ( DB_FIELD * ) malloc( columns * sizeof( DB_FIELD ) );

    if ( fields == nullptr )
    {
        return -1;
    }

    _file.seekSet( sizeof( DB_HEADER ) );

    auto nrd = _file.read( fields, columns * sizeof( DB_FIELD ) );

    if ( nrd == 0 )
    {
        //logerror( "In function ReadFieldInfo()" );
        return -1;
    }

    _pdbf->fields = fields;
    _pdbf->columns = columns;

    // The first byte of a record indicates whether it is deleted or not.
    int offset = 1;

    for ( auto i = 0; i < columns; i++ )
    {
        fields[i].field_offset = offset;
        offset += fields[i].field_length;
    }

    return 0;
}


// Writes the field specification into the output file.
int CDBF::WriteFieldInfo( DB_FIELD * fields, int numfields )
{
    _file.seekSet( sizeof( DB_HEADER ) );

    auto nwrt = _file.write( fields, numfields * sizeof( DB_FIELD ) );

    if ( nwrt == 0 )
    {
        //logerror( "In function WriteFieldInfo()" );
        return -1;
    }

    _file.write( ( void * ) "\r\0", 2 );

    return 0;
}


// Open the a dbf file and returns file handler.
P_DBF * CDBF::Open( const char * file )
{
    if ( !_file.open( ( char * ) file, FA_READ | FA_WRITE | FA_OPEN_ALWAYS ) )
    {
        return nullptr;
    }

    auto _pdbf = ( P_DBF * ) malloc( sizeof( P_DBF ) );

    if ( _pdbf == nullptr )
    {
        return nullptr;
    }

    _pdbf->header = nullptr;

    if ( ReadHeaderInfo() < 0 )
    {
        free( _pdbf );
        return nullptr;
    }

    _pdbf->fields = nullptr;

    if ( ReadFieldInfo() < 0 )
    {
        free( _pdbf->header );
        free( _pdbf );

        return nullptr;
    }

    _pdbf->cur_record = 0;

    return _pdbf;
}


// Create a new dbf file and returns file handler.
P_DBF * CDBF::CreateFH( DB_FIELD * fields, int numfields )
{            
    _pdbf = ( P_DBF * ) malloc( sizeof( P_DBF ) );

    if (  _pdbf == nullptr )
    {
        //logerror( "Can't alloc pdbf." );
        return nullptr;
    }

    auto header = ( DB_HEADER * ) malloc( sizeof( DB_HEADER ) );

    if ( header == nullptr )
    {
        //logerror( "Can't alloc header." );
        return nullptr;
    }

    memset( header, 0, sizeof( DB_HEADER ) );    

    header->version = dBase4;

    int reclen = 0;

    for ( auto i = 0; i < numfields; i++ )
    {
        reclen += fields[i].field_length;
    }

    // Add 1 to record length for deletion flag.
    header->record_length = reclen + 1;
    header->header_length = sizeof( DB_HEADER ) + numfields * sizeof( DB_FIELD ) + 2;

    if ( WriteHeaderInfo( header ) < 0 )
    {
        free( _pdbf->header );
        free( _pdbf );
        return nullptr;
    }

    if ( WriteFieldInfo( fields, numfields ) < 0 )
    {
        free( _pdbf->header );
        free( _pdbf );
        return nullptr;
    }

    _pdbf->header = header;
    _pdbf->fields = fields;
    _pdbf->cur_record = 0;

    return _pdbf;
}


// Create a new dbf file and returns file handler.
P_DBF * CDBF::Create( const char * file, DB_FIELD * fields, int numfields )
{
    if ( !_file.open( ( char * ) file, FA_READ | FA_WRITE | FA_CREATE_NEW ) )
    {
        //logerror( "Can't create file." );

        return nullptr;
    }

    return CreateFH( fields, numfields );
}


// Close the current open dbf file and free all memory.
int CDBF::Close()
{
    if ( _pdbf != nullptr )
    {
        if ( _pdbf->header != nullptr )
        {
            free( _pdbf->header );
        }

        if ( _pdbf->fields != nullptr )
        {
            free( _pdbf->fields );
        }

        free( _pdbf );
    }

    if ( !_file.close() )
    {
        return -1;
    }

    return 0;
}


/******************************************************************************
	Block with functions to get information about the amount of
		- rows and
		- columns
 ******************************************************************************/

// Returns the number of records.
int CDBF::NumRows()
{
    if ( _pdbf->header->records > 0 )
    {
        return _pdbf->header->records;
    }
    else
    {
        //logerror( "In function NumRows(): " );

        return -1;
    }

    return 0;
}


// Returns the number of fields.
int CDBF::NumCols()
{
    if ( _pdbf->header->header_length > 0 )
    {
        // TODO: Backlink muss noch eingerechnet werden
        return ( ( _pdbf->header->header_length - sizeof( DB_HEADER ) - 1 ) / sizeof( DB_FIELD ) );
    }
    else
    {
        //logerror( "In function NumCols(): " );

        return -1;
    }

    return 0;
}

/******************************************************************************
	Block with functions to get/set information about the columns
 ******************************************************************************/

// Returns the name of a column.Column names cannot be longer than 11 characters.
char * CDBF::ColumnName( uint32_t column )
{
    if ( column >= _pdbf->columns )
    {
        return ( char * ) "invalid";
    }

    return ( char * ) _pdbf->fields[ column ].field_name;
}


int CDBF::ColumnSize( uint32_t column )
{
    if ( column >= _pdbf->columns )
    {
        return -1;
    }

    return ( int ) _pdbf->fields[ column ].field_length;
}


char CDBF::ColumnType( uint32_t column )
{
    if ( column >= _pdbf->columns )
    {
        return -1;
    }

    return _pdbf->fields[ column ].field_type;
}


int CDBF::ColumnDecimals( uint32_t column )
{
    if ( column >= _pdbf->columns )
    {
        return -1;
    }

    return _pdbf->fields[ column ].field_decimals;
}


uint32_t CDBF::ColumnAddress( uint32_t column )
{
    if ( column >= _pdbf->columns )
    {
        return -1;
    }

    return _pdbf->fields[ column ].field_address;
}


int CDBF::SetField( DB_FIELD * field, char type, const char * name, char len, char dec )
{
    memset( field, 0, sizeof( DB_FIELD ) );

    field->field_type = type;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-truncation"
    strncpy( ( char * ) field->field_name, name, 11 );
#pragma GCC diagnostic pop

    field->field_length = len;
    field->field_decimals = dec;

    return 0;
}


/******************************************************************************
	Block with functions to read out special dBASE information, like
		- date
		- usage of memo
 ******************************************************************************/

// Returns the date of last modification as a human readable string.
const char * CDBF::GetDate()
{
    static char date[10];

    if ( _pdbf->header->last_update[0] )
    {
        sprintf_P( date, F( "%d-%02d-%02d" ), 1900 + _pdbf->header->last_update[0], 
            _pdbf->header->last_update[1], _pdbf->header->last_update[2] );

        return date;
    }
    else
    {
        //logerror( "In function GetDate(): " );

        return "";
    }

    return 0;
}


int CDBF::HeaderSize()
{
    if ( _pdbf->header->header_length > 0 )
    {
        return _pdbf->header->header_length;
    }
    else
    {
        //logerror( "In function HeaderSize(): " );

        return -1;
    }

    return 0;
}


// Returns the length of a record.
int CDBF::RecordLength()
{
    if ( _pdbf->header->record_length > 0 )
    {
        return _pdbf->header->record_length;
    }
    else
    {
        //logerror( "In function RecordLength(): " );

        return -1;
    }

    return 0;
}


// Returns the verion of the dbase file as a human readable string.
const char * CDBF::GetStringVersion()
{
    if ( _pdbf->header->version == 0 )
    {
        //logerror( "In function GetStringVersion(): " );

        return ( char * ) -1;
    }

    return GetDBVersion( _pdbf->header->version );
}


// Returns the verion field as it is storedi in the header.
int CDBF::GetVersion()
{
    if ( _pdbf->header->version == 0 )
    {
        //logerror( "In function GetVersion(): " );

        return -1;
    }

    return _pdbf->header->version;
}


int CDBF::IsMemo()
{
    if ( _pdbf->header->version == 0 )
    {
        //logerror( "In function IsMemo(): " );
        return -1;
    }

    return ( _pdbf->header->version & 128 ) == 128 ? 1 : 0;
}


/******************************************************************************
	Block with functions to read records
 ******************************************************************************/

int CDBF::SetRecordOffset( int offset )
{
    if ( offset == 0 )
    {
        return -3;
    }

    if ( offset > ( int ) _pdbf->header->records )
    {
        return -1;
    }

    if ( ( offset < 0 ) && ( abs( offset ) > _pdbf->header->records ) )
    {
        return -2;
    }

    if ( offset < 0 )
    {
        _pdbf->cur_record = ( int ) _pdbf->header->records + offset;
    }
    else
    {
        _pdbf->cur_record = offset - 1;
    }

    return _pdbf->cur_record;
}


int CDBF::ReadRecord( char * record, uint16_t len )
{
    if ( _pdbf->cur_record >= _pdbf->header->records )
    {
        return -1;
    }

    auto offset = _pdbf->header->header_length + _pdbf->cur_record * _pdbf->header->record_length;

    _file.seekSet( offset );
    
    //loginfo( "Offset = %d, Record length = %d\n", offset, _pdbf->header->record_length );

    auto nrd = _file.read( record, _pdbf->header->record_length > len ? len : _pdbf->header->record_length );

    if ( nrd == 0 )
    {
        return -1;
    }

    _pdbf->cur_record++;

    return _pdbf->cur_record - 1;
}


int CDBF::WriteRecord( char * record, uint16_t len )
{
    if ( len != _pdbf->header->record_length - 1 ) 
    {
        //logerror( "Length of record mismatches expected length (%d != %d).", len, _pdbf->header->record_length );        
        return -1;
    }

    _file.seekSet( _file.fileSize() );

    auto nwrt = _file.write( ( void * ) " ", 1 );

    if ( nwrt == 0 )
    {
        return -1;
    }

    nwrt = _file.write( record, _pdbf->header->record_length - 1 );

    if ( nwrt == 0 )
    {
        return -1;
    }

    _pdbf->header->records++;

    if ( WriteHeaderInfo( _pdbf->header ) < 0 ) 
    {
        return -1;
    }

    return _pdbf->header->records;
}


char * CDBF::GetRecordData( char * record, int column )
{
	return record + _pdbf->fields[ column ].field_offset;
}
