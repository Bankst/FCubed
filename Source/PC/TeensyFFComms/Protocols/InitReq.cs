using System;
using ProtoBuf;

namespace TeensyFFComms.Protocols
{
    [ProtoContract]
    public class InitReq : F3Message
    {
        [ProtoMember(1)]
        public ulong Epoch { get; set; } = (ulong)(DateTime.UtcNow - new DateTime(1970, 1, 1)).TotalMilliseconds;

        public F3Opcode Opcode { get; set; } = F3Opcode.InitReq;
    }
}
