# BitBuffer

All Read & Write methods use the current bit position

## Methods

### > SetCurBitPos( pos: **int** )

### > GetCurBitPos( )
returns **int**

### > Reset( )

### > WriteBit( val: **int** )

### > ReadBit( )
returns **int**

### > WriteByte( val: **int** )

### > ReadByte( )
returns **int**

### > WriteString( text: **string** )

### > ReadString( [maxlength: **int** = 256] )

### > WriteFloat( val: **number** )

### > ReadFloat( [numbits: **int** = 32] )

### > WriteInt( val: **int**, [numBits: **int** = 32] )

### > ReadInt( [numbits: **int** = 32] )

### > Delete( )
Call this when you want to delete the BitBuffer from memory