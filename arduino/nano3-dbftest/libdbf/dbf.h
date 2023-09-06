#pragma once

/****************************************************************************
 * libdbf.h
 ****************************************************************************
 * Library to read information from dBASE files
 * Author: Bjoern Berg, clergyman@gmx.de
 * (C) Copyright 2004, Bj�rn Berg
 *
 ****************************************************************************
 * Permission to use, copy, modify and distribute this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation. The
 * author makes no representations about the suitability of this software for
 * any purpose. It is provided "as is" without express or implied warranty.
 *
 *****************************************************************************
 * $Id: libdbf.h,v 1.6 2006/04/14 12:25:30 rollinhand Exp $
 ****************************************************************************/

#include "Defines.h"
#include "endian.h"

/*! \file libdbf.h
	\brief provides access to libdbf.

	If you want to use functions of libdbf in your own programme, you have to add
	libdbf.h to your source files. libdbf.h describes all necessary accessible functions
	that libdbf provides.
*/
/*!	\mainpage libdbf Documentation
	\section intro Introduction
	<p>libdbf is the backend library for dbf to convert dBASE/FoxPro files to any
	format you want to. libdbf provides generic interfaces to access information
	in a dBASE file very easily without knowing much about the structure of the
	file.</p>
	<p>The commands and functions of libdbf orientate much on PHP's functions to access
	MySQL or Oracle databases. So, first steps should be very easy.</p>

	\section data Data Structures
	<p>The Link "Data Structures" gives a brief overview of the used classes and
	structures in libdbf.</p>

	\section file File List
	<p>This section is less important</p>

	\section fields Data Fields
	<p>This section describes the used variables used in libdbf.</p>

	\section globals Globals
	<p>This is the most important section. Behind this section you find all accessible
	functions and defines you can use in your own programme.</p>

	\section license License
	<p>Permission to use, copy, modify and distribute this software and its
 	 documentation for any purpose is hereby granted without fee, provided that
 	 the above copyright notice appear in all copies and that both that copyright
 	 notice and this permission notice appear in supporting documentation. The
 	 author makes no representations about the suitability of this software for
 	 any purpose. It is provided "as is" without express or implied warranty.</p>
*/

/*! \def FoxBase Code for FoxBase */
#define FoxBase 0x02
/*! \def FoxBasePlus Code for FoxBasePlus, same as for dBase III */
#define FoxBasePlus 0x03
/*! \def dBase3 Code for dBase III */
#define dBase3 0x03
/*! \def dBase3WM Code for dBase III with memo fields */
#define dBase3WM 0x83
/*! \def dBase4 Code for dBase IV */
#define dBase4 0x04
/*! \def dBase4WM Code for dBase IV with memo fields */
#define dBase4WM 0x8B
/*! \def dBase4SQL Code for dBase IV with SQL table */
#define dBase4SQL 0x8E
/*! \def dBase5 Code for dBase 5.0 */
#define dBase5 0x05
/*! \def FoxPro2WM Code for FoxPro 2.0 (or earlier) with memo fields */
#define FoxPro2WM 0xF5
/*! \def VisualFoxPro Code for Visual FoxPro without memo fields */
#define VisualFoxPro 0x30

#ifndef O_BINARY
#define O_BINARY 0
#endif

// These defines are used to distinguish between types in the dbf fields.
#define IS_STRING 1
#define IS_NUMERIC 2


/*
*	STRUCTS
*/

#pragma pack(push, 1)

/*! \struct DB_HEADER
\brief table file header
Standard dBASE Header
Offsets of this header are the same in all versions of
dBASE except dBASE 7.0
\warning It is recommend not to access DB_HEADER directly.
*/
typedef struct 
{
    /*! Byte: 0; dBase version */
    unsigned char version;

    /*! Byte: 1-3; date of last update */
    unsigned char last_update[3];

    /*! Byte: 4-7; number of records in table */
    uint32_t records;

    /*! Byte: 8-9; number of bytes in the header */
    uint16_t header_length;

    /*! Byte: 10-11; number of bytes in the record */
    uint16_t record_length;

    /*! Byte: 12-13; reserved, see specification of dBase databases */
    unsigned char reserved01[2];

    /*! Byte: 14; Flag indicating incomplete transaction */
    unsigned char transaction;

    /*! Byte: 15; Encryption Flag */
    unsigned char encryption;

    /*! Byte: 16-27; reserved for dBASE in a multiuser environment*/
    unsigned char reserved02[12];

    /*! Byte: 28; Production MDX file flag */
    unsigned char mdx;

    /*! Byte: 29; Language driver ID, for Visual FoxPro */
    unsigned char language;

    /*! Byte: 30-31; reserved, filled with zero */
    unsigned char reserved03[2];

} DB_HEADER;

