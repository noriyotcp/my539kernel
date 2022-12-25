import json;

def generateIDTAsWords( idtAsJSON, nasmFormat = False ):
    idt = json.loads( idtAsJSON );
    idtAsWords = '';
    
    for entry in idt:
        if nasmFormat:
            idtAsWords += 'dw ';
        
        # ... #
        
        present = ( 1 if entry[ 'present' ] else 0 ) << 7;
        dpl = entry[ 'dpl' ] << 6;
        size = ( 1 if entry[ 'gate_descriptor_size' ] == '32-bit' else 0 ) << 3;
        gateType = ( 0 if entry[ 'interrupt_gate' ] else 1 );
        
        byteFive = present | dpl | ( 0 << 11 ) | size | ( 1 << 2 ) | ( 1 << 1 ) | gateType;
        
        wordThree = '0x' + format( byteFive, 'x' ).zfill( 2 ) + '00';
        
        # ... #
        
        idtAsWords += entry[ 'isr_routine_name' ] + ', ' + str( entry[ 'isr_segment_selector' ] ) + ', ' + wordThree + ', 0x0000'  + '\n';
        
    return idtAsWords;

idt = '''
[
    {   "isr_routine_name": "isr_0", "isr_segment_selector": 8, "present": true, "dpl": 0, "gate_descriptor_size": "32-bit", "interrupt_gate": true },
    {   "isr_routine_name": "isr_1", "isr_segment_selector": 8, "present": true, "dpl": 0, "gate_descriptor_size": "32-bit", "interrupt_gate": true }
]
''';

print( generateIDTAsWords( idt, True ) );

