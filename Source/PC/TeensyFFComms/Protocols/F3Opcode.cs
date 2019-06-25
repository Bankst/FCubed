using ProtoBuf;

namespace TeensyFFComms.Protocols
{
    public enum F3Opcode
    {
        InitReq = 0,
        InitAck = 1,
        HeartbeatReq = 2,
        HeartbeatAck = 3,
        SysInfoReq = 4,
        SysInfoAck = 5,
        SdInfoReq = 6,
        SdInfoAck = 7,
    }
}