/*! \struct DB_FIELD
\brief The field descriptor array
Offsets of this header are the same in all versions of dBASE
\warning It is recommend not to access DB_FIELD directly.
*/
/*! \brief Structure to store specification for one field

A pointer of type DB_FIELD is passed to \ref Create and
\ref CreateFH.
*/

typedef struct _DB_FIELD
{
    /*! Byte: 0-10; fieldname in ASCII */
    unsigned char field_name[ 11 ];

    /*! Byte: 11; field type in ASCII (C, D, L, M or N) */
    unsigned char field_type;

    /*! Byte: 12-15; field data adress */
    uint32_t field_address;

    /*! Byte: 16; field length in binary */
    unsigned char field_length;

    /*! Byte: 17; field decimal count in binary */
    unsigned char field_decimals;

    /*! Byte: 18-30; reserved */
    unsigned char reserved1[2];
    
    uint32_t field_offset;

    unsigned char reserved2[7];

    /*! Byte: 31; Production MDX field flag */
    unsigned char mdx;

} * PDB_FIELD, DB_FIELD;


/*! \struct P_DBF
\brief P_DBF is a global file handler

P_DBF store the file handlers for the dbf-file and if exists
the appropriate memo file.
*/
/*! \brief Object handle for dBASE file

A pointer of type P_DBF is used by all functions except for \ref Open
which returns it if the dBASE file could be successfully opened.
*/
typedef struct _P_DBF
{
    /*! filehandler of memo */
    int dbt_fh;

    /*! the pysical size of the file, as stated from filesystem */
    uint32_t real_filesize;

    /*! the calculated filesize */
    uint32_t calc_filesize;

    /*! header of .dbf file */
    DB_HEADER * header;

    /*! array of field specification */
    DB_FIELD * fields;

    /*! number of fields */
    uint32_t columns;

    /*! integrity could be: valid, invalid */
    unsigned char integrity[7];

    /*! record counter */
    uint32_t cur_record;

    /*! errorhandler, maximum of 254 characters */
    char errmsg[ 254 ];

} P_DBF;

#pragma pack(pop)

/* Memo File Structure (.FPT)
* Memo files contain one header record and any number of block structures.
* The header record contains a pointer to the next free block and the size
* of the block in bytes. The size is determined by the SET BLOCKSIZE command
* when the file is created. The header record starts at file position zero and
* occupies 512 bytes. The SET BLOCKSIZE TO 0 command sets the block size width to 1.
*/
/* Following the header record are the blocks that contain a block header and
* the text of the memo. The table file contains block numbers that are used to
* reference the memo blocks. The position of the block in the memo file is
* determined by multiplying the block number by the block size (found in the
* memo file header record). All memo blocks start at even block boundary
* addresses. A memo block can occupy more than one consecutive block.
*/


/* Memo Header Record */

struct DB_MEMO_HEADER 
{
    /* 00  03	Location of next free block [1] */
    unsigned int block_adress;

    /* 04  05	Unused */
    unsigned char reserved[2];

    /* 06  07	Block size (bytes per block) [1] */
    unsigned short block_size;

    /* 08  511	Unused */
    unsigned char reserved2[504];
};

/* [1] Integers stored with the most
significant byte first. See: endian.h   */

/* Memo Block Header and Memo Text */
struct DB_MEMO_BLOCK_TOP 
{
    /* 00  03			Block signature [1]  */
    /*	(indicates the type of data in the block)
    *	0  picture (picture field type)
    *	1  text (memo field type) */
    unsigned int signature;

    /* 04  07	Length [1] of memo (in bytes) */
    unsigned int block_length;

