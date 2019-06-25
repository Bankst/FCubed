using ProtoBuf;

namespace TeensyFFComms.Protocols
{
    [ProtoContract]
    public class InitAck : F3Message
    {
        [ProtoMember(1)]
        public bool RtcStatus { get; }

        private InitAck(bool rtcStatus)
        {
            RtcStatus = rtcStatus;
        }

        public F3Opcode Opcode { get; set; } = F3Opcode.InitAck;
    }
}
