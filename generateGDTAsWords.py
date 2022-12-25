import json;

def generateGDTAsWords( gdtAsJSON, nasmFormat = False ):
    gdt = json.loads( gdtAsJSON );
    gdtAsWords = '';
    
    for entry in gdt:
        if nasmFormat:
            gdtAsWords += entry[ 'name' ] + ': dw ';
                
        if entry[ 'type' ] == 'null':
            gdtAsWords += '0, 0, 0, 0\n';
        elif entry[ 'type' ] == 'code' or entry[ 'type' ] == 'data':
            baseAddress = int( entry[ 'base_address' ], 16 );
            limit = int( entry[ 'limit' ], 16 );
            
            baseAddressParts = [ baseAddress & 0xffff, ( baseAddress >> 16 ) & 0xff, ( baseAddress >> 24 ) & 0xff ]
            limitParts = [ limit & 0xffff, ( limit >> 16 ) & 0xf ];
            
            # ... #
            
            typeFlag = ( 1 if entry[ 'type' ] == 'code' else 0 ) << 3;
            accessed = 1 if entry[ 'accessed' ] else 0;
            typeField = None;
            dbFlag = None;
            
            if entry[ 'type' ] == 'code':
                conforming = ( 1 if entry[ 'conforming' ] else 0 ) << 2;
                readEnabled = ( 1 if entry[ 'read_enabled' ] else 0 ) << 1;
                
                typeField = typeFlag | conforming | readEnabled | accessed;
                
                dbFlag = ( 1 if entry[ 'operation_size' ] == '32bit' else 0 ) << 2
            else:
                expands = ( 1 if entry[ 'expands' ] == 'down' else 0 ) << 2;
                writeEnabled = ( 1 if entry[ 'write_enabled' ] else 0 ) << 1;
                
                typeField = typeFlag | expands | writeEnabled | accessed;
                
                dbFlag = ( 1 if entry[ 'upper_bound' ] == '4gb' else 0 ) << 2
                
            # ... #
            
            present = ( 1 if entry[ 'present' ] else 0 ) << 3
            privilegeLevel = entry[ 'privilege_level' ] << 1
            systemSegment = 1 if not entry[ 'system_segment' ] else 0
            
            firstPropSet = present | privilegeLevel | systemSegment;
            
            # ... #
            
            granularity = ( 1 if entry[ 'granularity' ] == '4kb' else 0 ) << 3
            longMode = ( 1 if entry[ '64bit' ] else 0 ) << 1
            
            secondPropSet = granularity | dbFlag | longMode | 0;
            
            words = [ limitParts[ 0 ], baseAddressParts[ 0 ],
                        ( ( ( firstPropSet << 4 ) | typeField ) << 8 ) | baseAddressParts[ 1 ],
                        ( ( ( baseAddressParts[ 2 ] << 4 ) | secondPropSet ) << 4 ) | limitParts[ 1 ] ];
                        
            words = list( map( lambda word: '0x' + format( word, 'x' ).zfill( 4 ), words ) );
            
            gdtAsWords += words[ 0 ] + ', ' + words[ 1 ] + ', ' + words[ 2 ] + ', ' + words[ 3 ] + '\n';
        else:
            raise Exception( 'Unkown Segment Type: ' + str( entry ) );
    
    return gdtAsWords;

gdt = '''
[
    {   "name": "null_descriptor", "type": "null" },

    {   "name": "kernel_code", "base_address": "0",
        "limit": "fffff", "granularity": "4kb",
        "system_segment": false, "type": "code",
        "accessed": false, "read_enabled": true, "conforming": false,
        "privilege_level": 0, "present": true, "operation_size": "32bit", "64bit": false  },

    {   "name": "kernel_data", "base_address": "0",
        "limit": "fffff", "granularity": "4kb",
        "system_segment": false, "type": "data",
        "accessed": false, "expands": "up", "write_enabled": true,
        "privilege_level": 0, "present": true, "upper_bound": "4gb", "64bit": false  },

    {   "name": "userspace_code", "base_address": "0",
        "limit": "fffff", "granularity": "4kb",
        "system_segment": false, "type": "code",
        "accessed": false, "read_enabled": true, "conforming": false,
        "privilege_level": 3, "present": true, "operation_size": "32bit", "64bit": false  },

    {   "name": "userspace_data", "base_address": "0",
        "limit": "fffff", "granularity": "4kb",
        "system_segment": false, "type": "data",
        "accessed": false, "expands": "up", "write_enabled": true,
        "privilege_level": 3, "present": true, "upper_bound": "4gb", "64bit": false  }
]
''';

print( generateGDTAsWords( gdt, True ) );