    /* 08 -  n	Memo text (n = length) */
};

// [1] Integers stored with the most significant byte first.

/*
 *	Class CDBF
 */

class CDBF
{
    static P_DBF * _pdbf;
    static CFileFs _file;

    static const char * GetDBVersion( int version );
    static int ReadHeaderInfo();
    static int WriteHeaderInfo( DB_HEADER *header );
    static int ReadFieldInfo();
    static int WriteFieldInfo( DB_FIELD *fields, int numfields );
    static char * GetRecordData( char *record, int column );

public:
    /*! \fn GetVersion(P_DBF *p_dbf)
    \brief return the version of dbf file as a human readable string
    \param *p_dbf the object handle of the opened file
    */
    static const char * GetStringVersion();

    /*! \fn GetVersion(P_DBF *p_dbf)
    \brief return the version of dbf file
    \param *p_dbf the object handle of the opened file

    \return version or -1 on error
    */
    static int GetVersion();

    /*! \fn P_DBF *Open (const char *file)
    \brief Open opens a dBASE \a file and returns the object handle
    \param file the filename of the dBASE file

    Opens a dBASE file and returns the object handle.
    Additional information from the dBASE header are read and stored
    internally.
    \return NULL in case of an error.
    */
    static P_DBF * Open( const char * file );

    /*! \fn P_DBF *CreateFH (int fh, DB_FIELD *fields, int numfields)
    \brief Create opens a new dBASE \a file and returns the object handle
    \param fh file handle of already open file
    \param fields record of field specification
    \param numfields number of fields

    Creates a dBASE file and returns the object handle.
    \return NULL in case of an error.
    */
    static P_DBF * CreateFH( DB_FIELD * fields, int numfields );

    /*! \fn P_DBF *Create (const char *file, DB_FIELD *fields, int numfields)
    \brief Create opens a new dBASE \a file and returns the object handle
    \param file the filename of the dBASE file
    \param fields record of field specification
    \param numfields number of fields

    Creates a dBASE file and returns the object handle.
    \return NULL in case of an error.
    */
    static P_DBF * Create( const char * file, DB_FIELD * fields, int numfields );

    /*! \fn int Close (P_DBF *p_dbf)
    \brief Close closes a dBASE file.
    \param *p_dbf the object handle of the opened file

    Closes a dBASE file and frees all memory.
    \return 0 if closing was successful and -1 if not.
    */
    static int Close();

    // Functions to info about rows and columns
    /*! \fn int NumRows (P_DBF *p_dbf)
    \brief NumRows returns the number of datasets/rows
    \param *p_dbf the object handle of the opened file

    Returns the number of datasets/rows.
    \return Number of rows or -1 in case of an error.
    */
    static int NumRows();

    /*! \fn int NumCols (P_DBF *p_dbf)
    \brief NumCols returns the number of attributes/columns
    \param *p_dbf the object handle of the opened file

    Returns the number of fields/columns.
    \return Number of columns or -1 in case of an error.
    */
    static int NumCols();

    /*! \fn const char *ColumnName(P_DBF *p_dbf, int column)
    \brief ColumnName returns the name of a selected \a column
    \param *p_dbf the object handle of the opened file
    \param column the number of the column

    Returns the name of a selected column.
    The first column has number 0. The maximum number of columns can
    be determined with \ref NumCols.
    \return Name of column or -1 on error
    */
    static char *ColumnName( uint32_t column );

    /*! \fn int ColumnSize(P_DBF *p_dbf, int column);
    \brief ColumnSize returns the field length of a column
    \param *p_dbf the object handle of the opened file
    \param column the number of the column

    Returns the field length of a column.
    The first column has number 0. The maximum number of columns can
    be determined with \ref NumCols.
    \return field length of column or -1 on error
    */
    static int ColumnSize( uint32_t column );

    /*! \fn const char ColumnType(P_DBF *p_dbf, int column)
    \brief ColumnType returns the type of a field resp. column
    \param *p_dbf the object handle of the opened file
    \param column the number of the column

    Returns the type of a column. Type can be any
    of 'N' (number), 'C' (string), 'D' (data), 'M' (memo), 'L' (boolean)
    for dBASE III files.
    The first column has number 0. The maximum number of columns can
    be determined with \ref NumCols.
    \return field type of column or -1 on error
    */
    static char ColumnType( uint32_t column );

