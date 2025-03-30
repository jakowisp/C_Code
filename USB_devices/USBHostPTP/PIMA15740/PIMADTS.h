typedef union _DTSUnion{
        int8_t              signed8bitInt;       //0x0001
        uint8_t             unsigned8bitInt;     //0x0002
        int16_t             signed16bitInt;      //0x0003
        uint16_t            unsigned16bitInt;    //0x0004
        int32_t             signed32bitInt;      //0x0005
        uint32_t            unsigned32bitint;    //0x0006
        int64_t             signed64bitInt;      //0x0007
        uint64_t            unsigned64bitInt;    //0x0008
        //int128_t signed32bitInt;            //0x0009
        //uint128_t unsigned32bitint;         //0x000a
        PIMAArray<int8_t>   *arraysigned8bitInt;         //0x4001
        PIMAArray<uint8_t>  *arrayunsigned8bitInt;      //0x4002
        PIMAArray<int16_t>  *arraysigned16bitInt;       //0x4003
        PIMAArray<uint16_t> *arrayunsigned16bitInt;    //0x4004
        PIMAArray<int32_t>  *arraysigned32bitInt;       //0x4005
        PIMAArray<uint32_t> *arrayunsigned32bitInt;    //0x4006
        PIMAArray<int64_t>  *arraysigned64bitInt;       //0x4007
        PIMAArray<uint64_t> *arrayunsigned64bitInt;    //0x4008
        //aint128_t arraysigned32bitInt;      //0x4009
        //auint128_t arrayunsigned32bitint;   //0x400a
        PIMAString          *stringValue;             //0xffff */ 
      } DTSUnion; 