    /*! \fn int ColumnDecimals(P_DBF *p_dbf, int column)
    \brief
    \param *p_dbf the object handle of the opened file
    \param column the number of the column

    Returns the number of digits right to the
    decimal point.
    The first column has number 0. The maximum number of columns can
    be determined with \ref NumCols.
    \return Number of decimals of column or -1 on error
    */
    static int ColumnDecimals( uint32_t column );

    /*! \fn uint32_t ColumnAddress(P_DBF *p_dbf, int column)
    \brief ColumnAddress returns the address of a column
    \param *p_dbf the object handle of the opened file
    \param column the number of the column

    The first column has number 0. The maximum number of columns can
    be determined with \ref NumCols.
    \return Address of column or -1 on error
    */
    static uint32_t ColumnAddress( uint32_t column );

    /*! \fn const char *GetDate(P_DBF *p_dbf)
    \brief GetDate returns a formatted date string
    \param *p_dbf the object handle of the opened file

    Returns a formatted date string of the type
    yyyy-mm-dd. The date indicates the time the file was last
    modified.

    \return formatted date or -1 on error
    */
    static const char * GetDate();

    /*! \fn int SetField(DB_FIELD *field, int type, const char *name, int len, int dec)
    \brief SetField fills a field structure
    \param *field pointer to field which shall be set
    \param type type of field
    \param name name of field
    \param len length of field
    \param dec number of decimals

    Sets the field structure with the given values.

    \return 0
    */
    static int SetField( DB_FIELD * field, char type, const char * name, char len, char dec );

    /*! \fn int RecordLength(P_DBF *p_dbf)
    \brief RecordLength returns length of a dataset
    \param *p_dbf the object handle of the opened file

    Returns the length of a dataset. Do not confuse this
    with \ref HeaderSize.

    \return record length or -1 on error
    */
    static int RecordLength();

    /*! \fn int HeaderSize(P_DBF *p_dbf)
    \brief HeaderSize returns length of the header
    \param *p_dbf the object handle of the opened file

    Returns the length of the header.

    \return header length or -1 on error
    */
    static int HeaderSize();

    /*! \fn int SetRecordOffset(P_DBF *p_dbf, int offset)
    \brief SetRecordOffset set the internal record counter
    \param *p_dbf the object handle of the opened file
    \param offset the new record number of the next record read

    Sets the internal record counter. The counter is an index counting
    through the records within a dBASE file. The first record has
    number 1. A negative offset will be counted from the end of the file.
    The last record has index -1.
    \ref ReadRecord uses the counter when reading the
    next record. Setting a value outside the range -numrecord to numrecords
    or 0 will cause an error.

    \return the new internal record counter or -1 in case of an error
    */
    static int SetRecordOffset( int offset );

    /*! \fn int ReadRecord(P_DBF *p_dbf, char *record, int len)
    \brief ReadRecord reads the current record
    \param *p_dbf the object handle of the opened file
    \param *record a memory block large enough to contain a record
    \param len the length of the record block

    Returns a record as it stored in the dBASE file. The memory must
    be large enough and allocated by the calling application.

    \return 0 if successful, -1 on error
    */
    static int ReadRecord( char * record, uint16_t len );

    /*! \fn int WriteRecord(P_DBF *p_dbf, char *record, int len)
    \brief WriteRecord writes a record
    \param *p_dbf the object handle of the opened file
    \param *record record data suitable for writing into the dBASE file
    \param len the length of the record block

    Writes a record into a dBASE file and returns the number of
    records. The record data must contain all field data but not
    the leading byte which indicates whether the record is deleted.
    Hence, len must be \ref RecordLength() - 1.

    \return number of records written, -1 on error
    */
    static int WriteRecord( char *record, uint16_t len );

    /*! \fn int IsMemo(P_DBF *p_dbf)
    \brief IsMemo tells if dbf provides also a memo file
    \param *p_dbf the object handle of the opened file

    IsMemo indicates if dbf provides also a memo file

    \return 0 no memo, 1 memo, -1 on error
    */
    static int IsMemo();
};
